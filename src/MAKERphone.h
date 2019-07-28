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

#ifndef MAKERphone_h
#define MAKERphone_h
#include <SD.h>
#include <FS.h>
#include <WiFi.h>
#include <esp32-hal-bt.h>
#include <driver/rtc_io.h>
#include <stdint.h>
#include <EEPROM.h>
#include "esp_ota_ops.h"

#include "FastLED/FastLED.h"
extern HardwareSerial Serial1;
#include "TFT_eSPI/TFT_eSPI.h" // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include "utility/ArduinoJson.h"
#include "utility/RTCLib/RTClib.h"
#include "utility/Buttons/Buttons.h"
#include <Update.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>

//Fonts and sprites to use
#include "utility/Free_Fonts.h"
#include "utility/sprites.c"

#include "utility/JPEGDecoder.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "utility/soundLib/MPAudioDriver.h"
#include "utility/soundLib/MPWavLib.h"

#define BTN_1 0
#define BTN_2 1
#define BTN_3 2
#define BTN_4 3
#define BTN_5 4
#define BTN_6 5
#define BTN_7 6
#define BTN_8 7
#define BTN_9 8
#define BTN_ASTERISK 9
#define BTN_0 10
#define BTN_HASHTAG 11
#define BTN_FUN_RIGHT 12
#define BTN_FUN_LEFT 15
#define BTN_A 16
#define BTN_B 17
#define BTN_UP 18
#define BTN_DOWN 19
#define BTN_LEFT 20
#define BTN_RIGHT 21


#define SIM800_DTR 26
#define BTN_INT 36
#define RTC_INT 34
#define SIM_INT 34
#define SD_INT 33
#define CHRG_INT 39

#define OFF_PIN 25
#define INTERRUPT_PIN 36
#define VOLTAGE_PIN 35
#define ADC_COUNT 500 //offset for the adc reading (mV)
#define SLEEP_WAKEUP_TIME 900 //intervals to wake up and check battery when sleeping (s)
#define NUMPIXELS 8 //number of pixels connected
#define PIXELPIN 12

#define colorSaturation 128

#define LCDWIDTH  160
#define LCDHEIGHT 128
#define LCD_BL_PIN 21
#define BUFWIDTH  80
#define BUFHEIGHT 64
#define BUF2WIDTH  160
#define BUF2HEIGHT 128

#define menuYOffset 9
#define settingsMenuYOffset 5
#define composeBoxHeight 12
#define map_width 7
#define RESET_MTP '~'

#define soundSwitchPin 32

#define LEDC_CHANNEL 1 // use second channel of 16 channels(started from zero)
#define LEDC_TIMER  13 // use 13 bit precission for LEDC timer
#define LEDC_BASE_FREQ  5000 // use 5000 Hz as a LEDC base frequencyF

// capacity = JSON_ARRAY_SIZE(number_of_contacts) + number_of_contacts*JSON_OBJECT_SIZE(2);
// The following size is calculated of 100 contacts and the formula is ^
#define capacity 8000
#define number_of_contacts 100

#define FIRMWARE_VERSION_ADDRESS 0
#define GSM_MODULE_ADDRESS 200
#define smsNumber 22


class MAKERphone:public Buttons, public DateTime
{
  public:
	PCF8563 RTC;
	Buttons buttons;

	TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
	TFT_eSprite display = TFT_eSprite(&tft);
	TFT_eSprite popupSprite = TFT_eSprite(&tft);
	// TFT_eSprite buf = TFT_eSprite(&tft);
	Oscillator* osc = new Oscillator();

	const esp_partition_t* partition;
	const esp_partition_t* partition2;
	bool resolutionMode = 0; //0 is native, 1 is halved

	void setResolution(bool res);
	bool spriteCreated = 0;

