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

#pragma config WDTEN = SWON
#pragma config WDTPS = 512
#pragma config FOSC = INTIO67
#pragma config XINST = OFF

#define LOWBYTE(v)   ((unsigned char) (v))
#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (v)) >> 8))

#define STANDARD 0
#define PWM_DEMO 1
#define PULSE 2
#define ADC 3
#define SLEEP 4
#define NO_OF_STATES 5
#define TMRL 0x58
#define TMRH 0x9E
int state = 0;

int btnPress = 0;
unsigned char patterns[] = {0b0001010, 0b0000000};

//List of Necessary Functions
void SysInit(void);
unsigned char isLeftBtnPressed(void);
unsigned char isRightBtnPressed(void);
unsigned char isBtnPressed(void);
int analogRead0(void);
void sendPulse(int);
void sendPWM(int);
int potLvl(void);
void enableSleep(void);
//void wakeUp(void);

/*void High_Priority_ISR(void);

#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void){
    _asm
        goto High_Priority_ISR
    _endasm
}

#pragma interrupt High_Priority_ISR
void High_Priority_ISR(void){
    wakeUp();
}*/

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
        char str[4]; 
        char per[4];

        //Start A/D Conversion
        volt = analogRead0();
        sprintf(str, "%04d", volt * 49 / 10); //Approximate conversion to 0-5V
        
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
                //LCDGoto(0, 0);
                LCDWriteStr("PWM Demo        ");
                
                sprintf(per, "%02d", volt);
                LCDGoto (9, 1);
                LCDPutChar(per[0]);
                LCDPutChar(per[1]);
                LCDPutChar(per[2]);
                LCDPutChar(per[3]);
                
                //sendPWM(volt);
                break;
            case PULSE:
                LCDWriteStr("Pulse");
                LCDPutByte(btnPress);
                if(isBtnPressed()){
                    LATBbits.LATB1 = 1; //this is correct...
                    sendPulse(1);
                    //btnPress++;
                }else{
                    LATBbits.LATB1 = 0;
                }
                break;
            case SLEEP:
                LCDWriteStr("Sleep");
                enableSleep();
                break;
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
    ANSELBbits.ANSB3 = 0; //Digital IO
    ANSELBbits.ANSB1 = 0;
    LATB=0b00000000; //LEDs off
    TRISBbits.RB3 = 0; //LEDs are outputs    //RB0 is tied to the button too
    //TRISBbits.RB1 = 0;
    ANSELBbits.ANSB5 = 0;
    TRISBbits.RB5 = 0;
    
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

    
    //Set up button on RB1
    ANSELBbits.ANSB1=0; //Digital
    TRISBbits.RB1=1; //Input

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
    
    //Set up PWM
    TRISDbits.TRISD7 = 0;  //set PWM pin RD7 output 
    T2CON = 0b00000111; // Prescale 1:16, timer on
    
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

unsigned char isBtnPressed(void){
    if (PORTBbits.RB1 == 1){
        LATBbits.LATB5 = 1;
        Delay10KTCYx(10);
        LATBbits.LATB5 = 0;
      //LCDGoto(0,1);
      //LCDWriteStr("                ");
      //LCDGoto(0,1);
      //sendPulse(2);
      return 1;
    }
 return 0;
}

void sendPulse(int num_of_pulse) {
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

int potLvl(void){
    int lvl = 0; 
    char lvl_str[4];
    int volt = analogRead0();
    lvl = (volt*4/1023)%4;
    sprintf(lvl_str, "%04d", lvl);
    LCDGoto(0, 1);
    LCDPutChar(lvl_str[0]);
    LCDPutChar(lvl_str[1]);
    LCDPutChar(lvl_str[2]);
    LCDPutChar(lvl_str[3]);
    return lvl;
}

void sendPWM(int DC){

    //DC = 90;
    PR2 = 249;          // Timer2 period register = 250 counts //DC?
    CCPR1L = HIGHBYTE(DC);//125;//0xBF;      // The 8 most sig bits of the period are 0x7D     
                        // DC% = CCPR1L = % * PR2
    CCP1CON = 0b01001100; // The 2 LSbs are 0b00, and CCP1Mz = 110 for PWM
}

void enableSleep(void){
    //OSCCONbits.IDLEN = 0;
    //WDTCONbits.SWDTEN = 1;
    INTCON3bits.INT1IE = 1;
    Sleep();
    INTCON3bits.INT1F = 0;
    state = 0;
}