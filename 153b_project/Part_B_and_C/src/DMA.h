/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */

#ifndef __STM32L476G_DISCOVERY_DMA_H
#define __STM32L476G_DISCOVERY_DMA_H

#include "stm32l476xx.h"

void DMA_Init_UARTx(DMA_Channel_TypeDef* tx_buffer, USART_TypeDef* uart);

void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);

void DMA1_Channel4_USART1_TX_setup(void);
void DMA1_Channel5_USART1_RX_setup(void);


#endif /* __STM32L476G_DISCOVERY_DMA_H */
