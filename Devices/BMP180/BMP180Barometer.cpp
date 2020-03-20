#include "BMP180Barometer.h"

using namespace IoT::Device;

bool BMP180Barometer::initialize() {
	if ((m_device = wiringPiI2CSetup (m_address)) < 0)
		return false;

	// Read calibration data
	AC1 = read16 (m_device, BMP085_CAL_AC1) ;
	AC2 = read16 (m_device, BMP085_CAL_AC2) ;
	AC3 = read16 (m_device, BMP085_CAL_AC3) ;
	AC4 = read16 (m_device, BMP085_CAL_AC4) ;
	AC5 = read16 (m_device, BMP085_CAL_AC5) ;
	AC6 = read16 (m_device, BMP085_CAL_AC6) ;
	VB1 = read16 (m_device, BMP085_CAL_B1) ;
	VB2 = read16 (m_device, BMP085_CAL_B2) ;
	MB  = read16 (m_device, BMP085_CAL_MB) ;
	MC  = read16 (m_device, BMP085_CAL_MC) ;
	MD  = read16 (m_device, BMP085_CAL_MD) ;

	return true;
}

void BMP180Barometer::acquire(BarometerData & data) {
	long UT = readRawTemperature();
	long UP = readRawPressure();

	// Temperature Calculation
	long X1 = ((UT - AC6) * AC5) >> 15;
	long X2 = (MC << 11) / (X1 + MD);
	long B5 = X1 + X2;
	data.setTemperature(((B5 + 8) >> 4) * .1f);


	// Pressure Calculations
	long B6 = B5 - 4000;

	X1 = (VB2 * (B6 * B6) >> 12) >> 11;
	X2 = (AC2 * B6) >> 11;
	long X3 = X1 + X2;
	long B3 = (((AC1 * 4 + X3) << m_mode) + 2) / 4;

	X1 = (AC3 * B6) >> 13;
	X2 = (VB1 * ((B6 * B6) >> 12)) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;
	auto B4 = (uint64_t)((AC4 * (X3 + 32768)) >> 15);
	auto B7 = (uint64_t)((UP - B3) * (50000 >> m_mode));

	long p;
	if (B7 < 0x80000000)
		p = (long)((B7 * 2) / B4);
	else
		p = (long)((B7 / B4) * 2);

	X1 = (p >> 8) * (p >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * p) >> 16;
	p += (X1 + X2 + 3791) >> 4;

	data.setPressure(p);
}

long BMP180Barometer::readRawPressure() {
	uint8_t data [3] ;

	// Start a pressure snsor reading
	wiringPiI2CWriteReg8 (m_device, BMP085_CONTROL, BMP085_READPRESSURECMD | (m_mode << 6)) ;

	// Sleep
	uint32_t s;
	if (m_mode == BMP085_ULTRALOWPOWER)
		s = 5;
	else if (m_mode == BMP085_HIGHRES)
		s = 14;
	else if (m_mode == BMP085_ULTRAHIGHRES)
		s = 26;
	else
		s = 8;
	delay(s) ;

	// Read the raw data
	uint16_t reg = uint16_t(wiringPiI2CReadReg16(m_device, BMP085_PRESSUREDATA | auto_increment));
	data [0] = uint8_t(reg);
	data [1] = uint8_t(reg >> 8);
	data [2] = uint8_t(wiringPiI2CReadReg8(m_device, BMP085_PRESSUREDATA + 2)) ;

	return (data[0] << 16 | data[1] << 8 | data[2]) >> (8 - m_mode);
}

long BMP180Barometer::readRawTemperature() {
	// Start a temperature sensor reading
	wiringPiI2CWriteReg8 (m_device, BMP085_CONTROL, BMP085_READTEMPCMD) ;
	delay (5) ;

	// Read the raw data
	uint16_t data = uint16_t(wiringPiI2CReadReg16(m_device, BMP085_TEMPDATA | auto_increment));
	return uint8_t(data) << 8 | uint8_t(data >> 8);
}
