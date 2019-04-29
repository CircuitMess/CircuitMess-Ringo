// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!

#ifndef _RTCLIB_H_
#define _RTCLIB_H_

struct alarm_flags {
  char minute;
  char hour;
  char day;
  char wday;
};

#define AE_M 1
#define AE_H 1
#define AE_D 1
#define AE_W 1

// TimeDelta which can represent changes in time with seconds accuracy.
class TimeDelta {
public:
	TimeDelta (uint32_t seconds =0);
	TimeDelta (uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);
	TimeDelta (const TimeDelta& copy);
	uint16_t days() const			{ return _sec / 86400L; }
	uint8_t hours() const			{ return _sec/ 3600 % 24; }
	uint8_t minutes() const			{ return _sec / 60 % 60; }
	uint8_t seconds() const			{ return _sec % 60; }
	uint32_t totalseconds() const		{ return _sec; }

	TimeDelta operator+(const TimeDelta& right);
	TimeDelta operator-(const TimeDelta& right);

protected:
	uint32_t _sec;
};

// Simple general-purpose date/time class (no TZ / DST / leap second handling!)
class DateTime {
public:
	char* format(char* ret);
	DateTime (uint32_t t =0);
	DateTime (uint16_t year, uint8_t month, uint8_t day,
				uint8_t hour =0, uint8_t min =0, uint8_t sec =0);
	DateTime (const char* date, const char* time);
	DateTime (const __FlashStringHelper* date, const __FlashStringHelper* time);
	DateTime (const char* sdate);
	uint16_t year() const		{ return 2000 + yOff; }
	uint8_t month() const		{ return m; }
	uint8_t day() const		{ return d; }
	uint8_t hour() const		{ return hh; }
	uint8_t minute() const		{ return mm; }
	uint8_t second() const		{ return ss; }
	uint8_t dayOfWeek() const;
	void setyear(uint16_t year) 	{ yOff = year - 2000; }
	void setmonth(uint8_t month)	{ m = month; }
	void setday(uint8_t day) 	{ d = day; }
	void sethour(uint8_t hour) 	{ hh = hour%24; }
	void setminute(uint8_t minute) 	{ mm = minute%60; }
	void setsecond(uint8_t second) 	{ ss = second%60; }
	// 32-bit times as seconds since 1/1/2000
	long secondstime() const;
	// 32-bit times as seconds since 1/1/1970
	uint32_t unixtime(void) const;
	unsigned char equals(const char* sdate) const;//DD-MM-YYYY hh:mm:ss
	unsigned char equals(const DateTime &date) const;
	unsigned char operator == (const char *sdate) const {return equals(sdate);}
	unsigned char operator == (const DateTime &date) const {return equals(date);}
	unsigned char operator != (const char *sdate) const {return !equals(sdate);}
	unsigned char operator != (const DateTime &date) const {return !equals(date);}
	unsigned char operator < (const DateTime &date) const;
	unsigned char operator > (const DateTime &date) const;
	unsigned char operator <= (const DateTime &date) const;
	unsigned char operator >= (const DateTime &date) const;

	DateTime operator + (const TimeDelta& delta);
	DateTime operator - (const TimeDelta& delta);
	TimeDelta operator - (const DateTime& right);

protected:
	uint8_t yOff, m, d, hh, mm, ss;
};

// RTC based on the DS1302 chip connected via pins
class DS1302 {
private:
	uint8_t read();
	void write(const uint8_t val);
public:
	DS1302 (uint8_t ce_pin = 4, uint8_t sck_pin = 5, uint8_t io_pin = 6);
	uint8_t begin(void);
	void adjust(const DateTime& dt);
	uint8_t isrunning(void);
	DateTime now();
	uint8_t read(const uint8_t addr);
	void write(const uint8_t addr, const uint8_t val);
protected:
	uint8_t ce, sck, io;
};

// RTC based on the DS1307 chip connected via I2C and the Wire library
class DS1307 {
public:
	uint8_t begin(void);
	void adjust(const DateTime& dt);
	uint8_t isrunning(void);
	DateTime now();
	uint8_t read(const uint8_t addr);
	void write(const uint8_t addr, const uint8_t val);
};

class DS3231 {
public:
	uint8_t begin(void);
	void adjust(const DateTime& dt);
	uint8_t isrunning(void);
	double getTemp(void);
	DateTime now();
	uint8_t read(const uint8_t addr);
	void write(const uint8_t addr, const uint8_t val);
};


class PCF8583 {
	int address;
  public:
	PCF8583();
	PCF8583(int device_address);
	uint8_t begin(void);	
	DateTime now();
	uint8_t isrunning(void);
	void adjust(const DateTime& dt);
	void off_alarm(void);
	DateTime get_alarm();
	void set_alarm(const DateTime& dt);
};

class PCF8563 {
	int address;
	int status1;
	int status2;
  public:
	PCF8563();
	PCF8563(int device_address);
	uint8_t begin(void);	
	DateTime now();
	uint8_t isrunning(void);
	void adjust(const DateTime& dt);
	void off_alarm(void);
	void on_alarm(void);
	DateTime get_alarm();
  void set_alarm(const DateTime& dt, struct alarm_flags flags);
};

// RTC using the internal millis() clock, has to be initialized before use
// NOTE: this clock won't be correct once the millis() timer rolls over (>49d?)
class RTC_Millis {
private:
	uint8_t ok;
public:
	void begin(const DateTime& dt) { adjust(dt);ok = 1; }
	void adjust(const DateTime& dt);
	DateTime now();
	uint8_t begin(void);	
	uint8_t isrunning(void);

protected:
	static long offset;

};

#endif // _RTCLIB_H_
