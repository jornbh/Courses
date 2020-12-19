#include "avr/interrupt.h"
#include "avr/io.h"
#include "stdio.h"
#include "../main.h"

#ifdef INTERRUPTS /* To avoid uneccecary headerfiles in the project */
#include "./interrupts.h"
#include "avr/interrupt.h"
#endif // INTERRUPTS



#define TIMER_VALUE 
void _setTimerInterruptMode();
void _setPrescaler();
void _setTimerValue();
void _enableTimerInterrupts();


void interrupts_timer_init(){



    #ifdef TIMER_INTERRUPTS
    _setTimerInterruptMode();
    _setPrescaler();
    _setTimerValue();
    _enableTimerInterrupts();
    // COMPLETE ??? 
    printf("Timer-interrupts enabled!\n");

    #else
    printf_char(" Timer-interrupts are not enabled\n");
    #endif //TIMER_INTERRUPTS






}


#ifdef TIMER_INTERRUPTS
void _setTimerInterruptMode(){
        // Set to normal timer-mode instead of PWM-Mode 
    // No timer-clear is done in normal-mode, it simply overruns. 
    // Clear timer on Compare-match mode is acheived by setting the value of the bits to 4 (or 12)
    TCCR3A &= ~( ( 1 << WGM30)|( 1 << WGM31) | ( 1 << WGM32)| ( 1 << WGM33));

    // The COM 0 and 1-bits must also be set 
        // set to 0b11 to set OCnx on compare-match ( Prefered )
        //  to 0 for to disconnect OCnx
    TCCR3A |= (1<< COM3A0) | (1 << COM3A1);

}

void _setPrescaler(){
    // Make sure the prescaler is set to a value fitting all devices using a clock
    TCCR3B = TCCR3B & ( 0b0111 & PRESCALER_VALUE ); // p.157 in arduino datasheet (CSn2:0)
	// 1=> 1, 2=> 8, 3=> 64, 4=> 256, 5=> 1024

	return; 
    // If the OCIE-bit is set, an interrupt happens on match 


}


void _setTimerValue(int timestep){


        //________________________________________________________________________
    // The counter is constantly compared with OCRnx (16- bit for some timers, 8 for others)
    // 16: 1(A,B,C) , 3(A,B,C) 4(A,B,C) , 5(A,B,C)
    // 8: THE REST (2 ? )
    int temp= timestep * F_CPU / PRESCALER; 
    OCR3A = temp; 
     //OCRnxH = (char)(0xFF&temp); // Both H and L- registers can usually be set by OCRnx= temp
     //OCRnxL = (char)(0xFF &(temp>>8));


    //________________________________________________________
}



void _enableTimerInterrupts(){

    SET_BIT(TIMSK3, ICIE3); 
    // • Bit 5 – ICIEn: Timer/Countern, Input Capture Interrupt Enable
        // When this bit is written to one, and the I-flag in the Status Register is set (interrupts globally enabled), the
        // Timer/Countern Input Capture interrupt is enabled

    SET_BIT(TIMSK3, OCIE3A); // X = A,B,C ( pin [3:1])
    // Enables interrupts on compare-match on compare-match A,B or C 
    CLEAR_BIT(TIMSK3, TOIE3); // Bit 0
    // Clearing the bit disables interrupts when the timer overflows
}



// void interrupts_timer_init(){



//     #ifdef TIMER_INTERRUPTS
//     _setTimerInterruptMode();
//     _setPrescaler();
//     _setTimerValue();
//     _enableTimerInterrupts();
//     // COMPLETE ??? 


//     #else
//     printf_char(" Timer-interrupts are not enabled\n");
//     #endif //TIMER_INTERRUPTS






// }


// #ifdef TIMER_INTERRUPTS
// void _setTimerInterruptMode(){
//         // Set to normal timer-mode instead of PWM-Mode 
//     // No timer-clear is done in normal-mode, it simply overruns. 
//     // Clear timer on Compare-match mode is acheived by setting the value of the bits to 4 (or 12)
//     TCCRnA &= ~( ( 1 << WGMn0)|( 1 << WGMn1) | ( 1 << WGMn2)| ( 1 << WGMn3));

//     // The COM 1 and 2-bits must also be set 
//         // set to 0b11 to set OCnx on compare-match ( Prefered )
//         //  to 0 for to disconnect OCnx
//     TCCRnA |= (1<< COMn1) | (1 << COMn2);

// }

// void _setPrescaler(){
//     // Make sure the prescaler is set to a value fitting all devices using a clock
//     TCCRnB = TCCRnB & ( 0b0111 & PRESCALER_VALUE ); // p.157 in arduino datasheet (CSn2:0)



//     // Set prescaler
    




//     // If the OCIE-bit is set, an interrupt happens on match 


// }


// void _setTimerValue(int timestep){


//         //________________________________________________________________________
//     // The counter is constantly compared with OCRnx (16- bit for some timers, 8 for others)
//     // 16: 1(A,B,C) , 3(A,B,C) 4(A,B,C) , 5(A,B,C)
//     // 8: THE REST (2 ? )
//     int temp= timestep * F_CPU / PRESCALER; 
//     OVRnx = temp; 
//     // OCRnxH = (char)(0xFF&temp); // Both H and L- registers can usually be set by OCRnx= temp
//     // OCRnxL = (char)(0xFF &(temp>>8));


//     //________________________________________________________
// }
// void _enableTimerInterrupts(){

//     SET_BIT(TMSKn, ICIEn); 
//     // • Bit 5 – ICIEn: Timer/Countern, Input Capture Interrupt Enable
//         // When this bit is written to one, and the I-flag in the Status Register is set (interrupts globally enabled), the
//         // Timer/Countern Input Capture interrupt is enabled

//     SET_BIT(TMSKn, OCIEnX); // X = A,B,C ( pin [3:1])
//     // Enables interrupts on compare-match on compare-match A,B or C 
//     CLEAR_BIT(TMSKn, TOIEn); // Bit 0
//     // Clearing the bit disables interrupts when the timer overflows
// }


#endif 
ISR(TIMER2_COMPA_vect){
    // The interrupt-flag is automatically cleared when an interrupt is executed

    printf( " A timer-interrupt has occured\n ");

}