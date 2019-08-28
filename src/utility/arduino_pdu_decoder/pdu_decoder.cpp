//Implementation of a pdu decoder.

#include "Arduino.h"
#include "pdu_decoder.h"
char* subchar(char* str, uint16_t start, uint16_t length)
{
    char* ret = new char[length + 1];
    for (short i = start; i < start + length; i++)
        ret[i - start] = str[i];
    ret[length] = '\0';
    return ret;
}
char* charReverse(char *str) {
  size_t len = strlen(str);
  size_t i = 0;
  while (len > i) {
    char tmp = str[--len];
    str[len] = str[i];
    str[i++] = tmp;
  }
  return str;
}
char* pdu_decode(char* pdu_text) {
  uint8_t len = strtol(subchar(pdu_text, 0, 2), NULL, 16); //the fist byte contains the number of chars
  return pdu_decode(subchar(pdu_text, 2, strlen(pdu_text) - 2), len);
}
char* pdu_decode(char* pdu_text, uint8_t len) {
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
    this_byte = strtol(subchar(pdu_text, i * 2, 2), NULL, 16);
    
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
  for(int x = 0; x < len; x++)
  {
    Serial.println((uint8_t)plain_bytes[x]);
  }
  char *plain_text = new char[y];
  strcpy(plain_text, plain_bytes);
  
  return plain_text;
}
