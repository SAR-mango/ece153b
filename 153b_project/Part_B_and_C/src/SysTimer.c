/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */
 
#include "SysTimer.h"
#include "motor.h"

static uint32_t volatile step;
uint32_t interrupts_per_step = 2U;

void SysTick_Init(void) {
	// SysTick Control & Status Register
	SysTick->CTRL = 0U; // Disable SysTick IRQ and SysTick Counter

	SysTick->LOAD = 79999U; // establishes 1ms SysTick interval
	SysTick->VAL = 0U;
	
	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does not assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	// Select clock source
	// If CLKSOURCE = 0, 10 MHz
	// If CLKSOURCE = 1, 80 MHz
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	
	// Enable SysTick IRQ and SysTick Timer
	// Configure and Enable SysTick interrupt in NVIC
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 1U); // Set Priority to 1
}

void SysTick_Handler(void) {
	step++;
	if (!(step % interrupts_per_step)) {
		rotate();
	}
	if (!(step % 1000U)) {
		step = 0U;
	}
}

void delay_ms(uint32_t ms) {
	uint32_t ms_passed = 0U;
	uint32_t current_step = 0U;
	while (ms_passed != ms) {
		current_step = step;
		while (step == current_step);
		ms_passed++;
	}
}
