#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

//#include <GenericTypeDefs.h>
#include <delays.h>
#include <p18f46k22.h>
#include <stdio.h>


#include "Lcd.h"
#include "General.h"

//*****************************************************************************
// Global Function Prototypes
//*****************************************************************************

void Pulse_ISR(void);
void High_Priority_ISR(void);

#endif