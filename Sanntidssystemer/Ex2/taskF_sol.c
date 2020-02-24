#include <stdio.h>
#include "pthread.h"
#include "semaphore.h"
#include "stdlib.h"

#define N_FORKS 5
#define N_PHIL 5
#define N_ITTERATIONS 10000
static pthread_mutex_t FORKS[N_FORKS];
static pthread_t PHILOSOPHERS[N_PHIL];
static pthread_mutex_t TABLE;
static int FORK_FREE[N_FORKS];
int FULLNESS[N_PHIL];
int GLOB_INT =0;
void initForks(){
    for(int i =0; i< N_FORKS; ++i){
        pthread_mutex_init(&FORKS[i], NULL);
        FORK_FREE[i] = 1; 
    }
}
void* philosopher(void* args){
    int ind = *(int*)args;
    int higher= (ind+1)%N_PHIL;
    int lower = (ind-1)%N_PHIL;
    if (lower == -1){ 
        lower = N_FORKS -1;}
    int taken = -1; 
    free(args);

    // work
    printf("START PHIL %i\n", ind);
    for(int i =0; i< N_ITTERATIONS; ++i){
        pthread_mutex_lock(&TABLE);
        // printf("%i: LOCK TABLE\n", ind);
        if( 1== FORK_FREE[higher] &&  1==FORK_FREE[lower]){
            pthread_mutex_lock(&FORKS[higher]);
            pthread_mutex_lock(&FORKS[lower]);
            FORK_FREE[higher] = FORK_FREE[lower] = 0;
            pthread_mutex_unlock(&TABLE);
        }
        else{
            if(FORK_FREE[higher] == 0){
                taken = higher; 
            }
            else{
                if(FORK_FREE[lower] == 0){
                taken = lower;
                }
                else {
                    printf("%i: ERROR; Both Forks are free: (%i:%i), (%i:%i)\n", i, higher, FORK_FREE[higher], lower, FORK_FREE[lower]);
                    taken = -123456789;
                    return NULL;
                }
                
            }
            // printf("%i: Fork %i is taken\n", ind, taken);
            pthread_mutex_unlock(&TABLE);
            
            // Wait for forks to be free
            pthread_mutex_lock(&FORKS[taken]);
            pthread_mutex_unlock(&FORKS[taken]);
            // printf("Done waiting for fork\n");

            --i; 
            continue;
        }
        ++ FULLNESS[ind];
        // ++GLOB_INT;
        // printf("%i: NOM (%i)\n", ind, GLOB_INT);

        pthread_mutex_lock(&TABLE);
        FORK_FREE[higher] = FORK_FREE[lower] = 1;
        pthread_mutex_unlock(&FORKS[higher]);
        pthread_mutex_unlock(&FORKS[lower]);
        pthread_mutex_unlock(&TABLE);


    }
    return NULL;
}

void startPhilosphers(){
    int* index;
    for (int i =0; i< N_PHIL; ++i){
        index = malloc(sizeof(int));
        *index = i; 
        pthread_create(&PHILOSOPHERS[i], NULL, philosopher, index);
        FULLNESS[i] =0;
    }
}

void joinPhilosophers(){
    for(int i =0; i< N_PHIL; ++i){
        pthread_join(PHILOSOPHERS[i], NULL);
    }
}
int main(){
    printf("START\n");
    pthread_mutex_init(&TABLE, NULL);
    initForks();
    startPhilosphers();

    joinPhilosophers();
    printf("Global int = %d\n", GLOB_INT);
    for(int i =0; i< N_PHIL; ++i){
        printf("%i: Fullness: %i\n", i, FULLNESS[i]);
    }
}