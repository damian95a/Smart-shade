#ifndef MENU_H
#define MENU_H

#include "MyRTC.h"
#include "MyLCD.h"
#include "MyKeyboard.h"
#include "shade.h"

#include <stdbool.h>
#include <stdio.h>

// adjust mode
enum mode {AUTO, MANUAL};
extern enum mode r_menu;

// priority of auto-adjust oprions
enum priority {FIRST, LAST, SENSOR, SCHED};
extern enum priority auto_adj;

// levels of light to be compared
extern short proc;
extern short down;
extern short tol; // acceptable mistake

// hours of schedule
extern short op_h;
extern short op_m;
extern short cl_h;
extern short cl_m;
extern bool cl_after;


void initSch(void); // initiate shcedule values
void menu_interface(void); // prints sutable menu page
void print_order(short choice, bool general); // prints menu of auto/manual-adjust
void automation(const char* txt, enum priority p, short choice, short line, bool general); // prints priority of auto-adjust options
void scroll_fun(short choice); // changes betweet auto- and manual-adjust
void read(short* value, const char* txt); // saves values from keyboard to a number
void enter_time(const char* head, const char* tail, short* h, short* m, short line); // saves values from keyboard to a time variable
void set_limits(void); // function called to set min and max range of shade

#endif
