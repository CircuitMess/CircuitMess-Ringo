#ifndef buttons_h
#define buttons_h

#include <Arduino.h>
#include "../Keypad_I2Ca/Keypad_I2Ca.h"
#include "../Keypad/src/Keypad.h"
#include "../Adafruit ADS1X15/Adafruit_ADS1015.h"


#define buttons_per_column 4
#define buttons_per_row 4
#define multi_tap_threshold 500
#define ROWS 4 //four rows
#define COLS 4
#define NUM_BTN 22
#define I2CADDRESS 0x74

class Buttons
{
	private:
		byte rowPins[ROWS] = { 0, 1, 2, 3 }; //connect to the row pinouts of the keypad
		byte colPins[COLS] = { 4, 5, 6, 7 }; //connect to the column pinouts of the keypad
		char keys[4][4] = {
		{ '1', '2', '3', 'A' },
		{ '4', '5', '6', 'B' },
		{ '7', '8', '9', 'C' },
		{ '*', '0', '#', 'D' }
		};
		Adafruit_ADS1015 ads;     
		int16_t joystick_x, joystick_y;

	public:
		///////////////////
		//Keypad variables
		//////////////////

		Keypad_I2Ca kpd = Keypad_I2Ca(makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDRESS, PCA9539);
		bool pressed(uint8_t button);
		void update();
		bool repeat(uint8_t button, uint16_t period);
		bool released(uint8_t button);
		bool held(uint8_t button, uint16_t time);
		uint16_t timeHeld(uint8_t button);
		uint16_t states[NUM_BTN];
		char key = NO_KEY;
		void begin();
		char getKey();
};
#endif