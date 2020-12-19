-module(queue_utility).
-compile(export_all).
-include_lib("stdlib/include/ms_transform.hrl").
-include("global_constants.hrl").







redundancy_and_lights_process( {ID, Floor, cab}, PIDS)->
    Self = self(), 
    Share_order = fun(Node)-> spawn( Node, order_queue, add_order, [{ID, Floor, cab}, Self]) end, 
    lists:foreach(Share_order, nodes()),
    save_order_to_file({ID, Floor, cab}), 
    turn_on_lights_button_pressed({ID, Floor, cab},on, PIDS);

redundancy_and_lights_process(Assigned_order, PIDS)->
    Self = self(), 
    Share_order = fun(Node)-> spawn( Node, order_queue, add_order, [Assigned_order, Self]) end, 
    lists:foreach( Share_order, nodes()), 
    receive 
        {ack, Assigned_order}-> % Only need redundancy from one elevator
            %io:format('redundancy_and_lights_process got ~p~n',[ack]),
            turn_on_lights_button_pressed(Assigned_order, on, PIDS);
        Catch_all ->
            %io:format('redundancy_and_lights_process~p~n',[Catch_all]),
            ok
    end.




%%%%%%%%% utility for recovery ( Probably move to another file)

spawn_recovery_process()->
    %io:format('recovery~n'),
    Spawn_querry_process = fun(Node_ID)->
                                spawn( fun()-> get_host_info_process(Node_ID, Node_ID) end)  %Hva skjer med returverdien?
                            end,
    lists:foreach( Spawn_querry_process, nodes()), 
    %io:format('Done~n'),
    ok.











% Inderface with file

save_order_to_file({_ID, Floor, cab})->
    {ok, File} = dets:open_file(cab_orders, []), 
    dets:insert( File, [{Floor, true}]),
    dets:close(File).

remove_order_from_file({_ID, Floor})->
    {ok, File} = dets:open_file(cab_orders, []), 
    dets:insert(File, [{ Floor, false}]), 
    dets:close(File).

read_all_orders_from_file()->
    {ok, File} = dets:open_file(cab_orders, []), 
    Orders_list = dets:select(cab_orders, ets:fun2ms( fun({ Floor, true}) when Floor >= 0, Floor < ?NUMBER_OF_FLOORS -> {Floor, cab}end)),
    dets:close(File), 
    Orders_list.








% Recovery of host-information
get_host_info_process(Unknown_host_ID, Node_to_ask)->
    %io:format('Get host info porcess querrying about ~p~n',[Unknown_host_ID]),
    spawn(Node_to_ask, order_queue, ask_host_info, [Unknown_host_ID, self()]), 
    receive
        {?HOST_INFO, false}-> % The node asked did not know anything
            %io:format('Unable to get any info on the node from the node itself, assuming no reason to ask more~n'),
            ok;
        {?HOST_INFO, {Unknown_host_ID, {Floor, Dir, Is_active}, Orders}}->
            %io:format('Recover got host info~p when asking about ~p ~n',[Orders, Unknown_host_ID]),            
            order_queue:new_floor_and_dir( Unknown_host_ID, Floor, Dir), 
            order_queue:change_is_host_active(Unknown_host_ID,Is_active), 
            lists:foreach(  fun({Order_Floor, Order_Dir} )->
                                order_queue:add_order({ Unknown_host_ID, Order_Floor, Order_Dir}, self()),
                                receive 
                                    M-> 
                                        %io:format('Recovery process got ~p as ack~n',[M]),
                                        ok end
                            end
                        , Orders),
            %io:format('Managed to send add all Orders~p~n',[Orders]),
            ok;
        Catch_all ->
            %io:format('get_host_info_process got an unknown message ~p~n',[Catch_all]),
            ok
        after ?TIME_UNTILL_NODE_ASSUMED_DEAD ->
            %io:format('Timed out~n'),
            case nodes() of 
                []-> ok;
                _-> get_host_info_process(Unknown_host_ID, lists:nth( random:uniform(length(nodes())), nodes())) 
            end
    end.






% Other          
assign_order({Floor, cab},_)-> %TODO Does not assign order
    {node(), Floor, cab};
assign_order(Unassigned_order, World)->
        Get_IDs_and_costs =   fun({ID, Status, Orders})->
                                {ID, cost_function:get_cost(Unassigned_order, {ID, Status, Orders})}
                            end,
        IDs_and_costs= lists:map(Get_IDs_and_costs, World),
        case lists:keymember(?REDUNDANT, 2, IDs_and_costs) of 
            true -> 
                ?REDUNDANT; 
            false->
                { ID, _Cost} = hd( lists:keysort(2, IDs_and_costs)),
                {Floor, Dir} = Unassigned_order,
                {ID, Floor, Dir}
            end.



