#include "Sensor.h"

//*****************************************************************************
//                            CONSTANT DEFINITION
//*****************************************************************************

//*****************************************************************************
//                                SUPPORT MACROS
//*****************************************************************************


//*****************************************************************************
//                               SUPPORT FUNCTIONS
//*****************************************************************************

void analogInit(void){          //Set up analog sensors
    Sensors frontSensor;
    Sensors backSensor;

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

void motorInit(void){
    //Set up PWM
    TRISDbits.TRISD7 = 0;  //set PWM pin RD7 output
    T2CON = 0b00000111; // Prescale 1:16, timer on
}

int analogRead(int pin){        // Analog Read from pin (argument)
    ADCON0bits.CHS = pin;
    int val = 0;
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO == 1){}    //GO bit automatically clears
    val = ADRESH;
    val = (val << 8) | ADRESL;
    if (val == 1023)
        val = 1022;
    return val;
}
void sendPulse (int us){         // Send number of pulses
    int c = 0; int n = 0;
    for (c = 0; c < us; c++){
        for (n = 0; n < 10; n++){
            Nop();
        }
    }
}
//void sendPWM (int);             // Send duty cycle
