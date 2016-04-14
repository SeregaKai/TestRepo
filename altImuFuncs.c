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