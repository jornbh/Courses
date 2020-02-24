#include "taskE.h"
#include "array.h"
#include "stdio.h"

void taskE(int argc, char** argv){
    int arrSize = 10;
    int itterations = 1000000;
    Array  arr = array_new(arrSize); 
    printf("Starting task E\n");

    for (int i =0; i< itterations; ++i){
        // printf("%i\n", i);
        array_insertBack(&arr, (long)i );

    }
    printf("Done with task E: arrSize = %i, itterations i %i \n", arrSize, itterations);


}