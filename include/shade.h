#ifndef SHADE_H
#define SHADE_H

#include "MyLCD.h"
#include <stdbool.h>
#include "MyADC.h"

extern int num_of_step; // current state of shade
extern int MAX_STEPS; // limit of shede movement (the another one always is 0)
extern char prev_k;
extern bool night; // if night is detected

void initShade(void);
void adjust(void); // sets the position of shade in manual mode
bool open(void); // makes one step up - false if limit is reached
bool close(void); // makes one step down - false if limit is reached

// logical statements that detects day/night
bool sensor_open(void);
bool sensor_close(void);
bool sched_open(void);
bool sched_close(void);

void adjust_sensor(void); // sets the position of shade based on sensor measurements
void simple_adjust(void); // same as adjust but witchout loop

#endif
