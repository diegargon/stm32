/**
	Diego Garcia Gonzalez (diegargon@gmail.com)
	CMSIS-ex5 - RTC
*/

#include <stdio.h>
#include "stm32f407xx.h"

void Disable_RTCDOMAIN() {
	/* Habilita el PWR clock */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	/* DBP habilita el a acceso de escritura al Backup Domain*/		
	PWR->CR |= PWR_CR_DBP;	
}

void Reset_RTCDOMAIN() { 
	printf("Reseteando RTC domain\n");
	Disable_RTCDOMAIN();
    RCC->BDCR |= RCC_BDCR_BDRST;
    RCC->BDCR &= ~(RCC_BDCR_BDRST);
}

void Init_RTC() {
	printf("Configurando el oscilador de referencia\n");
	
	Disable_RTCDOMAIN();
	/* Reset LSE */
    //RCC->BDCR &= ~(RCC_BDCR_LSEON);
	//RCC->BDCR &= ~(RCC_BDCR_LSEBYP);		
	/* LSE on */		
	RCC->BDCR |= RCC_BDCR_LSEON;    
	while((RCC->BDCR & RCC_BDCR_LSERDY) != RCC_BDCR_LSERDY);
	 
	 /* Resetear la configuracion y cambiar a LSE */
	//RCC->BDCR &= ~(RCC_BDCR_RTCSEL); /* RESET */
	RCC->BDCR |= RCC_BDCR_RTCSEL_0; /* LSE Clock */
	
	//Pone en funcionamiento el reloj	
	RCC->BDCR |= RCC_BDCR_RTCEN;	
}

void Set_RTC() {
	printf("Configurando la fecha y hora actuales\n");
	
	Disable_RTCDOMAIN();
	/* Desactivamos proteccion contra escritura */ 
	RTC->WPR = 0xCA;   
	
	RTC->WPR = 0x53; 
	
	//Entrar en modo INIT
	RTC->ISR = 0;
	RTC->ISR |= RTC_ISR_INIT; 
	//WAIT  poll initf
	
	while((RTC->ISR & RTC_ISR_INITF)!=RTC_ISR_INITF);

  	/* Configurar preescaler p823 */
	RTC->PRER = 0x7f00ff; //valor por defecto tambien  

    //RTC->CR = (1 << 6); /* Formato 12h */
	RTC->CR = (0 << 6); /* Formato 24h */
    RTC->TR = 0x101200;  /* Hora 10:12:00 */
    RTC->DR = 0x190804; 

    RTC->DR |= (7 << 13); //Domingo
	//exit initialization mode
	RTC->ISR &=~ RTC_ISR_INIT;  

	/* Habilitamos la proteccion contra escritura */
	RTC->WPR = 0xFF;
}

int main(void)
{
	//Reset_RTCDOMAIN();
	Init_RTC();
	Set_RTC();

    while (1) {
		printf("Dia %lu, Mes  %lu , Ano %lu\n", 
						(((RTC->DR & RTC_DR_DT)  >> 4) * 10) + ((RTC->DR & RTC_DR_DU)  >> 0),
						(((RTC->DR & RTC_DR_MT) >> 12) * 10) + ((RTC->DR & RTC_DR_MU) >>  8),
						(((RTC->DR & RTC_DR_YT)  >>  20) * 10) + ((RTC->DR & RTC_DR_YU)  >>  16));
	
		printf("Horas %lu , Minutos %lu , Segundos %lu\n", 
						(((RTC->TR & RTC_TR_HT)  >> 20) * 10) + ((RTC->TR & RTC_TR_HU)  >> 16),
						(((RTC->TR & RTC_TR_MNT) >> 12) * 10) + ((RTC->TR & RTC_TR_MNU) >>  8),
						(((RTC->TR & RTC_TR_ST)  >>  4) * 10) + ((RTC->TR & RTC_TR_SU)  >>  0));
	} 
}
