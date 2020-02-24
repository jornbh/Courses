#include <stdio.h>
#include <unistd.h>
#include "pthread.h"
#include "semaphore.h"
#include "stdlib.h"

#define N_FORKS 5
#define N_PHIL 5
#define N_ITTERATIONS 10000
static pthread_mutex_t FORKS[N_FORKS];
static pthread_t PHILOSOPHERS[N_PHIL];
int GLOB_INT =0;
void initForks(){
    for(int i =0; i< N_FORKS; ++i){
        pthread_mutex_init(&FORKS[i], NULL);
    }
}
void* philosopher(void* args){
    int ind = *(int*)args;
    int higher= ind+1%N_PHIL;
    int lower = (ind-1)%N_PHIL; 
    free(args);

    // work

    while(1){
        printf("%i:", ind);
        pthread_mutex_lock(&FORKS[higher]);
        pthread_mutex_lock(&FORKS[lower]);
        ++GLOB_INT;
        printf(" NOM\n");
        pthread_mutex_unlock(&FORKS[higher]);
        pthread_mutex_unlock(&FORKS[lower]);
        sleep(1);
    }


    return NULL;
}

void startPhilosphers(){
    int* index;
    for (int i =0; i< N_PHIL; ++i){
        index = malloc(sizeof(int));
        *index = i; 
        pthread_create(&PHILOSOPHERS[i], NULL, philosopher, index);
    }
}

void joinPhilosophers(){
    for(int i =0; i< N_PHIL; ++i){
        pthread_join(PHILOSOPHERS[i], NULL);
    }
}
int main(){
    printf("START\n");
    initForks();
    startPhilosphers();
    // pthread_mutex_t mtx;

    // pthread_mutex_init(&mtx, NULL);
    joinPhilosophers();
    printf("Global int = %d\n", GLOB_INT);
}