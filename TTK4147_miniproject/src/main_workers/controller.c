

#include "sharedVariables.h"
#include "stdio.h"

// To delete 




#include "../miniproject-files/network/udp.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <semaphore.h>
#include "timeDifference.h"
#include "globalHeader.h"



// Controller parameters
const double Kp = 5;
const double Ki = 1000;
const double Kd = 0.01;

// Declarations
double _calculateU(double dt, double error); 

void* controller_main(void* args){


	double u, error; 
	int i; 
	char send_buff[100];
	int buflen = 100;
	sprintf( send_buff, "SET:");





		
	while(1) {
    


	 double dt = timeDifference_getDt_s();
	 printf("Cont says; dt : %f\n",  dt); 
	 error = REF -Y; 
	 u = _calculateU( dt,  error);



	

	// Send controll output
	snprintf(send_buff+4, 100, "%lf",u); 
    	udpconn_send(CONN, send_buff);

    	printf("C: %s\n", send_buff);
	
    	timeDifference_wait_us((int)(CONTROLLER_SLEEP_PERIOD_S*S_TO_US)); 
    	udpconn_send(CONN, "GET");

    }
     

	return NULL;
} 


//Low-pass filters the signal for the derivative part
inline double limitedD(double dt, double error){
	static double x =0.1; 
	const double tau= 1;
	x = x + dt*( -x-error)/tau; 
	double y = error -x; 
	return y; 
}


double _calculateU(double dt, double error){
        static double I =0.0; 
        double P,D; 

	
	P  = error;		
	I += (error*dt);
	D  = limitedD(dt, error);
	


	double u = Kp*error + Ki*I + Kd*D;

        return u; 
}

