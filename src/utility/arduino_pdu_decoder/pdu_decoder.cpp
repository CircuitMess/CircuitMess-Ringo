//Implementation of a pdu decoder.

#include "Arduino.h"
#include "pdu_decoder.h"

String pdu_decode(String pdu_text) {
  uint8_t len = strtol(pdu_text.substring(0, 2).c_str(), NULL, 16); //the fist byte contains the number of chars
  return pdu_decode(pdu_text.substring(2), len);
}

String pdu_decode(String pdu_text, uint8_t len) {
  char plain_bytes[256];
  byte high_mask = 128; // byte:10000000;
  byte low_mask;
  byte shift = 0;
  byte this_byte;
  byte high_byte_new = 0;
  byte high_byte_old = 0;
  byte low_byte = 0;
  int y = 0;
  int i = 0;

  for (y = 0; y < len ; y++) {
    this_byte = strtol(pdu_text.substring(i * 2, (i * 2) + 2).c_str(), NULL, 16);

    low_mask = high_mask ^ 0xFF;                      //invert
    high_byte_new = this_byte & high_mask;            // 10000000 = 11000111 & 10000000
    low_byte = this_byte & low_mask;                  // 01000111 = 11000111 & 01111111

    plain_bytes[y] = low_byte << shift;               // 10001110
    high_byte_old = high_byte_old >> (8 - shift);     // 00000001
    plain_bytes[y] =  plain_bytes[y] + high_byte_old; // 10001111

    if (shift == 6) {
      y++;
      plain_bytes[y] = high_byte_new >> 1;
    }

    high_mask = high_mask >> 1;                       // 10000000 > 01000000
    high_mask = high_mask + 128;                      // 01000000 > 11000000
    if (high_mask == 255)high_mask = 128;             // 11111111 > 10000000
    low_mask = high_mask ^ 0xFF;                      // invert

    high_byte_old = high_byte_new;
    shift++;
    if (shift == 7) {
      shift = 0;
    }
    i++;
  }
  plain_bytes[y] = 0;

  char *plain_text = new char[y];
  strcpy(plain_text, plain_bytes);
  
  return plain_text;
}
