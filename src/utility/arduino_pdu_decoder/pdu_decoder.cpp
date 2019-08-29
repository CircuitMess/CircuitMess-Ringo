//Implementation of a pdu decoder.

#include "Arduino.h"
#include "pdu_decoder.h"
struct MapAsciiToGSM7bit
{
	char const* GSM7bitValue;
	char AsciiValue;
};
MapAsciiToGSM7bit my_map[93] =
{
  {"21", '!'},
  {"22", '"'},
  {"23", '#'},
  {"02", '$'},
  {"25", '%'},
  {"26", '&'},
  {"27", '\''},
  {"28", '('},
  {"29", ')'},
  {"2A", '*'},
  {"2B", '+'},
  {"2C", ','},
  {"2D", '-'},
  {"2E", '.'},
  {"2F", '/'},
  {"30", '0'},
  {"31", '1'},
  {"32", '2'},
  {"33", '3'},
  {"34", '4'},
  {"35", '5'},
  {"36", '6'},
  {"37", '7'},
  {"38", '8'},
  {"39", '9'},
  {"3A", ':'},
  {"3B", ';'},
  {"3C", '<'},
  {"3D", '='},
  {"3E", '>'},
  {"3F", '?'},
  {"00", '@'},
  {"41", 'A'},
  {"42", 'B'},
  {"43", 'C'},
  {"44", 'D'},
  {"45", 'E'},
  {"46", 'F'},
  {"47", 'G'},
  {"48", 'H'},
  {"49", 'I'},
  {"4A", 'J'},
  {"4B", 'K'},
  {"4C", 'L'},
  {"4D", 'M'},
  {"4E", 'N'},
  {"4F", 'O'},
  {"50", 'P'},
  {"51", 'Q'},
  {"52", 'R'},
  {"53", 'S'},
  {"54", 'T'},
  {"55", 'U'},
  {"56", 'V'},
  {"57", 'W'},
  {"58", 'X'},
  {"59", 'Y'},
  {"5A", 'Z'},
  {"1B3C", '['},
  {"1B2F", '\\'},
  {"1B3E", ']'},
  {"1B14", '^'},
  {"11", '_'},
  // {"21", '`'}, //grave accent replaced with asterisk
  {"61", 'a'},
  {"62", 'b'},
  {"63", 'c'},
  {"64", 'd'},
  {"65", 'e'},
  {"66", 'f'},
  {"67", 'g'},
  {"68", 'h'},
  {"69", 'i'},
  {"6A", 'j'},
  {"6B", 'k'},
  {"6C", 'l'},
  {"6D", 'm'},
  {"6E", 'n'},
  {"6F", 'o'},
  {"70", 'p'},
  {"71", 'q'},
  {"72", 'r'},
  {"73", 's'},
  {"74", 't'},
  {"75", 'u'},
  {"76", 'v'},
  {"77", 'w'},
  {"78", 'x'},
  {"79", 'y'},
  {"7A", 'z'},
  {"1B28", '{'},
  {"1B40", '|'},
  {"1B29", '}'},
  {"1B39", '~'},
};

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
    if(plain_bytes[y] < 21)
    {
      for(int z = 0; z < 93; z++)
        if(strtol(my_map[z].GSM7bitValue, nullptr, 16) == plain_bytes[y])
          plain_bytes[y] = my_map[z].AsciiValue;
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
