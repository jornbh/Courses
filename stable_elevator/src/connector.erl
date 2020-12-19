-module(connector).
-compile(export_all).
-export([spawn_connector/0]).
-define(SEND_PORT, 5676).
-define(RECV_PORT, 5675).
-define(SEEK_PERIOD, 2000).
-define( SEND_IP, {255,255,255,255}). 

spawn_connector() ->
    set_node_name(), % epmd -daemon  must be run in terminal first (Dunno why)
    erlang:set_cookie(node(), a_name), 
    spawn(fun() -> listen_for_connections() end),
    spawn(fun() -> broadcast_own_name() end), 
    io:format('Init connector complete~n').
		  
		  
listen_for_connections() ->
     %Open up an udp connection at some port on your computer, sets which port should receive messages
    {ok, RecvSocket} = gen_udp:open(?RECV_PORT, [list, {reuseaddr,true}, {active,false}]),
    listen_for_connections(RecvSocket).

listen_for_connections(RecvSocket) ->
    %Using udp to try and receive the adress port and the Nodename of the other computer broadcasting stuff
    {ok, {_Adress, ?SEND_PORT, NodeName}} = gen_udp:recv(RecvSocket, 0),
    %When the nodename is received, the Node is added to the cluster (if it has not been added before)
    Node = list_to_atom(NodeName),
    case is_in_cluster(Node) of
	true ->
        %If it is in the cluster, continue the process of listening to lifesigns
	    listen_for_connections(RecvSocket);
	false ->
        %If it is not in the cluster, let it join the party!
        Old_cluster = [ node() | nodes()], 
        net_adm:ping(Node),
        Unknowns = nodes() -- Old_cluster, 

        % Make sure all the queues know about eachother
        anounce_new_node_to_all(),
        spawn_recovery_processes(Old_cluster, Unknowns),

        %Then, continue the search for new mates.
	    listen_for_connections(RecvSocket)
    end.

is_in_cluster(Node) ->
    %Make a complete list of the cluster, don't forget to count yourself!
    NodeList = [node()|nodes()],
    %Take advantage of BIF that checks if element is in list    
    lists:member(Node, NodeList).


broadcast_own_name() ->
    {ok, Socket} = gen_udp:open(?SEND_PORT, [list, {reuseaddr,true}, {active,true}, {broadcast, true}]),
    broadcast_own_name(Socket).

broadcast_own_name(SendSocket) ->
    %Sending your own name as a list (why not just send the atom? I dont understand)
    %to all computers inn the entire world (or more realisticly, all the computers your router allows)
    ok = gen_udp:send(SendSocket,?SEND_IP, ?RECV_PORT, atom_to_list(node())),
    timer:sleep(?SEEK_PERIOD* (1 + length(nodes()))),
    broadcast_own_name(SendSocket).



set_node_name()->
    os:cmd("epmd -daemon"),
    try set_node_name([]) of 
        _->
            ok
    catch 
        _:_-> 
            io:format('connector:set_node_name() failed, 
                    most likely because epmd -daemon has not been written in the terminal, try to do that first ;)  ~n'),
            error
    end.


set_node_name(Extension)->
    IP_string = get_ip_address(), 
    Hostname = Extension ++"elevator@" ++ IP_string, 
    net_kernel:start( [ list_to_atom(Hostname), longnames]),

    % If another node has taken the name
    case node() of
        'nonode@nohost'->
            set_node_name( Extension ++ "E");
        _->
            ok
    end.



get_ip_address()->
    {ok, Addrs} = inet:getifaddrs(), 
    IP_tuple = hd([Addr || {_, Opts} <- Addrs,
    {addr, Addr} <- Opts,
    {flags, Flags} <- Opts,
    lists:member(loopback,Flags) =/= true]), 
    inet:ntoa(IP_tuple). % Returns it as a string 



anounce_new_node_to_all()-> % Only tells that something is new, nothing more. 
    Anounce_to_queue = fun(Node_ID)->
                            spawn(Node_ID, order_queue, add_host, [])
                        end,
    lists:foreach( Anounce_to_queue, [ node() | nodes()]).




spawn_recovery_processes(Old_cluster, Unknown_nodes)-> %TODO See if add_host handles this problem by itself 
    From_new_to_old = [{To, From} || To<- Old_cluster, From <- Unknown_nodes ], 
    From_old_to_new = [{To, From} || To<- Unknown_nodes, From <-Old_cluster ],
    Unknown_pairs = From_new_to_old ++ From_old_to_new, % All combinations of nodes that need to know about eachother
    Make_querry_process =fun({Asker, Sender})->
                                     spawn(Asker, queue_utility, get_host_info_process, [Sender, Sender]) 
                                end, 
    lists:foreach(Make_querry_process, Unknown_pairs), 

    ok.
    
