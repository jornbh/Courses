
%%Global constants
-define(NUMBER_OF_FLOORS, 4). 
-define(POLLING_PERIOD, 100).    %Number of milseconds between each time the elavator is polled
-define(EXPECTED_TIME_TO_REACH_FLOOR, 7000).
-define(TIME_UNTILL_CLOSING_DOOR, 10000). 
-define(INF, 100000000). 
-define(TIME_UNTILL_NODE_ASSUMED_DEAD, 3000).
-define(DOOR_OPEN_TIME, 3000).
-define(TIME_PERIOD_FOR_POLLING_IF_NAME_IS_SET, 500).


%Rgistered process IDs   ( USING IDs makes the program prone to chrashing when beeing started)
-define(ELEVATOR_WRITE_ID, elevator_write_ID).
-define(FSM_ID, fsm_ID).
-define(ORDER_SYNCRONIZER_ID, order_syncronizer_ID). 
-define(QUEUE_ID,  queue_ID).

% Test if atoms are of a specific type
-define(IS_MOTOR_DIR(Dir), Dir =:= up orelse Dir =:= down orelse Dir=:=still).
-define(IS_BUTTON_DIR(Dir), Dir =:= up orelse Dir =:= down orelse Dir=:=inside).




%%% COMMANDS TO ELEVATOR WRITE
-define(SET_MOTOR_DIR_COMMAND, set_motor_dir_command).
-define(SET_FLOOR_INDICATOR_COMMAND, set_floor_indicator_command).
-define(SET_ORDER_BUTTON_COMMAND, set_order_button_command).
-define(SET_DOOR_OPEN_COMMAND, set_door_open_command).



%% TAGS FOR MESSAGES

-define(NEW_FLOOR_AND_DIR, new_floor_and_dir).
-define(BUTTON_PRESSED, button_pressed).
-define( ADD_ORDER, add_order).
-define( PIDS, pids).
-define( GET_HOST_INFO, get_host_info).
-define( CHANGE_IS_HOST_ACTIVE,host_died).
-define( FLOOR_SERVED,floor_served).
-define( ASK_FOR_DESTINATIONS, ask_for_destinations ).

%% DEFINITIONS OF THE MESSAGES
-define( SERVING_FLOOR, serving_floor).
-define( FIRST_DESTINATION, first_destination ).
-define( ELEVATOR_IS_STUCK, elevator_is_stuck).                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
-define( GET_FIRST_DESTINATION, get_first_destination).
-define( ADD_HOST,add_host).

-define(HOST_INFO,host_info ).


-define(CHANGED_DIR,changed_dir).



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                              
%             ISSUES:                                                 
%          - Registering IDs causes the program to crash some of the time                                                     
%                                                              
%                                                                                                              
%                                                              
%                                                              
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%Status-macros 
-define(INACTIVE,false ).
-define(ACTIVE,true ).
-define(REDUNDANT,redundant ).