	void begin(bool splash = 1);
	void tone2(int pin, int freq, int duration);
	void vibration(int duration);
	void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255);
	bool update();
	void splashScreen();
	void sleep();
	void incomingCall(String _serialData);
	void addCall(String number, uint32_t dateTime, int duration, uint8_t direction);
	void incomingMessage(String _serialData);
	bool newMessage = 0;
	void saveMessage(String text, String number, bool isRead, bool direction, JsonArray *messages);
	void checkSim();
	void enterPin();
	void enterPUK();
	String currentDateTime();
	String textInput(String buffer, int16_t length = -1);
	uint32_t textPointer = 0;
	bool textLimitFlag = 0;
	void loader();
	void lockscreen();
	void updateFromFS(String FilePath);
  	void performUpdate(Stream &updateSource, size_t updateSize);

	//NeoPixels...
	int numberOfColors = 19;
	uint8_t pixelState;
	CRGB leds[NUMPIXELS];

	//Notification sounds
	void playNotificationSound(uint8_t notification);
	void updateNotificationSound();
	uint8_t notificationNotes[5][5] PROGMEM = {
		{80, 80, 0, 0, 0},
		{70, 70, 0, 0, 0},
		{75, 75, 0, 0, 0},
		{90, 90, 0, 90, 90},
		{75, 80, 85, 0, 0}
	};
	float notificationNotesDuration[5][5] PROGMEM = {
		{0.1, 0.1 , 0, 0, 0},
		{0.1, 0.1 , 0, 0, 0},
		{0.1, 0.1 , 0, 0, 0},
		{0.1, 0.1 , 0.1, 0.1, 0.1},
		{0.1, 0.1 , 0.1, 0, 0}
	};
	bool playingNotification = 0;
	uint8_t notesIndex = 0;
	uint32_t notificationMillis = millis();
	void addNotification(uint8_t _type, String _description, DateTime _time);
	void removeNotification(uint8_t index);
	//notification system
	uint8_t notificationTypeList[10] = {0,0,0,0,0,0,0,0,0,0}; //1-missed call, 2-message, 3-system notification, 0-empty
	String notificationDescriptionList[10] = {"", "", "","", "", "","", "", "", ""};
	DateTime notificationTimeList[10];


	//JPEG operations
	void drawJpeg(String filename, int xpos, int ypos);
	void jpegRender(int xpos, int ypos);
	void jpegInfo();

	//Json bufffer
	StaticJsonBuffer<capacity> jb;

	//Settings values
	// bool wifi = 1;
	// bool bt = 0;
	bool airplaneMode = 0;
	uint8_t brightness = 5;  //brightness 0-5
	uint8_t actualBrightness = 0; //value from 0 (brightest) to 255 (backlight off)
	uint16_t sleepTimeActual = 0; //in seconds
	uint8_t backgroundIndex = 0;
	uint8_t ringVolume = 10; //volume 0-14
	uint8_t mediaVolume = 10; //volume 0-14
	uint8_t pixelsBrightness = 5; //0-5
	bool pinLock;
	uint16_t pinNumber = 1234;
	bool simInserted = 0;
	bool simReady = 0;
	uint32_t sleepTimer = millis();
	volatile double batteryVoltage = 50000;
	uint16_t signalStrength = 0;
	String carrierName = "";
	uint8_t sleepTime = 0;
	String ringtone_path = "/Ringtones/Default ringtone.wav";
	String alarm_path = "/Ringtones/Default ringtone.wav";
	uint8_t notification = 0;
	uint16_t firmware_version = 2;
	//Settings app

	void applySettings();
	void saveSettings(bool debug = false);
	void loadSettings(bool debug = false);


	//update() variables
	int frameSpeed = 40;
	int lastFrameCount = 0;
	String updateBuffer;
	uint32_t refreshMillis = millis();
	uint32_t buttonsRefreshMillis = millis();
	uint32_t joystickMillis = millis();
	// uint32_t voltageMillis = millis();
	// uint32_t voltageSum = 0;
	// uint16_t voltageSample = 0;
	bool dataRefreshFlag = 0;
	bool receivedFlag = 1;
	bool SDinsertedFlag = 0;
	//SAVE manipulation
	JsonArray &getJSONfromSAV(const char *path);
	void saveJSONtoSAV(const char *path, JsonArray &json);

	//COLLISION
	bool collideRectRect(int16_t x1, int16_t y1, int16_t w1, int16_t h1, int16_t x2, int16_t y2, int16_t w2, int16_t h2);  // Returns TRUE if the 2 rects overlap
	bool collidePointRect(int16_t pointX, int16_t pointY, uint16_t rectX, uint16_t rectY, uint16_t rectW, uint16_t rectH);  // Returns TRUE if the point overlaps the rect
	bool collideCircleCircle(int16_t centerX1, int16_t centerY1, int16_t r1, int16_t centerX2, int16_t centerY2, int16_t r2);  // Returns TRUE if the 2 circles overlap
	bool collidePointCircle(int16_t pointX, int16_t pointY, int16_t centerX, int16_t centerY, int16_t r);  // Returns TRUE if the point overlaps the circle


	//SD functions
	String readFile(const char * path);
	void writeFile(const char * path, const char * message);
	void appendFile(const char * path, const char * message);

	//Clock functions and variables
	bool Century = false;
	bool h12;
	bool PM;
	uint8_t clockMonth, clockDay, clockHour, clockMinute, clockSecond;
	uint16_t clockYear;
	bool clockDy, clock12h, clockpm;
	DateTime currentTime;
	void updateTimeGSM();
	void updateTimeRTC();
	bool screenshotFlag = 0;


	//on-screen popup
	void popup(String text, uint16_t duration);
	void updatePopup();
	void homePopupEnable(bool enable);
	void shutdownPopupEnable(bool enable);
	void alarmPopup(bool animation = 1);
	void loadAlarms();
	void saveAlarms();
	bool inAlarmPopup = 0;
	bool alarmCleared = 0;
	uint8_t currentAlarm = 99;
	uint32_t alarmMillis = millis();
	uint8_t alarmHours[5] = {0, 0, 0, 0, 0};
	uint8_t alarmMins[5] = {0, 0, 0, 0, 0};
	uint8_t alarmEnabled[5] = {2, 2, 2, 2, 2};
	bool alarmRepeat[5] = {0, 0, 0, 0, 0};
	bool alarmRepeatDays[5][7] = {
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0}
	};
	String alarmTrack[5] = {
		"/Ringtones/Default ringtone.wav",
		"/Ringtones/Default ringtone.wav",
		"/Ringtones/Default ringtone.wav",
		"/Ringtones/Default ringtone.wav",
		"/Ringtones/Default ringtone.wav"
	};
	void saveNotifications(bool debug = 0);
	void deallocateAudio();
	void reallocateAudio();
	bool inCall = 0;
	uint8_t micGain = 15;
	uint16_t simVoltage = 3900;
	uint8_t sim_module_version = 255; // 0 - SIM7600, 1 - SIM800
	uint8_t oscillatorVolumeList[15] = {0,33,37,47,57,62,65,69,72,75,79,84,90,95,100};
	String waitForOK();
	bool exitedLockscreen = 0;
	bool homeButtonPressed = 0;
	private:
		MPTrack* ringtone = nullptr;
		int multi_tap(byte key);
		uint8_t timesRemaining;

		void homePopup(bool animation = 1);
		void loadNotifications(bool debug = 0);
		void drawNotificationWindow(uint8_t y, uint8_t index);
		void notificationView();
		void SDremovedPopup();
		void SDinsertedPopup();
	
		void checkAlarms();

		bool HOME_POPUP_ENABLE = 1;
		bool SHUTDOWN_POPUP_ENABLE = 1;
		String popupText;
		uint16_t popupTimeLeft = 0;
		uint16_t popupTotalTime = 0;
		const char *popupHomeItems[6] PROGMEM = {
			"Volume",
			"Home",
			"Screen brightness",
			"Screenshot",
			"Date & time",
			"LED brightness"
		};
		String monthsList[12] PROGMEM = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
		bool inHomePopup = 0;
		bool inShutdownPopup = 0;
		void shutdownPopup(bool animation = 1);
		void takeScreenshot();
		int backgroundColors[7] PROGMEM = {
			TFT_CYAN,
			TFT_GREEN,
			TFT_RED,
			TFT_YELLOW,
			TFT_WHITE,
			TFT_ORANGE,
			TFT_PINK
		};
		float notificationSoundDuration = 0;
		uint8_t notificationSoundNote = 0;
		bool SDerror = 0;
		char c;
		bool buttonsPressed = 0;
		bool wokeWithPWRBTN = 0;
		uint8_t alternatingRefresh = 1;
		bool inLockScreen = 0;
		// uint8_t oscillatorVolumeList[14] = {0,10,15,20,35,50,60,80,100,120,150,200,220,255};
};
#endif
