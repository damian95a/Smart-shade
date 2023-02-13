#include "LPC17xx.h"                    // Device header
#include "PIN_LPC17xx.h"                // Keil::Device:PIN
#include <stdbool.h>

#include "MyUART.h"
#include "MyRTC.h"
#include "MyADC.h"
#include "MyKeyboard.h"
#include "menu.h"
#include "shade.h"

int main(void){
	initADC();
	initUart();
	printUART("START\n\r");
	initKeyboard();
	initSch();
	initShade();
	initRTC();
	
	hours=get_hours();
	minutes=get_minutes();
	initLCD();

	while(true){
		
		menu_interface();

		if(r_menu==AUTO) {
			if(!night){
				if(unused[0] && unused[1]){
					printUART("Values: ");
					print_uint_UART(adv[0]);
					printUART(", ");
					print_uint_UART(adv[1]);
					printUART("\n\r");
					adjust_sensor(); // steering according to light level
					unused[0]=unused[1]=false;
				}


				static bool previous_sen=true;
				static bool previous_sch=true;
			
				// night detection
				switch(auto_adj){
				case SENSOR:
					if(sensor_close()){ // outer sensor detects night
						while(close());
						night=true;
					}

					break;
				case SCHED:
					if(sched_close()){
						while(close());
						night=true;
					}

					break;
				case LAST:
					if(sched_close() && sensor_close()){
						while(close());
						night=true;
					}

					break;
				case FIRST:
					if(!previous_sch && sched_close() || !previous_sen && sensor_close()){  // if one of sources changed false->true
						while(close());
						night=true;
						previous_sch=true; // default true - that doesn't cause change from false
						previous_sen=true;
					} else {
						previous_sch=sched_close();
						previous_sen=sensor_close();
					}

					break;
				}
			} else { // day detection
				if(unused[0] && unused[1]){
					printUART("Values noc: ");
					print_uint_UART(adv[0]);
					printUART(", ");
					print_uint_UART(adv[1]);
					printUART("\n\r");
					
					unused[0] = unused[1]=false;
				}
				static bool previous_sen=true;
				static bool previous_sch=true;

				switch(auto_adj){
				case SENSOR:
					if(sensor_open()){ // outer sensor detects day
						while(open());
						night=false;
					}

					break;
				case SCHED:
					if(sched_open()){
						while(open());
						night=false;
					}

					break;
				case LAST:
					if(sched_open() && sensor_open()){
						while(open());
						night=false;
					}

					break;
				case FIRST:
					if(!previous_sch && sched_open() || !previous_sen && sensor_open()){ // if one of sources changed false->true
						while(open());
						night=false;
						previous_sch=true; // default true - that doesn't cause change from false
						previous_sen=true;
					} else {
						previous_sch=sched_open(); 
						previous_sen=sensor_open();
					}

					break;
				}
			}
		}

		if(keyboard){
			read_kb(); // check pressed key when interrupt
			if(k!='\0'){
				
				adjust(); // check steering in manual mode
				menu_interface();	// do an action at menu - sets k as '\0'
			}
		} else {
			reenable(); // enable interrupts after keyboard is read
		}
		
	}
}
