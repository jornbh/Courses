#include <stdio.h>
#include "pthread.h"
#include "semaphore.h"
#include "stdlib.h"
struct Singleton{
    int data;
};

static struct Singleton* g_singleton =NULL; 
static sem_t initSem;
struct Singleton* slow_getSingleton(){
    sem_wait(&initSem);
    if(!g_singleton){
        g_singleton = malloc(sizeof(struct Singleton));

    }

    sem_post(&initSem);
    return g_singleton;
}




struct Singleton* getSingleton(){
    if(!g_singleton){
        sem_wait(&initSem);
        if(g_singleton){
            g_singleton = malloc(sizeof(struct Singleton));
        }
        sem_post(&initSem);

    }
    return g_singleton;
}


int main(){
    sem_init(&initSem, 0, 1);
    printf("NOOOOOOOOOOOO\n");
}