#ifndef buttons_h
#define buttons_h

#include <Arduino.h>
#include "../I2cExpander/I2cExpander.h"
#include "../Adafruit_ADS1X15/Adafruit_ADS1015.h"


#define buttons_per_column 4
#define buttons_per_row 4
#define multi_tap_threshold 500
#define ROWS 4 //four rows
#define COLS 4
#define NUM_BTN 22
#define I2CADDRESS 0x74
#define NO_KEY '\0'

class Buttons
{
	private:
		uint32_t buttonsData = 0;
		byte rowPins[ROWS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
		byte colPins[COLS] = {10, 11, 12, 13}; //connect to the column pinouts of the keypad
		char keys[4][4] = {
			{ '1', '2', '3', 'D' },
			{ '4', '5', '6', 'C' },
			{ '7', '8', '9', 'B' },
			{ '*', '0', '#', 'A' }
		};
		int16_t joystick_x, joystick_y, button_a, button_b;

	public:
		Adafruit_ADS1015 ads;

		///////////////////
		//Keypad variables
		//////////////////
		I2cExpander kpd;
		void begin();
		void update();

		bool pressed(uint8_t button);
		bool repeat(uint8_t button, uint16_t period);
		bool released(uint8_t button);
		bool held(uint8_t button, uint16_t time);

		uint16_t timeHeld(uint8_t button);
		uint16_t states[NUM_BTN];

		char currentKey = NO_KEY;
		char getKey(); //deprecated

		uint16_t getJoystickX(){ return joystick_x; }
		uint16_t getJoystickY(){ return joystick_y; }
		void activateInterrupt();
		bool holdForUnlock = 0;
};
#endif