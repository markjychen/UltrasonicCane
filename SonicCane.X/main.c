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

int state = 0;

void main(void)
{
    char str[4];
    int volt = 0;
    int myVolt;
    SysInit();
    //if (isLeftBtnPressed){state = 0;}
    //if (isRightBtnPressed){state++;}
    while(1){
         if (isRightBtnPressed()==1){
             state++;
         }
         switch (state%NO_OF_STATES) {
             case CONT_RECORD_PWM:
                 LCDGoto(0, 0);
                 LCDWriteStr("Demo: Continuous");
                 LCDGoto(0, 1); 
                 LCDWriteStr("                ");
                 //while (isBtnPressed() != 1){
                     sendPulse(3);
                     delayMillisecond(3);
                     myVolt = analogRead(1);
                     sendPWM(myVolt/4);
                     delayMillisecond(500);
                     LCDWriteLevels(analogRead(1));
                     stopPWM();  
                     delayMillisecond(100);
                     LCDGoto(0, 1);
                 //}
                 break;
                 
             case SLEEP_MODE:
                 LCDGoto(0, 0);
                 LCDWriteStr("Sleep until button");
                 LCDGoto(0,1);
                 LCDWriteStr("                ");
                 enableSleep();
             default : //error
                 break;
        }
    }
}

void LCDWriteLevels(int volt){
    char str[4];
    sprintf(str, "%04d", volt);
    LCDPutChar(str[0]);
    LCDPutChar(str[1]);
    LCDPutChar(str[2]);
    LCDPutChar(str[3]);
    LCDPutChar("|");

}
