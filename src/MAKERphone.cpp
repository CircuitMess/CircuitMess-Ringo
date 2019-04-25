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

//audio refresh task
TaskHandle_t Task1;
void Task1code( void * pvParameters ){
	while (true)
		updateWav();
}

//core
void MAKERphone::begin(bool splash) {
	SDFileSystemClass SD = _SD;

	String input="";
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
				leds[i] = CRGB(x, 0, 0);
				delay(1);
				FastLED.show();
			}
		}
	}
	FastLED.clear();
	buttons.begin();

	//Startup sounds
	// tone2(soundPin, 2000, 10);
	// buttons.kpd.writeMute(0);
	// buttons.kpd.writeVolume(0);
	//kpd.writeVolumeRight(78);

	//PWM SETUP FOR ESP32
	ledcSetup(0, 2000, 8);
	ledcAttachPin(soundPin, 0);
	ledcSetup(LEDC_CHANNEL, LEDC_BASE_FREQ, LEDC_TIMER);
	ledcAttachPin(LCD_BL_PIN, LEDC_CHANNEL);
	ledcAnalogWrite(LEDC_CHANNEL, 255);
	uint32_t tempMillis = millis();
	SDinsertedFlag = 1;
	while (!_SD.begin(5, SPI, 8000000))
	{
		Serial.println("SD ERROR");
		if(millis()-tempMillis > 5)
		{
			SDinsertedFlag = 0;
			break;
		}
	}

	if(SDinsertedFlag)
	{
		loadSettings(1);
		loadNotifications();
	}

	// osc->setVolume(150);
	//display initialization
	tft.init();
	tft.invertDisplay(0);
	tft.setRotation(1);
	display.setColorDepth(8); // Set colour depth of Sprite to 8 (or 16) bits
	display.createSprite(BUFWIDTH, BUFHEIGHT); // Create the sprite and clear background to black
	display.setTextWrap(0);             //setRotation(1);
	display.setTextSize(1); // landscape

	// buf.createSprite(BUF2WIDTH, BUF2HEIGHT); // Create the sprite and clear background to black
	// buf.setTextSize(1);
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

	updateTimeGSM();
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
	Serial.println("Serial1 up and running...");

	//Audio
	initWavLib();
	xTaskCreatePinnedToCore(
				Task1code,				/* Task function. */
				"Task1",				/* name of task. */
				10000,					/* Stack size of task */
				NULL,					/* parameter of the task */
				1,						/* priority of the task */
				&Task1,
				0);				/* Task handle to keep track of created task */
	addOscillator(osc);
	// osc->setADSR(10,20,0.8,10);
	if(SDinsertedFlag)
		ringtone = new MPTrack((char *)(ringtone_path.c_str()));
	applySettings();
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
	 //DacAudio.FillBuffer(); // Fill the sound buffer with data
	char c;
	uint16_t refreshInterval = 3000;
	if(screenshotFlag)
	{
		screenshotFlag = 0;
		takeScreenshot();
		homePopup(0);
	}
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
	// if(resolutionMode == 1)
	// {
	// 	for (int y = 0; y < BUFHEIGHT; y++) {
	// 		for (int x = 0; x < BUFWIDTH; x++) {
	// 			buf.fillRect(x * 2, y * 2, 2, 2, display.readPixel(x, y));
	// 		}
	// 	}
	// }
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

	//refreshing signal and battery info
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
	updateNotificationSound();
	if (millis() - lastFrameCount >= frameSpeed) {
		lastFrameCount = millis();
		updatePopup();

		if(resolutionMode == 0) //native res mode
			display.pushSprite(0, 0);

		// else//halved res mode
			// buf.pushSprite(0,0);

		buttons.update();

		if(HOME_POPUP_ENABLE && !inHomePopup)
		{
			if(buttons.kpdNum.getKey() == 'B')
			{
				inHomePopup = 1;
				homePopup();
				inHomePopup = 0;
			}
		}
		FastLED.setBrightness((float)(255/5*pixelsBrightness));
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
void MAKERphone::loader()
{
	display.fillScreen(TFT_BLACK);
	display.setCursor(0, display.height() / 2 - 12);
	display.setTextColor(TFT_WHITE);
	display.setTextFont(2);
	display.setTextSize(1);
	display.printCenter("LOADING...");
	while(!mp.update());
	const esp_partition_t* partition;
	partition = esp_ota_get_running_partition();
	const esp_partition_t* partition2;
	partition2 = esp_ota_get_next_update_partition(partition);
	esp_ota_set_boot_partition(partition2);
	ESP.restart();
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
	while(!SDFAT.begin(5, SD_SCK_MHZ(8)))
		Serial.println("SdFat error");
	File updateBin = SDFAT.open(FilePath);
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
void MAKERphone::incomingCall() //TODO
{
	if(!SDinsertedFlag)
	{
		bool state = 0;
		uint8_t tempNotification = notification;
		uint32_t callMillis = millis();
		while(!buttons.released(BTN_B))
		{
			if(millis() - callMillis >= 1000)
			{
				state = 1;
				callMillis = millis();
			}
			if(state)
			{
				playNotificationSound(4);
				state = 0;
			}
			update();
		}
		notification = tempNotification;
	}
	else
	{
		ringtone = new MPTrack((char *)ringtone_path.c_str());
		addTrack(ringtone);
		ringtone->setVolume(256 * volume / 14);
		ringtone->setRepeat(1);
		ringtone->play();
		while(!buttons.released(BTN_B))
			update();
		ringtone->stop();
	}
	while(!update());
	
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
			enterPUK();
	}
}
void MAKERphone::enterPin()
{
	char key = NO_KEY;
	String pinBuffer = "";
	String reply = "";
	uint32_t color = TFT_BLACK;
	if(backgroundColors[backgroundIndex] == TFT_BLACK)
		color = TFT_WHITE;
	while (reply.indexOf("+SPIC:") == -1)
	{
		Serial1.println("AT+SPIC");
		reply = Serial1.readString();
	}
	uint8_t foo = reply.indexOf(" ", reply.indexOf("+SPIC:"));
	timesRemaining = reply.substring(foo, reply.indexOf(",", foo)).toInt();

	while (1)
	{
		display.setTextColor(color);
		display.setTextSize(1);
		display.setTextFont(2);
		display.fillScreen(backgroundColors[backgroundIndex]);
		display.setCursor(5, 7);
		display.printCenter("Enter PIN:");
		display.drawRect(14, 45, 134, 30, color);
		display.drawRect(13, 44, 136, 32, color);
		display.setCursor(47, 52);
		display.printCenter(pinBuffer);

		display.setCursor(1, 112);
		display.print("Erase");
		display.setCursor(110, 112);
		display.print("Confirm");
		display.setCursor(5, 85);
		String temp = "Remaining attempts: ";
		temp+=timesRemaining;
		display.printCenter(temp);

		key = buttons.kpdNum.getKey();
		if (key == 'C' && pinBuffer != "")
			pinBuffer.remove(pinBuffer.length() - 1);
		else if (key != NO_KEY && isDigit(key) && pinBuffer.length() != 4)
			pinBuffer += key;

		if ((buttons.released(BTN_A) || key == 'A') && pinBuffer.length() == 4)//enter PIN
		{
			reply = "";
			Serial1.print(F("AT+CPIN=\""));
			Serial1.print(pinBuffer);
			Serial1.println("\"");
			while (!Serial1.available());
			while (reply.indexOf("+CPIN: READY") == -1 && reply.indexOf("ERROR") == -1)
				reply= Serial1.readString();
			display.fillScreen(backgroundColors[backgroundIndex]);
			display.setCursor(28, 52);
			display.setTextFont(2);
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
				if(timesRemaining == 0)
				{
					enterPUK();
					break;
				}
			}
		}
		if (buttons.released(BTN_B)) //sleeps
			sleep();
		update();
	}
}
void MAKERphone::enterPUK()
{
	char key = NO_KEY;
	String pukBuffer = "";
	String reply = "";
	uint32_t color = TFT_BLACK;
	if(backgroundColors[backgroundIndex] == TFT_BLACK)
		color = TFT_WHITE;
	while (reply.indexOf("+SPIC:") == -1)
	{
		Serial1.println("AT+SPIC");
		reply = Serial1.readString();
	}
	uint8_t foo = reply.indexOf(",", reply.indexOf("+SPIC:")) + 3;
	timesRemaining = reply.substring(foo, reply.indexOf(",", foo + 1)).toInt();
	Serial.println(reply);
	Serial.println(reply.substring(foo));
	delay(5);
	while (1)
	{
		display.setTextColor(color);
		display.setTextSize(1);
		display.setTextFont(2);
		display.fillScreen(backgroundColors[backgroundIndex]);
		display.setCursor(5, 7);
		display.printCenter("Enter PUK:");
		display.drawRect(14, 45, 134, 30, color);
		display.drawRect(13, 44, 136, 32, color);
		display.setCursor(47, 52);
		display.printCenter(pukBuffer);

		display.setCursor(1, 112);
		display.print("Erase");
		display.setCursor(110, 112);
		display.print("Confirm");
		display.setCursor(5, 85);
		String temp = "Remaining attempts: ";
		temp+=timesRemaining;
		display.printCenter(temp);

		key = buttons.kpdNum.getKey();
		if (key == 'C' && pukBuffer != "")
			pukBuffer.remove(pukBuffer.length() - 1);
		else if (key != NO_KEY && isDigit(key) && pukBuffer.length() != 8)
			pukBuffer += key;

		if ((buttons.released(BTN_A) || key == 'A') && pukBuffer.length() == 8)//enter PUK
		{
			reply = "";
			Serial1.print(F("AT+CPIN=\""));
			Serial1.print(pukBuffer);
			Serial1.println("\", \"0000\"");
			while (!Serial1.available());
			while (reply.indexOf("+CPIN: READY") == -1 && reply.indexOf("ERROR") == -1)
				reply= Serial1.readString();
			Serial.println(reply);
			delay(5);
			display.fillScreen(backgroundColors[backgroundIndex]);
			display.setCursor(28, 52);
			display.setTextFont(2);
			if (reply.indexOf("+CPIN: READY") != -1)
			{
				display.printCenter("PUK OK :)");
				while (!update());
				delay(1000);
				String pinBuffer = "";
				while (1)
				{
					display.setTextColor(color);
					display.setTextSize(1);
					display.setTextFont(2);
					display.fillScreen(backgroundColors[backgroundIndex]);
					display.setCursor(5, 7);
					display.printCenter("Enter new PIN:");
					display.drawRect(14, 45, 134, 30, color);
					display.drawRect(13, 44, 136, 32, color);
					display.setCursor(47, 52);
					display.printCenter(pinBuffer);
					display.setCursor(1, 112);
					display.print("Erase");
					display.setCursor(110, 112);
					display.print("Confirm");


					key = buttons.kpdNum.getKey();
					if (key == 'C' && pinBuffer != "")
						pinBuffer.remove(pinBuffer.length() - 1);
					else if (key != NO_KEY && isDigit(key) && pinBuffer.length() != 4)
						pinBuffer += key;

					if ((buttons.released(BTN_A) || key == 'A') && pinBuffer.length() == 4)//enter PIN
					{
						reply = "";
						Serial1.print(F("AT+CPWD= \"SC\",\"0000\", \""));
						Serial1.print(pinBuffer);
						Serial1.println("\"");
						while (!Serial1.available());
						while (reply.indexOf("OK", reply.indexOf("AT+CPWD")) == -1)
							reply = Serial1.readString();
						break;
					}
					if (buttons.released(BTN_B)) //sleeps
						sleep();
					update();
				}

				break;
			}
			else if (reply.indexOf("ERROR") != -1)
			{
				timesRemaining--;
				pukBuffer = "";
				display.printCenter("Wrong PUK :(");
				while (!update());
				delay(2000);
			}
		}
		if (buttons.released(BTN_B)) //sleeps
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
	if(textLimitFlag && buffer.length() == length)
		return buffer;
	else
		textLimitFlag = 0;
	if(length == -1 || length >= buffer.length()){
		if (key == '*')
			buffer += ' ';
		if (key != 'B' && key != 'D')
		{
			ret = multi_tap(key);// Feed the key press to the multi_tap function.
			if ((ret & 256) != 0) // If this is non-zero, we got a key. Handle some special keys or just print the key on screen
			{
				if(buffer.length() == length)
				{
					textLimitFlag = 1;
					return buffer;
				}
				else
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
	if(buffer.length() > length)
	{
		buffer = buffer.substring(0,length);
		textPointer--;
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


//JPEG drawing
void MAKERphone::drawJpeg(String filename, int xpos, int ypos) {

  Serial.println(F("==========================="));
  Serial.print(F("Drawing file: ")); Serial.println(filename);
  Serial.println(F("==========================="));

  // Open the named file (the Jpeg decoder library will close it after rendering image)
  SDAudioFile jpegFile = _SD.open( filename);    // SDAudioFile handle reference for SPIFFS
  //  SDAudioFile jpegSDAudioFile = _SD.open( filename, FILE_READ);  // or, file handle reference for SD library

  if ( !jpegFile ) {
    Serial.print(F("ERROR: SDAudioFile \"")); Serial.print(filename); Serial.println ("\" not found!");
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
    Serial.println(F("Jpeg file format not supported!"));
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

  Serial.println(F("==============="));
  Serial.println(F("JPEG image info"));
  Serial.println(F("==============="));
  Serial.print  (F("Width      :")); Serial.println(JpegDec.width);
  Serial.print  (F("Height     :")); Serial.println(JpegDec.height);
  Serial.print  (F("Components :")); Serial.println(JpegDec.comps);
  Serial.print  (F("MCU / row  :")); Serial.println(JpegDec.MCUSPerRow);
  Serial.print  (F("MCU / col  :")); Serial.println(JpegDec.MCUSPerCol);
  Serial.print  (F("Scan type  :")); Serial.println(JpegDec.scanType);
  Serial.print  (F("MCU width  :")); Serial.println(JpegDec.MCUWidth);
  Serial.print  (F("MCU height :")); Serial.println(JpegDec.MCUHeight);
  Serial.println(F("==============="));
  Serial.println();
}


//settings operations
void MAKERphone::saveSettings(bool debug)
{
	const char * path = "/.core/settings.json";
	Serial.println("");
	_SD.remove(path);
	JsonObject& settings = mp.jb.createObject();

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
		settings["notification"] = notification;
		settings["ringtone"] = ringtone_path;
		SDAudioFile file1 = _SD.open(path, "w");
		settings.prettyPrintTo(file1);
		file1.close();
	} else {
		Serial.println("Error saving new settings");
	}
}
void MAKERphone::loadSettings(bool debug)
{
	//create default folders if not present
	if(!_SD.exists("/Music"))
		_SD.mkdir("/Music");
	if(!_SD.exists("/Images"))
		_SD.mkdir("/Images");
	if(!_SD.exists("/Video"))
		_SD.mkdir("/Video");
	if(!_SD.exists("/Ringtones"))
		_SD.mkdir("/Ringtones");
	if(!_SD.exists("/.core"))
		_SD.mkdir("/.core");
	const char * path = "/.core/settings.json";
	Serial.println("");
	SDAudioFile file = _SD.open(path);
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
		notification = settings["notification"];
		ringtone_path = String(settings["ringtone"].as<char*>());
	} else {
		Serial.println("Error loading new settings");
		saveSettings();
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
	osc->setVolume(256 * volume / 14);

}

//save manipulation
JsonArray& MAKERphone::getJSONfromSAV(const char *path)
{
	String temp = readFile(path);
	JsonArray& json = mp.jb.parseArray(temp);
	return json;
}
void MAKERphone::saveJSONtoSAV(const char *path, JsonArray& json)
{
	while(!_SD.begin(5, SPI, 9000000))
        Serial.println("SD ERROR");
	SDAudioFile file = _SD.open(path, "w");
	if(file)
		json.prettyPrintTo(file);
	else
		Serial.println("SD ERROR");

	file.close();
}

//Collision
bool MAKERphone::collideRectRect(int16_t x1, int16_t y1, int16_t w1, int16_t h1, int16_t x2, int16_t y2, int16_t w2, int16_t h2) {
	return (x2 < x1 + w1 && x2 + w2 > x1 && y2 < y1 + h1 && y2 + h2 > y1);
}
bool MAKERphone::collidePointRect(int16_t pointX, int16_t pointY, uint16_t rectX, uint16_t rectY, uint16_t rectW, uint16_t rectH) {
	return (pointX >= rectX && pointX < rectX + rectW && pointY >= rectY && pointY < rectY + rectH);
}
bool MAKERphone::collideCircleCircle(int16_t centerX1, int16_t centerY1, int16_t r1, int16_t centerX2, int16_t centerY2, int16_t r2) {
	r1 = abs(r1);
	r2 = abs(r2);
	return (((centerX1 - centerX2) * (centerX1 - centerX2) + (centerY1 - centerY2) * (centerY1 - centerY2)) < (r1 + r2) * (r1 + r2));
}
bool MAKERphone::collidePointCircle(int16_t pointX, int16_t pointY, int16_t centerX, int16_t centerY, int16_t r) {
	return (((pointX - centerX) * (pointX - centerX) + (pointY - centerY) * (pointY - centerY)) < abs(r) ^ 2);
}

//SD operations
void MAKERphone::writeFile(const char * path, const char * message)
{
	while (!_SD.begin(5, SPI, 8000000));
	Serial.printf("Writing file: %s\n", path);

	SDAudioFile file = _SD.open(path);
	if (!file) {
		Serial.println("Failed to open file for writing");
		return;
	}
	if (file.print(message)) {
		Serial.println("SDAudioFile written");
	}
	else {
		Serial.println("Write failed");
	}
	file.close();
}
void MAKERphone::appendFile(const char * path, const char * message) {
	Serial.printf("Appending to file: %s\n", path);

	SDAudioFile file = _SD.open(path);
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
	while (!_SD.begin(5, SPI, 9000000));
	Serial.printf("Reading file: %s\n", path);
	String helper="";
	SDAudioFile file = _SD.open(path);
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
void MAKERphone::takeScreenshot()
{
	// display.setTextColor(TFT_BLACK);
	// display.setTextSize(1);
	// display.setTextFont(2);
	// display.drawRect(14, 45, 134, 38, TFT_BLACK);
	// display.drawRect(13, 44, 136, 40, TFT_BLACK);
	// display.fillRect(15, 46, 132, 36, 0xC59F);
	// display.setCursor(47, 55);
	// display.printCenter("Taking screenshot");
	while(!update());

	char name[] = "/Images/screenshot_00.bmp";
	while (!_SD.begin(5, SPI, 9000000))
		Serial.println("SD ERROR");
	for (int i = 0; i < 100;i++)
	{
		name[20] = i % 10 + '0';
		name[19] = i / 10 + '0';
		if(!_SD.exists(name))
			break;
	}
	Serial.println(name);
	delay(5);
	SDAudioFile file = _SD.open(name, "w");
	if(!file)
	{
		Serial.println("SD file error!");
		return;
	}

	uint8_t w = 160;
	uint8_t h = 128;
	int px[] = {255, 0, 255, 0, 255, 0
	 };
	bool debugPrint = 1;
	unsigned char *img = NULL;          // image data
	//  int filesize = 54 + 3 * w * h;      //  w is image width, h is image height
	int filesize = 54 + 4 * w * h;      //  w is image width, h is image height
	if (img) {
		free(img);
	}
	img = (unsigned char *)malloc(3*w);
	Serial.println(ESP.getFreeHeap());
	delay(5);
	memset(img,0,sizeof(img));        // not sure if I really need this; runs fine without...
	Serial.println(ESP.getFreeHeap());
	delay(5);
	// for (int y=0; y<h; y++) {
	// 	for (int x=0; x<w; x++) {
	// 		byte red, green, blue;
	// 		unsigned long rgb = display.readPixel(x, y);
	// 		red = rgb >> 16;
	// 		green = (rgb & 0x00ff00) >> 8;
	// 		blue = (rgb & 0x0000ff);
	// 		rgb = 0;
	// 		rgb |= red <<16;
	// 		rgb |= blue <<8;
	// 		rgb |=green;
	// 		img[(y*w + x)*3+2] = red;
	// 		img[(y*w + x)*3+1] = green;
	// 		img[(y*w + x)*3+0] = blue;
	// 		// Serial.printf("x: %d   y: %d\n", x, y);
	// 		// delay(5);
	// 		// int colorVal = px[y*w + x];
	// 		img[(y*w + x)*3+2] = (unsigned char)(red);
	// 		img[(y*w + x)*3+1] = (unsigned char)(green);
	// 		img[(y*w + x)*3+0] = (unsigned char)(blue);
	// 	}
	// }
	Serial.println("HERE");
	delay(5);
	// print px and img data for debugging
	// if (debugPrint) {
	// Serial.print("Writing \"");
	// Serial.print(name);
	// Serial.print("\" to file...\n");
	// for (int i=0; i<w*h; i++) {
	// 	Serial.print(px[i]);
	// 	Serial.print("  ");
	// }
	// }

	// create file headers (also taken from above example)
	unsigned char bmpFileHeader[14] = {
	'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0             };
	unsigned char bmpInfoHeader[40] = {
	40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0             };
	unsigned char bmpPad[3] = {
	0,0,0             };

	bmpFileHeader[ 2] = (unsigned char)(filesize    );
	bmpFileHeader[ 3] = (unsigned char)(filesize>> 8);
	bmpFileHeader[ 4] = (unsigned char)(filesize>>16);
	bmpFileHeader[ 5] = (unsigned char)(filesize>>24);

	bmpInfoHeader[ 4] = (unsigned char)(       w    );
	bmpInfoHeader[ 5] = (unsigned char)(       w>> 8);
	bmpInfoHeader[ 6] = (unsigned char)(       w>>16);
	bmpInfoHeader[ 7] = (unsigned char)(       w>>24);
	bmpInfoHeader[ 8] = (unsigned char)(       h    );
	bmpInfoHeader[ 9] = (unsigned char)(       h>> 8);
	bmpInfoHeader[10] = (unsigned char)(       h>>16);
	bmpInfoHeader[11] = (unsigned char)(       h>>24);

	// write the file (thanks forum!)
	file.write(bmpFileHeader, sizeof(bmpFileHeader));    // write file header
	file.write(bmpInfoHeader, sizeof(bmpInfoHeader));    // " info header

	for (int i=h; i>=0; i--) 			  // iterate image array
	{
		// memset(img,0,sizeof(img));        // not sure if I really need this; runs fine without...
		for (int x=0; x<w; x++)
		{
			// uint8_t rgb[3];
			uint16_t rgb = display.readPixelRGB(x, i);
			// Serial.println(rgb);
			// Serial.println(display.readPixelRGB(x, i)*257);
			// delay(5);
			// byte red, green, blue;
			// display.readPixelRGB(x, i, &rgb[0]);
			// display.readRectRGB(x, i, 1, 1, rgb);
			// uint8_t r = ((rgb >> 11) & 0x1F);
			// uint8_t g = ((rgb >> 5) & 0x3F);
			// uint8_t b = (rgb & 0x1F);

			uint8_t r = (rgb & 0xF800) >> 8;
			uint8_t g = (rgb & 0x07E0) >> 3;
			uint8_t b = (rgb & 0x1F) << 3;

			// r = (r * 255) / 31;
			// g = (g * 255) / 63;
			// b = (b * 255) / 31;



			//  r = rgb >> 16;
			//  g = (rgb & 0x00ff00) >> 8;
			//  b = (rgb & 0x0000ff);

			// Serial.println(r);
			// Serial.println(g);
			// Serial.println(b);

			// delay(1000);

			// rgb = 0;
			// rgb |= red <<16;
			// rgb |= blue <<8;
			// rgb |= green;
			// Serial.printf("x: %d   y: %d\n", x, y);
			// delay(5);
			// int colorVal = px[y*w + x];
			// img[x*3+2] = (unsigned char)(r);
			// img[x*3+1] = (unsigned char)(g);
			// img[x*3+0] = (unsigned char)(b);
			file.write(b);
			file.write(g);
			file.write(r);
			// file.write(rgb[0]);
			// Serial.println(rgb[0]);
			// file.write(rgb[1]);
			// Serial.println(rgb[1]);
			// file.write(rgb[2]);                // write px data
			// Serial.println(rgb[2]);
			// delay(5);
		}
		// file.write(img + (w * (h - i - 1) * 3), 3 * w);	// write px data
		// file.write(bmpPad, (4-(w*3)%4)%4);                 // and padding as needed
	}
	file.close();
	free(img);
	if (debugPrint) {
	Serial.println("\n---");
	}
	display.setTextColor(TFT_BLACK);
	display.setTextSize(1);
	display.setTextFont(2);
	display.drawRect(14, 45, 134, 38, TFT_BLACK);
	display.drawRect(13, 44, 136, 40, TFT_BLACK);
	display.fillRect(15, 46, 132, 36, 0xC59F);
	display.setCursor(47, 48);
	display.printCenter(&name[8]);
	display.setCursor(47, 61);
	display.printCenter("saved to SD!");
	uint32_t tempMillis = millis();
	while(millis() < tempMillis + 3000)
	{
		update();
		if(buttons.pressed(BTN_A) || buttons.pressed(BTN_B))
		{
			while(!buttons.released(BTN_A) && !buttons.released(BTN_B))
				update();
			break;
		}
	}
	while(!update());

}

//Popups
void MAKERphone::popup(String text, uint16_t duration) {
	popupText = text;
	popupTotalTime = popupTimeLeft = popupDuration + 16;
}

void MAKERphone::updatePopup() {
	if (!popupTimeLeft) {
		return;
	}
	uint8_t yOffset = 0;
	if (popupTimeLeft >= popupTotalTime - 18) {
		yOffset = (18 - (popupTotalTime - popupTimeLeft));
	}
	if (popupTimeLeft < 18) {
		yOffset = (18 - popupTimeLeft);
	}
	display.setTextFont(2);
	display.setTextSize(1);
	display.setTextColor(TFT_BLACK);
	display.fillRect(0, display.height() - 18 + yOffset, display.width(), 20, TFT_DARKGREY);
	display.fillRect(0, display.height() - 20 + yOffset, display.width(), 2, TFT_BLACK);
	display.setCursor(2, display.height() - 17 + yOffset);
	display.print(popupText);
	popupTimeLeft--;
}
void MAKERphone::homePopup(bool animation)
{
	dataRefreshFlag = 1;
	if(animation)
	{
		for (int i = 0; i < display.height(); i+=1)
		{
			display.drawFastHLine(0, i, display.width(), TFT_WHITE);
			update();
			delayMicroseconds(750);
		}
	}
	display.fillScreen(TFT_WHITE);
	display.drawIcon(popupVolume,12,25,20,20,2);
	display.drawIcon(popupExit,60,25,20,20,2);
	display.drawIcon(popupScreenBrightness,108,25,20,20,2);
	display.drawIcon(popupScreenshot,12,70,20,20,2);
	display.drawIcon(popupPixelBrightness,108,70,20,20,2);
	uint8_t cursor = 1;
	uint8_t scale = 2;
	uint32_t blinkMillis = millis();
	uint32_t notificationMillis = millis();
	uint8_t cursorState = 0;
	dataRefreshFlag = 1;
	String temp;
	while(!buttons.released(BTN_B))
	{
		display.fillScreen(TFT_WHITE);
		display.drawIcon(popupVolume,12,25,20,20,2);
		display.drawIcon(popupExit,60,25,20,20,2);
		display.drawIcon(popupScreenBrightness,108,25,20,20,2);
		display.drawIcon(popupScreenshot,12,70,20,20,2);
		display.drawIcon(popupPixelBrightness,108,70,20,20,2);
		display.fillRect(0,0, 160,18, TFT_WHITE);
		display.fillRect(0,114, 160,20, TFT_WHITE);


		//drawing the top icons
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


		if (millis() - blinkMillis >= 250) {
			blinkMillis = millis();
			cursorState = !cursorState;
		}
		display.setTextFont(2);
		display.setTextSize(1);
		display.setCursor(0,112);
		display.setTextColor(TFT_BLACK);
		if(millis() - notificationMillis >= 2000)
			notificationMillis = millis();

		if(millis() - notificationMillis > 1000 && millis() - notificationMillis < 2000)
		{
			display.printCenter("Notifications   ");
			display.drawBitmap(display.cursor_x - 10, display.cursor_y, arrowDown, TFT_BLACK, 2);
		}
		else
			display.printCenter(popupHomeItems[cursor]);
		display.drawRect(12 + cursor % 3 * 48 - 1, 25 + 45 * (int)(cursor / 3) - 1, 42, 42, cursorState ? TFT_RED : TFT_WHITE);
		display.drawRect(12 + cursor % 3 * 48 - 2, 25 + 45 * (int)(cursor / 3) - 2, 44, 44, cursorState ? TFT_RED : TFT_WHITE);

		// date and time
		updateTimeRTC();
		display.fillRect(60,70,40,40,0x963F);
		display.setFreeFont(TT1);
		display.setTextSize(2);
		display.setCursor(63, 85);
		temp = "";
		if (clockHour < 10)
			temp.concat("0");
		temp.concat(clockHour);
		temp.concat(":");
		if (clockMinute < 10)
			temp.concat("0");
		temp.concat(clockMinute);
		display.printCenter(temp);
		display.setCursor(63, 105);
		temp = "";
		if (clockDay < 10)
			temp.concat("0");
		temp.concat(clockDay);
		temp.concat("/");
		if (clockMonth < 10)
			temp.concat("0");
		temp.concat(clockMonth);
		display.printCenter(temp);



		if(buttons.released(BTN_UP))
		{
			osc->note(75, 0.05);
			osc->play();
			display.drawRect(12 + cursor % 3 * 48 - 2, 25 + 45 * (int)(cursor / 3) - 2, 44, 44,TFT_WHITE);
			display.drawRect(12 + cursor % 3 * 48 - 1, 25 + 45 * (int)(cursor / 3) - 1, 42, 42,TFT_WHITE);
			cursorState = 1;
			blinkMillis = millis();
			if(cursor < 3)
				cursor += 3;
			else
				cursor -= 3;
			notificationMillis = millis();
			while(!update());
		}
		if(buttons.released(BTN_DOWN))
		{
			osc->note(75, 0.05);
			osc->play();
			display.drawRect(12 + cursor % 3 * 48 - 2, 25 + 45 * (int)(cursor / 3) - 2, 44, 44,TFT_WHITE);
			display.drawRect(12 + cursor % 3 * 48 - 1, 25 + 45 * (int)(cursor / 3) - 1, 42, 42,TFT_WHITE);
			cursorState = 1;
			blinkMillis = millis();
			if(cursor < 3)
				cursor += 3;
			else
			{
				notificationView();
			}
			notificationMillis = millis();
			
			while(!update());
		}
		if(buttons.released(BTN_LEFT))
		{
			osc->note(75, 0.05);
			osc->play();
			display.drawRect(12 + cursor % 3 * 48 - 2, 25 + 45 * (int)(cursor / 3) - 2, 44, 44,TFT_WHITE);
			display.drawRect(12 + cursor % 3 * 48 - 1, 25 + 45 * (int)(cursor / 3) - 1, 42, 42,TFT_WHITE);
			cursorState = 1;
			blinkMillis = millis();
			if(cursor % 3 == 0)
				cursor += 2;
			else
				cursor -= 1;
			notificationMillis = millis();
			while(!update());
		}
		if(buttons.released(BTN_RIGHT))
		{
			osc->note(75, 0.05);
			osc->play();
			display.drawRect(12 + cursor % 3 * 48 - 2, 25 + 45 * (int)(cursor / 3) - 2, 44, 44,TFT_WHITE);
			display.drawRect(12 + cursor % 3 * 48 - 1, 25 + 45 * (int)(cursor / 3) - 1, 42, 42,TFT_WHITE);
			cursorState = 1;
			blinkMillis = millis();
			if(cursor % 3 == 2)
				cursor -= 2;
			else
				cursor += 1;
			notificationMillis = millis();
			while(!update());
		}
		if(buttons.released(BTN_A))
		{
			while(!update());
			switch (cursor)
			{
				case 0: //volume
					while(!buttons.released(BTN_B) && !buttons.released(BTN_A))
					{
						display.drawRect(14, 50, 134, 28, TFT_BLACK);
						display.drawRect(13, 49, 136, 30, TFT_BLACK);
						display.fillRect(15, 51, 132, 26, 0x9FFE);
						display.drawRect(37, 59, 86, 10, TFT_BLACK);
						display.drawRect(36, 58, 88, 12, TFT_BLACK);
						display.fillRect(38, 60, volume * 6, 8, TFT_BLACK);
						display.drawBitmap(18, 56, noSound, TFT_BLACK, 2);
						display.drawBitmap(126, 56, fullSound, TFT_BLACK, 2);
						if(buttons.released(BTN_LEFT) && volume > 0)
						{
							volume--;
							osc->setVolume(256 * volume / 14);
							osc->note(75, 0.05);
							osc->play();
							while(!update());
						}
						if(buttons.released(BTN_RIGHT) && volume < 14)
						{
							volume++;
							osc->setVolume(256 * volume / 14);
							osc->note(75, 0.05);
							osc->play();
							while(!update());
						}
						update();
					}
				break;

				case 1: //exit
					loader();
				break;

				case 2: //screen brightness
					while(!buttons.released(BTN_B) && !buttons.released(BTN_A))
					{
						display.drawRect(13, 49, 136, 30, TFT_BLACK);
						display.drawRect(14, 50, 134, 28, TFT_BLACK);
						display.fillRect(15, 51, 132, 26, 0xFF92);
						display.drawRect(33, 58, 89, 12, TFT_BLACK);
						display.drawRect(34, 59, 87, 10, TFT_BLACK);
						display.fillRect(35, 60, brightness * 17, 8, TFT_BLACK);
						display.drawBitmap(18, 59, noBrightness, TFT_BLACK, 2);
						display.drawBitmap(125, 53, fullBrightness, TFT_BLACK, 2);
						if(buttons.released(BTN_LEFT) && brightness > 0)
						{
							brightness--;
							osc->note(75, 0.05);
							osc->play();
							while(!update());
						}
						if(buttons.released(BTN_RIGHT) && brightness < 5)
						{
							brightness++;
							osc->note(75, 0.05);
							osc->play();
							while(!update());
						}
						if (brightness == 0)
							ledcAnalogWrite(LEDC_CHANNEL, 230);
						else
							ledcAnalogWrite(LEDC_CHANNEL, (5 - brightness) * 51);

						update();
					}
				break;

				case 3: //screenshot
					screenshotFlag = 1;
					return;
				break;

				case 4: //clock
				{
					uint32_t timer = millis();
					bool blinkState = 0;
					String temp = "";
					String monthsList[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

					while(!buttons.released(BTN_B) && !buttons.released(BTN_A))
					{
						display.fillScreen(0x963F);
						// date and time
						updateTimeRTC();
						display.setTextFont(2);
						display.setTextSize(2);
						display.setCursor(15, 25);
						temp = "";
						if (clockHour < 10)
							temp.concat("0");
						temp.concat(clockHour);
						temp.concat(":");
						if (clockMinute < 10)
							temp.concat("0");
						temp.concat(clockMinute);
						temp.concat(":");
						if (clockSecond < 10)
							temp.concat("0");
						temp.concat(clockSecond);

						display.printCenter(temp);
						display.setTextSize(1);
						display.setCursor(63, 85);
						temp = "";
						if (clockDay < 10)
							temp.concat("0");
						temp.concat(clockDay);
						if(clockDay < 20 && clockDay > 10)
							temp.concat("th");
						else if(clockDay%10 == 1)
							temp.concat("st");
						else if(clockDay%10 == 2)
							temp.concat("nd");
						else if(clockDay%10 == 3)
							temp.concat("rd");
						else
							temp.concat("th");
						temp.concat(" of ");
						temp.concat(monthsList[clockMonth - 1]);

						display.printCenter(temp);
						display.setCursor(0,100);
						display.printCenter(2000 + clockYear);


						if(millis()-timer >= 1000)
						{
							blinkState = !blinkState;
							timer = millis();
						}
						update();

					}
				}
				break;

				case 5: //LED brightness
				{
					while(!buttons.released(BTN_B) && !buttons.released(BTN_A))
					{
						for (int i = 0; i < 8; i++)
							leds[i] = CRGB::Red;
						display.drawRect(13, 49, 136, 30, TFT_BLACK);
						display.drawRect(14, 50, 134, 28, TFT_BLACK);
						display.fillRect(15, 51, 132, 26, 0xA794);
						display.drawRect(33, 58, 89, 12, TFT_BLACK);
						display.drawRect(34, 59, 87, 10, TFT_BLACK);
						display.fillRect(35, 60, pixelsBrightness * 17, 8, TFT_BLACK);
						display.drawBitmap(18, 59, noBrightness, TFT_BLACK, 2);
						display.drawBitmap(125, 53, fullBrightness, TFT_BLACK, 2);
						if(buttons.released(BTN_LEFT) && pixelsBrightness > 0)
						{
							pixelsBrightness--;
							osc->note(75, 0.05);
							osc->play();
							while(!update());
						}
						if(buttons.released(BTN_RIGHT) && pixelsBrightness < 5)
						{
							pixelsBrightness++;
							osc->note(75, 0.05);
							osc->play();
							while(!update());
						}
						update();
					}
				}
				break;
			}
			while(!update());
			display.fillScreen(TFT_WHITE);
			display.drawIcon(popupVolume,12,25,20,20,2);
			display.drawIcon(popupExit,60,25,20,20,2);
			display.drawIcon(popupScreenBrightness,108,25,20,20,2);
			display.drawIcon(popupScreenshot,12,70,20,20,2);
			display.drawIcon(popupPixelBrightness,108,70,20,20,2);
		}
		update();
	}
	while(!update());
}
void MAKERphone::drawNotificationWindow(uint8_t y, uint8_t index) {
	display.setTextFont(2);
	display.setTextSize(1);
	display.setCursor(30, y + 2);
	display.fillRoundRect(2, y + 2, 154, 20, 2, TFT_DARKGREY);
	display.fillRoundRect(4, y, 154, 20, 2, TFT_WHITE);
	display.print(notificationDescriprionList[index]);
	switch (notificationTypeList[index])
	{
		case 1:
			display.drawBitmap(7, y + 2, missedCallIcon, TFT_RED, 2);
			break;
		case 2:
			display.drawBitmap(7, y + 2, newMessageIcon, TFT_BLACK, 2);
			break;
		case 3:
			display.drawBitmap(7, y + 2, systemNotification, TFT_RED, 2);
			break;
	}
	display.setTextFont(1);
	display.setCursor(120, y + 2);
	String temp = "";
	DateTime now = notificationTimeList[index];
	if (now.hour() < 10)
		temp.concat("0");
	temp.concat(now.hour());
	temp.concat(":");
	if (now.minute() < 10)
		temp.concat("0");
	temp.concat(now.minute());
	display.print(temp);
	display.setCursor(119, y + 11);
	temp = "";
	temp.concat(monthsList[now.month() - 1]);
	temp.concat(" ");
	if (now.day() < 10)
		temp.concat("0");
	temp.concat(now.day());
	display.print(temp);
}
void MAKERphone::homePopupEnable(bool enabled)
{
	HOME_POPUP_ENABLE = enabled;
}
void MAKERphone::playNotificationSound(uint8_t _notification)
{
	notification = _notification;
	osc->stop();
	osc->setADSR(10,20,0.8,10);
	osc->setVolume(256 * volume / 14);
	osc->setWaveform(SINE);
	notificationMillis = 0;
	playingNotification = 1;
	notesIndex = 0;

	notificationSoundDuration = notificationNotesDuration[notification][notesIndex];
}
void MAKERphone::updateNotificationSound()
{
	if(playingNotification && millis() - notificationMillis >= notificationSoundDuration*1000 +  125)
	{
		notificationSoundDuration = notificationNotesDuration[notification][notesIndex];
		notificationSoundNote = notificationNotes[notification][notesIndex];
		if(notificationSoundDuration == 0 || notesIndex == 5)
		{
			Serial.println("stopped\n");
			playingNotification = 0;
			// osc->stop();
			return;
		}
		if(notificationSoundNote == 0)
		{
			notesIndex++;
			return;
		}
		notificationMillis = millis();
		osc->note(notificationSoundNote, notificationSoundDuration);
		
		notesIndex++;
	}
}
void MAKERphone::addNotification(uint8_t _type, char* _description, DateTime _time)
{
	for(int i = 0; i < sizeof(notificationTypeList);i++)
	{
		if(notificationTypeList[i] == 0)
		{
			notificationTypeList[i] = _type;
			notificationDescriprionList[i] = _description;
			notificationTimeList[i] = _time;
			break;
		}
	}
	saveNotifications();
}
void MAKERphone::loadNotifications(bool debug)
{
	const char * path = "/.core/notifications.json";
	Serial.println("");
	_SD.remove(path);
	SDAudioFile file = _SD.open(path);
	JsonArray& notifications = mp.jb.parseArray(file);
	file.close();
	
	if (notifications.success()) {
		int i = 0;
		for(JsonObject& tempNotification:notifications)
		{
			notificationTypeList[i] = tempNotification["Type"];
			notificationDescriprionList[i] = (char*)tempNotification["Description"].as<char*>();
			notificationTimeList[i] = DateTime(tempNotification["DateTime"].as<uint32_t>());	
			i++;
		}
	}
	else {
		saveNotifications();
		Serial.println("Error loading new notifications");
	}
	// notifications.prettyPrintTo(Serial);
}
void MAKERphone::saveNotifications(bool debug)
{
	const char * path = "/.core/notifications.json";
	Serial.println("");
	_SD.remove(path);
	JsonArray& notifications = jb.createArray();

	if (notifications.success()) {
		for(int i = 0; i<sizeof(notificationTypeList);i++)
		{
			JsonObject& tempNotification = jb.createObject();
			tempNotification["Type"] = notificationTypeList[i];
			tempNotification["Description"] = notificationDescriprionList[i];
			tempNotification["DateTime"] = notificationTimeList[i].unixtime();
			notifications.add(tempNotification);	
		}

		SDAudioFile file1 = _SD.open(path, "w");
		notifications.prettyPrintTo(file1);
		// notifications.prettyPrintTo(Serial); 
		file1.close();
	} else {
		Serial.println("Error saving notifications data");
	}
}
void MAKERphone::notificationView()
{
	char key = NO_KEY;
	while(!buttons.released(BTN_B) && !buttons.released(BTN_UP))
	{
		key = buttons.kpdNum.getKey();
		display.fillScreen(TFT_WHITE);
		display.setCursor(0, 2);
		display.setTextFont(2);
		display.setTextSize(1);
		display.printCenter("Notifications");
		display.setCursor(3, 114);
		display.print("Clear all");

		uint8_t temp = 0;
		bool anyNotifications = 0;
		for(int i = 0; i< sizeof(notificationTypeList);i++)
		{
			if(notificationTypeList[i] != 0)
			{
				temp++;
				if(temp == 1)
					drawNotificationWindow(20,i);
				else if(temp == 2)
					drawNotificationWindow(44,i);
				else if(temp == 3)
					drawNotificationWindow(68,i);
				else if(temp == 4)
					drawNotificationWindow(92,i);
				anyNotifications = 1;
			}
		}
		if(!anyNotifications)
		{
			display.setCursor(0, display.height() / 2 - 16);
			display.printCenter("No notifications...");
		}
		if(key == 'C')
		{
			for(int i = 0; i< sizeof(notificationTypeList);i++)
				notificationTypeList[i] = 0;
			if(SDinsertedFlag)
				saveNotifications();
		}
		update();
	}
	while(!update());
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

String MAKERphone::currentDateTime(){
	mp.updateTimeRTC();
	// 2019-04-18 12:00:00
	String dateTime = "20" + String(mp.clockYear);
	dateTime += "-";
	if(mp.clockMonth < 10){
		dateTime += "0";
	}
	dateTime += String(mp.clockMonth);
	dateTime += "-";
	if(mp.clockDay < 10){
		dateTime += "0";
	}
	dateTime += String(mp.clockDay);
	dateTime += " ";

	if(mp.clockHour < 10){
		dateTime += "0";
	}
	dateTime += String(mp.clockHour);
	dateTime += ":";
	if(mp.clockMinute < 10){
		dateTime += "0";
	}
	dateTime += String(mp.clockMinute);
	dateTime += ":";
	if(mp.clockSecond < 10){
		dateTime += "0";
	}
	dateTime += String(mp.clockSecond);
	return dateTime;
}