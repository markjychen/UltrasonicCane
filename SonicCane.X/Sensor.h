/********************************************************************
*
*
*
*********************************************************************
* FileName:        Sensor.h
* Dependencies:    See INCLUDES section below
* Processor: 	   PIC18F46K22
* Compiler: 	   C-18
* Company:         Microchip Technology, Inc.

*********************************************************************
* File Description: This file contains service prototype definition
*                   for LV-MaxSonar-EZ access.
*
* Change History:
* Author               Date        Comment
* Mark Chen            10/30/16  First version of module code
********************************************************************/

/**
*   @file Sensor.h
*   @brief This file contains the function prototypes for the LV-MaxSonar-EZ.
*
*/

#ifndef _Sensor_H
#define _Sensor_H

//*****************************************************************************
// Include and Header files
//*****************************************************************************

#include <p18cxxx.h>
//#include <GenericTypeDefs.h>

//#include "General.h"


//*****************************************************************************
// Global Knobs
//*****************************************************************************

//*****************************************************************************
// Global Definitions and Equates
//*****************************************************************************

//*****************************************************************************
// Global Variables
//*****************************************************************************

struct Sensors{
    unsigned int    pin;
    unsigned int    sensorID;
} ;

//*****************************************************************************
// Global Constant Strings
//*****************************************************************************

//*****************************************************************************
// Global Variable Strings
//*****************************************************************************

//*****************************************************************************
// Global Function Prototypes
//*****************************************************************************

void analogInit(void);
void motorInit(void);
void motorInit2(void);

void pulseInit(void);
int analogRead(unsigned char);           // Analog Read from pin (argument)
void sendPulse (int);           // Send number of pulses*/
void sendPWM (int);             // Send duty cycle
void stopPWM (void);
void sendPWM2 (int);             // Send duty cycle
void stopPWM2 (void);
void headMotorInit(void);
void sendHeadWarning(int);

#endif
