#include "taskA.h"
#include "taskB.h"
#include "taskC.h"
#include "taskD.h"
#include "taskE.h"





int main(int argc, char *argv[]){
    if(argc <= 1){
        printf("No arguments given\n");
        return 1;
    }

    switch (argv[1][0]){

        case 'A': 
            taskA(argc, argv);
            break;
        case 'B': 
            taskB(argc, argv);
            break;
        case 'C': 
            taskC(argc, argv);
            break;
        case 'D': 
            taskD(argc, argv);
            break;    
        case 'E': 
            taskE(argc, argv);
            break;   
        default: 
            printf("Invalid argument\n")                     ;


    }



}