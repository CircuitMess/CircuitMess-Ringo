#include <stdio.h>
#include <Arduino.h>
#include <MAKERphone.h>
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
MAKERphone mp;

bool gameState = 0;
//player variables
int playerScore = 0;
int playerHeight = 16;
int playerWidth = 3;
int playerX = 0;
int playerY = (mp.display.width() - playerHeight) / 2;
int playerSpeedY = 2;
//oponent variables
int opponentScore = 0;
int opponentHeight = 16;
int opponentWidth = 3;
int opponentX = mp.display.width() - opponentWidth;
int opponentY = (mp.display.height() - opponentHeight) / 2;
int opponentSpeedY = 2;
//ball variables
int ballSize = 6;
int ballX = mp.display.width() - ballSize - opponentWidth - 1;
int ballY = (mp.display.height() - ballSize) / 2;
int ballSpeedX = 3;
int ballSpeedY = 3;

const byte title[] PROGMEM = {56,20,
  B11111100,B00000000,B11110000,B00001100,B00110000,B00001111,B10000000,
  B11100110,B00000001,B11111000,B00001100,B00110000,B00011111,B10000000,
  B11100011,B00000011,B11001100,B00001110,B00110000,B00111000,B00000000,
  B11100011,B00000011,B10000100,B00001110,B00110000,B00110000,B00000000,
  B11100011,B00000011,B10000100,B00001110,B00110000,B00110000,B00000000,
  B11100011,B00000011,B10000100,B00001111,B00110000,B00110000,B00000000,
  B11100011,B00000011,B10000100,B00001111,B00110000,B00110000,B00000000,
  B11100011,B00000011,B10000100,B00001111,B00110000,B00110000,B00000000,
  B11100011,B00000011,B10000100,B00001101,B10110000,B00110000,B00000000,
  B11100011,B00000011,B10000100,B00001101,B10110000,B00110000,B00000000,
  B11100110,B00000011,B10000100,B00001101,B10110000,B00110000,B00000000,
  B11111100,B00000011,B10000100,B00001100,B11110000,B00110000,B00000000,
  B11100000,B00000011,B10000100,B00001100,B11110000,B00110001,B11000000,
  B11100000,B00000011,B10000100,B00001100,B11110000,B00110001,B11000000,
  B11100000,B00000011,B10000100,B00001100,B01110000,B00110000,B11000000,
  B11100000,B00000011,B10000100,B00001100,B01110000,B00110000,B11000000,
  B11100000,B00000011,B10000100,B00001100,B01110000,B00110000,B11000000,
  B11100000,B00000011,B11001100,B00001100,B00110000,B00111001,B11000000,
  B11100000,B00000001,B11111000,B00001100,B00110000,B00011111,B10000000,
  B11100000,B00000000,B11110000,B00001100,B00110000,B00001111,B00000000,
};

void setup() {
	mp.begin(0);
  mp.setResolution(1);
  mp.display.setTextFont(1); //change the font to the large one
	randomSeed(micros() * micros()); // can't use analogRad(0) as we have a speaker attached there
	
}

