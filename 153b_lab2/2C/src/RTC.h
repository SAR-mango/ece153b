/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 2C
 */


#ifndef __STM32L476G_NUCLEO_RTC_H
#define __STM32L476G_NUCLEO_RTC_H

#include "stm32l476xx.h"
#include "string.h"
#include "stdio.h"

void RTC_Init(void);
void RTC_Clock_Init(void);
void RTC_Disable_Write_Protection(void);
void RTC_Enable_Write_Protection(void);
void RTC_Set_Calendar_Date(uint32_t WeekDay, uint32_t Day, uint32_t Month, uint32_t Year);
void RTC_Set_Time(uint32_t Format12_24, uint32_t Hour, uint32_t Minute, uint32_t Second);
uint32_t getTens(uint32_t number);
uint32_t getOnes(uint32_t number);

uint32_t RTC_TIME_GetHour(void);
uint32_t RTC_TIME_GetMinute(void);
uint32_t RTC_TIME_GetSecond(void);

uint32_t RTC_DATE_GetMonth(void);
uint32_t RTC_DATE_GetDay(void);
uint32_t RTC_DATE_GetYear(void);
uint32_t RTC_DATE_GetWeekDay(void);

void Get_RTC_Calendar(char * strTime, char * strDate);

#endif
