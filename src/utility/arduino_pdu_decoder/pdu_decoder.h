//Library to decode pdu (8 bit octets representing 7 bit data).

#ifndef pdu_decoder_h
#define pdu_decoder_h

#include "Arduino.h"

String pdu_decode(String pdu_text);
String pdu_decode(String pdu_text, uint8_t len);

#endif
