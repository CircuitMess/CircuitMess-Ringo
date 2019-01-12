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

#include <WiFi.h>
#include <esp32-hal-bt.h>
#include <stdint.h>
#include <EEPROM.h>
#include "esp_ota_ops.h"
//#include <Arduino.h>
#include "FastLED/FastLED.h"
extern HardwareSerial Serial1;
#include "TFT_eSPI/TFT_eSPI.h" // Graphics and font library for ST7735 driver chip
#include <SPI.h>
// #include "utility/XT_DAC_Audio.h"

#include "utility/ArduinoJson.h"

//Includes for SD firmware update
//#include <SysCall.h>
//#include <sdios.h>
//#include <SdFatConfig.h>
//#include <SdFat.h>
//#include <MinimumSerial.h>
//#include <FreeStack.h>
//#include <BlockDriver.h>
//SdFatSdio SD;
//File file;
#include "utility/SdFat.h"
#include <Update.h>

//Keypad setup
#include "utility/Keypad_I2C.h"
#include "utility/Keypad.h"
//#include "utility/Wire2.h"

//Fonts and sprites to use
#include "utility/Free_Fonts.h"
#include "utility/sprites.c"
#include "utility/JPEGDecoder.h"

//Setup for MP3 playback

//#include <AudioFileSource.h>
//#include <AudioFileSourceID3.h>
#include <AudioGeneratorMP3.h>
//#include <AudioOutputI2S.h>
//#include <AudioFileSourceSD.h>
//#include <AudioFileSourceBuffer.h>
//#include "src/ESP8266Audio.h"
//#include "utility/AudioFileSource.h"
//#include "utility/AudioFileSourceID3.h"
//#include "utility/AudioGeneratorMP3.h"
//#include "utility/AudioOutputI2S.h"
//#include "utility/AudioFileSourceSD.h"
//#include "utility/AudioFileSourceBuffer.h"

//PCF8574 0x21 defines (keys)
#define JOYSTICK_A 0
#define JOYSTICK_B 1
#define JOYSTICK_C 2
#define JOYSTICK_D 3
#define JOYSTICK_CENTER 4
#define BTN_A 5
#define BTN_B 6
#define VIBRATION_MOTOR 7

#define SIM800_DTR 13
#define INTERRUPT_PIN 35

#define NUMPIXELS 8 //number of pixels connected to the MCU
#define PIXELPIN 33  // make sure to set this to the correct pin, ignored for Esp8266

#define colorSaturation 128

#define LCDWIDTH  160
#define LCDHEIGHT 128
#define LCD_BL_PIN 12
#define BUFWIDTH  80
#define BUFHEIGHT 64
#define BUF2WIDTH  160
#define BUF2HEIGHT 128

#define menuYOffset 9
#define settingsMenuYOffset 5
#define composeBoxHeight 12
#define map_width 7
#define RESET_MTP '~'
#define buttons_per_column 4
#define buttons_per_row 4
#define multi_tap_threshold 500

#define ROWS 4 //four rows
#define COLS 4

#define NUM_BTN 8
#define soundPin 32

#define LEDC_CHANNEL 1 // use second channel of 16 channels(started from zero)
#define LEDC_TIMER  13 // use 13 bit precission for LEDC timer
#define LEDC_BASE_FREQ  5000 // use 5000 Hz as a LEDC base frequency

// capacity = JSON_ARRAY_SIZE(number_of_contacts) + number_of_contacts*JSON_OBJECT_SIZE(2);
// The following size is calculated of 100 contracts and the formula is ^
#define capacity 5208
#define number_of_contacts 100

#define smsNumber 22
class Buttons
{
public:
	///////////////////
	//Keypad variables
	//////////////////

