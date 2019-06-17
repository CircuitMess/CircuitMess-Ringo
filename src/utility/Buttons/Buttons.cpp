#include "Buttons.h"

void Buttons::begin() {
	ads.begin();
	kpd.begin();


	// kpd.setDebounceTime(20);
	// kpd.setHoldTime(0);

	for(int i = 0;i<16;i++) {
		kpd.pin_mode(i, INPUT);
	}
}

void Buttons::update() {
	buttonsData = 0;
	
	delay(1);
	button_a = ads.readADC_SingleEnded(3);
	delay(1);
	joystick_x = ads.readADC_SingleEnded(1);
	delay(1);
	joystick_y = ads.readADC_SingleEnded(0);
	delay(1);
	button_b = ads.readADC_SingleEnded(2);

	bitWrite(buttonsData, 18, !(joystick_y < 100 && joystick_x > 300 && joystick_x < 800)); //BTN_UP
	bitWrite(buttonsData, 19, !(joystick_y > 1000 && joystick_x > 300 && joystick_x < 800)); //BTN_DOWN
	bitWrite(buttonsData, 20, !(joystick_x > 1000 && joystick_y > 300 && joystick_y < 800)); //BTN_LEFT
	bitWrite(buttonsData, 21, !(joystick_x < 100 && joystick_y > 300 && joystick_y < 800)); //BTN_RIGHT
	bitWrite(buttonsData, 16, !(button_a < 1));
	bitWrite(buttonsData, 17, !(button_b < 1));

	int tmp_value = kpd.port_read();

	for (uint8_t i = 0; i < NUM_BTN; i++) {
		bool pressed;
		if(i < 16) {
			pressed = !bitRead(tmp_value, i);
		} else {
			pressed = (buttonsData & (1 << i)) == 0;
		}

		if (pressed) { //if button pressed
			if (states[i] < 0xFFFE) { // we want 0xFFFE to be max value for the counter
				states[i]++; //increase button hold time
			}
			else if (states[i] == 0xFFFF) { // if we release / hold again too fast
				states[i] = 1;
			}
		}
		else {
			if (states[i] == 0) { //button idle
				continue;
			}
			if (states[i] == 0xFFFF) { //if previously released
				states[i] = 0; //set to idle
			}
			else {
				states[i] = 0xFFFF; //button just released
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

bool Buttons::pressed(uint8_t button) {
	return states[(uint8_t)button] == 1;
}

bool Buttons::released(uint8_t button) {
	return states[(uint8_t)button] == 0xFFFF;
}

bool Buttons::held(uint8_t button, uint16_t time) {
	return states[(uint8_t)button] == (time + 1);
}

char Buttons::getKey() {
	for(int i = 0; i < 22; i++) {
		if(Buttons::released(i)) {
			if(i == 9) {
				return '*';
			}else if(i == 10)
				return '0';
			else if(i == 11) {
				return '#';
			} else if(i < 11) {
				return '1' + i;
			} else {
				switch(i) {
					case 12:
						return 'A';
						Serial.println("asdasd");
						break;
					case 13:
						return 'B';
						Serial.println("asdasd");
						break;
					case 14:
						return 'C';
						Serial.println("asdasd");
						break;
					case 15:
						return 'D';
						Serial.println("asdasd");
						break;
					default:
						return NO_KEY;
						Serial.println("asdasd");
						break;
				}
			}
		}
	}
	return NO_KEY;
}

uint16_t Buttons::timeHeld(uint8_t button) {
	if (states[(uint8_t)button] != 0xFFFF) {
		return states[(uint8_t)button];
	}
	else {
		return 0;
	}
}

void Buttons::activateInterrupt()
{
	ads.startComparator_Windowed(3, 1199, 10);
	// ads.readADC_SingleEnded(0);
	// ads.startComparator_Windowed(0,1000,100);
	// ads.readADC_SingleEnded(1);
	// ads.startComparator_Windowed(1,1000,100);

}