add_order_to_own_world({Node_ID,Floor,cab}, World)->
        {Node_ID, Status, Orders} = lists:keyfind(Node_ID, 1,World), 
        New_orders =  [ {Floor, cab} | Orders -- [ {Floor, hall_up}, {Floor, hall_down}, {Floor, cab}]], 
        lists:keyreplace( Node_ID, 1, World, {Node_ID, Status, New_orders});
    
add_order_to_own_world({Node_ID, Floor, Dir}, World)->
    {Node_ID, Status, Orders} =  lists:keyfind(Node_ID, 1, World), 
    Is_redundant_order = fun( {Order_floor, Order_dir})-> 
                                (Order_dir == cab orelse Order_dir == Dir)
                                andalso Order_floor == Floor
                        end, 
    case lists:any(Is_redundant_order, Orders) of 
        true -> 
            World; 
        false-> 
            New_orders = Orders ++ [{Floor, Dir}], 
            lists:keyreplace( Node_ID, 1, World, { Node_ID, Status, New_orders})
    end.

    
    
remove_served_floor_from_own_world({Host_ID, Floor}, World)->
    Remove_covered_orders = fun({ID, Status, Orders})->
                                case ID of
                                    Host_ID ->
                                        New_orders = Orders-- [{Floor, hall_up}, {Floor, hall_down}, {Floor, cab}];
                                    _->
                                        New_orders = Orders-- [{Floor, hall_up}, {Floor, hall_down}]
                                end,
                                {ID, Status, New_orders}
                            end,
    lists:map(Remove_covered_orders, World).

    

%NOT IMPLEMENTED YET 
turn_on_lights_button_pressed({ID, Floor, Dir }, on, {_ , Driver_PID})-> % When button pressed and covered
    case {node(),Dir} of
        {ID, _} -> 
            driver:set_button_lamp( Floor,Dir, on, Driver_PID); 
        {_, _} when Dir =/= cab ->
            driver:set_button_lamp( Floor,Dir ,on, Driver_PID); 
        _->
            ok
    end.
turn_off_lights_floor_served( {ID, Floor}, off, Driver_PID) -> % When arriving at a floor
    case node() of 
        ID ->
            driver:set_button_lamp(Floor, cab,  off, Driver_PID);
        _->
            ok
    end,
    driver:set_button_lamp( Floor,hall_up, off, Driver_PID),
    driver:set_button_lamp( Floor,hall_down, off, Driver_PID).
    
get_next_destinations(World)->
    Own_host = lists:keyfind(node(),1 , World),
    get_next_destinations_from_own_host( Own_host ).


get_next_destinations_from_own_host( {_, _, []})->
    []; 
get_next_destinations_from_own_host( { _,_, [Only_order]})->
    {Floor, _}= Only_order,
    [Floor];
get_next_destinations_from_own_host( {_, {Current_floor, Current_dir,_ }, Orders})->
 

    case Current_dir of 
        up->
            Filter_function_floor = fun({Floor,_})-> 
                Floor >= Current_floor end, 
            Filter_function_dir = fun({_, Dir} )->
                ( Dir == hall_up orelse Dir == cab) end;
        down->
            Filter_function_floor = fun({Floor,_})-> 
                Floor >= Current_floor end, 
            Filter_function_dir = fun({_, Dir} )->
                ( Dir == hall_down orelse Dir == cab) end;
        idle-> 
            Filter_function_floor = fun(_)-> true end, 
            Filter_function_dir = fun(_)-> true end
    end,
    In_same_direction = lists:filter(Filter_function_floor, Orders), 
    Best_orders = lists:filter(Filter_function_dir ,In_same_direction), 
    Second_best_orders = In_same_direction -- Best_orders, 
    Rest_of_orders = Orders -- In_same_direction, 


    Compare_distance_to_host = fun({Floor1, _},{Floor2, _})-> abs(Floor1-Current_floor) =< abs(Floor2-Current_floor) end,
    Final_ordering = 
                        lists:sort(Compare_distance_to_host, Best_orders       ) ++
                        lists:sort(Compare_distance_to_host, Second_best_orders) ++
                        lists:sort(Compare_distance_to_host, Rest_of_orders    ),
    
    [ {Best, _},{ Second, _} | _ ] = Final_ordering, 
    [Best, Second].
    


