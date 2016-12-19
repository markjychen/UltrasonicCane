#include "Lcd.h"
#include "General.h"
#include "Serial.h"
#include <stdio.h>

#pragma config FOSC = INTIO67   // Internal OSC block, Port Function on RA6/7
#pragma config WDTEN = OFF      // Watch Dog Timer disabled. SWDTEN no effect
#pragma config XINST = OFF      // Instruction set Extension and indexed Addressing mode disabled

void High_Priority_ISR(void);

#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh (void)
{
  _asm
    goto High_Priority_ISR 			// Jump to ISR Interrupt funciton.
  _endasm				// ISR name is unimportant but must match.
}

#pragma interrupt High_Priority_ISR  // "Interrupt" pragma for high priority
void High_Priority_ISR(void)		   // Interrupt fuction. Name is unimportant. 
{
    Serial_ISR();
}

void main(void)
{
    unsigned int volt;
    char input;
    char str[4];
    
    OSCCON=0b01010110; //4 MHz internal oscillator

    //Initialize LCD
    ANSELD = 0x00;
    TRISD = 0x00; //Digital out
    LCDInit();

    //Set up A/D on AN0
    ANSELAbits.ANSA0 = 1;
    TRISAbits.RA0 = 1; //Analog in
    ADCON2bits.ACQT=001; //2 TAD
    ADCON2bits.ADCS=010; //FOSC/32
    ADCON2bits.ADFM=1; //Left justified
    ADCON0bits.ADON=1; //Turn on A/D

    //Initialize Serial port
    TRISCbits.RC6=1;
    TRISCbits.RC7=1;
    ANSELC=0x00;

    RCONbits.IPEN	= 1; //Priority levels
    INTCONbits.GIEH = 1; //Enable interrupts
    IPR1bits.RC1IP = 1; //High priority
    IPR1bits.TX1IP = 1;

    SERInit();			 // Initialized Serieal Come
    SERTxSave('\r');		 // Carriage return
    SERTxSave('\n');		 // Line Feed

    SERTxSave('\r');
    SERTxSave('\n');
    SERSendStr("Voltage:");
    LCDGoto(0,0);
    LCDWriteStr("Voltage:");
    while(1)
    {
        ADCON0bits.GO=1; //Start conversion
        while(ADCON0bits.GO==1){}; //Wait for finish
        volt=ADRESH;
        volt=(volt<<8) | ADRESL; //Math needs to be done in the int variable
        if(volt==1023) //Fix roundoff error
            volt=1022;
        LCDGoto(0,1); //Line 2
        sprintf(str,"%04d",volt*49/10); //Approximate conversion to 0-5V
        LCDPutChar(str[0]);
        LCDPutChar('.');
        LCDPutChar(str[1]);
        LCDPutChar(str[2]);
        LCDPutChar(str[3]);
        LCDPutChar('V');

        if(SERRxDatAvail())
        {
            input=SERRxGet();
            if(input==' ')
            {
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

        Delay10KTCYx(25); //Wait a little bit
    }

}

