#include "altImuFuncs.h"
#include "altImuRegs.h"
#include "altImuI2C.h"

extern uint8_t *pErrorText; 

// «начени€ регистров дл€ инициализации датчиков 
static uint8_t g_init [5] = {
	0b00111100, // частота обновлени§ данных ODR = 95, внутренний фильтр Cut-off = 25, оси = Z
	0b00000000,	// High-pass filter = Reference signal for filtering
	0b00000000,	// default 
	0b10000000,	// данные не обновл§ютс§ при чтении, шкала = 250 град/сек, младший адрес = младший байт
	0b00000000	// def FIFO выкл, СђД - выкл
};
static uint8_t a_init [6] = {
	0b01010111, // частота измерени§ 100 vц, измерение по трем ос§м
	0b00000000, // default
	0b00000000, // default
	0b10000000, // данные не обновл§ютс§ при чтении, диапазон измерени§ +/- 2G
	0b00000000, // default
	0b00000000  // default
};
static uint8_t c_init[3] = {
	0b00011000, // частота измерени§ 75 vц
	0b00100000, // диапазон измерени§ +/- 1.3 vаусс
	0b00000000 // Continuous-conversion mode
};

// ‘”Ќ ÷»» »Ќ»÷»јЋ»«ј÷»» ƒј“„» ќ¬ 
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