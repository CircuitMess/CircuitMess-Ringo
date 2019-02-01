/*
This file is part of the MAKERphone library,
Copyright (c) CircuitMess 2018-2019
This is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License (LGPL)
as published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.
This is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License (LGPL) for more details.
You should have received a copy of the GNU Lesser General Public
License (LGPL) along with the library.
If not, see <http://www.gnu.org/licenses/>.
Authors:
- Albert Gajsak
- Emil Gajsak
*/

#include "MAKERphone.h"
extern MAKERphone mp;
void MAKERphone::begin(bool splash) {
	
	String input="";
	dataRefreshFlag = 0;
	pinMode(SIM800_DTR, OUTPUT);
	digitalWrite(SIM800_DTR, 0);
	pinMode(INTERRUPT_PIN, INPUT_PULLUP);
	esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, 0); //1 = High, 0 = Low
	//Initialize and start with the NeoPixels
	FastLED.addLeds<NEOPIXEL, 33>(leds, 8);
	Serial1.begin(9600, SERIAL_8N1, 17, 16);
	//Serial1.println(F("AT+CFUN=1,1"));
	//Serial1.println("AT+CMEE=2");
	//Serial1.println(F("AT+CPIN?"));
	if (splash == 1) {
		for (uint8_t i = 0; i < NUMPIXELS; i++) {
			for (uint8_t x = 0; x <= 128; x += 2) {
				leds[i] = CRGB(x, 0, 0);
				delay(1);
				FastLED.show();
			}
		}
		for (uint8_t i = 0; i < NUMPIXELS; i++) {
			for (int16_t x = 128; x >= 0; x -= 2) {
				Serial.println(x);
				leds[i] = CRGB(x, 0, 0);
				delay(1);
				FastLED.show();
			}
		}
	}
	FastLED.clear();

	//Startup sounds
	tone2(soundPin, 2000, 10);
	buttons.begin();
	buttons.kpd.writeMute(0);
	buttons.kpd.writeVolume(0);
	//kpd.writeVolumeRight(78);

	//PWM SETUP FOR ESP32
	ledcSetup(0, 2000, 8);
	ledcAttachPin(soundPin, 0);
	ledcSetup(LEDC_CHANNEL, LEDC_BASE_FREQ, LEDC_TIMER);
	ledcAttachPin(LCD_BL_PIN, LEDC_CHANNEL);
	ledcAnalogWrite(LEDC_CHANNEL, 255);
	uint32_t tempMillis = millis();
	SDinsertedFlag = 1;
	while (!SD.begin(5, SD_SCK_MHZ(8)))
	{
		if(millis()-tempMillis > 5)
		{
			SDinsertedFlag = 0;
			break;
		}
	}
	if(SDinsertedFlag)
		loadSettings(1);
	applySettings();

	//display initialization
	tft.init();

	tft.invertDisplay(0);
	tft.setRotation(1);
	display.setColorDepth(8); // Set colour depth of Sprite to 8 (or 16) bits
	display.createSprite(BUFWIDTH, BUFHEIGHT); // Create the sprite and clear background to black
	display.setTextWrap(0);             //setRotation(1);
	display.setTextSize(1); // landscape

	buf.createSprite(BUF2WIDTH, BUF2HEIGHT); // Create the sprite and clear background to black
	buf.setTextSize(1);

	if (splash == 1)
	{
		display.fillScreen(TFT_RED);
		while (!update());
	}
	else {
		display.fillScreen(TFT_BLACK);
		while (!update());
	}

	ledcAnalogWrite(LEDC_CHANNEL, 255);
	for (uint8_t i = 255; i > actualBrightness; i--) {
		ledcAnalogWrite(LEDC_CHANNEL, i);
		delay(2);
	}

	//ledcAnalogWrite(LEDC_CHANNEL, 0);
	if (splash == 1)
		splashScreen(); //Show the main splash screen
	else
	{
		delay(500);
		checkSim();
	}

	// updateTimeGSM();
	Serial1.println(F("AT+CMEE=2"));
	Serial1.println(F("AT+CLVL=100"));
	Serial1.println(F("AT+CRSL=100"));
	Serial1.println(F("AT+CMIC=0,6"));
	Serial1.println(F("AT+CMGF=1"));
	Serial1.println(F("AT+CNMI=1,1,0,0,0"));
	Serial1.println(F("AT+CLTS=1")); //Enable local Timestamp mode (used for syncrhonising RTC with GSM time
	Serial1.println(F("AT+CPMS=\"SM\",\"SM\",\"SM\""));
	Serial1.println(F("AT+CLCC=1"));
	Serial1.println(F("AT&W"));
	//  while(1){
	//    if(Serial1.available())
	//      Serial.println(Serial1.read());
	//  }
	//Serial1.println(F("AT&W")); //Save  all settings in SIM's non-volatile memory
	Serial.println("Serial1 up and running...");
}

void MAKERphone::test() {
	// Serial.println("|------Test------|");

	// String contacts_default = "[{\"name\":\"Foobar\", \"number\":\"099123123\"}]";
	// String settings_default = "{ \"wifi\": 0, \"bluetooth\": 0, \"airplane_mode\": 0, \"brightness\": 0, \"sleep_time\": 0, \"background_color\": 0 }";

	// const char contacts_path[] = "/contacts.json";
	// const char settings_path[] = "/settings.json";

	// JsonArray& contacts = mp.jb.parseArray(contacts_default);
	// JsonObject& settings = mp.jb.parseObject(settings_default);

	// SD.remove(contacts_path);
	// SD.remove(settings_path);

	// File contacts_file = SD.open(contacts_path, FILE_REWRITE);
	// contacts.prettyPrintTo(contacts_file);
	// contacts_file.close();

	// File settings_file = SD.open(settings_path, FILE_REWRITE);
	// settings.prettyPrintTo(settings_file);
	// settings_file.close();
}

bool MAKERphone::update() {
	// bool pressed = 0;
	//if (digitalRead(INTERRUPT_PIN) == 0) //message is received or incoming call
	//{
	//	Serial.println("INTERRUPTED");
	//	Serial.println(readSerial());
	//	if (readSerial().indexOf("CMT") != -1 || readSerial().indexOf("CMTI") != -1)//it's a message!
	//	{

	//		gui.popup("Sample text", 50);
	//		Serial.println("SMS received");

	//		display.setCursor(18, 35);
	//		display.println("MESSAGE RECEIVED");
	//		Serial.println("received");
	//		delay(5000);
	//	}
	//	else if (readSerial().indexOf("RING") != -1)//it's a call!
	//	{
	//		Serial.println("CALL");
	//		//handleCall();
	//	}
	//}
	char c;
	uint16_t refreshInterval = 3000;
	if(!spriteCreated)
	{
		display.deleteSprite();
		if(resolutionMode)
			display.createSprite(BUFWIDTH, BUFHEIGHT);
		else
			display.createSprite(BUF2WIDTH, BUF2HEIGHT);
		display.setRotation(1);
		spriteCreated=1;
	}
	//halved resolution mode
	if(resolutionMode == 1)
	{
		for (int y = 0; y < BUFHEIGHT; y++) {
			for (int x = 0; x < BUFWIDTH; x++) {
				buf.fillRect(x * 2, y * 2, 2, 2, display.readPixel(x, y));
			}
		}
	}
	//buf2.invertDisplay(1);
	if (digitalRead(35) && sleepTime)
	{
		if (millis() - sleepTimer >= sleepTimeActual * 1000)
		{
			sleep();
			sleepTimer = millis();
		}
	}
	else if(!digitalRead(35) && sleepTime)
		sleepTimer = millis();
	if (millis() > 7000)
		simReady = 1;
	else
		simReady = 0;
	/////////////////////////////////////////////
	//refreshing signal and battery info////////
	///////////////////////////////////////////
	if (dataRefreshFlag)
	{
		if (millis() - refreshMillis >= refreshInterval)
		{
			receivedFlag = 0;
			updateBuffer = "";
			Serial1.println("AT+CBC");
			if (simInserted && !airplaneMode)
			{
				Serial1.println("AT+CSQ");
				if (carrierName == "")
					Serial1.println("AT+CSPN?");
				if (clockYear == 4 || clockYear == 80 || clockMonth == 0 || clockMonth > 12 || clockHour > 24 || clockMinute >= 60)
					Serial1.println("AT+CCLK?");
			}
			refreshMillis = millis();
		}
		if (Serial1.available())
		{
			c = Serial1.read();
			updateBuffer += c;
			if (simInserted && !airplaneMode)
			{
				if (carrierName == "")
				{
					if (updateBuffer.indexOf("\n", updateBuffer.indexOf("+CSPN:")) != -1)
					{
						carrierName = updateBuffer.substring(updateBuffer.indexOf("\"", updateBuffer.indexOf("+CSPN:")) + 1, updateBuffer.indexOf("\"", updateBuffer.indexOf("\"", updateBuffer.indexOf("+CSPN:")) + 1));
					}
				}
				if (updateBuffer.indexOf("\n", updateBuffer.indexOf("+CSQ:")) != -1)
					signalStrength = updateBuffer.substring(updateBuffer.indexOf(" ", updateBuffer.indexOf("+CSQ:")) + 1, updateBuffer.indexOf(",", updateBuffer.indexOf(" ", updateBuffer.indexOf("+CSQ:")))).toInt();
				if (clockYear == 4 || clockYear == 80 || clockMonth == 0 || clockMonth > 12 || clockHour > 24 || clockMinute >= 60)
					if (updateBuffer.indexOf("\n", updateBuffer.indexOf("+CCLK:")) != -1)
					{
						uint16_t index = updateBuffer.indexOf(F("+CCLK: \""));
						char c1, c2; //buffer for saving date and time numerals in form of characters
						c1 = updateBuffer.charAt(index + 8);
						c2 = updateBuffer.charAt(index + 9);
						clockYear = 2000 + ((c1 - '0') * 10) + (c2 - '0');
						Serial.println(F("CLOCK YEAR:"));
						Serial.println(clockYear);
						delay(5);
						clockYear = ((c1 - '0') * 10) + (c2 - '0');

						c1 = updateBuffer.charAt(index + 11);
						c2 = updateBuffer.charAt(index + 12);
						clockMonth = ((c1 - '0') * 10) + (c2 - '0');
						Serial.println(F("CLOCK MONTH:"));
						Serial.println(clockMonth);

						c1 = updateBuffer.charAt(index + 14);
						c2 = updateBuffer.charAt(index + 15);
						clockDay = ((c1 - '0') * 10) + (c2 - '0');
						Serial.println(F("CLOCK DAY:"));
						Serial.println(clockDay);

						c1 = updateBuffer.charAt(index + 17);
						c2 = updateBuffer.charAt(index + 18);
						clockHour = ((c1 - '0') * 10) + (c2 - '0');
						Serial.println(F("CLOCK HOUR:"));
						Serial.println(clockHour);

						c1 = updateBuffer.charAt(index + 20);
						c2 = updateBuffer.charAt(index + 21);
						clockMinute = ((c1 - '0') * 10) + (c2 - '0');
						Serial.println(F("CLOCK MINUTE:"));
						Serial.println(clockMinute);

						c1 = updateBuffer.charAt(index + 23);
						c2 = updateBuffer.charAt(index + 24);
						clockSecond = ((c1 - '0') * 10) + (c2 - '0');
						Serial.println(F("CLOCK SECOND:"));
						Serial.println(clockSecond);

						//TO-DO: UPDATE THE RTC HERE

						buttons.kpd.setHour(clockHour);
						buttons.kpd.setMinute(clockMinute);
						buttons.kpd.setSecond(clockSecond);
						buttons.kpd.setDate(clockDay);
						buttons.kpd.setMonth(clockMonth);
						buttons.kpd.setYear(clockYear);
						Serial.println(F("\nRTC TIME UPDATE OVER GSM DONE!"));
					}
			}

			if (updateBuffer.indexOf("\n", updateBuffer.indexOf("+CBC:")) != -1)
			{
				batteryVoltage = updateBuffer.substring(updateBuffer.indexOf(",", updateBuffer.indexOf(",", updateBuffer.indexOf("+CBC:")) + 1) + 1, updateBuffer.indexOf("\n", updateBuffer.indexOf("+CBC:"))).toInt();
			}
		}
	}
	///////////////////////////////////////////////
	if (millis() - lastFrameCount2 >= frameSpeed) {
		lastFrameCount2 = millis();
		if(resolutionMode == 0) //native res mode
			display.pushSprite(0, 0);

		else//halved res mode
			buf.pushSprite(0,0);

		buttons.update();
		gui.updatePopup();
		FastLED.show();
		delay(1);

		FastLED.clear();
		return true;
	}
	else
		return false;

}
void MAKERphone::splashScreen() {
	display.setFreeFont(TT1);
	Serial1.println(F("AT+CPIN?"));
	String input;
	for (int y = -49; y < 1; y++)
	{
		display.fillScreen(TFT_RED);
		display.drawBitmap(0, y, splashScreenLogo, TFT_WHITE);
		update();
		while (Serial1.available())
			input += (char)Serial1.read();

		delay(20);
	}
	Serial.println(input);
	delay(500);
	display.setTextColor(TFT_WHITE);
	display.setCursor(19, 54);
	display.print("CircuitMess");
	while (!update());
	while (input.indexOf("+CPIN:") == -1 && input.indexOf("ERROR") == -1)
	{
		Serial1.println(F("AT+CPIN?"));
		input = Serial1.readString();
	}
	Serial.println(input);
	delay(5);

	if (input.indexOf("NOT READY", input.indexOf("+CPIN:")) != -1 || input.indexOf("ERROR") != -1
		|| input.indexOf("NOT INSERTED") != -1 )
	{
		simInserted = 0;
	}
	else
	{
		simInserted = 1;
		if (input.indexOf("SIM PIN") != -1)
			enterPin();
	}





}
void MAKERphone::setResolution(bool res){
	mp.resolutionMode=res;
	mp.spriteCreated=0;
}
void MAKERphone::tone2(int pin, int freq, int duration) {
	ledcWriteTone(0, freq);
	delay(duration);
	ledcWriteTone(0, 0);
}
void MAKERphone::vibration(int duration) {
	kpd.pin_write(VIBRATION_MOTOR, 1);
	//delay(duration);
	kpd.pin_write(VIBRATION_MOTOR, 0);
}
void MAKERphone::ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax) {
	// calculate duty, 8191 from 2 ^ 13 - 1
	uint32_t duty = (8191 / 255) * _min(value, valueMax);
	// write duty to LEDC
	ledcWrite(channel, duty);
}
void MAKERphone::sleep() {
	digitalWrite(SIM800_DTR, 1);
	Serial1.println(F("AT+CSCLK=1"));

	FastLED.clear();

	ledcAnalogWrite(LEDC_CHANNEL, 255);
	for (uint8_t i = actualBrightness; i < 255; i++) {
		ledcAnalogWrite(LEDC_CHANNEL, i);
		delay(1);
	}

	ledcAnalogWrite(LEDC_CHANNEL, 255);
	tft.writecommand(16);//send 16 for sleep in, 17 for sleep out

	esp_light_sleep_start();

	tft.writecommand(17);
	ledcAnalogWrite(LEDC_CHANNEL, actualBrightness);

	digitalWrite(SIM800_DTR, 0);
	Serial1.println(F("AT"));
	Serial1.println(F("AT+CSCLK=0"));

	delay(2);
	FastLED.clear();

	while (!update());
	if (buttons.pressed(BTN_A))
		while (!buttons.released(BTN_A))
			update();
	else if (buttons.pressed(BTN_B))
		while (!buttons.released(BTN_B))
			update();
	while (!update());
}
void MAKERphone::lockScreen() {
	dataRefreshFlag = 1;
	
	bool goOut = 0;
	uint32_t elapsedMillis = millis();
	uint8_t scale;
	if(resolutionMode)
		scale = 1;
	else
		scale = 2;

	FastLED.clear();

	while (1)
	{
		display.fillScreen(backgroundColors[backgroundIndex]);

		display.setFreeFont(TT1);
		display.setTextSize(3*scale);//
		//Hour shadow
		display.setTextColor(TFT_DARKGREY);
		if (clockHour == 11)
			display.setCursor(10*scale, 28*scale);
		else if (clockHour % 10 == 1 || (int)clockHour / 10 == 1)
			display.setCursor(7*scale, 28*scale);
		else
			display.setCursor(4*scale, 28*scale);
		if (clockHour < 10)
			display.print("0");
		display.print(clockHour);

		//minute shadow
		display.setCursor(36*scale, 28*scale);
		if (clockMinute < 10)
			display.print("0");
		display.print(clockMinute);

		//Hour black
		display.setTextColor(TFT_BLACK);
		if (clockHour == 11)
			display.setCursor(9*scale, 27*scale);
		else if (clockHour % 10 == 1 || (int)clockHour / 10 == 1)
			display.setCursor(6*scale, 27*scale);
		else
			display.setCursor(3*scale, 27*scale);
		if (clockHour < 10)
			display.print("0");
		display.print(clockHour);

		//Minute black
		display.setCursor(35*scale, 27*scale);
		if (clockMinute < 10)
			display.print("0");
		display.print(clockMinute);

		display.setTextSize(1*scale);
		display.setCursor(60*scale, 19*scale);
		display.setTextWrap(false);
		if (clockDay < 10)
			display.print("0");
		display.print(clockDay);
		display.print("/");
		if (clockMonth < 10)
			display.print("0");
		display.print(clockMonth);
		display.setCursor(62*scale, 25*scale);
		display.print(2000 + clockYear);
		/*display.setTextSize(2);
		display.setCursor(10, 50);
		display.print("12:00");*/
		uint8_t helper = 11;
		if (simInserted && !airplaneMode)
		{
			if (signalStrength <= 3)
				display.drawBitmap(1*scale, 1*scale, noSignalIcon, TFT_BLACK, scale);
			else if (signalStrength > 3 && signalStrength <= 10)
				display.drawBitmap(1*scale, 1*scale, signalLowIcon, TFT_BLACK, scale);
			else if (signalStrength > 10 && signalStrength <= 20)
				display.drawBitmap(1*scale, 1*scale, signalHighIcon, TFT_BLACK, scale);
			else if (signalStrength > 20 && signalStrength <= 31)
				display.drawBitmap(1*scale, 1*scale, signalFullIcon, TFT_BLACK, scale);
			else if (signalStrength == 99)
				display.drawBitmap(1*scale, 1*scale, signalErrorIcon, TFT_BLACK, scale);
		}
		else if(!simInserted && !airplaneMode)
			display.drawBitmap(1*scale, 1*scale, signalErrorIcon, TFT_BLACK, scale);
		if (volume == 0)
		{
			display.drawBitmap(helper*scale, 1*scale, silentmode, TFT_BLACK, scale);
			helper += 10;
		}
		//display.drawBitmap(31, 1, missedcall);
		//display.drawBitmap(41, 1, newtext);
		if (!airplaneMode)
		{
			if (wifi == 1)
				display.drawBitmap(helper*scale, 1*scale, wifion, TFT_BLACK, scale);
			else
				display.drawBitmap(helper*scale, 1*scale, wifioff, TFT_BLACK, scale);
			helper += 10;
			if (bt)
				display.drawBitmap(helper*scale, 1*scale, BTon, TFT_BLACK, scale);
			else
				display.drawBitmap(helper*scale, 1*scale, BToff, TFT_BLACK, scale);
			helper += 10;
		}
		else
		{
			display.drawBitmap(scale, scale, airplaneModeIcon, TFT_BLACK, scale);
			helper += 10;
		}

		if(!SDinsertedFlag)
			display.drawBitmap(helper*scale, 1*scale, noSDIcon, TFT_BLACK, scale);

		if (batteryVoltage > 4000)
			display.drawBitmap(74*scale, 1*scale, batteryCharging, TFT_BLACK, scale);
		else if (batteryVoltage <= 4000 && batteryVoltage >= 3800)
			display.drawBitmap(74*scale, 1*scale, batteryFull, TFT_BLACK, scale);
		else if (batteryVoltage < 3800 && batteryVoltage >= 3700)
			display.drawBitmap(74*scale, 1*scale, batteryMid, TFT_BLACK, scale);
		else if (batteryVoltage < 3700 && batteryVoltage >= 3600)
			display.drawBitmap(74*scale, 1*scale, batteryMidLow, TFT_BLACK, scale);
		else if (batteryVoltage < 3600 && batteryVoltage >= 3500)
			display.drawBitmap(74*scale, 1*scale, batteryLow, TFT_BLACK, scale);
		else if (batteryVoltage < 3500)
			display.drawBitmap(74*scale, 1*scale, batteryEmpty, TFT_BLACK, scale);

		gui.drawNotificationWindow(2*scale, 32*scale, 77*scale, 10*scale, "Missed call from Dad");
		gui.drawNotificationWindow(2*scale, 44*scale, 77*scale, 10*scale, "Text from Jack");
		display.setFreeFont(TT1);
		display.setTextSize(2*scale);
		if (millis() - elapsedMillis >= 500) {
			elapsedMillis = millis();
			blinkState = !blinkState;
			if (clockYear != 4 && clockYear != 80)
				updateTimeRTC();
		}


		if (blinkState == 1)
		{
			display.setTextSize(1);
			if(scale == 1)
			{
				display.setCursor(1, 63);
				display.setFreeFont(TT1);
			}
			else
			{
				/* display.setCursor(2, 63*scale);
				display.setFreeFont(FSS9); */
				display.setCursor(2, 111);
				display.setTextFont(2);
			}
			display.setTextColor(TFT_BLACK);
			display.print("Hold \"A\" to unlock");
			display.setTextSize(3*scale);
			display.setTextColor(TFT_DARKGREY);
			display.setFreeFont(TT1);
			display.setCursor(29*scale, 28*scale);
			display.print(":");
			display.setTextColor(TFT_BLACK);
			display.setCursor(28*scale, 27*scale);
			display.print(":");
		}
		update();

		display.setTextSize(1*scale);

		if (buttons.pressed(BTN_A)) {
			display.setTextSize(1);
			vibration(200);

			display.fillRect(0, 56*scale, display.width(), 7*scale, backgroundColors[backgroundIndex]);
			if(mp.resolutionMode)
			{
				display.setCursor(1, 63);
				display.setFreeFont(TT1);
			}
			else
			{
				display.setCursor(2, 111);
				display.setTextFont(2);
			}
			display.print("Unlocking");
			update();
			buttonHeld = millis();
			pixelState = 0;
			while (buttons.kpd.pin_read(BTN_A) == 0)
			{

				if (millis() - buttonHeld > 250 && millis() - buttonHeld < 500) {
					display.fillRect(0, 57*scale, display.width(), 7*scale, backgroundColors[backgroundIndex]);
					if(mp.resolutionMode)
					{
						display.setCursor(1, 63);
						display.setFreeFont(TT1);
					}
					else
					{
						display.setCursor(2, 111);
						display.setTextFont(2);
					}
					leds[0] = CRGB::Red;
					leds[7] = CRGB::Red;
					display.print("Unlocking *");
				}
				else if (millis() - buttonHeld > 500 && millis() - buttonHeld < 750)
				{
					display.fillRect(0, 57*scale, display.width(), 7*scale, backgroundColors[backgroundIndex]);
					if(mp.resolutionMode)
					{
						display.setCursor(1, 63);
						display.setFreeFont(TT1);
					}
					else
					{
						display.setCursor(2, 111);
						display.setTextFont(2);
					}
					display.print("Unlocking * *");
					leds[0] = CRGB::Red;
					leds[7] = CRGB::Red;
					leds[1] = CRGB::Red;
					leds[6] = CRGB::Red;
				}
				else if (millis() - buttonHeld > 750 && millis() - buttonHeld < 1000)
				{
					display.fillRect(0, 57*scale, display.width(), 7*scale, backgroundColors[backgroundIndex]);
					if(mp.resolutionMode)
					{
						display.setCursor(1, 63);
						display.setFreeFont(TT1);
					}
					else
					{
						display.setCursor(2, 111);
						display.setTextFont(2);
					}
					display.print("Unlocking * * *");
					leds[0] = CRGB::Red;
					leds[7] = CRGB::Red;
					leds[1] = CRGB::Red;
					leds[6] = CRGB::Red;
					leds[2] = CRGB::Red;
					leds[5] = CRGB::Red;
				}
				else if (millis() - buttonHeld > 1000)
				{
					leds[0] = CRGB::Red;
					leds[7] = CRGB::Red;
					leds[1] = CRGB::Red;
					leds[6] = CRGB::Red;
					leds[2] = CRGB::Red;
					leds[5] = CRGB::Red;
					leds[3] = CRGB::Red;
					leds[4] = CRGB::Red;

					FastLED.show();

					while(!update());
					if(buttons.released(BTN_A))
					{
						vibration(200);
						Serial.println(millis() - buttonHeld);
						goOut = 1;
						FastLED.clear();
						delay(10);
						break;
					}
				}
				update();
			}
		}
		if (buttons.released(BTN_A))
			FastLED.clear();
		if (buttons.pressed(BTN_B)) {
			while (buttons.kpd.pin_read(BTN_B) == 0);
			sleep();
		}

		if (goOut == 1 && buttons.released(BTN_A))
			break;
	}
}

// TODO FastLED from now on

void MAKERphone::updateTimeGSM() {
	Serial1.write("AT+CCLK?\r");
	//Serial1.flush();
	//delay(500);
	String reply;
	while (!Serial1.available())
		update();
	while (Serial1.available()) {
		reply += (char)Serial1.read();

		//if (Serial1.read() == '"') {
		//  char c1 = Serial1.read();
		//  Serial.println(c1);
		//  char c2 = Serial1.read();
		//  Serial.println(c2);
		//  clockYear = 2000 + ((c1 - '0') * 10) + (c2 - '0');
		//}
		//Serial.println(clockYear);
	}
	Serial.print(F("REPLY:"));
	Serial.println(reply);

	uint16_t index = reply.indexOf(F("+CCLK: \""));
	Serial.println(F("INDEX:"));
	Serial.println(index);
	Serial.println(reply.charAt(index));

	char c1, c2; //buffer for saving date and time numerals in form of characters
	c1 = reply.charAt(index + 8);
	c2 = reply.charAt(index + 9);
	clockYear = 2000 + ((c1 - '0') * 10) + (c2 - '0');
	Serial.println(F("CLOCK YEAR:"));
	Serial.println(clockYear);
	clockYear = ((c1 - '0') * 10) + (c2 - '0');

	c1 = reply.charAt(index + 11);
	c2 = reply.charAt(index + 12);
	clockMonth = ((c1 - '0') * 10) + (c2 - '0');
	Serial.println(F("CLOCK MONTH:"));
	Serial.println(clockMonth);

	c1 = reply.charAt(index + 14);
	c2 = reply.charAt(index + 15);
	clockDay = ((c1 - '0') * 10) + (c2 - '0');
	Serial.println(F("CLOCK DAY:"));
	Serial.println(clockDay);

	c1 = reply.charAt(index + 17);
	c2 = reply.charAt(index + 18);
	clockHour = ((c1 - '0') * 10) + (c2 - '0');
	Serial.println(F("CLOCK HOUR:"));
	Serial.println(clockHour);

	c1 = reply.charAt(index + 20);
	c2 = reply.charAt(index + 21);
	clockMinute = ((c1 - '0') * 10) + (c2 - '0');
	Serial.println(F("CLOCK MINUTE:"));
	Serial.println(clockMinute);

	c1 = reply.charAt(index + 23);
	c2 = reply.charAt(index + 24);
	clockSecond = ((c1 - '0') * 10) + (c2 - '0');
	Serial.println(F("CLOCK SECOND:"));
	Serial.println(clockSecond);


	buttons.kpd.setHour(clockHour);
	buttons.kpd.setMinute(clockMinute);
	buttons.kpd.setSecond(clockSecond);
	buttons.kpd.setDate(clockDay);
	buttons.kpd.setMonth(clockMonth);
	buttons.kpd.setYear(clockYear);
	Serial.println(F("\nRTC TIME UPDATE OVER GSM DONE!"));
	delay(5);
}
void MAKERphone::updateTimeRTC() {
	clockHour = buttons.kpd.getHour(h12, PM);
	clockMinute = buttons.kpd.getMinute();
	clockSecond = buttons.kpd.getSecond();
	clockDay = buttons.kpd.getDate();
	clockMonth = buttons.kpd.getMonth(Century);
	clockYear = buttons.kpd.getYear();

	//Serial.println(F("CLOCK HOUR:"));
	//Serial.println(clockHour);

	//Serial.println(F("CLOCK MINUTE:"));
	//Serial.println(clockMinute);

	//Serial.println(F("CLOCK SECOND:"));
	//Serial.println(clockSecond);

	//Serial.println(F("\nGLOBAL TIME UPDATE OVER RTC DONE!"));
}
void MAKERphone::listBinaries(const char * dirname, uint8_t levels) {
	
	binaryCount = 0;
	Serial.printf("Listing directory: %s\n", dirname);

	File root = SD.open(dirname);
	if (!root) {
		Serial.println("Failed to open directory");
		return;
	}
	if (!root.isDirectory()) {
		Serial.println("Not a directory");
		return;

	}
	int counter = 1;

	File file = root.openNextFile();
	while (file) {

		/*if (file.isDirectory()) {
		  Serial.print("  DIR : ");
		  Serial.println(file.name());
		  if (levels) {
		  listBinaries(fs, file.name(), levels - 1);
		  }
		  }
		  else {
		  String Name = file.name();
		  if (Name.endsWith(F(".bin")))
		  {
		  Serial.print(file.name());
		  BinaryFiles[counter-1] = file.name();
		  }
		  }*/
		char temp[50];
		file.getName(temp,  50);
		String Name(temp);
		Serial.println(Name);
		if (Name.endsWith(F(".BIN")) || Name.endsWith(F(".bin")))
		{
			Serial.print(counter);
			Serial.print(".   ");
			Serial.println(Name);
			BinaryFiles[counter - 1] = Name;
			counter++;
			binaryCount++;
		}
		file = root.openNextFile();
	}
}
void MAKERphone::listDirectories(const char * dirname) {
	
	directoryCount = 0;
	Serial.printf("Listing directory: %s\n", dirname);

	File root = SD.open(dirname);
	if (!root) {
		Serial.println("Failed to open directory");
		return;
	}
	if (!root.isDirectory()) {
		Serial.println("Not a directory");
		return;

	}
	int counter = 0;

	File file = root.openNextFile();
	while (file) {

		if (file.isDirectory()) {
		  	char temp[100];
			file.getName(temp,  100);
			String Name(temp);
			if(Name != "Images" && Name != "Music" && Name != "Video" && Name != "System Volume Information")
			{
				
				Serial.println(Name);
				directories[directoryCount] = Name;
				counter++;
				directoryCount++;
			}
		  }
		file = root.openNextFile();
	}
}

