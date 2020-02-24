#include <stdio.h>
#include "pthread.h"
#include "semaphore.h"
long GLOBAL =0;



void* worker(void* arg){
    sem_t* p_sem = (sem_t*)arg;     
    long resource =0;
    int iterations = 15000000;
    for (int i =0; i< iterations; ++i){
        ++resource;
        sem_wait(p_sem);
        ++GLOBAL;
        sem_post(p_sem);
    }
    sem_wait(p_sem);
    printf("GLOBAL = %ld\n", GLOBAL);
    sem_post(p_sem);
    printf("resource = %ld\n", resource);

    return NULL;

}


int main(){
    printf("Start\n");
    pthread_t trd_A, trd_B;
    sem_t semaphore; 
    sem_init(&semaphore, 0, 1);
    pthread_create(&trd_A, NULL, worker , ((void*) &semaphore)); 
    pthread_create(&trd_B, NULL, worker , ((void*) &semaphore)); 

    pthread_join(trd_A, NULL);
    pthread_join(trd_B, NULL);

    printf("GLOBAL = %ld", GLOBAL);





  return 0;
}