void loop()
{
  if(!gameState)
  {
    bool cursor = 0;
    bool blinkState = 0;
    uint32_t blinkMillis = millis();
    while(!mp.buttons.released(BTN_A))
    {
      mp.display.fillScreen(TFT_BLACK);
      mp.display.drawBitmap(15, 7, title, TFT_WHITE);
      mp.display.setTextSize(1);
      mp.display.setTextFont(1);
      mp.display.setCursor(32, 36);
      mp.display.setTextColor(TFT_WHITE);
      mp.display.printCenter("1 PLAYER");
      mp.display.setCursor(46, 48);
      mp.display.setTextColor(TFT_DARKGREY);
      mp.display.printCenter("2 PLAYER");
      if(!cursor)
      {
        mp.display.drawRect(14, 34, 51, 11, blinkState ? TFT_RED : TFT_BLACK);
      }
      if(millis()-blinkMillis >= 200)
      {
        blinkState = !blinkState;
        blinkMillis = millis();
      }
      if(mp.buttons.released(BTN_B))
        mp.loader();
      mp.update();
    }
		//Starting game - setting all values to defaults
    gameState = 1;
    mp.display.setTextColor(TFT_WHITE);
    mp.display.setTextSize(2);
    playerScore = 0;
    playerX = 0;
		playerY = (mp.display.width() - playerHeight) / 2;
    playerSpeedY = 2;
    //oponent variables
    opponentScore = 0;
    opponentX = mp.display.width() - opponentWidth;
    opponentY = (mp.display.height() - opponentHeight) / 2;
    opponentSpeedY = 2;
    //ball variables
    ballX = mp.display.width() - ballSize - opponentWidth - 1;
    ballY = (mp.display.height() - ballSize) / 2;
    ballSpeedX = 3;
    ballSpeedY = 3;
  }
  if (mp.update()) {
		if (mp.buttons.released(BTN_B))
		{
			mp.display.fillScreen(TFT_BLACK);
			mp.display.setCursor(0, mp.display.height()/2 - 12);
			mp.display.setTextFont(2);
			mp.display.setTextSize(1);
			mp.display.printCenter("Paused");
      mp.display.setCursor(2, 55);
      mp.display.setFreeFont(TT1);
      mp.display.printCenter("A: resume      B:quit");
      while(!mp.update());
			while (!mp.buttons.released(BTN_A)) {
				if (mp.buttons.released(BTN_B))
        {
          gameState = 0;
          while (!mp.update());
          break;
        }
        while (!mp.update());
      }
				
			mp.display.setTextFont(1);
			mp.display.setTextSize(2);
		}
    mp.display.fillScreen(TFT_BLACK);
		//move the player
		if (mp.buttons.repeat(JOYSTICK_D, 1)) {
			playerY = max(0, playerY - playerSpeedY);
		}
		if (mp.buttons.repeat(JOYSTICK_B, 1)) {
			playerY = min(mp.display.height() - playerHeight, playerY + playerSpeedY);
		}

		//move the ball
		ballX = ballX + ballSpeedX;
		ballY = ballY + ballSpeedY;

		//check for ball collisions
		//collision with the top border
		if (ballY < 0) {
			ballY = 0;
			ballSpeedY = -ballSpeedY;
			//mp.sound.playTick();
		}
		//collision with the bottom border
		if ((ballY + ballSize) > mp.display.height()) {
			ballY = mp.display.height() - ballSize;
			ballSpeedY = -ballSpeedY;
			//mp.sound.playTick();
		}
		//collision with the player
		if (mp.collideRectRect(ballX, ballY, ballSize, ballSize, playerX, playerY, playerWidth, playerHeight)) {
			ballX = playerX + playerWidth;
			ballSpeedX = -ballSpeedX;
			//mp.sound.playTick();
		}
		//collision with the oponent
		if (mp.collideRectRect(ballX, ballY, ballSize, ballSize, opponentX, opponentY, opponentWidth, opponentHeight)) {
			ballX = opponentX - ballSize;
			ballSpeedX = -ballSpeedX;
			//mp.sound.playTick();
		}
		//collision with the left side
		if (ballX < 0) {
			opponentScore = opponentScore + 1;
			//mp.sound.playCancel();
			ballX = mp.display.width() - ballSize - opponentWidth - 1;
			ballSpeedX = -abs(ballSpeedX);
			ballY = random(0, mp.display.height() - ballSize);
		}
		//collision with the right side
		if ((ballX + ballSize) > mp.display.width()) {
			playerScore = playerScore + 1;
			//mp.sound.playOK();
			ballX = mp.display.width() - ballSize - opponentWidth - 16; //place the ball on the oponent side
			ballSpeedX = -abs(ballSpeedX);
			ballY = random(0, mp.display.height() - ballSize);

		}
		//reset score when 10 is reached
		if ((playerScore == 10) || (opponentScore == 10)) {
			playerScore = 0;
			opponentScore = 0;
		}

		//move the oponent
		if ((opponentY + (opponentHeight / 2)) < (ballY + (ballSize / 2))) { //if the ball is below the oponent
			opponentY = opponentY + opponentSpeedY; //move down
			opponentY = min(BUFHEIGHT - opponentHeight, opponentY); //don't go out of the screen
		}
		else {
			opponentY = opponentY - opponentSpeedY; //move up
			opponentY = max(0, opponentY); //don't go out of the screen
		}



		//draw the score
		mp.display.setTextSize(2);
		mp.display.setCursor(15, 16);
		mp.display.print(playerScore);
		mp.display.setCursor(57, 16);
		mp.display.print(opponentScore);
		//draw the ball
		mp.display.fillRect(ballX, ballY, ballSize, ballSize, TFT_WHITE);
		//draw the player
		mp.display.fillRect(playerX, playerY, playerWidth, playerHeight, TFT_RED);
		//draw the oponent
		mp.display.fillRect(opponentX, opponentY, opponentWidth, opponentHeight, TFT_BLUE);
	}
}
