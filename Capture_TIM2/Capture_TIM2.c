//
// Created by petry on 25.09.2023.
//

#include "Capture_TIM2.h"
#include "stm32f1xx_hal.h"
void Capture_TIM2_Init() {
    RCC->APB1ENR = RCC_APB1ENR_TIM2EN; //Подача тактирования на таймер 2
    TIM2->CCMR1 = TIM_CCMR1_CC1S_0; //CC1 channel is configured as input, IC1 is mapped on TI1
    TIM2->CCER = TIM_CCER_CC1E; //Разрешение захвата из счетчика в регистр захвата TIMx_CCR1
    TIM2->SR &= ~TIM_SR_CC1IF_Msk; //снятие флага
    TIM2->DIER = TIM_DIER_CC1IE;//Enable the related interrupt request
    TIM2->CR1 = TIM_CR1_CEN;//Включаем таймер
    NVIC_EnableIRQ(TIM2_IRQn); //Включаем прерывание от TIM2

}
typedef enum States_enum {
    BEGIN,
    CONTINUATION,
} States;
States states = BEGIN;
uint16_t data = 0;
uint16_t data_previous = 0;
uint16_t frequency = 0;
void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_CC1IF_Msk) {
        switch (states) {
            case BEGIN:
                data = TIM2->CCR1; //запись числа таймера, на котором произошло прерывание
                TIM2->SR &= ~TIM_SR_CC1IF_Msk; //снятие флага
                states = CONTINUATION;
                break;
            case CONTINUATION:
                data_previous = data;
                data = TIM2->CCR1;
                TIM2->SR &= ~TIM_SR_CC1IF_Msk; //снятие флага
                frequency = data - data_previous; // Вычисление частоты входящего сигнала
                break;
        }
    }
}


