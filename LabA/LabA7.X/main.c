/*
 * BME 464 - Lab A - Station 7
 *
 * Play music on the buzzer
 * Nick Bottenus - 8/23/12
 */

#include <p18f46k22.h>
#include <delays.h>
#include "Lcd.h"
#include "Sound.h"

#pragma config WDTEN=OFF
#pragma config FOSC=INTIO67
#pragma config XINST=OFF

void printNote(char PWM_Freq);
void makeMusic(void);

void main(void)
{
    OSCCON=0b01010110; //4 MHz internal oscillator

    //Set up LCD
    ANSELD = 0x00;
    TRISD = 0x00; //Digital out
    LCDInit();


    //Set up sound
    SNDInit();

    makeMusic();

    SNDSound(0,0);
    LCDClear();
    while(1){};
}

//Notes are not exact, sorry to any musicians
void printNote(char PWM_Freq)
{
    LCDGoto(0,0);
    LCDWriteStr("Note: ");
    switch (PWM_Freq)
    {
            case 10: 	LCDWriteStr("D8#");	break;
            case 15: 	LCDWriteStr("B7 ");	break;
            case 20: 	LCDWriteStr("F7#");	break;
            case 25: 	LCDWriteStr("D7#");	break;
            case 30: 	LCDWriteStr("C7 ");	break;
            case 35: 	LCDWriteStr("A6 ");	break;
            case 38: 	LCDWriteStr("G#6 ");	break;
            case 40: 	LCDWriteStr("G6 ");	break;
            case 45: 	LCDWriteStr("F6 ");	break;
            case 50: 	LCDWriteStr("D6#");	break;
            case 55: 	LCDWriteStr("D6 ");	break;
            case 60: 	LCDWriteStr("C6");	break;
            case 65: 	LCDWriteStr("A5#");	break;
            case 70: 	LCDWriteStr("A5 ");	break;
            case 75: 	LCDWriteStr("G5#");	break;
            case 80: 	LCDWriteStr("G5 ");	break;
            case 85: 	LCDWriteStr("F5#");	break;
            case 90: 	LCDWriteStr("F5 ");	break;
            case 95: 	LCDWriteStr("E5 ");	break;
            case 100: 	LCDWriteStr("D5#");	break;
            case 105: 	LCDWriteStr("D5 ");	break;
            case 110: 	LCDWriteStr("C5#");	break;
            case 115: 	LCDWriteStr("C5 ");	break;
            case 125: 	LCDWriteStr("B4 ");	break;
            case 135: 	LCDWriteStr("A4#");	break;
            case 140: 	LCDWriteStr("A4 ");	break;
            case 150: 	LCDWriteStr("G4#");	break;
            case 160: 	LCDWriteStr("G4 ");	break;
            case 170: 	LCDWriteStr("F4#");	break;
            case 180: 	LCDWriteStr("F4 ");	break;
            case 190: 	LCDWriteStr("E4 ");	break;
            case 200: 	LCDWriteStr("D4#");	break;
            case 210: 	LCDWriteStr("D4 ");	break;
            case 220: 	LCDWriteStr("C4#");	break;
            case 240: 	LCDWriteStr("C4 ");	break;
            case 255: 	LCDWriteStr("B3 ");	break;
            default: 	LCDWriteStr("---");	break;
    }
}

//BME 464L STUDENTS: ONLY EDIT THIS BLOCK OF CODE BELOW
void makeMusic(void)
{
    char notes[]={240,115,125,160,140,125,115}; //Somewhere over the rainbow
    char dur[]={100,100,50,25,25,50,50};
    char count=0;

    while(count<sizeof(notes))
    {
        printNote(notes[count]);
        SNDSound(1,notes[count]);
        Delay10KTCYx(dur[count++]); //100=1s, 50=.5s
    }
}