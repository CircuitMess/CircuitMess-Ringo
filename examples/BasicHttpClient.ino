/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiMulti.h>
#include <SPI.h>
#include <SdFat.h>
#include <HTTPClient.h>
SdFat SD;
#define USE_SERIAL Serial

WiFiMulti wifiMulti;
WiFiClient stream;

const char* ca = \ 
"-----BEGIN CERTIFICATE-----\n" \  
"MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n" \  
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \  
"DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n" \  
"PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n" \
"Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
"AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n" \
"rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n" \
"OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n" \
"xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n" \
"7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n" \
"aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n" \
"HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n" \
"SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n" \
"ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n" \
"AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n" \
"R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n" \
"JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n" \
"Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n" \
"-----END CERTIFICATE-----\n";

void listDir(const char * dirname, uint8_t levels) {
	Serial.begin(115200);
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
		char temp[50];
		file.getName(temp, 50);
		String Name(temp);
		Serial.println(Name);
		file = root.openNextFile();

	}
}
void writeFile(const char * path, String message)
{
	while (!SD.begin(5, SD_SCK_MHZ(8)));
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
String readFile(const char * path) {
	while (!SD.begin(5, SD_SCK_MHZ(8)));
	Serial.printf("Reading file: %s\n", path);
	String helper = "";
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

void setup() {
	while(!SD.begin(5, SD_SCK_MHZ(8)));
	Serial.begin(115200);
    USE_SERIAL.begin(115200);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
	File file = SD.open("/UPDATE.BIN", FILE_WRITE);
    wifiMulti.addAP("ISKONOVAC-487af4", "ISKON2738000957");
	// wait for WiFi connection
	//File downloadFile = SD.open("/UPDATE.BIN", FILE_WRITE);
	if ((wifiMulti.run() == WL_CONNECTED)) {

		HTTPClient http;

		USE_SERIAL.print("[HTTP] begin...\n");
		// configure traged server and url
		http.begin("https://www.makerbuino.com/wp-content/uploads/2018/12/Invaders-Copy.txt", ca); //HTTPS
		//http.begin("http://example.com/index.html"); //HTTP

		USE_SERIAL.print("[HTTP] GET...\n");
		// start connection and send HTTP header
		int httpCode = http.GET();

		// httpCode will be negative on error
		if (httpCode > 0) {
			// HTTP header has been send and Server response header has been handled
			USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

			// file found at server
			if (httpCode == HTTP_CODE_OK) {
				http.writeToStream(&file);
				//stream = http.getStream();
				//SD.remove("/UPDATE.BIN");
				//writeFile("/UPDATE.BIN", String(stream.read()));
				//USE_SERIAL.println(String(stream.read()));
				//delay(5);
			}
		}
		else {
			USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
		}

		http.end();
		file.close();
		//listDir("/", 0);
		//USE_SERIAL.println(readFile("/UPDATE.BIN"));
	}
}

void loop() {
   

    delay(5000);
}
