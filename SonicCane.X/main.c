/*
 * BME 464 - Lab A - Station 1
 *
 * Blink LEDs to draw a pattern as the board is shaken back and forth
 * Nick Bottenus - 8/23/12
 */

#include "Lcd.h"
#include <delays.h>
#include <p18f46k22.h>
#include <stdio.h>

#pragma config WDTEN=OFF
#pragma config FOSC=INTIO67
#pragma config XINST=OFF

#define LOWBYTE(v)   ((unsigned char) (v))
#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (v)) >> 8))

void main(void)
{
    /*unsigned char patterns[] = {0b00001110,
                                0b00001011,
                                0b00001001,
                                0b00001011,
                                0b00001110};
     * */
    unsigned char patterns[] = {0b0001010, 0b0000000};
    char count=0;
    char dir = 1;
    unsigned int volt = 0; //16 bits
    unsigned int DC = 0; //16 bits

    //char tmrh = 0x63;
    //char tmrl = 0xC0; //40 ms(2^16-4e-2/(1/(4e6/4)))
    //char tmrl = 0x00; //25 ms (2^16-(2.5e-2/(1/4e6/4)))
    char tmrl = 0x58;
    char tmrh = 0x9E;
    
    OSCCON=0b01010110; //4 MHz internal oscillator

    //Set up LEDs
    ANSELB=0b00000000; //Digital IO
    LATB=0b00000000; //LEDs off
    TRISB=0b00000000; //LEDs are outputs

     //Set up A/D on AN0
    ANSELAbits.ANSA0 = 1;
    TRISAbits.RA0 = 1; //Analog in
    ADCON2bits.ACQT=001; //2 TAD
    ADCON2bits.ADCS=010; //FOSC/32
    ADCON2bits.ADFM=1; //Left justified
    ADCON0bits.ADON=1; //Turn on A/D
    

    
    //Set up Timer0
    T0CONbits.T0CS=0; //Use internal clock (4 MHz/4)
    T0CONbits.T08BIT=0; //16 bit counter
    T0CONbits.PSA=1; //Don't use prescaler (1:1)
    TMR0H=tmrh;
    TMR0L=tmrl;

    //
    
    while(1){
        while(INTCONbits.TMR0IF==0){}
            ADCON0bits.GO = 1;
            while(ADCON0bits.GO ==1){};
            volt = ADRESH;
            volt = (volt<<8) | ADRESL;
            if (volt == 1023) //Fix roundoff error
                volt = 1022;
            DC = 40536 * volt / 1022;
        
        INTCONbits.TMR0IF=0; //Reset flag
        TMR0H=HIGHBYTE(DC);
        TMR0L=LOWBYTE(DC);

        count+=dir; //Increment counter
        if(count==0||count==sizeof(patterns)-1)
            dir*=-1;
        LATB=patterns[count]; //Light LEDs
    }
}
