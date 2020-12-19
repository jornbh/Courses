-module(order_queue).
-compile(export_all).
-include("global_constants.hrl").


%API - functions 

add_order({ID, Floor, Dir}, From_PID)->
    {?QUEUE_ID, node()} ! {?ADD_ORDER, {{ID, Floor, Dir}, From_PID}}.


button_pressed(Floor, Dir) when Dir == hall_up; Dir == hall_down;Dir == cab ->
    {?QUEUE_ID, node()} ! {?BUTTON_PRESSED, {Floor, Dir}}.


broadcast_floor_served(Floor)->
    Send_floor_served = fun(Node_ID)-> {?QUEUE_ID,Node_ID } ! {?FLOOR_SERVED, {node(), Floor}} end,
    lists:foreach(Send_floor_served, [node() | nodes()]).

broadcast_new_floor_and_dir(Floor,Dir)->
    Spawn_new_floor_and_dir = fun( Node_ID)-> spawn( Node_ID, ?MODULE, new_floor_and_dir, [node(), Floor, Dir]) end , 
    lists:foreach( Spawn_new_floor_and_dir, nodes()). 

new_floor_and_dir(Node_ID, Floor, Dir)->
    {?QUEUE_ID, node()} ! {?NEW_FLOOR_AND_DIR, {Node_ID, Floor, Dir}}.
broadcast_elevator_is_stuck()->
    Send_elevator_is_stuck_to_order_queue =fun(Node_ID)->
                                {?QUEUE_ID, Node_ID} ! {?CHANGE_IS_HOST_ACTIVE, { node(), false}}
                            end, 
    lists:foreach(Send_elevator_is_stuck_to_order_queue, [node() | nodes()]), 
    spawn( fun()-> watchdog:handle_node_down(node()) end). % Distribute all orders from the node that has become stuck 
change_is_host_active(Host_ID, Is_active)-> 
    {?QUEUE_ID, node()} ! {?CHANGE_IS_HOST_ACTIVE, {Host_ID, Is_active}}.
    

ask_host_info(Host_ID, Asker_PID)->
    {?QUEUE_ID, node()} ! {?GET_HOST_INFO, {Host_ID, Asker_PID}}.
    
add_host()->
    {?QUEUE_ID, node()} ! {?ADD_HOST}.

ask_for_destinations()-> % Only used to keep the elevator from waiting too long
    {?QUEUE_ID, node()} ! {?ASK_FOR_DESTINATIONS}.

spawn_queue()->
        spawn( fun()-> init_queue() end).
    

%%%%%%%%%% Initialization

init_queue()->
        case node() of % Makes sure the node has an actual name 
            'nonode@nohost'-> 
                timer:sleep(?TIME_PERIOD_FOR_POLLING_IF_NAME_IS_SET),
                init_queue();
            _-> ok
        end,
        receive % Waits for the elevator to be initialized
            {FSM_PID, Driver_PID}->
                PIDS = {FSM_PID, Driver_PID};
            Other -> 
                %io:format('init_queue got wromg message ~p~n',[Other]),
                 FSM_PID = error, PIDS = error, exit("")
        end,
        %io:format('Init queue...'),
        Make_host =   fun( Node_ID)-> % Elevators are assumed stuck untill status is returned
                {Node_ID, { 1, up, ?INACTIVE}, []} 
            end,
        Rest_of_world = lists:map(Make_host , nodes()), 
        Own_orders = queue_utility:read_all_orders_from_file(), %TODO set cab-button lights on recovery
        Own_host = {node(), {1,up, ?INACTIVE},Own_orders },
        World = [ Own_host | Rest_of_world ], 

        initialize:force_register(?QUEUE_ID),  
        queue_utility:spawn_recovery_process(),
        %! Give the fsm the first orders
        Destinations = queue_utility:get_next_destinations(World),
        fsm_elevator:give_destination_list(Destinations,FSM_PID), 
        %io:format('complete ~n'),
        queue_loop(World, PIDS), 
        %io:format('Queue loop exited~n'),
        try queue_loop(World, PIDS ) of 
            _->
                    %io:format('QUEUE_LOOP EXITED "NORMALY" ~n'),
                    ok
        catch
            A:B -> %io:format('QUEUE_LOOP CRASHED  ~p ~n', [{A,B}]),
            ok
        end.
        
    

