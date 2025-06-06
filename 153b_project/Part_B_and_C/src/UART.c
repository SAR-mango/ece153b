/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */


#include "UART.h"
#include "DMA.h"

static volatile DMA_Channel_TypeDef* tx;
static volatile char inputs[IO_SIZE];
static volatile uint8_t data_t_0[IO_SIZE];
static volatile uint8_t data_t_1[IO_SIZE];
static volatile uint8_t input_size = 0U;
static volatile uint8_t pending_size = 0U;
static volatile uint8_t* active = data_t_0;
static volatile uint8_t* pending = data_t_1;

#define SEL_0 1
#define BUF_0_EMPTY 2
#define BUF_1_EMPTY 4
#define BUF_0_PENDING 8
#define BUF_1_PENDING 16

void transfer_data(char ch);
void on_complete_transfer(void);

void UART1_Init(void) {
	// enable USART1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	// select system clock as USART1 clock source
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL_1;
	DMA_Init_UARTx((DMA_Channel_TypeDef*)active, USART1);
}

void UART2_Init(void) {
	// enable USART2 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	// select system clock as USART2 clock source
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL_1;
	DMA_Init_UARTx((DMA_Channel_TypeDef*)active, USART2);
}

void UART1_GPIO_Init(void) {
	// set PA9 and PA10 mode to alternative function (10)
	GPIOA->MODER &= ~GPIO_MODER_MODE9_0;
	GPIOA->MODER |= GPIO_MODER_MODE9_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE10_0;
	GPIOA->MODER |= GPIO_MODER_MODE10_1;
	// set PA9 and PA10 output type to push-pull (0)
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT10;
	// set PA9 and PA10 to pull-up (01)
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD9_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD10_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD10_1;
	// set PA9 and PA10 speed to very high (11)
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;
	// set PA9 alternative function to USART1_TX (AF7)
	GPIOA->AFR[1] |= (GPIO_AFRH_AFSEL9_0 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_2);
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL9_3;
	// set PA10 alternative function to USART1_RX (AF7)
	GPIOA->AFR[1] |= (GPIO_AFRH_AFSEL10_0 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_2);
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL10_3;
}

void UART2_GPIO_Init(void) {
	// set PA2 and PA3 mode to alternative function (10)
	GPIOA->MODER &= ~GPIO_MODER_MODE2_0;
	GPIOA->MODER |= GPIO_MODER_MODE2_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE3_0;
	GPIOA->MODER |= GPIO_MODER_MODE3_1;
	// set PA2 and PA3 output type to push-pull (0)
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;
	// set PA2 and PA3 to pull-up (01)
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD3_1;
	// set PA2 and PA3 speed to very high (11)
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3;
	// set PA2 alternative function to USART2_TX (AF7)
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2);
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2_3;
	// set PA3 alternative function to USART2_RX (AF7)
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2);
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3_3;
}

void USART_Init(USART_TypeDef* USARTx) {
	// disable USART
	USARTx->CR1 &= ~USART_CR1_UE;
	// set word length to 8 bits
	USARTx->CR1 &= ~USART_CR1_M1;
	// set oversampling mode to oversample by 16
	USARTx->CR1 &= ~USART_CR1_OVER8;
	// set number of stop bits to 1
	USARTx->CR2 &= ~USART_CR2_STOP;
	// set baud rate to 9600bps
	USARTx->BRR = 8333U;
	// enable transmitter and receiver
	USARTx->CR1 |= USART_CR1_TE;
	USARTx->CR1 |= USART_CR1_RE;
	// enable USART
	USARTx->CR1 |= USART_CR1_UE;
}

/**
 * This function accepts a string that should be sent through UART
*/
void UART_print(char* data) {
	//TODO

	//Transfer char array to buffer
	//Check DMA status. If DMA is ready, send data
	//If DMA is not ready, put the data aside
}

/**
 * This function should be invoked when a character is accepted through UART
*/
void transfer_data(char ch) {
	//TODO
	// Append character to input buffer.
	// If the character is end-of-line, invoke UART_onInput
}

/**
 * This function should be invoked when DMA transaction is completed
*/
void on_complete_transfer(void) {
	//TODO
	// If there are pending data to send, switch active and pending buffer, and send data
}

void USART1_IRQHandler(void){
	//TODO
	// When receive a character, invoke transfer_data
	// When complete sending data, invoke on_complete_transfer
}

void USART2_IRQHandler(void){
	//TODO
	// When receive a character, invoke transfer_data
	// When complete sending data, invoke on_complete_transfer
}
