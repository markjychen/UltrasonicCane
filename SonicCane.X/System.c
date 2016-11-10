/********************************************************************
*
*
*
*********************************************************************
* FileName:        Lcd.c
* Dependencies:    See INCLUDES section below
* Processor: 	   PIC18F46K22
* Compiler: 	   C-18
* Company:         Microchip Technology, Inc.

*********************************************************************
* File Description: This modules includes all service functions for LCD
*                   operations.
*
* Change History:
* Author               Date        Comment
* Chris Tucker      10/18/2010  First version of module code
********************************************************************/

/**
*   @file Lcd.c
*   @brief This modules includes all service functions for LCD
*          operations.
*   @defgroup LCD	LCD Display
*
*/

//*****************************************************************************
// Include and Header files
//*****************************************************************************

#include "System.h"
#include "Lcd.h"
#include "Sensor.h"
//*****************************************************************************
//                            CONSTANT DEFINITION
//*****************************************************************************
#define TMRL_1ms 0x17;
#define TMRH_1ms 0xFC;

#define windowSize 5;
int total = 0; 
int readings[5];
//int readings[windowSize] = {0,0,0,0,0};
//*****************************************************************************
//                                SUPPORT MACROS
//*****************************************************************************

//*****************************************************************************
//                               SUPPORT FUNCTIONS
//*****************************************************************************

void SysInit(void){
    OSCCON=0b01010110; //4 MHz internal oscillator
    LCDDisplayInit();
    LCDInit();
    buttonInit();
    analogInit();
    LEDInit();
    pulseInit();
    Tmr0Init();
    motorInit();
}
void buttonInit(void){          // Set up buttons
    ANSELBbits.ANSB0=0; //Digital
    ANSELAbits.ANSA0=0; //don't I need this?
    TRISAbits.RA4=1; //Input
    TRISBbits.RB0=1; //Input

    //Set up button on RB1
    ANSELBbits.ANSB1=0; //Digital
    TRISBbits.RB1=1; //Input

}

void LCDDisplayInit(void){
    ANSELD = 0x00;
    TRISD = 0x00;
}

void LEDInit(void){
    ANSELBbits.ANSB3 = 0; //Digital IO
    ANSELBbits.ANSB1 = 0;
    LATB=0b00000000; //LEDs off
    TRISBbits.RB3 = 0; //LEDs are outputs    //RB0 is tied to the button too
    //TRISBbits.RB1 = 0;
    ANSELBbits.ANSB5 = 0;
    TRISBbits.RB5 = 0;

    TRISAbits.RA3 = 0;
    TRISAbits.RA5 = 0;
    ANSELAbits.ANSA5 = 0;
    ANSELAbits.ANSA3 = 0;
    LATAbits.LATA5 = 0;
    LATAbits.LATA3 = 0;
}

void potentiometerInit(void){
    ANSELAbits.ANSA0 = 1;
    TRISAbits.RA0 = 1; //Analog in
    ADCON2bits.ACQT=001; //2 TAD
    ADCON2bits.ADCS=010; //FOSC/32
    ADCON2bits.ADFM=1; //Left justified
    ADCON0bits.ADON=1; //Turn on A/D
    ADCON0bits.CHS = 0000;
}

void Tmr0Init(void){
    T0CONbits.T0CS = 0;     //use internal clock (4MHz/ 4)
    T0CONbits.T08BIT=0; //16 bit counter
    T0CONbits.PSA=1; //Don't use prescaler (1:1)}
}

void delayMillisecond(int num_ms){
    int count = 0;
    for (count = 0; count < num_ms; count++){
        while (INTCONbits.TMR0IF == 0) {}
        INTCONbits.TMR0IF = 0;      //reset flag
        TMR0H = TMRH_1ms;
        TMR0L = TMRL_1ms;
    }
}

int isLeftBtnPressed(void){
  if (PORTAbits.RA4 == 0){
      Delay10KTCYx(10);
      return 1;
 }
 return 0;
}

int isRightBtnPressed(void){
  if (PORTBbits.RB0 == 0){
      Delay10KTCYx(10);
      return 1;
 }
 return 0;
}

int isBtnPressed(void){
    if (PORTBbits.RB1 == 1){
        Delay10KTCYx(10);
      return 1;
    }
 return 0;
}
void enableSleep(){
    OSCCONbits.IDLEN = 0;
    INTCON3bits.INT1IE = 1;
    if (1==0){ //enableWDT
        WDTCONbits.SWDTEN = 1;
    }
    Sleep();
    INTCON3bits.INT1F = 0;
    WDTCONbits.SWDTEN = 0;
    //LCDWriteStr("Awake!          ");
}
int smooth(int data, float filterVal, float smoothedVal){
//Need to have global int sensVal // for raw sensor values...
//Need global float filterVal //to determine smoothness 0.0001 is max, 1 is off (no smoothing)
//Need global float smoothedVal //this holds the last loop value (use unique variable for every different sensor)

    if (filterVal > 1){         // check to make sure params in range
        filterVal = 0.99;
    } else if (filterVal <= 0){
        filterVal = 0.001;
    }

    smoothedVal = (data * (1 - filterVal)) + (smoothedVal * filterVal);

    return (int)smoothedVal;
}

int boxcar_filter(int data, int readIndex){
    int average = 0;
    total = total - readings[readIndex];
    readings[readIndex] = data;
    total = total + readings[readIndex];
    readIndex = readIndex % windowSize;
    average = total / windowSize;
    if (average == 0){
        average = 0;
    }
    return average;
}
    
