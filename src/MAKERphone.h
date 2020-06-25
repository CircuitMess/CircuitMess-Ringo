/*
MIT License

Copyright (c) 2020 CircuitMess

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MAKERphone_h
#define MAKERphone_h

#ifdef MPMINIMAL
#pragma message(__FILE__ ": MPMINIMAL")
#endif

#include "FastLED/FastLED.h"
#include "TFT_eSPI/TFT_eSPI.h" // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include "utility/Buttons/Buttons.h"

//Fonts and sprites to use
#include "utility/Free_Fonts.h"
#include "utility/sprites.c"

#include <driver/rtc_io.h>
#include <esp_efuse.h>
#include <esp_adc_cal.h>
#include <driver/adc.h>

#ifndef MPMINIMAL

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include <driver/gpio.h>

#include "utility/ArduinoJson.h"
#include "utility/RTCLib/RTClib.h"

extern HardwareSerial Serial1;

#include <SD.h>
#include <FS.h>
#include <WiFi.h>
#include <esp32-hal-bt.h>

#include <EEPROM.h>
#include "esp_ota_ops.h"

#include <Update.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>

#include "utility/JPEGDecoder.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "utility/soundLib/MPAudioDriver.h"
#include "utility/soundLib/MPWavLib.h"
#include "utility/arduino_pdu_decoder/pdu_decoder.h"

#endif

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
#define BTN_HOME 13
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
#define ADC_COUNT 500		  //offset for the adc reading (mV)
#define SLEEP_WAKEUP_TIME 900 //intervals to wake up and check battery when sleeping (s)
#define NUMPIXELS 8			  //number of pixels connected
#define PIXELPIN 12

#define colorSaturation 128

#define LCDWIDTH 160
#define LCDHEIGHT 128
#define LCD_BL_PIN 21
#define BUFWIDTH 80
#define BUFHEIGHT 64
#define BUF2WIDTH 160
#define BUF2HEIGHT 128

#define menuYOffset 9
#define settingsMenuYOffset 5
#define composeBoxHeight 12
#define map_width 7
#define RESET_MTP '~'

#define soundSwitchPin 32

#define LEDC_CHANNEL 1		// use second channel of 16 channels(started from zero)
#define LEDC_TIMER 13		// use 13 bit precission for LEDC timer
#define LEDC_BASE_FREQ 5000 // use 5000 Hz as a LEDC base frequencyF

// capacity = JSON_ARRAY_SIZE(number_of_contacts) + number_of_contacts*JSON_OBJECT_SIZE(2);
// The following size is calculated of 100 contacts and the formula is ^
#define capacity 8000
#define number_of_contacts 100

#define FIRMWARE_VERSION_ADDRESS 0
#define GSM_MODULE_ADDRESS 200
#define SMS_LIMIT 30

class MAKERphone : public Buttons
#ifndef MPMINIMAL
	,
				   public DateTime
#endif
{
public:
	uint16_t firmware_version = 105;

	Buttons buttons;

	TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup.h
	TFT_eSprite display = TFT_eSprite(&tft);
	TFT_eSprite popupSprite = TFT_eSprite(&tft);
	// TFT_eSprite buf = TFT_eSprite(&tft);

	void begin(bool splash = 1);

	//NeoPixels...
	int numberOfColors = 19;
	uint8_t pixelState;
	CRGB leds[NUMPIXELS];
	void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255);
	bool update();
	void splashScreen();
	void shutdownPopupEnable(bool enable);

	//update() variables
	int frameSpeed = 40;
	int lastFrameCount = 0;
	String updateBuffer;
	uint32_t refreshMillis = millis();
	uint32_t buttonsRefreshMillis = millis();
	uint32_t joystickMillis = millis();

	bool resolutionMode = 0; //0 is native, 1 is halved
	void setResolution(bool res);
	bool spriteCreated = 0;

	String textInput(String buffer, int16_t length = -1);
	uint8_t brightness = 5;		  //brightness 0-5
	uint8_t actualBrightness = 0; //value from 0 (brightest) to 255 (backlight off)

	uint8_t sleepTime = 2;
	volatile double batteryVoltage = 50000;
	uint32_t sleepTimer = millis();

	uint32_t textPointer = 0;
	bool textLimitFlag = 0;

	bool dataRefreshFlag = 0;
	bool receivedFlag = 1;

	uint8_t pixelsBrightness = 5; //0-5

	bool screenshotFlag = 0;
	bool inCall = 0;

	String waitForOK();

#ifndef MPMINIMAL
	PCF8563 RTC;

	Oscillator *osc = new Oscillator();

	const esp_partition_t *partition;
	const esp_partition_t *partition2;

	void tone2(int pin, int freq, int duration);
	void vibration(int duration);

	void sleep();
	void incomingCall(String _serialData);
	void addCall(String number, String contact, uint32_t dateTime, int duration, uint8_t direction);
	void incomingMessage(String _serialData);
	bool newMessage = 0;
	void saveMessage(char *text, String contact, String number, bool isRead, bool direction, DateTime time);
	void checkSim();
	void enterPin();
	void enterPUK();
	String currentDateTime();
	void loader();
	void lockscreen();
	void updateFromFS(String FilePath);
	void performUpdate(Stream &updateSource, size_t updateSize);

	//In call speaker volume
	bool setCallVolume(uint8_t volume);

	//Emergency calls
	// void emergencyCall();
	// void callNumberEmergency(String number);
	// void emergencyCallDrawCursor(uint8_t i, int32_t y);

	//Notification sounds
	void playNotificationSound(uint8_t notification);
	void updateNotificationSound();
	uint8_t notificationNotes[5][5] PROGMEM = {
		{80, 80, 0, 0, 0},
		{70, 70, 0, 0, 0},
		{75, 75, 0, 0, 0},
		{90, 90, 0, 90, 90},
		{75, 80, 85, 0, 0}};
	float notificationNotesDuration[5][5] PROGMEM = {
		{0.1, 0.1, 0, 0, 0},
		{0.1, 0.1, 0, 0, 0},
		{0.1, 0.1, 0, 0, 0},
		{0.1, 0.1, 0.1, 0.1, 0.1},
		{0.1, 0.1, 0.1, 0, 0}};
	bool playingNotification = 0;
	uint8_t notesIndex = 0;
	uint32_t notificationMillis = millis();
	void addNotification(uint8_t _type, String _description, DateTime _time);
	void removeNotification(uint8_t index);
	//notification system
	uint8_t notificationTypeList[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //1-missed call, 2-message, 3-system notification, 0-empty
	String notificationDescriptionList[10] = {"", "", "", "", "", "", "", "", "", ""};
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
	uint16_t sleepTimeActual = 30; //in seconds
	uint8_t backgroundIndex = 0;
	uint8_t ringVolume = 10;  //volume 0-14
	uint8_t mediaVolume = 10; //volume 0-14
	uint8_t callSpeakerVolume = 5;
	bool pinLock;
	uint16_t pinNumber = 1234;
	bool simInserted = 0;
	bool simReady = 0;
	uint16_t signalStrength = 0;
	String carrierName = "";
	String ringtone_path = "/Ringtones/Default ringtone.wav";
	String alarm_path = "/Ringtones/Default ringtone.wav";
	uint8_t notification = 0;
	//Settings app

	void applySettings();
	void saveSettings(bool debug = false);
	void loadSettings(bool debug = false);

	// uint32_t voltageMillis = millis();
	// uint32_t voltageSum = 0;
	// uint16_t voltageSample = 0;
	bool SDinsertedFlag = 0;

	//COLLISION
	bool collideRectRect(int16_t x1, int16_t y1, int16_t w1, int16_t h1, int16_t x2, int16_t y2, int16_t w2, int16_t h2);	  // Returns TRUE if the 2 rects overlap
	bool collidePointRect(int16_t pointX, int16_t pointY, uint16_t rectX, uint16_t rectY, uint16_t rectW, uint16_t rectH);	  // Returns TRUE if the point overlaps the rect
	bool collideCircleCircle(int16_t centerX1, int16_t centerY1, int16_t r1, int16_t centerX2, int16_t centerY2, int16_t r2); // Returns TRUE if the 2 circles overlap
	bool collidePointCircle(int16_t pointX, int16_t pointY, int16_t centerX, int16_t centerY, int16_t r);					  // Returns TRUE if the point overlaps the circle

	//SD functions
	String readFile(const char *path);
	void writeFile(const char *path, const char *message);
	void appendFile(const char *path, const char *message);

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

	//on-screen popup
	void popup(String text, uint16_t duration);
	void updatePopup();
	void homePopupEnable(bool enable);
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
		{0, 0, 0, 0, 0, 0, 0}};
	String alarmTrack[5] = {
		"/Ringtones/Default ringtone.wav",
		"/Ringtones/Default ringtone.wav",
		"/Ringtones/Default ringtone.wav",
		"/Ringtones/Default ringtone.wav",
		"/Ringtones/Default ringtone.wav"};
	void saveNotifications(bool debug = 0);
	void deallocateAudio();
	void reallocateAudio();
	uint8_t micGain = 14;
	uint16_t simVoltage = 3900;
	uint8_t sim_module_version = 255; // 0 - SIM7600, 1 - SIM800, 255 - no module
	uint8_t oscillatorVolumeList[15] = {0, 33, 37, 47, 57, 62, 65, 69, 72, 75, 79, 84, 90, 95, 100};
	bool exitedLockscreen = 0;
	bool homeButtonPressed = 0;
	String checkContact(String contactNumber);
	void pduDecode(const char *PDU);
	unsigned long prevKeyMillis = 0;

	int8_t networkRegistered = -1;
	void networkModuleInit();
	static int DecodePDUMessage(const char *buffer, int buffer_length, char *output_sms_text, int sms_text_length);

#endif
private:
	bool inShutdownPopup = 0;
	void shutdownPopup(bool animation = 1);
	bool SHUTDOWN_POPUP_ENABLE = 1;
	String popupText;
	char c;
	int shutdownCounter = 0;
	int timesMeasured = 0;
	bool isCalibrated = 0;
	bool buttonsPressed = 0;
	bool wokeWithPWRBTN = 0;

#ifndef MPMINIMAL

	MPTrack *ringtone = nullptr;
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
	uint16_t popupTimeLeft = 0;
	uint16_t popupTotalTime = 0;
	const char *popupHomeItems[6] PROGMEM = {
		"Volume",
		"Home",
		"Screen brightness",
		"Screenshot",
		"Date & time",
		"LED brightness"};
	String monthsList[12] PROGMEM = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
	bool inHomePopup = 0;
	void takeScreenshot();
	int backgroundColors[7] PROGMEM = {
		TFT_CYAN,
		TFT_GREEN,
		TFT_RED,
		TFT_YELLOW,
		TFT_WHITE,
		TFT_ORANGE,
		TFT_PINK};
	float notificationSoundDuration = 0;
	uint8_t notificationSoundNote = 0;
	bool SDerror = 0;

	uint8_t alternatingRefresh = 1;
	bool inLockScreen = 0;
	MPTrack *currTracks[4] = {nullptr, nullptr, nullptr, nullptr};
	bool pausedTracks[4] = {0, 0, 0, 0};
	bool tracksSaved = 0;
	// uint8_t oscillatorVolumeList[14] = {0,10,15,20,35,50,60,80,100,120,150,200,220,255};
	uint32_t networkDisconnectMillis = millis();
	bool networkDisconnectFlag = 1;
	bool networkInitialized = 0;
	char _smsText[162];
	char _smsNumber[20];
	DateTime _smsDatetime;
	uint8_t _concatSMSCounter = 0;
	uint8_t _currentConcatSMS = 0;
	bool _concatSMSCodingScheme = 0;
	uint32_t carrierNameCounter = 0;
#endif
};
uint32_t ADCrawRead(void);

#endif

/*
Huge thanks to all of our supporters on Kickstarter.
It was a long way to go since the start of our Kickstarter campaign and we couldn't have done it without your help.
We thank each and every one of you, most notably:

	Niko Klansek
	Hrvoje Hafner
	Sandro Vrbanus
	Marko
	divit
	Matija
	Gabrijel Skoro
	Manon Cortial
	Barbara Kolarić
	Alain Pannetrat
	Paulo Pinto Adao
	Matea Zukolo
	Miroslav Rajter
	Cavan Vannice
	Echo Mae
	Sven Maricic
	Vela
	Antriksh
	Hoizichri
	Ivan Palcic
	Alen
	PhonicBloom Limited
	Justin Thurer
	Foodin
	Richard Hudspeth
	Julien B
	J.P.
	Ivančica Gajšak
	Markus Kruse
	jiayitong
	Joseph Migeed
	Chris Shell
	Daniel Crowley
	Darren P
	gordana podvezanec
	n0kturnal
	Travis Austin
	Sandro Pansa
	Mandy Savage
	doomslace
	Jeremy VanSchalkwyk
	John "johnkzin" Rudd
	Pierre Baillargeon
	Chap Ambrose
	Anthony
	Thilo-W. Finger
	Patrick Beaupré
	NetDr
	Anze Miklavec
	Kirsten Browne
	Beau Smith
	Thomas Hutchinson
	Daniel G.
	Alexander Arce
	Dimitris
	Hugh Reynolds
	Alexander River Gatensby
	MONOLIT
	Lee
	Gerren McKnight
	Qualified backer
	Pengouin
	Gifted Abilities Technical Institute
	Martin B Walters
	Candra N. Murphy
	Marko Pavisic
	Dino Vezos
	Jeanne
	Clay Cooper
	Jeroen
	Michael Hardin
	Christopher Smith
	E. H. Seale
	Ed
	Endre Czirbesz
	Kean
	Andreas Assmayr
	Matth Werner
	dikdust
	Ezequiel Lasnier
	Brian Dysart
	Josip
	Rasmus Berlin
	Stefan D.
	
*/
