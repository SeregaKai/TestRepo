#include "altImuI2C.h"
#include "altImuRegs.h"

#define I2C TWIM2 	// ������ I2C ATSAM4L
#define DELAY_TICKS	2500 	// �������� 100 ��� ��� Fclk = 48 Mhz

// ����� ������ ������ I2C 
void i2c_reset_errors (void) {
	
	I2C->TWIM_SCR |= (TWIM_SCR_ANAK | TWIM_SCR_DNAK | TWIM_SCR_ARBLST | TWIM_SCR_HSMCACK | TWIM_SCR_CCOMP); // ���������� ����� ������
}

// �������� ������� ��� ���������� �������� � ������ 
// ���������� 0 � ������ ���� ������ ����������� ������ ��� ������� ����� �������� 
// ���������� �� 0 � ������ ������ ��� �������� (������� ����� �� ����) 
static uint16_t i2c_wait_idle_tx(void) { 
	uint16_t Timer = DELAY_TICKS;//
	while ( (!(I2C->TWIM_SR & TWIM_SR_TXRDY)) && (--Timer) ); // ������� �� ������ �������
	return !Timer;
}
static uint16_t i2c_wait_idle_rx(void) {
	uint16_t Timer = DELAY_TICKS; 
	while (!(I2C->TWIM_SR & TWIM_SR_RXRDY) && (--Timer)); // ������� �� ������ �������
	return !Timer;
}

// ������ ������������������ ���� � �������� ����������
// ���������� 0 � ������ �������� �������� ������
// ���������� �� 0 � ������ ������ ��� ��������
uint16_t i2c_write_seq_byte(uint8_t deviceAddr, uint8_t regStartAddr, uint8_t num, uint8_t *d) {
	
	uint16_t errCounter = 0;
	
	if (!num) return 1;
	if (!deviceAddr) return 1;
	if (!d) return 1;
	
	//*** ����������� ������ 
	cpu_irq_disable(); 
	
	i2c_reset_errors();
	
	// ���������
	I2C->TWIM_CMDR =
	TWIM_CMDR_SADR(deviceAddr>>1)
	| TWIM_CMDR_NBYTES(num+1)
	| TWIM_CMDR_VALID
	| TWIM_CMDR_STOP
	| TWIM_CMDR_START;
	
	// �������� ������ 
	if (num > 1) I2C->TWIM_THR = (regStartAddr|AUTO_INC);
	else I2C->TWIM_THR = regStartAddr;
	
	errCounter += i2c_wait_idle_tx();
	// �������� ������ 
	for (uint8_t i = 0; i < num; i++) {
		errCounter += i2c_wait_idle_tx();
		I2C->TWIM_THR = d[i];
		delay_us(100);
	}

	//*** ����������� ������ 
	cpu_irq_enable();

	return errCounter;
}

// ������ ������������������ ���� �� ��������� ����������
// ���������� 0 � ������ �������� �������� ������
// ���������� �� 0 � ������ ������ ��� ������ 
uint16_t i2�_read_seq_byte (uint8_t deviceAddr, uint8_t regStartAddr, uint8_t num, uint8_t *d) {
	
	uint16_t errCounter = 0;
	
	if (!num) return 1;
	if (deviceAddr) return 1;
	if (!d) return 1;
	
	//*** ����������� ������ 
	cpu_irq_disable();
	
	i2c_reset_errors();
	
	// ��������� 
	I2C->TWIM_CMDR =
	TWIM_CMDR_SADR(deviceAddr>>1)
	| TWIM_CMDR_NBYTES(1)
	| TWIM_CMDR_VALID
	//| TWIM_CMDR_STOP
	| TWIM_CMDR_START;
		
	I2C->TWIM_NCMDR =
	TWIM_NCMDR_SADR(deviceAddr)
	| TWIM_NCMDR_NBYTES(num)
	| TWIM_NCMDR_VALID
	| TWIM_NCMDR_START
	| TWIM_NCMDR_STOP
	| TWIM_NCMDR_READ;
	
	// �������� ������ 
	if (num > 1) I2C->TWIM_THR = (regStartAddr|AUTO_INC);
	else I2C->TWIM_THR = regStartAddr;
	
	errCounter += i2c_wait_idle_tx();	
	// ������ 
	for (uint8_t i = 0; i < num; i++) {
		errCounter += i2c_wait_idle_rx();
		d[i] = I2C->TWIM_RHR;
	}
	
	//*** ����������� ������ 
	cpu_irq_enable();
	
	return errCounter;
}

/*
// ����� � �������� ������ ����� 
void i2c_write_byte (uint8_t byte) {
	I2C->TWIM_THR = byte; //TWIM_THR_TXDATA(data);
}
uint8_t i2c_read_byte (void){
	return I2C->TWIM_RHR;
}
// ��������� ������ ��� �������� ������
void i2c_start_write_ (uint8_t adr, uint8_t num) {
	I2C->TWIM_CMDR =
	TWIM_CMDR_SADR(adr>>1)	// D6 - ��������  BA - �������� 32 - ������������ 3C
	| TWIM_CMDR_NBYTES(num)
	| TWIM_CMDR_VALID
	| TWIM_CMDR_START
	| TWIM_CMDR_STOP;	
}
// ��������� ������ ��� ������ 
void i2c_start_read_ (uint8_t adr, uint8_t num) {
	I2C->TWIM_CMDR =
	TWIM_CMDR_SADR(adr>>1)	// D6 - ��������  BA - �������� 32 - ������������ 3C
	| TWIM_CMDR_NBYTES(num)
	| TWIM_CMDR_VALID
	| TWIM_CMDR_START
	| TWIM_CMDR_STOP
	| TWIM_CMDR_READ;	
}
*/