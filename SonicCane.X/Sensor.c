#include "Sensor.h"
#include "System.h"

//*****************************************************************************
//                            CONSTANT DEFINITION
//*****************************************************************************

//*****************************************************************************
//                                SUPPORT MACROS
//*****************************************************************************

/*#ifndef LOWBYTE(v)
#define LOWBYTE(v)  ((unsigned char) (v))
#ifndef HIGHBYTE(v)
#define HIGHBYTE(v) ((unsigned char) (((unsigned int) (v)) >> 8))*/

//*****************************************************************************
//                               SUPPORT FUNCTIONS
//*****************************************************************************

void analogInit(void){          //Set up analog sensors
    struct Sensors frontSensor;
    struct Sensors backSensor;

    ADCON2bits.ACQT = 001;      // 2 TAD (labA)
    ADCON2bits.ADFM = 1;        //Right Justified (labA)
    ADCON2bits.ADCS = 010;      //Fosc / 32
    ADCON0bits.ADON = 0x01;     //Enable A/D module

    TRISAbits.RA1 = 1;          //Turn RA1 to input
    ANSELAbits.ANSA1 = 1;
    frontSensor.pin = 0b0001;

    TRISAbits.RA2 = 1;          //Turn RA2 to input
    ANSELAbits.ANSA2 = 1;
    backSensor.pin = 0b0010;

}

void pulseInit(void){
    LATB = 0b00000000;
    ANSELBbits.ANSB3 = 0;
    TRISBbits.RB3 = 0;          //if 1, input
    ANSELBbits.ANSB5 = 0;
    TRISBbits.RB5 = 0;
}


int analogRead(unsigned char pin){        // Analog Read from pin (argument)
    int val = 0;
    //pin = 2;
    //LCDPutByte(pin);
    //pin = 1;
    //LCDPutByte(pin);
    ADCON0bits.CHS = pin;
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO == 1){}    //GO bit automatically clears
    val = ADRESH;
    val = (val << 8) | ADRESL;
    if (val == 1023)
        val = 1022;
    return val;
}
void sendPulse (int us){         // Send number of pulses
    //int c = 0; int n = 0;
    LATBbits.LATB3 = 1;
    //for (c = 0; c < us; c++){
    //    Nop(); Nop(); Nop(); 
    //}
    /*Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop();
    Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop();
    Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop();
    Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop();*/
    
    delayMillisecond(us);

    LATBbits.LATB3 = 0;

}
void sendPWM (int val){
    motorInit();
    TRISCbits.TRISC2 = 0;  //set PWM pin RC1 output  //hmm P1C
    PR2 = 249;          // Timer2 period register = 250 counts //DC?
    
    CCPR1L = val;      // The 8 most sig bits of the period are 0x7D     
                        // DC% = CCPR1L = % * PR2

}  

void stopPWM(){
    CCPR1L = 0;
    //TRISCbits.TRISC2 = 1;
    CCP1CONbits.CCP1M = 000;
}
             // Send duty cycle


void motorInit(void){
    //Set up PWM
    TRISCbits.TRISC2 = 1;
    CCP1CON = 0b01001100; // The 2 LSbs are 0b00, and CCP1Mz = 110 for PWM
    CCP1CONbits.CCP1M0 = 0; //unused
    CCP1CONbits.CCP1M1 = 0; //unused
    CCP1CONbits.CCP1M2 = 1; //need for pwm
    CCP1CONbits.CCP1M3 = 1; // need for pwm
    CCP1CONbits.DC1B0 = 0;  // lower byte
    CCP1CONbits.DC1B1 = 0;  // upper byte
    CCP1CONbits.P1M0 = 0;   // unused???... unimplemented -> set 00
    CCP1CONbits.P1M1 = 0;   // 
    T2CON = 0b00000111; // Prescale 1:16, timer on
}

void headMotorInit(void){
    TRISDbits.RD5 = 0;
    ANSELDbits.ANSD5 = 0;
    LATDbits.LATD5 = 0;
}
void sendHeadWarning(int ms){
    LATDbits.LATD5 = 1;
    delayMillisecond (100);
    LATDbits.LATD5 = 0;
}