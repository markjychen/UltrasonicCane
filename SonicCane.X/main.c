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
//#include "Sensor.h"

#pragma config WDTEN = SWON
#pragma config WDTPS = 1024
#pragma config FOSC = INTIO67
#pragma config XINST = OFF

#define LOWBYTE(v)   ((unsigned char) (v))
#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (v)) >> 8))

#define STANDARD 0
#define PWM_DEMO 1
#define PULSE 2
#define ADC 3
#define SLEEP 4
#define NO_OF_STATES 5
#define TMRL 0x58
#define TMRH 0x9E
int state = 0;

int btnPress = 0;
unsigned char patterns[] = {0b0001010, 0b0000000};
void SysInit(void);
//List of Necessary Functions
/*void SysInit(void);
//unsigned char isLeftBtnPressed(void);
//unsigned char isRightBtnPressed(void);
//unsigned char isBtnPressed(void);
int analogRead0(void);
void sendPulse(int);
void sendPWM(int);
int potLvl(void);
void enableSleep(void);
void send_us (int);*/

//void wakeUp(void);

/*void High_Priority_ISR(void);

#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void){
    _asm
        goto High_Priority_ISR
    _endasm
}

#pragma interrupt High_Priority_ISR
void High_Priority_ISR(void){
    wakeUp();
}*/

void main(void)
{
    unsigned int volt = 0;
    unsigned int adc1;
    unsigned int adc2;

    SysInit();
    LCDClear();

    while(1){
        // Set up variables
        unsigned int volt = 0; //16 bits
        char str[4];
        char per[4];

        LCDGoto(0, 1);
        if(isLeftBtnPressed()){
            state = 0;
        };
        if(isRightBtnPressed()){
            state++;
        };
        //isBtnPressed();
        Delay10KTCYx(10);
        //delay(100);

         switch (state%NO_OF_STATES) {
            case STANDARD :
                //Start A/D Conversion
                LCDGoto(0, 0);
                LCDWriteStr("Demo: Pot ADC  ");
                LCDGoto(0, 1);
                volt = analogRead(0);
                LCDWriteVolt(volt);
                break;
            /*case PWM_DEMO:
                //LCDGoto(0, 0);
                 LCDGoto(0, 0);
                ADCON0bits.CHS=0000; //Select RA0
                volt = analogRead0();
                LCDWriteStr("PWM Demo        ");
                sprintf(per, "%02d", volt);
                LCDGoto (0, 1);
                LCDPutChar(per[0]);
                LCDPutChar(per[1]);
                LCDPutChar(per[2]);
                LCDPutChar(per[3]);
                LCDWriteStr("            ");

                //sendPWM(volt);
                break;
            case PULSE:
                LCDGoto(0, 0);
                LCDWriteStr("One Pulse 20us  ");
                LCDGoto(0, 1);
                LCDPutByte(btnPress);
                LCDWriteStr("              ");
                if(isBtnPressed()){
                    LATBbits.LATB1 = 1; //this is correct...
                    //sendPulse(1);
                    send_us(1);
                    //btnPress++;
                }else{
                    LATBbits.LATB1 = 0;
                }
                break;
            case ADC:
                ADCON0bits.CHS = 0001;
                adc1 = analogRead();
                sprintf(str, "%04d", adc1 * 49 / 10); //Approximate conversion to 0-5V
                LCDGoto(0, 0);
                LCDWriteStr("RA1: ");
                LCDPutChar(str[0]);
                LCDPutChar('.');
                LCDPutChar(str[1]);
                LCDPutChar(str[2]);
                LCDPutChar(str[3]);
                LCDPutChar('V');
                LCDWriteStr("          ");

                LCDGoto(0, 1);
                ADCON0bits.CHS = 2; //ra2
                adc2 = analogRead();
                sprintf(str, "%04d", adc2 * 49 / 10); //Approximate conversion to 0-5V
                LCDWriteStr("RA2: ");
                LCDPutChar(str[0]);
                LCDPutChar('.');
                LCDPutChar(str[1]);
                LCDPutChar(str[2]);
                LCDPutChar(str[3]);
                LCDPutChar('V');
                LCDWriteStr("          ");

                break;
            case SLEEP:

                LCDGoto(0, 0);
                LCDWriteStr("Sleep Mode      ");
                LCDGoto(0, 1);;
                enableSleep();
                break;
                //sendPulse(2);*/
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

    analogInit();
    pulseInit();
    //motorInit();
    LCDInit();
    potentiometerInit();
    LEDInit();
    buttonInit();
}


int potLvl(void){
    int lvl = 0;
    char lvl_str[4];
    int volt = analogRead0();
    lvl = (volt*4/1023)%4;
    sprintf(lvl_str, "%04d", lvl);
    LCDGoto(0, 1);
    LCDPutChar(lvl_str[0]);
    LCDPutChar(lvl_str[1]);
    LCDPutChar(lvl_str[2]);
    LCDPutChar(lvl_str[3]);
    return lvl;
}

void sendPWM(int DC){

    //DC = 90;
    PR2 = 249;          // Timer2 period register = 250 counts //DC?
    CCPR1L = HIGHBYTE(DC);//125;//0xBF;      // The 8 most sig bits of the period are 0x7D
                        // DC% = CCPR1L = % * PR2
    CCP1CON = 0b01001100; // The 2 LSbs are 0b00, and CCP1Mz = 110 for PWM
}
