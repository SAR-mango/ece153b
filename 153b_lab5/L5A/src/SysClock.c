#include "SysClock.h"

void System_Clock_Init(void) {
	RCC->CR |= RCC_CR_MSION;
	while ((RCC->CR & RCC_CR_MSIRDY) == 0);

	RCC->CR |= RCC_CR_MSIRGSEL;
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= RCC_CR_MSIRANGE_7;

	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |=  RCC_CFGR_SW_MSI;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_MSI);
}
