#include "I2cExpander.h"

#define INPUT_REG 0x00	// input register location
#define OUTPUT_REG 0x02	// output register location
#define INVERT_REG 0x04	// polarity inversion register location
#define CONFIG_REG 0x06	// configuration register location

I2cExpander::I2cExpander()
{
}
I2cExpander::~I2cExpander()
{
}
void I2cExpander::begin(uint8_t _address, uint8_t _sda, uint8_t _scl)
{
	address = _address;
	Wire.begin(_sda, _scl);
	portWrite(0xFFFF);
	_write(0x0000, INVERT_REG);
	portConfig(0xFFFF);
}
void I2cExpander::_write(uint16_t _portData, uint8_t reg)
{
	Wire.beginTransmission(address);
	Wire.write(reg);
	// Wire.write(_portData);
	Wire.write(_portData & 0x00FF);
	Wire.write(_portData >> 8);
	Wire.endTransmission();
}
uint16_t I2cExpander::portRead()
{
	Wire.beginTransmission(address);
	Wire.write(INPUT_REG);
	Wire.endTransmission();
	Wire.requestFrom(address, 2);
	uint16_t readValue = Wire.read();
	readValue |= Wire.read() << 8;
	// Wire.readBytes((uint8_t*)readValue, 2);
	return readValue;
}
void I2cExpander::portConfig(uint16_t _portData)
{
	configState = _portData;
	_write(configState, CONFIG_REG);
}
void I2cExpander::portWrite(uint16_t writeData)
{
	outputState = writeData;
	_write(writeData, OUTPUT_REG);
}
void I2cExpander::pinMode(uint8_t pin, uint8_t mode)
{
	uint16_t configMask = 1 << pin;
	if(mode == OUTPUT)
		configState &= ~configMask;
	else
		configState |= configMask;
	
	portConfig(configState);
}
bool I2cExpander::pinRead(uint8_t pin)
{
	uint16_t _data = portRead();
	return bitRead(_data, pin);
}
void I2cExpander::pinWrite(uint8_t pin, bool state)
{
	uint16_t outputMask = 1 << pin;
	if(state)
		outputState |= outputMask;
	else
		outputState &= ~outputMask;
	portWrite(outputState);
}
