/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */

#include "stm32l476xx.h"
#include "motor.h"

#define GPIO_A GPIO_ODR_OD5
#define GPIO_A_PRIME GPIO_ODR_OD6
#define GPIO_B GPIO_ODR_OD8
#define GPIO_B_PRIME GPIO_ODR_OD9

static const uint32_t motor_pins_mask = (GPIO_ODR_OD5 | GPIO_ODR_OD6 | GPIO_ODR_OD8 | GPIO_ODR_OD9);
static volatile uint8_t step_index = 0U;
static const uint32_t half_step_words[8U] = {
	GPIO_A | GPIO_B_PRIME,
	GPIO_A,
	GPIO_A | GPIO_B,
	GPIO_B,
	GPIO_B | GPIO_A_PRIME,
	GPIO_A_PRIME,
	GPIO_A_PRIME | GPIO_B_PRIME,
	GPIO_B_PRIME
};

static volatile enum Direction direction = DIR_CW;

void Motor_Init(void) {	
	// set mode to output (01)
	GPIOC->MODER |= (GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0);
	GPIOC->MODER &= ~(GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1);
	// set output speed to "fast" (10 -> high speed, or 11 -> very high speed)
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5_0 | GPIO_OSPEEDR_OSPEED6_0 | GPIO_OSPEEDR_OSPEED8_0 | GPIO_OSPEEDR_OSPEED9_0);
	GPIOC->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5_1 | GPIO_OSPEEDR_OSPEED6_1 | GPIO_OSPEEDR_OSPEED8_1 | GPIO_OSPEEDR_OSPEED9_1);
	// set output type to push-pull (0)
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6 | GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);
	// set no pull-up, no pull-down (00)
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9);
	// set all pins to be off on startup
	GPIOC->ODR &= ~(GPIO_ODR_OD5 | GPIO_ODR_OD6 | GPIO_ODR_OD8 | GPIO_ODR_OD9);
}

void rotate() {
	// get current state of GPIOC->ODR
	uint32_t temp_gpio_odr = GPIOC->ODR;
	// reset all motor pins
	temp_gpio_odr &= ~motor_pins_mask;
	// set pins corresponding to current step
	temp_gpio_odr |= half_step_words[step_index];
	// write to GPIOC->ODR
	GPIOC->ODR = temp_gpio_odr;
	// increment/decrement index
	if (direction == DIR_CW) {
		step_index++;
		if (step_index == 8) {
			step_index = 0;
		}
	}
	else if (direction == DIR_CCW) {
		if (step_index == 0) {
			step_index = 8;
		}
		step_index--;
	}
	else {};
}

void setDirection(enum Direction motor_direction) {
	direction = motor_direction;
}
