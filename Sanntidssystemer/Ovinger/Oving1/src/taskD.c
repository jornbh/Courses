#include "stdio.h"
#include "stdlib.h"
void taskD(int argc, char** argv){
    long xy_size    = 1000*1000*600;//1000*1000*1000;       // 8 GB (sizeof(long) = 8 bytes) 

    if (xy_size > 1000*1000*600){
        printf("Memory is too large (HDD will halt the PC)\n");
        return;
    }
    long x_dim      = 100; 
    long y_dim      = xy_size/x_dim;    
    long** matrix   = malloc(y_dim*sizeof(long*)); 
    for(long y = 0; y < y_dim; y++){ 
        matrix[y] = malloc(x_dim*sizeof(long)); 
    } 
    printf("Allocation complete (press any key to continue...)\n"); 
    getchar();

    for(long y = 0; y < y_dim; y++){ 
        free(matrix[y]); 
    } 
    free(matrix);

}