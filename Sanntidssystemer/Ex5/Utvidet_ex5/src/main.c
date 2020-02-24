#define _GNU_SOURCE


#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>
#include "io.h"


#define SLEEP
#define SINGLE_CPU
#define SLEEP_TIME 1000*1000
#define N_THREADS (3)
#ifdef NOT_COMEDI
    void io_write(int A,int B){printf("%d, %d\n", A,B);}
    int io_read(int A){ unsigned static int i=0; i= i%5000;return  i++;}
    void io_init(){void; }

#endif
/// Assigning CPU core ///

int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}




/// 'struct timespec' functions ///

struct timespec timespec_normalized(time_t sec, long nsec){
    while(nsec >= 1000000000){
        nsec -= 1000000000;
        ++sec;
    }
    while(nsec < 0){
        nsec += 1000000000;
        --sec;
    }
    return (struct timespec){sec, nsec};
}

struct timespec timespec_add(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
}




void* A(void* args){
    #ifdef SINGLE_CPU
        set_cpu(1);
    #endif

    int port =*(int*)args;
    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);
    struct timespec period = {.tv_sec = 0, .tv_nsec = SLEEP_TIME};
    for(int i =0; i< 1000; ++i){
        if(io_read(port) == 0){
            io_write(port, 0);
            usleep(5);
            io_write(port,1);
        }

        #ifdef SLEEP
        waketime = timespec_add(waketime, period);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
        #endif
    }

    return NULL;
}

void* empty(void* args){
    #ifdef SINGLE_CPU
        set_cpu(1);
    #endif

    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);
    struct timespec period = {.tv_sec = 0, .tv_nsec = SLEEP_TIME};

    while(1){
    asm volatile("" ::: "memory");

    #ifdef SLEEP
    waketime = timespec_add(waketime, period);
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
    #endif
    }

}



int main() {

    io_init();

    set_cpu(1);

    int IDs[N_THREADS];
    pthread_attr_t* args[N_THREADS];
    for(int i =0; i< N_THREADS; ++i){
        IDs[i] =i;
        args[i] = (pthread_attr_t*)&IDs[i];
    }

	pthread_t threadHandle[N_THREADS];
	pthread_create(&threadHandle[0], NULL, A, args[0]);
	pthread_create(&threadHandle[1], NULL, A, args[1]);
 	pthread_create(&threadHandle[2], NULL, A, args[2]);

    for(int i = 3; i < N_THREADS; i++){
        pthread_create(&threadHandle[i], NULL, empty, args[i]);

    }


    for(int i = 0; i < 3; i++){ // Only wait for the active threads 
	    pthread_join(threadHandle[i], NULL);
    }




}
