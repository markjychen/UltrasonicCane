/*
    Testing the Timer0

    Math behind it: 8 bits mode, clock source from prescaler

    Prescaler = FCPU/256 (FCPU = Fosc/4)
    Overflow INT enabled

    Prescaler: T0PS<2:0>
    111 = 1:256
    110 = 1:128
    101 = 1:64
    100 = 1:32
    011 = 1:16
    010 = 1:8
    001 = 1:4
    000 = 1:2

    FCPU = XTAL / 4 = 4MHz/4 = 1 MHz

    Time period = 1/(1E9) = 1 nS

    Let prescaler = 100 (1:32)

    Prescaler = FCPU / 256 = 1 nS * 32 = 32 nS
    Overflow Period = 32 * 256 = 8192 ns = 8.192 uS (Each overflow takes 32 counts)

    So we need 20 uS, so we need ~ 3 overflows (for ~24 us)

    Code Skeleton:
    To fire a sensor
        initialize Timer0   //note: do NOT want interrupt routine (ISR)
        Turn output HIGH
        Let it run for X amt of overflows (>= 20uS)
        Turn output LOW
        Turn off Timer0




*/

#include<stdio.h>
#include "Lcd.h"
#include <delays.h>
#include <p18f46k22.h>
#include <stdlib.h>
#include <stdio.h>
#include "General.h"
#include "Serial.h"

//Configure the device
//__CONFIG(INTIO & WDTDIS & PWRTDIS & MCLRDIS & UNPROTECT & BORDIS & IESODIS & FCMDIS);

//Configure Data Memory
unsigned char counter; //counter variable to count # of TMR overflow

#pragma config FOSC = INTIO67   // Internal OSC block, Port Function on RA6/7
#pragma config WDTEN = OFF      // Watch Dog Timer disabled. SWDTEN no effect
#pragma config XINST = OFF      // Instruction set Extension and indexed Addressing mode disabled


//Define Statements
#define STANDARD 0
#define EXTENDED 1
#define TMR_RUN 2

#define One_Sec 0x80
#define Timer1 0x89 //So thius is do not touch?

unsigned char state;
void SysInit(void);

void tmr0Init(void);

void btnInit(void);
void LCDUCInit(void);
unsigned char isLeftBtnPressed(void);
unsigned char isRightBtnPressed(void);

void main(void)
{
  SysInit();
  LCDClear();
  LCDGoto(0, 0);
  LCDWriteStr("Testing Timer0");
  printf("Hello!\n");
  LCDGoto(1, 0);

  T0CONbits.TMR0ON = 1;   //1 = ON, 0 = OFF

  while(1)
  {
      LCDGoto(0, 1);
      if (INTCONbits.T0IF){
          counter++;
          PORTAbits.RA1 = 1;//RA6 HIGH
          INTCONbits.T0IF = 0;
          PORTAbits.RA1 = 0;
      }
      LCDPutByte(counter);
  };
}

void SysInit(void)
{
    //OSCCON=0b01010110; //4 MHz internal oscillator
                        // 16 MHz internal: 0b01110110;
    OSCCON = 0b01110110;

    btnInit();
    LCDUCInit();
    tmr0Init();
    state = 0;
}

void tmr0Init(void){
    //Timer0 is SHUT DOWN in sleep mode
    T0CONbits.T0PS0 = 0;    //Pre-scaler bits
    T0CONbits.T0PS1 = 0;
    T0CONbits.T0PS2 = 0;
    
    T0CONbits.PSA = 0;      //Prescaler Assignment bit (1=Timer0 PS not assigned) (0= PS assigned))
    T0CONbits.T0SE = 0;     //Edge select bit; 0 = rising edge, 1 = falling edge
    T0CONbits.T0CS = 0;     //Clock select (0 = internal, 1 = xtal_)
   
    T0CONbits.T08BIT = 1;   //1 = 8bit timer, 0 = 16bit timer
    T0CONbits.TMR0ON = 0;   //1 = ON, 0 = OFF

    INTCONbits.TMR0IF = 0;   //clear overflow flag 
    // List of Registers Associated with Timer0
    //INTCON
    //INTCON2

    //TMR0IF flag is the overflow
    //Interrupt can be masked by clearing the TMR0IE
}

void btnInit(void){
    //Set up buttons
    ANSELBbits.ANSB0=0; //Digital
    TRISAbits.RA4=1; //Input
    TRISBbits.RB0=1; //Input
}

void LCDUCInit(void){
    //Set up LCD
    ANSELD = 0x00;
    TRISD = 0x00; //Digital out
    LCDInit(); //Start LCD
    LCDGoto(0,1);
    LCDWriteStr("Init mode");
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
