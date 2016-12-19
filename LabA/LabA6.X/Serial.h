/********************************************************************
*
*                
*
*********************************************************************
* FileName:        Serial.h
* Dependencies:    See INCLUDES section below
* Processor: 	   PIC18F46K22      
* Compiler: 	   C-18       
* Company:         Microchip Technology, Inc.
*
* Software License Agreement:
*
* The software supplied herewith by Microchip Technology Incorporated
* (the "Company") for its PICmicro® Microcontroller is intended and
* supplied to you, the Company's customer, for use solely and
* exclusively on Microchip PICmicro Microcontroller products. The
* software is owned by the Company and/or its supplier, and is
* protected under applicable copyright laws. All rights are reserved.
* Any use in violation of the foregoing restrictions may subject the
* user to criminal sanctions under applicable laws, as well as to
* civil liability for the breach of the terms and conditions of this
* license.
*
* THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
*********************************************************************
* File Description:
*
* Change History:
* Author               Date        Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Chris Tucker      10/18/2010  First version of module code
********************************************************************/

/**
*   @file Serial.h
*   @brief 	This file contains the function prototypes for the Serial
*			functions used within the program.
*   
*/

#ifndef _SERIAL_H
#define _SERIAL_H

#include "General.h"

#define SERIAL_COM                  		///< Enables displayed data to be sent

void SERRxSave(UINT8_T datin);				///< Stores the specified character in the input buffer
void SERSendNext(void);  					///< Sends the next available char out on the serial line
void SERSendStr(const rom far char  *Str);  ///< Outputs a string on the serial line
void SERTxSave(UINT8_T value);         		///< Outputs the char to the outgoing buffer
void SERInit(void);							///< Initialize the serial peripherals
BOOL SERRxDatAvail(void);           		///< Checks if incoming data is available 
UINT8_T SERRxGet(void);                		///< Returns the next available char received
BOOL SERTxDatAvail(void);           		///< Checks if outgoing data is available
void Serial_ISR(void);						///< When Serial Interrupt is detected

#endif