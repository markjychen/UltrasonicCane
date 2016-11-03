/*
 * Integration
 */

#include <p18f46k22.h>
#include <delays.h>
#include "Lcd.h"
#include <stdio.h>
#include "System.h"

#pragma config WDTEN=OFF
#pragma config FOSC=INTIO67
#pragma config XINST=OFF


#define LOWBYTE(v)   ((unsigned char) (v))
#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (v)) >> 8))

#define STANDARD 0
#define PULSE 1
#define PULSE_RECORD 2
#define DELAY_TEST 3
#define NO_OF_STATES 4
//#define TMRL 0x58
//#define TMRH 0x9E
int state = 0;

int btnPress = 0;
unsigned char patterns[] = {0b0001010, 0b0000000};

void main(void)
{
    SysInit();
    //if (isLeftBtnPressed){state = 0;}
    //if (isRightBtnPressed){state++;}
    while(1){
         if (isRightBtnPressed()==1){
             state++;
         }
         switch (state%NO_OF_STATES) {
             case STANDARD :
                 //Start A/D Conversion
                 LCDGoto(0, 0);
                 LCDWriteStr("Demo: Pot ADC  ");
                 LCDGoto(0, 1);
                 //LCDWriteVolt(analogRead(1));
                 LCDPutByte(analogRead(1));
                 LATAbits.LATA3 = 1;
                 LATAbits.LATA5 = 0;

                 //volt = analogRead(0);
                 //LCDWriteVolt(volt);
                 break;
             case PULSE:
                 LATAbits.LATA5 = 1;
                 LATAbits.LATA3 = 0;
                 LCDGoto(0, 0);
                 LCDWriteStr("Demo: Pulse     ");
                 LCDGoto(0, 1);
                 if (isBtnPressed()==1){
                    sendPulse(1);
                 }
                 break;
             case PULSE_RECORD:
                 //Todo: write pulse and record
                 LATAbits.LATA5 = 1;
                 LATAbits.LATA3 = 1;
                 LCDGoto(0, 0);
                 LCDWriteStr("Demo:PulseRecord");
                 LCDGoto(0, 1);
                 if (isBtnPressed() == 1){
                    sendPulse(3);
                    delayMillisecond(30);
                    LCDPutByte(analogRead(1));
                 }
                 break;
             case DELAY_TEST:
                 LCDGoto(0, 0);
                 LCDWriteStr("Demo: Timer     ");
                 LCDGoto(0, 1);
                 while(1){
                    LATAbits.LATA5 = 1;
                    delayMillisecond(1);
                    LATAbits.LATA5 = 0;
                    delayMillisecond(2);
                 }
             default : //error
                 break;
        }
    }
}


