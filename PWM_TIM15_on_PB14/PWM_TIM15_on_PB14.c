//
// Created by petry on 28.09.2023.
//

#include "PWM_TIM15_on_PB14.h"
#include "stm32f1xx_hal.h"
#define PRESC       65536U
void Init_PWM_on_PB14_TIM15() {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN|RCC_APB2ENR_AFIOEN;// Включавем порт C GPIO, Включение альтернативной фуекции GPIO
    RCC->APB2RSTR |= RCC_APB2RSTR_IOPBRST;
    RCC->APB2RSTR &= ~RCC_APB2RSTR_IOPBRST;//Ресетим GPIO
    AFIO->MAPR2 |= AFIO_MAPR2_TIM15_REMAP;
    GPIOB->CRH &= ~GPIO_CRH_CNF14;
    GPIOB->CRH |= GPIO_CRH_MODE14_0|GPIO_CRH_CNF14_1;//Настраиваем ногу С4 на output

    //Создаем переменные для PWM
    uint32_t freq = HAL_RCC_GetSysClockFreq();
    uint16_t period_sec = freq / PRESC;

    uint16_t period = period_sec / 1;

    RCC->APB2ENR |= RCC_APB2ENR_TIM15EN; // Подача тактирования на таймер 12
    TIM15->ARR = period;
    TIM15->CCR1 = period/2;
//    TIM15->PSC = PRESC-1;
    TIM15->CCMR1 = TIM_CCMR1_OC1M_Msk | TIM_CCMR1_OC1PE; //Включаем ШИМ, идущий вначале в 1, потом в 0
    TIM15->CR1 = TIM_CR1_ARPE; //Активируем буффер
    TIM15->EGR = TIM_EGR_UG; //Искусственно создаем событие для загрузки значения в регистр
    TIM15->BDTR = TIM_BDTR_MOE;
    TIM15->CCER = TIM_CCER_CC1E; // Отправка сигнала на пин
    TIM15->CR1 = TIM_CR1_CEN;// Включение таймера

}

