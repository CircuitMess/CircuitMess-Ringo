/*
||
|| @file Keypad_I2C.h
|| @version 2.0 - PCF8575 support added by Paul Williamson
|| @author G. D. (Joe) Young, ptw
|| @contact "G. D. (Joe) Young" <jyoung@islandnet.com>
||
|| @description
|| | Keypad_I2C provides an interface for using matrix keypads that
|| | are attached with I2C port expanders. It supports multiple keypads,
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

#include "Keypad_I2C.h"
// number of days since 2000/01/01, valid for 2001..2099
const uint8_t daysInMonth [12] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };
static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
	if (y >= 2000)
		y -= 2000;
	uint16_t days = d;
	for (uint8_t i = 1; i < m; ++i)
		days += pgm_read_byte(daysInMonth + i - 1);
	if (m > 2 && y % 4 == 0)
		++days;
	return days + 365 * y + (y + 3) / 4 - 1;
}
static uint8_t conv2d(const char* p) {
	uint8_t v = 0;
	if ('0' <= *p && *p <= '9')
		v = *p - '0';
	return 10 * v + *++p - '0';
}
static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
	return ((days * 24L + h) * 60 + m) * 60 + s;
}

DateTime::DateTime (uint32_t t) {
	t -= 946684800;	// bring to 2000 timestamp from 1970

	ss = t % 60;
	t /= 60;
	mm = t % 60;
	t /= 60;
	hh = t % 24;
	uint16_t days = t / 24;
	uint8_t leap;
	for (yOff = 0; ; ++yOff) {
		leap = yOff % 4 == 0;
		if (days < 365 + leap)
			break;
		days -= 365 + leap;
	}
	for (m = 1; ; ++m) {
		uint8_t daysPerMonth = pgm_read_byte(daysInMonth + m - 1);
		if (leap && m == 2)
			++daysPerMonth;
		if (days < daysPerMonth)
			break;
		days -= daysPerMonth;
	}
	d = days + 1;
}

DateTime::DateTime (uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec) {
	if (year >= 2000)
		year -= 2000;
	yOff = year;
	m = month;
	d = day;
	hh = hour;
	mm = min;
	ss = sec;
}

// A convenient constructor for using "the compiler's time":
//   DateTime now (__DATE__, __TIME__);
// NOTE: using PSTR would further reduce the RAM footprint
DateTime::DateTime (const char* date, const char* time) {
	// sample input: date = "Dec 26 2009", time = "12:34:56"
	// or date="26-12-2009"
	yOff = conv2d(date + 9);
	// Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec 
	d = conv2d(date + 4);
	switch (date[0]) {
		case 'J': m = date[1] == 'a' ? 1 : m = date[2] == 'n' ? 6 : 7; break;
		case 'F': m = 2; break;
		case 'A': m = date[2] == 'r' ? 4 : 8; break;
		case 'M': m = date[2] == 'r' ? 3 : 5; break;
		case 'S': m = 9; break;
		case 'O': m = 10; break;
		case 'N': m = 11; break;
		case 'D': m = 12; break;
		default:
		  yOff = conv2d(date + 8);
		  d = conv2d(date);
		  m = conv2d(date + 3);
	}
	hh = conv2d(time);
	mm = conv2d(time + 3);
	ss = conv2d(time + 6);
}

// A convenient constructor for using "the compiler's time":
// This version will save RAM by using PROGMEM to store it by using the F macro.
// DateTime now (F(__DATE__), F(__TIME__));
DateTime::DateTime (const __FlashStringHelper* date, const __FlashStringHelper* time) {
	// sample input: date = "Dec 26 2009", time = "12:34:56"
	char _date[11];
	char _time[8];
	memcpy_P(_date, date, 11);
	memcpy_P(_time, time, 8);
	DateTime(date, time);
}

DateTime::DateTime(const char* sdate) {
  DateTime(sdate, sdate+11);
}

/*********************************************/
/*  Comparison							   */
/*********************************************/

unsigned char DateTime::equals(const char* sdate) const
{
	return equals(DateTime(sdate));
}
unsigned char DateTime::equals(const DateTime &date) const
{
	return yOff == date.year()-2000 && m == date.month() && d == date.day() && hh == date.hour() && mm == date.minute() && ss == date.second();
}
unsigned char DateTime::operator<(const DateTime &date) const
{
	return unixtime() < date.unixtime();
}
unsigned char DateTime::operator>(const DateTime &date) const
{
	return unixtime() > date.unixtime();
}
unsigned char DateTime::operator<=(const DateTime &date) const
{
	return unixtime() <= date.unixtime();
}
unsigned char DateTime::operator>=(const DateTime &date) const
{
	return unixtime() >= date.unixtime();
}

