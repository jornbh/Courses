-module(initialize). 
-compile(export_all). 
-include("global_constants.hrl").


start_network()-> % For testing

    connector:spawn_connector(), 
    PID = order_queue:spawn_queue(), 
    PID !  {s_probe(), s_probe()}, 
    watchdog:spawn_watchdog().


start_all()->
    %os:cmd( 'gnome-terminal -x [ ../Simulator-v2/SimElevatorServer  ] '), % Does not work
    Queue_PID = order_queue:spawn_queue(), 
    fsm_elevator:spawn_fsm(Queue_PID), 
    receive
        {FSM_PID, Driver_PID}-> ok
    end,
    reset_button_lights(Driver_PID), 
    Queue_PID ! {FSM_PID, Driver_PID}, 
    connector:spawn_connector(), 
    watchdog:spawn_watchdog(), 
    io:format('All spawned~n'), 
    Driver_PID. 






% Universal function
force_register(Process_Identifier)->
    try register_shell(Process_Identifier) of 
        _->
            io:format('Good: process ~p  registered as ~p ~n', [ self(), Process_Identifier])
    catch
        error:badarg->
            unregister(Process_Identifier), 
            io:format('~p already registered, unregistering it and trying again~n', [Process_Identifier]),
            force_register(Process_Identifier)
    end.


register_shell(Process_Identifier)->
    register(Process_Identifier, self()).





%Helpers (DEBUG)
s_probe()-> % TODO delete this
    spawn( fun()-> probe() end).
probe()->
    receive  M -> io:format('Probe got~p~n',[M]) end,
    probe().



reset_button_lights(Driver_PID)->
    Floors = lists:seq(0, ?NUMBER_OF_FLOORS -1), 
    Button_directions = [hall_up, hall_down, cab], 
    Clear_in_all_directions = fun( Floor)->
                                   lists:foreach( fun(Dir)-> driver:set_button_lamp(Floor, Dir, off, Driver_PID) end, Button_directions)
                            end,
    
    lists:foreach( Clear_in_all_directions, Floors).
