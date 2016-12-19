/********************************************************************
*
*                
*
*********************************************************************
* FileName:        Sound.c
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
* File Description: This module contains the functions used to generate a tone
*			from the on board 8 ohm speaker. Within these functions is 
*			also the ability to modify the Pulse Width Modulation period
*			which will change the pitch of the tone.
*
* Change History:
* Author               Date        Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Chris Tucker      10/18/2010  First version of module code
********************************************************************/

/**
*   @file Sound.c
*   @brief 	This module contains the functions used to generate a tone
*			from the on board 8 ohm speaker. Within these functions is 
*			also the ability to modify the Pulse Width Modulation period
*			which will change the pitch of the tone.
*   @defgroup Sound		Buzzer Sound
*   
*/

#include <p18cxxx.h>
#include <GenericTypeDefs.h>

#include "General.h"

//CCP1Con
#define CCP1_CONFIG			0b00001100;
///< Single PWM output; LSB of PWM duty cycle equal 0; ECCP1/2/3 PWM mode.

//CPP1TMRS0
#define CCP1_TIMER			0b00000001;
///< CCP4/3/2 (all not used) are based off Timer 2; CCP1 is  based of Timer 4 in PWM mode.


//T4CON
#define TIMER4_CONFIG		0b00000010;
///< 1:1 Postscaler; Timer 4 Disabled; Prescaler is 64 [Gives a full Audio Range]

UINT8_T	PWM_ON	=	0x05;
///< Duty Cycle of the PWM

///////////////////////////////////////////////////////////////////////////////
//*****************************************************************************
// SUPPORT FUNCTIONS
//*****************************************************************************
///////////////////////////////////////////////////////////////////////////////

/********************************************************************
* Function:         SNDInit
* 
* PreCondition: 	None
*
* Side Effects: 	None
*/
/**
* @ingroup  		Sound
*
* @brief    		This routine prepares the CCPR1 module for PWM operation. It
*                   is used to drive the buzzer on the PicDem 2 Plus board.
*
* @note    			This function should be called only once at start up.
*/
/*******************************************************************/

void SNDInit(void)
{
  TRISCbits.TRISC2 = SET;  			// Set the buzzer pin
  CCP1CON = CCP1_CONFIG	;     		// Configure CCP1 module for PWM operation
  CCP1CONbits.DC1B1 = CLEAR;   		// 10 Bits resolution => 2 LSB of duty cycle
  CCP1CONbits.DC1B0 = CLEAR;   		// 10 Bits resolution => 2 LSB of duty cycle
  CCPTMRS0 = CCP1_TIMER;       		// CCP1 is based off timer 4 for PWM operation
  PIR5bits.TMR4IF = CLEAR;     		// Clear Timer 4 Interrupt Flag
  T4CON = TIMER4_CONFIG;          	// Prescaler = 4
  T4CONbits.TMR4ON = ENABLE;       	// Turn Timer 4 is ENABLED
  while(!PIR5bits.TMR4IF);      	// Wait until timer 4 overflows   
}

/********************************************************************
* Function:         SNDSound
* 
* PreCondition: 	None
*
* Side Effects: 	None
*/
/**
* @ingroup  		Sound
*
* @brief    		This function allows the application to turn the buzzer on
*                   or off, according to the command set by the caller.
* 
* @param			OnOff 			Turns the buzzer on if TRUE,otherwise shuts it off
*
* @param			PWM_Period 		Adjust the Period of the PWM effecting pitch/tone.
*
* @note    			None.
*/
/*******************************************************************/

void SNDSound(UINT8_T OnOff, UINT8_T PWM_Period)
{
	PR4 = PWM_Period;							// Load PR4 (Timer 4) with PWM period value  

  	CCPR1L  = PWM_ON; 	                                 // Load CCPR1L with PWM duty cycle
	
    if (OnOff) 
  	  TRISCbits.TRISC2 = DISABLE;
    else
  	  TRISCbits.TRISC2 = ENABLE;
}
