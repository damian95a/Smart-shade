#include "MyKeyboard.h"

#define WAIT 5
#define DIV 1000

volatile char k='\0';
volatile bool keyboard=false;
volatile uint32_t time=0;
static const char kb[4][4] = {
	{'1', '2', '3', 'a'},
	{'4', '5', '6', 'b'},
	{'7', '8', '9', 'c'},
	{'0', 'f', 'e', 'd'}
};


void read_kb(){
    int row;
    for(row=0; row<=3; ++row){
        
        if(((LPC_GPIO0->FIOPIN >> row) & 1) == 0) break; // find row (causing interrupt)
    }

    // iteration over output pins (columns)
    int col=0;
    for(;col<=3; col++){
        LPC_GPIO1->FIOSET=ONES; // set all high
        LPC_GPIO1->FIOCLR=1<<(col+OUTPUT_OFFSET); // set low at one column
        uint32_t now = time;
        while(time-now<WAIT);
		
        if(((LPC_GPIO0->FIOPIN >> row) & 1) == 0) break; // serching pressed column
    }
    
    LPC_GPIO1->FIOCLR=ONES; // set all low

	if(row>=4 || col >=4) k='\0';
	else k = kb[row][col];
	
	// testing keyboard
	char txt[] = {k, '\n', '\r', '\0'};
	printUART(txt);
		
	
  	keyboard=false;
}

void reenable(){
	int row;
	for(row=0; row<=3; ++row){
        
        if(((LPC_GPIO0->FIOPIN >> row) & 1) == 0) break; // find row (causing interrupt)
	}
	
	if(row > 3)
		LPC_GPIOINT->IO0IntEnF = 0B1111; // enalbe interrupt (falling edge) at P0.0-3
}

void wait_for_kb(){
    while(!keyboard) reenable();
    read_kb();
}

void initKeyboard(){
	for(int i=18; i<=21; i++){
		PIN_Configure(1,i, 0, 0b10,0); // without pull-up/-down
	}
	
	for(int i=0; i<4; ++i){
		PIN_Configure(0,i, 0, 0b10,0); // without pull-up/-down
	}
	
	LPC_GPIO0->FIODIR &= ~(0B1111); // set pins P0.0-3 as input
	LPC_GPIO0->FIOMASK &= ~(0B1111); // enable pins P0.0-3 (by writting 0s)
	////
	LPC_GPIO1->FIODIR = ONES; // set pins P1.18/20/22/24 as output (one is out)
	LPC_GPIO1->FIOMASK &= ZEROS; // enable pins P1.18/20/22/24 (by writting 0s)
	LPC_GPIO1->FIOCLR = ONES; // set low lvl at output pins

    SysTick_Config(SystemCoreClock/DIV - 1); // timer to measure intervals 0.001 sec
	
	LPC_GPIOINT->IO0IntEnF = 0B1111; // enalbe interrupt (falling edge) at P0.0-3
	NVIC_EnableIRQ(EINT3_IRQn); // enable EINT3 interrupts (GPIO)
}

void EINT3_IRQHandler(){
	LPC_GPIOINT->IO0IntEnF=0; // temporarily disable interrupts
	LPC_GPIOINT->IO0IntClr=0xFF; // clear interrupts
	keyboard=true;
}

void SysTick_Handler(){
    time += 1;
}
