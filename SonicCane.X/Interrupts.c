/********************************************************************
*
*
*
*********************************************************************
* FileName:        Interrupts.c
* Dependencies:    See INCLUDES section below
* Processor: 	   PIC18F46K22
* Compiler: 	   C-18
* Company:         Microchip Technology, Inc.

*********************************************************************
* File Description: This modules includes all service functions for LCD
*                   operations.
*
* Change History:
* Author               Date        Comment
* Mark Chen      11/14/2016  First version of module code
********************************************************************/

/**
*   @file Interrupts.c
*   @brief This modules includes all service functions for interrupt
*          operations.
*
*/

//*****************************************************************************
// Include and Header files
//*****************************************************************************


#include <p18f46k22.h>
#include "Interrupts.h"
//#include <stdio.h>

//Define statements
#define One_ms_h 0xF2;
#define One_ms_l 0xE3;

//Variable definitions
unsigned int delayFireTick;
extern unsigned int timeToFire;

//Function definitions
//High priority interrupt
#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh (void)
{
  _asm
    goto High_Priority_ISR
  _endasm
}

#pragma interrupt High_Priority_ISR
void High_Priority_ISR(void) 
{
    Sys_Tick_ISR(); //Call real-time clock service routine
}

//Initialize necessary systems
void ISRInit(void)
{
    OSCCON=0b01010110; //4 MHz internal oscillator

    //Set up buttons
    ANSELBbits.ANSB5 = 0;
    TRISBbits.RB5 = 0;
    
    TMR1L = One_ms_l;
    TMR1H = One_ms_h;

    T1CON = 0b01000011;
    T1GCON = 0;                 // Timer 1 Gate function disabled
    RCONbits.IPEN=1;            // Allow interrupt priorities
    PIR1bits.TMR1IF = 0;        // Clear any pending Timer 1 Interrupt indication
    PIE1bits.TMR1IE = 1;        // Enable Timer 1 Interrupt
    INTCONbits.GIE=1;           // Enable interrupts

}

//Called every millisecond by the interrupt
//int timeToFire;

void Sys_Tick_ISR (void)
{
    if (PIR1bits.TMR1IF)            // If timer overflowed
    {
        TMR1H = One_ms_h;          // Reset timer to one millisecond
        TMR1L = One_ms_l;
        //RTCIncSec();                // Increment count
        LATBbits.LATB5 = ~LATBbits.LATB5;
        PIR1bits.TMR1IF = 0;        // Clear timer flag
        INTCONbits.INT0IF = 0;      // Clear interrupt flag
        
        // List of counters:
        delayFireTick++;
        /*ticks++;
        ticks2++;
        */
        
        //List of functions to fire:
        if (delayFireTick>timeToFire){
            //sendPWM(200);
            //sendPulse(1);
            sendPWM(240);
            delayMillisecond(1);
            stopPWM();
            delayFireTick = 0;
            LATBbits.LATB4 = ~LATBbits.LATB4;
        }
    }
    
}