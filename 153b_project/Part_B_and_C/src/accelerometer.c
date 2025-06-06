#include "SPI.h"
#include "SysTimer.h"
#include "accelerometer.h"

void accWrite(uint8_t addr, uint8_t val){
	SPI_Transfer_Data((addr << 8) | val);
}

uint8_t accRead(uint8_t addr){
	return ((uint8_t) (SPI_Transfer_Data((0x80 | addr) << 8) & ~0xFF00));
}

void initAcc(void){
	accWrite(0x2C, 0b00001010);
	accWrite(0x31, 0b00001000);
	accWrite(0x2D, 0b00001000);
}

void readValues(double* x, double* y, double* z) {
	uint8_t xlo = accRead(0x32);
	uint8_t xhi = accRead(0x33);
	uint8_t ylo = accRead(0x34);
	uint8_t yhi = accRead(0x35);
	uint8_t zlo = accRead(0x36);
	uint8_t zhi = accRead(0x37);

	int16_t x_raw = (int16_t)((xhi << 8) | xlo);
	int16_t y_raw = (int16_t)((yhi << 8) | ylo);
	int16_t z_raw = (int16_t)((zhi << 8) | zlo);

	*x = x_raw * 0.0039;
	*y = y_raw * 0.0039;
	*z = z_raw * 0.0039;
}
