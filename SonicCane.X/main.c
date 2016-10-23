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

#pragma config WDTEN=OFF
#pragma config FOSC=INTIO67
#pragma config XINST=OFF

#define LOWBYTE(v)   ((unsigned char) (v))
#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (v)) >> 8))

#define STANDARD = 0;
#define PULSE = 1;

int state = 0;

void SysInit(void);
unsigned char isLeftBtnPressed(void);
unsigned char isRightBtnPressed(void);

void main(void)
{
    unsigned char patterns[] = {0b0001010, 0b0000000};
    char count=0;
    char dir = 1;
    unsigned int volt = 0; 
    char tmrl = 0x58;   //25 ms (2^16-(2.5e-2/(1/4e6/4)))
    char tmrh = 0x9E;
    
    OSCCON=0b01010110; //4 MHz internal oscillator

    SysInit();
    LCDClear();
    LCDGoto(0, 0);
    LCDWriteStr("Hello World!");
    printf("Hello!\n");

    while(1){
    // Set up variables
     unsigned int volt = 7; //16 bits
     char str[4];

      //Start A/D Conversion
     ADCON0bits.GO = 1; 
     while (ADCON0bits.GO ==1){}; //GO bit automatically clears
     volt = ADRESH;
     volt=(volt<<8) | ADRESL; //Math needs to be done in the int variable
     if(volt==1023) //Fix roundoff error
        volt=1022;
     sprintf(str,"%04d",volt*49/10); //Approximate conversion to 0-5V
      
     LCDGoto(0,1);
     isLeftBtnPressed();
     isRightBtnPressed();
     Delay10KTCYx(10);
        //delay(100);

     switch (state%2){
            case STANDARD:
               LCDPutChar(str[0]);
               LCDPutChar('.');
                LCDPutChar(str[1]);
                LCDPutChar(str[2]);
                LCDPutChar(str[3]);
                LCDPutChar('V');        
                break;
                        
            case PULSE:
                LCDWriteStr("Placeholder B");
                break;
            default:
                //add error
                break;
    }
  };
}

void SysInit(void)
{
    //OSCCON=0b01010110; //4 MHz internal oscillator
                        // 16 MHz internal: 0b01110110; 
    OSCCON = 0b01110110;

    //Set up LEDs
    ANSELB=0b00000000; //Digital IO
    LATB=0b00000000; //LEDs off
    TRISB=0b00000000; //LEDs are outputs

     //Set up A/D on AN0
    ANSELAbits.ANSA0 = 1;
    TRISAbits.RA0 = 1; //Analog in
    ADCON2bits.ACQT=001; //2 TAD
    ADCON2bits.ADCS=010; //FOSC/32
    ADCON2bits.ADFM=1; //Left justified
    ADCON0bits.ADON=1; //Turn on A/D
    
    //Set up A/D on AN1        
    ADCON1 = 0b00001110;//VSS,VDD ref. AN0 analog only
	ADCON2 = 0b00001000;//ADCON2 setup: Left justified, Tacq=2Tad, Tad=2*Tosc (or Fosc/2)
    ADCON2bits.ACQT=001; //2 TAD (labA))
    ADCON2bits.ADFM=1; //Right justified (labA))
    ADCON2bits.ACQT=001; //2 TAD
    ADCON2bits.ADCS=010; //FOSC/32
    ADCON2bits.ADFM=1; //Left justified
    
    ADCON0bits.ADON=1; //Turn on A/D (labA)
 
    ADCON0 = 0b10001010;//clear ADCON0 to select channel 0 (AN0)
	ADCON0bits.ADON = 0x01;//Enable A/D module
    ADCON0bits.CHS=0001; //Select RA1
         
    //Set up LCD
    ANSELD = 0x00;
    TRISD = 0x00; //Digital out

    LCDInit(); //Start LCD
    LCDGoto(0,1);
    LCDWriteStr("Init mode");
    
    //Set up Timer0
    T0CONbits.T0CS=0; //Use internal clock (4 MHz/4)
    T0CONbits.T08BIT=0; //16 bit counter
    T0CONbits.PSA=1; //Don't use prescaler (1:1)
}
