// Finite State Machine
//
//
//         Initialization
//            |
//            |
//            |
//            V
//          Standard (loop)       ------ (R btn) ---->  Extended
//             ^                 <-------(L btn) ------    | 
//             |                                           | 
//             |                                    (R btn)  
//             |                                           | 
//             |                                           | 
//             |  <--------------------- (L btn) ----------| 
//             --------------- <------- (R btn) -----Battery Saver









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
#define BAT_SAVER 1
#define TEST_SER 2 
#define NO_OF_STATES 3

unsigned char state;

void SysInit(void);
void standardState(void);
void powerSaverState (void);
void debugState(void);
int analogRead(void);

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
     unsigned int volt = 0; //16 bits
     char str[4];           // initialize
     
     volt = analogRead();
     
     sprintf(str,"%04d",volt*49/10); //Approximate conversion to 0-5V
      
     LCDGoto(0,1);
     isLeftBtnPressed();
     isRightBtnPressed();
     Delay10KTCYx(10);
        //delay(100);

     switch (state%NO_OF_STATES){
            case STANDARD:
                LCDPutChar(str[0]);
                LCDPutChar('.');
                LCDPutChar(str[1]);
                LCDPutChar(str[2]);
                LCDPutChar(str[3]);
                LCDPutChar('V');        
                break;
            case BAT_SAVER:
                LCDWriteStr("Battery Saver");
                break;
            case TEST_SER:
                LCDWriteStr("Connect2Serial");
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

    //Set up A/D on AN1    
    OSCCON=0b01010110; //set to 4 MHz (labA))   
    
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

int analogRead(void){
          //Start A/D Conversion
     int val = 0;
     ADCON0bits.GO = 1; 
     while (ADCON0bits.GO ==1){}; //GO bit automatically clears
     val = ADRESH;
     val=(val<<8) | ADRESL; //Math needs to be done in the int variable
     if(val==1023) //Fix roundoff error
        val=1022;
}
