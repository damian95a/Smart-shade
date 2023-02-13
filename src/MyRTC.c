#include "MyRTC.h"
#include "MyUART.h"

volatile int minutes;
volatile int hours;

void initTimer1(){
	LPC_TIM1->PR=0; // without prescaler
	LPC_TIM1->MR0=60*25000000-1; // 1 MIN
	LPC_TIM1->MCR= 0b011; // with interrupt, reset timer on the match, don't stop timer at MR0

	LPC_TIM1->TCR=1; // enable timer
	NVIC_EnableIRQ(TIMER1_IRQn);
}

void initRTC(){
	
	initTimer1();
	LPC_SC->PCONP |= 1<<9; // PCRTC register - supply power to RTC (it's default on)

	minutes=get_minutes();
	hours=get_hours();
}

int get_hours(){
	return LPC_RTC->HOUR;
}

int get_minutes(){
	return LPC_RTC->MIN;
}

void set_hours(int h){
	hours=h;
	LPC_RTC->HOUR=h;
}

void set_minutes(int m){
	minutes=m;
	LPC_RTC->MIN=m;
}

void set_seconds(int s){
	LPC_RTC->SEC=s;
}

void TIMER1_IRQHandler(){ // update time
	LPC_TIM1->IR=1;
	hours=get_hours();
	minutes = get_minutes();

	print_int_UART(hours);
	printUART(":");
	print_int_UART(minutes);
	printUART("\n\r");
}
