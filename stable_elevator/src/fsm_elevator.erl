-module(fsm_elevator).
-export([spawn_fsm/1, give_destination_list/2]).
-include("global_constants.hrl").
%API function
give_destination_list(Floors, FSM_PID) when is_list(Floors)-> % Must be two, since the queue has no destinction between on a floor and between floors
    io:format('give_destination_list got~p~n',[{Floors, FSM_PID}]),    
    FSM_PID ! {new_destinations_list, Floors};
give_destination_list(Single_floor, FSM_PID)-> % When only one destination is sendt
    io:format('give_destination_list got~p~n',[{Single_floor, FSM_PID}]),
    FSM_PID ! {new_destinations_list, [Single_floor]}.

spawn_fsm(Queue_PID)->
    Self = self(), 
    spawn(fun()-> start( Self, Queue_PID) end).





% Internal functions 

start(Main_PID, Queue_PID) ->

    %Spawning schedule and driver
    Self=self(),
    Driver_PID = spawn(fun() -> driver:start(Self, Queue_PID) end),
    InitialFloor=drive_up_to_closest_floor(Driver_PID),
    io:format('Arrived at initial floor~n'),
    Direction=up,
    Main_PID ! { self(), Driver_PID},% { FSM_PID, Driver_PID}
    order_queue:broadcast_new_floor_and_dir(InitialFloor, Direction),
    state_idle(Direction,InitialFloor, Driver_PID).


state_running(stop, Destination, PreviousFloor, Driver_PID)-> %This function will only be called if software error  
    io:format('State_running was called while the direction was stop~n'),
    exit("No good"), % Delete this to make the elevator more "Robust"
    New_direction = translate_to_next_direction(PreviousFloor, Destination),
    driver:set_motor_direction(New_direction, Driver_PID),
    case New_direction of 
        stop ->
            hit_floor(New_direction, Destination, PreviousFloor,Driver_PID);
        _->
            state_running(New_direction, Destination, PreviousFloor, Driver_PID)   
    end;


state_running(Direction, Destination, PreviousFloor, Driver_PID) ->
    io:format('Running: {Direction, Destination, PreviousFloor, Driver_PID} = ~p~n',[{Direction, Destination, PreviousFloor, Driver_PID}]),
    timer:sleep(50),
    receive
        {floor_reached, Floor} ->
            hit_floor(Direction,Destination,Floor, Driver_PID);
        {new_destinations_list, NewDestinations_list}->
            io:format('state_running got~p~n',[{new_destinations_list, NewDestinations_list}]),
            case NewDestinations_list of
                []->
                    New_destination = none;
                [Single_destination]->
                    New_destination = Single_destination; 
                Several_destinations->
                    New_destination = hd(Several_destinations -- [PreviousFloor])
            end,
            % The elevator will still complete the journey to the next floor
            state_running(Direction, New_destination, PreviousFloor, Driver_PID) 
        after ?EXPECTED_TIME_TO_REACH_FLOOR ->
            order_queue:broadcast_elevator_is_stuck(),
            io:format('?~n'),
            %! JUST A HOTFIX
            New_direction = translate_to_next_direction(PreviousFloor, Destination),
            driver:set_motor_direction(New_direction, Driver_PID),
            %!
            state_running(New_direction, Destination, PreviousFloor, Driver_PID)

    end.

hit_floor(Direction,Destination,Floor, Driver_PID)->
    order_queue:broadcast_new_floor_and_dir(Floor, Direction),
    driver:set_floor_indicator(Floor, Driver_PID),
    case Destination of   %TODO: Test case destination of floor, none, _
        Floor -> 
            io:format('Hit destination~p~n',[Destination]),
            Driver_PID ! {set_motor_direction, stop},
            order_queue:broadcast_floor_served(Floor), 
            state_open(Direction,Destination,Floor, Driver_PID);
        none ->
            driver:set_motor_direction(stop, Driver_PID), 
            state_idle(Direction, Floor, Driver_PID);
        _ ->
            case Direction of % In case the orders change unexpectedly
                up when Destination < Floor-> driver:set_motor_direction(down, Driver_PID);
                down when Destination > Floor -> driver:set_motor_direction(up, Driver_PID); 
                _ -> ok
            end,
            %print_order1(Direction,Floor),
            state_running(Direction,Destination,Floor, Driver_PID)
    end.


