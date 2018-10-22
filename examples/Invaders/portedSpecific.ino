//----------------------------------------------------------------------------
void checkbuttons() {
	if (shipx < 0) shipx = 0;

	if (mp.buttons.repeat(JOYSTICK_A, 1) && shipx > 0 && deadcounter == -1) { shipx = --shipx; }
	if (mp.buttons.repeat(JOYSTICK_C, 1) and shipx < Maxwhidth and deadcounter == -1) { shipx = ++shipx; }
	//if (buttonsPressed(BUTTON_MENU)) {
	//	 gb.titleScreen(F("    Yoda's"),gamelogo);
	//	gamestatus = "title";
	//}
	if (mp.buttons.pressed(BTN_A) and shotx == -1 and deadcounter == -1) {
		shotx = shipx + 3;
		shoty = 43;
		for (uint8_t i = 0; i < 8; i++){
      mp.pixels.setPixelColor(i, mp.hslRed);
    //mp.pixels.show();
    pixelsTimer=millis();
    pixelsState=1;  
	  }
	}
  if(millis()-pixelsTimer >= 50 && pixelsState==1){
    for (uint8_t i = 0; i < 8; i++)
      mp.pixels.setPixelColor(i, mp.hslBlack);
    //mp.pixels.show();
    pixelsState=0;
	}
}
//----------------------------------------------------------------------------
void drawplayership() {
	if (deadcounter == -1) {
		mp.display.drawBitmap(shipx, 46, playership[0], TFT_WHITE);
	}
	else {
		mp.display.drawBitmap(shipx, 46, playership[1 + invadershotframe], TFT_YELLOW);
		handledeath();
	}
}
//----------------------------------------------------------------------------
void drawplayershot() {
	if (shotx != -1) {
		shoty = shoty - 2;
		mp.display.drawLine(shotx, shoty, shotx, shoty + 2, TFT_YELLOW);
		if (shoty < 0) {
			shotx = -1;
			shoty = -1;
		}
	}
}
//----------------------------------------------------------------------------
void invaderlogic() {
	// increment invader counter
	if (invaderanz > 0) {
		checkdir = 0;
		do {
			invaderctr = ++invaderctr;
			if (invaderctr > 39) {
				invaderctr = 0;
				checkdir = 1;
				invadersound = ++invadersound % 4;
				if (delayBip <= 0) {
					//gb.sound.fx(inv_move);
					if (invaderanz < 6) {
						delayBip = 5;
					}
					else if (invaderanz < 11) {
						delayBip = 3;
					} if (invaderanz < 21) {
						delayBip = 2;
					}
				}
				else { delayBip--; }
			}
		} while (invaders[invaderctr] == -1);

		// change direction?
		if (checkdir == 1) {
			if (nextydir != 0) {
				invaderxr = 0;
				invaderyr = 2;
			}
			else {
				invaderxr = nextxdir;
				invaderyr = 0;
			}
			checkdir = 0;
		}

		// change invader position
		invaderx[invaderctr] = invaderx[invaderctr] + invaderxr;
		invadery[invaderctr] = invadery[invaderctr] + invaderyr;

		// determine bunker removal if invaders are too low
		if (invadery[invaderctr] > 34) {
			for (int i = 0; i < 4; i++) {
				bunkers[i] = -1;
			}
		}

		// determine game over if invaders reach bottom
		if (invadery[invaderctr] > 40) {
			gamestatus = "gameover";
		}

		// determine screen border hit -> go down, then change direction
		if (invaderx[invaderctr] > 75 and invaderxr > 0) {
			nextxdir = -2;
			nextydir = 2;
		}
		if (invaderx[invaderctr] < 2 and invaderxr < 0) {
			nextxdir = 2;
			nextydir = 2;
		}
		if (invaderyr != 0) { nextydir = 0; }

		//change invader shape
		invaderframe[invaderctr] = ++invaderframe[invaderctr] % 2;

		// remove killed invader
		if (invaders[invaderctr] == 6) {
			invaders[invaderctr] = -1;
			invaderanz = --invaderanz;
		}

		// release invadershoot
		if (invadershottimer <= 0 and invadershots < gamelevel + 1 and invadershots < 4 and invadery[invaderctr] < 40) {
			invadershottimer = 40 - gamelevel * 10;
			invadershots = ++invadershots;
			int flag = 0;
			for (int u = 0; u < 4; u++) {
				if (flag == 0 and invadershotx[u] == -1) {
					invadershotx[u] = invaderx[invaderctr] + 1;
					invadershoty[u] = invadery[invaderctr];
					flag = 1;
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void drawinvaders() {
	infoshow = 1;
	for (int i = 0; i < 40; i++) {
		if (invaders[i] != -1) {
			if (invaders[i] == 0) mp.display.drawBitmap(invaderx[i], invadery[i], invader[invaders[i] + invaderframe[i]], TFT_ORANGE);
			if (invaders[i] == 2) mp.display.drawBitmap(invaderx[i], invadery[i], invader[invaders[i] + invaderframe[i]], TFT_PINK);
			if (invaders[i] == 4) mp.display.drawBitmap(invaderx[i], invadery[i], invader[invaders[i] + invaderframe[i]], TFT_BLUE);
			if (invaders[i] == 6) mp.display.drawBitmap(invaderx[i], invadery[i], invader[invaders[i] + invaderframe[i]], TFT_YELLOW);
			
			if (invadery[i] < 5) {
				infoshow = 0;
			}
		}

		// determine collission: invader & player shoot
		checkl = invaderx[i];
		checkr = invaderx[i] + 6;
		checkt = invadery[i];
		checkb = invadery[i] + 4;
		if (invaders[i] == 4) {
			checkl = ++checkl;
			checkr = --checkr;
		}
		if (invaders[i] != -1 and invaders[i] != 6 and shotx >= checkl and shotx <= checkr and shoty + 2 >= checkt and shoty <= checkb) {
			score = score + invaders[i] * 10 + 10;
			invaders[i] = 6;
			shotx = -1;
			shoty = -1;
			//gb.sound.fx(laser);
		}
	}
}
//----------------------------------------------------------------------------
void invadershot() {
	// handle invadershoot timer & framecounter
	invadershottimer = --invadershottimer;
	invadershotframe = ++invadershotframe % 2;

	// move invadershots, draw & check collission
	for (int i = 0; i < 4; i++) {
		if (invadershotx[i] != -1) {
			invadershoty[i] = invadershoty[i] + 1;
			mp.display.drawBitmap(invadershotx[i], invadershoty[i], bomb[invadershotframe], TFT_RED);

			// check collission: invadershot & bunker
			for (int u = 0; u < 4; u++) {
				checkl = 11 + u * 18;
				checkr = 11 + u * 18 + 7;
				checkt = 39;
				checkb = 43;
				if (bunkers[u] != -1 and invadershotx[i] + 1 >= checkl and invadershotx[i] <= checkr and invadershoty[i] + 3 >= checkt and invadershoty[i] <= checkb) {
					bunkers[u] = ++bunkers[u];
					if (bunkers[u] > 4) { bunkers[u] = -1; }
					invadershotx[i] = -1;
					invadershoty[i] = -1;
					invadershots = --invadershots;
				}
			}

			// check collission: invadershot & player
			checkl = shipx;
			checkr = shipx + 6;
			checkt = 46;
			checkb = 49;
			if (deadcounter == -1 and invadershotx[i] + 1 >= checkl and invadershotx[i] <= checkr and invadershoty[i] + 3 >= checkt and invadershoty[i] <= checkb) {
				deadcounter = 60;
				//gb.sound.fx(explosion);
			}

			//check collission: invadershot & playershoot
			checkl = invadershotx[i];
			checkr = invadershotx[i] + 1;
			checkt = invadershoty[i];
			checkb = invadershoty[i] + 3;
			if (shotx >= checkl and shotx <= checkr and shoty + 2 >= checkt and shoty <= checkb) {
				shotx = -1;
				shoty = -1;
				invadershotx[i] = -1;
				invadershoty[i] = -1;
				invadershots = --invadershots;
			}

			// invadershoot on bottom off screen?
			if (invadershoty[i] > 48) {
				invadershotx[i] = -1;
				invadershoty[i] = -1;
				invadershots = --invadershots;
			}
		}
	}
}
//----------------------------------------------------------------------------
void drawbunkers() {
	for (int i = 0; i < 4; i++) {
		checkl = 11 + i * 18;
		checkr = 11 + i * 18 + 7;
		checkt = 39;
		checkb = 43;
		if (bunkers[i] != -1 and shotx >= checkl and shotx <= checkr and shoty + 2 >= checkt and shoty <= checkb) {
			bunkers[i] = ++bunkers[i];
			shotx = -1;
			shoty = -1;
			if (bunkers[i] > 4) { bunkers[i] = -1; }
		}

		if (bunkers[i] != -1) {
			mp.display.drawBitmap(11 + i * 18, 39, bunker[bunkers[i]], TFT_GREEN);
		}
	}
}
//----------------------------------------------------------------------------
void saucerappears() {
	saucertimer = --saucertimer;
	if (saucertimer <= 0) {
		saucertimer = 240;
		if (infoshow == 1 and saucers == -1) {
			saucers = 0;
			int i = random(2);
			if (i == 0) {
				saucerx = 0;
				saucerdir = 1;
			}
			else {
				saucerx = 73;
				saucerdir = -1;
			}
		}
	}
}
//----------------------------------------------------------------------------
void movesaucer() {
	if (saucers == 0) {
		saucerx = saucerx + saucerdir;
		if (saucerx <= 0 or saucerx >= 73) {
			saucers = -1;
		}
		if (saucerx % 5 == 0) {
			// playsoundfx(3,0); 
			//gb.sound.fx(ufo);
		}

		// check collission: player shot & saucer  
		checkl = saucerx;
		checkr = saucerx + 10;
		checkt = 0;
		checkb = 3;
		if (shotx >= checkl and shotx <= checkr and shoty + 2 >= checkt and shoty <= checkb) {
			score = score + 100;
			saucers = 1;
			shotx = -1;
			shoty = -1;
			saucerwait = 30;
			// playsoundfx(1,0);
			//gb.sound.fx(explosion);
		}
	}
}
//----------------------------------------------------------------------------
void drawsaucer() {
	if (saucers != -1) {
		mp.display.drawBitmap(saucerx, 0, saucer[saucers], TFT_RED);
		if (saucers == 1) {
			saucerwait = --saucerwait;
			if (saucerwait <= 0) {
				saucers = -1;
			}
		}
	}
}
#define STAR_COUNT 50            // Number of stars on the screen. Arduino UNO maxes out around 250.
#define BACKGROUND_COLOR 0x0000   // Background color in hex. 0x0000 is black.
#define STAR_SPEED_MIN 1          // Minimum movement in pixels per update. (value is inclusive)
#define STAR_SPEED_MAX 3         // Maximum movement in pixels per update. (value is inclusive)
#define STAR_COLOR 0xffff  
Star stars[STAR_COUNT]; 
void starsSetup()
{
    // Loop through each star.
    for(int i = 0; i < STAR_COUNT; i++)
    {
        // Randomize its position and speed.
        stars[i].randomize(0, 80, 0, 64, STAR_SPEED_MIN, STAR_SPEED_MAX);
    }
}  
//----------------------------------------------------------------------------
void showtitle() {
  mp.display.fillScreen(TFT_BLACK);
  for(int i = 0; i < STAR_COUNT; i++)
    {
        // Remove the star from the screen by changing its pixel to the background color.
        mp.display.drawPixel(stars[i].x, stars[i].y, BACKGROUND_COLOR);

        // Update the position of the star.
        stars[i].update();

        // If the star's Y position is off the screen after the update.
        if(stars[i].y >= 64)
        {
            // Randomize its position.
            stars[i].randomize(0, 80, 0, 64, STAR_SPEED_MIN, STAR_SPEED_MAX);
            // Set its Y position back to the top.
            stars[i].y = 0;
        }

        // Draw the star at its new coordinate.
        mp.display.drawPixel(stars[i].x, stars[i].y, STAR_COLOR);
    }
	mp.display.setTextColor(TFT_WHITE);
	mp.display.cursor_x = 0;
	mp.display.cursor_y = 0;
	//buf2.print("  LAST         HIGH");
	//buf2.cursor_x = 14 - 2 * (score > 9) - 2 * (score > 99) - 2 * (score > 999);
	//buf2.cursor_y = 6;
	//buf2.print(score);
	//buf2.cursor_x = 66 - 2 * (highscore[0] > 9) - 2 * (highscore[0] > 99) - 2 * (highscore[0] > 999);
	//buf2.cursor_y = 6;
	//buf2.print(highscore[0]);
	//buf2.setTextColor(YELLOW);
	mp.display.drawBitmap(9, 15, gamelogo, TFT_RED);
	mp.display.cursor_x = 9;
	mp.display.cursor_y = 56;
	mp.display.setTextColor(TFT_RED);
	mp.display.print(" A:PLAY   MENU:QUIT");
	if (mp.buttons.released(BTN_A)) {
    Serial.println("NEW GAME");
		gamestatus = "newgame";
		//gb.sound.playOK();
	}
  if (mp.buttons.released(BTN_B))
      {
      while (!mp.update());
      while(1)
      {
         mp.bigIconsMainMenu();
         mp.lockScreen();// title
      }
    }
    
	//if (buttonsPressed(BUTTON_MENU)) {
	//	// Menu: Pause / Retour � l'�cran de titre
	//}
}


