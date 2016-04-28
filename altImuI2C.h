#ifndef ALT_IMU_I2C_H_
#define ALT_IMU_I2C_H_

#include <asf.h>

uint16_t i2c_write_seq_byte(uint8_t deviceAddr, uint8_t regStartAddr, uint8_t num, uint8_t *d); 
uint16_t i2ñ_read_seq_byte (uint8_t deviceAddr, uint8_t regStartAddr, uint8_t num, uint8_t *d);
void i2c_write_byte (uint8_t byte);
uint8_t i2c_read_byte (void);

#endif /* ALT_IMU_I2C_H_ */