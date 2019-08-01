/* 
	Debug semihosting y newlibs
	target extended-remote localhost:3333
	monitor arm semihosting enable
	monitor reset halt
	load main.elf (if no specify already)
	continue
*/
#include <stdio.h>
#include "stm32f407xx.h"


int main(void)
{
	printf("Hello World\n");
	
	while(1);
}

