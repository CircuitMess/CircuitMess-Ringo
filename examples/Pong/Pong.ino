#include <stdio.h>
#include <Arduino.h>
#include <MAKERphone.h>
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
MAKERphone mp;

bool gameState = 0;
//player variables
int playerScore = 0;
int playerHeight = 32;
int playerWidth = 6;
int playerX = 5;
int playerY = (mp.display.width() - playerHeight) / 2;
int playerSpeedY = 4;
//oponent variables
int opponentScore = 0;
int opponentHeight = 32;
int opponentWidth = 6;
int opponentX = mp.display.width() - opponentWidth - 5;
int opponentY = (mp.display.height() - opponentHeight) / 2;
int opponentSpeedY = 4;
//ball variables
int ballSize = 12;
int ballX = mp.display.width() - ballSize - opponentWidth - 1;
int ballY = (mp.display.height() - ballSize) / 2;
int ballSpeedX = 5;
int ballSpeedY = 5;

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
  // mp.setResolution(0);
  mp.display.setTextFont(2); //change the font to the large one
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
      mp.display.drawBitmap(30, 14, title, TFT_WHITE, 2);
      mp.display.setTextSize(1);
      mp.display.setTextFont(2);
      mp.display.setCursor(32, 72);
      mp.display.setTextColor(TFT_WHITE);
      mp.display.printCenter("1 PLAYER");
      mp.display.setCursor(46, 96);
      mp.display.setTextColor(TFT_DARKGREY);
      mp.display.printCenter("2 PLAYER");
      if(!cursor)
      {
        mp.display.drawRect(30, 69, 100, 22, blinkState ? TFT_RED : TFT_BLACK);
        mp.display.drawRect(31, 70, 98, 20, blinkState ? TFT_RED : TFT_BLACK);
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
    playerScore = 0;
    playerX = 5;
		playerY = (mp.display.width() - playerHeight) / 2;
    playerSpeedY = 4;
    //oponent variables
    opponentScore = 0;
    opponentX = mp.display.width() - opponentWidth - 5;
    opponentY = (mp.display.height() - opponentHeight) / 2;
    opponentSpeedY = 4;
    //ball variables
    ballX = mp.display.width() - ballSize - opponentWidth - 1;
    ballY = (mp.display.height() - ballSize) / 2;
    ballSpeedX = 6;
    ballSpeedY = 6;
  }
  if (mp.update()) {
		if (mp.buttons.released(BTN_B))
		{
			mp.display.fillScreen(TFT_BLACK);
			mp.display.setCursor(0, mp.display.height()/2 - 30);
			mp.display.setTextFont(2);
			mp.display.setTextSize(2);
			mp.display.printCenter("Paused");
      mp.display.setCursor(4, 110);
			mp.display.setTextSize(1);
      mp.display.printCenter("A: resume         B: quit");
      mp.update();
			while (!mp.buttons.released(BTN_A)) {
				if (mp.buttons.released(BTN_B))
        {
          gameState = 0;
          mp.update();
          break;
        }
        mp.update();
      }
				
			mp.display.setTextFont(1);
			mp.display.setTextSize(2);
		}
    mp.display.setTextSize(5);
    mp.display.setTextFont(1);
    mp.display.fillScreen(TFT_BLACK);
		//move the player
		if (mp.buttons.repeat(BTN_UP, 1)) {
			playerY = max(0, playerY - playerSpeedY);
		}
		if (mp.buttons.repeat(BTN_DOWN, 1)) {
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
			opponentY = min(mp.display.height() - opponentHeight, opponentY); //don't go out of the screen
		}
		else {
			opponentY = opponentY - opponentSpeedY; //move up
			opponentY = max(0, opponentY); //don't go out of the screen
		}



		//draw the score
		mp.display.setTextSize(4);
		mp.display.setCursor(30, 32);
		mp.display.print(playerScore);
		mp.display.setCursor(114, 32);
		mp.display.print(opponentScore);
		//draw the ball
		mp.display.fillRect(ballX, ballY, ballSize, ballSize, TFT_WHITE);
		//draw the player
		mp.display.fillRect(playerX, playerY, playerWidth, playerHeight, TFT_RED);
		//draw the oponent
		mp.display.fillRect(opponentX, opponentY, opponentWidth, opponentHeight, TFT_BLUE);
	}
}
