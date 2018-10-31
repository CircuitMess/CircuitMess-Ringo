#include <MAKERphone.h>

MAKERphone mp;

void setup() {
	mp.display.invertDisplay(1);
	mp.begin();
	Serial.begin(115200);
	//mp.callNumber("0923781859");
}

void loop() {
	mp.lockScreen();
	mp.bigIconsMainMenu();
}