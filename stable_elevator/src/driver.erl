-module(driver).
-export([start/0]).
-export([start/2]).
-define(POLL_PERIOD, 50).
-define(BUTTON_TYPES, [hall_up, hall_down, cab]).
-include("global_constants.hrl").
-compile(export_all).
%Module API - These are working!!
set_motor_direction(Direction, Driver_PID)           when is_pid(Driver_PID) ->  Driver_PID !  {set_motor_direction, Direction              }.
set_door_open_lamp(State, Driver_PID)                when is_pid(Driver_PID) ->  Driver_PID !  {set_door_open_lamp,  State                  }.
set_stop_lamp(State, Driver_PID)                     when is_pid(Driver_PID) ->  Driver_PID !  {set_stop_lamp,       State                  }.
set_floor_indicator(Floor, Driver_PID)               when is_pid(Driver_PID) ->  Driver_PID !  {set_floor_indicator, Floor                  }.
set_button_lamp(Floor, Direction, State, Driver_PID) when is_pid(Driver_PID) ->  Driver_PID !  {set_floor_indicator, Direction, Floor, State}.


spawn_driver(Floor_subscriber, Button_subscriber)->
    spawn( fun()-> start(Floor_subscriber, Button_subscriber) end).

start() ->
    {ok,Driver}=elevator_interface:start(),
    InitialFloor=elevator_interface:get_floor_sensor_state(Driver),
    print_floor_sensor_state(InitialFloor),
    spawn(fun()-> floor_sensor_poller(self(), InitialFloor, Driver) end),
    foreach_button(fun(Floor, Direction) -> spawn(fun() -> order_button_poller(self(), Floor, Direction, 0, Driver) end) end),
    elev_input_poller(Driver).


start( Floor_subscriber, Button_subscriber) ->
    case elevator_interface:start() of
        {ok, Driver}-> ok;
        {A,B} -> 
            io:format('Elevator interface unable to initialize, got ~p~n',[{A,B}]),
            exit("Elevator interface crashed on init"), 
            Driver = error
    end,

    InitialFloor=elevator_interface:get_floor_sensor_state(Driver),
    print_floor_sensor_state(InitialFloor),
    Floor_subscriber ! {initial_floor,InitialFloor},
    spawn(fun()-> floor_sensor_poller(Floor_subscriber, InitialFloor, Driver) end),
    foreach_button(fun(Floor, Direction) -> spawn(fun() -> order_button_poller(Button_subscriber, Floor, Direction, 0, Driver) end) end),
    elev_input_poller(Driver).

%Calls a function for each button on the elevator
%The function that is called for each button must take in the argumetns Floor, and Direction
foreach_button(FunctionForeachButton) ->

    TopFloorButtonTypes = lists:delete(hall_up, ?BUTTON_TYPES),
    BottomFloorButtonTypes = lists:delete(hall_down, ?BUTTON_TYPES),
    OtherFloorButtonTypes = ?BUTTON_TYPES,
    
    ForeachDirection = fun(FunctionForeachDirection, Floor) ->
			       if
				   Floor == 0 ->
				       lists:foreach(FunctionForeachDirection, BottomFloorButtonTypes);
				   Floor == ?NUMBER_OF_FLOORS-1 ->
				       lists:foreach(FunctionForeachDirection, TopFloorButtonTypes);
				   (Floor > 0) and (Floor =< ?NUMBER_OF_FLOORS-1) ->
				       lists:foreach(FunctionForeachDirection, OtherFloorButtonTypes)
			       end
		       end,

    ForeachDirectionWrapper = fun(Floor) -> ForeachDirection(fun(Direction) -> FunctionForeachButton(Floor, Direction) end, Floor) end,
    
    foreach_floor(ForeachDirectionWrapper).



%Calls ForEachDirectionFunction for each floor in NUMBER_OF_FLOORS
foreach_floor(ForEachDirectionFunction) -> 
    FloorIterator = fun(FloorIterator, Floor) ->
			    if 
				Floor == 0 ->
				    ForEachDirectionFunction(Floor);
				(Floor > 0) and (Floor =< ?NUMBER_OF_FLOORS-1) ->
				    ForEachDirectionFunction(Floor),
				    FloorIterator(FloorIterator, Floor-1)
			    end
		    end,
    
    FloorIterator(FloorIterator, ?NUMBER_OF_FLOORS-1),
    ok.





%pollers
floor_sensor_poller(Listener, LastFloor, Driver)->
    ThisFloor = elevator_interface:get_floor_sensor_state(Driver),
    case (ThisFloor /= LastFloor) of
	true ->
        floor_reached(Listener, ThisFloor);
	false ->
	    timer:sleep(?POLL_PERIOD)
    end,
    floor_sensor_poller(Listener, ThisFloor, Driver).



floor_reached(_, between_floors) -> % Simply ignore it
    ok; 
floor_reached(Listener, Floor) -> 
    %print_floor_sensor_state(Floor),
    Listener ! {floor_reached, Floor}.

order_button_poller(Listener, Floor, Direction, LastState, Driver) ->
    ThisState = elevator_interface:get_order_button_state(Driver, Floor, Direction),
    case (ThisState /= LastState) and (ThisState =:= 1) of
	true ->
	    new_order(Listener, Direction, Floor);
	false ->
	    timer:sleep(?POLL_PERIOD)
    end,
    order_button_poller(Listener, Floor, Direction, ThisState, Driver).

new_order(Button_subscriber, Direction, Floor) -> 
    order_queue:button_pressed(Floor, Direction),
    Button_subscriber ! {?BUTTON_PRESSED,{Floor, Direction}},
    print_order(Direction, Floor).



%A function to pass on messages to the elevator interface, as the elevator interface is picky and wants it's own pid as an argument
elev_input_poller(Driver)->
    receive 
        {set_motor_direction, Direction} -> elevator_interface:set_motor_direction(Driver, Direction);
        {set_door_open_lamp, State} -> elevator_interface:set_door_open_light(Driver, State);
        {set_stop_lamp, State} -> elevator_interface:set_stop_button_light(Driver, State);
        {set_floor_indicator, Floor} -> elevator_interface:set_floor_indicator(Driver, Floor);
        {set_floor_indicator, Direction, Floor, State} -> elevator_interface:set_order_button_light(Driver, Direction, Floor, State)
    end,
    timer:sleep(?POLL_PERIOD),
    elev_input_poller(Driver).





%Printers
print_floor_sensor_state(Floor_Sensor_State) ->
    case Floor_Sensor_State of
        between_floors -> io:format("between_floors ~n");
        X -> io:format("floor number: ~b~n",[X])
    end.

print_motor_dir(Direction)->
    case Direction of
        stop -> io:format("motor was stopped~n");
        up -> io:format("motor was started upwards~n");
        down -> io:format("motor was started downwards~n")
    end. 

print_order(Direction, Floor) ->
    case Direction of
        cab -> io:format("cab order at floor number: ~b~n", [Floor]);
        hall_up -> io:format("hall up order at floor number: ~b~n", [Floor]);
        hall_down -> io:format("hall down order at floor number: ~b~n", [Floor])
    end.