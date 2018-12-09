#include <stdio.h>
#include <Arduino.h>
#include <MAKERphone.h>
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
MAKERphone mp;

boolean paused = false;
//player variables
int player_score = 0;
int player_h = 16;
int player_w = 3;
int player_x = 0;
int player_y = (BUFHEIGHT - player_h) / 2;
int player_vy = 2;
//oponent variables
int oponent_score = 0;
int oponent_h = 16;
int oponent_w = 3;
int oponent_x = BUFWIDTH - oponent_w;
int oponent_y = (BUFHEIGHT - oponent_h) / 2;
int oponent_vy = 2;
//ball variables
int ball_size = 6;
int ball_x = BUFWIDTH - ball_size - oponent_w - 1;
int ball_y = (BUFHEIGHT - ball_size) / 2;
int ball_vx = 3;
int ball_vy = 3;

extern const byte font5x7[]; //get the default large font

///////////////////////////////////// SETUP
void setup() {
	mp.begin();
	mp.display.setTextFont(1); //change the font to the large one
	randomSeed(micros() * micros()); // can't use analogRad(0) as we have a speaker attached there
	
}

///////////////////////////////////// LOOP
void loop() {
	if (mp.update()) {
		if (mp.buttons.pressed(BTN_A))
		{
			mp.display.fillScreen(TFT_BLACK);
			mp.display.setCursor(16, 35);
			mp.display.setFreeFont(TT1);
			mp.display.setTextSize(2);
			mp.display.print("Paused");
			while(!mp.update());
			while (!mp.buttons.pressed(BTN_A)) {
				while(!mp.update());
				if (mp.buttons.pressed(BTN_B))
				{
					mp.loader();
				}
			}
				
			mp.display.setTextFont(1);
			mp.display.setTextSize(2);
		}
		if (mp.buttons.pressed(BTN_B))
		{
			mp.loader();
		}
		mp.display.fillScreen(TFT_BLACK);
		//move the player
		if (mp.buttons.repeat(JOYSTICK_D, 1)) {
			player_y = max(0, player_y - player_vy);
		}
		if (mp.buttons.repeat(JOYSTICK_B, 1)) {
			player_y = min(BUFHEIGHT - player_h, player_y + player_vy);
		}

		//move the ball
		ball_x = ball_x + ball_vx;
		ball_y = ball_y + ball_vy;

		//check for ball collisions
		//collision with the top border
		if (ball_y < 0) {
			ball_y = 0;
			ball_vy = -ball_vy;
			//mp.sound.playTick();
		}
		//collision with the bottom border
		if ((ball_y + ball_size) > BUFHEIGHT) {
			ball_y = BUFHEIGHT - ball_size;
			ball_vy = -ball_vy;
			//mp.sound.playTick();
		}
		//collision with the player
		if (mp.collideRectRect(ball_x, ball_y, ball_size, ball_size, player_x, player_y, player_w, player_h)) {
			ball_x = player_x + player_w;
			ball_vx = -ball_vx;
			//mp.sound.playTick();
		}
		//collision with the oponent
		if (mp.collideRectRect(ball_x, ball_y, ball_size, ball_size, oponent_x, oponent_y, oponent_w, oponent_h)) {
			ball_x = oponent_x - ball_size;
			ball_vx = -ball_vx;
			//mp.sound.playTick();
		}
		//collision with the left side
		if (ball_x < 0) {
			oponent_score = oponent_score + 1;
			//mp.sound.playCancel();
			ball_x = BUFWIDTH - ball_size - oponent_w - 1;
			ball_vx = -abs(ball_vx);
			ball_y = random(0, BUFHEIGHT - ball_size);
		}
		//collision with the right side
		if ((ball_x + ball_size) > BUFWIDTH) {
			player_score = player_score + 1;
			//mp.sound.playOK();
			ball_x = BUFWIDTH - ball_size - oponent_w - 16; //place the ball on the oponent side
			ball_vx = -abs(ball_vx);
			ball_y = random(0, BUFHEIGHT - ball_size);

		}
		//reset score when 10 is reached
		if ((player_score == 10) || (oponent_score == 10)) {
			player_score = 0;
			oponent_score = 0;
		}

		//move the oponent
		if ((oponent_y + (oponent_h / 2)) < (ball_y + (ball_size / 2))) { //if the ball is below the oponent
			oponent_y = oponent_y + oponent_vy; //move down
			oponent_y = min(BUFHEIGHT - oponent_h, oponent_y); //don't go out of the screen
		}
		else {
			oponent_y = oponent_y - oponent_vy; //move up
			oponent_y = max(0, oponent_y); //don't go out of the screen
		}



		//draw the score
		mp.display.setTextSize(2);
		mp.display.cursor_x = 15;
		mp.display.cursor_y = 16;
		mp.display.print(player_score);

		mp.display.cursor_x = 57;
		mp.display.cursor_y = 16;
		mp.display.print(oponent_score);
		//draw the ball
		mp.display.fillRect(ball_x, ball_y, ball_size, ball_size, TFT_WHITE);
		//draw the player
		mp.display.fillRect(player_x, player_y, player_w, player_h, TFT_RED);
		//draw the oponent
		mp.display.fillRect(oponent_x, oponent_y, oponent_w, oponent_h, TFT_BLUE);
	}
}
