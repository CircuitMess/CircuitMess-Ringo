/*
Phi-panel serial firmware version 1.6
----------------------------------------
Programmed by Dr. John Liu
Revision: 04/02/2012
All rights reserved. Free for non-commercial use.
----------------------------------------
This portion of code is the multi-tap function written by Dr. John Liu. It is provided to you without any warranty. Your use is limited to not-for-profit activities.
You may change the definition of the multi-tap keypad, which contains map_width symbols per numerical key for all 10 numerical keys.
To use the function:
1) Call your keypad's getKey method to receive a key. This key has to be '0' to '9' or other symbols such as '#' or NO_KEY or scan codes for phi_keypads objects.
2) Call multi_tap and pass the received key to it.
3) The returned value indicates what is going on.
If the most significant bit (MSB) is set to 1, then the multi-tap is in process. You should print the return value (with MSB reset to 0) in place.
If MSB is reset to 0, then the multi-tap has concluded and a final character is produced. You should print this character in place and move cursor to the next position.
If the return value is NO-KEY, then nothing is happening.
Pressing the '#' key engages or disengages shift to switch between upper and lower cases.
Calling the multi_tap with RESET_MTP will result in the multi_tap process to reset, useful if you are unsure of the function's status. Say you didn't finish a multi-tap process and switch to another piece of code and then later want to use multi-tap, you should reset it before using. The return value for this call is to be discarded.
Two symbols don't display correctly due to HD44780 font difference from ASCII
'\\'  or the back slash displays as a Yen symbol.
'~' or twiddle displays as a right arrow.
*/
// Please download phi_interfaces library from http://code.google.com/p/phi-prompt-user-interface-library/downloads/list
// To purchase a phi-panel to run this program http://www.inmojo.com/store/liudr-arduino-and-physics-gadgets/item/serial-lcd-back-pack---phi-panel/
#include <MAKERphone.h>
MAKERphone mp;
#define RESET_MTP '~'
#define map_width 7
#define LCD_RS 2
#define LCD_EN 3
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define lcd_rows 2
#define lcd_columns 16

#define buttons_per_column 4
#define buttons_per_row 4
#define multi_tap_threshold 500

#define blPin 10 // LCD back light pin
//
//char mapping[]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'}; // This is a matrix keypad.
//byte pins[]={17, 16, 15, 13, 12, 11, 9, 8}; // The first four pins are rows, the next 4 are columns. If you have 4*3 pad, then the first 4 are rows and the next 3 are columns.
//phi_matrix_keypads panel_keypad(mapping, pins, buttons_per_row, buttons_per_column);
//
int pointer = 0;
String buffer = "";
//LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7); // Create the lcd object

void setup()
{
	Serial.begin(115200);
	mp.begin(0);

}

void loop()
{
  int ret=0;
  byte key=mp.buttons.kpdNum.getKey(); // Get a key press from the keypad
  if (key == 'C' && buffer != "" && pointer >= 1)
  {
	  if (pointer == buffer.length())
		  pointer = pointer - 2;
	  else
		  pointer--;
	  buffer.remove(buffer.length() - 1);
  }
  else if (key == 'A') //clear number
  {
	  buffer = "";
	  pointer = 0;
  }
  if (key != 'B' && key != 'D' && key != 'A')
  {
	  ret = multi_tap(key);// Feed the key press to the multi_tap function.
	  if ((ret & 256) != 0) // If this is non-zero, we got a key. Handle some special keys or just print the key on screen
	  {
		  //switch (ret&255)
		  //{
		  //  case '\b':
		  //  if ((pointer%lcd_columns)==0)
		  //  {
		  //    if (rowp>0)
		  //    {
		  //      rowp--;
		  //      pointer=lcd_columns-2;
		  //    }
		  //  }
		  //  else pointer--;
		  //  lcd.setCursor(pointer,rowp);
		  //  lcd.write(' ');
		  //  lcd.setCursor(pointer,rowp);
		  //  return;
		  //  break;
		  //  case '\n':
		  //  pointer=0;
		  //  rowp++;
		  //  rowp%=lcd_rows;
		  //  lcd.setCursor(pointer,rowp);
		  //  return;
		  //  break;
		  //}
		  pointer++;

	  }
	  else if (ret) // We don't have a key but the user is still cycling through characters on one key so we need to update the screen
	  {

		  /*lcd.setCursor(pointer,rowp);
		  lcd.write(lowByte(ret));*/
		  if (pointer == buffer.length())
			  buffer += char(lowByte(ret));
		  else
			  buffer[buffer.length() - 1] = char(lowByte(ret));
	  }
  }
  Serial.print("Pointer");
  Serial.println(pointer);
  Serial.print("Buffer length");
  Serial.println(buffer.length());
  mp.display.fillScreen(TFT_BLACK);
  mp.display.setCursor(0, 5);
  mp.display.print(buffer);
  mp.update();
}