state_idle(Direction, Floor, Driver_PID)->
    io:format('State: idle~n'),
    order_queue:broadcast_new_floor_and_dir(Floor, idle), 
    receive 
        {new_destinations_list, []}->
            state_idle(Direction, Floor, Driver_PID);
        {new_destinations_list, NewDestinations_list}->
            io:format('state_idle got~p~n',[{new_destinations_list, NewDestinations_list}]),
            New_destination = hd(NewDestinations_list),
            New_direction = translate_to_next_direction( Floor, New_destination), 
            driver:set_motor_direction(New_direction, Driver_PID), 
            order_queue:broadcast_new_floor_and_dir(Floor, New_direction ),
            if
                New_destination == Floor -> 
                    hit_floor(New_destination, New_destination, Floor, Driver_PID);
                true->
                    state_running(New_direction, New_destination,Floor, Driver_PID)
            end
        after 10000 -> % Something might have gone wrong, saying floor is served to trigger a new order
            io:format('Make better solution for timeout on idle state " Served floor" ~p ~n', [Floor]),
            order_queue:ask_for_destinations(), 
            state_idle(Direction, Floor, Driver_PID)
        end.

state_open(Direction, _Destination, Floor, Driver_PID)->
    Time_left = ?DOOR_OPEN_TIME, 
    state_open(Direction, none, Floor, Driver_PID, Time_left).

state_open(Direction,Destination,Floor, Driver_PID, Time_left)-> %when you are just open, you preserve the direction you came from
    io:format('State: door open~n'),
    driver:set_door_open_lamp(on, Driver_PID),
    Start =erlang:system_time(milli_seconds),
    receive 
        Catch_all->
            case Catch_all of 
                {new_destinations_list, []}-> 
                    New_destination = none; 
                {new_destinations_list, New_destinations_list}->
                    New_destination = hd(New_destinations_list);
                _->
                    io:format('open got invalid message ~p~n',[Catch_all]),
                    New_destination = none
                end,
            
            case New_destination of 
                Floor ->
                    order_queue:broadcast_floor_served(Floor), 
                    state_open(Direction, none, Floor, Driver_PID);
                _->
                    Time_passed = erlang:system_time(milli_seconds) - Start, 
                    New_time_left = Time_left- Time_passed, 
                    if 
                        New_time_left < 0 -> state_open(Direction,New_destination,Floor, Driver_PID, 0            );
                        true              -> state_open(Direction,New_destination,Floor, Driver_PID, New_time_left)
                    end
            end
                                 
    after Time_left ->
            io:format("wait_time is over wihu~n"),
            case Destination of
                    Floor -> 
                        state_open(Direction, Destination, Floor, Driver_PID); % Continue to have door open
                    none ->
                        driver:set_button_lamp(Floor, cab, off, Driver_PID),
                        driver:set_door_open_lamp(off, Driver_PID),  % TODO make better than this hotfix              
                        state_idle(Direction, Floor, Driver_PID);
                    _ ->
                        driver:set_door_open_lamp(off, Driver_PID),
                        driver:set_button_lamp(Floor, cab, off, Driver_PID), % TODO make better than this hotfix                        
                        New_direction = translate_to_next_direction(Floor, Destination),
                        case New_direction of
                            stop-> io:format('open wants to enter state running, while direction is stop: {Dir, Dest, Floor} = ~p~n',[{New_direction, Destination, Floor}]),
                            exit("");
                            _->
                                Driver_PID ! {set_motor_direction, New_direction},
                                state_running(New_direction, Destination, Floor, Driver_PID) %This looks dangerous, why no flying elevator?
                        end
            end
    end.


%% Helping functions

drive_up_to_closest_floor(Driver_PID)->
    receive {initial_floor, InitialFloor}->
        io:format('Got initial-floor-message ~p~n',[{initial_floor, InitialFloor}]),
        case InitialFloor of
            between_floors ->
                Driver_PID ! {set_motor_direction, up},
                First_floor = receive_until_valid_initial_floor(),  
                io:format('got initial floor ~n'),
                io:format('Got valid floor ~p, asking motor to stop ~n',[First_floor]),
                Driver_PID ! {set_motor_direction, stop},
                driver:set_floor_indicator(First_floor, Driver_PID),
                First_floor;
            FloorNumber ->
                FloorNumber
        end;
    Catch_all ->
        io:format('drive_up_to_closest_floor got invalid Message~p~n',[Catch_all]),
        drive_up_to_closest_floor(Driver_PID)

    end.

receive_until_valid_initial_floor()->
    io:format('Process ~p is waiting for a valid floor ~n',[self()]),
    receive 
        {floor_reached, between_floors}->
            io:format('receive_until_valid_initial_floor got between_floors~n'),
            receive_until_valid_initial_floor();
        {floor_reached, Valid_floor}->
            io:format('receive_until_valid_initial_floor ( ~p ) got ~p~n',[self(), Valid_floor]),
            Valid_floor; 
        Catch_all ->
            io:format('receive_until_valid_initial_floor got invalid message ~p~n',[ Catch_all]),
            receive_until_valid_initial_floor()
    end.

translate_to_next_direction(Floor, Destination) ->
    if
        Destination == none -> stop;
        Destination > Floor -> up;
        Destination < Floor -> down; 
        Destination == Floor-> stop; 
        true -> 
            exit("translate_to_next_direction got an invalid type of floor or direction")
    end.
            


