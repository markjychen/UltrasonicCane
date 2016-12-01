/*
 * Integration
 */

#include <p18f46k22.h>
#include <delays.h>
#include "Lcd.h"
#include <stdio.h>
#include "System.h"
#include "Sensor.h"
#include "Interrupts.h"

#pragma config FOSC = INTIO67   // Internal OSC block, Port Function on RA6/7
#pragma config WDTEN = OFF      // Watch Dog Timer disabled. SWDTEN no effect
#pragma config XINST = OFF      // Instruction set Extension and indexed Addressing mode disabled

#define LOWBYTE(v)   ((unsigned char) (v))
#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (v)) >> 8))
#define One_ms_h 0xF2;
#define One_ms_l 0xE3;
#define STANDARD 0
#define SLEEP_MODE 3
#define HEAD_ONLY 1
#define RANGE_ONLY 2
#define NUM_OF_STATES = 4

unsigned int state = STANDARD;
unsigned int timeToFire = 0;
unsigned int dataReadyFlag1 = 0;
unsigned int dataReadyFlag2 = 0;
unsigned int sendHeadFlag = 0;
unsigned int headSensorVal = 0;
unsigned int casePWM = 1;
unsigned int sleep_mode = SLEEP_MODE;
unsigned int didStateChange = 0;
unsigned int volt = 0;

void main(void)
{

    ANSELBbits.ANSB5 = 0;
    TRISBbits.RB5 = 0;
    ANSELBbits.ANSB4 = 0;
    TRISBbits.RB4 = 0;
    SysInit();
    INTCONbits.GIE=0;           // Disable interrupts
    delayMillisecond(1000);
    ISRInit();
    headMotorInit();
    btnISRInit();
    LATDbits.LATD5 = 0;

    while (1) {

        switch (state%NUM_OF_STATES) {
             case STANDARD:
                 LATAbits.LATA3 = 1; 
                 LATAbits.LATA5 = 1;
                 casePWM = 1;
                 //sendPulse(1);
                 //delayMillisecond(40);
                 if (dataReadyFlag1){
                     volt = analogRead(1);
                     if (volt<66){
                         volt = 67;
                     }
                    timeToFire = (volt-66+50)*3;//*6 66 is end of cane
                    dataReadyFlag1 = 0;
                 }
                 if (dataReadyFlag2){
                     headSensorVal = analogRead(2);
                     if (headSensorVal < 70){
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
                /*if (didStateChange==1){
                    INTCONbits.GIE=0;           // Disable interrupts
                    didStateChange = 0;
                    sendPWM(150);
                    delayMillisecond(100);
                    stopPWM();
                    sendPWM(150);
                    delayMillisecond(100);
                    stopPWM();
                    INTCONbits.GIE=1;           // Enable interrupts
                }*/
                LATAbits.LATA3 = 1;
                LATAbits.LATA5 = 0;
                casePWM = 0;
                stopPWM();
                 /*if (dataReadyFlag1){
                    timeToFire = (analogRead(1)+50)*6;//*6
                    dataReadyFlag1 = 0;
                 }*/
                 if (dataReadyFlag2){
                     headSensorVal = analogRead(2);
                     if (headSensorVal < 90){
                         sendHeadFlag  = 1;
                     }else{
                         sendHeadFlag = 0;
                         LATDbits.LATD5 = 0;
                     }
                     dataReadyFlag2 = 0;
                 }
                
                break;
            case RANGE_ONLY:
                /*if (didStateChange==1){
                    INTCONbits.GIE=0;           // Disable interrupts
                    didStateChange = 0;
                    sendPWM(150);
                    delayMillisecond(100);
                    stopPWM();
                    sendPWM(150);
                    delayMillisecond(100);
                    stopPWM();
                    INTCONbits.GIE=1;           // Enable interrupts
                }*/
                casePWM = 1;
                LATAbits.LATA3 = 0;
                LATAbits.LATA5 = 1;
                sendHeadFlag = 0;
                LATDbits.LATD5 = 0;
                 if (dataReadyFlag1){
                    volt = analogRead(1);
                     if (volt<66){
                         volt = 67;
                     }
                    timeToFire = (volt-66+50)*3;//*6 66 is end of cane
                    dataReadyFlag1 = 0;
                 }
                break;
             case SLEEP_MODE:
                 casePWM = 0;
                 LATAbits.LATA3 = 0;
                 LATAbits.LATA5 = 0;
                 sendHeadFlag = 0;
                 LATDbits.LATD5 = 0;
                 stopPWM();
                 //LCDGoto(0, 0);
                 //LCDWriteStr("Sleep");
                 //LCDGoto(0,1);
                 //LCDPutByte(state);
                 //enableSleep();
                 break;
             default : //error
                 break;
}
        
    }
    
}
