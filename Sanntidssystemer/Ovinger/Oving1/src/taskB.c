#include "taskB.h"
#include "time_operators.h"
#include <stdio.h>
#include <string.h>
long NS_PER_S = 1000*1000*1000;
const long nSims = 1000*1000*10; //For latency test

int testAccessLatency_timespec(){
    struct timespec now, start;
    clock_gettime(CLOCK_MONOTONIC ,&start);
    

    for(int i =0; i< nSims; ++i){
        clock_gettime(CLOCK_MONOTONIC ,&now);
   }

   long dt = timespec_cmp(now, start);//s 
   printf("Time used to test latency (timespec) : %ld\n", dt);
   printf("Latency = %ld (ns)\n", (dt*NS_PER_S)/(nSims));


    return 0;
}

void testResolution_timespec(){

    gnuPlot_timespec();

}

void gnuPlot_timespec(){
    struct timespec first, last;
    int ns_max = 50; 
    int histogram[ns_max]; 
    memset(histogram, 0, sizeof(int)*ns_max); 
    for(int i = 0; i < nSims; i++){ 
        clock_gettime(CLOCK_MONOTONIC, &first);// t1 = timer() 
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


int testAccessLatency_times(){
    struct tms now, start;
    times(&start);
    const long nSims = 1000*1000*10;

    for(int i =0; i< nSims; ++i){
        times(&now);
   }

   long dt = diffTime(now, start);
   printf("Time used to test latency (times): %ld\n", dt);
   printf("Latency = %ld (ns)\n", (dt*NS_PER_S/100)/(nSims));


    return 0;
}


void testResolution_times(){

    gnuPlot_times();

}

void gnuPlot_times(){
    struct tms first, last;
    int ns_max = 50; 
    int histogram[ns_max]; 
    memset(histogram, 0, sizeof(int)*ns_max); 
    for(int i = 0; i < 10*10*1000; i++){ 
        times(&first);// t1 = timer() 
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

void taskB(int argc, char** argv){
    if (argc<3){
        printf("Not enough arguments");
        return;
    }
    char* programs[4] = {   "Latency_times"      ,
                            "Resolution_times"   ,
                            "Latency_timespec"   ,
                            "Resolution_timespec"};

    if(0== strcmp(argv[2], programs[0])){
        testAccessLatency_times();
    }
    else if(0 == strcmp(argv[2], programs[1])){
        testResolution_times();
    }
    else if(0 == strcmp(argv[2], programs[2])){
        testAccessLatency_timespec();
    }
    else if(0 == strcmp(argv[2], programs[3])){
        testResolution_timespec();
    }
}