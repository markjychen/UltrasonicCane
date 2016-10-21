#include <PIC18F8722.h> // Processor specific
#include <pwm.h> // Located in the h folder of MCC18
#include <timers.h> // Located in the h folder of MCC18

#pragma config DEBUG = ON // Turn off if you do not have a compatible debugger ICD2?
#pragma config OSC = HS // HS assumes external High Speed crystal has been used
#pragma config WDT = OFF
#pragma config LVP = OFF

void main (void)
{

TRISC=0;

OpenTimer2(TIMER_INT_ON & T2_PS_1_1 & T2_POST_1_1);

OpenPWM1(0x3f); // Period - frequency
while(1)
{
SetDCPWM1(255);
}
}
