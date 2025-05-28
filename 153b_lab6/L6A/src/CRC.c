/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6A
 */
 
#include "CRC.h"

/**
  * @brief  Algorithm implementation of the CRC
  * @param  Crc: specifies the previous Crc data
  * @param  Data: specifies the input data
  * @retval Crc: the CRC result of the input data
  */

uint32_t CrcSoftwareFunc(uint32_t Initial_Crc, uint32_t Input_Data, uint32_t POLY) {
  uint8_t bindex = 0;
  uint32_t Crc = 0;

  /* Initial XOR operation with the previous Crc value */
	Crc = Initial_Crc ^ Input_Data;

  /* The CRC algorithm routine */
  while (bindex < (sizeof(Input_Data) * 8)) {
		if ((Crc & 0x80000000) == 0x80000000) {
			Crc = (Crc << 1) ^ POLY;
		}
		else {
			Crc <<= 1;
		}
		bindex++;
	}
	
  return Crc;
}
