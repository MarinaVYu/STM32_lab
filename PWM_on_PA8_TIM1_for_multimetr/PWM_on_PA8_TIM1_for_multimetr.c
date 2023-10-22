//
// Created by petry on 19.09.2023.
//

#include "PWM_on_PA8_TIM1_for_multimetr.h"
#include "stm32f1xx_hal.h"
void Init_PWM_on_PA8_TIM1(){
    // Настраиваем выходную ногу
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN|RCC_APB2ENR_AFIOEN;//Включаем у GPIO port А, к которому относится нога PA8, включаем альтернативные функции IO
    RCC->APB2RSTR |= 1U << 2U;
    RCC->APB2RSTR &= ~(1U << 2U);//Ресетим GPIO
    GPIOA->CRH = GPIO_CRH_MODE8_0| GPIO_CRH_CNF8_1; // настраиваем на выход ногу A8 и настраиваем на ноге А8 альтернативную функцию

    //Переменные для настройки частоты
    uint32_t freq = HAL_RCC_GetSysClockFreq();
    uint16_t period = freq / 65536U;

    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // включение тактирования с шины apb2 на таймер 1
    TIM1->ARR = period;//Настройка counter на период
    TIM1->CCR1 = period / 2;//Настройка прерывания на половину периода таймера
//    TIM1->PSC = 65535U;
    TIM1->CCMR1 = TIM_CCMR1_OC1M_Msk|TIM_CCMR1_OC1PE;//Настраиваем ШИМ, идущий в начале в 1, потом в 0 и включение соответствующего регистра предварительной загрузки
    TIM1->CR1 = TIM_CR1_ARPE;
    TIM1->EGR = TIM_EGR_UG_Msk; // Генерация события переполнения для загрузки значения в counter
    TIM1->BDTR = TIM_BDTR_MOE_Msk;//Разрешаю включение каналов
    TIM1->CCER = TIM_CCER_CC1E_Msk;// Capture/Compare 1 output enable
    TIM1->CR1 |= TIM_CR1_CEN; // включение таймера

//Где бит MOE из отсутствующего регистра BDTR
// OC - output channel?

}