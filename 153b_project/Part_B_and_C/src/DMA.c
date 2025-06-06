/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */
 
#include "DMA.h"
#include "SysTimer.h"

void DMA_Init_UARTx(DMA_Channel_TypeDef* tx_buffer, USART_TypeDef* USARTx) {
	if (RCC->AHB1ENR | RCC_AHB1ENR_DMA1EN) {
		// enable clock for DMA
		RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
		// reset DMA clock
		RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA1RST;
		RCC->AHB1RSTR &= ~RCC_AHB1RSTR_DMA1RST;
		for (uint32_t i = 0U; i < 100000U; i++);
	}
	// set DMA1 channel 4 to USART1_TX
	DMA1_CSELR->CSELR &= ~DMA_CSELR_C4S;
	uint32_t DMA_CSELR_C4S_0 = DMA_CSELR_C4S & ~(DMA_CSELR_C4S << 1U);
	DMA1_CSELR->CSELR |= (DMA_CSELR_C4S_0 << 1U);
	// set DMA1 channel 5 to USART1_RX
	DMA1_CSELR->CSELR &= ~DMA_CSELR_C5S;
	uint32_t DMA_CSELR_C5S_0 = DMA_CSELR_C5S & ~(DMA_CSELR_C5S << 1U);
	DMA1_CSELR->CSELR |= (DMA_CSELR_C5S_0 << 1U);
	// set DMA1 channel 7 to USART2_TX
	DMA1_CSELR->CSELR &= ~DMA_CSELR_C7S;
	uint32_t DMA_CSELR_C7S_0 = DMA_CSELR_C7S & ~(DMA_CSELR_C7S << 1U);
	DMA1_CSELR->CSELR |= (DMA_CSELR_C7S_0 << 1U);
	// set DMA1 channel 6 to USART2_RX
	DMA1_CSELR->CSELR &= ~DMA_CSELR_C6S;
	uint32_t DMA_CSELR_C6S_0 = DMA_CSELR_C6S & ~(DMA_CSELR_C6S << 1U);
	DMA1_CSELR->CSELR |= (DMA_CSELR_C6S_0 << 1U);
	if (USARTx == USART1) {
		DMA1_Channel4_USART1_TX_setup();
		DMA1_Channel5_USART1_RX_setup();
	}
	else {
		// disable DMA channel 7
		DMA1_Channel7->CCR &= ~DMA_CCR_EN;
		// disable memory-to-memory mode
		DMA1_Channel7->CCR &= ~DMA_CCR_MEM2MEM;
		// set channel priority to high
		DMA1_Channel7->CCR &= ~DMA_CCR_PL;
		DMA1_Channel7->CCR |= DMA_CCR_PL_1;
		// set peripheral size to 8 bits
		DMA1_Channel7->CCR &= ~DMA_CCR_PSIZE;
		// set memory size to 8 bits
		DMA1_Channel7->CCR &= ~DMA_CCR_MSIZE;
		// disable peripheral increment mode
		DMA1_Channel7->CCR &= ~DMA_CCR_PINC;
		// enable memory increment mode
		DMA1_Channel7->CCR |= DMA_CCR_MINC;
		// disable circular mode
		DMA1_Channel7->CCR &= ~DMA_CCR_CIRC;
		// set data transfer direction to memory-to-peripheral
		DMA1_Channel7->CCR |= DMA_CCR_DIR;
		// set memory address to buffer
		DMA1_Channel7->CMAR = (uint32_t)tx_buffer;
		// set peripheral address to USART2->TDR
		DMA1_Channel7->CPAR = (uint32_t)&USART2->TDR;
		// disable half-transfer interrupt
		DMA1_Channel7->CCR &= ~DMA_CCR_HTIE;
		// disable transfer error interrupt
		DMA1_Channel7->CCR &= ~DMA_CCR_TEIE;
		// enable transfer complete interrupt
		DMA1_Channel7->CCR |= DMA_CCR_TCIE;
		// set priority in NVIC
		NVIC_SetPriority(DMA1_Channel7_IRQn, 0);
		// enable interrupt in NVIC
		NVIC_EnableIRQ(DMA1_Channel7_IRQn);
	}
}

