#include "Arduino.h"
#include "pdu_decoder.h"
struct MapAsciiToGSM7bit
{
	char const* GSM7bitValue;
	char AsciiValue;
};
MapAsciiToGSM7bit my_map[93] ={
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
 // 11011 0x1B
  //101001 0x29
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
  {"1B3D", '~'},
};

void subchar(char* str, uint16_t start, uint16_t length, char* destination)
{
    // char* ret = new char[length + 1];
    for (short i = start; i < start + length; i++)
        destination[i - start] = str[i];
    destination[length] = '\0';
}
void subchar(const char* str, uint16_t start, uint16_t length, char* destination)
{
    for (short i = start; i < start + length; i++)
        destination[i - start] = str[i];
    destination[length] = '\0';
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

void myDecode(char* pdu_text, uint8_t len, char* destination, uint8_t zerosPadding)
{
  bool escapeFlag = 0;
  bool buffer2[1288];
  memset(buffer2, 0, 1288);
  uint16_t cursor = 0;

  for(int i = 0; i < len; i++)
  {
    char temp[3];
    memset(temp, 0 , 3);
    subchar(pdu_text, i * 2, 2, temp);
    uint8_t current = strtol(temp, NULL, 16);
    for(int y = 0; y < 8; y++)
    {
      buffer2[cursor] = bitRead(current, y);
      cursor++;
    }
  }
  cursor = zerosPadding;
  for(int i = 0; i < len;i++)
  {
    uint8_t current = 0;
    for(int y = 0; y < 7; y++)
    {
      bitWrite(current, y, buffer2[cursor]);
      cursor++;
    }
    // Serial.println(current);
    if(escapeFlag)
    {
      escapeFlag = 0;
      // Serial.println(plain_bytes[y], HEX);
      for(int z = 0; z < 93; z++)
        if(strlen(my_map[z].GSM7bitValue) > 3)
        {
          char temp[3];
          memset(temp, 0 , 3);
          subchar(my_map[z].GSM7bitValue, 2, 2, temp);
          if(strtol(temp, nullptr, 16) == current)
            current = my_map[z].AsciiValue;
        }
    }
    else if(current == 0x1B)
      escapeFlag = 1;
    else if(current < 21)
    {
      // Serial.println(subchar(pdu_text, (i+1) * 2, 2));
      for(int z = 0; z < 93; z++)
        if(strtol(my_map[z].GSM7bitValue, nullptr, 16) == current)
          current = my_map[z].AsciiValue;
    }
    if(current != 0x1B)
    {
      char c = current;
      destination[i] = c;
    }
  }
  destination[len] = '\0';
  // Serial.println(destination);
  // for(int i = 0; i < len; i++)
  // {
  //   uint8_t current = strtol(subchar(pdu_text, i * 2, 2), NULL, 16);
  //   uint8_t lockedCurrent = current;
  //   current = current << shift;
  //   current = current >> shift;
  //   current = current << (shift - 1);

  //   nextMask = nextMask >> (9 - shift);
  //   current  = current | nextMask;
  //   uint8_t helper = 0xFF << (8 - shift);
  //   nextMask = helper & lockedCurrent;

  //   // Serial.print("shift: ");
  //   // Serial.println(shift);
  //   Serial.println((char)current);

  //   shift++;
  //   if(shift > 7)
  //     shift = 1;
    
  //   if(escapeFlag)
  //   {
  //     escapeFlag = 0;
  //     // Serial.println(plain_bytes[y], HEX);
  //     for(int z = 0; z < 93; z++)
  //       if(strlen(my_map[z].GSM7bitValue) > 3)
  //         if(strtol(subchar(my_map[z].GSM7bitValue, 2, 2), nullptr, 16) == current)
  //           current = my_map[z].AsciiValue;
  //   }
  //   else if(current == 0x1B)
  //     escapeFlag = 1;
  //   // else if(current < 21)
  //   // {
  //   //   // Serial.println(subchar(pdu_text, (i+1) * 2, 2));
  //   //   for(int z = 0; z < 93; z++)
  //   //     if(strtol(my_map[z].GSM7bitValue, nullptr, 16) == current)
  //   //       current = my_map[z].AsciiValue;
  //   // }
  //   if(current != 0x1B)
  //   {
  //     char c = current;
  //     strncat(buffer, &c, 1);
  //   }
  //   // Serial.print("current: ");
  //   // Serial.println(current, HEX);
  //   // Serial.println("----------------");
    
  //   // buffer[i] = (char)current;
  // }

  // free(buffer);
  // free(buffer2);
}
