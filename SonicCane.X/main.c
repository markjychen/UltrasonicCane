/*
 * Integration
 */

#include <p18f46k22.h>
#include <delays.h>
#include "Lcd.h"
#include <stdio.h>
//#include "System.h"
#include "Interrupts.h"

#pragma config FOSC = INTIO67   // Internal OSC block, Port Function on RA6/7
#pragma config WDTEN = OFF      // Watch Dog Timer disabled. SWDTEN no effect
#pragma config XINST = OFF      // Instruction set Extension and indexed Addressing mode disabled

#define LOWBYTE(v)   ((unsigned char) (v))
#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (v)) >> 8))
#define One_ms_h 0xF2;
#define One_ms_l 0xE3;
#define STANDARD 0
#define SLEEP_MODE 1
#define HEAD_ONLY 4
#define RANGE_ONLY 3
#define NUM_OF_STATES = 2

unsigned int state = STANDARD;
unsigned int timeToFire = 0;
unsigned int dataReadyFlag1 = 0;
unsigned int dataReadyFlag2 = 0;
unsigned int sendHeadFlag = 0;
unsigned int headSensorVal = 0;

void main(void)
{

    ANSELBbits.ANSB5 = 0;
    TRISBbits.RB5 = 0;
    ANSELBbits.ANSB4 = 0;
    TRISBbits.RB4 = 0;
    SysInit();
    INTCONbits.GIE=0;           // Enable interrupts
    delayMillisecond(1000);
    ISRInit();
    headMotorInit();
    btnISRInit();
    LATDbits.LATD5 = 0;

    while (1) {

        switch (state%NUM_OF_STATES) {
             case STANDARD:
                 LATAbits.LATA3 = 0;
                 //sendPulse(1);
                 //delayMillisecond(40);
                 if (dataReadyFlag1){
                    timeToFire = (analogRead(1)+50)*6;//*6
                    dataReadyFlag1 = 0;
                 }
                 if (dataReadyFlag2){
                     headSensorVal = analogRead(2);
                     if (headSensorVal < 60){
                         sendHeadFlag  = 1;
                     }else{
                         sendHeadFlag = 0;
                         LATDbits.LATD5 = 0;
                     }
                     dataReadyFlag2 = 0;
                 }
                 //if (analogRead(2) < 60){
                 //    sendHeadWarning(0);
                 //}else{
                 //    LATDbits.LATD5 = 0;
                // }
                 break;
                 
            case HEAD_ONLY:
                LATAbits.LATA3 = 1;
                stopPWM();
                 if (dataReadyFlag1){
                    timeToFire = (analogRead(1)+50)*6;//*6
                    dataReadyFlag1 = 0;
                 }
                 if (dataReadyFlag2){
                     headSensorVal = analogRead(2);
                     if (headSensorVal < 60){
                         sendHeadFlag  = 1;
                     }else{
                         sendHeadFlag = 0;
                         LATDbits.LATD5 = 0;
                     }
                     dataReadyFlag2 = 0;
                 }
                
                break;
             case SLEEP_MODE:
                 LATAbits.LATA3 = 0;
                 LATDbits.LATD5 = 0;
                 stopPWM();
                 //LCDGoto(0, 0);
                 //LCDWriteStr("Sleep");
                 //LCDGoto(0,1);
                 //LCDPutByte(state);
                 //enableSleep();
             default : //error
                 break;
}
        
    }
    
}
