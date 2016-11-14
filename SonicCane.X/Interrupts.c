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

#include "System.h"
#include "Lcd.h"
#include "Sensor.h"
//*****************************************************************************
//                            CONSTANT DEFINITION
//*****************************************************************************
#define TMRL_1ms 0x17;
#define TMRH_1ms 0xFC;

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
    Pulse_ISR(); //Call real-time clock service routine
}

void Pulse_ISR(void){
    //write some code
}