//
// Created by petry on 18.10.2023.
//

#include "ADC.h"
#define SIZE_OF_BUFFER 20

void init_ADC(){
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;// ADC 1 interface clock enable
    ADC1->CR2 |= ADC_CR2_ADON;//Enable ADC

    //Настройка пинов
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //Включавем порт A GPIO
    GPIOA->CRL &= ~GPIO_CRL_MODE0; //Input mode (reset state) - нога PA0
    GPIOA->CRL &= ~GPIO_CRL_CNF0;//Analog mode

    ADC1->SQR1 = 0; // 1 регулярный канал
    ADC1->SQR3 = 0; // 1е преобразование - канал 0
    ADC1->CR2 |= ADC_CR2_CONT; // Continuous conversion mode
    ADC1->CR2 |= ADC_CR2_ADON; // Start conversion
    ADC1->CR1 |= ADC_CR1_EOCIE;// EOC interrupt enabled
    NVIC_EnableIRQ(ADC1_IRQn);
}


uint16_t buffer_ADC[SIZE_OF_BUFFER] = {0};
int i_ADC = 0;//счетчик строки для записи - хвост буфера
int j_ADC = 0;//счетчик-указатель строки для передачи - голова буфера

int ADC1_IRQHandler(){
    if(ADC1->SR & ADC_SR_EOC){
        //При разных взаиморасположениях указателей выявляются разные условия переполнения буффера
        if (i_ADC>j_ADC){
            if(i_ADC - j_ADC == SIZE_OF_BUFFER){
                return 0;
            }
        }
        else if (i_ADC<j_ADC){
            if (i_ADC + SIZE_OF_BUFFER - j_ADC + 1 == SIZE_OF_BUFFER){
                return 0;
            }
        }
        if (i_ADC<SIZE_OF_BUFFER){
            buffer_ADC[i_ADC] = ADC1->DR;
            i_ADC++;

        }
        else {
            i_ADC=0;
            buffer_ADC[i_ADC] = ADC1->DR;
            i_ADC++;
        }
    }
    return 1;
}


