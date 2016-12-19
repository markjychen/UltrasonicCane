/*
 * BME 464 - Lab A - Station 4
 *
 * Sample the potentiometer and 14 samples of the waveform at 1 kHz, display alternately
 * Nick Bottenus - 8/23/12
 */

#include <p18f46k22.h>
#include <delays.h>
#include <stdio.h>
#include "Lcd.h"

#pragma config WDTEN=OFF
#pragma config FOSC=INTIO67
#pragma config XINST=OFF

void High_Priority_ISR(void);
void Low_Priority_ISR(void);
void writeVoltage(unsigned int volt);

unsigned int waveform[14];
char count=0;

#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh (void)
{
  _asm
    goto High_Priority_ISR
  _endasm
}

#pragma interrupt High_Priority_ISR
void High_Priority_ISR(void)
{
    unsigned int ivolt;

    TMR1H=0; //Reset Timer
    TMR1L=0;

    //Take sample
    ADCON0bits.GO=1; //Start conversion
    while(ADCON0bits.GO==1){}; //Wait for finish
    ivolt=ADRESH;
    ivolt=(ivolt<<8) | ADRESL; //Math needs to be done in the int variable
    if(ivolt==1023) //Fix roundoff error
        ivolt=1022;
    waveform[count]=ivolt;
    count++;

    PIR1bits.CCP1IF=0; //Reset flag
}

void main(void)
{
    unsigned int pvolt;

    OSCCON=0b01010110; //4 MHz internal oscillator

    //Set up A/D on AN0
    ANSELAbits.ANSA0 = 1;
    TRISAbits.RA0 = 1; //Analog in
    ADCON2bits.ACQT=001; //2 TAD
    ADCON2bits.ADCS=010; //FOSC/32
    ADCON2bits.ADFM=1; //Left justified
    ADCON0bits.ADON=1; //Turn on A/D

    //Acquire potentiometer value
    ADCON0bits.GO=1; //Start conversion
    while(ADCON0bits.GO==1){}; //Wait for finish
    pvolt=ADRESH;
    pvolt=(pvolt<<8) | ADRESL; //Math needs to be done in the int variable
    if(pvolt==1023) //Fix roundoff error
        pvolt=1022;

    //Set up A/D on AN2
    ANSELAbits.ANSA2 = 1;
    TRISAbits.RA2 = 1; //Analog in
    ADCON0bits.CHS=0b00010; //AN2

    //Set up Timer1
    T1CONbits.T1CKPS=0b00; //Prescaler 1:1

    //Set up ECCP1
    CCP1CONbits.CCP1M=1010; //Capture rising edge
    CCPTMRS0bits.C1TSEL=0b00; //Use Timer1
    CCPR1H=0x03; //1000, = 1 ms
    CCPR1L=0xE8;

    //Set up LCD
    ANSELD = 0x00;
    TRISD = 0x00; //Digital out

    LCDInit(); //Start LCD

    //Take 14 samples at 1 ms
    TMR1H=0; //Clear timer
    TMR1L=0;
    PIR1bits.CCP1IF=0; //Clear flag
    RCONbits.IPEN=1; //Allow interrupt priorities
    IPR1bits.CCP1IP=1; //Set to high priority
    PIE1bits.CCP1IE=1; //Enable CCP1 interrupt
    INTCONbits.GIE=1; //Enable interrupts
    T1CONbits.TMR1ON=1; //Turn timer on

    while(count<14){}
    T1CONbits.TMR1ON=0; //Turn timer off
    INTCONbits.GIE=0; //Disable interrupts

    //Display values
    count=0;
    while(1)
    {
        writeVoltage(pvolt);
        Delay10KTCYx(100); //Wait 1 s
        writeVoltage(waveform[count]);
        count++;
        if(count==14)
            count=0;
        Delay10KTCYx(100); //Wait 1 s
    }
}

void writeVoltage(unsigned int volt)
{
    char str[3];
    LCDGoto(0,0);

    sprintf(str,"%03d",volt*49/100); //Approximate conversion to 0-5V
    LCDPutChar(str[0]);
    LCDPutChar('.');
    LCDPutChar(str[1]);
    LCDPutChar(str[2]);
    LCDPutChar('V');
}