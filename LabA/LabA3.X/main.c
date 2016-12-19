/*
 * BME 464 - Lab A - Station 3
 *
 * Use the potentiometer to display the voltage on the LCD
 * Nick Bottenus - 8/23/12
 */

#include <p18f46k22.h>
#include <delays.h>
#include "Lcd.h"
#include <stdio.h>

#pragma config WDTEN=OFF
#pragma config FOSC=INTIO67
#pragma config XINST=OFF

void main(void)
{
    //Set up variables
    unsigned int volt; //16 bits
    char str[4];

    OSCCON=0b01010110; //4 MHz internal oscillator

    //Set up LCD
    ANSELD = 0x00;
    TRISD = 0x00; //Digital out

    //Set up A/D on AN0
    ANSELAbits.ANSA0 = 1;
    TRISAbits.RA0 = 1; //Analog in
    ADCON2bits.ACQT=001; //2 TAD
    ADCON2bits.ADCS=010; //FOSC/32
    ADCON2bits.ADFM=1; //Left justified
    ADCON0bits.ADON=1; //Turn on A/D

    LCDInit(); //Start LCD
    LCDGoto(0,0); //Line 1
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
                       
        Delay10KTCYx(1); //Wait a little bit
    }
}
