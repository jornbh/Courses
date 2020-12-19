-module(watchdog).
-compile(export_all). 
-include("global_constants.hrl").

-define(LOOK_FOR_NEW_NODES_PERIOD,5000).
-define(LOOTING_PERIOD,2000).
-define(NET_TICKTIME,2).
-define(CONCISTENCY_CHECK_PERIOD,500).


spawn_watchdog()->
    {?MODULE, spawn(fun()-> init_watchdog() end)}.

init_watchdog()->
    net_kernel:set_net_ticktime(?NET_TICKTIME, 10), % 10 = s untill the new time is set
    Node_IDs = nodes(), 
    start_monitoring(Node_IDs), 
    watchdog(Node_IDs).



%%Watchdog-ing
watchdog(Current_nodes)->
    receive
        {nodedown,Node} ->
            spawn( fun()->handle_node_down(Node) end), 
            watchdog(Current_nodes)
    after ?LOOK_FOR_NEW_NODES_PERIOD->
        start_monitoring( nodes() -- Current_nodes),
        watchdog(nodes())
    end.

start_monitoring([])->
    ok; 

start_monitoring(List_of_node_IDs)->
    io:format('Connecting to nodes ~p~n',[List_of_node_IDs]), 
    io:format('Nodes are ~p~n',[nodes()]),
    Monitor = fun(Node_ID)->
                    erlang:monitor_node(Node_ID, true)
                end,
    lists:foreach(Monitor, List_of_node_IDs). 




%Handling a node-breakdown, also works if the elevator stops 
handle_node_down(Node)-> 
    order_queue:change_is_host_active(Node, false), 
    order_queue:ask_host_info(Node, self()), 
    receive
        {?HOST_INFO, {Node, _, Orders}} ->
            io:format('Got host info~p~n',[{Node, status, Orders}]);
        Catch_all->
            io:format('handle_node_down got unknown message ~p~n',[Catch_all]),
            Orders = []
    after  10000->
        io:format('Queue seems to be dead (Seen from ~p )~n', [self()]),
        Orders = []
    end,
    Hall_orders = get_hall_orders(Orders),
    spawn( fun()-> add_orders_from_dead_node(Hall_orders) end), 
    self().




add_orders_from_dead_node([])->
    ok; 
add_orders_from_dead_node([ {Floor, Dir} | Rest])->
        order_queue:button_pressed(Floor, Dir),
        timer:sleep(?LOOTING_PERIOD+ random:uniform(?LOOTING_PERIOD*2)), 
        add_orders_from_dead_node(Rest). 
    


get_hall_orders(Orders)->
    io:format('Watchdog has gotten order ~p and removes all cab-orders~n',[Orders]),
    lists:filter( fun( {_, Dir})-> Dir =/= cab end, Orders).



%Runs slowly, but will prevent unknown orders from cloging the queue
concistency_checker_process()->
    timer:sleep(?CONCISTENCY_CHECK_PERIOD),
    Node_to_check = lists:nth( random:uniform(length(nodes())), nodes()), 
    flush(),
    order_queue:ask_host_info(Node_to_check, self()),
    receive 
        {?HOST_INFO, {Node_to_check, {Own_floor,Own_dir,Own_is_active} , Own_orders}}-> ok  end,
    spawn(Node_to_check, order_queue, ask_host_info, [Node_to_check,  self()]), 
    receive
        {?HOST_INFO, {Node_to_check, {Floor, Dir, Their_is_active}, Their_orders}}-> 
            Unaccounted_orders = Own_orders-- Their_orders, 
            if
                Their_is_active =/= Own_is_active ->
                order_queue:change_is_host_active(Node_to_check, Their_is_active);
                true -> ok
            end,
            if
                Floor =/= Own_floor; Dir =/= Own_dir ->
                    order_queue:new_floor_and_dir(Node_to_check,Floor, Dir);    
                true ->ok            
            end,
            Handle_inconsistent_order = fun({Order_floor, Order_dir})->
                                            spawn( Node_to_check, order_queue , add_order, [{Node_to_check, Order_floor, Order_dir}, self()] )

            end,
            lists:foreach(Handle_inconsistent_order, Unaccounted_orders),
            case {Own_is_active,Their_is_active} of 
                {?ACTIVE, ?INACTIVE}->
                    watchdog:handle_node_down(Node_to_check);
                _-> ok
            end
    
    after ?TIME_UNTILL_NODE_ASSUMED_DEAD->
        concistency_checker_process()
    end.
    




%Helpers 


flush()->
    receive 
        _->
            flush()
    after 0->
        ok
    end.