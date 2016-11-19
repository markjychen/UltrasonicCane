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

#define STANDARD 01111
#define PULSE 6666
#define PULSE_RECORD 777
#define PWM 888
#define DELAY_TEST 999
#define PULSE_RECORD_PWM 4

#define CONT_RECORD_PWM 0
#define SLEEP_MODE 1
#define NO_OF_STATES 2
//#define TMRL 0x58
//#define TMRH 0x9E

void LCDWriteLevels(int);
void btnISR(void);
void High_Priority_ISR(void);
void btnISRInit(void);
void isr(void);
int state = 0;

#pragma code InterruptVectorHigh = 0x08

#pragma interrupt High_Priority_ISR
void High_Priority_ISR(void){
    btnISR();
}
void btnISR(void){
    if (INTCON3bits.INT1IF){
        INTCON3bits.INT1IF = 0;
        state++;
    }
}

void btnISRInit(void){
    
    OSCCONbits.IDLEN = 0;

    ANSELBbits.ANSB1 = 0;
    TRISBbits.RB1 = 0;
    
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT1IE = 1;
    INTCON2bits.INTEDG1 = 1;
    INTCON3bits.INT1F = 0;
    INTCON3bits.INT1E = 1;
    

    //INTCONbits.RBIE = 1; //enable interrupt
    INTCON2bits.RBPU = 0;
   //  INTCONbits.INT0F = 0;
    //INTCONbits.INT0E = 1;
    INTCONbits.GIE = 1;
    RCONbits.IPEN = 1;
    INTCONbits.PEIE = 1;
}

void main(void)
{
    char str[4];
    int volt = 0;
    int myVolt;
    int myVolt2;
    unsigned int headSensorFlag = 0;

    SysInit();
    LCDClear();
    LCDWriteStr("HIIII");
    btnISRInit();

    
    while(1){
         switch (state%NO_OF_STATES) {
             case CONT_RECORD_PWM:
                    sendPulse(1);
                    delayMillisecond(40);
                    myVolt = analogRead(1);//boxcar_filter (analogRead(1), index);
                    myVolt2 = analogRead(2);
                    if (myVolt2<60){
                        headSensorFlag = 1;
                        sendPWM(200);
                        delayMillisecond(100);
                        stopPWM();
                    }else{
                        headSensorFlag = 0;
                    }
                    if (1==0){//!headSensorFlag){
                     sendPWM(150);
                     delayMillisecond(50); //interrupt for 50ms pwm
                     stopPWM();
                     delayMillisecond(myVolt*2);
                    }
                 break;
                 
             case SLEEP_MODE:
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

void LCDWriteLevels(int volt){
    char str[4];
    sprintf(str, "%04d", volt);
    LCDPutChar(str[0]);
    LCDPutChar(str[1]);
    LCDPutChar(str[2]);
    LCDPutChar(str[3]);
    LCDPutChar("|");

}





