/********************************************************************
*
*                
*
*********************************************************************
* FileName:        Serial.c
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
* File Description: This modules includes all service functions for Serial
*                   RS232 operations.
*
* Change History:
* Author               Date        Comment
* Chris Tucker      10/18/2010  First version of module code
********************************************************************/

/**
*   @file Serial.c
*   @brief 	This modules includes all service functions for Serial
*           RS232 operations.
*   @defgroup Serial	RS232 Connector (Serial)
*   
*/
#include <p18cxxx.h>
#include <GenericTypeDefs.h>

#include "General.h"
#include "Serial.h"

#define MAX_OUT_BUF_SZ 60				///< Output buffer size
#define MAX_IN_BUF_SZ  8				///< Input buffer size

#define BAUD2400_16MHZ 103				///< UART setting for 2400 baud at 16 MHz Fosc
#define BAUD2400_4MHZ  25				///< UART setting for 2400 baud at 4 MHz Fosc

UINT8_T outBuf[MAX_OUT_BUF_SZ],      	///< Serial transmit buffer
      	txHead,                       	///< Read & write indexes for the output buffer
      	txTail,						
      	inBuf[MAX_IN_BUF_SZ],         	///< Serial receive buffer
      	rxHead,                       	///< Read & Write Indexes for the input buffer
      	rxTail;

///////////////////////////////////////////////////////////////////////////////
//*****************************************************************************
// INTRRUPT
//*****************************************************************************
///////////////////////////////////////////////////////////////////////////////


/********************************************************************
* Function:         Serial_ISR
* 
* PreCondition: 	None
*
* Side Effects: 	None                                            
*/
/**
* @brief	        This function is called within the Interrupt Service
*					routine in main.c. All Serial related Interrupts are 
*					stored here and called from the main ISR.
*
* @note        		THIS ROUTINE IS ONLY CALLED BY THE HARDWARE. ITS
*                   EXECUTION SHOULD REMAIN AS QUICK AS POSSIBLE.
*/
/*******************************************************************/

void Serial_ISR (void)
{
/******************************* SERIAL RECEPTION ***************************/

  if ((PIR1bits.RCIF) && (PIE1bits.RCIE))   // If a new character has been received
  {
    SERRxSave(RCREG1);                 		// Go store the new character in Rx buffer
    PIR1bits.RCIF = CLEAR;                  // Receive event has been serviced
  }   										// If new character has been received                                  

/***************************** SERIAL TRANSMISSION ****************************/

  if((PIR1bits.TXIF) && (PIE1bits.TXIE))    // If end of transmit
  {
    if (SERTxDatAvail())             // If data available for output
	{
      	SERSendNext();               // Go send next available character
	}
  } 
}

///////////////////////////////////////////////////////////////////////////////
//*****************************************************************************
// SUPPORT FUNCTIONS
//*****************************************************************************
///////////////////////////////////////////////////////////////////////////////

/********************************************************************
* Function:         SERInit
* 
* PreCondition: 	None
*
* Side Effects: 	None
*/
/**
* @ingroup  		Serial
*
* @brief    		This function is called to initialize UART operations. It
*                   configures the USART for the right baudrate, communication
*                   parameters and comm-related interrupts.
* 
* @param			Str 		String to be output to serial comm.
*
* @result			{TRUE/FALSE}	If there is still data to be sent in the output buffer,
*                   				otherwise returns FALSE.
*
* @note    			This routine should be called only once at start up.
*/
/*******************************************************************/

void SERInit(void)
{
// Transmit non-inverted data to the RB7/TX/CK; 8 bit BRG used; 
// Receiver operates normal; Aut-Baud Detect is disabled

  SPBRGH1 = CLEAR;

  /********************
  * SPEN = Enable Serial Port RX9 = 8-bit SREN CREN ADDEN FERR OERR RX9D */
  // Set up the UART

  SPBRG1   = BAUD2400_4MHZ;
  TXSTA1bits.SYNC = CLEAR;                    // Transmisson is asynchronous
  RCSTA1bits.SPEN = ENABLE;                   // Enable serial port
  TXSTA1bits.TXEN = ENABLE;                   // Enable EUSART Transmit Operation
  RCSTA1bits.CREN = ENABLE;                   // Enable EUSART Receive Operation

  PIE1bits.RCIE = ENABLE;                     // Enable USART Receive Interrupt
  PIE1bits.TX1IE = ENABLE;                    // Disable USART Transmit Interrupt
  PIR1bits.RC1IF = CLEAR;                     // Clear USART Receive Interrupt Flag
  PIR1bits.TX1IF = CLEAR;                     // Clear USART Transmit Interrupt Flag

  rxHead = rxTail = txHead = txTail = CLEAR;  // Initialize all Read & Write Indexes
}

/********************************************************************
* Function:         IncNdx
* 
* PreCondition: 	None
*
* Side Effects: 	None
*/
/**
* @ingroup  		Serial
*
* @brief    		This routine is called to increment the specified Index
*                   check if end of buffer is reached, then rolls the pointer
*                   over to the beginning, and returns the new value.
* 
* @param   p 	    Index value to be incremented.
*
* @note    	    This function should only be used locally in the Serial module.
*/
/*******************************************************************/

UINT8_T IncNdx(UINT8_T Ndx,UINT8_T Max)
{
  if (++Ndx == Max)                                  // If end of buffer reached
  {
    Ndx = 0;                         	            // Start over from beginning
  }
  return(Ndx);
}

