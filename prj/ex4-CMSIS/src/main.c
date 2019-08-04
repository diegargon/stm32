/**
	Diego Garcia Gonzalez (diegargon@gmail.com)
	CMSIS-ex4
*/

#include "stm32f407xx.h"

void EXTI3_IRQHandler() {
	if (EXTI->PR & EXTI_PR_PR3) { //Asegurarnos de que la flag esta puesta
		GPIOA->ODR ^= (1 << 7);
		EXTI->PR |= EXTI_PR_PR3; //Limpia flag pendiente
	}
}

int main(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  //LED
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; //BOTON
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; //Activamos SYSCFG System configuration controller clock
	SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI3_PE; //Configura la interupcion para el PIN 3 del PORTE

    GPIOA->MODER |= (1 << 14); //PIN7 AS OUTPUT    
    GPIOE->MODER &= ~(0xC0); // Pin3 como input es bit 7(0) y 8(0) (por defecto pero nos aseguramos)
    GPIOE->PUPDR |= GPIO_PUPDR_PUPDR3_0; //PULLUP
	
	EXTI->RTSR |= EXTI_RTSR_TR3;  // activar rising
	//EXTI->FTSR |= EXTI_FTSR_TR3;  // activar falling
	EXTI->IMR |= EXTI_IMR_MR3;  // habilitar interrupcion (unmask)
	EXTI->PR |= EXTI_PR_PR4; // Limpiamos el registro de pendiente
	
	NVIC->ISER[0] |= 1 << EXTI3_IRQn; //Activamos interrupcion en NVIC IRQ 9 por lo que esta en [0]
	
    while (1);
}
