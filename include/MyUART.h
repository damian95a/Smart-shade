#ifndef MYUART_H
#define MYUART_H

#include "PIN_LPC17xx.h"      // Keil::Device:PIN
#include "LPC17xx.h"          // Device header  
#include <stdio.h>						// sprintf

void initUart(void);
void printUART(char* text);
void print_uint_UART(uint32_t v);
void print_int_UART(uint32_t v);

#endif