	char keys[4][3] = {
	{ '1','2','3' },
	{ '4','5','6' },
	{ '7','8','9' },
	{ '*','0','#' }
	};
	char keysNum[4][4] = {
	{ '1', '2', '3', 'A' },
	{ '4', '5', '6', 'B' },
	{ '7', '8', '9', 'C' },
	{ '*', '0', '#', 'D' }
	};
	byte rowPins[ROWS] = { 0, 1, 2, 3 }; //connect to the row pinouts of the keypad
	byte colPins[COLS] = { 4, 5, 6, 7 }; //connect to the column pinouts of the keypad
	int i2caddress = 0x21;
	int i2caddressNum = 0x20;
	Keypad_I2C kpd = Keypad_I2C(makeKeymap(keys), rowPins, colPins, ROWS, COLS, i2caddress);
	Keypad_I2C kpdNum = Keypad_I2C(makeKeymap(keysNum), rowPins, colPins, ROWS, COLS, i2caddressNum);
	bool pressed(uint8_t button);
	void update();
	bool repeat(uint8_t button, uint16_t period);
	bool released(uint8_t button);
	bool held(uint8_t button, uint16_t time);
	uint16_t timeHeld(uint8_t button);
	uint16_t states[NUM_BTN];
	void begin();

};
class GUI {
	public:
		//main menu GUI
	uint8_t drawCursor(uint8_t xoffset, uint8_t yoffset, uint8_t xelements, uint8_t yelements, uint8_t xstart, uint8_t ystart);
	int8_t drawBigIconsCursor(uint8_t xoffset, uint8_t yoffset, uint8_t xelements, uint8_t yelements, uint8_t xstart, uint8_t ystart);
	//generic menu GUI
	void menuDrawBox(String text, uint8_t i, int32_t y);
	void menuDrawCursor(uint8_t i, int32_t y);
	int8_t menu(const char* title, String* items, uint8_t length);
	//lock screen notifications
	void drawNotificationWindow(uint8_t x, uint8_t y, uint8_t width, uint8_t height, String text); 
	//popup GUI
	void popup(String text, uint8_t duration);
	void updatePopup();
	String popupText;
	uint8_t popupTimeLeft;
	uint8_t popupTotalTime;

	private:
	uint8_t cursorX = 0;
	uint8_t cursorY = 0;
	bool cursorState = 1;
	uint8_t index;
	bool previousButtonState = 0;
	uint8_t cursor = 0;
	int32_t cameraY = 0;
	int32_t cameraY_actual = 0;
};

class MAKERphone:public Buttons, public GUI
{
public:
	// XT_DAC_Audio_Class DacAudio = XT_DAC_Audio_Class(25,0);    // Create the main player class object. 
    //                                   // Use GPIO 25, one of the 2 DAC pins and timer 0
                                      
	// XT_Wav_Class StarWars = XT_Wav_Class(StarWarsWav);   // create an object of type XT_Wav_Class that is used by 
                                      // the dac audio class (above), passing wav data as parameter.
	SdFat SD;
	TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
	TFT_eSprite display = TFT_eSprite(&tft);
	TFT_eSprite buf = TFT_eSprite(&tft);
	const esp_partition_t* partition;
	const esp_partition_t* partition2;
	bool resolutionMode = 0; //0 is native, 1 is halved
	void setResolution(bool res);
	bool spriteCreated = 0;

