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

#define STANDARD 0
#define PWM_DEMO 1
#define PULSE 2
#define NO_OF_STATES 3
#define TMRL 0x58
#define TMRH 0x9E
int state = 0;
unsigned char patterns[] = {0b0001010, 0b0000000};

//List of Necessary Functions
void SysInit(void);
unsigned char isLeftBtnPressed(void);
unsigned char isRightBtnPressed(void);
unsigned char isBtnPressed(void);
int analogRead0(void);
void sendPulse(int);
void sendPWM(int);


void main(void)
{
    unsigned int volt = 0; 
   
    SysInit();
    LCDClear();
    LCDGoto(0, 0);
    LCDWriteStr("Hello World!");
    printf("Hello!\n");

    while(1){
        // Set up variables
        unsigned int volt = 0; //16 bits
        unsigned int lvl = 0;
        char str[4];
        char lvl_str[4];

        //Start A/D Conversion
        volt = analogRead0();
        lvl = (volt*4/1023)%4;
        sprintf(str, "%04d", volt * 49 / 10); //Approximate conversion to 0-5V
        sprintf(lvl_str, "%04d", lvl);
        
        LCDGoto(0, 1);
        isLeftBtnPressed();
        isRightBtnPressed();
        //isBtnPressed();
        Delay10KTCYx(10);
        //delay(100);

         switch (state%NO_OF_STATES) {
            case STANDARD :
                LCDPutChar(str[0]);
                LCDPutChar('.');
                LCDPutChar(str[1]);
                LCDPutChar(str[2]);
                LCDPutChar(str[3]);
                LCDPutChar('V'); 
                break;
            case PWM_DEMO:
                LCDGoto(0, 0);
                LCDWriteStr("        PWM Demo");
                LCDGoto(0, 1);
                LCDPutChar(lvl_str[0]);
                LCDPutChar(lvl_str[1]);
                LCDPutChar(lvl_str[2]);
                LCDPutChar(lvl_str[3]);
                sendPWM(lvl);
                break;
            case PULSE:
                LCDWriteStr("Pulse");
                //sendPulse(2);
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
    
    //Set up buttons
    ANSELBbits.ANSB0=0; //Digital
    TRISAbits.RA4=1; //Input
    TRISBbits.RB0=1; //Input
    
    //Set up LEDs
    ANSELB=0b00000000; //Digital IO
    LATB=0b00000000; //LEDs off
    TRISB=0b00000001; //LEDs are outputs    //RB0 is tied to the button too

     //Set up A/D on AN0
    /*ANSELAbits.ANSA0 = 1;
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
    ADCON0bits.CHS=0001; //Select RA1*/
    
    // from states.c
    //Set up RA1 for analog read
    TRISAbits.RA1=1; //Input  NEED THIS
    ANSELAbits.ANSA0 = 1;
    ADCON1 = 0b00001110;//VSS,VDD ref. AN0 analog only
	ADCON2 = 0b00001000;//ADCON2 setup: Left justified, Tacq=2Tad, Tad=2*Tosc (or Fosc/2)
    ADCON2bits.ACQT=001; //2 TAD (labA))
    ADCON2bits.ADFM=1; //Right justified (labA))
    ADCON2bits.ACQT=001; //2 TAD
    ADCON2bits.ADCS=010; //FOSC/32
    ADCON0 = 0b10001010;//clear ADCON0 to select channel 0 (AN0)
	ADCON0bits.ADON = 0x01;//Enable A/D module
    ADCON0bits.CHS=0001; //Select RA1
    
    //Set up RA0 for potentiometer read
    //Set up A/D on AN0
    ANSELAbits.ANSA0 = 1;
    TRISAbits.RA0 = 1; //Analog in
    ADCON2bits.ACQT=001; //2 TAD
    ADCON2bits.ADCS=010; //FOSC/32
    ADCON2bits.ADFM=1; //Left justified
    ADCON0bits.ADON=1; //Turn on A/D
    ADCON0bits.CHS = 0000;

    
    //Set up button on RC2
    //ANSELCbits.ANSC2=0; //Digital
    //TRISCbits.RC2=0; //Input

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

int analogRead0(void){
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

unsigned char isLeftBtnPressed(void){
  if (PORTAbits.RA4 == 0){
      Delay10KTCYx(10);
      state=0;
      sendPulse(2);
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

/*unsigned char isBtnPressed(void){
    if (PORTCbits.RC2 == 0){
      Delay10KTCYx(10);
      //LCDGoto(0,1);
      //LCDWriteStr("                ");
      //LCDGoto(0,1);
      sendPulse(2);
      return 1;
 }
 return 0;
}*/

void sendPulse(int n) {
    int count = 0;
    int dir = 1;
    int m = 0;
    while (m<4) {
        while (INTCONbits.TMR0IF == 0) {}
        INTCONbits.TMR0IF = 0; //Reset flag
        TMR0H = TMRH;
        TMR0L = TMRL;

        count += dir; //Increment counter
        if (count == 0 || count == sizeof (patterns) - 1){
            dir *= -1;
            m++;
        }
        LATB = patterns[count]; //Light LEDs
    }
    LATB = patterns[1];
}

void sendPWM(int lvl){
    unsigned char dc80[] = {0b0001010, 0b0001010,0b0001010,0b0001010, 0b0000000};
    unsigned char dc60[] = {0b0001010,0b0001010,0b0001010, 0b0000000, 0b0000000};
    unsigned char dc40[] = {0b0001010, 0b0001010, 0b0000000, 0b0000000, 0b0000000};
    unsigned char dc20[] = {0b0001010, 0b0000000,0b0000000,0b0000000,0b0000000};
    
    char count=0;
    char dir = 1;
    char tmrl = 0x58;
    char tmrh = 0x9E;
    
    
    unsigned char* patterns;
    
    switch (lvl){
         case 0 : patterns = dc20; break;
         case 1 : patterns = dc40; break;
         case 2 : patterns = dc60; break;
         case 3 : patterns = dc80; break;
         default : break; 
    }
    INTCONbits.TMR0IF=0; //Reset flag
    TMR0H=tmrh;
    TMR0L=tmrl;

    count+=dir; //Increment counter
    if(count==0||count==sizeof(patterns)-1)
        dir*=-1;
    LATB=patterns[count]; //Light LEDs
    
}