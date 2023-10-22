//
// Created by petry on 27.09.2023.
//

#include "Capture_TIM3.h"


void Capture_TIM3() {
    RCC->APB1ENR = RCC_APB1ENR_TIM3EN; //Подача тактирования на таймер 3, без которого таймер не настраивается
    TIM3->CCMR1 = TIM_CCMR1_CC1S_0;//Настройка каналана input, соединение IC1 с TI1(CH1) Нога PA6
    TIM3->CCER = TIM_CCER_CC1E_Msk;//Enable capture from the counter
    TIM3->SR &= ~TIM_SR_CC1IF_Msk;// Снятие флага
    TIM3->DIER = TIM_DIER_CC1IE_Msk;//Включение запроса на прерывание от таймера 3
    TIM3->CR1 = TIM_CR1_CEN;//Включение таймера
    NVIC_EnableIRQ(TIM3_IRQn); // Разрешение передачи прерывания на АЛУ
}
typedef enum states_enum {
    BEGIN,
    CONTINUATION,
}states;
states states1  = BEGIN;
int date  = 0;
uint8_t result = 0;
int data_previous_ = 0;
void TIM3_IRQHandler(void) {
    if(TIM3->SR & TIM_SR_CC1IF_Msk) {
        switch (states1) {
            case BEGIN:
                date = TIM3->CCR1;
                states1 = CONTINUATION;
                TIM3->SR &= ~TIM_SR_CC1IF_Msk;// Снятие флага
                break;
            case CONTINUATION:
                data_previous_ = date;
                date = TIM3->CCR1;
                result = date - data_previous_;
                TIM3->SR &= ~TIM_SR_CC1IF_Msk;// Снятие флага
                break;
        }
    }
}
uint8_t return_value(){
    return result;
}