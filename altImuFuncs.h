#ifndef _ALTIMU_FUNCS_H
#define _ALTIMU_FUNCS_H
/*
AltIMU - Gyro, Accelerimeter, Compas, Barometer on small board.
External interface - I2C. 
L3GD20 - 3-axis gyro
LSM303DLHC - 3D accelerometer and 3D magnetometer 
LPS331AP - pressure sensor
*/

void altIMU_initialize (void);

int16_t getAngVel (void);
void getGaus (int16_t *x, int16_t *y, int16_t *z);
void getAcceleration (uint16_t *x, uint16_t *y, uint16_t *z);

#endif //_ALTIMU_FUNCS_H

