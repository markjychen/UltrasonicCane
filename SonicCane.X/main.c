#define USE_OR_MASKS
#include <p18cxxx.h>
#include "pwm.h"

//http://www.microchip.com/forums/m506808.aspx

#pragma config WDTEN=OFF
#pragma config FOSC=INTIO67
#pragma config XINST=OFF
void sendPWM(void);

void sendPWM(void){
        PR2 = 249;          // Timer2 period register = 250 counts //DC?
    
    CCPR1L = 0xBF;      // The 8 most sig bits of the period are 0x7D     
                        // DC% = CCPR1L = % * PR2
    CCP1CON = 0b01001100; // The 2 LSbs are 0b00, and CCP1Mz = 110 for PWM
}

void main(void)
{
    OSCCON=0b01010110; //set to 4 MHz (labA))   
    TRISDbits.TRISD7 = 0;  //set PWM pin RD7 output 
    T2CON = 0b00000111; // Prescale 1:16, timer on
    while (1){
        sendPWM();
    }
}