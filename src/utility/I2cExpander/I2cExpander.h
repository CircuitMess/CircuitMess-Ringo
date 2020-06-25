#include <Arduino.h>
#include <Wire.h>

#ifndef CIRCUITOS_I2C_EXPANDER_H
#define CIRCUITOS_I2C_EXPANDER_H
/*
Made for Texas Instruments 16-bit GPIO expanders
(e.g. PCF9539)
*/

class I2cExpander
{
public:
	I2cExpander();
	~I2cExpander();
	void begin(uint8_t _address = 0x74, uint8_t _sda = 27, uint8_t _scl = 14);
	uint16_t portRead();
	void pinMode(uint8_t pin, uint8_t mode);
	bool pinRead(uint8_t pin);
	void pinWrite(uint8_t pin, bool state);
	void portWrite(uint16_t _portData);
	void portConfig(uint16_t _portData);
	
private:
	void _write(uint16_t port, uint8_t reg);
	uint8_t address = 0;
	uint16_t configState = 0;
	uint16_t outputState = 0;
};

#endif