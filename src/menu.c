#include "menu.h"

// options of main menu
enum page {MAIN, PROC_LEVEL, NIGHT_LEVEL, SCHEDULE, SET_TIME, ORDER, TOL, CALLIBRATE};
enum page menu_p = MAIN;

enum mode r_menu = MANUAL;

enum priority auto_adj = FIRST;

// text of priority menu
const char* const priors[] = {"Pierwsze zdarzenie", "Ostatnie zdarzenie", "Tylko czujnik", "Tylko harmonogram"};



// text of main menu
const char*const menu[]={"Ustaw docelowy poziom swiatla",
                        "Ustaw moment zasloniecia (noc)",
                        "Ustaw tolerancje czujnika",
                        "Ustaw harmonogram",
                        "Ustaw czas",
                        "Priorytet zdarzen",
                        "Kalibruj limity rolety"};

// variables containing current option in menu
short choice = 0;
short choice2= 0;
unsigned short size = sizeof(menu)/sizeof(char*);

short proc = 100;
short down = 0;
short tol = 10;

short op_h=7;
short op_m=0;
short cl_h=20;
short cl_m=0;
bool cl_after;

												
void initSch(){cl_after = (cl_h>op_h || cl_h==op_h && cl_m>op_m);}

// menu beg
void menu_interface(){
    if(menu_p == MAIN){
        switch(k){
        case '2':
            if(choice>0) choice -= 1;
            break;
        case '8':
            if(choice < size-1) choice += 1;
            break;
        case '6':
						k='\0';
						clear();
            switch(choice){
            case 0:
                menu_p = PROC_LEVEL;
                break;
            case 1:
                menu_p = NIGHT_LEVEL;
                break;
            case 2:
                menu_p = TOL;
                break;
            case 3:
                choice2=0;
                menu_p = SCHEDULE;
                break;
            case 4:
                menu_p = SET_TIME;
                break;
            case 5:
                choice2=0;
                menu_p = ORDER;
                break;
            case 6:
                menu_p = CALLIBRATE;
                break;
            }

            k='\0';

            return;
        }

        for(int i=0; i<size; ++i){
            printLCD(LINE_HEIGHT*i, 0, menu[i], i==choice);
        }

				k='\0';
        //wait_for_kb();
        return; // wait for keyboard without blocking
    } else if(menu_p == PROC_LEVEL){
        char txt[28];
        const char* head = "Poziom oswietlenia [%]: ";
        sprintf(txt, "%s%3d", head, proc);
        printLCD(0,0, txt, true);

        read(&proc, head);
        k='\0';
    } else if(menu_p == NIGHT_LEVEL){
        char txt[28];
        const char * head = "Moment zasloniecia [%]: ";
        sprintf(txt, "%s%3d", head, down);
        printLCD(0,0, txt, true);
        
        read(&down, head);
        k='\0';
    } else if(menu_p == SET_TIME){
        char txt[22];
        sprintf(txt, "Ustaw czas: %02d:%02d:00", hours, minutes);
        printLCD(0,0, txt, true);
				while(k=='\0')  wait_for_kb();
        if(k=='6'){
            short h=hours;
            short m=minutes;
            enter_time("Ustaw czas: ", ":00", &h, &m, 0);
            set_hours(h);
            set_minutes(m);
            set_seconds(0);
        } else if(k=='b'){
            menu_p=MAIN;
        }
        k='\0';
    } else if(menu_p == SCHEDULE){
        if(k != 'b'){
            if(choice2 == 0){ // highlighting chosen option
                char buff[22];
                sprintf(buff, "Odsloniecie: %02d:%02d", op_h, op_m);
                printLCD(0,0, buff, true);
                sprintf(buff, "Zasloniecie: %02d:%02d", cl_h, cl_m);
                printLCD(LINE_HEIGHT,0, buff, false);
            } else {
                char buff[22];
                sprintf(buff, "Odsloniecie: %02d:%02d", op_h, op_m);
                printLCD(0,0, buff, false);
                sprintf(buff, "Zasloniecie: %02d:%02d", cl_h, cl_m);
                printLCD(LINE_HEIGHT,0, buff, true);
            }

            //wait_for_kb();
        }

        if(k != '\0'){
            switch(k){
            case '2':
                if(choice2>0) choice2 -= 1; break;
            case '8':
                if(choice2 < 1) choice2 += 1; break;
            case '6':
								k='\0';
                if(choice2 == 0){
                    enter_time("Odsloniecie: ", "", &op_h, &op_m, 0);
                } else {
                    enter_time("Zasloniecie: ", "", &cl_h, &cl_m, 1);
                }

                cl_after = (cl_h>op_h || cl_h==op_h && cl_m>op_m);

                k='\0';

                return;
            case 'b':
                menu_p = MAIN;
                break;
            }
        }

        return; // wait for keyboard without blocking

    } else if(menu_p == ORDER){
        print_order(choice2, true);

        //wait_for_kb();

        if(k != '\0'){
            switch(k){
            case 'b':
                clear();
                menu_p = MAIN; 
						break;
            case '2':
                if(choice2 > 0) choice2 -=1; break;
            case '8':
                if(choice2 < 1) choice2 += 1; break;
            case '6':
                if(r_menu == MANUAL || choice2 == 0){
                    while(1){
                        wait_for_kb();
                        if(k == '2' || k == '8') {
                            r_menu = (enum mode)!r_menu;
                            prev_k='\0'; // stop moving shade
                        } else if (k == 'b') {
                            break;
                        }

                        print_order(choice2, false);
                    }
                } else {
                    scroll_fun(choice2);
                }
            }

            k='\0';
        }
    } else if(menu_p == TOL){
        char txt[28];
        const char* head = "Tolerancja [%]: ";
        sprintf(txt, "%s%3d", head, tol);
        printLCD(0,0, txt, true);

        read(&tol, head);
        k='\0';
    } else if(menu_p == CALLIBRATE){
        printLCD(0,0, "Nacisnij 6 aby zaczac kalibracje.", false);
        printLCD(LINE_HEIGHT, 0, "Nacisnij 'b', aby wyjsc.", false);
        printLCD(LINE_HEIGHT*2, 0, "C-w gore, D-w dol                       ", false); // fast clear
				
				k='\0';
        do{
            wait_for_kb();

            if(k=='6'){
                set_limits();
								break;
            } else if(k == 'b'){
                k='\0';
								menu_p = MAIN;
								break;
            }
        } while(k=='\0');
    }
        return; // wait for keyboard without blocking
    
}
//menu end


