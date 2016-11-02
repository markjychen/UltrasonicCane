/*
 * Integration
 */

#include "Lcd.h"
#include <delays.h>
#include <p18f46k22.h>
#include <stdio.h>


#pragma config WDTEN = SWON
#pragma config WDTPS = 1024
#pragma config FOSC = INTIO67
#pragma config XINST = OFF

#define LOWBYTE(v)   ((unsigned char) (v))
#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (v)) >> 8))

#define STANDARD 0
#define NO_OF_STATES 1
#define TMRL 0x58
#define TMRH 0x9E
int state = 0;

int btnPress = 0;
unsigned char patterns[] = {0b0001010, 0b0000000};
void SysInit(void);


void main(void)
{
    LCDInit();
    //LCDClear();

    while(1){
        LCDWriteStr("hi");
    }
    while(1){
         switch (state%NO_OF_STATES) {
            case STANDARD :
                //Start A/D Conversion
                LCDGoto(0, 0);
                LCDWriteStr("Demo: Pot ADC  ");
                LCDGoto(0, 1);
                //volt = analogRead(0);
                //LCDWriteVolt(volt);
                break;
            default : //error
                break;
        }
    }
}

void SysInit(void)
{
    //OSCCON=0b01010110; //4 MHz internal oscillator
                        // 16 MHz internal: 0b01110110;
    OSCCON=0b01010110; //4 MHz internal oscillator
    LCDInit();
}

