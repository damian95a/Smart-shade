#include "MyADC.h"
#include "MyUART.h"

volatile bool unused[2]={false,false};
volatile double adv[2];

void ADC_IRQHandler(){
	uint32_t r = LPC_ADC->ADGDR;
	int i = (r>>24) & 0B111; // read channel index
	r = (r>>4) & ADC_BITS; // read 12 bits of voltage value
	adv[i-1] = 100.0F*r/(ADC_BITS-1.0F); // convert to %
	unused[i-1]=true; // to be printed
	if(unused[0] && unused[1])LPC_ADC->ADCR &= ~(1<<16); // setting 0 at BURST bit in register (stop conversions)
}

void initADC(){
	initTimer0();
	
	LPC_SC->PCONP |= 1<<12; // register PCADC (power control for ADC)
	LPC_ADC->ADCR |= 1<<21	// bit PDN in register (enable ADC)
								| 0b11<<1; // AD0.1 and AD0.2 sampling
								//| 0b010<<24; // start converstion at MAT0.1
								
	LPC_SC->PCLKSEL0 |= 0B11<<24; // periferial clock as CCLK/8 (bits PCLK_ADC in PCLKSEL0)
	// set P0.24 as ADC0.1 (function no 1) - without pullup/pulldown resistor and opend drain mode
	PIN_Configure(0,24, 0b01, 0b10, 0);
	PIN_Configure(0,25, 0b01, 0b10, 0); // set P0.25 as ADC0.2 -||-
	
	LPC_ADC->ADINTEN = 1<<8; // enable global interrupt
	NVIC_EnableIRQ(ADC_IRQn);
}

void initTimer0(){
	LPC_TIM0->PR=0; // without prescaler
	LPC_TIM0->MR0=2*25000000-1; // 2 sec
	LPC_TIM0->MCR= 0b011; // with interrupt, reset timer on the match, don't stop timer at MR0

	LPC_TIM0->TCR=1; // enable timer

	NVIC_EnableIRQ(TIMER0_IRQn);
}

void TIMER0_IRQHandler(){
	LPC_TIM0->IR=1;
	LPC_ADC->ADCR |= (1<<16); // start ADC conversions (bit BURST)
}
