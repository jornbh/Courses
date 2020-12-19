 -module(cost_function).
-compile(export_all).
-include("global_constants.hrl").
-export([get_cost/2]).
-define(COST_OF_SERVING, 10).
-define(COST_OF_MOVING, 1).


%  An alternative implementation. Handles overlap, but is not perfect by any measns. Delete if needed

get_cost({Floor, cab}, {ID, _,Orders})->
    case {node(), lists:member({Floor,cab}, Orders)} of 
        {_, true}->
            ?REDUNDANT; 
        {ID, _} ->
            -1; 
        _ ->
            ?INF
    end;
get_cost(Order, {ID, Status, Orders})->
    case is_overlap(Order, {ID, Status, Orders}) of 
        true ->
            ?REDUNDANT; 
        false ->
            calculate_cost(Order, {ID, Status, Orders})
    end.



is_overlap( {Floor, Dir}, {_ID, {_,_,Is_active} , Orders}) when Dir =/= cab-> 
    Is_overlapping_order = fun( {O_floor, O_dir})->
                                Floor =:= O_floor
                                andalso (Dir =:= O_dir orelse O_dir =:= cab)
                                
                            end,
    lists:any(Is_overlapping_order,Orders ) andalso Is_active =/= ?INACTIVE;
is_overlap({ Floor, cab}, {ID, _Status, Orders})->
 
    lists:member({Floor, cab}, Orders) andalso node()=:= ID.


calculate_cost( _, {_, {_,_, ?INACTIVE},_ })->
    ?INF; 
calculate_cost({Floor, _Dir}, {_, {Start_floor, _Start_dir, ?ACTIVE}, Orders})->
    Floors_to_visit  = [ O_floor || {O_floor, _O_dir} <- Orders ]    ++ [Floor,Start_floor],
    Span = lists:max( Floors_to_visit)- lists:min(Floors_to_visit),                        
    ?COST_OF_SERVING * length(Orders) + ?COST_OF_MOVING * Span.