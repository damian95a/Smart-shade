#include "shade.h"
#include "MyKeyboard.h"
#include "menu.h"

int num_of_step = 0;
int MAX_STEPS = 0;
char prev_k = '\0';
bool night=false;

bool sensor_open(){return (adv[1] >= down*(1.0 + (double)tol/100.0));}
bool sensor_close(){return (adv[1] <= down);}
bool sched_open(){return (hours>op_h || hours==op_h && minutes >= op_m) && (!cl_after || hours<cl_h || hours==cl_h && minutes<cl_m);} // if closing is later than opening check close time
bool sched_close(){return (hours>cl_h || hours==cl_h && minutes >= cl_m) && (cl_after || hours<op_h || hours==op_h && minutes<op_m);} // if openning is later than closing check open time

void adjust_sensor(){
    // to start moving shade need to exceed leved with tolerance
		
            print_int_UART(proc*(1.0 - (double)tol/100.0));
            printUART("\n\r");
			print_int_UART(adv[0]/adv[1] * 100.0f);
			printUART("\n\r");
    if(proc*(1.0 + (double)tol/100.0) <= adv[0]/adv[1] * 100.0f){
			
        while(unused[0] && unused[1] && proc <= adv[0]/adv[1] * 100.0f){ // close window until brighter than given level or until reach shade limit
					for(int i=0; i<10;++i) if(!close()) return;
					
					if(keyboard) read_kb();
					menu_interface();
					reenable();
					
					LPC_ADC->ADCR |= (1<<16); // start ADC conversions (bit BURST)
					unused[0] = unused[1]=false;
				}
    } else if(proc*(1.0 - (double)tol/100.0) >= adv[0]/adv[1] * 100.0f){
			
			
        while(unused[0] && unused[1] && proc >= adv[0]/adv[1] * 100.0f){ // open window until darker than given level or untile reach shade limit
					for(int i=0; i<10;++i) if(!open()) return;
					
					if(keyboard) read_kb();
					menu_interface();
					reenable();
					
					LPC_ADC->ADCR |= (1<<16); // start ADC conversions (bit BURST)
					unused[0] = unused[1]=false;
				}
    }
}

void initShade(){
    // P3.25 - STEP
    // P3.26 - DIR
    PIN_Configure(3,25, 0, 0b10,0); // without pulling rezistor
    PIN_Configure(3,26, 0, 0b10,0); // without pulling rezistor
    LPC_GPIO3->FIODIR=0B11 << 25; // P3.25 and P3.26 set as output
    LPC_GPIO3->FIOMASK &= ~(0B11 << 25); // set mask by zeros
}

void adjust(){
    if(r_menu == MANUAL){
        while(true){
            if(k != '\0'){
                prev_k = k;
            }

            if(prev_k != 'c' && prev_k != 'd') break;
            
            switch(prev_k){
            case 'c':
                if(!open()) return;
						break;
            case 'd':
                if(!close()) return;
						break;
            }
				
            reenable();
            if(keyboard) read_kb();
        }
    }
}

void simple_adjust(){
    if(k != '\0'){
        prev_k = k;
    }
    
    switch(prev_k){
    case 'c':
        open(); break;
    case 'd':
        close(); break;
    }
}

void step(){
    LPC_GPIO3->FIOSET = 1 << 25; // set HIGH

    uint32_t start_time = time;
    while(time-start_time < 10); // wait 10ms
    start_time=time;

    LPC_GPIO3->FIOCLR = 1 << 25; // set LOW
    while(time-start_time < 10); // wait 10ms
}

bool open(){
    LPC_GPIO3->FIOSET = 1 << 26; // rolling up
    if(num_of_step <= 0) return false; // terminate movement at the limit
    num_of_step -= 1;
    step();

    return true;
}

bool close(){
    LPC_GPIO3->FIOCLR = 1 << 26; // rolling down

    if(num_of_step >= MAX_STEPS) return false; // terminate movement at the limit
		
    num_of_step += 1;
    step();

    return true;
}
