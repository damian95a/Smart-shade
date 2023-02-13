#include "MyUART.h"

void initUart(){
	LPC_UART1->LCR=0x83;
	LPC_UART1->DLM=0;
	LPC_UART1->DLL=9;
	LPC_UART1->FDR=(2<<4)|1;
	LPC_UART1->LCR=3;
	
	PIN_Configure(0,15, 1, 0,0);
	PIN_Configure(0,16, 1, 0,0);
}

void printUART(char* text){
	int i=0;
	while(1){
		char tmp = text[i++];
		
		while(!(LPC_UART1->LSR & 1<<5));
		if(tmp==0){
			i=0;
			break;
		}else{
			LPC_UART1->THR=tmp;
		}
	}
}

void print_uint_UART(uint32_t v){
	char tmp[10];
	sprintf(tmp, "%u", v);
	printUART(tmp);
}

void print_int_UART(uint32_t v){
	char tmp[10];
	sprintf(tmp, "%d", v);
	printUART(tmp);
}
