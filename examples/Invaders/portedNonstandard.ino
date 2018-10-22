//----------------------------------------------------------------------------
void newgame() {
	score = 0;
	lives = 3;
	gamelevel = 0;
	shipx = 40;
	shotx = -1;
	shoty = -1;
	deadcounter = -1;
	saucers = -1;
	for (int i = 0; i < 4; i++) {
		invadershotx[i] = -1;
		invadershoty[i] = -1;
	}
	gamestatus = "newlevel";
}
//----------------------------------------------------------------------------
void newlevel() {
	invaderanz = 40;
	invaderctr = 39;
	invaderxr = 2;
	invaderyr = 0;
	checkdir = 0;
	nextxdir = 2;
	nextydir = 0;
	yeahtimer = 0;
	delayBip = 0;
	invadershottimer = 60;
	saucertimer = 240;
	int down = gamelevel * 2;
	if (gamelevel > 8) { down = 16; }
	for (int i = 0; i < 8; i++) {
		invaderx[i] = 10 + i * 8;
		invaderx[i + 8] = 10 + i * 8;
		invaderx[i + 16] = 10 + i * 8;
		invaderx[i + 24] = 10 + i * 8;
		invaderx[i + 32] = 10 + i * 8;
		invadery[i] = 0 + down;
		invadery[i + 8] = 6 + down;
		invadery[i + 16] = 12 + down;
		invadery[i + 24] = 18 + down;
		invadery[i + 32] = 24 + down;
		invaders[i] = 4;
		invaders[i + 8] = 2;
		invaders[i + 16] = 2;
		invaders[i + 24] = 0;
		invaders[i + 32] = 0;
		invaderframe[i] = 0;
		invaderframe[i + 8] = 0;
		invaderframe[i + 16] = 0;
		invaderframe[i + 24] = 0;
		invaderframe[i + 32] = 0;
	}
	for (int i = 0; i < 4; i++) {
		bunkers[i] = 0;
		if (gamelevel > 5) { bunkers[i] = -1; }
	}
	gamestatus = "running";
}
//----------------------------------------------------------------------------
void showscore() {
	if (infoshow == 1 and saucers == -1) {
		if (lives > 1) { mp.display.drawBitmap(0, 0, playership[0], TFT_WHITE); }
		if (lives > 2) { mp.display.drawBitmap(9, 0, playership[0], TFT_WHITE); }
		mp.display.cursor_x= 42 - 2 * (score > 9) - 2 * (score > 99) - 2 * (score > 999);
		mp.display.cursor_y = 5;
		mp.display.print(score);
		mp.display.cursor_x = 72;
		mp.display.cursor_y = 5;
		mp.display.print(gamelevel + 1);
	}
}
//----------------------------------------------------------------------------
void nextlevelcheck() {
	// increment timer after all invaders killed
	if (invaderanz == 0) {
		yeahtimer = ++yeahtimer;
		if (yeahtimer >= 90) {
			gamelevel = ++gamelevel;
			gamestatus = "newlevel";
		}
	}
}
//----------------------------------------------------------------------------
void handledeath() {
	deadcounter = --deadcounter;
	if (deadcounter == 0) {
		deadcounter = -1;
		lives = --lives;
		shipx = 0;
		if (lives == 0) { gamestatus = "gameover"; }
	}
}

