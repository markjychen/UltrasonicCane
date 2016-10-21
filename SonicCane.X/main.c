/*
 * BME 464 - Lab A - Station 1
 *
 * Blink LEDs to draw a pattern as the board is shaken back and forth
 * Nick Bottenus - 8/23/12
 */

#include <p18f46k22.h>

#pragma config WDTEN=OFF
#pragma config FOSC=INTIO67
#pragma config XINST=OFF

void TMR0_ISR(void);
void SysInit(void);
void Tmr0Init(void);
void LEDInit(void);
void interruptEnabler(void);

char tmrh = 0x63;
char tmrl = 0xC0; //40 ms(2^16-4e-2/(1/(4e6/4)))
unsigned char pattern = 0b00001010;
int LEDflag = 0; //LED off;

//High priority interrupt
#pragma code InterruptVectorHigh = 0x08
#pragma interrupt High_Priority_ISR

void InterruptVectorHigh (void)
{
  _asm
    goto High_Priority_ISR
  _endasm
}
void High_Priority_ISR(void)
{
    TMR0_ISR(); //Call real-time clock service routine
}

void SysInit(void)
{
    OSCCON=0b01010110; //4 MHz internal oscillator

    //Set up buttons
    ANSELBbits.ANSB0=0; //Digital
    TRISAbits.RA4=1; //Input
    TRISBbits.RB0=1; //Input

    //Set up Tmr0
    Tmr0Init();
    LEDInit();
    interruptEnabler();
}

void LEDInit(void){
    //Set up LEDs
    ANSELB=0b00000000; //Digital IO
    LATB=0b00000000; //LEDs off
    TRISB=0b00000000; //LEDs are outputs
    LEDflag = 0;
}
void Tmr0Init(void){
    //Set up timer
    T0CONbits.T0CS=0; //Use internal clock (4 MHz/4)
    T0CONbits.T08BIT=0; //16 bit counter
    T0CONbits.PSA=1; //Don't use prescaler (1:1)
    T0CONbits.T0SE = 0;
    TMR0H=tmrh;
    TMR0L=tmrl;
}
void interruptEnabler(void){
    RCONbits.IPEN=1;            // Allow interrupt priorities
    INTCONbits.TMR0IF = 0;        // Clear any pending Timer 1 Interrupt indication
    INTCONbits.TMR0IE = 1;        // Enable Timer 1 Interrupt
    INTCONbits.GIE=1;           // Enable interrupts
}

//Called every second by the interrupt
void TMR0_ISR (void)
{
    if (INTCONbits.TMR0IF)            // If timer overflowed
    {

        //reset timer
        TMR0H=tmrh;
        TMR0L=tmrl;
        LATB = 0b00000010;            //Turn ON RB1
        INTCONbits.TMR0IF = 0;        // Clear timer flag
        INTCONbits.INT0IF = 0;      // Clear interrupt flag
    }
}

void main(void)
{
    SysInit();
    LCDClear();
    LCDGoto(0, 1);
    LCDWriteStr("Tmr0 LED RB1");
    while(1){
	       //LATB = 0b00000000; //LEDs off
    }
}
