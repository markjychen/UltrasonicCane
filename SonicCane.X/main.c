/*
    Testing the Timer0
*/

#include<stdio.h>
#include "Lcd.h"
#include <delays.h>
#include <p18f46k22.h>
#include <stdlib.h>
#include "General.h"
#include "Serial.h"

//Configure the device
//__CONFIG(INTIO & WDTDIS & PWRTDIS & MCLRDIS & UNPROTECT & BORDIS & IESODIS & FCMDIS);

//Configure Data Memory
unsigned char counter; //counter variable to count # of TMR overflow

#pragma config FOSC = INTIO67   // Internal OSC block, Port Function on RA6/7
#pragma config WDTEN = OFF      // Watch Dog Timer disabled. SWDTEN no effect
#pragma config XINST = OFF      // Instruction set Extension and indexed Addressing mode disabled

#define STANDARD 0
#define EXTENDED 1
#define TMR_RUN 2


unsigned char state;
void SysInit(void);

void tmr0Init(void);

void btnInit(void);
void LCDInit(void);
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

  while(1)
  {
      LCDWrite("LOLOLOLOL")
  };
}

void SysInit(void)
{
    //OSCCON=0b01010110; //4 MHz internal oscillator
                        // 16 MHz internal: 0b01110110;
    OSCCON = 0b01110110;

    btnInit();
    LCDInit();
    state = 0;
}

void tmr0Init(void){
    //Set up Timer0
    //Assigns prescaler to WDT so TMR0 increments as 1:1 ratio with WDT
    T0CONbits.PSA = 1; //Clear the TMR0 register, prescaler 1:1
    T0CONbits.T0CS = 0;
    T0CONbits.TMR0ON = 1;
    TMR0H = 0;
    TMR0L = 0;
}

void btnInit(void){
    //Set up buttons
    ANSELBbits.ANSB0=0; //Digital
    TRISAbits.RA4=1; //Input
    TRISBbits.RB0=1; //Input
}

void LCDInit(void){
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