int multi_tap(byte key)
{
  static boolean upperCase=true;
  static byte prevKeyPress=NO_KEY,cyclicPtr=0;
  static unsigned long prevKeyMillis=0;
  static const char multi_tap_mapping[10][map_width]={{'0','#','$','.','?','"','&'},{'1','+','-','*','/','\'',':'},{'A','B','C','2','!',';','<'},{'D','E','F','3','%','[','='},{'G','H','I','4','(','\\','>'},{'J','K','L','5',')',']','^'},{'M','N','O','6','@','_','`'},{'P','Q','R','S','7','{','|'},{'T','U','V','8',',','}','~'},{'W','X','Y','Z','9',' ',0}};
  if (key==RESET_MTP) // Received reset command. Flush everything and get ready for restart.
  {
    upperCase=true;
    prevKeyPress=NO_KEY;
    cyclicPtr=0;
    return 0;
  }
  if (key!=NO_KEY) // A key is pressed at this iteration.
  {
	  if (key == 'C')
	  {
		  char temp1 = multi_tap_mapping[prevKeyPress - '0'][cyclicPtr];
		  if ((!upperCase) && (temp1 >= 'A') && (temp1 <= 'Z')) temp1 += 'a' - 'A';
		  prevKeyPress = NO_KEY;
		  cyclicPtr = 0;
		  return(256 + (unsigned int)(temp1));
	  }
     if ((key>'9')||(key<'0')) // Function keys
    {
      if ((key==1)||(key=='#')) // Up for case change
      {
        upperCase=!upperCase;
        return 0;
      }
      else // Other function keys. These keys produce characters so they need to terminate the last keypress.
      {
        if (prevKeyPress!=NO_KEY)
        {
          char temp1=multi_tap_mapping[prevKeyPress-'0'][cyclicPtr];
          if ((!upperCase)&&(temp1>='A')&&(temp1<='Z')) temp1+='a'-'A';
          cyclicPtr=0;
          prevKeyMillis=0;
          switch (key)
          {
            case 2:
            // Call symbol list
				return 0; // Clear the buffer.
				break;
            case 3:
				prevKeyPress='\b';
				break;
			case 4:
            case '*':
				prevKeyPress=' ';
				break;
            case 5:
				prevKeyPress='\n';
				break;
            case 6:
				prevKeyPress=NO_KEY; // Clear the buffer.
				break;
          }
          return(256+(unsigned int)(temp1));
        }
        else
        {
          prevKeyPress=NO_KEY;
          cyclicPtr=0;
          prevKeyMillis=0;
          switch (key)
          {
            case 2:
            // Call symbol list
            return 0; // Clear the buffer.
            break;
            case 3:
            return (256+(unsigned int)('\b'));
            break;
            case 4:
            return (256+(unsigned int)(' '));
            break;
            case 5:
            return (256+(unsigned int)('\n'));
            break;
            case 6:
            return 0; // Clear the buffer.
            break;
          }
        }
      }
    
    }
     if (prevKeyPress!=NO_KEY)
    {
      if (prevKeyPress==key)
      {
        char temp1;
        cyclicPtr++;
        if ((multi_tap_mapping[key-'0'][cyclicPtr]==0)||(cyclicPtr==map_width)) cyclicPtr=0; //Cycle key
        prevKeyMillis=millis();
        temp1=multi_tap_mapping[key-'0'][cyclicPtr];
        if ((!upperCase)&&(temp1>='A')&&(temp1<='Z')) temp1+='a'-'A';
        return ((unsigned int)(temp1));
      }
      else
      {
        char temp1=multi_tap_mapping[prevKeyPress-'0'][cyclicPtr];
        if ((!upperCase)&&(temp1>='A')&&(temp1<='Z')) temp1+='a'-'A';
        prevKeyPress=key;
        cyclicPtr=0;
        prevKeyMillis=millis();
        //Print key on cursor+1
        return(256+(unsigned int)(temp1));
      }
    }
    else
    {
      char temp1=multi_tap_mapping[key-'0'][cyclicPtr];
      if ((!upperCase)&&(temp1>='A')&&(temp1<='Z')) temp1+='a'-'A';
      prevKeyPress=key;
      prevKeyMillis=millis();
      cyclicPtr=0;
      return ((unsigned int)(temp1));
    }
  
  }
  else // No key is pressed at this iteration.
  {
    if (prevKeyPress==NO_KEY) return 0; // No key was previously pressed.
    else if (millis()-prevKeyMillis<multi_tap_threshold) // Key was pressed previously but within threshold
    {
      char temp1=multi_tap_mapping[prevKeyPress-'0'][cyclicPtr];
      if ((!upperCase)&&(temp1>='A')&&(temp1<='Z')) temp1+='a'-'A';
      return((unsigned int)(temp1));
    }
    else // Key was pressed previously and threshold has passed
    {
      char temp1=multi_tap_mapping[prevKeyPress-'0'][cyclicPtr];
      if ((!upperCase)&&(temp1>='A')&&(temp1<='Z')) temp1+='a'-'A';
      prevKeyPress=NO_KEY;
      cyclicPtr=0;
      return(256+(unsigned int)(temp1));
    }
  }
  return 0;
}