void set_limits(){
    short state=0;

    while(true){
				wait_for_kb();
			
        switch(state){
        case 0:
            printLCD(0,0, "Ustaw rolete w gornym polozeniu    ", false);
            printLCD(LINE_HEIGHT,0, "i zatwierdz przez 6.    ", false);
            printLCD(LINE_HEIGHT*2, 0, "                                       ", false); // fast clear

            k='\0';
            while(k!='6'){
                if(k=='b') {
                    num_of_step=0;
                    MAX_STEPS=0;
                    return;
                }

                num_of_step += 2;
								MAX_STEPS = num_of_step + 1;
                simple_adjust();
								reenable();
								if(keyboard) read_kb();
            }

            num_of_step=0;
			MAX_STEPS=0;
            ++state;
            break;
        case 1:
            printLCD(0,0, "Ustaw rolete w dolnym polozeniu   ", false);
			printLCD(LINE_HEIGHT,0, "i zatwierdz przez '6'.    ", false);
            printLCD(LINE_HEIGHT*2, 0, "Nacisnij 'f', aby cofnac.", false);

            k='\0';
            while(k!='6' && k!='f'){
                if(k=='b') {
                    num_of_step=0;
                    MAX_STEPS=0;
                    return;
                }

                MAX_STEPS += 1;
                simple_adjust();
                reenable();
                if(keyboard) read_kb();
            }

            if(k == 'f') {
                --state;
                num_of_step=0;
			    MAX_STEPS=0;
            } else {
                MAX_STEPS=num_of_step;
                ++state;
            }

            break;
        case 2:
            clear();
            printLCD(0,0, "Zapisano zmiany.", false);
            
            uint32_t waiting = time;
            while(time-waiting < 1000);
            return;
        }
    }
}

void print_order(short choice, bool general){
    bool first = (choice == 0 || r_menu == MANUAL);
    int len = printLCD(0,0, "Tryb: ", first && general);

    if(r_menu==MANUAL){
        printLCD(0, len*CHAR_WIDTH, "manual", first);
				printLCD(LINE_HEIGHT, 0, "                                       ", false); // fast clear
    } else {
        printLCD(0, len*CHAR_WIDTH, "auto  ", first);

        automation("Aktywacja: ", auto_adj, choice, 1, general);
    }
}

void automation(const char* txt, enum priority p, short choice, short line, bool general){
    int i = (int)p;

    int len = printLCD(LINE_HEIGHT*line, 0, txt, choice==line && general);
    printLCD(LINE_HEIGHT*line, len*CHAR_WIDTH, priors[i], choice==line);
}

