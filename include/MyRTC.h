#ifndef MYRTC_H
#define MYRTC_H

#include "LPC17xx.h"          // Device header 

// time saved from RTC
extern volatile int minutes;
extern volatile int hours;

void initTimer1(void);
void initRTC(void);
int get_hours(void);
int get_minutes(void);
void set_hours(int);
void set_minutes(int);
void set_seconds(int);
void TIMER1_IRQHandler(void); // saves current time

#endif
