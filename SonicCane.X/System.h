/********************************************************************
*
*
*
*********************************************************************
* FileName:        System.h
* Dependencies:    See INCLUDES section below
* Processor: 	   PIC18F46K22
* Compiler: 	   C-18
* Company:         Microchip Technology, Inc.

*********************************************************************
* File Description: This file contains service prototype definition
*                   for the basic PIC functions.
*
* Change History:
* Author               Date        Comment
* Mark Chen            10/30/16  First version of module code
********************************************************************/

/**
*   @file System.h
*   @brief This file contains the function prototypes for the SonicCane PIC.
*
*/

//*****************************************************************************
// Include and Header files
//*****************************************************************************

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <p18cxxx.h>
#include <GenericTypeDefs.h>
#include <delays.h>
#include <p18f46k22.h>
#include <stdio.h>

#include "Lcd.h"
#include "General.h"


//*****************************************************************************
// Global Knobs
//*****************************************************************************

//*****************************************************************************
// Global Definitions and Equates
//*****************************************************************************
#pragma config WDTEN = SWON         //Turn ON WDTmr
#pragma config WDTPS = 1024         //WDT timer (512 = 2 sec)
#pragma config FOSC = INTIO67       //Connect to internal oscillator
#pragma config XINST = OFF

//*****************************************************************************
// Global Variables
//*****************************************************************************

int state;

//*****************************************************************************
// Global Constant Strings
//*****************************************************************************

#define STANDARD 0
#define PWM_DEMO 1
#define PULSE 2
#define ADC 3
#define SLEEP 4
#define NO_OF_STATES 5
#define TMRL 0x58
#define TMRH 0x9E

//*****************************************************************************
// Global Variable Strings
//*****************************************************************************

//*****************************************************************************
// Global Function Prototypes
//*****************************************************************************

void SysInit(void);
unsigned char isLeftBtnPressed(void);
unsigned char isRightBtnPressed(void);
unsigned char isBtnPressed(void);
int potLvl(void);
void enableSleep(void);


#endif
