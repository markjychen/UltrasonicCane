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
#define PWM 3
#define DELAY_TEST 999
#define PULSE_RECORD_PWM 4
#define CONT_RECORD_PWM 5
#define NO_OF_STATES 6
//#define TMRL 0x58
//#define TMRH 0x9E

void LCDWriteLevels(int);

int state = CONT_RECORD_PWM;

void main(void)
{
    int myVolt;
    SysInit();
    ANSELB=0b00000000; //Digital IO
    LATB=0b00000000; //LEDs off
    TRISB=0b00000000; //LEDs are outputs
    //if (isLeftBtnPressed){state = 0;}
    //if (isRightBtnPressed){state++;}

    LATBbits.LATB1 = 1;
    //LCDGoto(0, 0);
    //LCDWriteStr("Demo: Continuous");
    //LCDGoto(0, 1);
    //LCDWriteStr("                ");
    while (1) {
        sendPulse(1);
        delayMillisecond(1);
        //myVolt = smooth(analogRead(0), 0.75 ,myVolt);
        myVolt = analogRead(1); //boxcar_filter (analogRead(1), index);
        //myVolt2 = analogRead(2);
        //myVolt2 = boxcar_filter(analogRead(0), index);
        //index++;
        //index = index % 5;
        sendPWM(175);
        delayMillisecond(50); //interrupt for 50ms pwm
        stopPWM();
        //LCDWriteLevels(myVolt / 2);
        //LCDWriteStr(" ");
        //LCDWriteLevels(myVolt2 / 2);
        //LCDGoto(0, 1);
        delayMillisecond(myVolt * 0.85);

    }
    
}

void LCDWriteLevels(int volt){
    char str[4];
    sprintf(str, "%04d", volt);
    LCDPutChar(str[0]);
    LCDPutChar(str[1]);
    LCDPutChar(str[2]);
    LCDPutChar(str[3]);
//    LCDPutChar("|");

}
