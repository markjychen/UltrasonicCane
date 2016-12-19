/*
 * BME 464 - Lab A - Station 5
 *
 * Use the buttons to scroll through messages, store most recent in EEPROM
 * Nick Bottenus - 8/23/12
 */

#include <p18f46k22.h>
#include <delays.h>
#include "Lcd.h"

#pragma config WDTEN=OFF
#pragma config FOSC=INTIO67
#pragma config XINST=OFF

void writeEEPROM(char b);
char readEEPROM(void);

void main(void)
{
    int count;

    OSCCON=0b01010110; //4 MHz internal oscillator

    //Set up LCD
    ANSELD = 0x00;
    TRISD = 0x00; //Digital out

    //Set up buttons
    TRISA = 0xFF;
    TRISB = 0xFF;
    ANSELA = 0x01;
    ANSELB = 0x00;

    //EEPROM
    count=readEEPROM();
    if(count<0||count>3)
        count=0;

    LCDInit(); //Start LCD

    while(1)
    {
       LCDGoto(0,0); //Line 1
       switch(count)
       {
           case 0: LCDWriteStr("Welcome to lab!"); break;
           case 1: LCDWriteStr("BME 464        "); break;
           case 2: LCDWriteStr("Go Blue Devils!"); break;
           case 3: LCDWriteStr("Engineers rock "); break;
       }
       if(PORTBbits.RB0==0 && count<3)
       {
           count++;
           writeEEPROM(count);
       }
       else if(PORTAbits.RA4==0 && count>0)
       {
           count--;
           writeEEPROM(count);
       }
        Delay10KTCYx(15); //Wait a little bit
    }
}

void writeEEPROM(char b)
{
    EEADR=0x00; //Address
    EEADRH=0x00;
    EEDATA=b; //Data to write
    EECON1bits.CFGS=0; //Set for memory
    EECON1bits.EEPGD=0; //Data eeprom
    INTCONbits.GIE=0; //Turn off interrupts
    EECON1bits.WREN=1; //Enable writes
    EECON2=0x55; //Required
    EECON2=0xAA; //Required
    EECON1bits.WR=1; //Start write
    while(PIR2bits.EEIF==0){};
    EECON1bits.WREN=0; //Disable writes
}

char readEEPROM(void)
{
    char data;
    EEADR=0x00; //Address
    EEADRH=0x00;
    EECON1bits.CFGS=0; //Set for memory
    EECON1bits.EEPGD=0; //Data eeprom
    EECON1bits.RD=1; //Read
    data=EEDATA;
    return data;
}