	void begin(bool splash = 1);
	void test();
	void tone2(int pin, int freq, int duration);
	void vibration(int duration);
	void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255);
	bool update(AudioGeneratorMP3 *mp3 = NULL);
	void splashScreen();
	void lockScreen();
	void mainMenu();
	void bigIconsMainMenu();
	void listDir(const char * dirname, uint8_t levels);
	void sleep();
	void checkSMS();
	void incomingCall();
	void checkSim();
	void enterPin();
	String textInput(String buffer, int16_t length);
	int textPointer = 0;
	void debugMode();
	void loader();
	void popupMenu();

	//SMS functions
	uint16_t countSubstring(String string, String substring);
	String readSerial();
	String readSms(uint8_t index);
	String readAllSms();
	void viewSms(String content, String contact, String date);
	void smsMenuDrawBox(String contact, String date, String content, uint8_t i, int32_t y);
	void smsMenuComposeBox(uint8_t i, int32_t y);
	void smsMenuDrawCursor(uint8_t i, int32_t y);
	void smsMenuComposeBoxCursor(uint8_t i, int32_t y);
	int16_t smsMenu(const char* title, String* contact, String *date, String *content, uint8_t length);
	void messagesApp();
	void composeSMS();
	void incomingMessagePopup();

	void updateFromFS(String FilePath);


	//variables used for parsing SMS
	int16_t y;
	String smsContent[smsNumber];
	String phoneNumber[smsNumber];
	String tempDate[smsNumber];
	uint16_t smsYear[smsNumber];
	uint8_t smsDay[smsNumber];
	uint8_t smsMonth[smsNumber];
	uint8_t smsMinute[smsNumber];
	uint8_t smsSecond[smsNumber];
	uint8_t smsHour[smsNumber];
	uint32_t start = 0;
	uint32_t end = 0;
	String input;
	String buffer;

	//NeoPixels...
	int numberOfColors = 19;
	uint8_t pixelState;
	CRGB leds[NUMPIXELS];

	//Media app
	uint8_t cursor = 0;
	int32_t cameraY = 0;
	int32_t cameraY_actual = 0;
	String mp3Files[255];
	uint8_t mp3Count = 0;
	int16_t mp3Menu(const char* title, String* items, uint8_t length);
	void listMP3(const char * dirname, uint8_t levels);
	void mp3player(String songName);
	void mediaApp();
	int8_t mediaMenu(String* title, uint8_t length);
	void mediaMenuDrawBox(String title, uint8_t i, int32_t y);
	void mediaMenuDrawCursor(uint8_t i, int32_t y,  bool pressed);
	void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string);
	String mediaItems[3] = {
		"Music",
		"Photo",
		"Video",
	};
	String photoFiles[255];
	void listPhotos(const char *dirname, uint8_t levels);
	void drawJpeg(String filename, int xpos, int ypos);
	void jpegRender(int xpos, int ypos);
	void jpegInfo();
	uint8_t photoCount = 0;

	//Contacts app
	void contactsMenuDrawBox(String contact, String number, uint8_t i, int32_t y);
	void contactsMenuDrawBoxSD(String name, String number, uint8_t i, int32_t y);
	uint8_t deleteContact(String contact, String number, String id);
	uint8_t deleteContactSD(String name, String number);
	uint8_t newContact();
	uint8_t newContactSD(String *name, String *number);
	void parse_contacts();
	void contactsMenuNewBox(uint8_t i, int32_t y);
	void contactsMenuDrawCursor(uint8_t i, int32_t y);
	void contactsMenuNewBoxCursor(uint8_t i, int32_t y);
	int contactsMenu(const char* title, String* contact, String *number, uint8_t length);
	int contactsMenuSD(JsonArray *contacts);
	void contactsApp();
	void contactsAppSD();
	String readAllContacts();
	void callNumber(String number);

	StaticJsonBuffer<capacity> jb;

	//Phone app
	void phoneApp();
	void dialer();

	//Settings variables

	bool wifi = 1;
	bool bt = 0;
	bool airplaneMode = 0;
	uint8_t brightness = 5;  //brightness 0-5
	uint8_t actualBrightness = 0; //value from 0 (brightest) to 255 (backlight off)
	uint16_t sleepTimeActual = 0; //in seconds
	uint8_t backgroundIndex = 0;
	uint8_t volume = 10; //volume 0-14

	bool pinLock;
	uint16_t pinNumber = 1234;
	bool simInserted = 0;
	bool simReady = 0;
	uint32_t sleepTimer = millis();
	uint16_t batteryVoltage;
	uint16_t signalStrength;
	String carrierName = "";

	//Settings app
	uint8_t sleepTime = 0;
	String ringtone = "/ringtones/chiptune.mp3";
	String notification = "/notifications/to-the-point.mp3";
	uint8_t ringtoneCount=0;
	uint8_t notificationCount=0;
	String ringtoneFiles[255];
	String notificationFiles[255];
	int backgroundColors[7] = {
	TFT_CYAN,
	TFT_GREEN,
	TFT_RED,
	TFT_YELLOW,
	TFT_WHITE,
	TFT_ORANGE,
	TFT_PINK
	};
	String backgroundColorsNames[7] = {
	"Cyan",
	"Green",
	"Red",
	"Yellow",
	"White",
	"Orange",
	"Pink"
	};
	uint16_t firmware_version = 1;

	bool settingsApp();
	int8_t settingsMenu(String* title, uint8_t length);
	void settingsMenuDrawBox(String title, uint8_t i, int32_t y);
	void settingsMenuDrawCursor(uint8_t i, int32_t y,  bool pressed);
	String settingsItems[6] = {
	"Network",
	"Display",
	"Date & time",
	"Sound",
	"Security",
	"About"};
	void networkMenu();
	void displayMenu();
	void soundMenu();
	void securityMenu();
	void timeMenu();
	bool updateMenu();
	void applySettings();
	void saveSettings(bool debug = false);
	void loadSettings(bool debug = false);
	void wifiMenu();

	void listRingtones(const char * dirname, uint8_t levels);
	void listNotifications(const char * dirname, uint8_t levels);

	GUI gui;
	Buttons buttons;
	//Display display;

	////////////////////////////////Main_menu.ino variables
	String titles[6] = {
	"Messages",
	"Media",
	"Contacts",
	"Settings",
	"Phone",
	"Apps",
	};
	String BinaryFiles[255];
	uint8_t binaryCount = 0;//Number of binary files available for loading

	//update() variables

	unsigned long buttonHeld;
	bool blinkState;
	int frameSpeed = 40;
	int lastFrameCount2 = 0;
	int smsRefresh = 0;
	String updateBuffer;
	uint32_t refreshMillis = millis();
	bool dataRefreshFlag = 0;
	bool receivedFlag = 0;
	bool SDinsertedFlag = 0;
	bool popupMenuFlag = 1;
	/////////////////////////////////////////
	///////////////COLLISION//////////////////
	//////////////////////////////////////////
	bool collideRectRect(int16_t x1, int16_t y1, int16_t w1, int16_t h1, int16_t x2, int16_t y2, int16_t w2, int16_t h2);  // Returns TRUE if the 2 rects overlap
	bool collidePointRect(int16_t pointX, int16_t pointY, uint16_t rectX, uint16_t rectY, uint16_t rectW, uint16_t rectH);  // Returns TRUE if the point overlaps the rect
	bool collideCircleCircle(int16_t centerX1, int16_t centerY1, int16_t r1, int16_t centerX2, int16_t centerY2, int16_t r2);  // Returns TRUE if the 2 circles overlap
	bool collidePointCircle(int16_t pointX, int16_t pointY, int16_t centerX, int16_t centerY, int16_t r);  // Returns TRUE if the point overlaps the circle

