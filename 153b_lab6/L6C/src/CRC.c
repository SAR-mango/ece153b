/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6B
 */
 
#include "CRC.h"

/**
  * @brief  Computes the 32-bit CRC of a given buffer of data word(32-bit).
  * @param  pBuffer: pointer to the buffer containing the data to be computed
  * @param  BufferLength: length of the buffer to be computed					
  * @retval 32-bit CRC
  */
uint32_t CRC_CalcBlockCRC(const uint32_t * pBuffer, uint32_t BufferLength) {
  for (uint32_t i = 0; i < BufferLength; i++) {
		CRC->DR = pBuffer[i];
	}
	return CRC->DR;
}	

void CRC_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
	RCC->AHB1RSTR |= RCC_AHB1RSTR_CRCRST;
	RCC->AHB1RSTR &= ~RCC_AHB1RSTR_CRCRST;
	CRC->CR &= ~CRC_CR_POLYSIZE;
	CRC->POL = 0x04C11DB7;
	CRC->INIT = 0xFFFFFFFF;
	// Default CRC Polynomial (CRC->POL): 0x04C1_1DB7
	// Default initial CRC Value (CRC->INIT): 0xFFFF_FFFF
}

