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

unsigned int analogReadTick;
unsigned int delayPulseTick; //questionable
unsigned int isPWMFireFlag;

extern unsigned int timeToFire;
extern unsigned int state;
extern unsigned int dataReadyFlag1;
extern unsigned int dataReadyFlag2;
extern unsigned int sendHeadFlag;

//PWM Controls
unsigned int PWMFireFlag;
unsigned int PWMTime;
unsigned int delayPWMFireTick;

//analog read
unsigned int dataReadyTick1;
unsigned int dataReadyTick2;
unsigned int sendPulseFlag = 0;

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
    btnISR();
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
    PWMFireFlag = 0;

    timeToFire = 1000;
}

void btnISRInit(void){
    
    OSCCONbits.IDLEN = 0;

    ANSELBbits.ANSB1 = 0;
    TRISBbits.RB1 = 0;
    
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT1IE = 1;
    INTCON2bits.INTEDG1 = 1;
    INTCON3bits.INT1F = 0;
    INTCON3bits.INT1E = 1;
    

    //INTCONbits.RBIE = 1; //enable interrupt
    INTCON2bits.RBPU = 0;
   //  INTCONbits.INT0F = 0;
    //INTCONbits.INT0E = 1;
    INTCONbits.GIE = 1;
    RCONbits.IPEN = 1;
    INTCONbits.PEIE = 1;
}

void btnISR(void){
    if (INTCON3bits.INT1IF){
        INTCON3bits.INT1IF = 0;
        state++;
    }
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
        analogReadTick++;
        
        dataReadyTick1++;
        dataReadyTick2++;
        
        //List of functions to fire:
        if (analogReadTick > 110){
            analogReadTick = 0;
            dataReadyTick1 = 0;
            dataReadyTick2 = 0;
            sendPulseFlag = 1;
        }
        if (sendPulseFlag == 1){
            LATBbits.LATB3 = 1;
            sendPulseFlag = 0;
        }else{
            LATBbits.LATB3 = 0;
        }
        if (dataReadyTick1 > 42){
            dataReadyFlag1 = 1;
        }
        if (dataReadyTick2 > 84){
            dataReadyFlag2 = 1;
        }
        if (sendHeadFlag){
            sendHeadWarning(0);
        }
        
        if (delayFireTick>timeToFire){//timeToFire){
            //sendPWM(200);
            //sendPWM(200);
            //delayPWMFireTick = 0;
            delayFireTick = 0;
            PWMFireFlag = 1;
            PWMTime = 0;
            sendPWM(200);
            //stopPWM();
            //LATBbits.LATB3 = ~LATBbits.LATB3;
            //sendPulse(1);
        }
        if (PWMFireFlag == 1){
            PWMTime++;
        }
        if (PWMTime >= 75){
            PWMFireFlag = 0;
            stopPWM();
            PWMTime = 0;
        }
        //if (PWMFireFlag==0){
        //    stopPWM();
        //}
        
    }
    
}