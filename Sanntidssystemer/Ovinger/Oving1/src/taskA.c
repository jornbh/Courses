#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "unistd.h"
#include "taskA.h"
#include "sys/times.h"
#include "time_operators.h"



int taskA(int argc, char* argv[]){
    char* Programs[3]= {"sleep", "busy_wait","busy_wait_times" };
    for (int i =2; i< argc; ++i){ // First argument is the program that is run
        if (cmpStrings(argv[i], Programs[0])){
            printf("Legal argumet: %s\n", argv[i]);
            taskA_sleep(); 
        }
        else if (cmpStrings(argv[i], Programs[1])){
            printf("Legal argumet: %s\n", argv[i]);
            taskA_busy_wait(); 
        }
        else if (cmpStrings(argv[i], Programs[2])){
            printf("Legal argumet: %s\n", argv[i]);
            taskA_busy_wait_times(); 
        }                    
        else{
            printf("Invalid argumet: %s\n", argv[i]);
        }
    }
    if(argc ==0){
        printf("No arguments given");
    }
    return 0;
}

  
    



int taskA_sleep(){
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    sleep(1); // Sleep for a number of seconds
    clock_gettime(CLOCK_MONOTONIC, &end); 
    int dt = timespec_cmp( end, start);


    printf("Time %i\n", dt); 
    return 0; 
}
int taskA_busy_wait(){
    struct timespec start, end, waitTime;
    clock_gettime(CLOCK_MONOTONIC, &waitTime);
    waitTime.tv_sec = 1;
    waitTime.tv_nsec = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    busy_wait(waitTime);
    clock_gettime(CLOCK_MONOTONIC, &end); 
    int dt = timespec_cmp( end, start);


    printf("Time %i\n", dt); 
    return 0; 
}

int cmpStrings(char* A, char* B){
    int i; 
    for(i =0; A[i] != '\0' && B[i] != '\0'; ++i){
        if( A[i] != B[i]){
            return 0; 
        }
    }

    
    return (A[i] == B[i]);
}




void taskA_busy_wait_times(){
    struct tms start, now; 
    int dt = 100;
    printf("Start");
    times( &start);
    
    for(times( &now); diffTime(now, start) < dt ; times(&now)){
        // printf("Now %ld \n",now.tms_cutime +now.tms_stime);

    }
    printf("Time: %ld \n",diffTime(now, start));

}




