/*
    Testing the Serial Connection
*/

#include<stdio.h>
#include "Lcd.h"
#include <delays.h>
#include <p18f46k22.h>
#include <stdlib.h>
#include "General.h"
#include "Serial.h"

#pragma config FOSC = INTIO67   // Internal OSC block, Port Function on RA6/7
#pragma config WDTEN = OFF      // Watch Dog Timer disabled. SWDTEN no effect
#pragma config XINST = OFF      // Instruction set Extension and indexed Addressing mode disabled

#define STANDARD 0
#define EXTENDED 1
#define SERIAL_MODE 2

unsigned char state;
void SysInit(void);

unsigned char isLeftBtnPressed(void);
unsigned char isRightBtnPressed(void);

void High_Priority_ISR(void);

#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh (void)
{
  _asm
    goto High_Priority_ISR 			// Jump to ISR Interrupt function.
  _endasm				// ISR name is unimportant but must match.
}

#pragma interrupt High_Priority_ISR  // "Interrupt" pragma for high priority
void High_Priority_ISR(void)		   // Interrupt function. Name is unimportant. 
{
    Serial_ISR();
}

void main(void)
{
  SysInit();
  LCDClear();
  LCDGoto(0, 0);
  LCDWriteStr("Hello World!");
  printf("Hello!\n");

  while(1)
  {
    // Set up variables
     unsigned int volt; //16 bits
     char input; //for serial
     char str[4];// for voltage output

      //Start A/D Conversion
     ADCON0bits.GO = 1;
     while (ADCON0bits.GO ==1){}; //GO bit automatically clears
     volt = ADRESH;
     volt=(volt<<8) | ADRESL; //Math needs to be done in the int variable
     if(volt==1023) //Fix roundoff error
        volt=1022;
     sprintf(str,"%04d",volt*49/10); //Approximate conversion to 0-5V

     LCDGoto(0,1);
     isLeftBtnPressed();
     isRightBtnPressed();
     Delay10KTCYx(10);
        //delay(100);

     switch (state%3){
            case STANDARD:
               LCDPutChar(str[0]);
               LCDPutChar('.');
                LCDPutChar(str[1]);
                LCDPutChar(str[2]);
                LCDPutChar(str[3]);
                LCDPutChar('V');
                break;
            case EXTENDED:
                LCDWriteStr("Extended");
                break;
	    case SERIAL_MODE:
	    	LCDWriteStr("Serial ");
            LCDPutChar(str[0]);
               LCDPutChar('.');
                LCDPutChar(str[1]);
                LCDPutChar(str[2]);
                LCDPutChar(str[3]);
                LCDPutChar('V');
            if(SERRxDatAvail()){
                input=SERRxGet();
                if(input==' '){
                    SERTxSave('\r');
                    SERTxSave('\n');
                    SERTxSave(str[0]);
                    SERTxSave('.');
                    SERTxSave(str[1]);
                    SERTxSave(str[2]);
                    SERTxSave(str[3]);
                    SERTxSave('V');
                }
            }
		break;
            default:
                //add error
                break;
    }
  };
}

void SysInit(void)
{
    //OSCCON=0b01010110; //4 MHz internal oscillator
                        // 16 MHz internal: 0b01110110;
    OSCCON = 0b01110110;

    //Set up buttons
    ANSELBbits.ANSB0=0; //Digital
    TRISAbits.RA4=1; //Input
    TRISBbits.RB0=1; //Input

    //Set up A/D on AN0
    ANSELAbits.ANSA0 = 1;
    TRISAbits.RA0 = 1; //Analog in
    ADCON2bits.ACQT=001; //2 TAD
    ADCON2bits.ADCS=100; //FOSC/32
    ADCON2bits.ADFM=1; //Left justified
    ADCON0bits.ADON=1; //Turn on A/D

    //Set up LCD
    ANSELD = 0x00;
    TRISD = 0x00; //Digital out

    LCDInit(); //Start LCD
    LCDGoto(0,1);
    LCDWriteStr("Init mode");

    //Initialize Serial port
    TRISCbits.RC6=1;
    TRISCbits.RC7=1;
    ANSELC=0x00;

    RCONbits.IPEN	= 1; //Priority levels
    INTCONbits.GIEH = 1; //Enable interrupts
    IPR1bits.RC1IP = 1; //High priority
    IPR1bits.TX1IP = 1;

    SERInit();			 // Initialized Serial Come
    SERTxSave('\r');		 // Carriage return
    SERTxSave('\n');		 // Line Feed

    SERTxSave('\r');
    SERTxSave('\n');
    SERSendStr("Voltage:");
    
    state = 0;
}

unsigned char isLeftBtnPressed(void){
  if (PORTAbits.RA4 == 0){
      Delay10KTCYx(10);
      state=0;
      LCDGoto(0,1);
      LCDWriteStr("                ");
      LCDGoto(0,1);
      return 1;
 }
 return 0;
}

unsigned char isRightBtnPressed(void){
  if (PORTBbits.RB0 == 0){
      Delay10KTCYx(10);
      state++;
      LCDGoto(0,1);
      LCDWriteStr("                ");
      LCDGoto(0,1);
      return 1;
 }
 return 0;
}
