#include <MAKERphone.h>

MAKERphone mp;

void setup() {
	mp.display.invertDisplay(1);
	mp.begin();
	Serial.begin(115200);
	mp.callNumber("+385 92 381 8897");
}

void loop() {
	mp.display.fillScreen(TFT_BLACK);
	mp.update();
	//mp.callNumber("+385 92 151 8476");
	


	/*mp.lockScreen();
	mp.bigIconsMainMenu();*/
}