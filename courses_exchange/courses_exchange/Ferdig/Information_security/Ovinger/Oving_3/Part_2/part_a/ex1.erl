-module(ex1). 
-compile(export_all).
-export([start/0]).
% -import(lists, [for])


start()-> 
    io:format('Start~n'),
    Master_PID = spawn( fun main/0 ), 
    io:format('Spawned~n'), 
    Master_PID. 
main()-> 
    io:format('Started~n'),
    Passwords = [

        90792937094113405555085365844785559631068690185944582196836712576996875098365,
        59750918889120849302495167790612318061274033782502201997179034346185110339133,
        73082067550867551622655113613128550001744408664494945567625016080292353385515,
        78676163539168230107823418552966867837870200609886463556465144829450992734750,
        57584953443923523694497966717271866037989909994165483836363430250215486587103,
        71740340355429439683724763612476193950881099835554509790683419679034293093455,
        92776796231306472109036570565024678987830846970176840507035160340495164858968,
        87648603532466382439442897133291108765567239528886166023291613945311319448615,
        12864219034843368427789276651181136557129805490571651006961653545606538242172,
        65111174471666760020666763667497422884650861440289982917619844850269574542795,
        61889384266393409576963907938984124852536365869469708533778895900682962015625 
        ], 


     
    Alphabet = lists:seq($a, $z)++ lists:seq($0, $9), 
    register(  master, self()), 
    Worker_fun = fun(Base)-> worker_top_fun( Alphabet, 4,6, Base, Passwords ) end,
    Spawn_worker = fun(Base)-> spawn( fun()-> Worker_fun(Base) end) end,
    PIDS = lists:map(Spawn_worker, [ [A,B] || A <- Alphabet, B<-[$a]] ), 
    io:format('PIDS~p~n',[PIDS]),
    Drop_PID = spawn(fun()-> drop_writer(100) end),
    register(drop_writer, Drop_PID), 
    io:format('Starting to listen~p~n',[variable]),


    master_loop(Passwords, []). 

% master_loop(_,_) when size([1,2,3] ) =<10->
master_loop(Passwords, Cracked ) when size(Passwords) =< size(Cracked)-> 
    [io:format('Password: ~p~n',[A]) || A <- Cracked];
master_loop(Passwords_to_get, Cracked_passwords)->
    % io:format('Master_loop~n'),
    receive 
        {status, PID, Current_base}-> 
            {drop_writer, node()} ! {status, PID, Current_base},
            % io:format('Got base ~p from node ~p~n',[Current_base, PID]),
            master_loop(Passwords_to_get, Cracked_passwords); 
        {password, Password}->
            io:format('Got a Password: ~p~n',[Password]), 
            master_loop(Passwords_to_get, [Password | Cracked_passwords]);

        Message -> 
            io:format('Got message: ~p~n',[Message]),
            master_loop(Passwords_to_get, Cracked_passwords)

    after 10000-> io:format('Still going~p~n', [self()]), 
        master_loop(Passwords_to_get, Cracked_passwords)
    end.



worker_top_fun(Alphabet, Min_len, Max_len, Current_base, Passwords)->
    worker(Alphabet, Min_len, Max_len, Current_base, Passwords ), 
    io:format('Done~p~n',[ self() ]).

worker( _, _, Max_len,  Current_base, _Passwords ) when length(Current_base)>Max_len-> 
    case Current_base of
        [$a, $a, $a,$a | _ ]->
            { master, node()} ! {status, self(), Current_base};
        _ -> found_nothing
    end;
worker(Alphabet, Min_len, Max_len, Current_base, Passwords )->

    Hashed = crypto:hash(sha256, Current_base), 
    Deeper_fun = fun(Char)-> 
                    worker(Alphabet, Min_len, Max_len, [ Char| Current_base], Passwords )
                end,
    case lists:any( fun(Password)-> Password =:= Hashed end, Passwords ) of 
        true -> {node(), master} ! Hashed;
        _ -> nothing
    end,
    lists:foreach( Deeper_fun, Alphabet ). 

drop_writer(Sensitivity)-> 
    drop_writer(Sensitivity, 0).

drop_writer(Sensitivity, Counter)-> 
    receive 
        A when is_number(A), A>0, A<20000-> 
            io:format('Changed sensitivity (counter =~p )~p~n',[A, Counter]),

            drop_writer(A, A); 
            
        Message -> 
            case Counter of
                    A when A > Sensitivity  -> 
                        io:format('Drow writer ~p~n',[Message]),
                        % io:format('index~p~n',[variable]),
                        drop_writer(Sensitivity, 0);
                    _ -> 
                        drop_writer(Sensitivity, Counter +1)
            end
            
    end.