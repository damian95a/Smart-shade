#ifndef MYLCD_H
#define MYLCD_H

#include "LPC17xx.h"                    // Device header
#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "asciiLib.h"
#include <stdbool.h>

// text displaying parameters
#define LINE_HEIGHT 16
#define CHAR_WIDTH 8

void initLCD(void);
int printLCD(int x, int y, const char * txt, bool highlight);
void printLCD_char(int x, int y, char c, bool highlight);
void clear(void);

#endif
