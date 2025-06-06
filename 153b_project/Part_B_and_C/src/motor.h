#ifndef __STM32L476G_DISCOVERY_MOTOR_H
#define __STM32L476G_DISCOVERY_MOTOR_H

#include "stm32l476xx.h"

enum Direction {DIR_CW, DIR_CCW, DIR_STOP};

void Motor_Init(void);
void rotate(void);
void setDirection(enum Direction direction);

#endif /* __STM32L476G_DISCOVERY_MOTOR_H */
