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

//#include <GenericTypeDefs.h>
#include <delays.h>
#include <p18f46k22.h>
#include <stdio.h>


#include "Lcd.h"
#include "General.h"


//*****************************************************************************
// Global Knobs
//*****************************************************************************

//*****************************************************************************
// Global Variables
//*****************************************************************************

//*****************************************************************************
// Global Constant Strings
//*****************************************************************************

//*****************************************************************************
// Global Variable Strings
//*****************************************************************************

//*****************************************************************************
// Global Function Prototypes
//*****************************************************************************

void SysInit(void);
void buttonInit(void);
int isLeftBtnPressed(void);
int isRightBtnPressed(void);
int isBtnPressed(void);
int potLvl(void);
void enableSleep(void);
void LCDDisplayInit(void);
void LEDInit(void);
void Tmr0Init(void);
void delayMillisecond(int num_ms);
#endif
