#include <stdio.h>
#include "pthread.h"
long GLOBAL =0;



void* worker(void* args){
  long resource =0;
  int iterations = 15000000;
  for (int i =0; i< iterations; ++i){
    ++resource;
    ++GLOBAL;
  }
  printf("GLOBAL = %ld\n", GLOBAL);
  printf("resource = %ld\n", resource);

  return NULL;

}


int main(){
  pthread_t trd_A, trd_B;
  pthread_create(&trd_A, NULL, worker , NULL); 
  pthread_create(&trd_B, NULL, worker , NULL); 
  
  pthread_join(trd_A, NULL);
  pthread_join(trd_B, NULL);

  printf("GLOBAL = %ld", GLOBAL);





  return 0;
}
