//Library to decode pdu (8 bit octets representing 7 bit data).

#ifndef pdu_decoder_h
#define pdu_decoder_h

#include "Arduino.h"

char* charReverse(char *str);
void subchar(char* str, uint16_t start, uint16_t length, char* destination);
void subchar(const char* str, uint16_t start, uint16_t length, char* destination);
void myDecode(char* pdu_text, uint8_t len, char* destination, uint8_t zerosPadding);
#endif
