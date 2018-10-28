/*
||
|| @file Keypad_I2C.h
|| @version 2.0 - PCF8575 support added by Paul Williamson
|| @author G. D. (Joe) Young, ptw
|| @contact "G. D. (Joe) Young" <jyoung@islandnet.com>
||
|| @description
|| | Keypad_I2C provides an interface for using matrix keypads that
|| | are attached to I2C port expanders. It supports multiple keypads,
|| | user selectable pins, and user defined keymaps.
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/

#ifndef KEYPAD_I2C_H
#define KEYPAD_I2C_H

#include "../Keypad.h"
#include "Wire2.h"

//TwoWire i2c = TwoWire(0);

#define	PCF8574	1	// PCF8574 I/O expander device is 1 byte wide
#define PCF8575 2	// PCF8575 I/O expander device is 2 bytes wide


class DateTime {
public:
	DateTime(uint32_t t = 0);
	DateTime(uint16_t year, uint8_t month, uint8_t day,
		uint8_t hour = 0, uint8_t min = 0, uint8_t sec = 0);
	DateTime(const char* date, const char* time);
	uint16_t year() const { return 2000 + yOff; }
	uint8_t month() const { return m; }
	uint8_t day() const { return d; }
	uint8_t hour() const { return hh; }
	uint8_t minute() const { return mm; }
	uint8_t second() const { return ss; }
	uint8_t dayOfTheWeek() const;

	// 32-bit times as seconds since 1/1/2000
	long secondstime() const;
	// 32-bit times as seconds since 1/1/1970
	// THE ABOVE COMMENT IS CORRECT FOR LOCAL TIME; TO USE THIS COMMAND TO
	// OBTAIN TRUE UNIX TIME SINCE EPOCH, YOU MUST CALL THIS COMMAND AFTER
	// SETTING YOUR CLOCK TO UTC
	uint32_t unixtime(void) const;

protected:
	uint8_t yOff, m, d, hh, mm, ss;
};

class Keypad_I2C : public Keypad, public TwoWire {
public:
	Keypad_I2C(char* userKeymap, byte* row, byte* col, byte numRows, byte numCols, byte address, byte width = 1) :
		
		Keypad(userKeymap, row, col, numRows, numCols) {
		i2caddr = address;
		i2cwidth = width;
		
		uint8_t volume = 0;

	}

	//DS3231 FUNCTIONS #######################################
	byte getSecond();
	byte getMinute();
	byte getHour(bool& h12, bool& PM);
	// In addition to returning the hour register, this function
	// returns the values of the 12/24-hour flag and the AM/PM flag.
	byte getDoW();
	byte getDate();
	byte getMonth(bool& Century);
	// Also sets the flag indicating century roll-over.
	byte getYear();
	// Last 2 digits only

	// Time-setting functions
	// Note that none of these check for sensibility: You can set the
	// date to July 42nd and strange things will probably result.

	void setSecond(byte Second);
	// In addition to setting the seconds, this clears the 
	// "Oscillator Stop Flag".
	void setMinute(byte Minute);
	// Sets the minute
	void setHour(byte Hour);
	// Sets the hour
	void setDoW(byte DoW);
	// Sets the Day of the Week (1-7);
	void setDate(byte Date);
	// Sets the Date of the Month
	void setMonth(byte Month);
	// Sets the Month of the year
	void setYear(byte Year);
	// Last two digits of the year
	void setClockMode(bool h12);
	// Set 12/24h mode. True is 12-h, false is 24-hour.
	
	byte decToBcd(byte val);
	// Convert normal decimal numbers to binary coded decimal
	byte bcdToDec(byte val);
	// Convert binary coded decimal to normal decimal numbers
	byte readControlByte(bool which);
	// Read selected control byte: (0); reads 0x0e, (1) reads 0x0f
	void writeControlByte(byte control, bool which);


	//PT2257 FUNCTIONS ######################################
	void writeVolumeLeft(uint8_t dB);
	void writeVolumeRight(uint8_t dB);
	void writeMute(bool state);
	void writeVolume(uint8_t dB);
	uint8_t level(uint8_t dB);

	// Keypad function
	void begin(char *userKeymap);
	// Wire function
	void begin(uint8_t SDA, uint8_t SCL);
	// Wire function
	void begin(byte address);
	// Wire function
	void begin(int address);

	void pin_mode(byte pinNum, byte mode) {}
	void pin_write(byte pinNum, boolean level);
	int  pin_read(byte pinNum);
	// read initial value for pinState
	word pinState_set( );
	// write a whole byte or word (depending on the port expander chip) to i2c port
	void port_write( word i2cportval );

private:
    // I2C device address
    byte i2caddr;
    // I2C port expander device width in bytes (1 for 8574, 2 for 8575)
    byte i2cwidth;
	// I2C pin_write state persistant storage
	// least significant byte is used for 8-bit port expanders
	word pinState;
};



#endif // KEYPAD_I2C_H

/*
|| @changelog
|| |
|| | 2.0 2013-08-31 - Paul Williamson : Added i2cwidth parameter for PCF8575 support
|| |
|| | 1.0 2012-07-12 - Joe Young : Initial Release
|| #
*/