private:
	int multi_tap(byte key);
	bool newMessage = 0;
	uint8_t currentMessageNumber;
  void performUpdate(Stream &updateSource, size_t updateSize);
	uint8_t coverWidth = 45;
	uint8_t coverHeight = 45;
	bool mute = false;
	uint8_t timesRemaining;

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
	void updateTimeGSM();
	void updateTimeRTC();

	int colorArray[19] = {
	TFT_BLACK,
	TFT_NAVY,
	TFT_DARKGREEN,
	TFT_DARKCYAN,
	TFT_MAROON,
	TFT_PURPLE,
	TFT_OLIVE,
	TFT_LIGHTGREY,
	TFT_DARKGREY,
	TFT_BLUE,
	TFT_GREEN,
	TFT_CYAN,
	TFT_RED,
	TFT_MAGENTA,
	TFT_YELLOW,
	TFT_WHITE,
	TFT_ORANGE,
	TFT_GREENYELLOW,
	TFT_PINK
	};

	//SIM800 setup
//	HardwareSerial sim800 = HardwareSerial(1);

	//Audio objects
	/*AudioGeneratorMP3 *mp3;
	AudioFileSourceSD *file;
	AudioOutputI2S *out;
	AudioFileSourceID3 *id3;
	AudioFileSourceBuffer *buff;*/
};

#endif
