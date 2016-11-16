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
/*#define STANDARD 0
#define PULSE 1
#define PULSE_RECORD 2
#define PWM 3
#define DELAY_TEST 999
#define PULSE_RECORD_PWM 4
#define CONT_RECORD_PWM 5
#define NO_OF_STATES 6
//#define TMRL 0x58
//#define TMRH 0x9E

int state = CONT_RECORD_PWM;
*/


unsigned int timeToFire = 0;


void main(void)
{
    ANSELBbits.ANSB5 = 0;
    TRISBbits.RB5 = 0;
    ANSELBbits.ANSB4 = 0;
    TRISBbits.RB4 = 0;
    SysInit();
    //ISRInit();
    //LATBbits.LATB1 = 1;

    while (1) {
        //LATBbits.LATB5 = ~LATBbits.LATB5;
        //LATBbits.LATB2 = 0;
        /*sendPulse(1);
        delayMillisecond(1);
        //myVolt = smooth(analogRead(0), 0.75 ,myVolt);
        myVolt = analogRead(1); //boxcar_filter (analogRead(1), index);
        //myVolt2 = analogRead(2);
        //myVolt2 = boxcar_filter(analogRead(0), index);
        //index++;
        //index = index % 5;
        sendPWM(225);
        delayMillisecond(50); //interrupt for 50ms pwm
        stopPWM();
        //LCDWriteLevels(myVolt / 2);
        //LCDWriteStr(" ");
        //LCDWriteLevels(myVolt2 / 2);
        //LCDGoto(0, 1);
        delayMillisecond(myVolt);*/
        LCDGoto(0, 0);
        timeToFire = analogRead(0) * 2 + 200;
    }
    
}
