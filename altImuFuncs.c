#include "altImuFuncs.h"
#include "altImuRegs.h"
#include "altImuI2C.h"

extern uint8_t *pErrorText; 

// �������� ��������� ��� ������������� �������� 
static uint8_t g_init [5] = {
	0b00111100, // ������� ��������� ������ ODR = 95, ���������� ������ Cut-off = 25, ��� = Z
	0b00000000,	// High-pass filter = Reference signal for filtering
	0b00000000,	// default 
	0b10000000,	// ������ �� ��������� ��� ������, ����� = 250 ����/���, ������� ����� = ������� ����
	0b00000000	// def FIFO ����, ��� - ����
};
static uint8_t a_init [6] = {
	0b01010111, // ������� �������� 100 v�, ��������� �� ���� ���
	0b00000000, // default
	0b00000000, // default
	0b10000000, // ������ �� ��������� ��� ������, �������� �������� +/- 2G
	0b00000000, // default
	0b00000000  // default
};
static uint8_t c_init[3] = {
	0b00011000, // ������� �������� 75 v�
	0b00100000, // �������� �������� +/- 1.3 v����
	0b00000000 // Continuous-conversion mode
};

// ������� ������������� �������� 
static uint8_t gyro_initialize (void) {
	
	return twi_write_seq_byte(GYRO, GYRO_CTRL_REG1, 4, &g_init[0]);
}

static uint8_t accel_initialize (void) {

	return twi_write_seq_byte(ACCEL, ACCEL_CTRL_REG1, 4, &a_init[0]);
}

static uint8_t compas_initialize (void) {
	
	return twi_write_seq_byte(COMPAS, COMPAS_CRA_REG, 3, &c_init);
}

void altIMU_initialize (void) {

	if (compas_initialize()) {
		pErrorText = "Corrupt compas_initialize()"; 
		return 1;
	}
	//delay_ms(10); 
	if (accel_initialize()) { 
		pErrorText = "Corrupt accel_initialize()";
		return 1;
	}
	//delay_ms(10); 
	if (gyro_initialize()) {
		pErrorText = "Corrupt gyro_initialize()";
		return 1;
	}
	
	return 0; 
} 

// ����� ������

// ������� ���������� ������� �������� ������� �������� (��������)
int16_t getAngVel (void) {
	
	uint8_t d[2]; 

	i2c_read_seq_byte(GYRO, GYRO_OUT_Z_L, 2, &d[0]); 
			
	return ((d[1] << 8) & 0xFF00) | (d[0] & 0x00FF); 
}

// ������� ���������� �������� ������� ���������� ���� � ������� ��������� �������
void getGaus (int16_t *x, int16_t *y, int16_t *z) {

	#define A_NUM 6
	uint8_t d[A_NUM] = {};
	
	i2c_read_seq_byte(COMPAS, COMPAS_OUT_X_H, A_NUM, &d[0]);
	
	*x = (d[0] | (d[1] << 8)); 
	*y = (d[2] | (d[3] << 8)); 
	*z = (d[4] | (d[5] << 8)); 
}

// ������� ���������� �������� ������� ���� ������� � ������ ��������� �������
void getAcceleration (uint16_t *x, uint16_t *y, uint16_t *z) {
	
	#define A_NUM 6 
	uint8_t d[A_NUM] = {};
	int16_t temp[A_NUM/2]; 
	double a_axis[A_NUM/2]; 	
	
	i2c_read_seq_byte(ACCEL, ACCEL_OUT_X_L, A_NUM, &d[0]);
		
	// ��������
	uint8_t j = 0;  
	for (uint8_t i = 0; i < A_NUM/2; i ++) {
		temp [i] = (d[j] | (d[++j] << 8)); // ??? ++j
		j++;  
	}
	
	for (uint8_t i = 0; i < A_NUM/2; i ++) a_axis [i] = (double)temp[i];
	
	double x_temp = atan(a_axis[0]/sqrt(a_axis[1]*a_axis[1] + a_axis[2]*a_axis[2]));
	double y_temp = atan(a_axis[1]/sqrt(a_axis[2]*a_axis[2] + a_axis[0]*a_axis[0]));
	double z_temp = atan(a_axis[2]/sqrt(a_axis[1]*a_axis[1] + a_axis[0]*a_axis[0]));
	
	*x = (int16_t)(180 * x_temp / 3.14);
	*y = (int16_t)(180 * y_temp / 3.14);
	*z = (int16_t)(180 * z_temp / 3.14);	
}