uint8_t DateTime::dayOfWeek() const {
	uint16_t day = date2days(yOff, m, d);
	return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
}

uint32_t DateTime::unixtime(void) const {
	uint32_t t;
	uint16_t days = date2days(yOff, m, d);
	t = time2long(days, hh, mm, ss);
	t += 946684800;  // seconds from 1970 to 2000

	return t;
}

char* DateTime::format(char* ret){
		for(int i=0;i<strlen(ret)-1;i++){
		if(ret[i] == 'h' && ret[i+1] == 'h'){
			ret[i] = '0'+hh/10;
			ret[i+1] = '0'+hh%10;
		}
		if(ret[i] == 'm' && ret[i+1] == 'm'){
			ret[i] = '0'+mm/10;
			ret[i+1] = '0'+mm%10;
		}
		if(ret[i] == 's' && ret[i+1] == 's'){
			ret[i] = '0'+ss/10;
			ret[i+1] = '0'+ss%10;
		}
		if(ret[i] == 'D' && ret[i+1] == 'D'){
			ret[i] = '0'+d/10;
			ret[i+1] = '0'+d%10;
		}
		if(ret[i] == 'M' && ret[i+1] == 'M'){
			ret[i] = '0'+m/10;
			ret[i+1] = '0'+m%10;
		}
		if(ret[i] == 'Y'&& ret[i+3] == 'Y'){
			ret[i] = '2';
			ret[i+1] = '0';
			ret[i+2] = '0'+yOff/10;
			ret[i+3] = '0'+yOff%10;
		}else
		if(ret[i] == 'Y'&& ret[i+1] == 'Y'){
			ret[i] = '0'+yOff/10;
			ret[i+1] = '0'+yOff%10;
		}

	}
	return ret;
}
// Let the user define a keymap - assume the same row/column count as defined in constructor
void Keypad_I2C::begin(char *userKeymap) {
    Keypad::begin(userKeymap);
	TwoWire::begin();
	pinState = pinState_set( );
}


// Initialize I2C
void Keypad_I2C::begin(uint8_t SDA, uint8_t SCL) {
	TwoWire::begin(SDA, SCL);
//	pinState = 0xff;
	pinState = pinState_set( );
}

// Initialize I2C
void Keypad_I2C::begin(byte address) {
	i2caddr = address;
	TwoWire::begin(address);
//	pinState = 0xff;
	pinState = pinState_set( );
}

// Initialize I2C
void Keypad_I2C::begin(int address) {
	i2caddr = address;
	TwoWire::begin(address);
//	pinState = 0xff;
	pinState = pinState_set( );
}


void Keypad_I2C::pin_write(byte pinNum, boolean level) {
	word mask = 1<<pinNum;
	if( level == HIGH ) {
		pinState |= mask;
	} else {
		pinState &= ~mask;
	}
	port_write( pinState );
} // I2CxWrite( )


int Keypad_I2C::pin_read(byte pinNum) {
	word mask = 0x1<<pinNum;
	TwoWire::requestFrom((int)i2caddr, (int)i2cwidth);
	word pinVal = TwoWire::read( );
	if (i2cwidth > 1) {
		pinVal |= TwoWire::read( ) << 8;
	} 
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
}

void Keypad_I2C::port_write( word i2cportval ) {
	TwoWire::beginTransmission((int)i2caddr);
	TwoWire::write( i2cportval & 0x00FF);
	if (i2cwidth > 1) {
		TwoWire::write( i2cportval >> 8 );
	}
	TwoWire::endTransmission();
	pinState = i2cportval;
} // port_write( )

word Keypad_I2C::pinState_set( ) {
	TwoWire::requestFrom( (int)i2caddr, (int)i2cwidth );
	pinState = TwoWire::read( );
	if (i2cwidth > 1) {
		pinState |= TwoWire::read( ) << 8;
	}
	return pinState;
} // set_pinState( )


  #define PT2257_ADDR 0x44        //Chip address
  #define EVC_OFF     0b11111111  //Function OFF (-79dB)
  #define EVC_2CH_1   0b11010000  //2-Channel, -1dB/step
  #define EVC_2CH_10  0b11100000  //2-Channel, -10dB/step
  #define EVC_L_1     0b10100000  //Left Channel, -1dB/step
  #define EVC_L_10    0b10110000  //Left Channel, -10dB/step
  #define EVC_R_1     0b00100000  //Right Channel, -1dB/step
  #define EVC_R_10    0b00110000  //Right Channel, -10dB/step
  #define EVC_MUTE    0b01111000  //2-Channel MUTE
  
  

