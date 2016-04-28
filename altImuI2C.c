#include "altImuI2C.h"
#include "altImuRegs.h"

#define I2C TWIM2 	// модуль I2C ATSAM4L
#define DELAY_TICKS	2500 	// примерно 100 мкс при Fclk = 48 Mhz

// сброс ошибок модуля I2C 
void i2c_reset_errors (void) {
	
	I2C->TWIM_SCR |= (TWIM_SCR_ANAK | TWIM_SCR_DNAK | TWIM_SCR_ARBLST | TWIM_SCR_HSMCACK | TWIM_SCR_CCOMP); // сбрасываем флаги ошибок
}

// задержка времени при выполнение передачи и приема 
// возвращает 0 в случае если модуль освободился раньше чем истекло время задержки 
// возвращает НЕ 0 в случае ошибки при передаче (счетчик дошел до нуля) 
static uint16_t i2c_wait_idle_tx(void) { 
	uint16_t Timer = DELAY_TICKS;//
	while ( (!(I2C->TWIM_SR & TWIM_SR_TXRDY)) && (--Timer) ); // выходим по любому условию
	return !Timer;
}
static uint16_t i2c_wait_idle_rx(void) {
	uint16_t Timer = DELAY_TICKS; 
	while (!(I2C->TWIM_SR & TWIM_SR_RXRDY) && (--Timer)); // выходим по любому условию
	return !Timer;
}

// запись последовательности байт в регистры устройства
// возвращает 0 в случае успешной передачи данных
// возвращает НЕ 0 в случае ошибок при передаче
uint16_t i2c_write_seq_byte(uint8_t deviceAddr, uint8_t regStartAddr, uint8_t num, uint8_t *d) {
	
	uint16_t errCounter = 0;
	
	if (!num) return 1;
	if (!deviceAddr) return 1;
	if (!d) return 1;
	
	//*** критическая секция 
	cpu_irq_disable(); 
	
	i2c_reset_errors();
	
	// настройка
	I2C->TWIM_CMDR =
	TWIM_CMDR_SADR(deviceAddr>>1)
	| TWIM_CMDR_NBYTES(num+1)
	| TWIM_CMDR_VALID
	| TWIM_CMDR_STOP
	| TWIM_CMDR_START;
	
	// передача адреса 
	if (num > 1) I2C->TWIM_THR = (regStartAddr|AUTO_INC);
	else I2C->TWIM_THR = regStartAddr;
	
	errCounter += i2c_wait_idle_tx();
	// отправка данных 
	for (uint8_t i = 0; i < num; i++) {
		errCounter += i2c_wait_idle_tx();
		I2C->TWIM_THR = d[i];
		delay_us(100);
	}

	//*** критическая секция 
	cpu_irq_enable();

	return errCounter;
}

// чтение последовательности байт из регистров устройства
// возвращает 0 в случае успешной передачи данных
// возвращает НЕ 0 в случае ошибок при приеме 
uint16_t i2с_read_seq_byte (uint8_t deviceAddr, uint8_t regStartAddr, uint8_t num, uint8_t *d) {
	
	uint16_t errCounter = 0;
	
	if (!num) return 1;
	if (deviceAddr) return 1;
	if (!d) return 1;
	
	//*** критическая секция 
	cpu_irq_disable();
	
	i2c_reset_errors();
	
	// настройка 
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
	
	// отправка адреса 
	if (num > 1) I2C->TWIM_THR = (regStartAddr|AUTO_INC);
	else I2C->TWIM_THR = regStartAddr;
	
	errCounter += i2c_wait_idle_tx();	
	// чтение 
	for (uint8_t i = 0; i < num; i++) {
		errCounter += i2c_wait_idle_rx();
		d[i] = I2C->TWIM_RHR;
	}
	
	//*** критическая секция 
	cpu_irq_enable();
	
	return errCounter;
}

/*
// прием и передача одного байта 
void i2c_write_byte (uint8_t byte) {
	I2C->TWIM_THR = byte; //TWIM_THR_TXDATA(data);
}
uint8_t i2c_read_byte (void){
	return I2C->TWIM_RHR;
}
// настройка модуля для передачи данных
void i2c_start_write_ (uint8_t adr, uint8_t num) {
	I2C->TWIM_CMDR =
	TWIM_CMDR_SADR(adr>>1)	// D6 - гироскоп  BA - барометр 32 - акселерометр 3C
	| TWIM_CMDR_NBYTES(num)
	| TWIM_CMDR_VALID
	| TWIM_CMDR_START
	| TWIM_CMDR_STOP;	
}
// настройка модуля для приема 
void i2c_start_read_ (uint8_t adr, uint8_t num) {
	I2C->TWIM_CMDR =
	TWIM_CMDR_SADR(adr>>1)	// D6 - гироскоп  BA - барометр 32 - акселерометр 3C
	| TWIM_CMDR_NBYTES(num)
	| TWIM_CMDR_VALID
	| TWIM_CMDR_START
	| TWIM_CMDR_STOP
	| TWIM_CMDR_READ;	
}
*/