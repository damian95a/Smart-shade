#include "MyLCD.h"
#include <string.h>

#define HS 20
#define VS 20

void initLCD(){
	lcdConfiguration();
	init_ILI9325();

	/*lcdWriteIndex(HADRPOS_RAM_START);
	lcdWriteData(0);
	
	lcdWriteIndex(VADRPOS_RAM_START);
	lcdWriteData(0);
	
	lcdWriteIndex(HADRPOS_RAM_END);
	lcdWriteData(LCD_MAX_X);
	
	lcdWriteIndex(VADRPOS_RAM_END);
	lcdWriteData(LCD_MAX_Y);
	
	lcdWriteIndex(DATA_RAM);*/
	
	lcdWriteIndex(ADRX_RAM);
	lcdWriteData(0);
	lcdWriteIndex(ADRY_RAM);
	lcdWriteData(0);

	clear();
}

int printLCD(int x, int y, const char * txt, bool highlight){
    unsigned int size = strlen(txt);

	int i;
    for(i=0; i<size; ++i){
        printLCD_char(x, y+i*CHAR_WIDTH, txt[i], highlight);
    }

	return i;
}

void printLCD_char(int x, int y, char c, bool highlight){
	long long bg_col;
	long long fg_col;


	x+=HS;
	x=LCD_MAX_X-x;
	y+=VS;

	if(!highlight){
		fg_col = LCDBlack;
		bg_col = LCDWhite;
	} else {
		fg_col = LCDBlack;
		bg_col = LCDBlue;
	}

	unsigned char buff[17];

	GetASCIICode(1, buff, (unsigned char)c);
	for(int i=0; i<LINE_HEIGHT; ++i){
		for(int j=0; j<CHAR_WIDTH; ++j){
			lcdWriteIndex(ADRX_RAM);
			lcdWriteData(x-i);
			lcdWriteIndex(ADRY_RAM);
			lcdWriteData(y+CHAR_WIDTH-j);
			
			lcdWriteIndex(DATA_RAM);
			
			long long color;
			if(buff[i]&1<<j) color=fg_col;
			else color=bg_col;
			
			lcdWriteData(color);
		}
	}
}

void clear(void){
	lcdWriteIndex(DATA_RAM);

	for(long i=0; i<LCD_MAX_X*LCD_MAX_Y*2; ++i){
		lcdWriteData(LCDWhite);
	}
}
