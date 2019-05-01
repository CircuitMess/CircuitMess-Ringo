/*
||
|| @file Keypad_I2C.h
|| @version 3.0 - PCA9554, PCA9554A, PCA9555 support G. D. Young
|| @version 2.0 - PCF8575 support added by Paul Williamson
|| @author G. D. (Joe) Young, ptw
|| @contact "G. D. (Joe) Young" <gdyoung@telus.net>
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

#ifndef KEYPAD_I2Ca_H
#define KEYPAD_I2Ca_H

#include "../Keypad/src/Keypad.h"

// literals to use to define the width of the port--last parameter in constructors
#define PCA9554 1	// PCA9554 I/O expander device is 1 byte wide
#define PCA9555 2	// PCA9555 I/O expander device is 2 bytes wide
#define PCA9539 2
#define PCA9534 1
#define PCA9535 2
#define TCA6408 1
#define TCA6416 2


class Keypad_I2Ca : public Keypad {
public:
	Keypad_I2Ca(char* userKeymap, byte* row, byte* col, byte numRows, byte numCols, byte address, byte width = 1) :
		Keypad(userKeymap, row, col, numRows, numCols) { i2caddr = address; i2cwidth = width;}	
	

	// Keypad function
	void begin(char *userKeymap);
	// Wire function
	void begin(void);
	// Wire function
	void begin(byte address);
	// Wire function
	void begin(int address);

	void pin_mode(byte pinNum, byte mode);
	void pin_write(byte pinNum, boolean level);
	int  pin_read(byte pinNum);
	// read initial value for pinState
	word pinState_set( );
	// write a whole byte or word (depending on the port expander chip) to i2c
	void port_write( word i2cportval );
	// access functions for IODIR state copy
	word iodir_read( );
	void iodir_write( word iodir );

private:
    // I2C device address
    byte i2caddr;
    // I2C port expander device width in bytes (1 for 8574, 2 for 8575)
    byte i2cwidth;
	// I2C pin_write state persistant storage
	// least significant byte is used for 8-bit port expanders
	word pinState;
	word iodir_state;    // copy of IODIR register
	void _begin( void );
	void p_write( word i2cportval, byte reg );
};



#endif // KEYPAD_I2Ca_H

/*
|| @changelog
|| |
|| | 3.0 2014-05-22 - GDY support totem-pole output ports PCA9554(8-bit), PCA9555(16-bit)
|| |
|| | 2.0 2013-08-31 - Paul Williamson : Added i2cwidth parameter for PCF8575 support
|| |
|| | 1.0 2012-07-12 - Joe Young : Initial Release
|| #
*/
