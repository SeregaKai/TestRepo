#ifndef ALTIMU_REGS_H_
#define ALTIMU_REGS_H_

#define ACCEL 		0x32
#define COMPAS 		0x3C
#define GYRO 		0xD6
#define BAROMETER 	0xBA

//	�������� ���������
#define GYRO_CTRL_REG1 0x20 // REG2 = 0x21 � �.�. ����� 5 ���������

#define GYRO_OUT_Z_L 0x2C
#define GYRO_OUT_Z_H 0x2D

// �������� �������������
#define ACCEL_CTRL_REG1	0x20 // REG2 = 0x21 � �.�. ����� 6 ���������

#define ACCEL_OUT_X_L	0x28
#define ACCEL_OUT_X_H	0x29
#define ACCEL_OUT_Y_L	0x2A
#define ACCEL_OUT_Y_H	0x2B
#define ACCEL_OUT_Z_L	0x2C
#define ACCEL_OUT_Z_H	0x2D

// �������� ������������
#define COMPAS_CRA_REG	0x00 // REG2 = 0x01 � �.�. ����� 3 ���������

#define COMPAS_OUT_X_H	0x03
#define COMPAS_OUT_X_L	0x04
#define COMPAS_OUT_Y_H	0x05
#define COMPAS_OUT_Y_L	0x06
#define COMPAS_OUT_Z_H	0x07
#define COMPAS_OUT_Z_L	0x08


#define AUTO_INC	0x80 // ������������� ������ ��� ������/������ ������ 


#endif /* ALTIMU_REGS_H_ */