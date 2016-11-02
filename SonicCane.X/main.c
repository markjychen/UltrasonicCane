#define USE_OR_MASKS
#include <p18cxxx.h>
#include "pwm.h"

//http://www.microchip.com/forums/m506808.aspx

#pragma config WDTEN=OFF
#pragma config FOSC=INTIO67
#pragma config XINST=OFF
void sendPWM(int);
int analogRead(void);
void writeToScreen(int);

void sendPWM(int val){
    PR2 = 249;          // Timer2 period register = 250 counts //DC?
    
    CCPR1L = val;      // The 8 most sig bits of the period are 0x7D     
                        // DC% = CCPR1L = % * PR2
    CCP1CON = 0b01001100; // The 2 LSbs are 0b00, and CCP1Mz = 110 for PWM
}

void main(void)
{
    int volt = analogRead();
    OSCCON=0b01010110; //set to 4 MHz (labA))   
    TRISDbits.TRISD7 = 0;  //set PWM pin RD7 output 
    T2CON = 0b00000111; // Prescale 1:16, timer on
    
    // ANSELD = 0x00; TRISD = 0x00;
    //LCDInit();

        //Set up RA0 for potentiometer read
    //Set up A/D on AN0
    ANSELAbits.ANSA0 = 1;
    TRISAbits.RA0 = 1; //Analog in
    ADCON2bits.ACQT=001; //2 TAD
    ADCON2bits.ADCS=010; //FOSC/32
    ADCON2bits.ADFM=1; //Left justified
    ADCON0bits.ADON=1; //Turn on A/D
    ADCON0bits.CHS = 0000;

    
    while (1){
        volt = analogRead();
        //writeToScreen(volt/4);
        sendPWM(volt/4);
    }
}
int analogRead(void){
          //Start A/D Conversion
     int val = 0;
     ADCON0bits.GO = 1; 
     while (ADCON0bits.GO ==1){}; //GO bit automatically clears
     val = ADRESH;
     val=(val<<8) | ADRESL; //Math needs to be done in the int variable
     if(val==1023) //Fix roundoff error
        val=1022;
     return val;
}
void writeToScreen(int val){
    char str[4];
    sprintf(str,"%04d",val); //Approximate conversion to 0-5V
    LCDGoto(0,0);
    LCDPutChar(str[0]);
    LCDPutChar(str[1]);
    LCDPutChar(str[2]);
    LCDPutChar(str[3]);
}