/********************************************************************
* Function:         SERRxSave
* 
* PreCondition: 	None
*
* Side Effects: 	None
*/
/**
* @ingroup  		Serial
*
* @brief    		This routine is called when a new char is received on the
*                   serial communication, to store it into the input buffer.
* 
* @param			datin	Input data to be stored in the input buffer.
*
* @note    			Handles circular and full buffer conditions.
*/
/*******************************************************************/

void SERRxSave(UINT8_T datin)
{
  inBuf[rxHead] = datin;         // Fetch the recieved byte value into Rx buffer
  rxHead = IncNdx(rxHead,MAX_IN_BUF_SZ);      // Prepare next write location
  if (rxHead == rxTail)                      // If writing to next read location
  {
    rxTail = IncNdx(rxTail,MAX_IN_BUF_SZ);                      // Write farther
  }
}	

/********************************************************************
* Function:         SERRxDatAvail
* 
* PreCondition: 	None
*
* Side Effects: 	None
*/
/**
* @ingroup  		Serial
*
* @brief    		This service when called, lets the application know if there
*                   is still received data available for the application to read.	
*
* @result			{TRUE/FALSE} If data has been received and not yet read, otherwi
*                   returns FALSE.
*
* @note    			Handles circular and full buffer conditions.
*/
/*******************************************************************/

BOOL SERRxDatAvail(void)
{
  return(rxHead != rxTail);
}


/********************************************************************
* Function:         SERRxGet
* 
* PreCondition: 	None
*
* Input:        	None.
*
* Output:     		Oldest character in the receive buffer.
*
* Side Effects: 	None
*/
/**
* @ingroup  		Serial
*
* @brief    		This function retrieves a received value from the Rx buffer.
*
* @note    			None
*/
/*******************************************************************/

UINT8_T SERRxGet(void)
{
  UINT8_T c = inBuf[rxTail];		      // Read next character from buffer
  rxTail = IncNdx(rxTail,MAX_IN_BUF_SZ);       // Prepare to read next character
  return(c);						// Return received char
}	

/********************************************************************
* Function:         SERTxSave
* 
* PreCondition: 	None
*
* Side Effects: 	None
*/
/**
* @ingroup  		Serial
*
* @brief    		This function stores a char to be transmitted value 
*                   into the Tx buffer.
* 
* @param			value	Character to be sent.
*
* @result			{txTail}	Oldest character in the receive buffer.
* @note    			None
*/
/*******************************************************************/

void SERTxSave(UINT8_T value)
{
  outBuf[txHead] = value;                 // Store char to send in output buffer
  txHead = IncNdx(txHead,MAX_OUT_BUF_SZ);         // Prepare next write location
  if (txHead == txTail)                      // If writing to next read location
  {
    txTail = IncNdx(txTail,MAX_OUT_BUF_SZ);                     // Write farther
  }
  PIE1bits.TXIE = ENABLE;                    	                      // Enable transmit
}	

/********************************************************************
* Function:         SERTxDatAvail
* 
* PreCondition: 	None
*
* Side Effects: 	None
*/
/**
* @ingroup  		Serial
*
* @brief    		This service when called, lets the application know if there
*                   is still data available for transmission (output buffer).
* 
* @param			value 	Character to be sent.
*
* @result			{TRUE/FALSE} 	If there is still data to be sent in the output buffer,
*                   				otherwise returns FALSE.
*
* @note    			None
*/
/*******************************************************************/

BOOL SERTxDatAvail(void)
{
  if (txHead != txTail)                                   // If Buffer not empty yet
    PIE1bits.TXIE = ENABLE;                                            // Enable transmission
  else                       // Otherwise buffer is empty => no need to send anymore
    PIE1bits.TXIE = DISABLE;                                          // Disable transmission
  return(PIE1bits.TXIE);                     	 // Let caller know if still data to be sent
}

/********************************************************************
* Function:         SERSendNext
* 
* PreCondition: 	None
*
* Side Effects: 	None
*/
/**
* @ingroup  		Serial
*
* @brief    		This function is called to output next character (from buffer),
*                   to the serial communication line.
* 
* @param			value		 Character to be sent.
*
* @result			{TRUE/FALSE} If there is still data to be sent in the output buffer,
*                   			 otherwise returns FALSE.
*
* @note    			This routine should be called by the system (not meant to be
*                   called by the application).
*/
/*******************************************************************/

void SERSendNext(void)
{
  TXREG = outBuf[txTail];                     // Output next available character
  txTail = IncNdx(txTail,MAX_OUT_BUF_SZ);  // Select next char to be transmitted
}

/********************************************************************
* Function:         SERSendStr
* 
* PreCondition: 	None
*
* Side Effects: 	None
*/
/**
* @ingroup  		Serial
*
* @brief    		This service routine allows the application to output 
*                   a given string to the serial communication line.
* 
* @param			Str 		 String to be output to serial comm.
*
* @result			{TRUE/FALSE} 	If there is still data to be sent in the output buffer,
*                   			 	otherwise returns FALSE.
*
* @note    			None.
*/
/*******************************************************************/

void SERSendStr(const rom far char  *Str)
{
  UINT8_T i = 0;                              // Loop count

  	while(Str[i])                           // While still a char in the string
	{
    	SERTxSave(Str[i++]);                // Go Set char for output
	}
}
