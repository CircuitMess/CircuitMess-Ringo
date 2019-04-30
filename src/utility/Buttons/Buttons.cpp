#include "Buttons.h"
bool Buttons::pressed(uint8_t button) {
	return states[(uint8_t)button] == 1;
}
void Buttons::begin() {
  	ads.begin();
	kpd.begin();
}
void Buttons::update() {
	uint32_t buttonsData = 0;
	key = kpd.getKey();
	for (int y = 0; y < ROWS;y++)
	{
		for (int x = 0; x < COLS; x++)
		{
			if(key == keys[y][x] && key != 'B' && key != 'C')
				bitWrite(buttonsData, y * 4 + x, 1);
		}
	}
	bitWrite(buttonsData, 16, !(bool)kpd.pin_read(5));//BTN_A
	bitWrite(buttonsData, 17, !(bool)kpd.pin_read(4));//BTN_B

	joystick_y = ads.readADC_SingleEnded(0);
  	joystick_x = ads.readADC_SingleEnded(1);

	bitWrite(buttonsData, 18, joystick_y < 100); //BTN_UP
	bitWrite(buttonsData, 19, joystick_y > 1000); //BTN_DOWN
	bitWrite(buttonsData, 20, joystick_x > 1000); //BTN_LEFT
	bitWrite(buttonsData, 21, joystick_x < 100); //BTN_RIGHT

	for (uint8_t thisButton = 0; thisButton < NUM_BTN; thisButton++) {
		//extract the corresponding bit corresponding to the current button
		//Inverted logic : button pressed = low state = 0
		bool pressed = (buttonsData & (1 << thisButton)) == 0;

		if (pressed) { //if button pressed
			if (states[thisButton] < 0xFFFE) { // we want 0xFFFE to be max value for the counter
				states[thisButton]++; //increase button hold time
			}
			else if (states[thisButton] == 0xFFFF) { // if we release / hold again too fast
				states[thisButton] = 1;
			}
		}
		else {
			if (states[thisButton] == 0) {//button idle
				continue;
			}
			if (states[thisButton] == 0xFFFF) {//if previously released
				states[thisButton] = 0; //set to idle
			}
			else {
				states[thisButton] = 0xFFFF; //button just released
			}
		}
	}
}
bool Buttons::repeat(uint8_t button, uint16_t period) {
	if (period <= 1) {
		if ((states[(uint8_t)button] != 0xFFFF) && (states[(uint8_t)button])) {
			return true;
		}
	}
	else {
		if ((states[(uint8_t)button] != 0xFFFF) && ((states[(uint8_t)button] % period) == 1)) {
			return true;
		}
	}
	return false;
}
bool Buttons::released(uint8_t button) {
	return states[(uint8_t)button] == 0xFFFF;
}
bool Buttons::held(uint8_t button, uint16_t time) {
	return states[(uint8_t)button] == (time + 1);
}
char Buttons::getKey()
{
	return (key != 'B' && key != 'C') ? key : NO_KEY;
}
uint16_t Buttons::timeHeld(uint8_t button) {
	if (states[(uint8_t)button] != 0xFFFF) {
		return states[(uint8_t)button];
	}
	else {
		return 0;
	}
}