void DMA1_Channel4_IRQHandler(void) {
	if (DMA1->ISR & DMA_ISR_TCIF4) {
		DMA1->IFCR |= DMA_IFCR_CTCIF4;
		// do something
	}
	DMA1->IFCR |= DMA_IFCR_CGIF4;
}

void DMA1_Channel7_IRQHandler(void) {
	if (DMA1->ISR & DMA_ISR_TCIF7) {
		DMA1->IFCR |= DMA_IFCR_CTCIF7;
		// do something
	}
	DMA1->IFCR |= DMA_IFCR_CGIF7;
}

void DMA1_Channel4_USART1_TX_setup(void) {
	// disable DMA1 channel 4
	DMA1_Channel4->CCR &= ~DMA_CCR_EN;
	// disable memory-to-memory mode
	DMA1_Channel4->CCR &= ~DMA_CCR_MEM2MEM;
	// set channel priority to high
	DMA1_Channel4->CCR &= ~DMA_CCR_PL;
	DMA1_Channel4->CCR |= DMA_CCR_PL_1;
	// set peripheral size to 8 bits
	DMA1_Channel4->CCR &= ~DMA_CCR_PSIZE;
	// set memory size to 8 bits
	DMA1_Channel4->CCR &= ~DMA_CCR_MSIZE;
	// disable peripheral increment mode
	DMA1_Channel4->CCR &= ~DMA_CCR_PINC;
	// enable memory increment mode
	DMA1_Channel4->CCR |= DMA_CCR_MINC;
	// disable circular mode
	DMA1_Channel4->CCR &= ~DMA_CCR_CIRC;
	// set data transfer direction to memory-to-peripheral
	DMA1_Channel4->CCR |= DMA_CCR_DIR;
	// set memory address to buffer
	DMA1_Channel4->CMAR = (uint32_t)tx_buffer;
	// set peripheral address to USART1->TDR
	DMA1_Channel4->CPAR = (uint32_t)&USART1->TDR;
	// disable half-transfer interrupt
	DMA1_Channel4->CCR &= ~DMA_CCR_HTIE;
	// disable transfer error interrupt
	DMA1_Channel4->CCR &= ~DMA_CCR_TEIE;
	// enable transfer complete interrupt
	DMA1_Channel4->CCR |= DMA_CCR_TCIE;
	// set priority in NVIC
	NVIC_SetPriority(DMA1_Channel4_IRQn, 0);
	// enable interrupt in NVIC
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

void DMA1_Channel5_USART1_RX_setup(void) {
	// TODO
	// disable DMA1 channel 4
	DMA1_Channel4->CCR &= ~DMA_CCR_EN;
	// disable memory-to-memory mode
	DMA1_Channel4->CCR &= ~DMA_CCR_MEM2MEM;
	// set channel priority to high
	DMA1_Channel4->CCR &= ~DMA_CCR_PL;
	DMA1_Channel4->CCR |= DMA_CCR_PL_1;
	// set peripheral size to 8 bits
	DMA1_Channel4->CCR &= ~DMA_CCR_PSIZE;
	// set memory size to 8 bits
	DMA1_Channel4->CCR &= ~DMA_CCR_MSIZE;
	// disable peripheral increment mode
	DMA1_Channel4->CCR &= ~DMA_CCR_PINC;
	// enable memory increment mode
	DMA1_Channel4->CCR |= DMA_CCR_MINC;
	// disable circular mode
	DMA1_Channel4->CCR &= ~DMA_CCR_CIRC;
	// set data transfer direction to memory-to-peripheral
	DMA1_Channel4->CCR |= DMA_CCR_DIR;
	// set memory address to buffer
	DMA1_Channel4->CMAR = (uint32_t)tx_buffer;
	// set peripheral address to USART1->TDR
	DMA1_Channel4->CPAR = (uint32_t)&USART1->TDR;
	// disable half-transfer interrupt
	DMA1_Channel4->CCR &= ~DMA_CCR_HTIE;
	// disable transfer error interrupt
	DMA1_Channel4->CCR &= ~DMA_CCR_TEIE;
	// enable transfer complete interrupt
	DMA1_Channel4->CCR |= DMA_CCR_TCIE;
	// set priority in NVIC
	NVIC_SetPriority(DMA1_Channel4_IRQn, 0);
	// enable interrupt in NVIC
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}