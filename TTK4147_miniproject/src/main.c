#include "../miniproject-files/network/udp.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <semaphore.h>
#include "timeDifference.h"
#include "globalHeader.h"
#include "sharedVariables.h"


// Actual global variables
float Y; 
UDPConn* CONN; 
float REF; 




// Worker functions from ./main_workers/
void* receiver_main(void* args); 
void* controller_main(void* args); 


int main(){

	timeDifference_init(); // getDt will input one wrong value if it is not run

    //pthread_mutex_init(&Y_LOCK, NULL); 
    //sem_init(&SIG_SEM, true, 0);
    CONN = udpconn_new(IP_ADDR, 9999);
    
    pthread_t threadHandles[2];
    pthread_create(&threadHandles[0], NULL, receiver_main, NULL);
    pthread_create(&threadHandles[1], NULL, controller_main, NULL);
    
    REF = 1;
    usleep(1000*1000);
    REF = 0;
    usleep(1000*1000);

    udpconn_send(CONN, "SIGNAL_ACK");
    udpconn_send(CONN, "STOP");
    
     
   

	

	return 0;

}

