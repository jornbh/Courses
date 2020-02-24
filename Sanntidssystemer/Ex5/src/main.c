#include "other.h"
#include "stdio.h"
#include "stdlib.h"
#include <pthread.h>
#include <sched.h>

void* A(void* args){
    printf("In thread\n");
    int* argPtr = args;

    int ID = *argPtr;

    for(int i =0; i< 3; ++i){
        printf(" HEllo from the thread %d\n", ID);
    }
}



int set_cpu(int cpu_number)
{
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_number, &cpu);
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

int main(){
    int ID = 1;
    int nThreads =200; 

    pthread_t* threadHandles= malloc(sizeof(pthread_t)*nThreads);

    pthread_attr_t* args =(pthread_attr_t*) malloc(sizeof(ID)*nThreads);


    for(int i =0; i< nThreads; ++i){
        *((int*)args +i) = i; 
        pthread_create(threadHandles+i, NULL, A, (pthread_attr_t*) ((int*)args+i));
    }


    other();
    for(int i =0; i< nThreads; ++i){
        pthread_join(threadHandles[i], NULL);
    }
    return 0; 
}