void MAKERphone::performUpdate(Stream &updateSource, size_t updateSize) {
	if (Update.begin(updateSize)) {
		size_t written = Update.writeStream(updateSource);
		if (written == updateSize) {
			Serial.println("Written : " + String(written) + " successfully");
		}
		else {
			Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
		}
		if (Update.end()) {
			Serial.println("OTA done!");
			if (Update.isFinished()) {
				Serial.println("Update successfully completed. Rebooting.");
				ESP.restart();
			}
			else {
				Serial.println("Update not finished? Something went wrong!");
			}
		}
		else {
			Serial.println("Error Occurred. Error #: " + String(Update.getError()));
		}

	}
	else
	{
		Serial.println("Not enough space to begin OTA");
	}
}
void MAKERphone::updateFromFS(String FilePath) {
	Serial.println(FilePath);
	File updateBin = SD.open(FilePath);
	if (updateBin) {
		if (updateBin.isDirectory()) {
			Serial.println("Error, update.bin is not a file");
			updateBin.close();
			return;
		}

		size_t updateSize = updateBin.size();

		if (updateSize > 0) {
			Serial.println("Try to start update");
			performUpdate(updateBin, updateSize);
		}
		else {
			Serial.println("Error, file is empty");
		}

		updateBin.close();
	}
	else {
		Serial.println("Could not load update.bin from sd root");
	}
}
void MAKERphone::mainMenu()
{
	while (buttons.kpd.pin_read(BTN_A) == 0);
	Serial.println("entered main menu");
	while (1)
	{

		
		int8_t index = gui.scrollingMainMenu();
		Serial.println(index);
		delay(5);
		if(index < 10)
		{
			if (titles[index] == "Apps")
			{
				display.fillScreen(TFT_BLACK);
				update();
				if(SDinsertedFlag)
				{
					while(!SD.begin(5, SD_SCK_MHZ(8)));
					listBinaries("/", 0);
					int8_t index = gui.menu("Load from SD", BinaryFiles, binaryCount);

					if (index != -1) {  //IF BUTTON "BACK" WAS NOT PRESSED
						display.fillScreen(TFT_BLACK);
						display.setCursor(0,display.height() / 2 - 16);
						display.printCenter("LOADING NOW...");
						while(!update());


						updateFromFS(BinaryFiles[index]);
					}
				}
				else
				{
					display.setCursor(0, display.height()/2 - 20);
					display.setTextFont(2);
					display.printCenter("No SD inserted!");
					display.setCursor(0, display.height()/2);
					display.printCenter("Insert SD card and reset");
					uint32_t tempMillis = millis();
					while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
						update();
					while(!update());
				}



			}

			if (titles[index] == "Messages")
			{
				display.fillScreen(TFT_BLACK);
				display.setTextColor(TFT_WHITE);
				if(simInserted && !airplaneMode)
				{
					display.fillScreen(TFT_BLACK);
					if(resolutionMode)
						display.setCursor(0, display.height()/2);
					else
						display.setCursor(0, display.height()/2 - 16);
					display.printCenter("Loading messages...");
					while(!update());
					messagesApp();
				}
				else if(!simInserted)
				{
					display.setCursor(0, display.height()/2 - 20);
					display.setTextFont(2);
					display.printCenter("No SIM inserted!");
					display.setCursor(0, display.height()/2);
					display.printCenter("Insert SIM and reset");
					uint32_t tempMillis = millis();
					while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
						update();
					while(!update());
				}
				else if(airplaneMode)
				{
					display.setCursor(0, display.height()/2 - 20);
					display.setTextFont(2);
					display.printCenter("Can't access SMS!");
					display.setCursor(0, display.height()/2);
					display.printCenter("Turn off airplane mode");
					uint32_t tempMillis = millis();
					while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
						update();
					while(!update());
				}

			}

			if (titles[index] == "Media")
			{
				display.fillScreen(TFT_BLACK);
				display.setTextColor(TFT_WHITE);
				if(SDinsertedFlag)
					mediaApp();
				else
				{
					display.setCursor(0, display.height()/2 - 20);
					display.setTextFont(2);
					display.printCenter("No SD inserted!");
					display.setCursor(0, display.height()/2);
					display.printCenter("Insert SD card and reset");
					uint32_t tempMillis = millis();
					while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
						update();
					while(!update());
				}
			}

			if (titles[index] == "Phone")
			{
				display.fillScreen(TFT_BLACK);
				display.setTextColor(TFT_WHITE);
				if(simInserted && !airplaneMode)
					phoneApp();
				else if(!simInserted)
				{
					display.setCursor(0, display.height()/2 - 20);
					display.setTextFont(2);
					display.printCenter("No SIM inserted!");
					display.setCursor(0, display.height()/2);
					display.printCenter("Insert SIM and reset");
					uint32_t tempMillis = millis();
					while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
						update();
					while(!update());
				}
				else if(airplaneMode)
				{
					display.setCursor(0, display.height()/2 - 20);
					display.setTextFont(2);
					display.printCenter("Can't dial numbers");
					display.setCursor(0, display.height()/2);
					display.printCenter("Turn off airplane mode");
					uint32_t tempMillis = millis();
					while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
						update();
					while(!update());
				}
			}
			if (titles[index] == "Contacts") {
				display.fillScreen(TFT_BLACK);
				display.setTextColor(TFT_WHITE);
				if(SDinsertedFlag && !airplaneMode)
				{
					display.fillScreen(TFT_BLACK);
					if(resolutionMode)
					{
						display.setCursor(22, 30);
						display.print("Loading");
						display.setCursor(20, 36);
						display.print("contacts...");
					}
					else
					{
						display.setCursor(0,display.height()/2 -16);
						display.printCenter("Loading contacts...");
					}
					contactsAppSD();
				}
				else if(!SDinsertedFlag)
				{
					display.setCursor(0, display.height()/2 - 20);
					display.setTextFont(2);
					display.printCenter("No SD inserted!");
					display.setCursor(0, display.height()/2);
					display.printCenter("Insert SD and reset");
					uint32_t tempMillis = millis();
					while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
						update();
					while(!update());
				}
				else if(airplaneMode)
				{
					display.setCursor(0, display.height()/2 - 20);
					display.setTextFont(2);
					display.printCenter("Can't access contacts!");
					display.setCursor(0, display.height()/2);
					display.printCenter("Turn off airplane mode");
					uint32_t tempMillis = millis();
					while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
						update();
					while(!update());
				}

			}

			if (titles[index] == "Settings")
			{
				Serial.println("entering");
				delay(5);
				if(settingsApp())
					return;
			}
			if (index == -2)
			{
				Serial.println("pressed");
				break;
			}
			if (index == -3) // DEBUG MODE
				debugMode();
		}
		else
		{
			display.fillScreen(TFT_BLACK);
			display.setCursor(0,display.height() / 2 - 16);
			display.printCenter("LOADING NOW...");
			while(!update());

			String foo = directories[index - 10];
			updateFromFS(String("/" + foo + "/" + foo + ".bin"));
		}
		update();
	}
}
void MAKERphone::bigIconsMainMenu() {
	
	while (buttons.kpd.pin_read(BTN_A) == 0);
	Serial.println("entered main menu");
	Serial.flush();
	while (1)
	{

		
		int8_t index = gui.drawBigIconsCursor((width+2)*2, (bigIconHeight*2 + 3), 3, 2, 3, 17);
		Serial.println(index);
		delay(5);
		if (titles[index] == "Apps")
		{
			display.fillScreen(TFT_BLACK);
			update();
			if(SDinsertedFlag)
			{
				while(!SD.begin(5, SD_SCK_MHZ(8)));
				listBinaries("/", 0);
				int8_t index = gui.menu("Load from SD", BinaryFiles, binaryCount);

				if (index != -1) {  //IF BUTTON "BACK" WAS NOT PRESSED
					display.fillScreen(TFT_BLACK);
					display.setCursor(0,display.height() / 2 - 16);
					display.printCenter("LOADING NOW...");
					while(!update());


					updateFromFS(BinaryFiles[index]);
				}
			}
			else
			{
				display.setCursor(0, display.height()/2 - 20);
				display.setTextFont(2);
				display.printCenter("No SD inserted!");
				display.setCursor(0, display.height()/2);
				display.printCenter("Insert SD card and reset");
				uint32_t tempMillis = millis();
				while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
					update();
				while(!update());
			}



		}

		if (titles[index] == "Messages")
		{
			display.fillScreen(TFT_BLACK);
			display.setTextColor(TFT_WHITE);
			if(simInserted && !airplaneMode)
			{
				display.fillScreen(TFT_BLACK);
				if(resolutionMode)
					display.setCursor(0, display.height()/2);
				else
					display.setCursor(0, display.height()/2 - 16);
				display.printCenter("Loading messages...");
				while(!update());
				messagesApp();
			}
			else if(!simInserted)
			{
				display.setCursor(0, display.height()/2 - 20);
				display.setTextFont(2);
				display.printCenter("No SIM inserted!");
				display.setCursor(0, display.height()/2);
				display.printCenter("Insert SIM and reset");
				uint32_t tempMillis = millis();
				while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
					update();
				while(!update());
			}
			else if(airplaneMode)
			{
				display.setCursor(0, display.height()/2 - 20);
				display.setTextFont(2);
				display.printCenter("Can't access SMS!");
				display.setCursor(0, display.height()/2);
				display.printCenter("Turn off airplane mode");
				uint32_t tempMillis = millis();
				while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
					update();
				while(!update());
			}

		}

		if (titles[index] == "Media")
		{
			display.fillScreen(TFT_BLACK);
			display.setTextColor(TFT_WHITE);
			if(SDinsertedFlag)
				mediaApp();
			else
			{
				display.setCursor(0, display.height()/2 - 20);
				display.setTextFont(2);
				display.printCenter("No SD inserted!");
				display.setCursor(0, display.height()/2);
				display.printCenter("Insert SD card and reset");
				uint32_t tempMillis = millis();
				while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
					update();
				while(!update());
			}
		}

		if (titles[index] == "Phone")
		{
			display.fillScreen(TFT_BLACK);
			display.setTextColor(TFT_WHITE);
			if(simInserted && !airplaneMode)
				phoneApp();
			else if(!simInserted)
			{
				display.setCursor(0, display.height()/2 - 20);
				display.setTextFont(2);
				display.printCenter("No SIM inserted!");
				display.setCursor(0, display.height()/2);
				display.printCenter("Insert SIM and reset");
				uint32_t tempMillis = millis();
				while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
					update();
				while(!update());
			}
			else if(airplaneMode)
			{
				display.setCursor(0, display.height()/2 - 20);
				display.setTextFont(2);
				display.printCenter("Can't dial numbers");
				display.setCursor(0, display.height()/2);
				display.printCenter("Turn off airplane mode");
				uint32_t tempMillis = millis();
				while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
					update();
				while(!update());
			}
		}
		if (titles[index] == "Contacts") {
			display.fillScreen(TFT_BLACK);
			display.setTextColor(TFT_WHITE);
			if(SDinsertedFlag && !airplaneMode)
			{
				display.fillScreen(TFT_BLACK);
				if(resolutionMode)
				{
					display.setCursor(22, 30);
					display.print("Loading");
					display.setCursor(20, 36);
					display.print("contacts...");
				}
				else
				{
					display.setCursor(0,display.height()/2 -16);
					display.printCenter("Loading contacts...");
				}
				contactsAppSD();
			}
			else if(!SDinsertedFlag)
			{
				display.setCursor(0, display.height()/2 - 20);
				display.setTextFont(2);
				display.printCenter("No SD inserted!");
				display.setCursor(0, display.height()/2);
				display.printCenter("Insert SD and reset");
				uint32_t tempMillis = millis();
				while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
					update();
				while(!update());
			}
			else if(airplaneMode)
			{
				display.setCursor(0, display.height()/2 - 20);
				display.setTextFont(2);
				display.printCenter("Can't access contacts!");
				display.setCursor(0, display.height()/2);
				display.printCenter("Turn off airplane mode");
				uint32_t tempMillis = millis();
				while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
					update();
				while(!update());
			}

		}

		if (titles[index] == "Settings")
		{
			Serial.println("entering");
			delay(5);
			if(settingsApp())
				return;
		}
		if (index == -2)
		{
			Serial.println("pressed");
			break;
		}
		if (index == -3) // DEBUG MODE
			debugMode();
		update();
	}
}
void MAKERphone::callNumber(String number) {
	dataRefreshFlag = 0;
	
	String localBuffer = "";
	Serial1.print(F("ATD"));
	Serial1.print(number);
	Serial1.print(";\r\n");
	display.setFreeFont(TT1);
	display.setTextColor(TFT_BLACK);
	bool firstPass = 1;
	uint32_t timeOffset = 0;
	uint16_t textLength;
	uint8_t scale;
	String temp;
	if(resolutionMode)
	{
		scale = 1;
		display.setFreeFont(TT1);
	}
	else
	{
		scale = 2;
		display.setTextFont(2);
	}
	display.setTextSize(1);

	display.printCenter(number);
	textLength = display.cursor_x - textLength;
	while (1)
	{
		display.fillScreen(TFT_WHITE);
		if (Serial1.available())
			localBuffer = Serial1.readString();
		Serial.println(localBuffer);
		delay(5);
		if (localBuffer.indexOf("CLCC:") != -1)
		{
			if (localBuffer.indexOf(",0,0,0,0") != -1)
			{
				if (firstPass == 1)
				{
					timeOffset = millis();
					firstPass = 0;
				}


				temp = "";
				if ((int((millis() - timeOffset) / 1000) / 60) > 9)
					temp += (int((millis() - timeOffset) / 1000) / 60) ;
				else
				{
					temp += "0";
					temp += (int((millis() - timeOffset) / 1000) / 60);
				}
				temp += ":";
				if (int((millis() - timeOffset) / 1000) % 60 > 9)
					temp += (int((millis() - timeOffset) / 1000) % 60);
				else
				{
					temp += "0";
					temp += (int((millis() - timeOffset) / 1000) % 60);
				}
				display.setCursor(9, 9);
				display.printCenter(temp);
				Serial.println("CALL ACTIVE");
				display.drawBitmap(29*scale, 24*scale, call_icon, TFT_GREEN, scale);
			}

			else if (localBuffer.indexOf(",0,3,") != -1)
			{
				display.setCursor(25, 9);
				Serial.println("ringing");
				display.printCenter("Ringing...");
				display.drawBitmap(29*scale, 24*scale, call_icon, TFT_DARKGREY, scale);
			}
			else if (localBuffer.indexOf(",0,2,") != -1)
			{
				display.setCursor(25, 9);
				display.printCenter("Calling...");
				display.drawBitmap(29*scale, 24*scale, call_icon, TFT_DARKGREY, scale);
			}
			else if (localBuffer.indexOf(",0,6,") != -1)
			{
				display.fillScreen(TFT_WHITE);
				display.setCursor(32, 9);
				if (timeOffset == 0)
					display.printCenter("00:00");
				else
				{
					temp = "";
					if ((int((millis() - timeOffset) / 1000) / 60) > 9)
						temp += (int((millis() - timeOffset) / 1000) / 60) ;
					else
					{
						temp += "0";
						temp += (int((millis() - timeOffset) / 1000) / 60);
					}
					temp += ":";
					if (int((millis() - timeOffset) / 1000) % 60 > 9)
						temp += (int((millis() - timeOffset) / 1000) % 60);
					else
					{
						temp += "0";
						temp += (int((millis() - timeOffset) / 1000) % 60);
					}
					display.setCursor(9, 9);
					display.printCenter(temp);
				}
				display.drawBitmap(29*scale, 24*scale, call_icon, TFT_RED, scale);
				if(resolutionMode)
					display.setCursor(11, 20);
				else
					display.setCursor(11, 28);
				display.printCenter(number);
				display.fillRect(0, 51*scale, 80*scale, 13*scale, TFT_RED);
				if(resolutionMode)
					display.setCursor(2, 62);
				else
					display.setCursor(2, 112);
				display.print("Call ended");
				Serial.println("ENDED");
				while (!update());
				delay(1000);
				break;
			}
			if(resolutionMode)
					display.setCursor(11, 20);
			else
				display.setCursor(11, 28);
			display.printCenter(number);
			display.fillRect(0, 51*scale, 80*scale, 13*scale, TFT_RED);
			if(resolutionMode)
			{
				display.setCursor(2, 62);
				display.print("press");
				display.drawBitmap(24, 52, letterB, TFT_BLACK);
				display.setCursor(35, 62);
				display.print("to hang up");
			}
			else
			{
				display.setCursor(2, 112);
				display.print("press");
				display.drawBitmap(37, 105, letterB, TFT_BLACK, scale);
				display.setCursor(55, 112);
				display.print("to hang up");
			}

		}
		else if (localBuffer.indexOf("CLCC:") == -1)
		{
			if (localBuffer.indexOf("ERROR") != -1)
			{

				display.setCursor(3, 9);
				display.printCenter("Couldn't dial number!");
				display.drawBitmap(29*scale, 24*scale, call_icon, TFT_RED, scale);
				if(resolutionMode)
					display.setCursor(11, 20);
				else
					display.setCursor(11, 28);
				display.printCenter(number);
				display.fillRect(0, 51*scale, 80*scale, 13*scale, TFT_RED);
				if(resolutionMode)
				{
					display.setCursor(2, 57);
					display.print("Invalid number or");
					display.setCursor(2, 63);
					display.print("SIM card missing!");
				}
				else
				{
					display.setCursor(2, 100);
					display.print("Invalid number or");
					display.setCursor(2, 112);
					display.print("SIM card missing!");
				}
				while (!buttons.released(BTN_B))
					update();
				break;
			}
			else
			{
				display.setCursor(25, 9);
				display.printCenter("Calling...");
				display.drawBitmap(29*scale, 24*scale, call_icon, TFT_DARKGREY, scale);
				if(resolutionMode)
					display.setCursor(11, 20);
				else
					display.setCursor(11, 28);
				display.printCenter(number);
				display.fillRect(0, 51*scale, 80*scale, 13*scale, TFT_RED);
				if(resolutionMode)
				{
					display.setCursor(2, 62);
					display.print("press");
					display.drawBitmap(24, 52, letterB, TFT_BLACK);
					display.setCursor(35, 62);
					display.print("to hang up");
				}
				else
				{
					display.setCursor(2, 112);
					display.print("press");
					display.drawBitmap(37, 105, letterB, TFT_BLACK, scale);
					display.setCursor(55, 112);
					display.print("to hang up");
				}
			}
		}
		if (buttons.pressed(BTN_B)) // hanging up
		{
			Serial.println("B PRESSED");
			Serial1.println("ATH");
			while (readSerial().indexOf(",0,6,") == -1)
			{
				Serial1.println("ATH");
			}
			display.fillScreen(TFT_WHITE);
			display.setCursor(32, 9);
			if (timeOffset == 0)
				display.printCenter("00:00");
			else
			{
				temp = "";
				if ((int((millis() - timeOffset) / 1000) / 60) > 9)
					temp += (int((millis() - timeOffset) / 1000) / 60) ;
				else
				{
					temp += "0";
					temp += (int((millis() - timeOffset) / 1000) / 60);
				}
				temp += ":";
				if (int((millis() - timeOffset) / 1000) % 60 > 9)
					temp += (int((millis() - timeOffset) / 1000) % 60);
				else
				{
					temp += "0";
					temp += (int((millis() - timeOffset) / 1000) % 60);
				}
				display.setCursor(9, 9);
				display.printCenter(temp);
			}
			display.drawBitmap(29*scale, 24*scale, call_icon, TFT_RED, scale);
			if(resolutionMode)
					display.setCursor(11, 20);
			else
				display.setCursor(11, 28);
			display.printCenter(number);
			display.fillRect(0, 51*scale, 80*scale, 13*scale, TFT_RED);
			display.setCursor(2, 112);
			display.print("Call ended");
			Serial.println("ENDED");
			while (!update());
			delay(1000);
			break;
		}
		update();
	}
}
void MAKERphone::checkSMS() {
}
String MAKERphone::readSerial() {
	uint8_t _timeout = 0;
	while (!Serial1.available() && _timeout < 300)
	{
		delay(20);
		_timeout++;
	}
	if (Serial1.available()) {
		return Serial1.readString();
	}
	return "";
}
void MAKERphone::incomingCall()
{
	//String localBuffer = "";
	//Serial1.print(F("ATD"));
	//Serial1.print(number);
	//Serial1.print(";\r\n");
	//display.setFreeFont(TT1);
	//display.setTextColor(TFT_BLACK);
	//bool firstPass = 1;
	//uint32_t timeOffset = 0;
	//display.setTextSize(1);
	//while (1)
	//{
	//	display.fillScreen(TFT_WHITE);
	//	if (Serial1.available())
	//		localBuffer = Serial1.readString();
	//	if (localBuffer.indexOf("CLCC:") != -1)
	//	{
	//		if (localBuffer.indexOf(",0,0,0,0") != -1)
	//		{
	//			if (firstPass == 1)
	//			{
	//				timeOffset = millis();
	//				firstPass = 0;
	//			}

	//			display.setCursor(32, 9);
	//			if ((int((millis() - timeOffset) / 1000) / 60) > 9)
	//				display.print(int((millis() - timeOffset) / 1000) / 60);
	//			else
	//			{
	//				display.print("0");
	//				display.print(int((millis() - timeOffset) / 1000) / 60);
	//			}
	//			display.print(":");
	//			if (int((millis() - timeOffset) / 1000) % 60 > 9)
	//				display.print(int((millis() - timeOffset) / 1000) % 60);
	//			else
	//			{
	//				display.print("0");
	//				display.print(int((millis() - timeOffset) / 1000) % 60);
	//			}
	//			Serial.println("CALL ACTIVE");
	//			display.drawBitmap(29, 24, call_icon, TFT_GREEN);
	//		}

	//		else if (localBuffer.indexOf(",0,3,") != -1)
	//		{
	//			display.setCursor(25, 9);
	//			Serial.println("ringing");
	//			display.println("Ringing...");
	//			display.drawBitmap(29, 24, call_icon, TFT_DARKGREY);
	//		}
	//		else if (localBuffer.indexOf(",0,2,") != -1)
	//		{
	//			display.setCursor(25, 9);
	//			display.println("Calling...");
	//			display.drawBitmap(29, 24, call_icon, TFT_DARKGREY);
	//		}
	//		else if (localBuffer.indexOf(",0,6,") != -1)
	//		{
	//			display.fillScreen(TFT_WHITE);
	//			display.setCursor(32, 9);
	//			if (timeOffset == 0)
	//				display.print("00:00");
	//			else
	//			{
	//				if ((int((millis() - timeOffset) / 1000) / 60) > 9)
	//					display.print(int((millis() - timeOffset) / 1000) / 60);
	//				else
	//				{
	//					display.print("0");
	//					display.print(int((millis() - timeOffset) / 1000) / 60);
	//				}
	//				display.print(":");
	//				if ((int((millis() - timeOffset) / 1000) % 60) > 9)
	//					display.print(int((millis() - timeOffset) / 1000) % 60);
	//				else
	//				{
	//					display.print("0");
	//					display.print(int((millis() - timeOffset) / 1000) % 60);
	//				}
	//			}
	//			display.drawBitmap(29, 24, call_icon, TFT_RED);
	//			display.setCursor(11, 20);
	//			display.println(number);
	//			display.fillRect(0, 51, 80, 13, TFT_RED);
	//			display.setCursor(2, 62);
	//			display.print("Call ended");
	//			Serial.println("ENDED");
	//			while (!update());
	//			delay(1000);
	//			break;
	//		}
	//		display.setCursor(11, 20);
	//		display.println(number);
	//		display.fillRect(0, 51, 80, 13, TFT_RED);
	//		display.setCursor(2, 62);
	//		display.print("press");
	//		display.drawBitmap(24, 52, letterB, TFT_BLACK);
	//		display.setCursor(35, 62);
	//		display.print("to hang up");

	//	}
	//	else if (localBuffer.indexOf("CLCC:") == -1)
	//	{
	//		display.setCursor(25, 9);
	//		display.println("Calling...");
	//		display.drawBitmap(29, 24, call_icon, TFT_DARKGREY);
	//		display.setCursor(11, 20);
	//		display.println(number);
	//		display.fillRect(0, 51, 80, 13, TFT_RED);
	//		display.setCursor(2, 62);
	//		display.print("press");
	//		display.drawBitmap(24, 52, letterB, TFT_BLACK);
	//		display.setCursor(35, 62);
	//		display.print("to hang up");
	//	}
	//	if (buttons.pressed(BTN_B)) // hanging up
	//	{
	//		Serial.println("B PRESSED");
	//		Serial1.println("ATH");
	//		while (readSerial().indexOf(",0,6,") == -1)
	//		{
	//			Serial1.println("ATH");
	//		}
	//		Serial.println("EXITED");
	//		display.fillScreen(TFT_WHITE);
	//		display.setCursor(32, 9);
	//		if (timeOffset == 0)
	//			display.print("00:00");
	//		else
	//		{
	//			if ((int((millis() - timeOffset) / 1000) / 60) > 9)
	//				display.print(int((millis() - timeOffset) / 1000) / 60);
	//			else
	//			{
	//				display.print("0");
	//				display.print(int((millis() - timeOffset) / 1000) / 60);
	//			}
	//			display.print(":");
	//			if ((int((millis() - timeOffset) / 1000) % 60) > 9)
	//				display.print(int((millis() - timeOffset) / 1000) % 60);
	//			else
	//			{
	//				display.print("0");
	//				display.print(int((millis() - timeOffset) / 1000) % 60);
	//			}
	//		}
	//		display.drawBitmap(29, 24, call_icon, TFT_RED);
	//		display.setCursor(11, 20);
	//		display.println(number);
	//		display.fillRect(0, 51, 80, 13, TFT_RED);
	//		display.setCursor(2, 62);
	//		display.print("Call ended");
	//		Serial.println("ENDED");
	//		while (!update());
	//		delay(1000);
	//		break;
	//	}
	//	update();
	//}
}
void MAKERphone::checkSim()
{
	String input = "";

	while (input.indexOf("+CPIN:") == -1 && input.indexOf("ERROR", input.indexOf("+CPIN")) == -1) {
		Serial1.println(F("AT+CPIN?"));
		input = Serial1.readString();
		Serial.println(input);
		delay(10);
	}
	if (input.indexOf("NOT READY", input.indexOf("+CPIN:")) != -1 || (input.indexOf("ERROR") != -1 && input.indexOf("+CPIN:") == -1)
		|| input.indexOf("NOT INSERTED") != -1)
	{
		simInserted = 0;
	}
	else
	{
		simInserted = 1;
		if (input.indexOf("SIM PIN") != -1)
			enterPin();
		if(input.indexOf("SIM PUK") != -1)
			simInserted = 0;
	}

}
void MAKERphone::enterPin()
{
	char key = NO_KEY;
	String pinBuffer = "";
	String reply = "";

	while (reply.indexOf("+SPIC:") == -1)
	{
		Serial1.println("AT+SPIC");
		reply = Serial1.readString();
	}
	timesRemaining = reply.substring(reply.indexOf(" ", reply.indexOf("+SPIC:")), reply.indexOf(",", reply.indexOf(" ", reply.indexOf("+SPIC:")))).toInt();

	while (1)
	{
		display.setTextFont(1);
		display.setTextColor(TFT_WHITE);
		display.fillScreen(TFT_BLACK);
		display.setCursor(5, 5);
		display.printCenter("Enter pin:");

		display.setCursor(1, 30);
		display.printCenter(pinBuffer);
		display.setCursor(1, 63);
		display.setFreeFont(TT1);
		display.print("Press A to confirm");
		display.setCursor(5, 19);
		String temp = "Remaining attempts: ";
		temp+=timesRemaining;
		display.printCenter(temp);

		key = buttons.kpdNum.getKey();
		if (key == 'A') //clear number
			pinBuffer = "";
		else if (key == 'C' && pinBuffer != "")
			pinBuffer.remove(pinBuffer.length() - 1);
		if (key != NO_KEY && isDigit(key) && pinBuffer.length() != 4)
			pinBuffer += key;


		if (buttons.released(BTN_A))//enter PIN
		{
			reply = "";
			Serial1.print(F("AT+CPIN=\""));
			Serial1.print(pinBuffer);
			Serial1.println("\"");
			while (!Serial1.available());
			while (reply.indexOf("+CPIN: READY") == -1 && reply.indexOf("ERROR") == -1)
				reply= Serial1.readString();
			display.fillScreen(TFT_BLACK);
			display.setCursor(28, 28);
			display.setTextFont(1);
			if (reply.indexOf("+CPIN: READY") != -1)
			{
				display.printCenter("PIN OK :)");
				while (!update());
				delay(1000);
				break;
			}
			else if (reply.indexOf("ERROR") != -1)
			{
				timesRemaining--;
				pinBuffer = "";
				display.printCenter("Wrong PIN :(");
				while (!update());
				delay(2000);
			}
		}
		if (buttons.released(BTN_B) == 1) //sleeps
			sleep();
		update();
	}
}
String MAKERphone::textInput(String buffer, int16_t length = -1)
{
	int ret = 0;
	byte key = mp.buttons.kpdNum.getKey(); // Get a key press from the keypad
	if (key == 'C' && buffer != "")
	{
		if (textPointer == buffer.length())
			textPointer = textPointer - 1;
		buffer.remove(buffer.length() - 1);
	}
	else if (key == 'A') //clear number
	{
		buffer = "";
		textPointer = 0;
	}

	if(length == -1 || length > buffer.length()){
		if (key == '*') buffer += ' ';
		if (key != 'B' && key != 'D')
		{
			ret = multi_tap(key);// Feed the key press to the multi_tap function.
			if ((ret & 256) != 0) // If this is non-zero, we got a key. Handle some special keys or just print the key on screen
			{
				textPointer++;

			}
			else if (ret) // We don't have a key but the user is still cycling through characters on one key so we need to update the screen
			{
				if (textPointer == buffer.length())
					buffer += char(lowByte(ret));
				else
					buffer[buffer.length() - 1] = char(lowByte(ret));
			}
		}
	}
	return buffer;
}
int MAKERphone::multi_tap(byte key)
{
	static boolean upperCase = true;
	static byte prevKeyPress = NO_KEY, cyclicPtr = 0;
	static unsigned long prevKeyMillis = 0;
	static const char multi_tap_mapping[10][map_width] = { {'0','#','$','.','?','"','&'},{'1','+','-','*','/','\'',':'},{'A','B','C','2','!',';','<'},{'D','E','F','3','%','[','='},{'G','H','I','4','(','\\','>'},{'J','K','L','5',')',']','^'},{'M','N','O','6','@','_','`'},{'P','Q','R','S','7','{','|'},{'T','U','V','8',',','}','~'},{'W','X','Y','Z','9',' ',0} };
	if (key == RESET_MTP) // Received reset command. Flush everything and get ready for restart.
	{
		upperCase = true;
		prevKeyPress = NO_KEY;
		cyclicPtr = 0;
		return 0;
	}
	if (key != NO_KEY) // A key is pressed at this iteration.
	{
		if (key == 'C' || key == 'A')
		{
			prevKeyPress = NO_KEY;
			cyclicPtr = 0;
			return 0;
		}
		if (key == '*')
		{
			prevKeyPress = NO_KEY;
			cyclicPtr = 0;
			return (256 + (unsigned int)(' '));
		}
		if ((key > '9') || (key < '0')) // Function keys
		{
			if ((key == 1) || (key == '#')) // Up for case change
			{
				upperCase = !upperCase;
				return 0;
			}
			else // Other function keys. These keys produce characters so they need to terminate the last keypress.
			{
				if (prevKeyPress != NO_KEY)
				{
					char temp1 = multi_tap_mapping[prevKeyPress - '0'][cyclicPtr];
					if ((!upperCase) && (temp1 >= 'A') && (temp1 <= 'Z')) temp1 += 'a' - 'A';
					cyclicPtr = 0;
					prevKeyMillis = 0;
					switch (key)
					{
					case 2:
						// Call symbol list
						return 0; // Clear the buffer.
						break;
					case 3:
						prevKeyPress = '\b';
						break;
					case 5:
						prevKeyPress = '\n';
						break;
					case 6:
						prevKeyPress = NO_KEY; // Clear the buffer.
						break;
					}
					return(256 + (unsigned int)(temp1));
				}
				else
				{
					prevKeyPress = NO_KEY;
					cyclicPtr = 0;
					prevKeyMillis = 0;
					switch (key)
					{
					case 2:
						// Call symbol list
						return 0; // Clear the buffer.
						break;
					case 3:
						return (256 + (unsigned int)('\b'));
						break;
					case 4:
						return (256 + (unsigned int)(' '));
						break;
					case 5:
						return (256 + (unsigned int)('\n'));
						break;
					case 6:
						return 0; // Clear the buffer.
						break;
					}
				}
			}

		}
		if (prevKeyPress != NO_KEY)
		{
			if (prevKeyPress == key)
			{
				char temp1;
				cyclicPtr++;
				if ((multi_tap_mapping[key - '0'][cyclicPtr] == 0) || (cyclicPtr == map_width)) cyclicPtr = 0; //Cycle key
				prevKeyMillis = millis();
				temp1 = multi_tap_mapping[key - '0'][cyclicPtr];
				if ((!upperCase) && (temp1 >= 'A') && (temp1 <= 'Z')) temp1 += 'a' - 'A';
				return ((unsigned int)(temp1));
			}
			else
			{
				char temp1 = multi_tap_mapping[prevKeyPress - '0'][cyclicPtr];
				if ((!upperCase) && (temp1 >= 'A') && (temp1 <= 'Z')) temp1 += 'a' - 'A';
				prevKeyPress = key;
				cyclicPtr = 0;
				prevKeyMillis = millis();
				//Print key on cursor+1
				return(256 + (unsigned int)(temp1));
			}
		}
		else
		{
			char temp1 = multi_tap_mapping[key - '0'][cyclicPtr];
			if ((!upperCase) && (temp1 >= 'A') && (temp1 <= 'Z')) temp1 += 'a' - 'A';
			prevKeyPress = key;
			prevKeyMillis = millis();
			cyclicPtr = 0;
			return ((unsigned int)(temp1));
		}

	}
	else // No key is pressed at this iteration.
	{
		if (prevKeyPress == NO_KEY) return 0; // No key was previously pressed.
		else if (millis() - prevKeyMillis < multi_tap_threshold) // Key was pressed previously but within threshold
		{
			char temp1 = multi_tap_mapping[prevKeyPress - '0'][cyclicPtr];
			if ((!upperCase) && (temp1 >= 'A') && (temp1 <= 'Z')) temp1 += 'a' - 'A';
			return((unsigned int)(temp1));
		}
		else // Key was pressed previously and threshold has passed
		{
			char temp1 = multi_tap_mapping[prevKeyPress - '0'][cyclicPtr];
			if ((!upperCase) && (temp1 >= 'A') && (temp1 <= 'Z')) temp1 += 'a' - 'A';
			prevKeyPress = NO_KEY;
			cyclicPtr = 0;
			return(256 + (unsigned int)(temp1));
		}
	}
	return 0;
}
void MAKERphone::debugMode()
{
	dataRefreshFlag = 0;
	String percentage = "";
	String voltage = "";
	String signal = "";
	String Arsp, Grsp, reply;
	uint32_t elapsedMillis=millis();
	while (!buttons.released(BTN_B))
	{
		display.fillScreen(TFT_BLACK);
		display.setCursor(29, 2);
		display.setTextFont(1);
		display.printCenter("DEBUG MODE");
		if (millis() - elapsedMillis >= 100)
		{
			Serial1.println("AT+CBC");
			while (reply.indexOf("+CBC:") == -1 && !buttons.pressed(BTN_B))
			{
				reply = readSerial();
				buttons.update();
				Serial.println(reply);
				delay(5);
			}
			elapsedMillis = millis();
			percentage = reply.substring(reply.indexOf(",", reply.indexOf("+CBC:"))+1, reply.indexOf(",", reply.indexOf(",", reply.indexOf("+CBC:"))+1));
			voltage = reply.substring(reply.indexOf(",", reply.indexOf(",", reply.indexOf("+CBC:")) + 1)+1, reply.indexOf("\n", reply.indexOf("+CBC:")));
			batteryVoltage = voltage.toInt();
			Serial1.println("AT+CSQ");
			while (reply.indexOf("+CSQ:") == -1 && !buttons.pressed(BTN_B))
			{
				reply = readSerial();
				buttons.update();
				Serial.println(reply);
				delay(5);
			}
			signal = reply.substring(reply.indexOf(" ", reply.indexOf("+CSQ:"))+1, reply.indexOf(",", reply.indexOf(" ", reply.indexOf("+CSQ:"))));

		}
		display.setFreeFont(TT1);
		display.setCursor(30, 25);
		reply = "Battery: " + percentage + "%";
		display.printCenter(reply);
		reply = "Voltage: " + voltage + "mV";
		display.setCursor(30, 32);
		display.printCenter(reply);
		reply = "Signal: " + signal;
		display.setCursor(30, 39);
		display.printCenter(reply);

		if (voltage.toInt() > 4000)
			display.drawBitmap(30, 50, batteryCharging, TFT_WHITE);
		else if(voltage.toInt() <= 4000 && voltage.toInt() >= 3800)
			display.drawBitmap(30, 50, batteryFull, TFT_WHITE);
		else if (voltage.toInt() < 3800 && voltage.toInt() >= 3700)
			display.drawBitmap(30, 50, batteryMid, TFT_WHITE);
		else if(voltage.toInt() < 3700)
			display.drawBitmap(30, 50, batteryEmpty, TFT_WHITE);

		if (signal.toInt() <= 2)
			display.drawBitmap(40, 50, noSignalIcon, TFT_WHITE);
		else if(signal.toInt() > 3 && signal.toInt() <= 10)
			display.drawBitmap(40, 50, signalLowIcon, TFT_WHITE);
		else if (signal.toInt() > 10 && signal.toInt() <= 20)
			display.drawBitmap(40, 50, signalHighIcon, TFT_WHITE);
		else if (signal.toInt() > 20 && signal.toInt() <= 31)
			display.drawBitmap(40, 50, signalFullIcon, TFT_WHITE);
		else if(signal.toInt() == 99)
			display.drawBitmap(40, 50, signalErrorIcon, TFT_WHITE);

		/*while (!update());
		if (Serial1.available())
		{
			Grsp = mp.readSerial();;
			Serial.println(Grsp);
		}

		if (Serial.available())
		{
			Arsp = Serial.readString();
			Serial1.println(Arsp);
		}*/
		reply = "";
		update();
	}
}

