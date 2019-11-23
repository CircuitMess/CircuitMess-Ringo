// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!

#include <Wire.h>
#include "RTClib.h"
#ifdef __AVR__
 #include <avr/pgmspace.h>
 #define WIRE Wire
#else
 #define PROGMEM
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
 #define WIRE Wire
#endif

#define DS1307_ADDRESS			  0x68
#define PCF8563_ADDRESS			 0xA3
#define PCF8583_ADDRESS			 0xA0

#define PCF8563_ALARM_REG_OFF  0x80
#define PCF8563_ALARM_AIE		   0x02
#define PCF8563_ALARM_AF			0x08 // 0x08 : not 0x04!!!!
/* optional val for no alarm setting */
#define PCF8563_NO_ALARM			0x99

// i2c slave address of the DS3231 chip
#define DS3231_ADDRESS			  0x68

// registers
#define DS3231_TIME_ADDR			0x00
#define DS3231_ALARM_1_ADDR		 0x07
#define DS3231_ALARM_2_ADDR		 0x0B
#define DS3231_CONTROL_ADDR		 0x0E
#define DS3231_STATUS_ADDR		  0x0F
#define DS3231_AGING_OFFSET_ADDR	0x10
#define DS3231_TEMPERATURE_ADDR	 0x11

// control bits
#define DS3231_ALARM_1_IE		   0x1
#define DS3231_ALARM_2_IE		   0x2
#define DS3231_INTC				 0x4

// status bits
#define DS3231_ALARM_1_AF		   0x1
#define DS3231_ALARM_2_AF		   0x2
#define DS3231_OSF				  0x80

#define SECONDS_PER_DAY 86400L

#define SECONDS_FROM_1970_TO_2000 946684800

#if (ARDUINO >= 100)
 #include <Arduino.h> // capital A so it is error prone on case-sensitive filesystems
#else
 #include <WProgram.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// utility code, some of this could be exposed in the DateTime API if needed
static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

const uint8_t daysInMonth [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };

// number of days since 2000/01/01, valid for 2001..2099
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

static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
	return ((days * 24L + h) * 60 + m) * 60 + s;
}

static uint8_t conv2d(const char* p) {
	uint8_t v = 0;
	if ('0' <= *p && *p <= '9')
		v = *p - '0';
	return 10 * v + *++p - '0';
}

uint8_t _read(const int dev, const uint8_t addr) {
  WIRE.beginTransmission(dev);
  WIRE.write(addr);
  WIRE.endTransmission();
  WIRE.requestFrom(dev, 1);
  uint8_t s = WIRE.read();
  return s;
}
void _write(const int dev, const uint8_t addr, const uint8_t val) {
  WIRE.beginTransmission(dev);
  WIRE.write(addr);
  WIRE.write(val);
  WIRE.endTransmission();
}

////////////////////////////////////////////////////////////////////////////////
// DateTime implementation - ignores time zones and DST changes
// NOTE: also ignores leap seconds, see http://en.wikipedia.org/wiki/Leap_second

DateTime::DateTime (uint32_t t) {
	t -= SECONDS_FROM_1970_TO_2000;	// bring to 2000 timestamp from 1970

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
		case 'J': m = date[1] == 'a' ? 1 : date[2] == 'n' ? 6 : 7; break;
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
	t += SECONDS_FROM_1970_TO_2000;  // seconds from 1970 to 2000

	return t;
}

