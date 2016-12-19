/*
 * BME 464 - Lab A - Station 2
 *
 * Use CCP to count a "heartbeat"
 * Nick Bottenus - 8/23/12
 */

#include <p18f46k22.h>
#include "Lcd.h"
#include <stdio.h>

#pragma config WDTEN=OFF
#pragma config FOSC=INTIO67
#pragma config XINST=OFF

void LCDPutStr(char *Str);

void main(void)
{
    unsigned int newTime;
    unsigned int oldTime;
    unsigned int diff;
    char str[8];

    OSCCON=0b01010110; //4 MHz internal oscillator

    //Set up LCD
    ANSELD = 0x00;
    TRISD = 0x00; //Digital out

    //Set up Timer1
    T1CONbits.T1CKPS=0b11; //Prescaler 8:1
    T1CONbits.TMR1ON=1; //Turn timer on

    //Set up ECCP1
    CCP1CONbits.CCP1M=0b0101; //Capture rising edge
    CCPTMRS0bits.C1TSEL=0b00; //Use Timer1
    TRISCbits.RC2=1; //Input
    ANSELCbits.ANSC2=0; //Digital

    LCDInit(); //Start LCD

    PIR1bits.CCP1IF=0; //Clear flag

    while(1)
    {
        while(PIR1bits.CCP1IF==0){}; //Wait for flag
        PIR1bits.CCP1IF=0; //Reset flag
        oldTime=CCPR1H;
        oldTime=(oldTime<<8)|CCPR1L; //Save Timer1 value

        while(PIR1bits.CCP1IF==0){}; //Wait for flag
        PIR1bits.CCP1IF=0; //Reset flag
        newTime=CCPR1H;
        newTime=(newTime<<8)|CCPR1L; //Save Timer1 value

        diff=newTime-oldTime;
        diff=62500/(diff/2);
        sprintf(str,"%03d bps\0",diff);
        LCDGoto(0,0);
        LCDPutStr(&str[0]);
    }
}

void LCDPutStr(char  *Str)
{
  UINT8_T i = 0;                                     // Char index buffer

  while (Str[i])                                   // While string not finished
    LCDPutChar(Str[i++]);                          // Go display current char
}
