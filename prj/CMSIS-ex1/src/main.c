/* 
	Encendido de los dos leds de la placa
*/

#include "stm32f407xx.h"

int main(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
     
    GPIOA->MODER |= (1 << 14); 
    GPIOA->MODER |= (1 << 12);

    while (1);
}
