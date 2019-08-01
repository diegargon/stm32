/**
	CMSIS-ex2
*/
#include "stm32f407xx.h"

void TIM2_IRQHandler(void) {
   if (TIM2->SR & TIM_SR_UIF) {	
	GPIOA->ODR ^= (1 << 7); //PIN7
  }
   
  TIM2->SR = 0x0;
}
 
int main(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
     
    GPIOA->MODER |= (1 << 14); //PIN7
    
    NVIC->ISER[0] |= 1<< (TIM2_IRQn);
     
    TIM2->PSC = 0x0;
    TIM2->DIER |= TIM_DIER_UIE; 
	TIM2->ARR = 0xF42400 -1;    
	TIM2->CR1 |= TIM_CR1_CEN; 
    TIM2->EGR = 1;
     
    while (1);
}
