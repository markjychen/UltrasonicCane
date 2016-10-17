/*
 *  PWM Development Branch
 */

#include<stdio.h>
#include "Lcd.h"
#include <delays.h>
#include <p18f46k22.h>
#include <stdlib.h>
#include "General.h"
#include "Serial.h"

#pragma config FOSC = INTIO67   // Internal OSC block, Port Function on RA6/7
#pragma config WDTEN = OFF      // Watch Dog Timer disabled. SWDTEN no effect
#pragma config XINST = OFF      // Instruction set Extension and indexed Addressing mode disabled

#define STANDARD 0
#define EXTENDED 1
#define PWM 2

//__CONFIG(INTIO & WDTDIS & PWRTEN & MCLRDIS & UNPROTECT & BORDIS & IESODIS & FCMDIS)

int Dlay; //LED Time on Delay variable
char ADCValue;


unsigned char state;
void SysInit(void);

unsigned char isLeftBtnPressed(void);
unsigned char isRightBtnPressed(void);

void main(void)
{
  SysInit();
  LCDClear();
  LCDGoto(0, 0);
  LCDWriteStr("Hello World!");
  printf("Hello!\n");

  while(1)
  {
    // Set up variables
     unsigned int volt; //16 bits
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

     switch (state%3){
            case STANDARD:
               LCDPutChar(str[0]);
               LCDPutChar('.');
                LCDPutChar(str[1]);
                LCDPutChar(str[2]);
                LCDPutChar(str[3]);
                LCDPutChar('V');
                break;
            case EXTENDED:
                LCDWriteStr("Extended");
                break;
            case PWM:
                LCDWriteStr("PWM");
                Nop();
                for (Dlay = 0; Dlay < 6666; Dlay++); //100 ms between
                Nop();  //Samples
                ADCON0bits.GO = 1; //Read Pot Value
                while (ADCON0bits.GO);
                ADCValue = ADRESH; //Read in ADC Value
                if (ADCValue > 0x80){ //go Forwards{
                  CCPR1L = (ADCValue - 80) >> 1;
                  CCP1CON = 0b01001110;
                  TRISC = 0b011011; // RC5/RC2 Output, RC3/RC4 Input
                } else { // Go in Reverse
                  CCPR1L = (ADCValue ^ 0x7F) >> 1;
                  CCP1CON = 0b11001110;
                  
                  TRISC = 0b100111; //RC5/RC2 Output, RC3/RC4 Input
                } //fi //elihw
            //end motor
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

    //Set up buttons
    ANSELBbits.ANSB0=0; //Digital
    TRISAbits.RA4=1; //Input
    TRISBbits.RB0=1; //Input

    //Set up A/D on AN0
    ANSELAbits.ANSA0 = 1;
    TRISAbits.RA0 = 1; //Analog in
    ADCON2bits.ACQT=001; //2 TAD
    ADCON2bits.ADCS=010; //FOSC/32  //was 100 while running ADC test; 010 from lab
    ADCON2bits.ADFM=1; //Left justified
    ADCON0bits.ADON=1; //Turn on A/D

    //Set up PWM
    PORTC = 0;
    //CMCON0 = 7; //Turn off Comparators
    //ANSELA = 1<<3; //RA4 (AN3) is the ADC Input

    ADCON0 = 0b00001101;
      //Turn on the ADC
      //Bit 7 - Left Justified Sample
      //Bit 6 - Use VDD
      //Bit 4:2 - RA4
      //Bit 1 - Do not Start
      //Bit 0 - Turn on ADC

    TMR2 = 0; //TMR2 Provides PWM Period
    PR2 = 64; //15 kHz PWM Freq

    T2CON = 0b0000100; //Enable TMR2

    CCPR1L = 0; //0 Duty Cycle to start off

    //Set up LCD
    ANSELD = 0x00;
    TRISD = 0x00; //Digital out

    LCDInit(); //Start LCD
    LCDGoto(0,1);
    LCDWriteStr("Init mode");

    state = 0;
}

unsigned char isLeftBtnPressed(void){
  if (PORTAbits.RA4 == 0){
      Delay10KTCYx(10);
      state=0;
      LCDGoto(0,1);
      LCDWriteStr("                ");
      LCDGoto(0,1);
      return 1;
 }
 return 0;
}

unsigned char isRightBtnPressed(void){
  if (PORTBbits.RB0 == 0){
      Delay10KTCYx(10);
      state++;
      LCDGoto(0,1);
      LCDWriteStr("                ");
      LCDGoto(0,1);
      return 1;
 }
 return 0;
}
