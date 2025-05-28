/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6C
 */
 
#include "DMA.h"
#include "CRC.h"

void DMA_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA1RST;
	RCC->AHB1RSTR &= ~RCC_AHB1RSTR_DMA1RST;
	for (uint32_t i = 0; i < 100000; i++);
	DMA1_Channel6->CCR &= ~DMA_CCR_EN;
	DMA1_Channel6->CCR |= DMA_CCR_MEM2MEM;
	DMA1_Channel6->CCR &= ~DMA_CCR_PL;
	DMA1_Channel6->CCR |= DMA_CCR_PL_1;
	DMA1_Channel6->CCR &= ~DMA_CCR_PSIZE;
	DMA1_Channel6->CCR |= DMA_CCR_PSIZE_1;
	DMA1_Channel6->CCR &= ~DMA_CCR_MSIZE;
	DMA1_Channel6->CCR |= DMA_CCR_MSIZE_1;
	DMA1_Channel6->CCR &= ~DMA_CCR_PINC;
	DMA1_Channel6->CCR |= DMA_CCR_MINC;
	DMA1_Channel6->CCR &= ~DMA_CCR_CIRC;
	DMA1_Channel6->CCR |= DMA_CCR_DIR;
	DMA1_Channel6->CMAR = (uint32_t)DataBuffer;
	DMA1_Channel6->CPAR = (uint32_t)&CRC->DR;
	DMA1_Channel6->CCR &= ~DMA_CCR_HTIE;
	DMA1_Channel6->CCR &= ~DMA_CCR_TEIE;
	DMA1_Channel6->CCR |= DMA_CCR_TCIE;
	NVIC_SetPriority(DMA1_Channel6_IRQn, 0);
	NVIC_EnableIRQ(DMA1_Channel6_IRQn);
}

void DMA1_Channel6_IRQHandler(void){ 
	if (DMA1->ISR & DMA_ISR_TCIF6) {
		DMA1->IFCR |= DMA_IFCR_CTCIF6;
		completeCRC(CRC->DR);
	}
	DMA1->IFCR |= DMA_IFCR_CGIF6;
}

