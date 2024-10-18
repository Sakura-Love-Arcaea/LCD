//
// LCD_keypad : 3x3 keypad input and display on LCD
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN  (LQPF-100)
#include <stdio.h>
#include <string.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Scankey.h"
char ipv4[16] = "               ";
char Text[16] = "keypad:         ";
int i = 0;

uint8_t keyin;

int key = 0;

int flag = 0;


void EINT1_IRQHandler(void)
{
	if (i >= 15) {
		return;
	}
	if (i == 0) {
					strcpy(ipv4, "                ");
				}
	GPIO_CLR_INT_FLAG(PB, BIT15);	
	ipv4[i] = '0';
	if ((i == 2 || i == 6 || i == 10)) {
					ipv4[i+1] = '.';
					i+=2;
	} else {
					i+=1;
	}
	print_Line(2, ipv4);
	
}

int main(void)
{
	
	
	SYS_Init();
	  
	init_LCD();
	clear_LCD();
	
	OpenKeyPad();	      
	
	GPIO_SetMode(PB, BIT14, GPIO_MODE_INPUT);
    GPIO_EnableEINT0(PB, 14, GPIO_INT_FALLING);
    NVIC_EnableIRQ(EINT0_IRQn);
	
	GPIO_SetMode(PB, BIT15, GPIO_MODE_INPUT);
    GPIO_EnableEINT1(PB, 15, GPIO_INT_RISING); // RISING, FALLING, BOTH_EDGE, HIGH, LOW
    NVIC_EnableIRQ(EINT1_IRQn);
	
	GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_64);
    GPIO_ENABLE_DEBOUNCE(PB, BIT14);
    GPIO_ENABLE_DEBOUNCE(PB, BIT15);
	

	// initialize 3x3 keypad
	print_Line(0,"IPv4 Checker"); // print title

	print_Line(2, ipv4);
	
	while(1)
	{
		
	  keyin = ScanKey();
		if (i >= 15) {
			if ((ipv4[0] ^ 48) * 100 + (ipv4[1] ^ 48) * 10 + (ipv4[2] ^ 48) > 255) {
					flag = 1;
			} else if ((ipv4[4] ^ 48) * 100 + (ipv4[5] ^ 48) * 10 + (ipv4[6] ^ 48) > 255) {
					flag = 1;
			} else if ((ipv4[8] ^ 48) * 100 + (ipv4[9] ^ 48) * 10 + (ipv4[10] ^ 48) > 255) {
				flag = 1;
			} else if ((ipv4[12] ^ 48) * 100 + (ipv4[13] ^ 48) * 10 + (ipv4[14] ^ 48) > 255) {
				flag = 1;
			}
			
			if (flag == 1) {
				print_Line(3, "                ");
				print_Line(3, "ERROR");
			} else {
				print_Line(3, "                ");
				print_Line(3, "OK");
			}
			
			i = 0;
			flag = 0;
			
			
			
			continue;
		}		
		
		if (keyin == key) 
		{
			continue;
		}
		else
		{
			key = keyin ;
			
			// input
			CLK_SysTickDelay(500000);
			if (key != 0) {
				if (i == 0) {
					strcpy(ipv4, "                ");
				}
				ipv4[i] = key ^ 48;
				if ((i == 2 || i == 6 || i == 10)) {
					ipv4[i+1] = '.';
					i+=2;
				} else {
					i+=1;
				}
			}
		}
		

	
		print_Line(2, ipv4);
		CLK_SysTickDelay(5000); 	   // delay 																	 
	}
}

