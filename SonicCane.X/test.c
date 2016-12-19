// Finite State Machine
//
//
//         Initialization
//            |
//            |
//            |
//            V
//          Standard (loop)       ------ (R btn) ---->  Extended
//             ^                 <-------(L btn) ------    | ^
//             |                                           | |
//             |                                    (R btn)  (L btn)
//             |                                           | |
//             |                                           | |
//             |                                           V |
//             --------------- <------- (R btn) -----Battery Saver









#include<stdio.h>
#include "Lcd.h"
#include <delays.h>
#include <p18f46k22.h>
#include <stdlib.h>

#define STANDARD 0
#define EXTENDED 1
#define BAT_SAVER 2

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
    LCDGoto(0,1);
    isLeftBtnPressed();
    isRightBtnPressed();
    Delay10KTCYx(10);
    //delay(100);

    switch (state%3){
      case STANDARD:
        LCDWriteStr("Standard\n");
        break;
      case EXTENDED:
        LCDWriteStr("Extended\n");
        break;
      case BAT_SAVER:
        LCDWriteStr("Battery Saver\n");
        break;
      default:
        break;
    }
  };
}

void SysInit(void)
{
    OSCCON=0b01010110; //4 MHz internal oscillator

    //Set up buttons
    ANSELBbits.ANSB0=0; //Digital
    TRISAbits.RA4=1; //Input
    TRISBbits.RB0=1; //Input

    //Set up LCD
    ANSELD = 0x00;
    TRISD = 0x00; //Digital out

    LCDInit(); //Start LCD
    LCDWriteStr("Starting clock...");

    state = 0;
}

unsigned char isLeftBtnPressed(void){
  if (PORTAbits.RA4 == 0){
      Delay10KTCYx(5);
      state++;
      return 1;
 }
 return 0;
}

unsigned char isRightBtnPressed(void){
  if (PORTBbits.RB0 == 0){
      Delay10KTCYx(5);
      state--;
      return 1;
 }
 return 0;
}