void Keypad_I2C::writeVolume(uint8_t dB) {
	byte bbbaaaa = level(dB);

	byte aaaa = bbbaaaa & 0b00001111;
	byte bbb = (bbbaaaa >> 4) & 0b00001111;

	TwoWire::beginTransmission(PT2257_ADDR);
	TwoWire::write(EVC_2CH_10 | bbb);
	TwoWire::write(EVC_2CH_1 | aaaa);
	TwoWire::endTransmission();
}
void Keypad_I2C::writeVolumeRight(uint8_t dB) {
	byte bbbaaaa = level(dB);

	byte aaaa = bbbaaaa & 0b00001111;
	byte bbb = (bbbaaaa >> 4) & 0b00001111;

	TwoWire::beginTransmission(PT2257_ADDR);
	TwoWire::write(EVC_R_10 | bbb);
	TwoWire::write(EVC_R_1 | aaaa);
	TwoWire::endTransmission();
	
}

void Keypad_I2C::writeVolumeLeft(uint8_t dB) {
	byte bbbaaaa = level(dB);

	byte aaaa = bbbaaaa & 0b00001111;
	byte bbb = (bbbaaaa >> 4) & 0b00001111;

	TwoWire::write(PT2257_ADDR);
	TwoWire::write(EVC_L_10 | bbb);
	TwoWire::write(EVC_L_1 | aaaa);
	TwoWire::endTransmission();
	
}

uint8_t Keypad_I2C::level(uint8_t dB) {

	if (dB > 79) dB = 79;

	int b = dB / 10; //get the second digit
	int a = dB % 10; //get the first digit

	b = b & 0b0000111; //limit the most significant digit to 3 bit (7)

	return (b << 4) | a; //return both numbers in one byte (0BBBAAAA)
}

void Keypad_I2C::writeMute(bool state) {
	TwoWire::beginTransmission(PT2257_ADDR);
	TwoWire::write(EVC_MUTE | (state & 0b00000001));
	TwoWire::endTransmission();
}



//DS3231 FUNCTIONS BELOW ###############################################################

#define CLOCK_ADDRESS 0x68

#define SECONDS_FROM_1970_TO_2000 946684800




byte Keypad_I2C::getSecond() {
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x00);
	TwoWire::endTransmission();

	TwoWire::requestFrom(CLOCK_ADDRESS, 1);
	return bcdToDec(TwoWire::read());
}

byte Keypad_I2C::getMinute() {
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x01);
	TwoWire::endTransmission();

	TwoWire::requestFrom(CLOCK_ADDRESS, 1);
	return bcdToDec(TwoWire::read());
}

byte Keypad_I2C::getHour(bool& h12, bool& PM) {
	byte temp_buffer;
	byte hour;
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x02);
	TwoWire::endTransmission();

	TwoWire::requestFrom(CLOCK_ADDRESS, 1);
	temp_buffer = TwoWire::read();
	h12 = temp_buffer & 0b01000000;
	if (h12) {
		PM = temp_buffer & 0b00100000;
		hour = bcdToDec(temp_buffer & 0b00011111);
	}
	else {
		hour = bcdToDec(temp_buffer & 0b00111111);
	}
	return hour;
}

byte Keypad_I2C::getDoW() {
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x03);
	TwoWire::endTransmission();

	TwoWire::requestFrom(CLOCK_ADDRESS, 1);
	return bcdToDec(TwoWire::read());
}

byte Keypad_I2C::getDate() {
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x04);
	TwoWire::endTransmission();

	TwoWire::requestFrom(CLOCK_ADDRESS, 1);
	return bcdToDec(TwoWire::read());
}

byte Keypad_I2C::getMonth(bool& Century) {
	byte temp_buffer;
	byte hour;
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x05);
	TwoWire::endTransmission();

	TwoWire::requestFrom(CLOCK_ADDRESS, 1);
	temp_buffer = TwoWire::read();
	Century = temp_buffer & 0b10000000;
	return (bcdToDec(temp_buffer & 0b01111111));
}

byte Keypad_I2C::getYear() {
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x06);
	TwoWire::endTransmission();

	TwoWire::requestFrom(CLOCK_ADDRESS, 1);
	return bcdToDec(TwoWire::read());
}