//Messages app
void MAKERphone::messagesApp() {
	
	dataRefreshFlag = 0;
	//sim800.println("AT+CMGL=\"REC READ\"");
	//sim800.flush();
	/*while (sim800.available())
	input += (char)sim800.read();*/
	while(!simReady)
		update();
	input = readAllSms();
	while (input == "")
	{
		input = readSerial();
	}

	if (input == "ERROR")
	{
		display.fillScreen(TFT_BLACK);
		if(resolutionMode)
		{
			display.setCursor(0, display.height()/2);
			display.setFreeFont(TT1);
		}
		else
		{
			display.setCursor(0, display.height()/2 - 16);
			display.setTextFont(2);
		}
		display.setTextColor(TFT_WHITE);
		display.printCenter("Error loading SMS!");
		while (!update());
		while (!buttons.released(BTN_A) && !buttons.released(BTN_B))
			update();
	}
	else if (input == "OK")
	{
		display.setFreeFont(TT1);
		display.fillScreen(TFT_BLACK);
		if(resolutionMode)
		{
			display.setCursor(0, display.height()/2);
			display.setFreeFont(TT1);
		}
		else
		{
			display.setCursor(0, display.height()/2 - 16);
			display.setTextFont(2);
		}
		display.setTextColor(TFT_WHITE);
		display.printCenter("No messages :(");
		while (!update());
		while (!buttons.released(BTN_A) && !buttons.released(BTN_B))
			update();
	}
	else
	{
		Serial.println(input);
		delay(5);


		/////////////////////////////////////////////////////
		//parsing the raw input data for contact number,
		//date and text content
		////////////////////////////////////////////////////
		uint16_t numberOfTexts = countSubstring(input, "+CMGL:");
		for (uint8_t i = 0; i < numberOfTexts; i++)
		{
			start = input.indexOf("\n", input.indexOf("CMGL:", end));
			end = input.indexOf("\n", start + 1);
			smsContent[i] = input.substring(start + 1, end);
		}


		end = 0;
		for (uint8_t i = 0; i < numberOfTexts; i++)
		{
			start = input.indexOf("D\",\"", input.indexOf("CMGL:", end));
			end = input.indexOf("\"", start + 4);
			phoneNumber[i] = input.substring(start + 4, end);
		}
		//////////////////////////////////////////
		//Parsing for date and time of sending
		////////////////////////////
		end = 0;
		for (uint8_t i = 0; i < numberOfTexts; i++)
		{
			start = input.indexOf("\"\",\"", input.indexOf("CMGL:", end));
			end = input.indexOf("\"", start + 4);
			tempDate[i] = input.substring(start + 4, end);
		}

		int8_t index = -8;
		for (uint8_t i = 0; i < numberOfTexts; i++)
		{

			char c1, c2; //buffer for saving date and time numerals in form of characters
			c1 = tempDate[i].charAt(index + 8);
			c2 = tempDate[i].charAt(index + 9);
			smsYear[i] = 2000 + ((c1 - '0') * 10) + (c2 - '0');

			c1 = tempDate[i].charAt(index + 11);
			c2 = tempDate[i].charAt(index + 12);
			smsMonth[i] = ((c1 - '0') * 10) + (c2 - '0');

			c1 = tempDate[i].charAt(index + 14);
			c2 = tempDate[i].charAt(index + 15);
			smsDay[i] = ((c1 - '0') * 10) + (c2 - '0');

			c1 = tempDate[i].charAt(index + 17);
			c2 = tempDate[i].charAt(index + 18);
			smsHour[i] = ((c1 - '0') * 10) + (c2 - '0');

			c1 = tempDate[i].charAt(index + 20);
			c2 = tempDate[i].charAt(index + 21);
			smsMinute[i] = ((c1 - '0') * 10) + (c2 - '0');

			c1 = tempDate[i].charAt(index + 23);
			c2 = tempDate[i].charAt(index + 24);
			smsSecond[i] = ((c1 - '0') * 10) + (c2 - '0');
		}

		while (1)
		{
			int16_t menuChoice = smsMenu("Messages", phoneNumber, tempDate, smsContent, numberOfTexts);
			Serial.println(menuChoice);
			if (menuChoice == -1)
				break;
			else if (menuChoice == 0)
				composeSMS();
			else
				viewSms(smsContent[menuChoice-1], phoneNumber[menuChoice-1], tempDate[menuChoice-1]);
		}
	}
}
uint16_t MAKERphone::countSubstring(String string, String substring) {
	if (substring.length() == 0) return 0;
	int count = 0;
	for (size_t offset = string.indexOf(substring); offset != -1;
		offset = string.indexOf(substring, offset + substring.length()))
	{
		count++;
	}
	return count;
}
String MAKERphone::readSms(uint8_t index) {
	String buffer;
	Serial1.print(F("AT+CMGF=1\r"));
	if ((readSerial().indexOf("ER")) == -1) {
		Serial1.print(F("AT+CMGR="));
		Serial1.print(index);
		Serial1.print("\r");
		buffer = readSerial();
		if (buffer.indexOf("CMGR:") != -1) {
			return buffer;
		}
		else return "";
	}
	else
		return "";
}
String MAKERphone::readAllSms() {
	
	Serial1.print(F("AT+CMGL=\"ALL\"\r"));
	buffer = readSerial();
	delay(10);
	if (buffer.indexOf("CMGL:") != -1) {
		return buffer;
	}
	else if (buffer.indexOf("ERROR", buffer.indexOf("AT+CMGL")) != -1)
		return "ERROR";
	else if (buffer.indexOf("CMGL:") == -1 && buffer.indexOf("OK", buffer.indexOf("AT+CMGL")) != -1)
		return "OK";
	else
		return "";


}
void MAKERphone::viewSms(String content, String contact, String date) {
	y = 14;  //Beggining point
	unsigned long buttonHeld = millis();
	unsigned long elapsedMillis = millis();
	bool blinkState = 1;
	while (1)
	{
		display.fillScreen(TFT_DARKGREY);
		display.setTextWrap(1);
		//while (kpd.pin_read(3))
		//{

		display.setCursor(1, y);
		display.print(content);
		if (buttons.kpd.pin_read(JOYSTICK_B) == 0) { //BUTTON DOWN
			Serial.println(display.cursor_y);
			if (display.cursor_y >= 128)
			{
				buttonHeld = millis();

				if (buttons.kpd.pin_read(JOYSTICK_B) == 1)
				{
					y -= 4;
					break;
				}

				while (buttons.kpd.pin_read(JOYSTICK_B) == 0)
				{
					if (millis() - buttonHeld > 100) {
						y -= 4;
						break;
					}
				}
			}
		}

		if (buttons.kpd.pin_read(JOYSTICK_D) == 0) { //BUTTON UP
			if (y < 14)
			{
				buttonHeld = millis();

				if (buttons.kpd.pin_read(JOYSTICK_D) == 1)
				{
					y += 4;
					break;
				}

				while (buttons.kpd.pin_read(JOYSTICK_D) == 0)
				{
					if (millis() - buttonHeld > 100) {
						y += 4;
						break;
					}
				}
			}

		}

		if (buttons.released(BTN_B)) //BUTTON BACK
		{
			while (!update());
			break;
		}
		//}
		// last draw the top entry thing

		if (millis() - elapsedMillis >= 1000) {
			elapsedMillis = millis();
			blinkState = !blinkState;
		}

		if (blinkState == 1)
		{
			display.setTextColor(TFT_WHITE);
			if(resolutionMode)
			{
				display.fillRect(0, 0, display.width(), 7, TFT_DARKGREY);
				display.setFreeFont(TT1);
				display.setCursor(1, 6);
				display.drawFastHLine(0, 7, LCDWIDTH, TFT_BLACK);
			}
			else
			{
				display.fillRect(0, 0, display.width(), 14, TFT_DARKGREY);
				display.setTextFont(2);
				display.setCursor(2,-1);
				display.drawFastHLine(0, 14, display.width(), TFT_WHITE);

			}
			display.print("From: ");
			display.print(contact);
		}
		else
		{
			display.setTextColor(TFT_WHITE);
			if(resolutionMode)
			{
				display.fillRect(0, 0, display.width(), 7, TFT_DARKGREY);
				display.setFreeFont(TT1);
				display.setCursor(1, 6);
				display.drawFastHLine(0, 7, LCDWIDTH, TFT_BLACK);
			}
			else
			{
				display.fillRect(0, 0, display.width(), 14, TFT_DARKGREY);
				display.setTextFont(2);
				display.setCursor(2,-1);
				display.drawFastHLine(0, 14, display.width(), TFT_WHITE);
			}
			display.print(date);
		}

		update();
	}
}
void MAKERphone::smsMenuDrawBox(String contact, String date, String content, uint8_t i, int32_t y) {
	uint8_t scale;
	uint8_t offset;
	uint8_t boxHeight;
	uint8_t composeHeight;
	display.setTextSize(1);
	if(resolutionMode)
	{
		scale = 1;
		offset = menuYOffset;
		composeHeight=12;
		boxHeight = 21;
		display.setFreeFont(TT1);
	}
	else
	{
		scale = 2;
		offset = 19;
		composeHeight=21;
		boxHeight = 30;
		display.setTextFont(2);
	}
	y += (i-1) * (boxHeight-1) + composeHeight + offset;
	if (y < 0 || y > display.height()) {
		return;
	}
	if(resolutionMode)
	{
		display.fillRect(scale, y + 1, display.width() - 2, boxHeight-2, TFT_DARKGREY);
		display.setTextColor(TFT_WHITE);
		display.setCursor(2*scale, y + 2);
		display.drawString(contact, 3, y + 2);
		display.drawString(date, 3, y + 8);
		display.drawString(content, 3, y + 14);
	}
	else
	{
		String monthsList[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
		display.fillRect(1, y + 1, display.width() - 2, boxHeight-2, TFT_DARKGREY);
		display.setTextColor(TFT_WHITE);
		display.setCursor(4, y + 2);
		display.drawString(contact, 3, y-1);

		//display.drawString(date, 3, y + 10);
		display.drawString(content, 3, y + 13);
		display.setTextFont(1);
		display.setCursor(124, y+3);
		display.print(monthsList[smsMonth[i-1]-1]);
		display.print(" ");
		display.print(smsDay[i - 1]);
	}

}
void MAKERphone::smsMenuDrawCursor(uint8_t i, int32_t y) {
	uint8_t offset;
	uint8_t boxHeight;
	uint8_t composeHeight;
	if(resolutionMode)
	{
		offset = menuYOffset;
		composeHeight=12;
		boxHeight = 21;
		display.setTextSize(1);
	}
	else
	{
		offset = 19;
		composeHeight=21;
		boxHeight = 30;
		display.setTextSize(2);
	}
	if (millis() % 500 <= 250) {
		return;
	}
	y += (i-1) * (boxHeight-1) + composeHeight + offset;
	display.drawRect(0, y, display.width(), boxHeight, TFT_RED);
}
int16_t MAKERphone::smsMenu(const char* title, String* contact, String *date, String *content, uint8_t length) {
	uint8_t cursor = 0;
	int32_t cameraY = 0;
	int32_t cameraY_actual = 0;
	uint8_t scale;
	uint8_t offset;
	uint8_t boxHeight;
	if(resolutionMode)
	{
		scale = 1;
		offset = menuYOffset;
		boxHeight = 21;
	}
	else
	{
		scale = 2;
		offset = 19;
		boxHeight = 30;
	}
	while (1) {
		while (!update());
		display.fillScreen(TFT_BLACK);
		display.setCursor(0, 0);
		cameraY_actual = (cameraY_actual + cameraY) / 2;
		if (cameraY_actual - cameraY == 1) {
			cameraY_actual = cameraY;
		}

		for (uint8_t i = 0; i < length+1; i++) {
			if (i == 0)
				smsMenuComposeBox(i, cameraY_actual);
			else
				smsMenuDrawBox(contact[i-1], date[i-1], content[i-1], i, cameraY_actual);
		}
		if (cursor == 0)
			smsMenuComposeBoxCursor(cursor, cameraY_actual);
		else
			smsMenuDrawCursor(cursor, cameraY_actual);

		// last draw the top entry thing

		if(resolutionMode)
		{
			display.fillRect(0, 0, display.width(), 6, TFT_DARKGREY);
			display.setFreeFont(TT1);
			display.setCursor(0,5);
			display.drawFastHLine(0, 6, BUF2WIDTH, TFT_WHITE);
		}
		else
		{
			display.fillRect(0, 0, display.width(), 14, TFT_DARKGREY);
			display.setTextFont(2);
			display.setCursor(1,-2);
			display.drawFastHLine(0, 14, BUF2WIDTH, TFT_WHITE);
		}
		display.setTextSize(1);
		display.setTextColor(TFT_WHITE);
		display.print(title);

		if (buttons.released(BTN_A)) {   //BUTTON CONFIRM

			while (!update());// Exit when pressed
			break;
		}

		if (buttons.released(JOYSTICK_D)) {  //BUTTON UP
			if (cursor == 0) {
				cursor = length - 1;
				if (length > 2) {
					cameraY = -(cursor - 1) * (boxHeight-1);
				}
			}
			else {
				cursor--;
				if (cursor > 0 && (cursor * (boxHeight-1) + cameraY + offset) < 14*scale) {
					cameraY += (boxHeight-1);
				}
			}
		}

		if (buttons.released(JOYSTICK_B)) { //BUTTON DOWN
			cursor++;
			if (cursor > 0)
			{
				if (((cursor - 1) * (boxHeight-1) + composeBoxHeight + cameraY + offset) > 40*scale) {
					cameraY -= boxHeight-1;
				}
			}
			else
			{
				if ((cursor * (boxHeight-1) + cameraY + offset) > 40*scale) {
					cameraY -= boxHeight-1;
				}
			}
			if (cursor > length) {
				cursor = 0;
				cameraY = 0;

			}

		}

		if (buttons.released(BTN_B) == 1) //BUTTON BACK
		{
			return -1;
		}
	}
	return cursor;

}
void MAKERphone::smsMenuComposeBoxCursor(uint8_t i, int32_t y) {
	uint8_t offset;
	uint8_t boxHeight;
	if(resolutionMode)
	{
		offset = menuYOffset;
		boxHeight=composeBoxHeight;
	}
	else
	{
		offset = 19;
		boxHeight=21;
	}
	if (millis() % 500 <= 250) {
		return;
	}
	y += offset;
	display.drawRect(0, y, display.width(), boxHeight+1, TFT_RED);
}
void MAKERphone::smsMenuComposeBox(uint8_t i, int32_t y) {
	uint8_t scale;
	uint8_t offset;
	uint8_t boxHeight;
	display.setTextSize(1);
	if(resolutionMode)
	{
		scale = 1;
		offset = menuYOffset;
		boxHeight=12;
		display.setTextFont(1);

	}
	else
	{
		scale = 2;
		offset = 19;
		boxHeight=21;
		display.setTextFont(2);
	}
	y += offset;
	if (y < 0 || y > display.height()) {
		return;
	}
	display.fillRect(1, y + 1, display.width() - 2, boxHeight-1, TFT_DARKGREY);
	display.drawBitmap(2*scale, y + 2, composeIcon, TFT_WHITE, scale);
	display.setTextColor(TFT_WHITE);
	display.drawString("New SMS", 20*scale, y+3);
	display.setFreeFont(TT1);

}
void MAKERphone::composeSMS()
{
	textPointer = 0;
	if(resolutionMode)
		y = 10;  //Beggining point
	else
		y = 16;
	String content = "";
	String contact = "";
	String prevContent = "";
	char key = NO_KEY;
	bool cursor = 0; //editing contacts or text content
	unsigned long elapsedMillis = millis();
	bool blinkState = 1;
	uint8_t scale;

	while (1)
	{


		display.fillScreen(TFT_DARKGREY);

		display.setTextColor(TFT_WHITE);
		if(resolutionMode)
		{
			scale = 1;
			display.fillRect(0, 0, display.width(), 7, TFT_DARKGREY);
			display.setFreeFont(TT1);
			display.setCursor(1, 6);
			display.drawFastHLine(0, 7, LCDWIDTH, TFT_BLACK);
		}
		else
		{
			scale = 2;
			display.fillRect(0, 0, display.width(), 14, TFT_DARKGREY);
			display.setTextFont(2);
			display.setCursor(2,-1);
			display.drawFastHLine(0, 14, display.width(), TFT_WHITE);

		}
		display.print("To: ");

		if (millis() - elapsedMillis >= multi_tap_threshold) //cursor blinking routine
		{
			elapsedMillis = millis();
			blinkState = !blinkState;
		}
		if (cursor == 0) //inputting the contact number
		{
			key = buttons.kpdNum.getKey();
			if (key == 'A') //clear number
				contact = "";
			else if (key == 'C')
				contact.remove(contact.length() - 1);
			if (key != NO_KEY && isdigit(key))
				contact += key;
			display.setTextWrap(1);
			display.setCursor(1*scale, y);
			if(resolutionMode)
				display.setTextFont(1);
			if (content == "")
			{
				display.setTextColor(TFT_LIGHTGREY);
				display.print(F("Compose..."));
				display.setTextColor(TFT_WHITE);
			}
			else
				display.print(content);
			if(resolutionMode)
			{
				display.setFreeFont(TT1);
				display.setCursor(13, 6);
			}
			else
				display.setCursor(27, -1);

			display.print(contact);
			if (blinkState == 1)
			{
				if(resolutionMode)
					display.drawFastVLine(display.getCursorX(), display.getCursorY()-5, 5, TFT_WHITE);
				else
					display.drawFastVLine(display.getCursorX(), display.getCursorY()+3, 10, TFT_WHITE);
			}
		}
		else
		{
			display.setTextColor(TFT_WHITE);
			if(resolutionMode)
				display.setCursor(1, 6);
			else
				display.setCursor(2, -1);
			display.print("To: ");
			display.print(contact);
			prevContent = content;
			content = textInput(content);
			if (prevContent != content)
			{
				blinkState = 1;
				elapsedMillis = millis();
			}
			display.setTextWrap(1);
			display.setCursor(1*scale, y);
			if(resolutionMode)
				display.setTextFont(1);
			display.print(content);
			if(resolutionMode)
				display.setFreeFont(TT1);
			if(blinkState == 1)
			{
				if(resolutionMode)
					display.drawFastVLine(display.getCursorX(), display.getCursorY(), 7, TFT_WHITE);
				else
					display.drawFastVLine(display.getCursorX(), display.getCursorY()+3, 10, TFT_WHITE);
			}

			/*if (blinkState == 1)
			{
				display.fillRect(0, 0, display.width(), 7, TFT_DARKGREY);
				display.setTextColor(TFT_WHITE);
				display.setCursor(1, 6);
				display.print("From: ");
				display.print(contact);
				display.drawFastHLine(0, 7, LCDWIDTH, TFT_BLACK);
			}
			else
			{
				display.setTextColor(TFT_WHITE);
				display.setCursor(1, 6);
				display.drawFastHLine(0, 7, LCDWIDTH, TFT_BLACK);
			}*/
		}
		if (buttons.kpd.pin_read(JOYSTICK_D) == 0 && cursor == 1) { //BUTTON UP
			/*Serial.println(display.cursor_y);
			if (display.cursor_y >= 64)
			{
				buttonHeld = millis();

				if (buttons.kpd.pin_read(JOYSTICK_D) == 1)
				{
					y -= 2;
					break;
				}

				while (buttons.kpd.pin_read(JOYSTICK_D) == 0)
				{
					if (millis() - buttonHeld > 100) {
						y -= 2;
						break;
					}
				}
			}*/
			cursor = 0;
		}

		if (buttons.kpd.pin_read(JOYSTICK_B) == 0 && cursor == 0) { //BUTTON DOWN
			/*if (y < 14)
			{


				buttonHeld = millis();

				if (buttons.kpd.pin_read(JOYSTICK_B) == 1)
				{
					y += 2;
					break;
				}

				while (buttons.kpd.pin_read(JOYSTICK_B) == 0)
				{
					if (millis() - buttonHeld > 100) {
						y += 2;
						break;
					}
				}
			}*/
			cursor = 1;

		}

		if (buttons.released(BTN_B)) //BUTTON BACK
		{
			while (!update());
			break;
		}
		if (buttons.released(BTN_A) && contact != "" && content != "") // SEND SMS
		{
			display.fillScreen(TFT_BLACK);
			if(resolutionMode)
			{
				display.setCursor(0, display.height()/2);
				display.setFreeFont(TT1);
			}
			else
			{
				display.setCursor(0, display.height()/2 - 16);
				display.setTextFont(2);
			}
			display.printCenter("Sending text...");
			while (!update());
			Serial1.print("AT+CMGS=\"");
			Serial1.print(contact);
			Serial1.println("\"");
			while (!Serial1.available());
			Serial.println(Serial1.readString());
			delay(5);
			Serial1.print(content);
			while (!Serial1.available());
			Serial.println(Serial1.readString());
			delay(5);
			Serial1.println((char)26);
			while (Serial1.readString().indexOf("OK") != -1);
			display.fillScreen(TFT_BLACK);
			display.printCenter("Text sent!");
			while (!update());
			delay(1000);
			break;
		}




		update();
	}
}

//Contacts app

uint8_t MAKERphone::deleteContact(String contact, String number, String id)
{
	unsigned long elapsedMillis = millis();
	bool blinkState = 1;
	while (1)
	{
		display.fillScreen(TFT_BLACK);
		if(resolutionMode)
		{
			display.fillRect(0, 0, display.width(), 6, TFT_DARKGREY);
			display.setFreeFont(TT1);
			display.setCursor(0,5);
			display.drawFastHLine(0, 6, BUF2WIDTH, TFT_WHITE);
		}
		else
		{
			display.fillRect(0, 0, display.width(), 14, TFT_DARKGREY);
			display.setTextFont(2);
			display.setCursor(0,-2);
			display.drawFastHLine(0, 14, BUF2WIDTH, TFT_WHITE);
		}
		display.setTextColor(TFT_WHITE);
		display.print("Delete contact");

		if (millis() - elapsedMillis >= multi_tap_threshold) {
		elapsedMillis = millis();
		blinkState = !blinkState;
		}

		display.setTextColor(TFT_WHITE);
		display.setCursor(4, 17);
		display.print("Are you sure?");
		display.setCursor(4, 33);
		display.print(contact);
		display.setCursor(4, 49);
		display.print(number);

		if (blinkState){
			display.drawRect(display.width() / 2 - 29, 102, 30*2, 9*2, TFT_RED);
			display.setTextColor(TFT_RED);
			display.setCursor(28*2, 103);
			display.printCenter("DELETE");
		}
		else {
			display.fillRect(display.width() / 2 - 29, 102, 30*2, 9*2, TFT_RED);
			display.setTextColor(TFT_WHITE);
			display.setCursor(28*2, 103);
			display.print("DELETE");
		}



		if (buttons.released(BTN_B)) //BUTTON BACK
		{
			Serial.println("Go back");
			while (!update());
			break;
		}
		if (buttons.released(BTN_A)) // DELETE
		{
			Serial.println("DElete");
			display.fillScreen(TFT_BLACK);
			display.setTextFont(2);
			display.setCursor(34, display.height()/2 -16);
			display.printCenter("Deleting contact...");
			while (!update());

			Serial1.print("AT+CPBW=");
			Serial1.println(id);
			Serial.print("AT+CPBW=");
			Serial.println(id);

			while (Serial1.readString().indexOf("OK") != -1);
			display.fillScreen(TFT_BLACK);
			display.printCenter("Contact deleted!");
			Serial.println("Contact deleted");
			while (!update());
			delay(1000);
			return 1;
		}

		update();
	}
	return 0;
}

uint8_t MAKERphone::newContact()
{
	textPointer = 0;
	y = 20;  //Beggining point
	String content = "";
	String contact = "";
	String prevContent = "";
	char key = NO_KEY;
	bool cursor = 0; //editing contacts or text content
	unsigned long elapsedMillis = millis();
	bool blinkState = 1;
	while (1)
	{
		display.fillScreen(TFT_BLACK);
		if(resolutionMode)
		{
			display.fillRect(0, 0, display.width(), 6, TFT_DARKGREY);
			display.setFreeFont(TT1);
			display.setCursor(0,5);
			display.drawFastHLine(0, 6, BUF2WIDTH, TFT_WHITE);
		}
		else
		{
			display.fillRect(0, 0, display.width(), 14, TFT_DARKGREY);
			display.setTextFont(2);
			display.setCursor(0,-2);
			display.drawFastHLine(0, 14, BUF2WIDTH, TFT_WHITE);
		}
		display.setTextColor(TFT_WHITE);
		display.print("Contacts");
		if (millis() - elapsedMillis >= multi_tap_threshold) //cursor blinking routine
		{
		elapsedMillis = millis();
		blinkState = !blinkState;
		}
		if (cursor == 0) //inputting the contact number
		{
			key = buttons.kpdNum.getKey();
			if (key == 'A') //clear number
				contact = "";
			else if (key == 'C')
				contact.remove(contact.length() - 1);
			if (key != NO_KEY && isdigit(key) && contact.length() < 14)
				contact += key;
			display.setTextWrap(1);
			display.setCursor(4, 20);
			display.setTextFont(2);
			if (content == "")
			{
				display.setTextColor(TFT_LIGHTGREY);
				display.print(F("Name"));
				display.setTextColor(TFT_WHITE);
			}
			else
				display.print(content);
			display.setTextFont(2);
			display.setCursor(4, 38);
			display.print("Num: ");
			display.print(contact);
			if (blinkState == 1)
				display.drawFastVLine(display.getCursorX() + 1, display.getCursorY() + 3, 11, TFT_WHITE);
		}
		else //inputting contact name
		{
			display.setTextColor(TFT_WHITE);
			display.setCursor(2*2, 38);
			display.print("Num: ");
			if (contact == "")
			{
				display.setTextColor(TFT_LIGHTGREY);
				display.print(F("xxxxxxxx"));
				display.setTextColor(TFT_WHITE);
			}
			else
				display.print(contact);
			prevContent = content;
			content = textInput(content, 12);
			if (prevContent != content)
			{
				blinkState = 1;
				elapsedMillis = millis();
			}
			display.setTextColor(TFT_LIGHTGREY);
			display.setTextWrap(1);
			display.setCursor(2*2, 10*2);
			display.print(content);
			display.setTextColor(TFT_WHITE);
			if(blinkState == 1)
				display.drawFastVLine(display.getCursorX() + 1, display.getCursorY() + 3, 11, TFT_WHITE);
		}

		display.fillRect(display.width() / 2 - 29, 102, 30*2, 9*2, TFT_GREENYELLOW);
		display.setTextColor(TFT_WHITE);
		display.setCursor(31*2, 103);
		display.printCenter("SAVE");

		if (buttons.kpd.pin_read(JOYSTICK_B) == 0 && cursor == 1) { //BUTTON UP
		cursor = 0;
		}

		if (buttons.kpd.pin_read(JOYSTICK_D) == 0 && cursor == 0) { //BUTTON DOWN
		cursor = 1;
		}

		if (buttons.released(BTN_B)) //BUTTON BACK
		{
		while (!update());
		break;
		}
		if (buttons.released(BTN_A)) // SAVE CONTACT
		{
			if(contact != "" && content != "")
			{
				// international numbers ?
				// AT+CPBW=,”6187759088″,129,”Adam”

				display.fillScreen(TFT_BLACK);
				display.setCursor(34, display.height()/2 -16);
				display.printCenter("Inserting contact");
				while (!update());

				Serial1.print("AT+CPBW=,\"");
				Serial1.print(contact);
				Serial1.print("\",129,\"");
				Serial1.print(content);
				Serial1.println("\"");

				while (Serial1.readString().indexOf("OK") != -1);
				display.fillScreen(TFT_BLACK);
				display.setCursor(34, display.height()/2 -16);
				display.printCenter("Contact inserted");
				while (!update());
				delay(1000);
				return 1;
			}
		}
		update();
	}
	return 0;

}


void MAKERphone::contactsMenuDrawBox(String contact, String number, uint8_t i, int32_t y) {
	uint8_t offset;
	uint8_t boxHeight;
	if(resolutionMode)
	{
		offset = menuYOffset;
		boxHeight = 14;
	}
	else
	{
		offset = 19;
		boxHeight = 28;
	}
	y += i * boxHeight + offset;
	if (y < 0 || y > display.height()) {
		return;
	}
	if(resolutionMode)
	{
		display.setTextSize(1);
		display.setFreeFont(TT1);
		display.fillRect(1, y + 1, display.width() - 2, 13, TFT_DARKGREY);
		display.setTextColor(TFT_WHITE);
		display.setCursor(2, y + 2);
		display.drawString(contact, 3, y + 2);
		display.drawString(number, 3, y + 8);
	}
	else
	{
		display.setTextSize(1);
		display.setTextFont(2);
		display.fillRect(1, y + 1, display.width() - 2, boxHeight-1, TFT_DARKGREY);
		display.setTextColor(TFT_WHITE);
		display.setCursor(2, y + 2);
		display.drawString(contact, 4, y);
		display.drawString(number, 4, y + 12);
	}

}
void MAKERphone::contactsMenuDrawCursor(uint8_t i, int32_t y) {
	uint8_t offset;
	uint8_t boxHeight;
	if(resolutionMode)
	{
		offset = menuYOffset;
		boxHeight = 14;
	}
	else
	{
		offset = 19;
		boxHeight = 28;
	}
	if (millis() % 500 <= 250) {
		return;
	}
	y += i * boxHeight + offset;
	display.drawRect(0, y, display.width(), boxHeight + 1, TFT_RED);
}
void MAKERphone::contactsMenuNewBoxCursor(uint8_t i, int32_t y) {
	uint8_t offset;
	uint8_t boxHeight;
	if(resolutionMode)
	{
		offset = menuYOffset;
		boxHeight = 14;
	}
	else
	{
		offset = 19;
		boxHeight = 28;
	}
	if (millis() % 500 <= 250) {
		return;
	}
	y += offset + 1;
	display.drawRect(0, y, display.width(), boxHeight, TFT_RED);
}
void MAKERphone::contactsMenuNewBox(uint8_t i, int32_t y) {
	uint8_t offset;
	uint8_t boxHeight;
	if(resolutionMode)
	{
		offset = menuYOffset;
		boxHeight = 14;
	}
	else
	{
		offset = 19;
		boxHeight = 28;
	}
	y += offset + 1;
	if (y < 0 || y > display.height()) {
		return;
	}
	if(resolutionMode)
	{
		display.fillRect(1, y + 1, display.width() - 2, boxHeight - 2, TFT_DARKGREY);
		display.drawBitmap(0, y + 2, newContactIcon, TFT_WHITE);
		display.setTextColor(TFT_WHITE);
		display.setCursor(12, y + 3);
		display.setTextFont(1);
		display.print("New contact");
		display.setFreeFont(TT1);
	}
	else
	{
		display.setTextSize(1);
		display.fillRect(1, y + 1, display.width() - 2, boxHeight - 2, TFT_DARKGREY);
		display.drawBitmap(2, y + 4, newContactIcon, TFT_WHITE, 2);
		display.setTextColor(TFT_WHITE);
		display.setCursor(32, y + 6);
		display.setTextFont(2);
		display.print("New contact");
	}

}
int MAKERphone::contactsMenu(const char* title, String* contact, String *number, uint8_t length) {
	Serial.println("contactsMenu()");
	uint8_t cursor = 0;
	int32_t cameraY = 0;
	int32_t cameraY_actual = 0;
	uint8_t offset;
	uint8_t boxHeight;
	if(resolutionMode)
	{
		offset = menuYOffset;
		boxHeight = 14;
	}
	else
	{
		offset = 19;
		boxHeight = 28;
	}
	while (1) {
		while (!update());
		display.fillScreen(TFT_BLACK);
		display.setCursor(0, 0);
		cameraY_actual = (cameraY_actual + cameraY) / 2;
		if (cameraY_actual - cameraY == 1) {
		cameraY_actual = cameraY;
		}

		for (uint8_t i = 0; i < length + 1; i++) {
		if(i == 0){
			contactsMenuNewBox(i, cameraY_actual);
		} else {
			contactsMenuDrawBox(contact[i-1], number[i-1], i, cameraY_actual);
		}
		}
		if(cursor == 0){
		contactsMenuNewBoxCursor(cursor, cameraY_actual);
		} else {
		contactsMenuDrawCursor(cursor, cameraY_actual);
		}

		// last draw the top entry thing
		if(resolutionMode)
		{
			display.fillRect(0, 0, display.width(), 6, TFT_DARKGREY);
			display.setFreeFont(TT1);
			display.setCursor(0,5);
			display.drawFastHLine(0, 6, BUF2WIDTH, TFT_WHITE);
		}
		else
		{
			display.fillRect(0, 0, display.width(), 14, TFT_DARKGREY);
			display.setTextFont(2);
			display.setCursor(0,-2);
			display.drawFastHLine(0, 14, BUF2WIDTH, TFT_WHITE);
		}
		display.setTextSize(1);
		display.setTextColor(TFT_WHITE);
		display.print(title);

		if (buttons.kpd.pin_read(BTN_A) == 0) {   //BUTTON CONFIRM
		while (buttons.kpd.pin_read(BTN_A) == 0);// Exit when pressed
		break;
		}
		if (buttons.kpd.pin_read(JOYSTICK_A) == 0 && cursor != 0) {
		while (buttons.kpd.pin_read(JOYSTICK_A) == 0); // Delete
		return -1000 + cursor;
		}
		if (buttons.kpd.pin_read(JOYSTICK_C) == 0 && cursor != 0) {
		while (buttons.kpd.pin_read(JOYSTICK_C) == 0); // Edit contact
		return -3000 + cursor;
		}

		if (buttons.kpd.pin_read(JOYSTICK_D) == 0) {  //BUTTON UP
		while (buttons.kpd.pin_read(JOYSTICK_D) == 0);
		if (cursor == 0) {
			cursor = length;
			if (length > 2) {
			cameraY = -(cursor - 2) * (boxHeight+1);
			}
		}
		else {
			cursor--;
			if (cursor > 0 && (cursor * (boxHeight+1) + cameraY + offset) < (boxHeight+1)) {
			cameraY += (boxHeight+1);
			}
		}
		}

		if (buttons.kpd.pin_read(JOYSTICK_B) == 0) { //BUTTON DOWN
		while (buttons.kpd.pin_read(JOYSTICK_B) == 0);

		cursor++;
		if ((cursor * (boxHeight+1) + cameraY + offset) > 48) {
			cameraY -= (boxHeight+1);
		}
		if (cursor >= length + 1) {
			cursor = 0;
			cameraY = 0;

		}

		}
		if (buttons.released(BTN_B) == 1) //BUTTON BACK
		{
		while (!update());
		return -2;
		}
	}
	return cursor;
}
void MAKERphone::contactsApp() {
	delay(5);
	Serial.println("Loaded ?");
	int change = 0;
	String input = readAllContacts();
	int count_try = 0;
	while (input == "") {
		if(count_try > 0) delay(1000);
		if(count_try > 4) return;
		Serial.println("try again");
		input = readAllContacts();
		count_try++;
	}
	if (input.indexOf("CPBR:") == -1)
	{
		display.fillScreen(TFT_BLACK);
		if(resolutionMode)
		{
			display.setCursor(0, display.height()/2);
			display.setFreeFont(TT1);
		}
		else
		{
			display.setCursor(0, display.height()/2 - 16);
			display.setTextFont(2);
		}
		display.printCenter("No contacts  :(");
		while (buttons.released(BTN_B) == 0)//BUTTON BACK
		while (!update());
		while (!update());
	}
	else
	{
		uint8_t contactNumber = countSubstring(input, "CPBR:");
		Serial.println(contactNumber);

		/////////////////////////////////
		//Variables for contact parsing
		////////////////////////////////
		String phoneNumber[contactNumber];
		String contactName[contactNumber];
		String contact_id[contactNumber];
		uint16_t start;
		uint16_t end = 0;
		uint16_t foo = 0;
		uint16_t bar = 0;
		/////////////////////////////////////////////////////
		//parsing the raw data input for contact number,
		//date and text content
		////////////////////////////////////////////////////
		Serial.println(input);
		for (uint8_t i = 0; i < contactNumber; i++)
		{
			foo = input.indexOf(" ", input.indexOf("CPBR:", end));
			bar = input.indexOf("\"", input.indexOf("CPBR:", end));
			contact_id[i] = input.substring(foo+1, bar-1);

			start = input.indexOf("\"", input.indexOf("CPBR:", end));
			end = input.indexOf("\"", start + 1);
			phoneNumber[i] = input.substring(start + 1, end);

			start = input.indexOf("\"", end + 1);
			end = input.indexOf("\"", start + 1);
			contactName[i] = input.substring(start + 1, end);
		}

		while(1){
			int menuChoice = -1;
			if(change == 1 || change == -10){
				if(change == 1)
				{
					display.fillScreen(TFT_BLACK);
					display.setTextFont(2);
					display.setCursor(34, display.height()/2 - 16);
					display.printCenter("Reloading data...");
					while (!update());

					delay(1000);

					input = "";
					int count_try = 0;
					bool flag = 0;
					while (input == "") {
						if(count_try > 4) { flag = 1; break; }
						delay(1000);
						Serial.println("try again");
						input = readAllContacts();
						count_try++;
					}

					if (input.indexOf("CPBR:") == -1 || flag) {
						display.fillScreen(TFT_BLACK);
						display.setCursor(16, 35);
						display.setFreeFont(TT1);
						display.print("No contacts  :(");
						while (buttons.released(BTN_B) == 0) //BUTTON BACK
						while (!update());
						while (!update());
						break;
					}
				}

				uint8_t contactNumber = countSubstring(input, "CPBR:");
				Serial.println(contactNumber);

				/////////////////////////////////
				//Variables for contact parsing
				////////////////////////////////
				// String phoneNumber[contactNumber];
				// String contactName[contactNumber];
				// String contact_id[contactNumber];
				uint16_t start;
				uint16_t end = 0;
				uint16_t foo = 0;
				uint16_t bar = 0;
				/////////////////////////////////////////////////////
				//parsing the raw data input for contact number,
				//date and text content
				////////////////////////////////////////////////////
				Serial.println(input);
				for (uint8_t i = 0; i < contactNumber; i++)
				{
					foo = input.indexOf(" ", input.indexOf("CPBR:", end));
					bar = input.indexOf("\"", input.indexOf("CPBR:", end));
					contact_id[i] = input.substring(foo+1, bar-1);

					start = input.indexOf("\"", input.indexOf("CPBR:", end));
					end = input.indexOf("\"", start + 1);
					phoneNumber[i] = input.substring(start + 1, end);

					start = input.indexOf("\"", end + 1);
					end = input.indexOf("\"", start + 1);
					contactName[i] = input.substring(start + 1, end);
				}

				Serial.print("done parsing data\ncontact number:");
				Serial.println(contactNumber);
				Serial.println(sizeof(contactName));
				for(int i = 0; i<contactNumber; i++){
					Serial.println(contactName[i]);
					delay(5);
				}

				menuChoice = contactsMenu("Contacts", contactName, phoneNumber, contactNumber);
				change = -10;
			} else menuChoice = contactsMenu("Contacts", contactName, phoneNumber, contactNumber);

			update();
			if (menuChoice != -2)
			{
				Serial.println(menuChoice);
				if (menuChoice == 0){
				if(newContact()){
					change = 1;
				}
				} else if (menuChoice < -1000){
					Serial.println("Edit this concat");
				} else if (menuChoice < -10){
					int id = menuChoice + 1000 - 1;
					if(deleteContact(contactName[id], phoneNumber[id], contact_id[id])){
						change = 1;
					}
				} else {
					callNumber(phoneNumber[menuChoice - 1]);
					while(!update());
				}
			} else {
				break;
			}
		}
	}
}

uint8_t MAKERphone::deleteContactSD(String name, String number)
{
	unsigned long elapsedMillis = millis();
	bool blinkState = 1;
	while (1)
	{
		display.fillScreen(TFT_BLACK);
		if(resolutionMode)
		{
			display.fillRect(0, 0, display.width(), 6, TFT_DARKGREY);
			display.setFreeFont(TT1);
			display.setCursor(0,5);
			display.drawFastHLine(0, 6, BUF2WIDTH, TFT_WHITE);
		}
		else
		{
			display.fillRect(0, 0, display.width(), 14, TFT_DARKGREY);
			display.setTextFont(2);
			display.setCursor(0,-2);
			display.drawFastHLine(0, 14, BUF2WIDTH, TFT_WHITE);
		}
		display.setTextColor(TFT_WHITE);
		display.print("Delete contact");

		if (millis() - elapsedMillis >= multi_tap_threshold) {
		elapsedMillis = millis();
		blinkState = !blinkState;
		}

		display.setTextColor(TFT_WHITE);
		display.setCursor(4, 17);
		display.print("Are you sure?");
		display.setCursor(4, 33);
		display.print(name);
		display.setCursor(4, 49);
		display.print(number);

		if (blinkState){
			display.drawRect(display.width() / 2 - 29, 102, 30*2, 9*2, TFT_RED);
			display.setTextColor(TFT_RED);
			display.setCursor(28*2, 103);
			display.printCenter("DELETE");
		}
		else {
			display.fillRect(display.width() / 2 - 29, 102, 30*2, 9*2, TFT_RED);
			display.setTextColor(TFT_WHITE);
			display.setCursor(28*2, 103);
			display.print("DELETE");
		}



		if (buttons.released(BTN_B)) //BUTTON BACK
		{
			Serial.println("Go back");
			while (!update());
			break;
		}
		if (buttons.released(BTN_A)) // DELETE
		{
			Serial.println("DElete");
			display.fillScreen(TFT_BLACK);
			display.setTextFont(2);
			display.setCursor(34, display.height()/2 -16);
			display.printCenter("Deleting contact...");
			while (!update());
			return 1;
		}
		update();
	}
	return 0;
}

void MAKERphone::contactsAppSD(){
	Serial.println("");
	Serial.println("Begin contacts");
	File file = SD.open("/contacts.json");

	if(file.size() < 2){ // empty -> FILL
		Serial.println("Override");
		file.close();
		JsonArray& jarr = mp.jb.parseArray("[{\"name\":\"foo\", \"number\":\"099\"}]");
		delay(10);
		File file1 = SD.open("/contacts.json", FILE_REWRITE);
		jarr.prettyPrintTo(file1);
		file1.close();
		file = SD.open("/contacts.json");
		while(!file) {}
	}

	JsonArray& jarr = mp.jb.parseArray(file);

	if(!jarr.success())
	{
		Serial.println("Error");
		display.fillScreen(TFT_BLACK);
		if(resolutionMode)
		{
			display.setCursor(0, display.height()/2);
			display.setFreeFont(TT1);
		}
		else
		{
			display.setCursor(0, display.height()/2 - 16);
			display.setTextFont(2);
		}
		display.printCenter("Error loading contacts");
		while (buttons.released(BTN_B) == 0)//BUTTON BACK
		while (!update());
		while (!update());
	}
	else
	{
		// Maybe later read from core.json
		// uint8_t contactNumber = countSubstring(input, "CPBR:");
		// Serial.println(contactNumber);

		Serial.println("Read from .json");
		for (JsonObject& elem : jarr) {
			Serial.println(elem["name"].as<char*>());
			Serial.println(elem["number"].as<char*>());
		}

		while(1){
			int menuChoice = -1;
			menuChoice = contactsMenuSD(&jarr);

			update();
			if (menuChoice != -2)
			{
				if (menuChoice == 0){
					String name, number;
					if(newContactSD(&name, &number)){
						JsonObject& newContact = mp.jb.createObject();
						newContact["name"] = name;
						newContact["number"] = number;
						jarr.add(newContact);
						File file = SD.open("/contacts.json", FILE_REWRITE);
						jarr.prettyPrintTo(file);
						file.close();
					}
				} else if (menuChoice < -10){
					int id = menuChoice + 1000 - 1;
					if(deleteContactSD(jarr[id]["name"], jarr[id]["number"])){
						jarr.remove(id);
						File file = SD.open("/contacts.json", FILE_REWRITE);
						jarr.prettyPrintTo(file);
						file.close();
					}
				// EDIT
				} else {
					callNumber(jarr[menuChoice - 1]["number"].as<char*>());
					while(!update());
				}
			} else {
				break;
			}
		}
	}
}

uint8_t MAKERphone::newContactSD(String *name, String *number)
{
	textPointer = 0;
	y = 20;  //Beggining point
	String content = "";
	String contact = "";
	String prevContent = "";
	char key = NO_KEY;
	bool cursor = 0; //editing contacts or text content
	unsigned long elapsedMillis = millis();
	bool blinkState = 1;
	while (1)
	{
		display.fillScreen(TFT_BLACK);
		if(resolutionMode)
		{
			display.fillRect(0, 0, display.width(), 6, TFT_DARKGREY);
			display.setFreeFont(TT1);
			display.setCursor(0,5);
			display.drawFastHLine(0, 6, BUF2WIDTH, TFT_WHITE);
		}
		else
		{
			display.fillRect(0, 0, display.width(), 14, TFT_DARKGREY);
			display.setTextFont(2);
			display.setCursor(0,-2);
			display.drawFastHLine(0, 14, BUF2WIDTH, TFT_WHITE);
		}
		display.setTextColor(TFT_WHITE);
		display.print("Contacts");
		if (millis() - elapsedMillis >= multi_tap_threshold) //cursor blinking routine
		{
		elapsedMillis = millis();
		blinkState = !blinkState;
		}
		if (cursor == 0) //inputting the contact number
		{
			key = buttons.kpdNum.getKey();
			if (key == 'A') //clear number
				contact = "";
			else if (key == 'C')
				contact.remove(contact.length() - 1);
			if (key != NO_KEY && isdigit(key) && contact.length() < 14)
				contact += key;
			display.setTextWrap(1);
			display.setCursor(4, 20);
			display.setTextFont(2);
			if (content == "")
			{
				display.setTextColor(TFT_LIGHTGREY);
				display.print(F("Name"));
				display.setTextColor(TFT_WHITE);
			}
			else
				display.print(content);
			display.setTextFont(2);
			display.setCursor(4, 38);
			display.print("Num: ");
			display.print(contact);
			if (blinkState == 1)
				display.drawFastVLine(display.getCursorX() + 1, display.getCursorY() + 3, 11, TFT_WHITE);
		}
		else //inputting contact name
		{
			display.setTextColor(TFT_WHITE);
			display.setCursor(2*2, 38);
			display.print("Num: ");
			if (contact == "")
			{
				display.setTextColor(TFT_LIGHTGREY);
				display.print(F("xxxxxxxx"));
				display.setTextColor(TFT_WHITE);
			}
			else
				display.print(contact);
			prevContent = content;
			content = textInput(content, 12);
			if (prevContent != content)
			{
				blinkState = 1;
				elapsedMillis = millis();
			}
			display.setTextColor(TFT_LIGHTGREY);
			display.setTextWrap(1);
			display.setCursor(2*2, 10*2);
			display.print(content);
			display.setTextColor(TFT_WHITE);
			if(blinkState == 1)
				display.drawFastVLine(display.getCursorX() + 1, display.getCursorY() + 3, 11, TFT_WHITE);
		}

		display.fillRect(display.width() / 2 - 29, 102, 30*2, 9*2, TFT_GREENYELLOW);
		display.setTextColor(TFT_WHITE);
		display.setCursor(31*2, 103);
		display.printCenter("SAVE");

		if (buttons.kpd.pin_read(JOYSTICK_B) == 0 && cursor == 1) { //BUTTON UP
		cursor = 0;
		}

		if (buttons.kpd.pin_read(JOYSTICK_D) == 0 && cursor == 0) { //BUTTON DOWN
		cursor = 1;
		}

		if (buttons.released(BTN_B)) //BUTTON BACK
		{
		while (!update());
		break;
		}
		if (buttons.released(BTN_A)) // SAVE CONTACT
		{
			if(contact != "" && content != "")
			{
				*name = contact;
				*number = content;
				return 1;
			}
		}
		update();
	}
	return 0;
}

int MAKERphone::contactsMenuSD(JsonArray *contacts){
	uint8_t cursor = 0;
	int32_t cameraY = 0;
	int32_t cameraY_actual = 0;
	uint8_t offset;
	uint8_t boxHeight;
	uint8_t length = contacts->size();
	if(resolutionMode)
	{
		offset = menuYOffset;
		boxHeight = 14;
	}
	else
	{
		offset = 19;
		boxHeight = 28;
	}
	while (1) {
		while (!update());
		display.fillScreen(TFT_BLACK);
		display.setCursor(0, 0);
		cameraY_actual = (cameraY_actual + cameraY) / 2;
		if (cameraY_actual - cameraY == 1) {
		cameraY_actual = cameraY;
		}

		contactsMenuNewBox(0, cameraY_actual);

		int i = 0;
		for (JsonObject& elem : *contacts) {
			contactsMenuDrawBoxSD(elem["name"].as<char*>(), elem["number"].as<char*>(), i+1, cameraY_actual);
			i++;
		}
		if(cursor == 0){
			contactsMenuNewBoxCursor(cursor, cameraY_actual);
		} else {
			contactsMenuDrawCursor(cursor, cameraY_actual);
		}

		// last draw the top entry thing
		if(resolutionMode)
		{
			display.fillRect(0, 0, display.width(), 6, TFT_DARKGREY);
			display.setFreeFont(TT1);
			display.setCursor(0,5);
			display.drawFastHLine(0, 6, BUF2WIDTH, TFT_WHITE);
		}
		else
		{
			display.fillRect(0, 0, display.width(), 14, TFT_DARKGREY);
			display.setTextFont(2);
			display.setCursor(0,-2);
			display.drawFastHLine(0, 14, BUF2WIDTH, TFT_WHITE);
		}
		display.setTextSize(1);
		display.setTextColor(TFT_WHITE);
		display.print("Contacts");

		if (buttons.kpd.pin_read(BTN_A) == 0) {   //BUTTON CONFIRM
		while (buttons.kpd.pin_read(BTN_A) == 0);// Exit when pressed
		break;
		}
		if (buttons.kpd.pin_read(JOYSTICK_A) == 0 && cursor != 0) {
		while (buttons.kpd.pin_read(JOYSTICK_A) == 0); // Delete
		return -1000 + cursor;
		}
		if (buttons.kpd.pin_read(JOYSTICK_C) == 0 && cursor != 0) {
		while (buttons.kpd.pin_read(JOYSTICK_C) == 0); // Edit contact
		return -3000 + cursor;
		}

		if (buttons.kpd.pin_read(JOYSTICK_D) == 0) {  //BUTTON UP
		while (buttons.kpd.pin_read(JOYSTICK_D) == 0);
		if (cursor == 0) {
			cursor = length;
			if (length > 2) {
			cameraY = -(cursor - 2) * (boxHeight+1);
			}
		}
		else {
			cursor--;
			if (cursor > 0 && (cursor * (boxHeight+1) + cameraY + offset) < (boxHeight+1)) {
			cameraY += (boxHeight+1);
			}
		}
		}

		if (buttons.kpd.pin_read(JOYSTICK_B) == 0) { //BUTTON DOWN
		while (buttons.kpd.pin_read(JOYSTICK_B) == 0);

		cursor++;
		if ((cursor * (boxHeight+1) + cameraY + offset) > 48) {
			cameraY -= (boxHeight+1);
		}
		if (cursor >= length + 1) {
			cursor = 0;
			cameraY = 0;

		}

		}
		if (buttons.released(BTN_B) == 1) //BUTTON BACK
		{
		while (!update());
		return -2;
		}
	}
	return cursor;
}

void MAKERphone::contactsMenuDrawBoxSD(String name, String number, uint8_t i, int32_t y) {
	uint8_t offset;
	uint8_t boxHeight;
	if(resolutionMode)
	{
		offset = menuYOffset;
		boxHeight = 14;
	}
	else
	{
		offset = 19;
		boxHeight = 28;
	}
	y += i * boxHeight + offset;
	if (y < 0 || y > display.height()) {
		return;
	}
	if(resolutionMode)
	{
		display.setTextSize(1);
		display.setFreeFont(TT1);
		display.fillRect(1, y + 1, display.width() - 2, 13, TFT_DARKGREY);
		display.setTextColor(TFT_WHITE);
		display.setCursor(2, y + 2);
		display.drawString(name, 3, y + 2);
		display.drawString(number, 3, y + 8);
	}
	else
	{
		display.setTextSize(1);
		display.setTextFont(2);
		display.fillRect(1, y + 1, display.width() - 2, boxHeight-1, TFT_DARKGREY);
		display.setTextColor(TFT_WHITE);
		display.setCursor(2, y + 2);
		display.drawString(name, 4, y);
		display.drawString(number, 4, y + 12);
	}

}

String MAKERphone::readAllContacts() {
	String buffer;
	Serial1.print(F("AT+CPBR=1,250\r"));
	buffer = readSerial();
	delay(5);
	if (buffer.indexOf("CPBR:") != -1 || buffer.indexOf("OK") != -1) {
		return buffer;
	}
	else return "";
}


//Phone app
void MAKERphone::phoneApp() {
	dataRefreshFlag = 0;
	dialer();
	update();
}
void MAKERphone::dialer() {
	String callBuffer = "";
	char key = NO_KEY;
	display.setTextWrap(0);
	if(resolutionMode)
		display.setFreeFont(TT1);
	else
		display.setTextFont(2);
	while (1)
	{
		Serial.println(display.cursor_x);
		display.fillScreen(TFT_BLACK);
		display.setTextSize(1);
		if(resolutionMode)
		{
			display.fillRect(0, 42, display.width(), 14, TFT_DARKGREY);
			display.setCursor(1, 62);
			display.print("Press A to call");
			display.setCursor(0, 6);
			display.setTextColor(TFT_LIGHTGREY);
			display.print("Dialer");
		}
		else
		{
			display.fillRect(0, 79, display.width(), 26, TFT_DARKGREY);
			display.setCursor(2, 112);
			display.print("Press A to call");
			display.setCursor(2, -1);
			display.setTextColor(TFT_LIGHTGREY);
			display.print("Dialer");
		}
		display.setTextColor(TFT_WHITE);


		key = buttons.kpdNum.getKey();
		if (key == 'A') //clear number
			callBuffer = "";
		else if (key == 'C')
			callBuffer.remove(callBuffer.length()-1);
		if (key != NO_KEY && key!= 'A' && key != 'C')
			callBuffer += key;
		if(resolutionMode)
			display.setCursor(1, 53);
		else
			display.setCursor(0, 76);
		display.setTextSize(2);
		display.print(callBuffer);

		if (display.cursor_x + 4  >= display.width())
		{
			if(resolutionMode)
			{
				display.fillRect(0, 42, BUFWIDTH, 14, TFT_DARKGREY);
				display.setCursor(display.width() - display.cursor_x, 53);
			}
			else
			{
				display.fillRect(0, 79, display.width(), 26, TFT_DARKGREY);
				display.setCursor(display.width() - display.cursor_x - 14, 76);
			}

			display.print(callBuffer);
		}


		if (buttons.pressed(BTN_A))//initate call
		{
			callNumber(callBuffer);
			while (!update());
			callBuffer = "";
		}
		if (buttons.released(BTN_B)) //BACK BUTTON
			break;

		update();
	}
}

//Media app
int16_t MAKERphone::mp3Menu(const char* title, String* items, uint8_t length) {
	cursor = 0;
	cameraY = 0;
	cameraY_actual = 0;
	String Name;
	uint8_t scale;
	uint8_t offset;
	uint8_t boxHeight;
	if(resolutionMode)
	{
		scale = 1;
		offset = menuYOffset;
		boxHeight = 7;
	}
	else
	{
		scale = 2;
		offset = 19;
		boxHeight = 15;
	}
	while (1) {
		display.fillScreen(TFT_BLACK);
		display.setCursor(0, 0);
		cameraY_actual = (cameraY_actual + cameraY) / 2;
		if (cameraY_actual - cameraY == 1) {
			cameraY_actual = cameraY;
		}

		for (uint8_t i = 0; i < length; i++) {
			Name = items[i];
			while (Name.indexOf("/", start) != -1)
				start = Name.indexOf("/", start) + 1;
			Name = Name.substring(start, Name.indexOf("."));
			gui.menuDrawBox(Name, i, cameraY_actual);
		}
		gui.menuDrawCursor(cursor, cameraY_actual);

		// last draw the top entry thing
		if(resolutionMode)
		{
			display.fillRect(0, 0, display.width(), 6, TFT_DARKGREY);
			display.setFreeFont(TT1);
			display.setCursor(0,5);
			display.drawFastHLine(0, 6, display.width(), TFT_WHITE);
		}
		else
		{
			display.fillRect(0, 0, display.width(), 14, TFT_DARKGREY);
			display.setTextFont(2);
			display.setCursor(0,-2);
			display.drawFastHLine(0, 14, display.width(), TFT_WHITE);
		}
		display.setTextSize(1);
		display.setTextColor(TFT_WHITE);
		display.print(title);

		if (buttons.released(BTN_A)) {   //BUTTON CONFIRM
			while (!update());
			break;
		}

		if (buttons.released(JOYSTICK_D)) {  //BUTTON UP

			while (!update());
			if (cursor == 0) {
				cursor = length - 1;
				if (length > 6*scale) {
					cameraY = -(cursor - 5) * (boxHeight + 1);
				}
			}
			else {
				cursor--;
				if (cursor > 0 && (cursor * (boxHeight + 1) + cameraY + offset) < boxHeight*2) {
					cameraY += (boxHeight + 1);
				}
			}
		}

		if (buttons.released(JOYSTICK_B)) { //BUTTON DOWN
			while (!update());
			cursor++;
			if ((cursor * (boxHeight + 1) + cameraY + offset) > 54*scale) {
				cameraY -= (boxHeight + 1);
			}
			if (cursor >= length) {
				cursor = 0;
				cameraY = 0;

			}

		}
		if (buttons.released(BTN_B)) //BUTTON BACK
		{
			while (!update());
			return -1;
		}
		update();
	}
	return cursor;

}
void MAKERphone::listMP3(const char * dirname, uint8_t levels) {
	mp3Count = 0;
	
	Serial.printf("Listing directory: %s\n", dirname);
	File root = SD.open(dirname);
	if (!root) {
		Serial.println("Failed to open directory");
		return;
	}
	if (!root.isDirectory()) {
		Serial.println("Not a directory");
		return;
	}
	int counter = 1;
	uint8_t start = 0;
	File file = root.openNextFile();
	while (file) {
		char temp[50];
		file.getName(temp,  50);
		String Name(temp);
		Serial.println(Name);
		if (Name.endsWith(F(".MP3")) || Name.endsWith(F(".mp3")))
		{
			Serial.print(counter);
			Serial.print(".   ");
			Serial.println(Name);
			mp3Files[counter - 1] = Name;
			Serial.println(Name);
			mp3Count++;
			counter++;
		}
		file = root.openNextFile();
	}
}
void MAKERphone::listPhotos(const char * dirname, uint8_t levels) {
	photoCount = 0;
	
	Serial.printf("Listing directory: %s\n", dirname);
	File root = SD.open(dirname);
	if (!root) {
		Serial.println("Failed to open directory");
		return;
	}
	if (!root.isDirectory()) {
		Serial.println("Not a directory");
		return;
	}
	int counter = 1;
	uint8_t start = 0;
	File file = root.openNextFile();
	while (file) {
		char temp[50];
		file.getName(temp,  50);
		String Name(temp);
		Serial.println(Name);
		if (Name.endsWith(F(".jpeg")) || Name.endsWith(F(".JPEG")) || Name.endsWith(F(".jpg")) || Name.endsWith(F(".JPG")))
		{
			Serial.print(counter);
			Serial.print(".   ");
			Serial.println(Name);
			photoFiles[counter - 1] = Name;
			Serial.println(Name);
			photoCount++;
			counter++;
		}
		file = root.openNextFile();
	}
}
void MAKERphone::mp3player(String songName) {
	/* 
	char test[songName.length() + 1];
	songName.toCharArray(test, songName.length() + 1);

	Serial.println(test);

	uint8_t x = 1;
	uint8_t y = 53;
	int8_t i = 0;

	bool playState = 1;
	long elapsedMillis = millis();

	file = new AudioFileSourceSD(test);
	//buff = new AudioFileSourceBuffer(file, 4056);
	id3 = new AudioFileSourceID3(file);
	//id3->RegisterMetadataCB(MDCallback, (void*)"ID3TAG");
	out = new AudioOutputI2S();
	mp3 = new AudioGeneratorMP3();
	mp3->begin(id3, out);
	Serial.println("Audio files setup complete");
	//out->SetRate(44100);

	while (1) {
		display.fillScreen(TFT_LIGHTGREY);

		//draw bitmaps
		display.drawBitmap(2, 20, previous);
		display.drawBitmap(65, 20, next);
		display.drawBitmap(2, 37, repeatSprite);
		display.drawBitmap(66, 37, shuffle);
		display.drawBitmap(37, 19, play);
		display.drawBitmap(17, 2, cover2);



		//prepare for text printing
		display.setTextColor(TFT_BLACK);
		display.setTextSize(1);
		display.setTextWrap(0);

		//drawtext
		display.setCursor(x - i, y + 5);
		display.print(songName);
		display.fillRect(0, y, x - 1, 5, TFT_LIGHTGREY);
		display.fillRect(BUFWIDTH, y, BUFWIDTH - x - width, 5, TFT_LIGHTGREY);
		if (millis() - elapsedMillis > 200) {
			i += 3;
			elapsedMillis = millis();
			update();

		}
		if (i >= display.textWidth(songName) + 3) {
			i = -80;
		}


		if (buttons.kpd.pin_read(BTN_B) == 0)
		{

			mp3->stop();
			while (buttons.kpd.pin_read(BTN_B) == 0);
			break;
		}

		if (buttons.kpd.pin_read(BTN_A) == 0) //PLAY/PAUSE BUTTON
		{
			while (buttons.kpd.pin_read(BTN_A) == 0);
			playState = !playState;
		}

		if (buttons.kpd.pin_read(JOYSTICK_D) == 0) //DOWN
		{
			while (buttons.kpd.pin_read(JOYSTICK_D) == 0);

			volume--;
			Serial.print("volume:");
			Serial.print(volume);
			buttons.kpd.writeVolumeRight(volume);
		}

		if (buttons.kpd.pin_read(JOYSTICK_B) == 0) //UP
		{
			while (buttons.kpd.pin_read(JOYSTICK_B) == 0);

			volume++;
			Serial.print("volume:");
			Serial.print(volume);
			buttons.kpd.writeVolumeRight(volume);
		}

		if (playState == 1)
		{
			if (mp3->isRunning()) {
				if (!mp3->loop())
					mp3->stop();
			}
			else {
				display.fillScreen(TFT_BLACK);
				display.setCursor(0, 5);
				display.printf("MP3 done\n");
			}
		}
		else
			out->stop();
	} */
}
void MAKERphone::mediaApp() {
	while(1)
	{
		int8_t input = mediaMenu(mediaItems, 3);

		if(input == 0) //music
		{
			if (!SD.begin(5, SD_SCK_MHZ(8)))
				Serial.println("SD card error");
			listMP3("/", 1);
			while (1)
			{
				int16_t index = mp3Menu("Select file to play:", mp3Files, mp3Count);
				if (index == -1)
					break;
				display.fillScreen(TFT_LIGHTGREY);
				mp3player(mp3Files[index]);
			} 
		}
		else if(input == 1) //photos
		{
			while (!SD.begin(5, SD_SCK_MHZ(8)))
				Serial.println("SD card error");
			listPhotos("/Images/", 0);
			while (1)
			{
				int16_t index = gui.menu("Select photo to open:", photoFiles, photoCount);
				if (index == -1)
					break;

				String temp = String("/Images/" + photoFiles[index]);
				Serial.println(temp);
				drawJpeg(temp, 0, 0);
				while(!buttons.released(BTN_A) && !buttons.released(BTN_B))
					update();
			} 
			while(!update());
		}
		else if(input == -1)
			break;
	}
}
void MAKERphone::drawJpeg(String filename, int xpos, int ypos) {

  Serial.println("===========================");
  Serial.print("Drawing file: "); Serial.println(filename);
  Serial.println("===========================");

  // Open the named file (the Jpeg decoder library will close it after rendering image)
  File jpegFile = SD.open( filename, FILE_READ);    // File handle reference for SPIFFS
  //  File jpegFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library

  if ( !jpegFile ) {
    Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
    return;
  }

  // Use one of the three following methods to initialise the decoder:
  //boolean decoded = JpegDec.decodeFsFile(jpegFile); // Pass a SPIFFS file handle to the decoder,
  //boolean decoded = JpegDec.decodeSdFile(jpegFile); // or pass the SD file handle to the decoder,
  boolean decoded = JpegDec.decodeSdFile(jpegFile);  // or pass the filename (leading / distinguishes SPIFFS files)
  // Note: the filename can be a String or character array type
  if (decoded) {
    // print information about the image to the serial port
    jpegInfo();

    // render the image onto the screen at given coordinates
    jpegRender(xpos, ypos);
  }
  else {
    Serial.println("Jpeg file format not supported!");
  }
}
void MAKERphone::jpegRender(int xpos, int ypos) {
	Serial.println("JPEG render");
	delay(5);

	// retrieve infomration about the image
	uint16_t *pImg;
	uint16_t mcu_w = JpegDec.MCUWidth;
	uint16_t mcu_h = JpegDec.MCUHeight;
	uint32_t max_x = JpegDec.width;
	uint32_t max_y = JpegDec.height;

	// Jpeg images are drawn as a set of image block (tiles) called Minimum Coding Units (MCUs)
	// Typically these MCUs are 16x16 pixel blocks
	// Determine the width and height of the right and bottom edge image blocks
	Serial.println("Here");
	delay(5);
	uint32_t min_w = min(mcu_w, max_x % mcu_w);
	
	uint32_t min_h = min(mcu_h, max_y % mcu_h);

	// save the current image block size
	uint32_t win_w = mcu_w;
	uint32_t win_h = mcu_h;

	// record the current time so we can measure how long it takes to draw an image
	uint32_t drawTime = millis();

	// save the coordinate of the right and bottom edges to assist image cropping
	// to the screen size
	max_x += xpos;
	max_y += ypos;
	
	// read each MCU block until there are no more
	while (JpegDec.readSwappedBytes())
	{ // Swap byte order so the SPI buffer can be used

		// save a pointer to the image block
		pImg = JpegDec.pImage;

		// calculate where the image block should be drawn on the screen
		int mcu_x = JpegDec.MCUx * mcu_w + xpos; // Calculate coordinates of top left corner of current MCU
		int mcu_y = JpegDec.MCUy * mcu_h + ypos;

		// check if the image block size needs to be changed for the right edge
		if (mcu_x + mcu_w <= max_x)
			win_w = mcu_w;
		else
			win_w = min_w;

		// check if the image block size needs to be changed for the bottom edge
		if (mcu_y + mcu_h <= max_y)
			win_h = mcu_h;
		else
			win_h = min_h;

		// copy pixels into a contiguous block
		if (win_w != mcu_w)
		{
			uint16_t *cImg;
			int p = 0;
			cImg = pImg + win_w;
			for (int h = 1; h < win_h; h++)
			{
				p += mcu_w;
				for (int w = 0; w < win_w; w++)
				{
					*cImg = *(pImg + w + p);
					cImg++;
				}
			}
		}

		// draw image MCU block only if it will fit on the screen
		if ((mcu_x + win_w) <= display.width() && (mcu_y + win_h) <= display.height())
		{
			display.setSwapBytes(1);
			display.pushImage(mcu_x, mcu_y, win_w, win_h, pImg);
		}

		else if ((mcu_y + win_h) >= display.height())
			JpegDec.abort();

  }

  // calculate how long it took to draw the image
  drawTime = millis() - drawTime; // Calculate the time it took

  // print the results to the serial port
  Serial.print  ("Total render time was    : "); Serial.print(drawTime); Serial.println(" ms");
  Serial.println("=====================================");

}
void MAKERphone::jpegInfo() {

  Serial.println("===============");
  Serial.println("JPEG image info");
  Serial.println("===============");
  Serial.print  ("Width      :"); Serial.println(JpegDec.width);
  Serial.print  ("Height     :"); Serial.println(JpegDec.height);
  Serial.print  ("Components :"); Serial.println(JpegDec.comps);
  Serial.print  ("MCU / row  :"); Serial.println(JpegDec.MCUSPerRow);
  Serial.print  ("MCU / col  :"); Serial.println(JpegDec.MCUSPerCol);
  Serial.print  ("Scan type  :"); Serial.println(JpegDec.scanType);
  Serial.print  ("MCU width  :"); Serial.println(JpegDec.MCUWidth);
  Serial.print  ("MCU height :"); Serial.println(JpegDec.MCUHeight);
  Serial.println("===============");
  Serial.println("");
}
int8_t MAKERphone::mediaMenu(String* title, uint8_t length) {
	bool pressed = 0;
	uint8_t cursor = 0;
	int32_t cameraY = 0;
	int32_t cameraY_actual = 0;
	dataRefreshFlag = 0;

	uint8_t boxHeight;
	boxHeight = 40; //actually 2 less than that
	while (1) {
		while (!update());
		display.fillScreen(TFT_BLACK);
		display.setCursor(0, 0);
		cameraY_actual = (cameraY_actual + cameraY) / 2;
		if (cameraY_actual - cameraY == 1) {
			cameraY_actual = cameraY;
		}

		for (uint8_t i = 0; i < length; i++) {
			mediaMenuDrawBox(title[i], i, cameraY_actual);
		}
		mediaMenuDrawCursor(cursor, cameraY_actual, pressed);

		if (buttons.kpd.pin_read(JOYSTICK_B) == 1 && buttons.kpd.pin_read(JOYSTICK_D) == 1)
			pressed = 0;

		if (buttons.released(BTN_A)) {   //BUTTON CONFIRM

			while (!update());// Exit when pressed
			break;
		}

		if (buttons.released(JOYSTICK_D)) {  //BUTTON UP
			if (cursor == 0) {
				cursor = length - 1;
				if (length > 6) {
					cameraY = -(cursor - 2) * boxHeight;
				}
			}
			else {
				cursor--;
				if (cursor > 0 && (cursor * boxHeight + cameraY + settingsMenuYOffset) < boxHeight) {
					cameraY += 15;
				}
			}
			pressed = 1;
		}

		if (buttons.released(JOYSTICK_B)) { //BUTTON DOWN
			cursor++;
			if ((cursor * boxHeight + cameraY + settingsMenuYOffset) > 128) {
				cameraY -= boxHeight;
			}
			if (cursor >= length) {
				cursor = 0;
				cameraY = 0;

			}
			pressed = 1;
		}


		if (buttons.released(BTN_B) == 1) //BUTTON BACK
		{
			return -1;
		}
	}

	return cursor;

}
void MAKERphone::mediaMenuDrawBox(String title, uint8_t i, int32_t y) {
	uint8_t scale;
	uint8_t boxHeight;
	scale = 2;
	boxHeight = 40;
	y += i * boxHeight + settingsMenuYOffset;
	if (y < 0 || y > display.width()) {
		return;
	}


	if (title == "Music") //red
	{
		display.fillRect(2, y + 1, display.width() - 4, boxHeight-2, TFT_DARKGREY);
	}
	if (title == "Photo") //green
	{
		display.fillRect(2, y + 1, display.width() - 4, boxHeight-2, TFT_DARKGREY);
	}
	if (title == "Video") //yellow
	{
		display.fillRect(2, y + 1, display.width() - 4, boxHeight-2, TFT_DARKGREY);
	}
	display.setTextColor(TFT_BLACK);
	display.setTextSize(2);
	display.setTextFont(2);
	display.drawString(title, 60, y +  5);
	display.setTextColor(TFT_WHITE);
	display.setTextSize(1);
}
void MAKERphone::mediaMenuDrawCursor(uint8_t i, int32_t y, bool pressed) {
	uint8_t boxHeight;
	boxHeight = 40;
	if (millis() % 500 <= 250 && pressed == 0) {
		return;
	}
	y += i * boxHeight + settingsMenuYOffset;
	display.drawRect(0, y-1, display.width()-1, boxHeight+2, TFT_RED);
	display.drawRect(1, y, display.width()-3, boxHeight, TFT_RED);
}
void MAKERphone::MDCallback(void *cbData, const char *type, bool isUnicode, const char *string) {
	/* (void)cbData;
	Serial.printf("ID3 callback for: %s = '", type);

	if (isUnicode) {
		string += 2;
	}

	while (*string) {
		char a = *(string++);
		if (isUnicode) {
			string++;
		}
		Serial.printf("%c", a);
	}
	Serial.printf("'\n");
	Serial.flush();
*/}


//Settings app
int8_t MAKERphone::settingsMenu(String* title, uint8_t length) {
	bool pressed = 0;
	uint8_t cursor = 0;
	int32_t cameraY = 0;
	int32_t cameraY_actual = 0;
	dataRefreshFlag = 0;

	uint8_t boxHeight;
	if(resolutionMode)
		boxHeight = 15;
	else
		boxHeight = 20; //actually 2 less than that
	while (1) {
		while (!update());
		display.fillScreen(TFT_BLACK);
		display.setCursor(0, 0);
		cameraY_actual = (cameraY_actual + cameraY) / 2;
		if (cameraY_actual - cameraY == 1) {
			cameraY_actual = cameraY;
		}

		for (uint8_t i = 0; i < length; i++) {
			settingsMenuDrawBox(title[i], i, cameraY_actual);
		}
		settingsMenuDrawCursor(cursor, cameraY_actual, pressed);

		if (buttons.kpd.pin_read(JOYSTICK_B) == 1 && buttons.kpd.pin_read(JOYSTICK_D) == 1)
			pressed = 0;

		if (buttons.released(BTN_A)) {   //BUTTON CONFIRM

			while (!update());// Exit when pressed
			break;
		}

		if (buttons.released(JOYSTICK_D)) {  //BUTTON UP
			if (cursor == 0) {
				cursor = length - 1;
				if (length > 6) {
					cameraY = -(cursor - 2) * boxHeight;
				}
			}
			else {
				cursor--;
				if (cursor > 0 && (cursor * boxHeight + cameraY + settingsMenuYOffset) < boxHeight) {
					cameraY += 15;
				}
			}
			pressed = 1;
		}

		if (buttons.released(JOYSTICK_B)) { //BUTTON DOWN
			cursor++;
			if ((cursor * boxHeight + cameraY + settingsMenuYOffset) > 128) {
				cameraY -= boxHeight;
			}
			if (cursor >= length) {
				cursor = 0;
				cameraY = 0;

			}
			pressed = 1;
		}


		if (buttons.released(BTN_B) == 1) //BUTTON BACK
		{
			while(!update());
			return -1;
		}
	}

	return cursor;

}
void MAKERphone::settingsMenuDrawBox(String title, uint8_t i, int32_t y) {
	uint8_t scale;
	uint8_t boxHeight;
	if(resolutionMode)
	{
		scale = 1;
		boxHeight = 15;
	}
	else
	{
		scale = 2;
		boxHeight = 20;
	}
	y += i * boxHeight + settingsMenuYOffset;
	if (y < 0 || y > display.width()) {
		return;
	}


	if (title == "Network") //red
	{
		display.fillRect(2, y + 1, display.width() - 4, boxHeight-2, 0xFB6D);
		display.drawBitmap(6, y + 2*scale, network, 0x7800);
	}
	if (title == "Display") //green
	{
		display.fillRect(2, y + 1, display.width() - 4, boxHeight-2, 0x8FEA);
		display.drawBitmap(6, y + 2*scale, displayIcon, 0x0341);
	}
	if (title == "Date & time") //yellow
	{
		display.fillRect(2, y + 1, display.width() - 4, boxHeight-2, 0xFFED);
		display.drawBitmap(6, y + 2*scale, timeIcon, 0x6B60);
	}
	if (title == "Sound")//blue
	{
		display.fillRect(2, y + 1, display.width() - 4, boxHeight-2, 0xA7FF);
		display.drawBitmap(6, y + 2*scale, soundIcon, 0x010F);
	}
	if (title == "Security")//purple
	{
		display.fillRect(2, y + 1, display.width() - 4, boxHeight-2, 0xED1F);
		display.drawBitmap(6, y + 2*scale, security, 0x600F);
	}
	if (title == "About")//orange
	{
		display.fillRect(2, y + 1, display.width() - 4, boxHeight-2, 0xFD29);
		display.drawBitmap(6, y + 2*scale, about, 0x8200);
	}
	display.setTextColor(TFT_BLACK);
	display.setTextSize(1);
	display.setTextFont(2);
	display.drawString(title, 30, y + 2 );
	display.setTextColor(TFT_WHITE);
	display.setFreeFont(TT1);
}
void MAKERphone::settingsMenuDrawCursor(uint8_t i, int32_t y, bool pressed) {
	uint8_t boxHeight;
	if(resolutionMode)
		boxHeight = 15;
	else
		boxHeight = 20;
	if (millis() % 500 <= 250 && pressed == 0) {
		return;
	}
	y += i * boxHeight + settingsMenuYOffset;
	display.drawRect(0, y-1, display.width()-1, boxHeight+2, TFT_RED);
	display.drawRect(1, y, display.width()-3, boxHeight, TFT_RED);
}
bool MAKERphone::settingsApp() {
	while (!update());
	while (1)
	{
		int8_t input = settingsMenu(settingsItems, 6);
		if (input == -1) //BUTTON BACK
			break;
		if (input == 0)
			networkMenu();
		if (input == 1)
			displayMenu();
		if (input == 2)
			timeMenu();
		if (input == 3)
			soundMenu();
		if (input == 4)
			securityMenu();
		if (input == 5)
			if(updateMenu())
				return true;
	}
	

	applySettings();
	display.fillScreen(TFT_BLACK);
	display.setTextColor(TFT_WHITE);
	if(SDinsertedFlag)
	{
		saveSettings(1);
		display.setCursor(0, display.height()/2 - 16);
		display.setTextFont(2);
		display.printCenter("Settings saved!");
		uint32_t tempMillis = millis();
		while(millis() < tempMillis + 2000 && !buttons.released(BTN_A) && !buttons.released(BTN_B))
			update();
		while(!update());
	}
	else
	{
		display.setCursor(0, display.height()/2 - 20);
		display.setTextFont(2);
		display.printCenter("Can't save - No SD!");
		display.setCursor(0, display.height()/2);
		display.printCenter("Insert SD card and reset");
		uint32_t tempMillis = millis();
		while(millis() < tempMillis + 2000 && !buttons.pressed(BTN_A) && !buttons.pressed(BTN_B))
			update();
		while(!update());
	}
	return false;
	
}
void MAKERphone::networkMenu() {
	uint8_t cursor = 0;
	while (1)
	{
		Serial.println(airplaneMode);
		display.setTextColor(TFT_BLACK);
		display.fillScreen(0xFB6D);
		display.setTextFont(2);
		display.setTextSize(1);
		display.setCursor(18, 20);
		display.print("Wifi");
		display.setCursor(22, 58);
		display.print("BT");
		display.setCursor(16, 88);
		display.print("Plane\n   mode");

		display.setTextFont(2);
		display.setTextSize(1);
		display.setCursor(79, 19);
		display.print("ON");
		display.setCursor(122, 19);
		display.print("OFF");
		display.setCursor(79, 57);
		display.print("ON");
		display.setCursor(122, 57);
		display.print("OFF");
		display.setCursor(79, 95);
		display.print("ON");
		display.setCursor(122, 95);
		display.print("OFF");
		switch (cursor) {

		case 0:
			if (bt == 1)
			{
				display.drawRect(35*2, 27*2, 17*2, 11*2, TFT_BLACK);
			}
			else
			{
				display.drawRect(57*2, 27*2, 20*2, 11*2, TFT_BLACK);
			}
			if (airplaneMode == 1)
			{
				display.drawRect(35*2, 46*2, 17*2, 11*2, TFT_BLACK);
			}
			else
			{
				display.drawRect(57*2, 46*2, 20*2, 11*2, TFT_BLACK);
			}
			break;

		case 1:
			if (wifi == 1)
			{
				display.drawRect(35*2, 8*2, 17*2, 11*2, TFT_BLACK);
			}
			else
			{
				display.drawRect(57*2, 8*2, 20*2, 11*2, TFT_BLACK);
			}

			if (airplaneMode == 1)
			{
				display.drawRect(35*2, 46*2, 17*2, 11*2, TFT_BLACK);
			}
			else
			{
				display.drawRect(57*2, 46*2, 20*2, 11*2, TFT_BLACK);
			}
			break;

		case 2:
			if (wifi == 1)
			{
				display.drawRect(35*2, 8*2, 17*2, 11*2, TFT_BLACK);
			}
			else
			{
				display.drawRect(57*2, 8*2, 20*2, 11*2, TFT_BLACK);
			}

			if (bt == 1)
			{
				display.drawRect(35*2, 27*2, 17*2, 11*2, TFT_BLACK);
			}
			else
			{
				display.drawRect(57*2, 27*2, 20*2, 11*2, TFT_BLACK);
			}
		}




		if (cursor == 0)
		{
			if (millis() % 500 <= 250 && wifi == 1)
			{
				display.drawRect(35*2, 8*2, 17*2, 11*2, TFT_BLACK);
			}
			else if (millis() % 500 <= 250 && wifi == 0)
			{
				display.drawRect(57*2, 8*2, 20*2, 11*2, TFT_BLACK);
			}
			if (buttons.kpd.pin_read(JOYSTICK_A) == 0 && wifi == 0)
				wifi = !wifi;
			if (buttons.kpd.pin_read(JOYSTICK_C) == 0 && wifi == 1)
				wifi = !wifi;
		}
		if (cursor == 1)
		{
			if (millis() % 500 <= 250 && bt == 1)
			{
				display.drawRect(35*2, 27*2, 17*2, 11*2, TFT_BLACK);
			}
			else if (millis() % 500 <= 250 && bt == 0)
			{
				display.drawRect(57*2, 27*2, 20*2, 11*2, TFT_BLACK);
			}
			if (buttons.kpd.pin_read(JOYSTICK_A) == 0 && bt == 0)
				bt = !bt;
			if (buttons.kpd.pin_read(JOYSTICK_C) == 0 && bt == 1)
				bt = !bt;
		}
		if (cursor == 2)
		{
			if (millis() % 500 <= 250 && airplaneMode == 1)
			{
				display.drawRect(35*2, 46*2, 17*2, 11*2, TFT_BLACK);
			}
			else if (millis() % 500 <= 250 && airplaneMode == 0)
			{
				display.drawRect(57*2, 46*2, 20*2, 11*2, TFT_BLACK);
			}
			if (buttons.kpd.pin_read(JOYSTICK_A) == 0 && airplaneMode == 0)
				airplaneMode = !airplaneMode;
			if (buttons.kpd.pin_read(JOYSTICK_C) == 0 && airplaneMode == 1)
				airplaneMode = !airplaneMode;
		}

		if (buttons.kpd.pin_read(JOYSTICK_D) == 0) 
		{
			while (buttons.kpd.pin_read(JOYSTICK_D) == 0);
			if (cursor == 0)
				cursor = 2;
			else
				cursor--;
		}
		if (buttons.kpd.pin_read(JOYSTICK_B) == 0)
		{
			while (buttons.kpd.pin_read(JOYSTICK_B) == 0);
			if (cursor == 2)
				cursor = 0;
			else
				cursor++;

		}
		if (buttons.released(BTN_B)) //BUTTON BACK
		{
			while(!update());
			break;
		}
			

		update();
	}
}
void MAKERphone::displayMenu() {
	display.setTextFont(1);
	display.setTextColor(TFT_BLACK);
	uint8_t sleepTimeBuffer = sleepTime;
	uint16_t sleepTimeActualBuffer = sleepTimeActual;
	uint8_t cursor = 0;
	while (1)
	{
		display.setTextFont(2);
		display.setTextSize(1);
		display.fillScreen(0x8FEA);
		display.setCursor(9*2, 2*2);
		display.printCenter("Brightness");
		display.drawRect(33, 28, 47*2, 4*2, TFT_BLACK);
		display.drawBitmap(12, 27, noBrightness, TFT_BLACK, 2);
		display.drawBitmap(132, 21, fullBrightness, TFT_BLACK, 2);
		display.fillRect(35, 30, brightness * 9*2, 2*2, TFT_BLACK);

		String foo = "Sleep: ";
		if (sleepTimeActualBuffer > 60)
		{
			foo += sleepTimeActualBuffer / 60;
			foo += "min";
		}
		else
		{
			foo += sleepTimeActualBuffer;
			foo += "s";
		}
		display.setCursor(10*2, 44);
		display.printCenter(foo);

		display.drawRect(33, 65, 47*2, 4*2, TFT_BLACK);
		display.fillRect(35, 67, sleepTimeBuffer * 9*2, 2*2, TFT_BLACK);

		display.setCursor(12, 60);
		display.print("0s");
		display.setCursor(132, 61);
		display.print("30m");

		display.setCursor(11*2, 80);
		display.printCenter("Background");
		display.fillRect(16*2, 100, 48*2, 9*2, backgroundColors[backgroundIndex]);
		display.setCursor(18*2, 102);
		display.printCenter(backgroundColorsNames[backgroundIndex]);
		display.drawBitmap(11*2, 102, arrowLeft, TFT_BLACK, 2);
		display.drawBitmap(65*2, 102, arrowRight, TFT_BLACK, 2);

		if (cursor == 0)
		{
			if (millis() % 1000 <= 500)
			{
				display.drawBitmap(12, 27, noBrightness, TFT_BLACK, 2);
				display.drawBitmap(132, 21, fullBrightness, TFT_BLACK, 2);

			}
			else
			{
				display.drawBitmap(12, 27, noBrightness, 0x8FEA, 2);
				display.drawBitmap(132, 21, fullBrightness, 0x8FEA, 2);
			}
			if (buttons.released(JOYSTICK_A) && brightness != 0)
			{
				brightness--;
				while(!update());
			}
			if (buttons.released(JOYSTICK_C) && brightness != 5)
			{
				brightness++;
				while(!update());
			}
		}
		if (cursor == 1)
		{
			if (millis() % 1000 <= 500)
			{
				display.setCursor(12, 60);
				display.print("0s");
				display.setCursor(132, 61);
				display.print("30m");
			}
			else
			{
				display.setTextColor(0x8FEA);
				display.setCursor(12, 60);
				display.print("0s");
				display.setCursor(132, 61);
				display.print("30m");
				display.setTextColor(TFT_BLACK);
			}
			if (buttons.released(JOYSTICK_A) && sleepTimeBuffer!= 0)
			{
				sleepTimeBuffer--;
				while(!update());
			}
			if (buttons.released(JOYSTICK_C) && sleepTimeBuffer!= 5)
			{
				sleepTimeBuffer++;
				while(!update());
			}
		}
		if (cursor == 2)
		{
			if (millis() % 1000 <= 500)
			{
				if (backgroundIndex == 0)
				{
					display.fillRect(65*2 , 100, 20, 20, 0x8FEA);
					display.drawBitmap(11*2, 102, arrowLeft, TFT_BLACK, 2);
					display.drawBitmap(66*2, 102, arrowRight, TFT_BLACK, 2);
				}
				else if (backgroundIndex == 6)
				{
					display.fillRect(5*2 , 100, 20, 20, 0x8FEA);
					display.drawBitmap(10*2, 102, arrowLeft, TFT_BLACK, 2);
					display.drawBitmap(65*2, 102, arrowRight, TFT_BLACK, 2);
				}
				else
				{
					display.fillRect(65*2 , 100, 20, 20, 0x8FEA);
					display.fillRect(5*2, 100, 20, 20, 0x8FEA);
					display.drawBitmap(10*2, 102, arrowLeft, TFT_BLACK, 2);
					display.drawBitmap(66*2, 102, arrowRight, TFT_BLACK, 2);
				}
			}
			else
			{
				display.drawBitmap(11*2, 102, arrowLeft, TFT_BLACK, 2);
				display.drawBitmap(65*2, 102, arrowRight, TFT_BLACK, 2);
			}
			if (buttons.released(JOYSTICK_A) && backgroundIndex != 0)
			{
				backgroundIndex--;
				while(!update());
			}
			if (buttons.released(JOYSTICK_C) && backgroundIndex != 6)
			{
				backgroundIndex++;
				while(!update());
			}
		}

		if (buttons.released(JOYSTICK_D))
		{
			while (!update());
  		if (cursor == 0)
				cursor = 2;
			else
				cursor--;
		}
		if (buttons.released(JOYSTICK_B))
		{
			while (!update());
			if (cursor == 2)
				cursor = 0;
			else
				cursor++;
		}

		switch (sleepTimeBuffer) { //interpreting value into actual numbers
			case 0:
				sleepTimeActualBuffer = 0;
				break;
			case 1:
				sleepTimeActualBuffer = 10;
				break;
			case 2:
				sleepTimeActualBuffer = 30;
				break;
			case 3:
				sleepTimeActualBuffer = 60;
				break;
			case 4:
				sleepTimeActualBuffer = 600;
				break;
			case 5:
			sleepTimeActualBuffer = 1800;
			break;
		}
		if (buttons.released(BTN_B)) //BUTTON BACK
			break;
		update();

		if (brightness == 0)
			actualBrightness = 230;
		else
			actualBrightness = (5 - brightness) * 51;
		ledcAnalogWrite(LEDC_CHANNEL, actualBrightness);
	}
	sleepTimer = millis();
	sleepTime = sleepTimeBuffer;
	sleepTimeActual = sleepTimeActualBuffer;
}
void MAKERphone::soundMenu() {
	SD.begin(5, SD_SCK_MHZ(8));
	listRingtones("/ringtones", 0);
	listNotifications("/notifications", 0);
	
	ringtone = "/ringtones/chiptune.mp3";
	notification = "/notifications/to-the-point.mp3";
	String parsedRingtone;
	String parsedNotification;
	uint8_t start = 0;
	int8_t i;
	display.setTextFont(1);
	display.setTextColor(TFT_BLACK);
	uint8_t cursor = 0;
	while (ringtone.indexOf("/", start) != -1)
		start = ringtone.indexOf("/", start) + 1;
	parsedRingtone = ringtone.substring(start, ringtone.indexOf("."));
	start = 0;
	while (notification.indexOf("/", start) != -1)
		start = notification.indexOf("/", start) + 1;
	parsedNotification = notification.substring(start, notification.indexOf("."));
	while (1)
	{
		display.setTextFont(2);
		display.fillScreen(0xA7FF);
		display.setCursor(20, 4);
		display.printCenter("Volume");
		display.drawRect(33, 12*2, 47*2, 4*2, TFT_BLACK);
		display.drawBitmap(4*2, 10*2, noSound, TFT_BLACK, 2);
		display.drawBitmap(67*2, 10*2, fullSound, TFT_BLACK, 2);
		display.fillRect(35, 13*2, volume * 3*2, 2*2, TFT_BLACK);
		display.setCursor(15, 37);
		display.printCenter("Ringtone");
		display.drawRect(3*2, 55, 74*2, 11*2, TFT_BLACK);
		display.setCursor(6*2, 58);
		display.print(parsedRingtone);

		display.setCursor(5, 82);
		display.printCenter("Notification");
		display.drawRect(3*2, 100, 74*2, 11*2, TFT_BLACK);
		display.setCursor(6*2, 103);
		display.print(parsedNotification);

		if (cursor == 0)
		{
			if (millis() % 1000 <= 500)
			{
				display.drawBitmap(4*2, 10*2, noSound, TFT_BLACK, 2);
				display.drawBitmap(67*2, 10*2, fullSound, TFT_BLACK, 2);
			}
			else
			{
				display.drawBitmap(4*2, 10*2, noSound, 0xA7FF, 2);
				display.drawBitmap(67*2, 10*2, fullSound, 0xA7FF, 2);
			}
			if (buttons.released(JOYSTICK_A) && volume != 0)
			{
				volume--;
				while(!update());
			}
			if (buttons.released(JOYSTICK_C) && volume != 15)
			{
				volume++;
				while(!update());
			}
		}
		if (cursor == 1)
		{
			if (millis() % 1000 <= 500)
				display.drawRect(3*2, 55, 74*2, 11*2, TFT_BLACK);
			else
				display.drawRect(3*2, 55, 74*2, 11*2, 0xA7FF);

			if (buttons.released(BTN_A))
			{
				while(!update());
				display.setFreeFont(TT1);
				Serial.println(ringtoneCount);
				i = mp3Menu("Select ringtone:", ringtoneFiles, ringtoneCount);
				display.setTextColor(TFT_BLACK);
				if (i >= 0)
					ringtone = ringtoneFiles[i];
				else
					Serial.println("pressed B in menu");
				start = 0;
				while (ringtone.indexOf("/", start) != -1)
					start = ringtone.indexOf("/", start) + 1;
				parsedRingtone = ringtone.substring(start, ringtone.indexOf("."));
			}
		}
		if (cursor == 2)
		{
			if (millis() % 1000 <= 500)
				display.drawRect(3*2, 100, 74*2, 11*2, TFT_BLACK);
			else
				display.drawRect(3*2, 100, 74*2, 11*2, 0xA7FF);

			if (buttons.released(BTN_A))
			{
				while(!update());
				display.setFreeFont(TT1);
				i = mp3Menu("Select notification:", notificationFiles, notificationCount);
				display.setTextColor(TFT_BLACK);
				if (i >= 0)
					notification = notificationFiles[i];
				start = 0;
				while (notification.indexOf("/", start) != -1)
					start = notification.indexOf("/", start) + 1;
				parsedNotification = notification.substring(start, notification.indexOf("."));
			}
		}

		if (buttons.released(JOYSTICK_D))
		{
			if (cursor == 0)
				cursor = 2;
			else
				cursor--;
			while(!update());
		}
		if (buttons.released(JOYSTICK_B))
		{
			if (cursor == 2)
				cursor = 0;
			else
				cursor++;
			while(!update());
		}
		if (buttons.released(BTN_B)) //BUTTON BACK
			break;
		update();
	}
}
void MAKERphone::listRingtones(const char * dirname, uint8_t levels) {
	ringtoneCount = 0;
	
	Serial.printf("Listing directory: %s\n", dirname);

	File root = SD.open(dirname);
	if (!root) {
		Serial.println("Failed to open directory");
		return;
	}
	if (!root.isDirectory()) {
		Serial.println("Not a directory");
		return;

	}
	int counter = 1;
	File file = root.openNextFile();
	while (file) {
		String Name = file.name();
		if (Name.endsWith(F(".MP3")) || Name.endsWith(F(".mp3")))
		{
			Serial.print(counter);
			Serial.print(".   ");
			Serial.println(Name);
			ringtoneFiles[counter - 1] = Name;
			Serial.println(Name);
			ringtoneCount++;
			counter++;
		}
		file = root.openNextFile();
	}
}
void MAKERphone::listNotifications(const char * dirname, uint8_t levels) {
	notificationCount = 0;
	
	Serial.printf("Listing directory: %s\n", dirname);

	File root = SD.open(dirname);
	if (!root) {
		Serial.println("Failed to open directory");
		return;
	}
	if (!root.isDirectory()) {
		Serial.println("Not a directory");
		return;

	}
	int counter = 1;
	File file = root.openNextFile();
	while (file) {
		String Name = file.name();
		if (Name.endsWith(F(".MP3")) || Name.endsWith(F(".mp3")))
		{
			Serial.print(counter);
			Serial.print(".   ");
			Serial.println(Name);
			notificationFiles[counter - 1] = Name;
			Serial.println(Name);
			notificationCount++;
			counter++;
		}
		file = root.openNextFile();
	}
}
void MAKERphone::securityMenu() {
	
	pinNumber = 1234;

	String pinBuffer = "";
	String reply = "";
	String oldPin = "";
	uint32_t elapsedMillis = millis();
	uint32_t blinkMillis = millis();
	uint8_t cursor = 0;
	bool errorMessage = 0;
	bool confirmMessage = 0;
	bool saved = 0;
	char key = NO_KEY;
	bool blinkState = 0;
	while (!simReady)
	{
		if(resolutionMode)
		{
			display.setCursor(0, display.height()/2);
			display.setFreeFont(TT1);
		}
		else
		{
			display.setCursor(0, display.height()/2 - 16);
			display.setTextFont(2);
		}
		display.setTextColor(TFT_WHITE);
		display.fillScreen(TFT_BLACK);
		display.printCenter("GSM still booting...");
		update();
	}
	while (reply.indexOf("+SPIC:") == -1)
	{
		Serial1.println("AT+SPIC");
		reply = Serial1.readString();
		Serial.println(reply);
		delay(5);
	}
	timesRemaining = reply.substring(reply.indexOf(" ", reply.indexOf("+SPIC:")), reply.indexOf(",", reply.indexOf(" ", reply.indexOf("+SPIC:")))).toInt();
	Serial.println(timesRemaining);
	delay(5);
	if (timesRemaining == 0) //PUK lock WIP
	{
		display.fillScreen(TFT_BLACK);
		display.setTextColor(TFT_WHITE);
		display.setCursor(0, display.height()/2 - 16);
		display.setTextFont(2);
		display.printCenter("PUK lock");
		while (!buttons.released(BTN_A))
			update();
	}
	//check if the SIM card is locked
	while (reply.indexOf("+CLCK:") == -1)
	{
		Serial1.println("AT+CLCK=\"SC\", 2");
		reply = Serial1.readString();
		Serial.println(reply);
		delay(5);
	}
	if (reply.indexOf("+CLCK: 0") != -1)
		pinLock = 0;
	else if (reply.indexOf("+CLCK: 1") != -1)
		pinLock = 1;
	bool pinLockBuffer = pinLock;

	while (1)
	{
		if (timesRemaining == 0) //PUK lock WIP
		{
			display.fillScreen(TFT_BLACK);
			display.setTextColor(TFT_WHITE);
			display.setCursor(34, 30);
			display.printCenter("PUK lock");
			while (!buttons.pressed(BTN_A))
				update();
		}
		else
		{
			display.setTextColor(TFT_BLACK);
			display.fillScreen(0xED1F);
			display.setTextFont(2);
			display.setCursor(8, 15);
			display.print("PIN lock");
			display.setCursor(78, 15);
			display.print("ON");
			display.setCursor(120, 15);
			display.print("OFF");

			if (pinLockBuffer == 1)
			{
				display.setTextColor(TFT_BLACK);
				display.drawRect(3*2, 40*2, 74*2, 11*2, TFT_BLACK);
			}
			else
			{
				display.setTextColor(TFT_DARKGREY);
				display.fillRect(3*2, 40*2, 74*2, 11*2, TFT_DARKGREY);
			}
			display.setCursor(4*2, 31*2);
			display.print("PIN:");
			display.setCursor(6*2, 83);
			if (pinBuffer != "")
				display.printCenter(pinBuffer);
			else if (pinBuffer == "" && cursor != 1)
				display.printCenter("****");
			if (pinLockBuffer == 1 && cursor != 0)
				display.drawRect(69, 12, 17*2, 11*2, TFT_BLACK);
			else if (pinLockBuffer == 0 && cursor != 0)
				display.drawRect(113, 12, 38, 11*2, TFT_BLACK);
			display.setCursor(2, 111);
			if (cursor == 1 && !errorMessage && !confirmMessage)
				display.print("Press A to save PIN");
			if (millis() - blinkMillis >= multi_tap_threshold) //cursor blinking routine
			{
				blinkMillis = millis();
				blinkState = !blinkState;
			}

			if (cursor == 0)
			{
				if (millis() % 500 <= 250 && pinLockBuffer == 1)
					display.drawRect(69, 12, 17*2, 11*2, TFT_BLACK);
				else if (millis() % 500 <= 250 && pinLockBuffer == 0)
					display.drawRect(113, 12, 38, 11*2, TFT_BLACK);
				if (buttons.released(JOYSTICK_A) && pinLockBuffer == 0)
					pinLockBuffer = !pinLockBuffer;
				if (buttons.released(JOYSTICK_C) && pinLockBuffer == 1)
				{
					pinBuffer = "";
					pinLockBuffer = !pinLockBuffer;
					if (pinLock)
						while (1)
						{
							display.setTextFont(2);
							display.setTextColor(TFT_WHITE);
							display.fillScreen(TFT_BLACK);
							display.setCursor(5, 10);
							display.printCenter("Enter PIN:");
							display.setCursor(5, 40);
							display.setTextFont(1);
							String temp = "Remaining attempts: ";
							temp += timesRemaining;
							display.printCenter(temp);
							display.setTextFont(2);
							display.setCursor(1, 60);
							display.printCenter(pinBuffer);
							display.setCursor(2, 111);
							display.setTextFont(2);
							display.print("Press A to confirm");

							key = buttons.kpdNum.getKey();
							if (key == 'A') //clear number
								pinBuffer = "";
							else if (key == 'C' && pinBuffer != "")
								pinBuffer.remove(pinBuffer.length() - 1);
							if (key != NO_KEY && isDigit(key) && pinBuffer.length() != 4)
								pinBuffer += key;

							if (buttons.released(BTN_A))//enter PIN
							{
								while(!update());
								reply = "";
								Serial1.print(F("AT+CLCK=\"SC\", 0, \""));
								Serial1.print(pinBuffer);
								Serial1.println("\"");
								while (!Serial1.available());
								while (reply.indexOf("OK", reply.indexOf("AT+CLCK")) == -1 && reply.indexOf("ERROR", reply.indexOf("AT+CLCK")) == -1)
									reply = Serial1.readString();
								display.fillScreen(TFT_BLACK);
								display.setCursor(0, display.height()/2 - 16);
								display.setTextFont(2);
								if (reply.indexOf("OK", reply.indexOf("AT+CLCK")) != -1)
								{
									display.printCenter("PIN OK :)");
									while (!update());
									delay(1000);
									pinLock = 0;
									pinLockBuffer = 0;
									timesRemaining = 3;
									break;
								}
								else if (reply.indexOf("ERROR", reply.indexOf("AT+CLCK")) != -1)
								{


									Serial.println("Start:");
									Serial.println(reply);
									Serial.println("End");
									delay(5);
									pinBuffer = "";
									Serial.println(reply.indexOf("password"));
									delay(5);
									if (reply.indexOf("password") != -1)
									{
										display.printCenter("Wrong PIN :(");
										timesRemaining--;
									}
									else if (reply.indexOf("PUK") != -1)
									{
										timesRemaining = 0;
										break;
									}
									else
										display.printCenter("Invalid PIN");
									while (!update());
									delay(2000);
								}
								pinBuffer = "";
							}
							if (timesRemaining == 0)
								break;
							if (buttons.released(BTN_B))
							{
								pinLockBuffer = 1;
								while (!update());
								break;
							}

							update();
						}

				}
				pinBuffer = "";
			}


			else if (cursor == 1 && pinLockBuffer == 1)
			{
				key = buttons.kpdNum.getKey();
				if (key == 'A') //clear number
					pinBuffer = "";
				else if (key == 'C')
					pinBuffer.remove(pinBuffer.length() - 1);
				if (key != NO_KEY && isdigit(key) && pinBuffer.length() < 4)
					pinBuffer += key;
				display.setCursor(6*2, 83);
				display.setTextFont(2);
				display.printCenter(pinBuffer);
				if (blinkState == 1)
					display.drawFastVLine(display.getCursorX()+1, display.getCursorY()+2, 12, TFT_BLACK);
				if (buttons.released(BTN_A) && pinBuffer.length() == 4 && confirmMessage == 0)
				{

					while (!update());
					pinNumber = pinBuffer.toInt();

					reply = "";

					display.fillScreen(0xED1F);

					while (1)
					{
						display.setTextFont(2);
						display.setTextColor(TFT_WHITE);
						display.fillScreen(TFT_BLACK);
						display.setCursor(5, 10);
						display.printCenter("Enter old PIN:");
						display.setCursor(5, 40);
						display.setTextFont(1);
						String temp = "Remaining attempts: ";
						temp += timesRemaining;
						display.printCenter(temp);
						display.setTextFont(2);
						display.setCursor(1, 60);
						display.printCenter(oldPin);
						display.setCursor(2, 111);
						display.setTextFont(2);
						display.print("Press A to confirm");

						key = buttons.kpdNum.getKey();
						if (key == 'A') //clear number
							oldPin = "";
						else if (key == 'C' && oldPin != "")
							oldPin.remove(oldPin.length() - 1);
						if (key != NO_KEY && isDigit(key) && oldPin.length() != 4)
							oldPin += key;

						if (buttons.released(BTN_A))//enter PIN
						{
							while(!update());
							if (pinLock)
							{
								reply = "";
								Serial1.print(F("AT+CPWD=\"SC\", \""));
								Serial1.print(oldPin);
								Serial1.print("\", \"");
								Serial1.print(pinBuffer);
								Serial1.println("\"");
								while (!Serial1.available());
								while (reply.indexOf("OK", reply.indexOf("AT+CPWD")) == -1 && reply.indexOf("ERROR", reply.indexOf("AT+CPWD")) == -1)
								{
									reply = Serial1.readString();
									Serial.println(reply);
									delay(5);
								}
								display.fillScreen(TFT_BLACK);
								display.setCursor(0, display.height()/2 - 16);
								display.setTextFont(2);
								if (reply.indexOf("OK", reply.indexOf("AT+CPWD")) != -1)
								{
									timesRemaining = 3;
									pinLock = 1;
									saved = 1;
									break;
								}
								else if (reply.indexOf("ERROR", reply.indexOf("AT+CPWD")) != -1)
								{

									if(reply.indexOf("incorrect") != -1)
									{
										oldPin = "";
										timesRemaining--;
										display.printCenter("Wrong PIN :(");
									}
									else if (reply.indexOf("PUK") != -1)
									{
										timesRemaining = 0;
										break;
									}
									else
										display.printCenter("Invalid PIN");
									while (!update());
									delay(2000);
								}
							}
							else
							{
								Serial1.print(F("AT+CLCK=\"SC\", 1, \""));
								Serial1.print(oldPin);
								Serial1.println("\"");
								while (!Serial1.available());
								while (reply.indexOf("OK", reply.indexOf("AT+CLCK")) == -1 && reply.indexOf("ERROR", reply.indexOf("AT+CLCK")) == -1)
								{
									reply = Serial1.readString();
									Serial.println(reply);
									delay(5);
								}
								if (reply.indexOf("OK", reply.indexOf("AT+CLCK")) != -1)
								{
									reply = "";
									Serial1.print(F("AT+CPWD=\"SC\", \""));
									Serial1.print(oldPin);
									Serial1.print("\", \"");
									Serial1.print(pinBuffer);
									Serial1.println("\"");
									while (!Serial1.available());
									while (reply.indexOf("OK", reply.indexOf("AT+CPWD")) == -1 && reply.indexOf("ERROR", reply.indexOf("AT+CPWD")) == -1)
									{
										reply = Serial1.readString();
										Serial.println(reply);
										delay(5);
									}
									display.fillScreen(TFT_BLACK);
									display.setCursor(0, display.height()/2 - 16);
									display.setTextFont(2);
									if (reply.indexOf("OK", reply.indexOf("AT+CPWD")) != -1)
									{
										timesRemaining = 3;
										pinLock = 1;
										saved = 1;
										break;
									}
									else if (reply.indexOf("ERROR", reply.indexOf("AT+CPWD")) != -1)
									{
										oldPin = "";
										while (!update());
										timesRemaining--;
										saved = 0;
										if (timesRemaining == 0)
											break;
										saved = 0;
									}
								}
								else
								{

									saved = 0;

									display.fillScreen(TFT_BLACK);
									display.setTextFont(2);
									display.setCursor(0, display.height()/2 - 16);
									if (reply.indexOf("incorrect") != -1)
									{
										timesRemaining--;
										display.printCenter("Wrong PIN :(");
									}
									else if (reply.indexOf("PUK") != -1)
									{
										timesRemaining = 0;
										break;
									}
									else
										display.printCenter("Invalid PIN");
									while (!update());
									delay(2000);
									Serial.println(timesRemaining);
									delay(5);
									if (timesRemaining == 0)
										break;
								}

							}
							oldPin = "";
						}
						if (buttons.released(BTN_B))
						{
							saved = 0;
							while (!update());
							break;
						}
						if (timesRemaining == 0)
							break;
						update();
					}

					oldPin = "";
					pinBuffer = "";
					if (saved)
					{
						display.setCursor(2, 111);
						display.fillRect(2*2, 57*2, 78*2, 5*2, 0xED1F);
						display.print("PIN saved!");
						elapsedMillis = millis();
						confirmMessage = 1;
						errorMessage = 0;
						pinLock = 1;
						timesRemaining = 3;
					}
					else
					{
						confirmMessage = 0;
						errorMessage = 0;
						pinLock = 0;
					}
				}

				if (buttons.released(BTN_A) && pinBuffer.length() < 4 && errorMessage == 0)
				{
					while (!update());
					display.setCursor(2, 111);
					display.print("Pin must have 4+ digits");
					elapsedMillis = millis();
					errorMessage = 1;
					confirmMessage = 0;
				}
				if (millis() - elapsedMillis >= 2000 && errorMessage == 1)
				{
					display.fillRect(2*2, 57*2, 78*2, 5*2, 0xED1F);
					errorMessage = 0;
				}
				else if (millis() - elapsedMillis < 2000 && errorMessage == 1)
				{
					display.fillRect(2*2, 57*2, 78*2, 5*2, 0xED1F);
					display.setCursor(2, 111);
					display.print("Pin must have 4+ digits");
				}
				if (millis() - elapsedMillis >= 2000 && confirmMessage == 1)
				{
					display.fillRect(2*2, 57*2, 78*2, 5*2, 0xED1F);
					confirmMessage = 0;
				}
				else if (millis() - elapsedMillis < 2000 && confirmMessage == 1)
				{
					display.fillRect(2*2, 56*2, 78*2, 6*2, 0xED1F);
					display.setCursor(2, 111);
					display.print("PIN saved!");
				}
			}

			if (buttons.released(JOYSTICK_D))
			{
				while (!update());
				if (cursor == 0 && pinLockBuffer == 1)
					cursor = 1;
				else if (pinLockBuffer == 1 && cursor == 1)
					cursor--;
			}
			if (buttons.released(JOYSTICK_B))
			{
				while (!update());
				if (cursor == 1)
					cursor = 0;
				else if (pinLockBuffer == 1 && cursor == 0)
					cursor++;
			}
			if (buttons.released(BTN_B)) //BUTTON BACK
				break;
			update();
		}

	}
}
void MAKERphone::timeMenu()
{
	bool blinkState = 0;
	uint32_t previousMillis = millis();
	uint8_t cursor = 0;
	uint8_t editCursor = 0;
	String foo="";
	char key;
	display.setTextWrap(0);
	while(1)
	{
		display.fillScreen(0xFFED);
		display.setTextFont(2);
		display.setTextColor(TFT_BLACK);

		display.setCursor(52,10);
		if (clockHour < 10)
			display.print("0");
		display.print(clockHour);
		if(blinkState)
			display.print(":");
		else
			display.setCursor(display.cursor_x + 3, 10);
		if (clockMinute < 10)
			display.print("0");
		display.print(clockMinute);
		if(blinkState)
			display.print(":");
		else
			display.setCursor(display.cursor_x + 3, 10);
		if (clockSecond < 10)
			display.print("0");
		display.print(clockSecond);

		display.setCursor(40,30);
		if (clockDay < 10)
			display.print("0");
		display.print(clockDay);
		display.print("/");

		if (clockMonth < 10)
			display.print("0");
		display.print(clockMonth);
		display.print("/");
		display.print(2000 + clockYear);

		display.setCursor( 0, 65);
		display.printCenter("Edit time");
		display.drawRect(46,63, 68, 20, TFT_BLACK);
		display.setCursor( 40, 95);
		display.printCenter("Force time sync");
		display.drawRect(23, 93, 110, 20, TFT_BLACK);

		if(cursor == 0)
		{
			if(!blinkState)
				display.drawRect(46,63, 68, 20, 0xFFED);
			if(buttons.released(BTN_A))
			{
				while(!update());
				String inputBuffer;
				if(clockHour == 0)
					inputBuffer = "";
				else
					inputBuffer = String(clockHour);

				while(1)
				{
					display.fillScreen(0xFFED);
					display.setCursor(2, 98);
					display.print("Press A to save");
					display.setCursor(2, 110);
					display.print("Press B to cancel");
					switch (editCursor)
					{
						case 0:
							display.setCursor(52,10);
							if(inputBuffer == "")
								display.cursor_x+=16;
							else if (inputBuffer.length() == 1)
							{
								display.print("0"); display.print(inputBuffer);
							}
							else
								display.print(inputBuffer);
							if(blinkState)
								display.drawFastVLine(display.getCursorX() - 1, display.getCursorY() + 3, 11, TFT_BLACK);

							display.print(":");
							if (clockMinute < 10)
								display.print("0");
							display.print(clockMinute);
							display.print(":");
							if (clockSecond < 10)
								display.print("0");
							display.print(clockSecond);

							display.setCursor(40,30);
							if (clockDay < 10)
								display.print("0");
							display.print(clockDay);
							display.print("/");
							if (clockMonth < 10)
								display.print("0");
							display.print(clockMonth);
							display.print("/");
							display.print(2000 + clockYear);
							break;

						case 1:
							display.setCursor(52,10);
							if(clockHour < 10)
								display.print("0");
							display.print(clockHour);
							display.print(":");
							if(inputBuffer == "")
								display.cursor_x+=16;
							else if (inputBuffer.length() == 1)
							{
								display.print("0"); display.print(inputBuffer);
							}
							else
								display.print(inputBuffer);
							if(blinkState)
								display.drawFastVLine(display.getCursorX() - 1, display.getCursorY() + 3, 11, TFT_BLACK);
							display.print(":");
							if (clockSecond < 10)
								display.print("0");
							display.print(clockSecond);

							display.setCursor(40,30);
							if (clockDay < 10)
								display.print("0");
							display.print(clockDay);
							display.print("/");
							if (clockMonth < 10)
								display.print("0");
							display.print(clockMonth);
							display.print("/");
							display.print(2000 + clockYear);
							break;

						case 2:
							display.setCursor(52,10);
							if(clockHour < 10)
								display.print("0");
							display.print(clockHour);
							display.print(":");
							if(clockMinute < 10)
								display.print("0");
							display.print(clockMinute);
							display.print(":");
							if(inputBuffer == "")
								display.cursor_x+=16;
							if (inputBuffer.length() == 1)
							{
								display.print("0"); display.print(inputBuffer);
							}
							else
								display.print(inputBuffer);
							if(blinkState)
								display.drawFastVLine(display.getCursorX() - 1, display.getCursorY() + 3, 11, TFT_BLACK);

							display.setCursor(40,30);
							if (clockDay < 10)
								display.print("0");
							display.print(clockDay);
							display.print("/");
							if (clockMonth < 10)
								display.print("0");
							display.print(clockMonth);
							display.print("/");
							display.print(2000 + clockYear);
							break;

						case 3:
							display.setCursor(52,10);
							if(clockHour < 10)
								display.print("0");
							display.print(clockHour);
							display.print(":");
							if(clockMinute < 10)
								display.print("0");
							display.print(clockMinute);
							display.print(":");
							if (clockSecond < 10)
								display.print("0");
							display.print(clockSecond);

							display.setCursor(40,30);
							if(inputBuffer == "")
								display.cursor_x+=16;
							else if (inputBuffer.length() == 1)
							{
								display.print("0"); display.print(inputBuffer);
							}
							else
								display.print(inputBuffer);
							if(blinkState)
								display.drawFastVLine(display.getCursorX() - 1, display.getCursorY() + 3, 11, TFT_BLACK);

							display.print("/");
							if (clockMonth < 10)
								display.print("0");
							display.print(clockMonth);
							display.print("/");
							display.print(2000 + clockYear);
							break;

						case 4:
							display.setCursor(52,10);
							if(clockHour < 10)
								display.print("0");
							display.print(clockHour);
							display.print(":");
							if(clockMinute < 10)
								display.print("0");
							display.print(clockMinute);
							display.print(":");
							if (clockSecond < 10)
								display.print("0");
							display.print(clockSecond);

							display.setCursor(40,30);
							if (clockDay < 10)
								display.print("0");
							display.print(clockDay);
							display.print("/");
							if(inputBuffer == "")
								display.cursor_x+=16;
							else if (inputBuffer.length() == 1)
							{
								display.print("0"); display.print(inputBuffer);
							}
							else
								display.print(inputBuffer);
							if(blinkState)
								display.drawFastVLine(display.getCursorX() - 1, display.getCursorY() + 3, 11, TFT_BLACK);

							display.print("/");
							display.print(2000 + clockYear);
							break;

						case 5:
							display.setCursor(52,10);
							if(clockHour < 10)
								display.print("0");
							display.print(clockHour);
							display.print(":");
							if(clockMinute < 10)
								display.print("0");
							display.print(clockMinute);
							display.print(":");
							if (clockSecond < 10)
								display.print("0");
							display.print(clockSecond);

							display.setCursor(40,30);
							if (clockDay < 10)
								display.print("0");
							display.print(clockDay);
							display.print("/");
							if (clockMonth < 10)
								display.print("0");
							display.print(clockMonth);
							display.print("/");
							if(inputBuffer == "")
								display.print(2000);
							else
								display.print(2000 + inputBuffer.toInt());
							if(blinkState)
								display.drawFastVLine(display.getCursorX() - 1, display.getCursorY() + 3, 11, TFT_BLACK);
							break;

					}
					key = buttons.kpdNum.getKey();
					if (key == 'A') //clear number
						inputBuffer = "";
					else if (key == 'C')
						inputBuffer.remove(inputBuffer.length() - 1);
					if (key != NO_KEY && isdigit(key) && inputBuffer.length() < 2)
						inputBuffer += key;
					if(millis()-previousMillis >= 500)
					{
						previousMillis = millis();
						blinkState = !blinkState;
					}
					update();
					if(buttons.released(JOYSTICK_C) && editCursor < 5) //RIGHT
					{
						blinkState = 1;
						previousMillis = millis();
						while(!update());
						if(inputBuffer == "")
						{
							switch (editCursor)
							{
								case 0:
									clockHour = 0;
									break;
								case 1:
									clockMinute = 0;
									break;
								case 2:
									clockSecond = 0;
									break;
								case 3:
									clockDay = 0;
									break;
								case 4:
									clockMonth = 0;
									break;
								case 5:
									clockYear = 0;
									break;
							}
						}
						else
						{
							switch (editCursor)
							{
								case 0:
									clockHour = inputBuffer.toInt();
									break;
								case 1:
									clockMinute = inputBuffer.toInt();
									break;
								case 2:
									clockSecond = inputBuffer.toInt();
									break;
								case 3:
									clockDay = inputBuffer.toInt();
									break;
								case 4:
									clockMonth = inputBuffer.toInt();
									break;
								case 5:
									clockYear = inputBuffer.toInt();
									break;
							}
						}
						switch (editCursor)
						{
							case 0:
								if(clockMinute != 0)
									inputBuffer = String(clockMinute);
								else
									inputBuffer = "";
								break;
							case 1:
								if(clockSecond != 0)
									inputBuffer = String(clockSecond);
								else
									inputBuffer = "";
								break;
							case 2:
								if(clockDay != 0)
									inputBuffer = String(clockDay);
								else
									inputBuffer = "";
								break;
							case 3:
								if(clockMonth != 0)
									inputBuffer = String(clockMonth);
								else
									inputBuffer = "";
								break;
							case 4:
								if(clockYear != 0)
									inputBuffer = String(clockYear);
								else
									inputBuffer = "";
								break;
						}
						editCursor++;

					}
					if(buttons.released(JOYSTICK_A) && editCursor > 0) //LEFT
					{
						while(!update());
						blinkState = 1;
						previousMillis = millis();
						if(inputBuffer == "")
						{
							switch (editCursor)
							{
								case 0:
									clockHour = 0;
									break;
								case 1:
									clockMinute = 0;
									break;
								case 2:
									clockSecond = 0;
									break;
								case 3:
									clockDay = 0;
									break;
								case 4:
									clockMonth = 0;
									break;
								case 5:
									clockYear = 0;
									break;
							}
						}
						else
						{
							switch (editCursor)
							{
								case 0:
									clockHour = inputBuffer.toInt();
									break;
								case 1:
									clockMinute = inputBuffer.toInt();
									break;
								case 2:
									clockSecond = inputBuffer.toInt();
									break;
								case 3:
									clockDay = inputBuffer.toInt();
									break;
								case 4:
									clockMonth = inputBuffer.toInt();
									break;
								case 5:
									clockYear = inputBuffer.toInt();
									break;
							}
						}
						switch (editCursor)
						{
							case 1:
								if(clockHour != 0)
									inputBuffer = String(clockHour);
								else
									inputBuffer = "";
								break;
							case 2:
								if(clockMinute != 0)
									inputBuffer = String(clockMinute);
								else
									inputBuffer = "";
								break;
							case 3:
								if(clockSecond != 0)
									inputBuffer = String(clockSecond);
								else
									inputBuffer = "";
								break;
							case 4:
								if(clockDay != 0)
									inputBuffer = String(clockDay);
								else
									inputBuffer = "";
								break;
							case 5:
								if(clockMonth != 0)
									inputBuffer = String(clockMonth);
								else
									inputBuffer = "";
								break;
						}
						editCursor--;

					}

					if(buttons.released(JOYSTICK_D) && editCursor > 2) //UP
					{
						while(!update());
						blinkState = 1;
						previousMillis = millis();
						if(inputBuffer == "")
						{
							switch (editCursor)
							{
								case 0:
									clockHour = 0;
									break;
								case 1:
									clockMinute = 0;
									break;
								case 2:
									clockSecond = 0;
									break;
								case 3:
									clockDay = 0;
									break;
								case 4:
									clockMonth = 0;
									break;
								case 5:
									clockYear = 0;
									break;
							}
						}
						else
						{
							switch (editCursor)
							{
								case 0:
									clockHour = inputBuffer.toInt();
									break;
								case 1:
									clockMinute = inputBuffer.toInt();
									break;
								case 2:
									clockSecond = inputBuffer.toInt();
									break;
								case 3:
									clockDay = inputBuffer.toInt();
									break;
								case 4:
									clockMonth = inputBuffer.toInt();
									break;
								case 5:
									clockYear = inputBuffer.toInt();
									break;
							}
						}
						switch (editCursor)
						{
							case 3:
								if(clockHour != 0)
									inputBuffer = String(clockHour);
								else
									inputBuffer = "";
								break;
							case 4:
								if(clockMinute != 0)
									inputBuffer = String(clockMinute);
								else
									inputBuffer = "";
								break;
							case 5:
								if(clockSecond != 0)
									inputBuffer = String(clockSecond);
								else
									inputBuffer = "";
								break;
						}
						editCursor-=3;
					}
					if(buttons.released(JOYSTICK_B) && editCursor < 3) //DOWN
					{
						while(!update());
						blinkState = 1;
						previousMillis = millis();
						if(inputBuffer == "")
						{
							switch (editCursor)
							{
								case 0:
									clockHour = 0;
									break;
								case 1:
									clockMinute = 0;
									break;
								case 2:
									clockSecond = 0;
									break;
								case 3:
									clockDay = 0;
									break;
								case 4:
									clockMonth = 0;
									break;
								case 5:
									clockYear = 0;
									break;
							}
						}
						else
						{
							switch (editCursor)
							{
								case 0:
									clockHour = inputBuffer.toInt();
									break;
								case 1:
									clockMinute = inputBuffer.toInt();
									break;
								case 2:
									clockSecond = inputBuffer.toInt();
									break;
								case 3:
									clockDay = inputBuffer.toInt();
									break;
								case 4:
									clockMonth = inputBuffer.toInt();
									break;
								case 5:
									clockYear = inputBuffer.toInt();
									break;
							}
						}
						switch (editCursor)
						{
							case 0:
								if(clockDay != 0)
									inputBuffer = String(clockDay);
								else
									inputBuffer = "";
								break;
							case 1:
								if(clockMonth != 0)
									inputBuffer = String(clockMonth);
								else
									inputBuffer = "";
								break;
							case 2:
								if(clockYear != 0)
									inputBuffer = String(clockYear);
								else
									inputBuffer = "";
								break;
						}
						editCursor+=3;
					}

					if(buttons.released(BTN_A))
					{
						if(inputBuffer == "")
						{
							switch (editCursor)
							{
								case 0:
									clockHour = 0;
									break;
								case 1:
									clockMinute = 0;
									break;
								case 2:
									clockSecond = 0;
									break;
								case 3:
									clockDay = 0;
									break;
								case 4:
									clockMonth = 0;
									break;
								case 5:
									clockYear = 0;
									break;
							}
						}
						else
						{
							switch (editCursor)
							{
								case 0:
									clockHour = inputBuffer.toInt();
									break;
								case 1:
									clockMinute = inputBuffer.toInt();
									break;
								case 2:
									clockSecond = inputBuffer.toInt();
									break;
								case 3:
									clockDay = inputBuffer.toInt();
									break;
								case 4:
									clockMonth = inputBuffer.toInt();
									break;
								case 5:
									clockYear = inputBuffer.toInt();
									break;
							}
						}
						buttons.kpd.setHour(clockHour);
						buttons.kpd.setMinute(clockMinute);
						buttons.kpd.setSecond(clockSecond);
						buttons.kpd.setDate(clockDay);
						buttons.kpd.setMonth(clockMonth);
						buttons.kpd.setYear(clockYear);
						break;
					}


					if(buttons.released(BTN_B))
						break;

				}
				while(!update());
			}
		}
		else if(cursor == 1)
		{
			if(!blinkState)
				display.drawRect(23, 93, 110, 20, 0xFFED);
			if(buttons.released(BTN_A))
			{
				clockYear = 0;
				previousMillis = millis();
				while(1)
				{
					display.fillScreen(0xFFED);
					display.setCursor(0, display.height()/2 - 16);
					display.printCenter("Fetching time...");
					if(millis() - previousMillis >= 4000)
					{
						display.fillScreen(0xFFED);
						display.setCursor(0, display.height()/2 - 20);
						display.printCenter("Couldn't fetch time");
						display.setCursor(0, display.height()/2);
						display.printCenter("Set it manually");
						while(!update());
						delay(2000);
						break;
					}
					updateTimeGSM();
					if(clockYear < 80 && clockYear >= 19)
					{
						delay(200);
						display.fillScreen(0xFFED);
						display.setCursor(0, display.height()/2 - 16);
						display.printCenter("Time fetched over GSM!");
						while(!update());
						delay(1500);
						break;
					}
				}


			}
		}


		if(millis()-previousMillis >= 500)
		{
			previousMillis = millis();
			blinkState = !blinkState;
			updateTimeRTC();
		}
		if (buttons.released(JOYSTICK_D) && cursor > 0)
		{
			blinkState = 1;
			previousMillis = millis() + 400;
			while (!update());
			cursor--;
		}
		if (buttons.released(JOYSTICK_B) && cursor < 1)
		{
			blinkState = 1;
			previousMillis = millis() + 400;
			while (!update());
			cursor++;
		}
		if (buttons.released(BTN_B)) //BUTTON BACK
			break;
		update();
	}

}
bool MAKERphone::updateMenu()
{
	bool blinkState = 0;
	uint32_t previousMillis = millis();
	uint8_t cursor = 0;
	String foo="";
	display.setTextWrap(0);
	display.setTextFont(2);

	while(1)
	{
		display.fillScreen(0xFD29);
		display.setTextColor(TFT_BLACK);
		display.setCursor(10, 20);
		display.printCenter("Check for update");
		display.setCursor(40, 50);
		display.printCenter("Factory reset");
		display.setCursor(90, 100);
		display.setTextColor(TFT_DARKGREY);
		foo = "Version: " + (String)((int)firmware_version / 100) + "." + (String)((int)firmware_version / 10) + "." + (String)(firmware_version % 10);
		display.printCenter(foo);
		if(cursor == 0)
		{
			if(blinkState)
				display.drawRect(20,18,117, 20, TFT_BLACK);
			else
				display.drawRect(20,18,117, 20, 0xFD29);
			if(buttons.released(BTN_A))
			{

			}
		}
		else if (cursor == 1)
		{
			if(blinkState)
				display.drawRect(30, 48, 96, 20, TFT_BLACK);
			else
				display.drawRect(30, 48, 96, 20, 0xFD29);
			if(buttons.released(BTN_A))
			{
				while(!update());
				if(SDinsertedFlag)
				{
					display.fillScreen(0xFD29);
					display.setTextColor(TFT_BLACK);
					display.setCursor(10, 20);
					display.printCenter("Erasing in progress...");
					while(!update());

					String contacts_default = "[{\"name\":\"Foobar\", \"number\":\"099123123\"}]";
					String settings_default = "{ \"wifi\": 0, \"bluetooth\": 0, \"airplane_mode\": 0, \"brightness\": 5, \"sleep_time\": 0, \"background_color\": 0 }";

					const char contacts_path[] = "/contacts.json";
					const char settings_path[] = "/settings.json";

					JsonArray& contacts = mp.jb.parseArray(contacts_default);
					JsonObject& settings = mp.jb.parseObject(settings_default);

					SD.remove(contacts_path);
					SD.remove(settings_path);

					File contacts_file = SD.open(contacts_path, FILE_REWRITE);
					contacts.prettyPrintTo(contacts_file);
					contacts_file.close();

					File settings_file = SD.open(settings_path, FILE_REWRITE);
					settings.prettyPrintTo(settings_file);
					settings_file.close();

					wifi = settings["wifi"];
					bt = settings["bluetooth"];
					airplaneMode = settings["airplane_mode"];
					brightness = settings["brightness"];
					sleepTime = settings["sleep_time"];
					backgroundIndex = settings["background_color"];

					applySettings();

					return true;
				}
			}
		}
		if(millis()-previousMillis >= 250)
		{
			previousMillis = millis();
			blinkState = !blinkState;
		}
		if (buttons.released(JOYSTICK_D) && cursor > 0)
		{
			blinkState = 1;
			previousMillis = millis();
			while (!update());
			cursor--;
		}
		if (buttons.released(JOYSTICK_B) && cursor < 1)
		{
			blinkState = 1;
			previousMillis = millis();
			while (!update());
			cursor++;
		}
		if (buttons.released(BTN_B)) //BUTTON BACK
			break;
		update();
	}
	while(!update());
	return false;
}

void MAKERphone::saveSettings(bool debug)
{
	const char * path = "/settings.json";
	Serial.println("");
	File file = SD.open(path);
	JsonObject& settings = mp.jb.parseObject(file);
	file.close();

	if (settings.success()) {
		if(debug){
			Serial.print("wifi: ");
			Serial.println(settings["wifi"].as<bool>());
			Serial.print("bluetooth: ");
			Serial.println(settings["bluetooth"].as<bool>());
			Serial.print("airplane_mode: ");
			Serial.println(settings["airplane_mode"].as<bool>());
			Serial.print("brightness: ");
			Serial.println(settings["brightness"].as<int>());
			Serial.print("sleep_time: ");
			Serial.println(settings["sleep_time"].as<int>());
			Serial.print("background_color: ");
			Serial.println(settings["background_color"].as<int>());
		}

		settings["wifi"] = wifi;
		settings["bluetooth"] = bt;
		settings["airplane_mode"] = airplaneMode;
		settings["brightness"] = brightness;
		settings["sleep_time"] = sleepTime;
		settings["background_color"] = backgroundIndex;

		File file1 = SD.open(path, FILE_REWRITE);
		settings.prettyPrintTo(file1);
		file1.close();
	} else {
		Serial.println("Error saving new settings");
	}
}

void MAKERphone::loadSettings(bool debug)
{
	const char * path = "/settings.json";
	Serial.println("");
	File file = SD.open(path);
	JsonObject& settings = mp.jb.parseObject(file);
	file.close();

	if (settings.success()) {
		if(debug){
			Serial.print("wifi: ");
			Serial.println(settings["wifi"].as<bool>());
			Serial.print("bluetooth: ");
			Serial.println(settings["bluetooth"].as<bool>());
			Serial.print("airplane_mode: ");
			Serial.println(settings["airplane_mode"].as<bool>());
			Serial.print("brightness: ");
			Serial.println(settings["brightness"].as<int>());
			Serial.print("sleep_time: ");
			Serial.println(settings["sleep_time"].as<int>());
			Serial.print("background_color: ");
			Serial.println(settings["background_color"].as<int>());
		}

		wifi = settings["wifi"];
		bt = settings["bluetooth"];
		airplaneMode = settings["airplane_mode"];
		brightness = settings["brightness"];
		sleepTime = settings["sleep_time"];
		backgroundIndex = settings["background_color"];
	} else {
		Serial.println("Error loading new settings");
	}
}


void MAKERphone::applySettings()
{
	if(wifi)
	{
		// WiFi.begin();
		// delay(1);
	}
	else
	{
		// WiFi.disconnect(true); delay(1); // disable WIFI altogether
		// WiFi.mode(WIFI_MODE_NULL); delay(1);
	}

	if(bt)
		btStart();
	else
		btStop();
	if(airplaneMode)
		Serial1.println("AT+CFUN=4");
	else
		Serial1.println("AT+CFUN=1");

	if (brightness == 0)
		actualBrightness = 230;
	else
		actualBrightness = (5 - brightness) * 51;
	ledcAnalogWrite(LEDC_CHANNEL, actualBrightness);
	switch (sleepTime) { //interpreting value into actual numbers
	case 0:
		sleepTimeActual = 0;
		break;
	case 1:
		sleepTimeActual = 10;
		break;
	case 2:
		sleepTimeActual = 30;
		break;
	case 3:
		sleepTimeActual = 60;
		break;
	case 4:
		sleepTimeActual = 600;
		break;
	case 5:
		sleepTimeActual = 1800;
		break;
	}

}

//Buttons class
bool Buttons::pressed(uint8_t button) {
	return states[(uint8_t)button] == 1;
}
void Buttons::begin() {
	kpd.begin(14, 27);
	kpdNum.begin(14, 27);
}
void Buttons::update() {
	byte buttonsData = 0b0000000;

	for (uint8_t i = 0; i < 8; i++)
		bitWrite(buttonsData, i, (bool)kpd.pin_read(i));
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
uint16_t Buttons::timeHeld(uint8_t button) {
	if (states[(uint8_t)button] != 0xFFFF) {
		return states[(uint8_t)button];
	}
	else {
		return 0;
	}
}

//Collision
bool MAKERphone::collideRectRect(int16_t x1, int16_t y1, int16_t w1, int16_t h1, int16_t x2, int16_t y2, int16_t w2, int16_t h2) {
	return (x2 < x1 + w1 && x2 + w2 > x1
		&& y2 < y1 + h1 && y2 + h2 > y1);
}
bool MAKERphone::collidePointRect(int16_t pointX, int16_t pointY, uint16_t rectX, uint16_t rectY, uint16_t rectW, uint16_t rectH) {
	return (pointX >= rectX && pointX < rectX + rectW && pointY >= rectY && pointY < rectY + rectH);
}
bool MAKERphone::collideCircleCircle(int16_t centerX1, int16_t centerY1, int16_t r1, int16_t centerX2, int16_t centerY2, int16_t r2) {
	// True if the distance between the centers is smaller than the sum of the radii : dist < r1 + r2
	// dist = sqrt( (centerX1 - centerX2)^2 + (centerY1 - centerY2)^2 ) but we can't use sqrt (too slow).
	// however, sqrt(a) < b  <==>  a < b*b  for all natural numbers
	// in our case, 'a' is by definition a natural numbers
	// So let's make sure 'b' is a natural numbers
	r1 = abs(r1);
	r2 = abs(r2);
	// Now apply the formula
	return (((centerX1 - centerX2) * (centerX1 - centerX2) + (centerY1 - centerY2) * (centerY1 - centerY2)) < (r1 + r2) * (r1 + r2));
}
bool MAKERphone::collidePointCircle(int16_t pointX, int16_t pointY, int16_t centerX, int16_t centerY, int16_t r) {
	// Just like circleCircle collision, but r2 = 0
	r = abs(r);
	return (((pointX - centerX) * (pointX - centerX) + (pointY - centerY) * (pointY - centerY)) < r * r);
}

//SD operations
void MAKERphone::writeFile(const char * path, const char * message)
{
	while (!SD.begin(5, SD_SCK_MHZ(8)));
	Serial.printf("Writing file: %s\n", path);

	File file = SD.open(path, FILE_REWRITE);
	if (!file) {
		Serial.println("Failed to open file for writing");
		return;
	}
	if (file.print(message)) {
		Serial.println("File written");
	}
	else {
		Serial.println("Write failed");
	}
	file.close();
}
void MAKERphone::appendFile(const char * path, const char * message) {
	Serial.printf("Appending to file: %s\n", path);

	File file = SD.open(path, FILE_WRITE);
	if (!file) {
		Serial.println("Failed to open file for appending");
		return;
	}
	if (file.print(message)) {
		Serial.println("Message appended");
		delay(5);
	}
	else {
		Serial.println("Append failed");
		delay(5);
	}
	file.close();
}
String MAKERphone::readFile(const char * path) {
	while (!SD.begin(5, SD_SCK_MHZ(8)));
	Serial.printf("Reading file: %s\n", path);
	String helper="";
	File file = SD.open(path);
	if (!file) {
		Serial.println("Failed to open file for reading");
		return "";
	}

	Serial.print("Read from file: ");
	while (file.available()) {
		helper += (char)file.read();

	}
	file.close();

	return helper;
}

//GUI class
void GUI::drawNotificationWindow(uint8_t x, uint8_t y, uint8_t width, uint8_t height, String text) {
	uint8_t scale;
	if(mp.resolutionMode)
	{
		scale = 1;
		mp.display.setFreeFont(TT1);
		mp.display.setTextSize(1);
		mp.display.setCursor(x + 3*scale, y + 3*scale + 5*scale);
	}
	else
	{
		scale = 2;
		mp.display.setTextFont(2);
		mp.display.setTextSize(1);
		mp.display.setCursor(x + 3*scale, y + scale);
	}
	mp.display.fillRoundRect(x - scale, y + scale, width, height, scale, TFT_DARKGREY);
	mp.display.fillRoundRect(x, y, width, height, scale, TFT_WHITE);
	//mp.display.setCursor(x + 3*scale, y + 3*scale + 5*scale);
	mp.display.print(text);
}
void GUI::menuDrawBox(String text, uint8_t i, int32_t y) {
	uint8_t scale;
	uint8_t offset;
	uint8_t boxHeight;
	if(mp.resolutionMode)
	{
		scale = 1;
		offset = menuYOffset;
		boxHeight = 7;
	}
	else
	{
		scale = 2;
		offset = 19;
		boxHeight = 15;
	}
	y += i * (boxHeight + 1) + offset;
	if (y < 0 || y > mp.display.height()) {
		return;
	}
	mp.display.fillRect(1, y + 1, mp.display.width() - 2, boxHeight - (scale-1), TFT_DARKGREY);
	mp.display.setTextColor(TFT_WHITE);
	mp.display.setCursor(2, y + 2);
	if(mp.resolutionMode)
		mp.display.drawString(text, 3, y + 2);
	else
		mp.display.drawString(text, 3, y);
}
void GUI::menuDrawCursor(uint8_t i, int32_t y) {
	uint8_t offset;
	uint8_t boxHeight;
	if(mp.resolutionMode)
	{
		offset = menuYOffset;
		boxHeight = 7;
	}
	else
	{
		offset = 19;
		boxHeight = 15;
	}
	if (millis() % 500 <= 250) {
		return;
	}
	y += i * (boxHeight + 1) + offset;
	mp.display.drawRect(0, y, mp.display.width(), boxHeight + 2, TFT_RED);
}
int8_t GUI::menu(const char* title, String* items, uint8_t length) {
	while (1) {
		while (!mp.update());
		mp.display.fillScreen(TFT_BLACK);
		mp.display.setCursor(0, 0);
		cameraY_actual = (cameraY_actual + cameraY) / 2;
		if (cameraY_actual - cameraY == 1) {
			cameraY_actual = cameraY;
		}

		for (uint8_t i = 0; i < length; i++) {
			menuDrawBox(items[i], i, cameraY_actual);
		}
		menuDrawCursor(cursor, cameraY_actual);

		// last draw the top entry thing
		if(mp.resolutionMode)
		{
			mp.display.fillRect(0, 0, mp.display.width(), 6, TFT_DARKGREY);
			mp.display.setFreeFont(TT1);
			mp.display.setCursor(0,5);
			mp.display.drawFastHLine(0, 6, mp.display.width(), TFT_WHITE);
		}
		else
		{
			mp.display.fillRect(0, 0, mp.display.width(), 14, TFT_DARKGREY);
			mp.display.setTextFont(2);
			mp.display.setCursor(1,-2);
			mp.display.drawFastHLine(0, 14, mp.display.width(), TFT_WHITE);
		}
		mp.display.print(title);

		if (mp.buttons.released(BTN_A)) {   //BUTTON CONFIRM

			while (!mp.update());// Exit when pressed
			break;
		}
		if (mp.buttons.kpd.pin_read(BTN_B) == 0) {   //BUTTON BACK

			while (mp.buttons.kpd.pin_read(BTN_B) == 0);// Exit when pressed
			return -1;
		}

		if (mp.buttons.kpd.pin_read(JOYSTICK_D) == 0) {  //BUTTON DOWN
			mp.leds[3] = CRGB::Blue;
			mp.leds[4] = CRGB::Blue;
			while(!mp.update());
			mp.vibration(200);
			FastLED.clear();
			while (!mp.update());

			while (mp.buttons.kpd.pin_read(JOYSTICK_D) == 0);
			if (cursor == 0) {
				cursor = length - 1;
				if (length > 6) {
					cameraY = -(cursor - 5) * 8;
				}
			}
			else {
				cursor--;
				if (cursor > 0 && (cursor * 8 + cameraY + menuYOffset) < 14) {
					cameraY += 8;
				}
			}
		}

		if (mp.buttons.kpd.pin_read(JOYSTICK_B) == 0) { //BUTTON UP
			mp.leds[0] = CRGB::Blue;
			mp.leds[7] = CRGB::Blue;
			while (!mp.update());
			mp.vibration(200);
			FastLED.clear();
			while (!mp.update());

			while (mp.buttons.kpd.pin_read(JOYSTICK_B) == 0);

			cursor++;
			if ((cursor * 8 + cameraY + menuYOffset) > 54) {
				cameraY -= 8;
			}
			if (cursor >= length) {
				cursor = 0;
				cameraY = 0;

			}

		}
	}
	return cursor;

}
uint8_t GUI::drawCursor(uint8_t xoffset, uint8_t yoffset, uint8_t xelements, uint8_t yelements, uint8_t xstart, uint8_t ystart) {
	uint8_t index = 0;
	uint8_t cursorX = 0;
	uint8_t cursorY = 0;
	long elapsedMillis = millis();
	long elapsedMillis2 = millis();
	while (1)
	{
		mp.display.fillScreen(TFT_BLACK);
		mp.display.setFreeFont(TT1);
		mp.display.setTextSize(1);
		mp.display.setTextColor(TFT_WHITE);
		mp.display.drawString("Test", 0, 0);
		mp.display.drawFastHLine(0, 6, BUFWIDTH, TFT_WHITE);
		// Draw the icons
		mp.display.drawIcon(messages, 1, 8, width, height);
		mp.display.drawIcon(media, 26, 8, width, height);
		mp.display.drawIcon(contacts, 51, 8, width, height);
		mp.display.drawIcon(settings, 1, 33, width, height);
		mp.display.drawIcon(phone, 26, 33, width, height);
		mp.display.drawIcon(apps, 51, 33, width, height);

		mp.display.fillRect(0, 0, 80, 6, TFT_BLACK);
		index = cursorY * xelements + cursorX;
		mp.display.drawString(mp.titles[index], 0, 0);
		//kpd.begin(14, 27);

		if (millis() - elapsedMillis >= 250) {
			elapsedMillis = millis();
			cursorState = !cursorState;
		}
		if (millis() - elapsedMillis2 >= 100) {
			elapsedMillis2 = millis();
			previousButtonState = mp.kpd.pin_read(BTN_B);
		}


		if (cursorState == 1)
			mp.display.drawRect(xstart + cursorX * xoffset, ystart + cursorY * yoffset, width + 2, height + 2, TFT_RED);
		else
			mp.display.drawRect(xstart + cursorX * xoffset, ystart + cursorY * yoffset, width + 2, height + 2, TFT_BLACK);

		///////////////////////////////
		//////Checking for button input
		///////////////////////////////
		if (mp.buttons.kpd.pin_read(BTN_A) == 0) //CONFIRM
		{
			while (mp.buttons.kpd.pin_read(BTN_A) == 0);
			return cursorY * xelements + cursorX;  //returns index of selected icon

		}
		if (mp.buttons.kpd.pin_read(JOYSTICK_B) == 0) //UP
		{
			mp.leds[0] = CRGB::Black;
			mp.leds[7] = CRGB::Black;
			FastLED.show();
			mp.vibration(200);
			FastLED.clear();

			while (mp.buttons.kpd.pin_read(JOYSTICK_B) == 0);
			mp.display.drawRect(xstart + cursorX * xoffset, ystart + cursorY * yoffset, width + 2, height + 2, TFT_BLACK);
			if (cursorY == 0) {
				cursorY = yelements - 1;
			}
			else
				cursorY--;
			elapsedMillis = millis();
			cursorState = 1;
		}
		if (mp.buttons.kpd.pin_read(JOYSTICK_D) == 0)//DOWN
		{

			mp.leds[3] = CRGB::Blue;
			mp.leds[4] = CRGB::Blue;
			FastLED.show();
			mp.vibration(200);
			FastLED.clear();

			while (mp.buttons.kpd.pin_read(JOYSTICK_D) == 0);
			mp.display.drawRect(xstart + cursorX * xoffset, ystart + cursorY * yoffset, width + 2, height + 2, TFT_BLACK);
			if (cursorY == yelements - 1) {
				cursorY = 0;
			}
			else
				cursorY++;
			elapsedMillis = millis();
			cursorState = 1;
		}
		if (mp.buttons.kpd.pin_read(JOYSTICK_A) == 0) //LEFT
		{
			mp.leds[6] = CRGB::Blue;
			mp.leds[5] = CRGB::Blue;
			FastLED.show();
			mp.vibration(200);
			FastLED.clear();

			while (mp.buttons.kpd.pin_read(JOYSTICK_A) == 0);
			mp.display.drawRect(xstart + cursorX * xoffset, ystart + cursorY * yoffset, width + 2, height + 2, TFT_BLACK);
			if (cursorX == 0) {
				cursorX = xelements - 1;
			}
			else
				cursorX--;
			elapsedMillis = millis();
			cursorState = 1;
		}
		if (mp.buttons.kpd.pin_read(JOYSTICK_C) == 0)//RIGHT
		{
			mp.leds[1] = CRGB::Blue;
			mp.leds[2] = CRGB::Blue;
			FastLED.show();
			mp.vibration(200);
			FastLED.clear();

			while (mp.buttons.kpd.pin_read(JOYSTICK_C) == 0);
			mp.display.drawRect(xstart + cursorX * xoffset, ystart + cursorY * yoffset, width + 2, height + 2, TFT_BLACK);
			if (cursorX == xelements - 1) {
				cursorX = 0;
			}
			else
				cursorX++;
			elapsedMillis = millis();
			cursorState = 1;
		}
		if (mp.buttons.kpd.pin_read(BTN_B) == 0 && previousButtonState == 1)//LOCK BUTTON
		{
			mp.leds[0] = CRGB::Red;
			mp.leds[7] = CRGB::Red;
			FastLED.show();
			mp.vibration(200);
			FastLED.clear();

			Serial.println("entered lock screen");
			mp.lockScreen();
			Serial.println("exited lock screen");
		}
		mp.update();


	}
}
int8_t GUI::drawBigIconsCursor(uint8_t xoffset, uint8_t yoffset, uint8_t xelements, uint8_t yelements, uint8_t xstart, uint8_t ystart) {
	String passcode = "";
	uint8_t index = 0;
	uint8_t cursorX = 0;
	uint8_t cursorY = 0;
	uint32_t passcodeMillis = millis();
	long elapsedMillis = millis();
	long elapsedMillis2 = millis();
	uint8_t scale;
	if(mp.resolutionMode)
		scale = 1;
	else
		scale = 2;
	while(!mp.update());
	while (1)
	{
		
		mp.display.fillScreen(TFT_BLACK);

		mp.display.setTextSize(1);
		mp.display.setTextColor(TFT_WHITE);

		// Draw the icons
		mp.display.drawIcon(bigMessages, 2*scale, 9*scale, width, bigIconHeight, scale);
		mp.display.drawIcon(bigMedia, 28*scale, 9*scale, width, bigIconHeight, scale);
		mp.display.drawIcon(bigContacts, 54*scale, 9*scale, width, bigIconHeight, scale);
		mp.display.drawIcon(bigSettings, 2*scale, 37*scale, width, bigIconHeight, scale);
		mp.display.drawIcon(bigPhone, 28*scale, 37*scale, width, bigIconHeight, scale);
		mp.display.drawIcon(bigApps, 54*scale, 37*scale, width, bigIconHeight, scale);

		mp.display.fillRect(0, 0, 80*scale, 6*scale, TFT_BLACK);
		index = cursorY * xelements + cursorX;
		if(mp.resolutionMode)
		{
			mp.display.setFreeFont(TT1);
			mp.display.setCursor(0,5);
			mp.display.drawFastHLine(0, 6, BUF2WIDTH, TFT_WHITE);
		}
		else
		{
			mp.display.setTextFont(2);
			mp.display.setCursor(0,-2);
			mp.display.drawFastHLine(0, 14, BUF2WIDTH, TFT_WHITE);
		}
		mp.display.print(mp.titles[index]);

		if (millis() - elapsedMillis >= 250) {
			elapsedMillis = millis();
			cursorState = !cursorState;
		}
		if (millis() - elapsedMillis2 >= 100) {
			elapsedMillis2 = millis();
			previousButtonState = mp.kpd.pin_read(BTN_B);
		}
		if (millis() - passcodeMillis >= 1000)
			passcode = "";

		if (cursorState == 1)
		{
			// mp.display.drawRect((xstart + cursorX * xoffset)*scale, (ystart + cursorY * yoffset)*scale+1, (width + 2)*scale, (bigIconHeight + 2)*scale-1, TFT_RED);
			mp.display.drawRect((xstart + cursorX * xoffset), (ystart + cursorY * yoffset), (width + 1)*scale, (bigIconHeight + 1)*scale, TFT_RED);
			mp.display.drawRect((xstart + cursorX * xoffset)-1, (ystart + cursorY * yoffset)-1, (width + 2)*scale, (bigIconHeight + 2)*scale, TFT_RED);
		}
		else
		{
			// mp.display.drawRect((xstart + cursorX * xoffset)*scale, (ystart + cursorY * yoffset)*scale + 1, (width + 2)*scale, (bigIconHeight + 2)*scale-1, TFT_BLACK);
			mp.display.drawRect((xstart + cursorX * xoffset), (ystart + cursorY * yoffset), (width + 1)*scale, (bigIconHeight + 1)*scale, TFT_BLACK);
			mp.display.drawRect((xstart + cursorX * xoffset)-1, (ystart + cursorY * yoffset)-1, (width + 2)*scale, (bigIconHeight + 2)*scale, TFT_BLACK);
		}

		///////////////////////////////////////
		//////Checking for button input////////
		///////////////////////////////////////
		if (mp.buttons.released(BTN_A)) //CONFIRM
		{
			while (!mp.update());
			return cursorY * xelements + cursorX;  //returns index of selected icon

		}
		if (mp.buttons.pressed(JOYSTICK_B)) //DOWN
		{
			passcode += "DOWN";
			passcodeMillis = millis();
			mp.leds[0] = CRGB::Blue;
			mp.leds[7] = CRGB::Blue;
			while(!mp.update());
			mp.vibration(200);
			FastLED.clear();
			while (!mp.update());

			while (mp.buttons.kpd.pin_read(JOYSTICK_B) == 0);
			mp.display.drawRect((xstart + cursorX * xoffset), (ystart + cursorY * yoffset), (width + 2)*scale, (bigIconHeight + 2)*scale, TFT_BLACK);
			if (cursorY == 0) {
				cursorY = yelements - 1;
			}
			else
				cursorY--;
			elapsedMillis = millis();
			cursorState = 1;
		}
		if (mp.buttons.kpd.pin_read(JOYSTICK_D) == 0)//UP
		{
			passcode += "UP";
			passcodeMillis = millis();
			mp.leds[3] = CRGB::Blue;
			mp.leds[4] = CRGB::Blue;
			while (!mp.update());
			mp.vibration(200);
			FastLED.clear();
			while (!mp.update());

			while (mp.buttons.kpd.pin_read(JOYSTICK_D) == 0);
			mp.display.drawRect((xstart + cursorX * xoffset), (ystart + cursorY * yoffset), (width + 2)*scale, (bigIconHeight + 2)*scale, TFT_BLACK);
			if (cursorY == yelements - 1) {
				cursorY = 0;
			}
			else
				cursorY++;
			elapsedMillis = millis();
			cursorState = 1;
		}
		if (mp.buttons.kpd.pin_read(JOYSTICK_A) == 0) //LEFT
		{
			passcode += "LEFT";
			passcodeMillis = millis();
			mp.leds[6] = CRGB::Blue;
			mp.leds[5] = CRGB::Blue;
			while (!mp.update());
			mp.vibration(200);
			FastLED.clear();
			while (!mp.update());

			while (mp.buttons.kpd.pin_read(JOYSTICK_A) == 0);
			mp.display.drawRect((xstart + cursorX * xoffset), (ystart + cursorY * yoffset), (width + 2)*scale, (bigIconHeight + 2)*scale, TFT_BLACK);
			if (cursorX == 0) {
				cursorX = xelements - 1;
			}
			else
				cursorX--;
			elapsedMillis = millis();
			cursorState = 1;
		}
		if (mp.buttons.kpd.pin_read(JOYSTICK_C) == 0)//RIGHT
		{
			passcode += "RIGHT";
			passcodeMillis = millis();
			mp.leds[1] = CRGB::Blue;
			mp.leds[2] = CRGB::Blue;
			while (!mp.update());
			mp.vibration(200);
			FastLED.clear();
			while (!mp.update());

			while (mp.buttons.kpd.pin_read(JOYSTICK_C) == 0);
			mp.display.drawRect((xstart + cursorX * xoffset), (ystart + cursorY * yoffset), (width + 2)*scale, (bigIconHeight + 2)*scale, TFT_BLACK);
			if (cursorX == xelements - 1) {
				cursorX = 0;
			}
			else
				cursorX++;
			elapsedMillis = millis();
			cursorState = 1;
		}
		if (mp.buttons.released(BTN_B))//LOCK BUTTON
		{

			mp.leds[0] = CRGB::Red;
			mp.leds[7] = CRGB::Red;
			FastLED.show();
			mp.vibration(200);
			FastLED.clear();
			while(!mp.update());
			return -2;
		}
		if (passcode == "UPUPDOWNDOWNLEFTRIGHTLEFTRIGHT")
			return -3;
		mp.update();
	}
}
int16_t GUI::scrollingMainMenu()
{
	mp.listDirectories("/");
	uint16_t index = 0;
	uint8_t cursorX = 0;
	uint8_t cursorY = 0;
	uint8_t elements = 10 + mp.directoryCount; //5 default apps
	Serial.println(elements);
	delay(5);
	uint8_t x_elements = 3;
	uint8_t y_elements = ceil((float)elements/x_elements);
	
	uint8_t pageNumber;
	if(elements < 6)
		pageNumber = 0;
	else

		pageNumber = ceil((float)(elements - 6)/3);
	
	Serial.println(pageNumber);
	Serial.println(y_elements);
	delay(5);
	uint8_t pageIndex = 0;
	uint8_t cameraY = 0;
	String appNames[] = {"Clock", "Calculator", "Flashlight", "Calendar", "Invaders"};
	String passcode = "";
	uint32_t passcodeMillis = millis();
	long elapsedMillis = millis();
	long elapsedMillis2 = millis();
	while(!mp.update());
	while (1)
	{
		
		mp.display.fillScreen(TFT_BLACK);

		mp.display.setTextSize(1);
		mp.display.setTextColor(TFT_WHITE);

		// Draw the icons
		for (int i = 0; i < 6;i++)
		{
			uint8_t tempX = i%x_elements;
			uint8_t tempY = i/x_elements;
			switch (pageIndex * 3 + i)
			{
				case 0:
					// Serial.println(index);
					// Serial.println(i);
					// Serial.println(tempX);
					// Serial.println(tempY);
					// Serial.println("-------------");
					// delay(5);
					mp.display.drawIcon(bigMessages, 4 + tempX*52, 18 + tempY*56, width, bigIconHeight, 2);
					break;
				case 1:
					mp.display.drawIcon(bigMedia, 4 + tempX*52, 18 + tempY*56, width, bigIconHeight, 2);
					break;
				case 2:
					mp.display.drawIcon(bigContacts, 4 + tempX*52, 18 + tempY*56, width, bigIconHeight, 2);
					break;
				case 3:
					mp.display.drawIcon(bigSettings, 4 + tempX*52, 18 + tempY*56, width, bigIconHeight, 2);
					break;
				case 4:
					mp.display.drawIcon(bigPhone, 4 + tempX*52, 18 + tempY*56, width, bigIconHeight, 2);
					break;
				case 5:
					mp.display.drawIcon(bigApps, 4 + tempX*52, 18 + tempY*56, width, bigIconHeight, 2);
					break;
				case 6:
					mp.display.drawIcon(clock_icon, 4 + tempX*52, 18 + tempY*56, width, bigIconHeight, 2);
					break;
				case 7:
					mp.display.drawIcon(calculator_icon, 4 + tempX*52, 18 + tempY*56, width, bigIconHeight, 2);
					break;
				case 8:
					mp.display.drawIcon(flash_icon, 4 + tempX*52, 18 + tempY*56, width, bigIconHeight, 2);
					break;
				case 9:
					mp.display.drawIcon(calendar_icon, 4 + tempX*52, 18 + tempY*56, width, bigIconHeight, 2);
					break;
				// case 10:
				// 	mp.display.drawBmp("/Invaders/icon.bmp", 4 + tempX * 52, 18 + tempY * 56, 2);
				// 	break;
				default: 
					if(pageIndex * 3 + i < elements)
					{
						Serial.println(mp.directories[pageIndex * 3 + i - 10]);
						delay(5);
						mp.display.drawBmp(String("/" + mp.directories[pageIndex * 3 + i - 10] + "/icon.bmp"), 4 + tempX * 52, 18 + tempY * 56, 2);
					}
					break;
			}
		}

		mp.display.fillRect(0, 0, 160, 12, TFT_BLACK);
		while(cursorY*x_elements + cursorX >= elements)
			cursorX--;
	
		index = cursorY * x_elements + cursorX;
		// Serial.println(index);
		// Serial.println(pageIndex);
		// Serial.println("-----------------");
		// delay(5);
		mp.display.setTextFont(2);
		mp.display.setCursor(0,-2);
		mp.display.drawFastHLine(0, 14, mp.display.width(), TFT_WHITE);
		if(index < 10)
			mp.display.print(mp.titles[index]);
		else
			mp.display.print(mp.directories[index-10]);
		
		if (millis() - elapsedMillis >= 250) {
			elapsedMillis = millis();
			cursorState = !cursorState;
		}
		if (millis() - elapsedMillis2 >= 100) {
			elapsedMillis2 = millis();
			previousButtonState = mp.kpd.pin_read(BTN_B);
		}
		if (millis() - passcodeMillis >= 1000)
			passcode = "";

		// mp.display.drawIcon(bigMessages, 4 + cursorX*52, 18 + cursorY*56, width, bigIconHeight, 2);

		mp.display.drawRect(3 + cursorX * 52, 17 + (cameraY) * 56, 50, 54, cursorState ? TFT_RED : TFT_BLACK);
		mp.display.drawRect(2 + cursorX * 52, 16 + (cameraY) * 56, 52, 56, cursorState ? TFT_RED : TFT_BLACK);

		///////////////////////////////////////
		//////Checking for button input////////
		///////////////////////////////////////
		if (mp.buttons.released(BTN_A)) //CONFIRM
		{
			while (!mp.update());
			return cursorY * x_elements + cursorX;  //returns index of selected icon
		}
		if (mp.buttons.released(JOYSTICK_D)) //UP
		{
			passcode += "UP";
			passcodeMillis = millis();
			mp.leds[0] = CRGB::Blue;
			mp.leds[7] = CRGB::Blue;
			while(!mp.update());
			mp.vibration(200);
			FastLED.clear();
			while (!mp.update());

			if (cursorY == 0) 
			{
				cursorY = y_elements-1;
				
				if(pageNumber > 0)
				{
					cameraY = 1;
					pageIndex = pageNumber;
				}
			}
			else if(cameraY % 2 == 1)
			{
				cursorY--;
				cameraY = 0;
			}
			else if(cameraY % 2 == 0 && pageIndex > 0)
			{
				cameraY = 0;
				cursorY--;
				pageIndex--;
			}
			elapsedMillis = millis();
			cursorState = 1;
		}
		if (mp.buttons.released(JOYSTICK_B))//DOWN
		{
			passcode += "DOWN";
			passcodeMillis = millis();
			mp.leds[3] = CRGB::Blue;
			mp.leds[4] = CRGB::Blue;
			while (!mp.update());
			mp.vibration(200);
			FastLED.clear();
			while (!mp.update());

			if (cursorY == y_elements - 1) {
				cursorY = 0;
				pageIndex = 0;
				cameraY = 0;
			}
			else if(cameraY % 2 == 0)
			{
				cursorY++;
				cameraY++;
			}
			else if (cameraY % 2 == 1 && pageIndex < pageNumber)
			{
				cameraY = 1;
				cursorY++;
				pageIndex++;
			}
			
			elapsedMillis = millis();
			cursorState = 1;
		}
		if (mp.buttons.released(JOYSTICK_A)) //LEFT
		{
			passcode += "LEFT";
			passcodeMillis = millis();
			mp.leds[6] = CRGB::Blue;
			mp.leds[5] = CRGB::Blue;
			while (!mp.update());
			mp.vibration(200);
			FastLED.clear();
			while (!mp.update());

			if (cursorX == 0) {
				cursorX = x_elements - 1;
			}
			else
				cursorX--;
			elapsedMillis = millis();
			cursorState = 1;
		}
		if (mp.buttons.released(JOYSTICK_C))//RIGHT
		{
			passcode += "RIGHT";
			passcodeMillis = millis();
			mp.leds[1] = CRGB::Blue;
			mp.leds[2] = CRGB::Blue;
			while (!mp.update());
			mp.vibration(200);
			FastLED.clear();
			while (!mp.update());

			if (cursorX == x_elements - 1) {
				cursorX = 0;
			}
			else
				cursorX++;
			elapsedMillis = millis();
			cursorState = 1;
		}
		if (mp.buttons.released(BTN_B))//B BUTTON
		{

			mp.leds[0] = CRGB::Red;
			mp.leds[7] = CRGB::Red;
			FastLED.show();
			mp.vibration(200);
			FastLED.clear();
			while(!mp.update());
			return -2;
		}
		if (passcode == "UPUPDOWNDOWNLEFTRIGHTLEFTRIGHT")
			return -3;
		mp.update();

	}
}
void GUI::popup(String text, uint8_t duration) {
	popupText = text;
	popupTotalTime = popupTimeLeft = duration + 16;
}
void GUI::updatePopup() {
	if (!popupTimeLeft) {
		return;
	}
	uint8_t scale = mp.display.textsize;
	uint8_t yOffset = 0;
	if (popupTimeLeft >= popupTotalTime - 8) {
		yOffset = (8 - (popupTotalTime - popupTimeLeft))*scale;
	}
	if (popupTimeLeft < 8) {
		yOffset = (8 - popupTimeLeft)*scale;
	}

	mp.display.fillRect(0, BUFHEIGHT - (7 * scale) + yOffset,BUFWIDTH, 7 * scale, TFT_DARKGREY);
	mp.display.fillRect(0, BUFHEIGHT - (8 * scale) + yOffset, BUFWIDTH, scale, TFT_BLACK);
	mp.display.setCursor(1, BUFHEIGHT - (6 * scale) + yOffset, TFT_WHITE);
	mp.display.print(popupText);
	popupTimeLeft--;
}
