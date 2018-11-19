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
	pinMode(SIM800_DTR, OUTPUT);
	digitalWrite(SIM800_DTR, 0);
	pinMode(INTERRUPT_PIN, INPUT_PULLUP);
	esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, 0); //1 = High, 0 = Low
	//Initialize and start with the NeoPixels
	pixels.begin();
	Serial1.begin(9600, SERIAL_8N1, 17, 16);
	//Serial1.println(F("AT+CFUN=1,1"));
	//Serial1.println("AT+CMEE=2");
	//Serial1.println(F("AT+CPIN?"));
	if (splash == 1) {
		for (uint8_t i = 0; i < NUMPIXELS; i++) {
			for (uint8_t x = 0; x < 128; x += 2) {
				mp.pixels.setPixelColor(i, x, 0, 0);
				delay(1);
				mp.pixels.show(); // This sends the updated pixel color to the hardware.
			}
		}
		for (uint8_t i = 0; i < NUMPIXELS; i++) {
			for (uint8_t x = 128; x > 0; x -= 2) {
				mp.pixels.setPixelColor(i, x, 0, 0);
				delay(1);
				mp.pixels.show(); // This sends the updated pixel color to the hardware.M

			}
		}

	}	
	pixels.clear();
	for (uint8_t x = 0; x < NUMPIXELS;x++)
		pixels.setPixelColor(x, 0, 0, 0);

	/*for (uint8_t i = 0; i < NUMPIXELS; i++)
		mp.pixels.setPixelColor(i, hslBlack);
	delay(2);
	mp.pixels.show();*/

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

	applySettings();

	Serial.begin(115200);

	//display initialization
	tft.init();

	tft.invertDisplay(0);
	tft.setRotation(1);

	display.setColorDepth(8); // Set colour depth of Sprite to 8 (or 16) bits
	display.createSprite(BUFWIDTH, BUFHEIGHT); // Create the sprite and clear background to black
	display.setTextWrap(0);             //setRotation(1);
	display.setTextSize(1); // landscape

	buf.setColorDepth(8); // Set colour depth of Sprite to 8 (or 16) bits
	buf.createSprite(BUF2WIDTH, BUF2HEIGHT); // Create the sprite and clear background to black
	//buf2.setRotation(1);
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
	for (uint8_t i = 255; i > 0; i--) {
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

	updateTimeGSM();
	Serial1.println("AT+CMEE=2");
	Serial1.println(F("AT+CLVL=100"));
	Serial1.println(F("AT+CRSL=100"));
	Serial1.println(F("AT+CMIC=0,6"));
	Serial1.println(F("AT+CMGF=1"));
	Serial1.println(F("AT+CNMI=1,1,0,0,0"));
	Serial1.println(F("AT+CLTS=1")); //Enable local Timestamp mode (used for syncrhonising RTC with GSM time
	Serial1.println(F("AT+CPMS=\"SM\",\"SM\",\"SM\""));
	Serial1.println(F("AT+CLCC=1"));
	//  while(1){
	//    if(Serial1.available())
	//      Serial.println(Serial1.read());
	//  }
	//Serial1.println(F("AT&W")); //Save  all settings in SIM's non-volatile memory

	Serial.println("Serial1 up and running...");

}
bool MAKERphone::update() {
	bool pressed = 0;
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

	for (int y = 0; y < BUFHEIGHT; y++) {
		for (int x = 0; x < BUFWIDTH; x++) {
			buf.fillRect(x * 2, y * 2, 2, 2, display.readPixel(x, y));
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
			updateBuffer = "";
			Serial1.println("AT+CBC");
			if (simInserted)
			{
				Serial1.println("AT+CSQ");
				if (carrierName == "")
					Serial1.println("AT+CSPN?");
			}
			if (clockYear == 4 || clockYear == 80)
				Serial1.println("AT+CCLK?");
			refreshMillis = millis();
		}
		if (Serial1.available())
		{
			c = Serial1.read();
			updateBuffer += c;
			if (simInserted)
			{
				if (carrierName == "")
				{
					if (updateBuffer.indexOf("\n", updateBuffer.indexOf("+CSPN:")) != -1)
					{
						carrierName = updateBuffer.substring(updateBuffer.indexOf("\"", updateBuffer.indexOf("+CSPN:"))+1, updateBuffer.indexOf("\"", updateBuffer.indexOf("\"", updateBuffer.indexOf("+CSPN:"))+1));
					}
				}
				if (updateBuffer.indexOf("\n", updateBuffer.indexOf("+CSQ:")) != -1)
					signalStrength = updateBuffer.substring(updateBuffer.indexOf(" ", updateBuffer.indexOf("+CSQ:")) + 1, updateBuffer.indexOf(",", updateBuffer.indexOf(" ", updateBuffer.indexOf("+CSQ:")))).toInt();
			}
			if(clockYear == 4)
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
			if (updateBuffer.indexOf("\n", updateBuffer.indexOf("+CBC:")) != -1)
				batteryVoltage = updateBuffer.substring(updateBuffer.indexOf(",", updateBuffer.indexOf(",", updateBuffer.indexOf("+CBC:")) + 1) + 1, updateBuffer.indexOf("\n", updateBuffer.indexOf("+CBC:"))).toInt();
		}
	}
	///////////////////////////////////////////////
	if (millis() - lastFrameCount2 >= frameSpeed) {
		lastFrameCount2 = millis();
		
		buf.pushSprite(0, 0);
		buttons.update();
		gui.updatePopup();
		delay(1);
		pixels.show();
		pixels.clear();
		/*if (pixelUpdate == 0)
			pixelUpdate = 1;
		if (pixelUpdate == 1)
			pixelUpdate = 2;
		if (pixelUpdate == 2)
			pixelUpdate = 3;
		if (pixelUpdate == 3)
		{
			pixelUpdate = 0;
			
		}
			*/
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

	/*for (uint8_t i = 0; i < NUMPIXELS; i++)
		pixels.setPixelColor(i, hslBlack);
	pixels.show();*/

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

	pixels.clear();
	delay(2);
	pixels.show();
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
	Serial.begin(115200);
	bool goOut = 0;
	uint8_t updatePixels = 0;	
	uint32_t elapsedMillis = millis();
	//pixels.clear();
	//delay(1);
	//pixels.show();
	//pixels.clear();
	//delay(1);
	//pixels.show();
	while (1)
	{
		if (clockYear != 4 && clockYear != 80)
			updateTimeRTC();
		

		display.fillScreen(backgroundColors[backgroundIndex]);

		display.setFreeFont(TT1);
		display.setTextSize(3);//
		//Hour shadow
		display.setTextColor(TFT_DARKGREY);
		if (clockHour == 11)
			display.setCursor(10, 28);
		else if (clockHour % 10 == 1 || (int)clockHour / 10 == 1)
			display.setCursor(7, 28);
		else
			display.setCursor(4, 28);
		if (clockHour < 10)
			display.print("0");
		display.print(clockHour);

		//minute shadow
		display.setCursor(36, 28);
		if (clockMinute < 10)
			display.print("0");
		display.print(clockMinute);

		//Hour black
		display.setTextColor(TFT_BLACK);
		if (clockHour == 11)
			display.setCursor(9, 27);
		else if (clockHour % 10 == 1 || (int)clockHour / 10 == 1)
			display.setCursor(6, 27);
		else
			display.setCursor(3, 27);
		if (clockHour < 10)
			display.print("0");
		display.print(clockHour);

		//Minute black
		display.setCursor(35, 27);
		if (clockMinute < 10)
			display.print("0");
		display.print(clockMinute);

		display.setTextSize(1);
		display.setCursor(60, 19);
		display.setTextWrap(false);
		if (clockDay < 10)
			display.print("0");
		display.print(clockDay);
		display.print("/");
		if (clockMonth < 10)
			display.print("0");
		display.print(clockMonth);
		display.setCursor(62, 25);
		display.print(2000 + clockYear);
		/*display.setTextSize(2);
		  display.setCursor(10, 50);
		  display.print("12:00");*/
		uint8_t helper = 11;
		if (simInserted)
		{
			if (signalStrength <= 3)
				display.drawBitmap(1, 1, noSignalIcon);
			else if (signalStrength > 3 && signalStrength <= 10)
				display.drawBitmap(1, 1, signalLowIcon);
			else if (signalStrength > 10 && signalStrength <= 20)
				display.drawBitmap(1, 1, signalHighIcon);
			else if (signalStrength > 20 && signalStrength <= 31)
				display.drawBitmap(1, 1, signalFullIcon);
			else if (signalStrength == 99)
				display.drawBitmap(1, 1, signalErrorIcon);
		}
		else
			display.drawBitmap(1, 1, signalErrorIcon);
		if (volume == 0)
		{
			display.drawBitmap(helper, 1, silentmode);
			helper += 10;
		}
		//display.drawBitmap(31, 1, missedcall);
		//display.drawBitmap(41, 1, newtext);
		if (!airplaneMode)
		{
			if (wifi == 1)
				display.drawBitmap(helper, 1, wifion);
			else
				display.drawBitmap(helper, 1, wifioff);
			helper += 10;
			if (bt)
				display.drawBitmap(helper, 1, BTon);
			else
				display.drawBitmap(helper, 1, BToff);
		}
		else
		{
			display.drawBitmap(helper, 1, airplaneModeIcon);
			helper += 10;
		}
		if (batteryVoltage > 4000)
			display.drawBitmap(74, 1, batteryCharging);
		else if (batteryVoltage <= 4000 && batteryVoltage >= 3800)
			display.drawBitmap(74, 1, batteryFull);
		else if (batteryVoltage < 3800 && batteryVoltage >= 3700)
			display.drawBitmap(74, 1, batteryMid);
		else if (batteryVoltage < 3700 && batteryVoltage >= 3600)
			display.drawBitmap(74, 1, batteryMidLow);
		else if (batteryVoltage < 3600 && batteryVoltage >= 3500)
			display.drawBitmap(74, 1, batteryLow);
		else if (batteryVoltage < 3500)
			display.drawBitmap(74, 1, batteryEmpty);

		gui.drawNotificationWindow(2, 32, 77, 10, "Missed call from Dad");
		gui.drawNotificationWindow(2, 44, 77, 10, "Text from Jack");

		if (millis() - elapsedMillis >= 500) {
			elapsedMillis = millis();
			blinkState = !blinkState;
		}


		if (blinkState == 1)
		{
			display.setTextSize(1);
			display.setTextColor(TFT_BLACK);
			display.setCursor(1, 63);
			display.print("Hold \"A\" to unlock");
			display.setTextSize(3);
			display.setTextColor(TFT_DARKGREY);
			display.setCursor(29, 28);
			display.print(":");
			display.setTextColor(TFT_BLACK);
			display.setCursor(28, 27);
			display.print(":");
		}
		update();

		display.setTextSize(1);

		if (buttons.pressed(BTN_A)) {

			vibration(200);

			display.fillRect(0, 57, BUFWIDTH, 7, backgroundColors[backgroundIndex]);
			display.setCursor(1, 63);
			display.print("Unlocking");
			update();
			buttonHeld = millis();
			pixelState = 0;
			while (buttons.kpd.pin_read(BTN_A) == 0)
			{
				if (millis() - buttonHeld > 250 && millis() - buttonHeld < 500) {
					display.fillRect(0, 64, BUFWIDTH, 7, TFT_CYAN);
					display.setCursor(1, 63);
					display.print("Unlocking *");
					pixels.setPixelColor(0, hslRed);
					pixels.setPixelColor(7, hslRed);

					/*if (updatePixels == 0) {
						pixels.clear();
						
						delay(1);
						pixels.show();
						Serial.println("Pixels.show()");
						updatePixels = 1;
					}*/
				}
				else if (millis() - buttonHeld > 500 && millis() - buttonHeld < 750)
				{
					display.fillRect(0, 64, BUFWIDTH, 7, TFT_CYAN);
					display.setCursor(1, 63);
					display.print("Unlocking * *");
					pixels.setPixelColor(0, hslRed);
					pixels.setPixelColor(7, hslRed);
					pixels.setPixelColor(1, hslRed);
					pixels.setPixelColor(6, hslRed);
					/*if (updatePixels == 1) {
						
						delay(1);
						pixels.show();
						Serial.println("Pixels.show()");
						updatePixels = 2;
					}*/
				}
				else if (millis() - buttonHeld > 750 && millis() - buttonHeld < 1000)
				{
					display.fillRect(0, 64, BUFWIDTH, 7, TFT_CYAN);
					display.setCursor(1, 63);
					display.print("Unlocking * * *");
					pixels.setPixelColor(0, hslRed);
					pixels.setPixelColor(7, hslRed);
					pixels.setPixelColor(1, hslRed);
					pixels.setPixelColor(6, hslRed);
					pixels.setPixelColor(2, hslRed);
					pixels.setPixelColor(5, hslRed);
					/*if (updatePixels == 2) {
						
						delay(1);
						pixels.show();
						Serial.println("Pixels.show()");
						updatePixels = 3;
					}*/
				}
				else if (millis() - buttonHeld > 1000)
				{
					pixels.setPixelColor(0, hslRed);
					pixels.setPixelColor(7, hslRed);
					pixels.setPixelColor(1, hslRed);
					pixels.setPixelColor(6, hslRed);
					pixels.setPixelColor(2, hslRed);
					pixels.setPixelColor(5, hslRed);
					pixels.setPixelColor(3, hslRed);
					pixels.setPixelColor(4, hslRed);
					/*if (updatePixels == 3) {
						
						delay(1);
						pixels.show();
						Serial.println("Pixels.show()");
						updatePixels = 4;
					}*/
					update();

					vibration(200);
					Serial.println(millis() - buttonHeld);
					goOut = 1;
					delay(10);
					for (uint8_t i = 0; i < NUMPIXELS; i++)
						pixels.setPixelColor(i, hslBlack);
					delay(1);
					pixels.show();
					delay(10);
					break;
				}
				update();
			}
		}
		if (buttons.released(BTN_A))
		{
			mp.pixels.clear();
			delay(2);
			mp.pixels.show();

			updatePixels = 0;
		}
		if (buttons.pressed(BTN_B)) {
			while (buttons.kpd.pin_read(BTN_B) == 0);
			sleep();

		}

		if (buttons.pressed(JOYSTICK_B)) {
			while (buttons.kpd.pin_read(JOYSTICK_B) == 0);
			digitalWrite(SIM800_DTR, 1);
			Serial1.println(F("AT+CSCLK=1"));
		}

		if (goOut == 1)
			break;
	}
}
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

	//TO-DO: UPDATE THE RTC HERE

	buttons.kpd.setHour(clockHour);
	buttons.kpd.setMinute(clockMinute);
	buttons.kpd.setSecond(clockSecond);
	buttons.kpd.setDate(clockDay);
	buttons.kpd.setMonth(clockMonth);
	buttons.kpd.setYear(clockYear);
	Serial.println(F("\nRTC TIME UPDATE OVER GSM DONE!"));
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
void MAKERphone::listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
	Serial.begin(115200);
	binaryCount = 0;
	Serial.printf("Listing directory: %s\n", dirname);

	File root = fs.open(dirname);
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
		  listDir(fs, file.name(), levels - 1);
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
		String Name = file.name();
		if (Name.endsWith(F(".BIN")) || Name.endsWith(F(".bin")))
		{
			Serial.print(counter);
			Serial.print(".   ");
			Serial.println(file.name());
			BinaryFiles[counter - 1] = file.name();
			counter++;
			binaryCount++;
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
void MAKERphone::updateFromFS(fs::FS &fs, String FilePath) {
	Serial.println(FilePath);
	File updateBin = fs.open(FilePath);
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
void MAKERphone::mainMenu() {
	while (buttons.kpd.pin_read(BTN_A) == 0);
	Serial.println("entered main menu");

	uint8_t index = gui.drawCursor(width + 1, width + 1, 3, 2, 0, 7);

	if (titles[index] == "Apps")
	{
		display.fillScreen(TFT_BLACK);
		update();
		if (!SD.begin()) {
			display.setCursor(0, 0);
			display.println("\nCard Mount Failed");
			update();
			return;
		}
		listDir(SD, "/", 0);

		update();
		int8_t index = gui.menu("Load from SD", BinaryFiles, binaryCount);

		if (index != -1) {  //IF BUTTON "BACK" WAS NOT PRESSED
			display.fillScreen(TFT_BLACK);
			display.setCursor(0, 0);
			display.print("You picked:");
			display.println(BinaryFiles[index]);
			display.print("LOADING NOW...");
			update();
			delay(1000);
			if (!SD.begin()) {
				display.println("Card Mount Failed");
				return;
			}
			listDir(SD, "/", 0);
			updateFromFS(SD, BinaryFiles[index]);
		}


	}

	if (titles[index] == "Messages")
	{
		display.fillScreen(TFT_BLACK);
		display.setCursor(22, 30);
		display.print("Loading");
		display.setCursor(20, 36);
		display.print("messages...");
		update();
		messagesApp();
	}

	if (titles[index] == "Media")
		mediaApp();
	if (titles[index] == "Phone")
		phoneApp();
	if (titles[index] == "Contacts")
		contactsApp();
	if (titles[index] == "Settings")
	{
		settingsApp();
		Serial.println(brightness);
		applySettings();
		
	}



}
void MAKERphone::bigIconsMainMenu() {
	Serial.begin(115200);
	while (buttons.kpd.pin_read(BTN_A) == 0);
	Serial.println("entered main menu");
	Serial.flush();
	while (1)
	{

		//Serial.println(index);
		int8_t index = gui.drawBigIconsCursor(width + 2, bigIconHeight + 1, 3, 2, 1, 8);
		Serial.flush();
		delay(10);
		if (titles[index] == "Apps")
		{
			display.fillScreen(TFT_BLACK);
			update();
			if (!SD.begin()) {
				display.setCursor(0, 0);
				display.println("\nCard Mount Failed");
				update();
				return;
			}
			listDir(SD, "/", 0);

			update();
			int8_t index = gui.menu("Load from SD", BinaryFiles, binaryCount);

			if (index != -1) {  //IF BUTTON "BACK" WAS NOT PRESSED
				display.fillScreen(TFT_BLACK);
				display.setCursor(0, 0);
				display.print("You picked:");
				display.println(BinaryFiles[index]);
				display.print("LOADING NOW...");
				update();
				delay(1000);
				if (!SD.begin()) {
					display.println("Card Mount Failed");
					return;
				}
				listDir(SD, "/", 0);
				updateFromFS(SD, BinaryFiles[index]);
			}


		}

		if (titles[index] == "Messages")
		{
			display.fillScreen(TFT_BLACK);
			display.setCursor(22, 30);
			display.print("Loading");
			display.setCursor(20, 36);
			display.print("messages...");
			update();
			messagesApp();
		}

		if (titles[index] == "Media")
			mediaApp();
		if (titles[index] == "Phone")
			phoneApp();
		if (titles[index] == "Contacts") {
			contactsApp();
		}

		if (titles[index] == "Settings")
			settingsApp();
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
	Serial.begin(115200);
	String localBuffer = "";
	Serial1.print(F("ATD"));
	Serial1.print(number);
	Serial1.print(";\r\n");
	display.setFreeFont(TT1);
	display.setTextColor(TFT_BLACK);
	bool firstPass = 1;
	uint32_t timeOffset = 0;
	uint16_t textLength;
	display.setTextSize(1);
	//centering the on-screen number
	display.setCursor(11, -20);
	textLength = display.cursor_x;
	Serial.println(textLength);
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

				display.setCursor(32, 9);
				if ((int((millis() - timeOffset) / 1000) / 60) > 9)
					display.print(int((millis() - timeOffset) / 1000) / 60);
				else
				{
					display.print("0");
					display.print(int((millis() - timeOffset) / 1000) / 60);
				}
				display.print(":");
				if (int((millis() - timeOffset) / 1000) % 60 > 9)
					display.print(int((millis() - timeOffset) / 1000) % 60);
				else
				{
					display.print("0");
					display.print(int((millis() - timeOffset) / 1000) % 60);
				}
				Serial.println("CALL ACTIVE");
				display.drawBitmap(29, 24, call_icon, TFT_GREEN);
			}

			else if (localBuffer.indexOf(",0,3,") != -1)
			{
				display.setCursor(25, 9);
				Serial.println("ringing");
				display.println("Ringing...");
				display.drawBitmap(29, 24, call_icon, TFT_DARKGREY);
			}
			else if (localBuffer.indexOf(",0,2,") != -1)
			{
				display.setCursor(25, 9);
				display.println("Calling...");
				display.drawBitmap(29, 24, call_icon, TFT_DARKGREY);
			}
			else if (localBuffer.indexOf(",0,6,") != -1)
			{
				display.fillScreen(TFT_WHITE);
				display.setCursor(32, 9);
				if (timeOffset == 0)
					display.print("00:00");
				else
				{
					if ((int((millis() - timeOffset) / 1000) / 60) > 9)
						display.print(int((millis() - timeOffset) / 1000) / 60);
					else
					{
						display.print("0");
						display.print(int((millis() - timeOffset) / 1000) / 60);
					}
					display.print(":");
					if ((int((millis() - timeOffset) / 1000) % 60) > 9)
						display.print(int((millis() - timeOffset) / 1000) % 60);
					else
					{
						display.print("0");
						display.print(int((millis() - timeOffset) / 1000) % 60);
					}
				}
				display.drawBitmap(29, 24, call_icon, TFT_RED);
				display.setCursor(11, 20);
				display.println(number);
				display.fillRect(0, 51, 80, 13, TFT_RED);
				display.setCursor(2, 62);
				display.print("Call ended");
				Serial.println("ENDED");
				while (!update());
				delay(1000);
				break;
			}
			display.setCursor(int((80 - textLength) / 2), 20);
			display.printCenter(number);
			display.fillRect(0, 51, 80, 13, TFT_RED);
			display.setCursor(2, 62);
			display.print("press");
			display.drawBitmap(24, 52, letterB, TFT_BLACK);
			display.setCursor(35, 62);
			display.print("to hang up");

		}
		else if (localBuffer.indexOf("CLCC:") == -1)
		{
			if (localBuffer.indexOf("ERROR") != -1)
			{
				
				display.setCursor(3, 9);
				display.println("Couldn't dial number!");
				display.drawBitmap(29, 24, call_icon, TFT_RED);
				display.setCursor(int((80-textLength)/2), 20);
				display.printCenter(number);
				display.fillRect(0, 51, 80, 13, TFT_RED);
				display.setCursor(2, 57);
				display.print("Invalid number or");
				display.setCursor(2, 63);
				display.print("SIM card missing!");
				while (!buttons.released(BTN_B))
					update();
				break;
			}
			else
			{
				display.setCursor(25, 9);
				display.println("Calling...");
				display.drawBitmap(29, 24, call_icon, TFT_DARKGREY);
				display.setCursor(int((80 - textLength) / 2), 20);
				display.printCenter(number);
				display.fillRect(0, 51, 80, 13, TFT_RED);
				display.setCursor(2, 62);
				display.print("press");
				display.drawBitmap(24, 52, letterB, TFT_BLACK);
				display.setCursor(35, 62);
				display.print("to hang up");
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
				display.print("00:00");
			else
			{
				if ((int((millis() - timeOffset) / 1000) / 60) > 9)
					display.print(int((millis() - timeOffset) / 1000) / 60);
				else
				{
					display.print("0");
					display.print(int((millis() - timeOffset) / 1000) / 60);
				}
				display.print(":");
				if ((int((millis() - timeOffset) / 1000) % 60) > 9)
					display.print(int((millis() - timeOffset) / 1000) % 60);
				else
				{
					display.print("0");
					display.print(int((millis() - timeOffset) / 1000) % 60);
				}
			}
			display.drawBitmap(29, 24, call_icon, TFT_RED);
			display.setCursor(11, 20);
			display.printCenter(number);
			display.fillRect(0, 51, 80, 13, TFT_RED);
			display.setCursor(2, 62);
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
	if (input.indexOf("NOT READY", input.indexOf("+CPIN:")) != -1 || input.indexOf("ERROR") != -1 && input.indexOf("+CPIN:") == -1
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
String MAKERphone::textInput(String buffer)
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
	else if (key == '*')
		buffer += ' ';
	if (key != 'B' && key != 'D' && key != 'A')
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
		if (key == 'C')
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
	Serial.begin(115200);
	dataRefreshFlag = 0;
	//sim800.println("AT+CMGL=\"REC READ\"");
	//sim800.flush();
	/*while (sim800.available())
	  input += (char)sim800.read();*/

	input = readAllSms();
	while (input == "")
	{
		delay(100);
		input = readAllSms();
	}
	//Serial.println(input);
	if (input == "ERROR")
	{
		display.fillScreen(TFT_BLACK);
		display.setCursor(5, 34);
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
		display.setCursor(5, 34);
		display.setTextColor(TFT_WHITE);
		display.printCenter("No messages :(");
		while (!update());
		while (!buttons.released(BTN_A) && !buttons.released(BTN_B))
			update();
	}
	else
	{
		currentMessageNumber = countSubstring(input, "CMGL:");


		/////////////////////////////////////////////////////
		//parsing the raw data input for contact number,
		//date and text content
		////////////////////////////////////////////////////
		for (uint8_t i = 0; i < smsNumber; i++)
		{
			start = input.indexOf("\n", input.indexOf("CMGL:", end));
			end = input.indexOf("\n", start + 1);
			smsContent[i] = input.substring(start + 1, end);
		}


		end = 0;
		for (uint8_t i = 0; i < smsNumber; i++)
		{
			start = input.indexOf("D\",\"", input.indexOf("CMGL:", end));
			end = input.indexOf("\"", start + 4);
			phoneNumber[i] = input.substring(start + 4, end);
		}
		//////////////////////////////////////////
		//Parsing for date and time of sending
		////////////////////////////
		end = 0;
		for (uint8_t i = 0; i < smsNumber; i++)
		{
			start = input.indexOf("\"\",\"", input.indexOf("CMGL:", end));
			end = input.indexOf("\"", start + 4);
			tempDate[i] = input.substring(start + 4, end);
		}

		int8_t index = -8;
		for (uint8_t i = 0; i < smsNumber; i++)
		{

			char c1, c2; //buffer for saving date and time numerals in form of characters
			c1 = tempDate[i].charAt(index + 8);
			c2 = tempDate[i].charAt(index + 9);
			smsYear = 2000 + ((c1 - '0') * 10) + (c2 - '0');

			c1 = tempDate[i].charAt(index + 11);
			c2 = tempDate[i].charAt(index + 12);
			smsMonth = ((c1 - '0') * 10) + (c2 - '0');

			c1 = tempDate[i].charAt(index + 14);
			c2 = tempDate[i].charAt(index + 15);
			smsDay = ((c1 - '0') * 10) + (c2 - '0');

			c1 = tempDate[i].charAt(index + 17);
			c2 = tempDate[i].charAt(index + 18);
			smsHour = ((c1 - '0') * 10) + (c2 - '0');

			c1 = tempDate[i].charAt(index + 20);
			c2 = tempDate[i].charAt(index + 21);
			smsMinute = ((c1 - '0') * 10) + (c2 - '0');

			c1 = tempDate[i].charAt(index + 23);
			c2 = tempDate[i].charAt(index + 24);
			smsSecond = ((c1 - '0') * 10) + (c2 - '0');
		}

		while (1)
		{
			int16_t menuChoice = smsMenu("Messages", phoneNumber, tempDate, smsContent, smsNumber);
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
uint8_t MAKERphone::countSubstring(String string, String substring) {
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
	Serial.begin(115200);
	Serial1.print(F("AT+CMGL=\"ALL\"\r"));
	buffer = readSerial();
	Serial.println(buffer);
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
		uint16_t x = 0;
		//while (kpd.pin_read(3))
		//{

		display.setCursor(1, y);
		display.print(content);
		if (buttons.kpd.pin_read(JOYSTICK_D) == 0) { //BUTTON DOWN
			Serial.println(display.cursor_y);
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
			}
		}

		if (buttons.kpd.pin_read(JOYSTICK_B) == 0) { //BUTTON UP
			if (y < 14)
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
			display.fillRect(0, 0, display.width(), 7, TFT_DARKGREY);
			display.setTextColor(TFT_WHITE);
			display.setCursor(1, 6);
			display.print("From: ");
			display.print(contact);
			display.drawFastHLine(0, 7, LCDWIDTH, TFT_BLACK);
		}
		else
		{
			display.fillRect(0, 0, display.width(), 7, TFT_DARKGREY);
			display.setTextColor(TFT_WHITE);
			display.setCursor(1, 6);
			display.print(date);
			display.drawFastHLine(0, 7, LCDWIDTH, TFT_BLACK);
		}

		update();
	}
}
void MAKERphone::smsMenuDrawBox(String contact, String date, String content, uint8_t i, int32_t y) {
	
	y += (i-1) * 20 + composeBoxHeight + menuYOffset;
	if (y < 0 || y > BUFHEIGHT) {
		return;
	}
	display.fillRect(1, y + 1, BUFWIDTH - 2, 19, TFT_DARKGREY);
	display.setTextColor(TFT_WHITE);
	display.setCursor(2, y + 2);
	display.drawString(contact, 3, y + 2);
	display.drawString(date, 3, y + 8);
	display.drawString(content, 3, y + 14);

}
void MAKERphone::smsMenuDrawCursor(uint8_t i, int32_t y) {
	if (millis() % 500 <= 250) {
		return;
	}
	y += (i-1) * 20 + composeBoxHeight + menuYOffset;
	display.drawRect(0, y, display.width(), 21, TFT_RED);
}
int16_t MAKERphone::smsMenu(const char* title, String* contact, String *date, String *content, uint8_t length) {
	uint8_t cursor = 0;
	int32_t cameraY = 0;
	int32_t cameraY_actual = 0;

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
		display.fillRect(0, 0, display.width(), 7, TFT_DARKGREY);
		display.setTextColor(TFT_WHITE);
		display.setCursor(1, 1);
		display.drawString(title, 1, 1);
		display.drawFastHLine(0, 7, LCDWIDTH, TFT_BLACK);

		if (buttons.released(BTN_A)) {   //BUTTON CONFIRM

			while (!update());// Exit when pressed
			break;
		}

		if (buttons.kpd.pin_read(JOYSTICK_D) == 0) {  //BUTTON UP

			while (buttons.kpd.pin_read(JOYSTICK_D) == 0);
			if (cursor == 0) {
				cursor = length - 1;
				if (length > 2) {
					cameraY = -(cursor - 1) * 20;
				}
			}
			else {
				cursor--;
				if (cursor > 0 && (cursor * 20 + cameraY + menuYOffset) < 14) {
					cameraY += 20;
				}
			}
		}

		if (buttons.kpd.pin_read(JOYSTICK_B) == 0) { //BUTTON DOWN
			while (buttons.kpd.pin_read(JOYSTICK_B) == 0);
			cursor++;
			if (cursor > 0)
			{
				if (((cursor - 1) * 20 + composeBoxHeight + cameraY + menuYOffset) > 40) {
					cameraY -= 20;
				}
			}
			else
			{
				if ((cursor * 20 + cameraY + menuYOffset) > 40) {
					cameraY -= 20;
				}
			}
			if (cursor >= length) {
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
	if (millis() % 500 <= 250) {
		return;
	}
	y += menuYOffset;
	display.drawRect(0, y, display.width(), composeBoxHeight+1, TFT_RED);
}
void MAKERphone::smsMenuComposeBox(uint8_t i, int32_t y) {
	y += menuYOffset;
	if (y < 0 || y > BUFHEIGHT) {
		return;
	}
	display.fillRect(1, y + 1, BUFWIDTH - 2, composeBoxHeight-1, TFT_DARKGREY);
	display.drawBitmap(2, y + 2, composeIcon, TFT_WHITE);
	display.setTextColor(TFT_WHITE);
	display.setCursor(20, y + 3);
	display.setTextFont(1);
	display.print("New SMS");
	display.setFreeFont(TT1);

}
void MAKERphone::composeSMS()
{
	textPointer = 0;
	y = 10;  //Beggining point
	String content = "";
	String contact = "";
	String prevContent = "";
	char key = NO_KEY;
	bool cursor = 0; //editing contacts or text content
	uint16_t contentCursor = 0;
	unsigned long elapsedMillis = millis();
	bool blinkState = 1;
	while (1)
	{
		display.fillScreen(TFT_DARKGREY);
		display.fillRect(0, 0, display.width(), 7, TFT_DARKGREY);
		display.setTextColor(TFT_WHITE);
		display.setCursor(1, 6);
		display.print("To: ");
		
		display.drawFastHLine(0, 7, LCDWIDTH, TFT_BLACK);
		display.setFreeFont(TT1);
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
			display.setCursor(1, y);
			display.setTextFont(1);
			if (content == "")
			{
				display.setTextColor(TFT_LIGHTGREY);
				display.print(F("Compose..."));
				display.setTextColor(TFT_WHITE);
			}
			else
				display.print(content);
			display.setFreeFont(TT1);
			display.setCursor(13, 6);
			display.print(contact);
			if (blinkState == 1)
				display.drawFastVLine(display.getCursorX(), display.getCursorY()-5, 5, TFT_WHITE);
		}
		else
		{
			display.setTextColor(TFT_WHITE);
			display.setCursor(1, 6);
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
			display.setCursor(1, y);
			display.setTextFont(1);
			display.print(content);
			display.setFreeFont(TT1);
			if(blinkState == 1)
				display.drawFastVLine(display.getCursorX(), display.getCursorY(), 7, TFT_WHITE);

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
		if (buttons.released(BTN_A)) // SEND SMS
		{
			display.fillScreen(TFT_BLACK);
			display.setFreeFont(TT1);
			display.setCursor(34, 32);
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
void MAKERphone::contactsMenuDrawBox(String contact, String number, uint8_t i, int32_t y) {
	y += i * 14 + menuYOffset;
	if (y < 0 || y > BUFHEIGHT) {
		return;
	}
	display.setTextSize(1);
	display.fillRect(1, y + 1, BUFWIDTH - 2, 13, TFT_DARKGREY);
	display.setTextColor(TFT_WHITE);
	display.setCursor(2, y + 2);
	display.drawString(contact, 3, y + 2);
	display.drawString(number, 3, y + 8);

}
void MAKERphone::contactsMenuDrawCursor(uint8_t i, int32_t y) {
	if (millis() % 500 <= 250) {
		return;
	}
	y += i * 14 + menuYOffset;
	display.drawRect(0, y, display.width(), 15, TFT_RED);
}
int8_t MAKERphone::contactsMenu(const char* title, String* contact, String *number, uint8_t length) {

	uint8_t cursor = 0;
	int32_t cameraY = 0;
	int32_t cameraY_actual = 0;

	while (1) {
		while (!update());
		display.fillScreen(TFT_BLACK);
		display.setCursor(0, 0);
		cameraY_actual = (cameraY_actual + cameraY) / 2;
		if (cameraY_actual - cameraY == 1) {
			cameraY_actual = cameraY;
		}

		for (uint8_t i = 0; i < length; i++) {
			contactsMenuDrawBox(contact[i], number[i], i, cameraY_actual);
		}
		contactsMenuDrawCursor(cursor, cameraY_actual);

		// last draw the top entry thing
		display.fillRect(0, 0, display.width(), 7, TFT_DARKGREY);
		display.setTextColor(TFT_WHITE);
		display.setCursor(1, 1);
		display.drawString(title, 1, 1);
		display.drawFastHLine(0, 7, LCDWIDTH, TFT_BLACK);

		if (buttons.kpd.pin_read(BTN_A) == 0) {   //BUTTON CONFIRM

			while (buttons.kpd.pin_read(BTN_A) == 0);// Exit when pressed
			break;
		}

		if (buttons.kpd.pin_read(JOYSTICK_D) == 0) {  //BUTTON UP

			while (buttons.kpd.pin_read(JOYSTICK_D) == 0);
			if (cursor == 0) {
				cursor = length - 1;
				if (length > 2) {
					cameraY = -(cursor - 2) * 14;
				}
			}
			else {
				cursor--;
				if (cursor > 0 && (cursor * 14 + cameraY + menuYOffset) < 14) {
					cameraY += 14;
				}
			}
		}

		if (buttons.kpd.pin_read(JOYSTICK_B) == 0) { //BUTTON DOWN
			while (buttons.kpd.pin_read(JOYSTICK_B) == 0);

			cursor++;
			if ((cursor * 14 + cameraY + menuYOffset) > 48) {
				cameraY -= 14;
			}
			if (cursor >= length) {
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
	dataRefreshFlag = 0;
	String input = readAllContacts();
	while (input == "")
		input = readAllContacts();
	if (input.indexOf("CPBR:") == -1)
	{
		display.fillScreen(TFT_BLACK);
		display.setCursor(16, 35);
		display.setFreeFont(TT1);
		display.print("No contacts  :(");
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
		uint16_t start;
		uint16_t end = 0;
		/////////////////////////////////////////////////////
		//parsing the raw data input for contact number, 
		//date and text content
		////////////////////////////////////////////////////
		for (uint8_t i = 0; i < contactNumber; i++)
		{
			start = input.indexOf("\"", input.indexOf("CPBR:", end));
			end = input.indexOf("\"", start + 1);
			phoneNumber[i] = input.substring(start + 1, end);
			start = input.indexOf("\"", end + 1);
			end = input.indexOf("\"", start + 1);
			contactName[i] = input.substring(start + 1, end);
		}
		int8_t menuChoice = contactsMenu("Contacts", contactName, phoneNumber, contactNumber);
		if (menuChoice != -2)
		{
			callNumber(phoneNumber[menuChoice]);
			update();
			while (buttons.kpd.pin_read(BTN_B) == 1);
			Serial1.println("ATH");
		}
		update();
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
	
	while (1)
	{
		display.fillScreen(TFT_BLACK);
		display.fillRect(0, 42, BUFWIDTH, 14, TFT_DARKGREY);
		display.setTextSize(1);
		display.setCursor(1, 62);
		display.print("Press A to call");
		display.setCursor(0, 6);
		display.setTextColor(TFT_LIGHTGREY);
		display.print("Dialer");
		display.setTextColor(TFT_WHITE);


		key = buttons.kpdNum.getKey();
		if (key == 'A') //clear number
			callBuffer = "";
		else if (key == 'C')
			callBuffer.remove(callBuffer.length()-1);
		if (key != NO_KEY && key!= 'A' && key != 'C')
			callBuffer += key;
		display.setCursor(1, 53);
		display.setTextSize(2);
		display.print(callBuffer);

		if (display.cursor_x > BUFWIDTH)
		{
			display.fillRect(0, 42, BUFWIDTH, 14, TFT_DARKGREY);
			display.setCursor(BUFWIDTH - display.cursor_x, 53);
			display.print(callBuffer);
		}
		

		if (buttons.kpd.pin_read(BTN_A) == 0)//initate call
		{
			callNumber(callBuffer);
			while (!update());
			callBuffer = "";
		}
		if (buttons.released(BTN_B) == 1) //BACK BUTTON
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

	while (1) {
		while (!update());
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
		display.fillRect(0, 0, display.width(), 7, TFT_DARKGREY);
		display.setTextColor(TFT_WHITE);
		display.setCursor(1, 1);
		display.drawString(title, 1, 1);
		display.drawFastHLine(0, 7, LCDWIDTH, TFT_BLACK);

		if (buttons.kpd.pin_read(BTN_A) == 0) {   //BUTTON CONFIRM

			while (buttons.kpd.pin_read(BTN_A) == 0);// Exit when pressed
			break;
		}

		if (buttons.kpd.pin_read(JOYSTICK_D) == 0) {  //BUTTON DOWN

			while (buttons.kpd.pin_read(JOYSTICK_D) == 0);
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

		if (buttons.kpd.pin_read(JOYSTICK_B) == 0) { //BUTTON UP
			while (buttons.kpd.pin_read(JOYSTICK_B) == 0);

			cursor++;
			if ((cursor * 8 + cameraY + menuYOffset) > 54) {
				cameraY -= 8;
			}
			if (cursor >= length) {
				cursor = 0;
				cameraY = 0;

			}

		}
		if (buttons.kpd.pin_read(BTN_B) == 0) //BUTTON BACK
		{
			while (buttons.kpd.pin_read(BTN_B) == 0);
			return -1;
		}
	}
	return cursor;

}
void MAKERphone::listMP3(fs::FS &fs, const char * dirname, uint8_t levels) {
	mp3Count = 0;
	Serial.begin(115200);
	Serial.printf("Listing directory: %s\n", dirname);

	File root = fs.open(dirname);
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
		String Name = file.name();
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
void MAKERphone::mp3player(String songName) {
	Serial.begin(115200);
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
	}
}
void MAKERphone::mediaApp() {
	if (!SD.begin())
		Serial.println("SD card error");
	listMP3(SD, "/", 0);
	while (1)
	{
		int16_t index = mp3Menu("Select file to play:", mp3Files, mp3Count);
		if (index == -1)
			break;
		display.fillScreen(TFT_LIGHTGREY);
		mp3player(mp3Files[index]);
	}
}
void MAKERphone::MDCallback(void *cbData, const char *type, bool isUnicode, const char *string) {
	(void)cbData;
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
}

//Settings app
int8_t MAKERphone::settingsMenu(String* title, uint8_t length) {
	bool pressed = 0;
	uint8_t cursor = 0;
	int32_t cameraY = 0;
	int32_t cameraY_actual = 0;
	dataRefreshFlag = 0;
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

		// last draw the top entry thing
	//    display.fillRect(0, 0, display.width(), 7, TFT_DARKGREY);
	//    display.setTextColor(TFT_WHITE);
	//    display.setCursor(1, 1);
	//    display.drawString(title, 1, 1);
	//    display.drawFastHLine(0, 7, LCDWIDTH, TFT_BLACK);

		if (buttons.kpd.pin_read(JOYSTICK_B) == 1 && buttons.kpd.pin_read(JOYSTICK_D) == 1)
			pressed = 0;

		if (buttons.released(BTN_A)) {   //BUTTON CONFIRM

			while (!update());// Exit when pressed
			break;
		}

		if (buttons.kpd.pin_read(JOYSTICK_D) == 0) {  //BUTTON DOWN

			while (buttons.kpd.pin_read(JOYSTICK_D) == 0);
			if (cursor == 0) {
				cursor = length - 1;
				if (length > 5) {
					cameraY = -(cursor - 2) * 15;
				}
			}
			else {
				cursor--;
				if (cursor > 0 && (cursor * 15 + cameraY + settingsMenuYOffset) < 15) {
					cameraY += 15;
				}
			}
			pressed = 1;
		}

		if (buttons.kpd.pin_read(JOYSTICK_B) == 0) { //BUTTON UP
			while (buttons.kpd.pin_read(JOYSTICK_B) == 0);

			cursor++;
			if ((cursor * 15 + cameraY + settingsMenuYOffset) > 48) {
				cameraY -= 15;
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
void MAKERphone::settingsMenuDrawBox(String title, uint8_t i, int32_t y) {
	y += i * 15 + settingsMenuYOffset; // 2px padding on top bezel
	if (y < 0 || y > BUFHEIGHT) {
		return;
	}
	display.setTextFont(1);
	display.setTextSize(1);

	display.setTextColor(TFT_BLACK);
	if (title == "Network") //red
	{
		display.fillRect(1, y + 1, BUFWIDTH - 2, 14, 0xFB6D);
		display.drawBitmap(3, y + 2, network, 0x7800);
	}
	if (title == "Display") //green
	{
		display.fillRect(1, y + 1, BUFWIDTH - 2, 14, 0x8FEA);
		display.drawBitmap(3, y + 2, displayIcon, 0x0341);
	}
	if (title == "Sound")//blue
	{
		display.fillRect(1, y + 1, BUFWIDTH - 2, 14, 0xA7FF);
		display.drawBitmap(3, y + 2, soundIcon, 0x010F);
	}
	if (title == "Security")//purple
	{
		display.fillRect(1, y + 1, BUFWIDTH - 2, 14, 0xED1F);
		display.drawBitmap(3, y + 2, security, 0x600F);
	}
	display.drawString(title, 19, y + 4);
	display.setTextSize(1);
	display.setTextColor(TFT_WHITE);
	display.setFreeFont(TT1);
}
void MAKERphone::settingsMenuDrawCursor(uint8_t i, int32_t y, bool pressed) {
	if (millis() % 500 <= 250 && pressed == 0) {
		return;
	}
	y += i * 15 + settingsMenuYOffset;
	display.drawRect(0, y, display.width(), 16, TFT_RED);
}
void MAKERphone::settingsApp() {
	while (!update());
	Serial.begin(115200);
	while (1)
	{
		int8_t input = settingsMenu(settingsItems, 4);
		if (input == -1) //BUTTON BACK
			break;
		if (input == 0)
			networkMenu();
		if (input == 1)
			displayMenu();
		if (input == 2)
			soundMenu();
		if (input == 3)
			securityMenu();
	}
	applySettings();
}
void MAKERphone::networkMenu() {
	uint8_t cursor = 0;
	while (1)
	{
		display.setTextColor(TFT_BLACK);
		display.fillScreen(0xFB6D);
		display.setTextFont(1);
		display.setCursor(4, 10);
		display.print("Wifi");
		display.setCursor(38, 10);
		display.print("ON");
		display.setCursor(59, 10);
		display.print("OFF");
		display.setCursor(8, 29);
		display.print("BT");
		display.setCursor(38, 29);
		display.print("ON");
		display.setCursor(59, 29);
		display.print("OFF");
		display.setCursor(3, 44);
		display.print("Plane\n mode");
		display.setCursor(38, 48);
		display.print("ON");
		display.setCursor(59, 48);
		display.print("OFF");
		switch (cursor) {

		case 0:
			if (bt == 1)
				display.drawRect(35, 27, 17, 11, TFT_BLACK);
			else
				display.drawRect(57, 27, 21, 11, TFT_BLACK);

			if (airplaneMode == 1)
				display.drawRect(35, 46, 17, 11, TFT_BLACK);
			else
				display.drawRect(57, 46, 21, 11, TFT_BLACK);
			break;

		case 1:
			if (wifi == 1)
				display.drawRect(35, 8, 17, 11, TFT_BLACK);
			else
				display.drawRect(57, 8, 21, 11, TFT_BLACK);

			if (airplaneMode == 1)
				display.drawRect(35, 46, 17, 11, TFT_BLACK);
			else
				display.drawRect(57, 46, 21, 11, TFT_BLACK);
			break;

		case 2:
			if (wifi == 1)
				display.drawRect(35, 8, 17, 11, TFT_BLACK);
			else
				display.drawRect(57, 8, 21, 11, TFT_BLACK);

			if (bt == 1)
				display.drawRect(35, 27, 17, 11, TFT_BLACK);
			else
				display.drawRect(57, 27, 21, 11, TFT_BLACK);

		}




		if (cursor == 0)
		{
			if (millis() % 500 <= 250 && wifi == 1)
				display.drawRect(35, 8, 17, 11, TFT_BLACK);
			else if (millis() % 500 <= 250 && wifi == 0)
				display.drawRect(57, 8, 21, 11, TFT_BLACK);
			if (buttons.kpd.pin_read(JOYSTICK_A) == 0 && wifi == 0)
				wifi = !wifi;
			if (buttons.kpd.pin_read(JOYSTICK_C) == 0 && wifi == 1)
				wifi = !wifi;
		}
		if (cursor == 1)
		{
			if (millis() % 500 <= 250 && bt == 1)
				display.drawRect(35, 27, 17, 11, TFT_BLACK);
			else if (millis() % 500 <= 250 && bt == 0)
				display.drawRect(57, 27, 21, 11, TFT_BLACK);
			if (buttons.kpd.pin_read(JOYSTICK_A) == 0 && bt == 0)
				bt = !bt;
			if (buttons.kpd.pin_read(JOYSTICK_C) == 0 && bt == 1)
				bt = !bt;
		}
		if (cursor == 2)
		{
			if (millis() % 500 <= 250 && airplaneMode == 1)
				display.drawRect(35, 46, 17, 11, TFT_BLACK);
			else if (millis() % 500 <= 250 && airplaneMode == 0)
				display.drawRect(57, 46, 21, 11, TFT_BLACK);
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
			break;

		update();
	}
}
void MAKERphone::displayMenu() {
	display.setTextFont(1);
	display.setTextColor(TFT_BLACK);
	uint8_t sleepTimeBuffer = 0;
	uint16_t sleepTimeActualBuffer = 0;
	uint8_t cursor = 0;
	while (1)
	{
		display.setTextFont(1);
		display.fillScreen(0x8FEA);
		display.setCursor(9, 2);
		display.print("Brightness");
		display.drawBitmap(5, 14, noBrightness);
		display.drawRect(14, 15, 47, 4, TFT_BLACK);
		display.drawBitmap(65, 11, fullBrightness);
		display.drawRect(15, 16, brightness * 9, 2, TFT_BLACK);

		display.setCursor(10, 23);
		display.print("Sleep:");
		if (sleepTimeActualBuffer > 60)
		{
			display.print(sleepTimeActualBuffer / 60);
			display.print("min");
		}
		else
		{
			display.print(sleepTimeActualBuffer);
			display.print("s");
		}
		display.setTextFont(1);
		display.drawRect(14, 35, 47, 4, TFT_BLACK);
		display.drawRect(15, 36, sleepTimeBuffer* 9, 2, TFT_BLACK);
		display.setCursor(1, 33);
		display.print("0s");
		display.setCursor(62, 33);
		display.print("30m");

		display.setCursor(11, 43);
		display.print("Background");
		display.fillRect(16, 52, 48, 9, backgroundColors[backgroundIndex]);
		display.setCursor(18, 53);
		display.print(backgroundColorsNames[backgroundIndex]);
		display.drawBitmap(11, 53, arrowLeft);
		display.drawBitmap(65, 53, arrowRight);

		if (cursor == 0)
		{
			if (millis() % 1000 <= 500)
			{
				display.drawBitmap(5, 14, noBrightness);
				display.drawBitmap(65, 11, fullBrightness);
			}
			else
			{
				display.drawBitmap(5, 14, noBrightness, 0x8FEA);
				display.drawBitmap(65, 11, fullBrightness, 0x8FEA);
			}
			if (buttons.kpd.pin_read(JOYSTICK_A) == 0 && brightness != 0)
			{
				while (buttons.kpd.pin_read(JOYSTICK_A) == 0);
				brightness--;
			}
			if (buttons.kpd.pin_read(JOYSTICK_C) == 0 && brightness != 5)
			{
				while (buttons.kpd.pin_read(JOYSTICK_C) == 0);
				brightness++;
			}
		}
		if (cursor == 1)
		{
			if (millis() % 1000 <= 500)
			{
				display.setCursor(1, 33);
				display.print("0s");
				display.setCursor(62, 33);
				display.print("30m");
			}
			else
			{
				display.fillRect(1, 33, 12, 7, 0x8FEA);
				display.fillRect(62, 33, 17, 7, 0x8FEA);
			}
			if (buttons.kpd.pin_read(JOYSTICK_A) == 0 && sleepTimeBuffer!= 0)
			{
				while (buttons.kpd.pin_read(JOYSTICK_A) == 0);
				sleepTimeBuffer--;
			}
			if (buttons.kpd.pin_read(JOYSTICK_C) == 0 && sleepTimeBuffer!= 5)
			{
				while (buttons.kpd.pin_read(JOYSTICK_C) == 0);
				sleepTimeBuffer++;
			}
		}
		if (cursor == 2)
		{
			if (millis() % 1000 <= 500)
			{
				if (backgroundIndex == 0)
				{
					display.drawLine(65, 53, 65, 59, 0x8FEA);
					display.drawBitmap(11, 53, arrowLeft);
					display.drawBitmap(66, 53, arrowRight);
				}
				else if (backgroundIndex == 6)
				{
					display.drawLine(14, 53, 14, 59, 0x8FEA);
					display.drawBitmap(10, 53, arrowLeft);
					display.drawBitmap(65, 53, arrowRight);
				}
				else
				{
					display.drawLine(65, 53, 65, 59, 0x8FEA);
					display.drawLine(14, 53, 14, 59, 0x8FEA);
					display.drawBitmap(10, 53, arrowLeft);
					display.drawBitmap(66, 53, arrowRight);
				}
			}
			else
			{
				display.drawBitmap(11, 53, arrowLeft);
				display.drawBitmap(65, 53, arrowRight);
			}
			if (buttons.kpd.pin_read(JOYSTICK_A) == 0 && backgroundIndex != 0)
			{
				while (buttons.kpd.pin_read(JOYSTICK_A) == 0);
				backgroundIndex--;
			}
			if (buttons.kpd.pin_read(JOYSTICK_C) == 0 && backgroundIndex != 6)
			{
				while (buttons.kpd.pin_read(JOYSTICK_C) == 0);
				backgroundIndex++;
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
	SD.begin();
	listRingtones(SD, "/ringtones", 0);
	listNotifications(SD, "/notifications", 0);
	Serial.begin(115200);
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
		display.setTextFont(1);
		display.fillScreen(0xA7FF);
		display.setCursor(20, 2);
		display.print("Volume");
		display.drawBitmap(4, 10, noSound);
		display.drawRect(16, 12, 47, 4, TFT_BLACK);
		display.drawBitmap(67, 10, fullSound);
		display.drawRect(17, 13, volume * 3, 2, TFT_BLACK);
		display.setCursor(15, 19);
		display.print("Ringtone");
		display.drawRect(3, 28, 74, 11, TFT_BLACK);
		display.setCursor(6, 30);
		if (parsedRingtone.length() > 11)
		{
			display.print(parsedRingtone.substring(0, 10));
			display.print("..");
		}
		else
			display.print(parsedRingtone);

		display.setCursor(5, 42);
		display.print("Notification");
		display.drawRect(3, 51, 74, 11, TFT_BLACK);
		display.setCursor(6, 53);
		if (parsedNotification.length() > 11)
		{
			display.print(parsedNotification.substring(0, 10));
			display.print("..");
		}
		else
			display.print(parsedNotification);

		if (cursor == 0)
		{
			if (millis() % 1000 <= 500)
			{
				display.drawBitmap(4, 10, noSound);
				display.drawBitmap(67, 10, fullSound);
			}
			else
			{
				display.drawBitmap(4, 10, noSound, 0xA7FF);
				display.drawBitmap(67, 10, fullSound, 0xA7FF);
			}
			if (buttons.kpd.pin_read(JOYSTICK_A) == 0 && volume != 0)
			{
				while (buttons.kpd.pin_read(JOYSTICK_A) == 0);
				volume--;
			}
			if (buttons.kpd.pin_read(JOYSTICK_C) == 0 && volume != 15)
			{
				while (buttons.kpd.pin_read(JOYSTICK_C) == 0);
				volume++;
			}
		}
		if (cursor == 1)
		{
			if (millis() % 1000 <= 500)
				display.drawRect(3, 28, 74, 11, TFT_BLACK);
			else
				display.drawRect(3, 28, 74, 11, 0xA7FF);

			if (buttons.kpd.pin_read(BTN_A) == 0)
			{
				while (buttons.kpd.pin_read(BTN_A) == 0);
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
				display.drawRect(3, 51, 74, 11, TFT_BLACK);
			else
				display.drawRect(3, 51, 74, 11, 0xA7FF);

			if (buttons.kpd.pin_read(BTN_A) == 0)
			{
				while (buttons.kpd.pin_read(BTN_A) == 0);
				display.setFreeFont(TT1);
				i = mp3Menu("Select ringtone:", notificationFiles, notificationCount);
				display.setTextColor(TFT_BLACK);
				if (i >= 0)
					notification = notificationFiles[i];
				start = 0;
				while (notification.indexOf("/", start) != -1)
					start = notification.indexOf("/", start) + 1;
				parsedNotification = notification.substring(start, notification.indexOf("."));
			}
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
			break;
		update();
	}
}
void MAKERphone::listRingtones(fs::FS &fs, const char * dirname, uint8_t levels) {
	ringtoneCount = 0;
	Serial.begin(115200);
	Serial.printf("Listing directory: %s\n", dirname);

	File root = fs.open(dirname);
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
void MAKERphone::listNotifications(fs::FS &fs, const char * dirname, uint8_t levels) {
	notificationCount = 0;
	Serial.begin(115200);
	Serial.printf("Listing directory: %s\n", dirname);

	File root = fs.open(dirname);
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
	Serial.begin(115200);
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
		display.fillScreen(TFT_BLACK);
		display.setCursor(34, 34);
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
		display.setCursor(34, 30);
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
			display.setTextFont(1);
			display.setCursor(4, 10);
			display.print("PIN\n lock");
			display.setCursor(38, 14);
			display.print("ON");
			display.setCursor(59, 14);
			display.print("OFF");
			display.drawRect(3, 40, 74, 11, TFT_BLACK);


			if (pinLockBuffer == 1)
			{
				display.setTextColor(TFT_BLACK);
				display.drawRect(3, 40, 74, 11, TFT_BLACK);
			}
			else
			{
				display.setTextColor(TFT_DARKGREY);
				display.fillRect(3, 40, 74, 11, TFT_DARKGREY);
			}
			display.setCursor(4, 31);
			display.print("PIN:");
			display.setCursor(6, 42);
			if (pinBuffer != "")
				display.printCenter(pinBuffer);
			else if (pinBuffer == "" && cursor != 1)
				display.printCenter("****");
			if (pinLockBuffer == 1 && cursor != 0)
				display.drawRect(35, 12, 17, 11, TFT_BLACK);
			else if (pinLockBuffer == 0 && cursor != 0)
				display.drawRect(57, 12, 21, 11, TFT_BLACK);
			display.setCursor(2, 61);
			display.setFreeFont(TT1);
			if (cursor == 1 && !errorMessage && !confirmMessage)
				display.print("Press A to save PIN");
			display.setTextFont(1);
			if (millis() - blinkMillis >= multi_tap_threshold) //cursor blinking routine 
			{
				blinkMillis = millis();
				blinkState = !blinkState;
			}

			if (cursor == 0)
			{
				if (millis() % 500 <= 250 && pinLockBuffer == 1)
					display.drawRect(35, 12, 17, 11, TFT_BLACK);
				else if (millis() % 500 <= 250 && pinLockBuffer == 0)
					display.drawRect(57, 12, 21, 11, TFT_BLACK);
				if (buttons.released(JOYSTICK_A) && pinLockBuffer == 0)
					pinLockBuffer = !pinLockBuffer;
				if (buttons.released(JOYSTICK_C) && pinLockBuffer == 1)
				{
					pinBuffer = "";
					pinLockBuffer = !pinLockBuffer;
					if (pinLock)
						while (1)
						{
							display.setTextFont(1);
							display.setTextColor(TFT_WHITE);
							display.fillScreen(TFT_BLACK);
							display.setCursor(5, 5);
							display.printCenter("Enter pin:");
							display.setFreeFont(TT1);
							display.setCursor(5, 19);
							String temp = "Remaining attempts: ";
							temp += timesRemaining;
							display.printCenter(temp);
							display.setTextFont(1);
							display.setCursor(1, 30);
							display.printCenter(pinBuffer);
							display.setCursor(1, 63);
							display.setFreeFont(TT1);
							display.print("Press A to confirm");
							display.setCursor(5, 19);

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
								Serial1.print(F("AT+CLCK=\"SC\", 0, \""));
								Serial1.print(pinBuffer);
								Serial1.println("\"");
								while (!Serial1.available());
								while (reply.indexOf("OK", reply.indexOf("AT+CLCK")) == -1 && reply.indexOf("ERROR", reply.indexOf("AT+CLCK")) == -1)
									reply = Serial1.readString();
								display.fillScreen(TFT_BLACK);
								display.setCursor(28, 28);
								display.setTextFont(1);
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
				display.setCursor(6, 42);
				display.printCenter(pinBuffer);
				if (blinkState == 1)
					display.drawFastVLine(display.getCursorX(), display.getCursorY(), 7, TFT_BLACK);
				if (buttons.released(BTN_A) && pinBuffer.length() == 4 && confirmMessage == 0)
				{

					while (!update());
					pinNumber = pinBuffer.toInt();

					reply = "";

					display.fillScreen(0xED1F);
					display.setTextFont(1);

					while (1)
					{
						display.setTextFont(1);
						display.setTextColor(TFT_WHITE);
						display.fillScreen(TFT_BLACK);
						display.setCursor(5, 5);
						display.printCenter("Old PIN:");
						display.setFreeFont(TT1);
						display.setCursor(5, 19);
						String temp = "Remaining attempts: ";
						temp += timesRemaining;
						display.printCenter(temp);
						display.setTextFont(1);
						display.setCursor(1, 30);
						display.printCenter(oldPin);
						display.setCursor(1, 63);
						display.setFreeFont(TT1);
						display.print("Press A to confirm");
						display.setCursor(5, 19);

						key = buttons.kpdNum.getKey();
						if (key == 'A') //clear number
							oldPin = "";
						else if (key == 'C' && oldPin != "")
							oldPin.remove(oldPin.length() - 1);
						if (key != NO_KEY && isDigit(key) && oldPin.length() != 4)
							oldPin += key;

						if (buttons.released(BTN_A))//enter PIN
						{
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
								display.setCursor(28, 28);
								display.setTextFont(1);
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
									display.setCursor(28, 28);
									display.setTextFont(1);
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
									display.setTextFont(1);
									display.setCursor(30, 30);
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
						display.setCursor(2, 61);
						display.setFreeFont(TT1);
						display.fillRect(2, 57, 78, 5, 0xED1F);
						display.print("PIN saved!");
						display.setTextFont(1);
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
				
				if (buttons.kpd.pin_read(BTN_A) == 0 && pinBuffer.length() < 4 && errorMessage == 0)
				{
					while (buttons.kpd.pin_read(BTN_A) == 0);
					display.setCursor(2, 61);
					display.setFreeFont(TT1);
					display.print("Pin must have 4+ digits");
					display.setTextFont(1);
					elapsedMillis = millis();
					errorMessage = 1;
					confirmMessage = 0;
				}
				if (millis() - elapsedMillis >= 2000 && errorMessage == 1)
				{
					display.fillRect(2, 57, 78, 5, 0xED1F);
					errorMessage = 0;
				}
				else if (millis() - elapsedMillis < 2000 && errorMessage == 1)
				{
					display.fillRect(2, 57, 78, 5, 0xED1F);
					display.setCursor(2, 61);
					display.setFreeFont(TT1);
					display.print("Pin must have 4+ digits");
					display.setTextFont(1);
				}
				if (millis() - elapsedMillis >= 2000 && confirmMessage == 1)
				{
					display.fillRect(2, 57, 78, 5, 0xED1F);
					confirmMessage = 0;
				}
				else if (millis() - elapsedMillis < 2000 && confirmMessage == 1)
				{
					display.fillRect(2, 56, 78, 6, 0xED1F);
					display.setCursor(2, 61);
					display.setFreeFont(TT1);
					display.print("PIN saved!");
					display.setTextFont(1);
				}
			}

			if (buttons.kpd.pin_read(JOYSTICK_D) == 0)
			{
				while (buttons.kpd.pin_read(JOYSTICK_D) == 0);
				if (cursor == 0 && pinLockBuffer == 1)
					cursor = 1;
				else if (pinLockBuffer == 1 && cursor == 1)
					cursor--;
			}
			if (buttons.kpd.pin_read(JOYSTICK_B) == 0)
			{
				while (buttons.kpd.pin_read(JOYSTICK_B) == 0);
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
void MAKERphone::applySettings()
{
	
	const char * path = "/settings.mph";
	char helper[20] = "";
	writeFile(path, "MAKERphone settings file\n");
	appendFile(path, "Wifi: ");
	switch (wifi)
	{
	case 0:
		//WiFi.disconnect(true); delay(1); // disable WIFI altogether
		//WiFi.mode(WIFI_MODE_NULL); delay(1);
		appendFile(path, "0\n");
		break;

	case 1:
		//WiFi.begin();
		//delay(1);
		appendFile(path, "1\n");
		break;
	}
	appendFile(path, "Bluetooth: ");
	switch (bt)
	{
	case 0:
		appendFile(path, "0\n");
		//btStop();
		break;
	case 1:
		appendFile(path, "1\n");
		//btStart();
		break;
	}
	appendFile(path, "Airplane mode: ");
	switch (airplaneMode)
	{
	case 0:
		appendFile(path, "0\n");
		Serial1.println("AT+CFUN=1");
		break;

	case 1:
		appendFile(path, "1\n");
		Serial1.println("AT+CFUN=4");
		break;
	}
	appendFile(path, "Brightness: ");
	itoa(brightness, helper, 10);
	appendFile(path, helper);
	if (brightness == 0)
		actualBrightness = 230;
	else
		actualBrightness = (5 - brightness) * 51;
	ledcAnalogWrite(LEDC_CHANNEL, actualBrightness);
	appendFile(path, "\nSleep time: ");
	itoa(sleepTime, helper, 10);
	appendFile(path, helper);
	appendFile(path, "\nBackground color: ");
	itoa(backgroundIndex, helper, 10);
	appendFile(path, helper);
	Serial.println(readFile(path));
	delay(5);
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

	byte buttonsData;

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
	while (!SD.begin());
	Serial.printf("Writing file: %s\n", path);

	File file = SD.open(path, FILE_WRITE);
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

	File file = SD.open(path, FILE_APPEND);
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
	while (!SD.begin());
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
	mp.display.fillRoundRect(x - 1, y + 1, width, height, 1, TFT_DARKGREY);
	mp.display.fillRoundRect(x, y, width, height, 1, TFT_WHITE);
	mp.display.setCursor(x + 3, y + 3 + 5);
	mp.display.print(text);
}
void GUI::menuDrawBox(String text, uint8_t i, int32_t y) {
	y += i * 8 + menuYOffset;
	if (y < 0 || y > BUFHEIGHT) {
		return;
	}
	mp.display.fillRect(1, y + 1, BUFWIDTH - 2, 7, TFT_DARKGREY);
	mp.display.setTextColor(TFT_WHITE);
	mp.display.setCursor(2, y + 2);
	mp.display.drawString(text, 3, y + 2);
}
void GUI::menuDrawCursor(uint8_t i, int32_t y) {
	if (millis() % 500 <= 250) {
		return;
	}
	y += i * 8 + menuYOffset;
	mp.display.drawRect(0, y, mp.display.width(), 9, TFT_RED);
}
int8_t GUI::menu(const char* title, String* items, uint8_t length) {
	long elapsedMillis = millis();
	long elapsedMillis2 = millis();
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
		mp.display.fillRect(0, 0, mp.display.width(), 7, TFT_DARKGREY);
		mp.display.setTextColor(TFT_WHITE);
		mp.display.setCursor(1, 1);
		mp.display.drawString(title, 1, 1);
		mp.display.drawFastHLine(0, 7, LCDWIDTH, TFT_BLACK);

		if (mp.buttons.kpd.pin_read(BTN_A) == 0) {   //BUTTON CONFIRM

			while (mp.buttons.kpd.pin_read(BTN_A) == 0);// Exit when pressed
			break;
		}
		if (mp.buttons.kpd.pin_read(BTN_B) == 0) {   //BUTTON BACK

			while (mp.buttons.kpd.pin_read(BTN_B) == 0);// Exit when pressed
			return -1;
		}

		if (mp.buttons.kpd.pin_read(JOYSTICK_D) == 0) {  //BUTTON DOWN
			mp.pixels.setPixelColor(3, mp.hslBlue);
			mp.pixels.setPixelColor(4, mp.hslBlue);
			while(!mp.update());
			mp.vibration(200);
			for (uint8_t i = 0; i < NUMPIXELS; i++)
				mp.pixels.setPixelColor(i, mp.hslBlack);
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
			mp.pixels.setPixelColor(0, mp.hslBlue);
			mp.pixels.setPixelColor(7, mp.hslBlue);
			while (!mp.update());
			mp.vibration(200);
			for (uint8_t i = 0; i < NUMPIXELS; i++)
				mp.pixels.setPixelColor(i, mp.hslBlack);
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
	Serial.begin(115200);
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

			mp.pixels.setPixelColor(0, mp.hslBlue);
			mp.pixels.setPixelColor(7, mp.hslBlue);
			mp.pixels.show();
			mp.vibration(200);
			for (uint8_t i = 0; i < NUMPIXELS; i++)
				mp.pixels.setPixelColor(i, mp.hslBlack);
			mp.pixels.show();

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

			mp.pixels.setPixelColor(3, mp.hslBlue);
			mp.pixels.setPixelColor(4, mp.hslBlue);
			mp.pixels.show();
			mp.vibration(200);
			for (uint8_t i = 0; i < NUMPIXELS; i++)
				mp.pixels.setPixelColor(i, mp.hslBlack);
			mp.pixels.show();

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
			mp.pixels.setPixelColor(6, mp.hslBlue);
			mp.pixels.setPixelColor(5, mp.hslBlue);
			mp.pixels.show();
			mp.vibration(200);
			for (uint8_t i = 0; i < NUMPIXELS; i++)
				mp.pixels.setPixelColor(i, mp.hslBlack);
			mp.pixels.show();

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
			mp.pixels.setPixelColor(1, mp.hslBlue);
			mp.pixels.setPixelColor(2, mp.hslBlue);
			mp.pixels.show();
			mp.vibration(200);
			for (uint8_t i = 0; i < NUMPIXELS; i++)
				mp.pixels.setPixelColor(i, mp.hslBlack);
			mp.pixels.show();

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
			mp.pixels.setPixelColor(0, mp.hslRed);
			mp.pixels.setPixelColor(7, mp.hslRed);
			mp.pixels.show();
			mp.vibration(200);
			for (uint8_t i = 0; i < NUMPIXELS; i++)
				mp.pixels.setPixelColor(i, mp.hslBlack);
			mp.pixels.show();

			Serial.println("entered lock screen");
			mp.lockScreen();
			Serial.println("exited lock screen");
		}
		mp.update();


	}
}
int8_t GUI::drawBigIconsCursor(uint8_t xoffset, uint8_t yoffset, uint8_t xelements, uint8_t yelements, uint8_t xstart, uint8_t ystart) {
	String passcode = "";
	uint32_t passcodeMillis = millis();
	Serial.begin(115200);
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
		mp.display.drawIcon(bigMessages, 2, 9, width, bigIconHeight);
		mp.display.drawIcon(bigMedia, 28, 9, width, bigIconHeight);
		mp.display.drawIcon(bigContacts, 54, 9, width, bigIconHeight);
		mp.display.drawIcon(bigSettings, 2, 36, width, bigIconHeight);
		mp.display.drawIcon(bigPhone, 28, 36, width, bigIconHeight);
		mp.display.drawIcon(bigApps, 54, 36, width, bigIconHeight);

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
		if (millis() - passcodeMillis >= 1000)
			passcode = "";

		if (cursorState == 1)
			mp.display.drawRect(xstart + cursorX * xoffset, ystart + cursorY * yoffset, width + 2, bigIconHeight + 2, TFT_RED);
		else
			mp.display.drawRect(xstart + cursorX * xoffset, ystart + cursorY * yoffset, width + 2, bigIconHeight + 2, TFT_BLACK);

		///////////////////////////////
		//////Checking for button input
		///////////////////////////////
		if (mp.buttons.kpd.pin_read(BTN_A) == 0) //CONFIRM
		{
			while (mp.buttons.kpd.pin_read(BTN_A) == 0);
			return cursorY * xelements + cursorX;  //returns index of selected icon

		}
		if (mp.buttons.pressed(JOYSTICK_B)) //DOWN
		{
			passcode += "DOWN";
			passcodeMillis = millis();
			mp.pixels.setPixelColor(0, mp.hslBlue);
			mp.pixels.setPixelColor(7, mp.hslBlue);
			while(!mp.update());
			mp.vibration(200);
			for (uint8_t i = 0; i < NUMPIXELS; i++)
				mp.pixels.setPixelColor(i, mp.hslBlack);
			while (!mp.update());

			while (mp.buttons.kpd.pin_read(JOYSTICK_B) == 0);
			mp.display.drawRect(xstart + cursorX * xoffset, ystart + cursorY * yoffset, width + 2, bigIconHeight + 2, TFT_BLACK);
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
			mp.pixels.setPixelColor(3, mp.hslBlue);
			mp.pixels.setPixelColor(4, mp.hslBlue);
			while (!mp.update());
			mp.vibration(200);
			for (uint8_t i = 0; i < NUMPIXELS; i++)
				mp.pixels.setPixelColor(i, mp.hslBlack);
			while (!mp.update());

			while (mp.buttons.kpd.pin_read(JOYSTICK_D) == 0);
			mp.display.drawRect(xstart + cursorX * xoffset, ystart + cursorY * yoffset, width + 2, bigIconHeight + 2, TFT_BLACK);
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
			mp.pixels.setPixelColor(6, mp.hslBlue);
			mp.pixels.setPixelColor(5, mp.hslBlue);
			while (!mp.update());
			mp.vibration(200);
			for (uint8_t i = 0; i < NUMPIXELS; i++)
				mp.pixels.setPixelColor(i, mp.hslBlack);
			while (!mp.update());

			while (mp.buttons.kpd.pin_read(JOYSTICK_A) == 0);
			mp.display.drawRect(xstart + cursorX * xoffset, ystart + cursorY * yoffset, width + 2, bigIconHeight + 2, TFT_BLACK);
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
			mp.pixels.setPixelColor(1, mp.hslBlue);
			mp.pixels.setPixelColor(2, mp.hslBlue);
			mp.pixels.show();
			while (!mp.update());
			mp.vibration(200);
			for (uint8_t i = 0; i < NUMPIXELS; i++)
				mp.pixels.setPixelColor(i, mp.hslBlack);
			while (!mp.update());

			while (mp.buttons.kpd.pin_read(JOYSTICK_C) == 0);
			mp.display.drawRect(xstart + cursorX * xoffset, ystart + cursorY * yoffset, width + 2, bigIconHeight + 2, TFT_BLACK);
			if (cursorX == xelements - 1) {
				cursorX = 0;
			}
			else
				cursorX++;
			elapsedMillis = millis();
			cursorState = 1;
		}
		if (mp.buttons.states[BTN_B] == 1)//LOCK BUTTON
		{

			mp.pixels.setPixelColor(0, mp.hslRed);
			mp.pixels.setPixelColor(7, mp.hslRed);
			mp.pixels.show();
			mp.vibration(200);

			for (uint8_t i = 0; i < NUMPIXELS; i++)
				mp.pixels.setPixelColor(i, mp.hslBlack);
			mp.pixels.show();

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

