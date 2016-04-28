#ifndef ALT_IMU_IC_H
#define ALT_IMU_IC_H

//#include <asf.h>

uint16_t i2c_write_seq_byte(uint8_t deviceAddr, uint8_t regStartAddr, uint8_t num, uint8_t *d);
uint16_t i2ñ_read_seq_byte (uint8_t deviceAddr, uint8_t regStartAddr, uint8_t num, uint8_t *d);
void i2c_reset_errors (void); 


#endif // ALT_IMU_IC_H