void scroll_fun(short choice){

    while(1){
        wait_for_kb();

        // changing priority
        switch(k){
        case '2':
            if(auto_adj != FIRST) auto_adj -= 1; break;
        case '8':
            if(auto_adj != SCHED) auto_adj += 1; break;
        case 'b':
            return;
        }

        print_order(choice, false);
    }
}

void read(short* value, const char* txt){
    int tmp =0;
    bool changed=false;
    while(1){
        char buff[4];
        int txt_len = printLCD(0,0, txt, false);
		
        sprintf(buff, "%3d", *value);
        printLCD(0, txt_len*CHAR_WIDTH, buff, true);

				k='\0';
        wait_for_kb();

        switch(k){
        case 'b':
            menu_p = MAIN;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            changed = true;
            tmp *= 10;
            tmp += (int)(k-'0');
            break;
        }

        if(changed){
            if(tmp > 100) tmp = 100;
            else if(tmp < 0) tmp = 0;

            *value = tmp;
        }

        if(k=='b'){
			clear();
            return;
        }
    }
}

void enter_time(const char* head, const char* tail, short* h, short* m, short line){
    short tmp_h=0;
    short tmp_m=0;
    for(int i=0; i<=4; ++i){

        // reading several digits
            if(i==0){
                int len = printLCD(line*LINE_HEIGHT, 0, head, false);
                char buff[50];
                printLCD_char(line*LINE_HEIGHT, len*CHAR_WIDTH, (char)(*h/10 + '0'), true);
                sprintf(buff, "%d:%02d%s", *h%10, *m, tail);
                printLCD(line*LINE_HEIGHT, (len+1)*CHAR_WIDTH, buff, false);
            }
            else if(i==1) {
                char buff[50];
                sprintf(buff, "%s%d", head, *h/10);
                int len = printLCD(line*LINE_HEIGHT, 0,buff, false);
                printLCD_char(line*LINE_HEIGHT, len*CHAR_WIDTH, (char)(*h%10 + '0'), true);
                sprintf(buff, ":%02d%s", *m, tail);
                printLCD(line*LINE_HEIGHT, (len+1)*CHAR_WIDTH, buff, false);
            }
            else if(i==2){
                char buff[50];
                sprintf(buff, "%s%02d:", head, *h);
                int len = printLCD(line*LINE_HEIGHT, 0, buff, false);
                printLCD_char(line*LINE_HEIGHT, len*CHAR_WIDTH, (char)(*m/10 + '0'), true);
                sprintf(buff, "%d%s", *m%10, tail);
                printLCD(line*LINE_HEIGHT, (len+1)*CHAR_WIDTH, buff, false);
            }
            else if(i==3) {
                char buff[50];
                sprintf(buff, "%s%02d:%d", head, *h, *m/10);
                int len = printLCD(line*LINE_HEIGHT, 0, buff, false);
                printLCD_char(line*LINE_HEIGHT, len*CHAR_WIDTH, (char)(*m%10 + '0'), true);
                printLCD(line*LINE_HEIGHT, (len+1)*CHAR_WIDTH, tail, false);
            }
            else {
                //printf("%s%02d:%02d%s", head, *h, *m, tail);
                char buff[50];
                sprintf(buff, "%s%02d:%02d%s", head, *h, *m, tail);
                printLCD(line*LINE_HEIGHT, 0, buff, false);
            }

            wait_for_kb();


            if(k == 'f'){
                if(i==0) tmp_h=0;
                else{ // reading values
                    if(i==1) tmp_h=0;
                    else if(i==2) tmp_h /= 10;
                    else if(i==3) tmp_m=0;
                    else tmp_m /=10;
                }

                i-=2;
            } else {
                switch(k){
                case 'b':
                    return;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    if(i==0 || i==1){
                        tmp_h *= 10;
                        tmp_h += (int)(k-'0');

                        if(i==0){ // saving read value
                            *h = 10*tmp_h + *h%10;
                            if(*h>23) *h=23;
                        } else {
                            if(tmp_h>23) tmp_h=23;

                            *h=tmp_h;
                        }
                    } else if (i==2 || i==3) {
                        tmp_m *= 10;
                        tmp_m += (int)(k-'0');

                        if(i==2){ // saving read value
                            *m = 10*tmp_m + *m%10;
                            if(*m>=60) *m=59;
                        } else {
                            if(tmp_m>=60) tmp_m=59;

                            *m=tmp_m;
                        }
                    }
                    break;

                default:
                    i -= 1;
                }
            }
    }

    k='\0';
}
