
#include "sharedVariables.h"




#include "../miniproject-files/network/udp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "timeDifference.h"







void* receiver_main(void* args){
	char rcv_buff[100] = "Thread \n";
	int buflen = 100;
	
    
	printf("START_DATA\n"); 
	udpconn_send(CONN, "START");
	udpconn_send(CONN, "GET");
	printf("Q\n");

	
	while(1){
        	udpconn_receive(CONN,rcv_buff,buflen);
        	if(rcv_buff[0] == 'G'){
            		Y= atof(rcv_buff + 8); // 32-bit word on a 32-bit processor will not suffer data-races
        	}
        	else{
            		// Signal 
		udpconn_send(CONN, "SIGNAL_ACK");
        	}

        
	}

	return NULL;
} 
