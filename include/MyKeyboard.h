#ifndef MYKEYBOARD_H
#define MYKEYBOARD_H

#include "LPC17xx.h"                    // Device header
#include "PIN_LPC17xx.h"                // Keil::Device:PIN
#include <stdbool.h>

#include "MyUART.h"

#define OUTPUT_OFFSET 18
#define ONES (0b1111<<OUTPUT_OFFSET)
#define ZEROS (~ONES)

extern volatile char k; // saved value of pressed key
extern volatile bool keyboard; // if key was pressed
extern volatile uint32_t time; // systick time

/** 
 * called if interrupt set 'keyboard' as true
 * sets 'keyboard' as false
 * saves read key to 'k' variable
*/
void read_kb(void);

void wait_for_kb(void); // blockgin function to get value from keyboard
void initKeyboard(void);

/** 
 * keyboard interrupts
 * disables interrupts to be called only once on key pressed
*/
void EINT3_IRQHandler(void);
void SysTick_Handler(void);
void reenable(void); // reenables keyboard interrupts

#endif
