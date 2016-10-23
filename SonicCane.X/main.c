/*
 *  PWM Development Branch
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

void SysInit(void);

void SysInit(void){

    OSCCON = 0b00001010;    //8 MHz internal

    TRISCbits.TRISC7 = 0;
    CCPTMRS1bits.C4TSEL = 0b00;
    CCPR2 = 0x65;
    CCP4CONbits.CCP4M2 = 1;
    CCP4CONbits.CCP4M3 = 1;
    CCP4CONbits.CCP4M0 = 0;
    CCP4CONbits.CCP4M1 = 0;
    //CCPR4L = ;
    //CCPR4H = ;

    //Enable Timer2
    T2CONbits.TMR2ON = 1;

    //Enable output
    TRISCbits.TRISC1 = 0; //make CCP2 pin output
    TRISCbits.TRISC2 = 0; //make CCP1 pin output //RC2
}

void main(void){
    SysInit();
    while(1){}
}
