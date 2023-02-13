#ifndef MYADC_H
#define MYADC_H

#include "LPC17xx.h"          // Device header 
#include "PIN_LPC17xx.h"      // Keil::Device:PIN
#include <stdbool.h>

#define ADC_BITS 0b111111111111

extern volatile double adv[2]; // % of max voltage - 1:outside, 0:inside
extern volatile bool unused[2]; // if new value of adv is ready to read

void ADC_IRQHandler(void);
void initADC(void);
void initTimer0(void); // timer to call ADC conversions
void TIMER0_IRQHandler(void);

#endif