queue_loop(World, PIDS)->
    %io:format('New round of queue_loop, World: ~p~n', [World]), 

    receive 
        {?ADD_HOST}->
            New_world = handle_add_host(World);
        {?CHANGE_IS_HOST_ACTIVE, {Host_ID, Is_active}}->
            New_world = handle_change_is_host_active({Host_ID, Is_active}, World, PIDS);
        {?NEW_FLOOR_AND_DIR, {Host_ID, Floor, Dir}}->
            New_world = handle_new_floor_and_dir({Host_ID, Floor, Dir}, World, PIDS);
        {?ADD_ORDER,{Order, From_PID}}->
            New_world = handle_add_order(Order, From_PID, World, PIDS);
        {?BUTTON_PRESSED, {Floor, Dir}}->
            New_world = handle_button_pressed( {Floor, Dir}, World, PIDS);

        {?GET_HOST_INFO, {Host_ID, Asker_PID}}->
            Host = lists:keyfind(Host_ID, 1, World), 
            Asker_PID ! {?HOST_INFO, Host}, 
            New_world = World;
        {?FLOOR_SERVED, {Host_ID, Floor}}->
            New_world = handle_floor_served({Host_ID, Floor}, World, PIDS);
        {?ASK_FOR_DESTINATIONS}->
            { FSM_PID, _} = PIDS, 
            update_dest(World, FSM_PID), 
            New_world = World;

        Catch_all->
            %io:format('Unknown message in queue loop; got ~p~n', [Catch_all]), 
            %io:format('Printing world~p~n',[World]),
            New_world = World
    end,
    queue_loop(lists:sort(New_world), PIDS). % Sorting makes things more readable and deterministic


%Handlers (Separate functions to make testing easier)

handle_add_host(World)->
    IDs = lists:map( fun( {ID, _, _})-> ID end, World), 
    Unknowns = nodes() -- IDs, 
    New_hosts = lists:map( fun( ID)-> { ID, {1, up, ?INACTIVE}, [] }end, Unknowns),
    Spawn_querry_process = fun(Node_ID)-> 
                                spawn( fun()-> queue_utility:get_host_info_process(Node_ID, Node_ID) end)
                           end,
    lists:foreach( Spawn_querry_process, Unknowns),
    World ++ New_hosts.



handle_button_pressed( {Floor, Dir}, World, {FSM_PID, Driver_PID})->
    %io:format('handle_button_pressed is called~p~n',[{Floor, Dir}]),
    case queue_utility:assign_order({Floor, Dir}, World) of 
        ?REDUNDANT -> 
            %io:format('Redundant order~p~n',[{Floor, Dir}]),
            World; % Someone else already handles the order
        Assigned_order -> 
            %io:format('Order assigned: ~p~n',[Assigned_order]),
            spawn( fun() ->  queue_utility:redundancy_and_lights_process(Assigned_order, {FSM_PID, Driver_PID}) end),
            {ID, _,_} = Assigned_order, 
            case lists:keymember(ID, 1, World) of 
                true ->
                    ok; 
                false -> 
                    %io:format('REEEEEEEEEEEEEEEE, Not member of world~p ~n', [{ID, nodes()}]) 
                        ok
                end,
            New_world =queue_utility:add_order_to_own_world(Assigned_order, World),
            update_dest(New_world, FSM_PID),
            New_world
    end.




handle_change_is_host_active({Host_ID, Is_active}, World, {FSM_PID, _})->
        case node() of 
            Host_ID-> update_dest(World, FSM_PID);
            _-> ok
        end,
        case lists:keyfind(Host_ID, 1, World) of 
            false -> 
                %io:format('Host_ID~p~n',[{Host_ID, Is_active}]),
                %io:format('World~p~n',[World]),
                New_world = handle_add_host(World), 
                handle_change_is_host_active({Host_ID, Is_active}, New_world, {FSM_PID, filler});
            {Host_ID, {Floor, Dir, _old_is_active}, Orders}->
                lists:keyreplace(Host_ID, 1,  World, {Host_ID, {Floor, Dir, Is_active}, Orders})
        end.


handle_new_floor_and_dir({Host_ID, Floor, Dir}, World, {FSM_PID, _})-> % not serving the floor
        case node() of 
            Host_ID -> update_dest(World, FSM_PID);
            _-> ok
        end,
        case lists:keyfind(Host_ID, 1, World) of 
            false ->
                New_world = handle_add_host(World), 
                handle_new_floor_and_dir({Host_ID, Floor, Dir}, New_world, {FSM_PID, filler});
            {Host_ID, {_, _, _ }, Orders}->
                Updated_host = { Host_ID, {Floor,Dir,  true}, Orders},
                lists:keyreplace( Host_ID, 1, World, Updated_host)
        end.


handle_add_order(Order,From_PID, World, {FSM_PID, Driver_PID})->
    queue_utility:turn_on_lights_button_pressed(Order, on,  {FSM_PID, Driver_PID}),
    io:format("turned light on"),
    From_PID ! {ack, Order},
    New_world = queue_utility:add_order_to_own_world(Order, World),
    update_dest(World, FSM_PID), 
    New_world.

handle_floor_served({Host_ID, Floor}, World, {FSM_PID, Driver_PID})->
    queue_utility:turn_off_lights_floor_served({Host_ID, Floor}, off, Driver_PID),
    io:format("turned light off"),
    case node() of 
        Host_ID ->
            queue_utility:remove_order_from_file({Host_ID, Floor});
        _-> ok
    end,
    New_world = queue_utility:remove_served_floor_from_own_world({Host_ID, Floor}, World), 
    update_dest(New_world, FSM_PID), 
    New_world.
        


update_dest(World, FSM_PID)-> %TODO: Ikke superbeskrivende navn?
    io:format("update dest: W =  ~p ~n", [World]),
    Next_destinations =queue_utility:get_next_destinations(World), 
    fsm_elevator:give_destination_list(Next_destinations, FSM_PID). 

