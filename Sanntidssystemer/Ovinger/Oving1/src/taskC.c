#include "taskC.h"
#include "time_operators.h"
#include <stdio.h>
#include <string.h>
#include "sched.h"
long C_NS_PER_S = 1000*1000*1000;
const long C_nSims = 1000*1000*10; //For latency test

int C_testAccessLatency_timespec(){
    struct timespec now, start;
    clock_gettime(CLOCK_MONOTONIC ,&start);
    

    for(int i =0; i< C_nSims; ++i){
        sched_yield();
        clock_gettime(CLOCK_MONOTONIC ,&now);
   }

   long dt = timespec_cmp(now, start);//s 
   printf("Time used to test latency (timespec) : %ld\n", dt);
   printf("Latency = %ld (ns)\n", (dt*C_NS_PER_S)/(C_nSims));


    return 0;
}

void C_testResolution_timespec(){

    C_gnuPlot_timespec();

}

void C_gnuPlot_timespec(){
    struct timespec first, last;
    int ns_max = 200; 
    int histogram[ns_max]; 
    memset(histogram, 0, sizeof(int)*ns_max); 
    for(int i = 0; i < C_nSims; i++){ 
        clock_gettime(CLOCK_MONOTONIC, &first);// t1 = timer() 
        sched_yield();

        clock_gettime(CLOCK_MONOTONIC, &last);// t2 = timer() 
        int ns = timespec_cmp(last, first); // (t2 - t1) * ?? 
        if(ns >= 0 && ns < ns_max){ 
            histogram[ns]++; 
        } 
    } 
    for(int i = 0; i < ns_max; i++){ 
        printf("%d\n", histogram[i]); 
} 
}


int C_testAccessLatency_times(){
    struct tms now, start;
    times(&start);
    const long C_nSims = 1000*1000*10;

    for(int i =0; i< C_nSims; ++i){
        sched_yield();
        times(&now);
   }

   long dt = diffTime(now, start);
   printf("Time used to test latency (times): %ld\n", dt);
   printf("Latency = %ld (ns)\n", (dt*C_NS_PER_S/100)/(C_nSims));


    return 0;
}


void C_testResolution_times(){

    C_gnuPlot_times();

}

void C_gnuPlot_times(){
    struct tms first, last;
    int ns_max = 200; 
    int histogram[ns_max]; 
    memset(histogram, 0, sizeof(int)*ns_max); 
    for(int i = 0; i < 10*10*1000; i++){ 
        times(&first);// t1 = timer() 
        sched_yield();
        times(&last);// t2 = timer() 
        int ns = diffTime(last, first); // (t2 - t1) * ?? 
        if(ns >= 0 && ns < ns_max){ 
            histogram[ns]++; 
        } 
    } 
    for(int i = 0; i < ns_max; i++){ 
        printf("%d\n", histogram[i]); 
} 
}

void taskC(int argc, char** argv){
    if (argc<3){
        printf("Not enough arguments");
        return;
    }
    char* programs[4] = {   "Latency_times"      ,
                            "Resolution_times"   ,
                            "Latency_timespec"   ,
                            "Resolution_timespec"};

    if(0== strcmp(argv[2], programs[0])){
        C_testAccessLatency_times();
    }
    else if(0 == strcmp(argv[2], programs[1])){
        C_testResolution_times();
    }
    else if(0 == strcmp(argv[2], programs[2])){
        C_testAccessLatency_timespec();
    }
    else if(0 == strcmp(argv[2], programs[3])){
        C_testResolution_timespec();
    }
}