void Keypad_I2C::setSecond(byte Second) {
	// Sets the seconds 
	// This function also resets the Oscillator Stop Flag, which is set
	// whenever power is interrupted.
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x00);
	TwoWire::write(decToBcd(Second));
	TwoWire::endTransmission();
	// Clear OSF flag
	byte temp_buffer = readControlByte(1);
	writeControlByte((temp_buffer & 0b01111111), 1);
}

void Keypad_I2C::setMinute(byte Minute) {
	// Sets the minutes 
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x01);
	TwoWire::write(decToBcd(Minute));
	TwoWire::endTransmission();
}

void Keypad_I2C::setHour(byte Hour) {
	// Sets the hour, without changing 12/24h mode.
	// The hour must be in 24h format.

	bool h12;

	// Start by figuring out what the 12/24 mode is
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x02);
	TwoWire::endTransmission();
	TwoWire::requestFrom(CLOCK_ADDRESS, 1);
	h12 = (TwoWire::read() & 0b01000000);
	// if h12 is true, it's 12h mode; false is 24h.

	if (h12) {
		// 12 hour
		if (Hour > 12) {
			Hour = decToBcd(Hour - 12) | 0b01100000;
		}
		else {
			Hour = decToBcd(Hour) & 0b11011111;
		}
	}
	else {
		// 24 hour
		Hour = decToBcd(Hour) & 0b10111111;
	}

	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x02);
	TwoWire::write(Hour);
	TwoWire::endTransmission();
}

void Keypad_I2C::setDoW(byte DoW) {
	// Sets the Day of Week
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x03);
	TwoWire::write(decToBcd(DoW));
	TwoWire::endTransmission();
}

void Keypad_I2C::setDate(byte Date) {
	// Sets the Date
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x04);
	TwoWire::write(decToBcd(Date));
	TwoWire::endTransmission();
}

void Keypad_I2C::setMonth(byte Month) {
	// Sets the month
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x05);
	TwoWire::write(decToBcd(Month));
	TwoWire::endTransmission();
}

void Keypad_I2C::setYear(byte Year) {
	// Sets the year
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x06);
	TwoWire::write(decToBcd(Year));
	TwoWire::endTransmission();
}

void Keypad_I2C::setClockMode(bool h12) {
	// sets the mode to 12-hour (true) or 24-hour (false).
	// One thing that bothers me about how I've written this is that
	// if the read and right happen at the right hourly millisecnd,
	// the clock will be set back an hour. Not sure how to do it better, 
	// though, and as long as one doesn't set the mode frequently it's
	// a very minimal risk. 
	// It's zero risk if you call this BEFORE setting the hour, since
	// the setHour() function doesn't change this mode.

	byte temp_buffer;

	// Start by reading byte 0x02.
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x02);
	TwoWire::endTransmission();
	TwoWire::requestFrom(CLOCK_ADDRESS, 1);
	temp_buffer = TwoWire::read();

	// Set the flag to the requested value:
	if (h12) {
		temp_buffer = temp_buffer | 0b01000000;
	}
	else {
		temp_buffer = temp_buffer & 0b10111111;
	}

	// Write the byte
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	TwoWire::write(0x02);
	TwoWire::write(temp_buffer);
	TwoWire::endTransmission();
}

//PRIVATE FUNCTIONS:

byte Keypad_I2C::decToBcd(byte val) {
	// Convert normal decimal numbers to binary coded decimal
	return ((val / 10 * 16) + (val % 10));
}

byte Keypad_I2C::bcdToDec(byte val) {
	// Convert binary coded decimal to normal decimal numbers
	return ((val / 16 * 10) + (val % 16));
}

byte Keypad_I2C::readControlByte(bool which) {
	// Read selected control byte
	// first byte (0) is 0x0e, second (1) is 0x0f
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	if (which) {
		// second control byte
		TwoWire::write(0x0f);
	}
	else {
		// first control byte
		TwoWire::write(0x0e);
	}
	TwoWire::endTransmission();
	TwoWire::requestFrom(CLOCK_ADDRESS, 1);
	return TwoWire::read();
}

void Keypad_I2C::writeControlByte(byte control, bool which) {
	// Write the selected control byte.
	// which=false -> 0x0e, true->0x0f.
	TwoWire::beginTransmission(CLOCK_ADDRESS);
	if (which) {
		TwoWire::write(0x0f);
	}
	else {
		TwoWire::write(0x0e);
	}
	TwoWire::write(control);
	TwoWire::endTransmission();
}


/*
|| @changelog
|| |
|| | 2.0 2013-08-31 - Paul Williamson : Added i2cwidth parameter for PCF8575 support
|| |
|| | 1.0 2012-07-12 - Joe Young : Initial Release
|| #
*/
