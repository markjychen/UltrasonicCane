#define USE_OR_MASKS
#include <p18cxxx.h>
#include "pwm.h"

//http://www.microchip.com/forums/m506808.aspx

void main(void)
{
  char period=0x00;
  unsigned char outputconfig=0,outputmode=0,config=0;
  unsigned int duty_cycle=0;

//----Configure pwm ----
    period = 0xFF;
    OpenPWM1( period);            //Configure PWM module and initialize PWM period

//-----set duty cycle----
        duty_cycle = 0x0F00;
        SetDCPWM1(duty_cycle);        //set the duty cycle

//----set pwm output----
    outputconfig = FULL_OUT_FWD ;
    outputmode = PWM_MODE_1;
    SetOutputPWM1( outputconfig, outputmode);    //output PWM in respective modes

    while(1);                    //observe output on CCP1 pin

//-----close pwm----
  ClosePWM1();

}