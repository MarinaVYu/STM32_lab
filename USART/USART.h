//
// Created by petry on 09.10.2023.
//

#ifndef TEST_USART_H
#define TEST_USART_H
#include "stm32f1xx_hal.h"
void init_usart1();
void init_TIM4();
int write_to_buffer(uint8_t value);
#endif //TEST_USART_H