DateTime DateTime::operator + (const TimeDelta& delta) {
	return DateTime(unixtime() + delta.totalseconds());
}
DateTime DateTime::operator - (const TimeDelta& delta) {
	return DateTime(unixtime() - delta.totalseconds());
}
TimeDelta DateTime::operator - (const DateTime& right) {
	return TimeDelta(unixtime() - right.unixtime());
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

////////////////////////////////////////////////////////////////////////////////
// TimeDelta implementation
TimeDelta::TimeDelta (uint32_t seconds):
_sec(seconds)
{}
TimeDelta::TimeDelta (uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds):
	_sec(days*86400L + hours*3600 + minutes*60 + seconds){}
TimeDelta::TimeDelta (const TimeDelta& copy):
	_sec(copy._sec){}
TimeDelta TimeDelta::operator + (const TimeDelta& right) {
	return TimeDelta(_sec + right._sec);
}
TimeDelta TimeDelta::operator-(const TimeDelta& right) {
	return TimeDelta(_sec - right._sec);
}

////////////////////////////////////////////////////////////////////////////////

// RTC_DS1302 implementation
DS1302:: DS1302 (uint8_t ce_pin, uint8_t sck_pin, uint8_t io_pin) {
	ce = ce_pin;
	sck = sck_pin;
	io = io_pin;
}
uint8_t DS1302::begin(void) {
	pinMode(ce, OUTPUT);
	pinMode(sck, OUTPUT);
	pinMode(io, INPUT);
	write(7,0);
	uint8_t sec = read(0);
	sec |= (1 << 7);
	write(0, sec);
	return 1;
}
uint8_t DS1302::read() {
	pinMode(io, INPUT);
	uint8_t value = 0;
	for (uint8_t i = 0; i < 8; ++i) {
		uint8_t bit = digitalRead(io);
		value |= (bit << i);  // Bits are read LSB first.
		digitalWrite(sck, HIGH);
		digitalWrite(sck, LOW);
	}
	return value;
}

void DS1302::write(const uint8_t val) {
	pinMode(io, OUTPUT);
	shiftOut(io, sck, LSBFIRST, val);
}
uint8_t DS1302::read(const uint8_t addr) {
	const uint8_t cmd = (0x81 | (addr << 1));
	write(cmd);
	return read();
}

void DS1302::write(const uint8_t addr, const uint8_t val) {
	const uint8_t cmd = (0x80 | (addr << 1));
	write(cmd);
	write(val);
}

uint8_t DS1302::isrunning(void) {
	uint8_t ss = read(0);
	return !(ss>>7);
}
DateTime DS1302::now() {
	write(0xBF);
	uint8_t ss = bcd2bin(read() & 0x7F);
	uint8_t mm = bcd2bin(read());
	uint8_t hh = bcd2bin(read());
	uint8_t d = bcd2bin(read());
	uint8_t m = bcd2bin(read());
	read();
	uint16_t y = bcd2bin(read()) + 2000;
	return DateTime (y, m, d, hh, mm, ss);
}
void DS1302::adjust(const DateTime& dt) {
	write(0xBE);
	write(bin2bcd(dt.second()));
	write(bin2bcd(dt.minute()));
	write(bin2bcd(dt.hour()));
	write(bin2bcd(dt.day()));
	write(bin2bcd(dt.month()));
	write(bin2bcd(dt.dayOfWeek()));
	write(bin2bcd(dt.year() - 2000));
	write(0);
}
// RTC_DS1307 implementation

uint8_t DS1307::begin(void) {
  return 1;
}

uint8_t DS1307::read(const uint8_t addr) {
  return _read(DS1307_ADDRESS,addr);
}
void DS1307::write(const uint8_t addr, const uint8_t val) {
  _write(DS1307_ADDRESS,addr,val);
}

uint8_t DS1307::isrunning(void) {
  uint8_t ss = read(0);
  return !(ss>>7);
}

void DS1307::adjust(const DateTime& dt) {
	WIRE.beginTransmission(DS1307_ADDRESS);
	WIRE.write(0);
	WIRE.write(bin2bcd(dt.second()));
	WIRE.write(bin2bcd(dt.minute()));
	WIRE.write(bin2bcd(dt.hour()));
	WIRE.write(bin2bcd(0));
	WIRE.write(bin2bcd(dt.day()));
	WIRE.write(bin2bcd(dt.month()));
	WIRE.write(bin2bcd(dt.year() - 2000));
	WIRE.write(0);
	WIRE.endTransmission();
}

DateTime DS1307::now() {
  WIRE.beginTransmission(DS1307_ADDRESS);
  WIRE.write(0);
  WIRE.endTransmission();

  WIRE.requestFrom(DS1307_ADDRESS, 7);
  uint8_t ss = bcd2bin(WIRE.read() & 0x7F);
  uint8_t mm = bcd2bin(WIRE.read());
  uint8_t hh = bcd2bin(WIRE.read());
  WIRE.read();
  uint8_t d = bcd2bin(WIRE.read());
  uint8_t m = bcd2bin(WIRE.read());
  uint16_t y = bcd2bin(WIRE.read()) + 2000;
  
  return DateTime (y, m, d, hh, mm, ss);
}

uint8_t DS3231::begin(void) {
  write(DS3231_CONTROL_ADDR, DS3231_INTC);
  return 0;
}

uint8_t DS3231::read(const uint8_t addr) {
  return _read(DS3231_ADDRESS,addr);
}
void DS3231::write(const uint8_t addr, const uint8_t val) {
  _write(DS3231_ADDRESS,addr,val);
}
uint8_t DS3231::isrunning(void) {
  uint8_t ss = read(0);
  return !(ss>>7);
}

void DS3231::adjust(const DateTime& dt) {
	uint8_t year,cen;
	if(dt.year() > 2000){
		year = dt.year() - 2000;
		cen = 0x80;
	}else{
		year = dt.year() - 1900;
		cen = 0;
	}
	WIRE.beginTransmission(DS3231_ADDRESS);
	WIRE.write(DS3231_TIME_ADDR);
	WIRE.write(bin2bcd(dt.second()));
	WIRE.write(bin2bcd(dt.minute()));
	WIRE.write(bin2bcd(dt.hour()));
	WIRE.write(bin2bcd(0));
	WIRE.write(bin2bcd(dt.day()));
	WIRE.write(bin2bcd(dt.month())+ cen);
	WIRE.write(bin2bcd(year));
	WIRE.endTransmission();
}

DateTime DS3231::now() {
  WIRE.beginTransmission(DS3231_ADDRESS);
  WIRE.write(DS3231_TIME_ADDR);
  WIRE.endTransmission();
  WIRE.requestFrom(DS3231_ADDRESS, 7);
  uint8_t ss = bcd2bin(WIRE.read() & 0x7F);
  uint8_t mm = bcd2bin(WIRE.read());
  uint8_t hh = bcd2bin(WIRE.read());
  WIRE.read();
  uint8_t d = bcd2bin(WIRE.read());
  uint8_t cen = WIRE.read();
  uint8_t m = bcd2bin(cen & 0x1F);
  uint16_t y = bcd2bin(WIRE.read());
  if((cen & 0x80) >> 7 == 1){
	y += 2000;
  }else{
	y += 1900;
  }
  return DateTime (y, m, d, hh, mm, ss);
}

double DS3231::getTemp() {
  double temp;
  WIRE.beginTransmission(DS3231_ADDRESS);
  WIRE.write(DS3231_TEMPERATURE_ADDR);
  WIRE.endTransmission();
  WIRE.requestFrom(DS3231_ADDRESS, 2);
  temp = (double)WIRE.read();
  temp += (double)0.25*(WIRE.read() >> 6);
  return temp;
}

// provide device address as a full 8 bit address (like the datasheet)
PCF8583::PCF8583(int device_address) {
  address = device_address >> 1; // convert to 7 bit so Wire doesn't choke
}
PCF8583::PCF8583() {
  address = PCF8583_ADDRESS >> 1; // convert to 7 bit so Wire doesn't choke
}

// initialization
uint8_t PCF8583::begin()
{
  _write(address,0x00,0x04);// Set alarm on int\ will turn to vcc
  return 1;
}

DateTime PCF8583::now()
{
  Wire.beginTransmission(address);
  Wire.write(0xC0); // stop counting, don't mask
  Wire.endTransmission();

  Wire.beginTransmission(address);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(address, 5);

  uint8_t second = bcd2bin(Wire.read());
  uint8_t minute = bcd2bin(Wire.read());
  uint8_t hour = bcd2bin(Wire.read());
  uint8_t incoming = Wire.read(); // year/date counter
  uint8_t day = bcd2bin(incoming & 0x3f);
  uint8_t year = (int)((incoming >> 6) & 0x03); // it will only hold 4 years...
  incoming = Wire.read();
  uint8_t month = bcd2bin(incoming & 0x1f);
  //uint8_t dow = incoming >> 5;

  // but that's not all - we need to find out what the base year is
  // so we can add the 2 bits we got above and find the real year
  Wire.beginTransmission(address);
  Wire.write(0x10);
  Wire.endTransmission();
  Wire.requestFrom(address, 2);
  uint8_t year_base = 0;
  year_base = Wire.read();
  year_base = year_base << 8;
  year_base = year_base | Wire.read();
  year = year + year_base;
  return DateTime (year, month, day, hour, minute, second);
}


void PCF8583::adjust(const DateTime& dt)
{
  Wire.beginTransmission(address);
  Wire.write(0xC0); // stop counting, don't mask
  Wire.endTransmission();

  Wire.beginTransmission(address);
  Wire.write(0x02);
  Wire.write(bin2bcd(dt.second()));
  Wire.write(bin2bcd(dt.minute()));
  Wire.write(bin2bcd(dt.hour()));
  Wire.write(((byte)(dt.year() - 2000) << 6) | bin2bcd(dt.day()));
  Wire.write((dt.dayOfWeek() << 5) | (bin2bcd(dt.month()) & 0x1f));
  Wire.endTransmission();

  Wire.beginTransmission(address);
  Wire.write(0x10);
  Wire.write(2000 >> 8);
  Wire.write(2000 & 0x00ff);
  Wire.endTransmission();
  
  begin(); // re set the control/status register to 0x04

}
uint8_t PCF8583::isrunning(void){
  uint8_t ss = _read(address,0);
  return !(ss>>7);
}

//Get the alarm at 0x09 adress
DateTime PCF8583::get_alarm()
{
  Wire.beginTransmission(address);
  Wire.write(0x0A); // Set the register pointer to (0x0A)
  Wire.endTransmission();

  Wire.requestFrom(address, 4); // Read 4 values

  uint8_t second = bcd2bin(Wire.read());
  uint8_t minute = bcd2bin(Wire.read());
  uint8_t hour = bcd2bin(Wire.read());

  Wire.beginTransmission(address);
  Wire.write(0x0E);
  Wire.endTransmission();

  Wire.requestFrom(address, 1); // Read weekday value

  uint8_t day = bcd2bin(Wire.read());
  return DateTime (2000, 01, day, hour, minute, second);
}

//Set a daily alarm
void PCF8583::set_alarm(const DateTime& dt)
{
  Wire.beginTransmission(address);
  Wire.write(0x08);
  Wire.write(0x90); // daily alarm set
  Wire.endTransmission();

  Wire.beginTransmission(address);
  Wire.write(0x09); // Set the register pointer to (0x09)
  Wire.write(0x00); // Set 00 at milisec
  Wire.write(bin2bcd(dt.second()));
  Wire.write(bin2bcd(dt.minute()));
  Wire.write(bin2bcd(dt.hour()));
  Wire.write(0x00); // Set 00 at day
  Wire.endTransmission();
}
void PCF8583::off_alarm()
{
  Wire.beginTransmission(address);
  Wire.write(0x08);
  Wire.write(0x00); // off alarm set
  Wire.endTransmission();
}

// provide device address as a full 8 bit address (like the datasheet)
PCF8563::PCF8563(int device_address) {
  address = device_address >> 1; // convert to 7 bit so Wire doesn't choke
}
PCF8563::PCF8563() {
  address = PCF8563_ADDRESS >> 1; // convert to 7 bit so Wire doesn't choke
}

// initialization
uint8_t PCF8563::begin()
{
  // Wire.begin(14, 27);
	Wire.begin();
  Wire.beginTransmission(address);
  Wire.write(0x00);			// Start address
  Wire.write(0);			   // Control and status 1
  Wire.write(0);			   // Control and status 2
  return Wire.endTransmission();
}

DateTime PCF8563::now()
{
  Wire.beginTransmission(address);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(address, 9);
  
  status1 = Wire.read();
  status2 = Wire.read();
  uint8_t second = bcd2bin(Wire.read() & 0x7F);
  uint8_t minute = bcd2bin(Wire.read() & 0x7F);
  uint8_t hour = bcd2bin(Wire.read() & 0x3F);
  uint8_t day = bcd2bin(Wire.read() & 0x3F);
  uint8_t wekday = Wire.read() & 0x07; // year/date counter
  uint8_t month = Wire.read();
  uint8_t century = month >> 7;
  (void)wekday;
  (void)century;
  month = bcd2bin(month & 0x1F);
  uint8_t year = bcd2bin(Wire.read()); // it will only hold 4 years...
  return DateTime (year, month, day, hour, minute, second);
}

void PCF8563::adjust(const DateTime& dt)
{
  Wire.beginTransmission(address);
  Wire.write(0x02);						  // Start address
  Wire.write(bin2bcd(dt.second()));		  // Second (0-59)
  Wire.write(bin2bcd(dt.minute()));		  // Minute (0-59)
  Wire.write(bin2bcd(dt.hour()));			// Hour (0-23)
  Wire.write(bin2bcd(dt.day()));			 // Day (1-31)
  Wire.write(bin2bcd(dt.dayOfWeek()));	   // Weekday (0-6 = Sunday-Saturday)
  Wire.write(bin2bcd(dt.month()) | 0x80);	// Month (1-12, century bit (bit 7) = 1)
  Wire.write(bin2bcd(dt.year() % 100));	  // Year (00-99)
  Wire.endTransmission();
  
  begin(); // re set the control/status register to 0x04
}
uint8_t PCF8563::isrunning(void){
  WIRE.beginTransmission(address);
  WIRE.write(0);
  WIRE.endTransmission();

  WIRE.requestFrom(address, 2);
  
  status1 = Wire.read();
  status2 = Wire.read();
  return !(bitRead(status1,5));
}

//Get the alarm at 0x09 adress
DateTime PCF8563::get_alarm()
{
  Wire.beginTransmission(address);
  Wire.write(0x09); // Set the register pointer to (0x0A)
  Wire.endTransmission();

  Wire.requestFrom(address, 4); // Read 4 values

  uint8_t minute = bcd2bin(Wire.read());
  uint8_t hour = bcd2bin(Wire.read());
  uint8_t day = bcd2bin(Wire.read());
  uint8_t wday = bcd2bin(Wire.read());
  return DateTime (0, wday, day, hour, minute, 0);
}

//Set a daily alarm
void PCF8563::set_alarm(const DateTime &dt, struct alarm_flags flags)
{
  byte minute    = bin2bcd(dt.minute());
  byte hour      = bin2bcd(dt.hour());
  byte day       = bin2bcd(dt.day());
  byte dayOfWeek = bin2bcd(dt.month());

  if( !flags.minute)
    minute |= PCF8563_ALARM_REG_OFF;

  if( !flags.hour)
    hour |= PCF8563_ALARM_REG_OFF;

  if( !flags.day)
    day |= PCF8563_ALARM_REG_OFF;

  if( !flags.wday)
    dayOfWeek |= PCF8563_ALARM_REG_OFF;

  Wire.beginTransmission(address);
  Wire.write(0x09); // Set the register pointer to (0x09)
  Wire.write(minute);
  Wire.write(hour);
  Wire.write(day);
  Wire.write(dayOfWeek);
  Wire.endTransmission();
}

void PCF8563::off_alarm()
{
	//set status2 AF val to zero to reset alarm
	status2 &= ~PCF8563_ALARM_AF;
	Wire.beginTransmission(address);
	Wire.write(0x01);
	Wire.write(status2);
	Wire.endTransmission();
}

void PCF8563::on_alarm()
{
	//set status2 AF val to zero
	status2 &= ~PCF8563_ALARM_AF;
	//enable the interrupt
	status2 |= PCF8563_ALARM_AIE;
	Wire.beginTransmission(address);  // Issue I2C start signal
	Wire.write(0x01);
	Wire.write(status2);
	Wire.endTransmission();
}

////////////////////////////////////////////////////////////////////////////////
// RTC_Millis implementation

long RTC_Millis::offset = 0;

void RTC_Millis::adjust(const DateTime& dt) {
	offset = dt.unixtime() - millis() / 1000;
	ok = 1;
}

DateTime RTC_Millis::now() {
  return DateTime((uint32_t)(offset + millis() / 1000));
}

uint8_t RTC_Millis::isrunning(void){
  return ok;
}
uint8_t RTC_Millis::begin()
{
  ok = 0;
  return 1;
}
////////////////////////////////////////////////////////////////////////////////
