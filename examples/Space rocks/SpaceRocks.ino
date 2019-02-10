#include <stdio.h>
#include <MAKERphone.h>
#include "Sprites.h"
MAKERphone mp;
/*
    Space Rocks
    Copyright (C) 2019 CircuitMess

    original Arduboy game:
    ASTEROIDS
    Copyright (C) 2018 Xavier
    https://github.com/CDRXavier/ASTEROID/tree/master

    Colorized and ported to MAKERphone by CircuitMess

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
float shipX;
float shipY;
float velocityX;
float velocityY;
int8_t HDG;
//bool released;
signed short bullet[8][5];
float asteroids[8][4];
uint8_t asteroidDraw[8];
float rocks[16][4];
uint8_t rockDraw[16];
float pebbles[32][4];
uint8_t pebbleDraw[32];
byte bulletCount;
byte asteroidCount;
byte rockCount;
byte pebbleCount;
int8_t life;
uint8_t level = 0;
unsigned short score;
char tick;
bool turnC;
enum class ProgState : uint8_t {Main, Simulation,  DataEntry, DataDisplay, Pause, DataErasure};
char initials[3];
uint16_t battery;
static const PROGMEM float heading[24][2] = {
  { +0.00, -0.20}, { +0.05, -0.19},
  { +0.10, -0.17}, { +0.14, -0.14},
  { +0.17, -0.10}, { +0.19, -0.05},
  { +0.20, +0.00}, { +0.19, +0.05},
  { +0.17, +0.10}, { +0.14, +0.14},
  { +0.10, +0.17}, { +0.05, +0.19},
  { +0.00, +0.20}, { -0.05, +0.19},
  { -0.10, +0.17}, { -0.14, +0.14},
  { -0.17, +0.10}, { -0.19, +0.05},
  { -0.20, +0.00}, { -0.19, -0.05},
  { -0.17, -0.10}, { -0.14, -0.14},
  { -0.10, -0.17}, { -0.05, -0.19}
};

static const int8_t shipTbl[6][24] PROGMEM = {
  //HDG
  //  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
  {  +3, +3, +2, +1, -1, -2, -3, -2, -1, +1, +2, +3, +3, -3, -2, -1, +1, +2, +3, +2, +1, -1, -2, -3,},
  {  +3, +3, +3, +3, +3, +3, +3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, +3, +3, +3, +3, +3, +3,},
  {  +0, +2, +3, +3, +3, +4, +4, +4, +3, +3, +3, +2, +0, -2, -3, -3, -3, -4, -4, -4, -3, -3, -3, -2,},
  {  -4, -4, -3, -3, -3, -2, +0, +2, +3, +3, +3, +4, +4, +4, +3, +3, +3, +2, +0, -2, -3, -3, -3, -4,},
  {  -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, +3, +3, +3, +3, +3, +3, +3, +3, +3, +3, +3,},
  {  +3, +2, +1, -1, -2, -3, -3, +3, +2, +1, -1, -2, -3, -2, -1, +1, +2, +3, -3, -3, -2, -1, +1, +2,},
};
#define shipX0  shipX + (int8_t)(pgm_read_byte(&shipTbl[0][HDG]))
#define shipY0  shipY + (int8_t)(pgm_read_byte(&shipTbl[1][HDG]))
#define shipX1  shipX + (int8_t)(pgm_read_byte(&shipTbl[2][HDG]))
#define shipY1  shipY + (int8_t)(pgm_read_byte(&shipTbl[3][HDG]))
#define shipX2  shipX + (int8_t)(pgm_read_byte(&shipTbl[4][HDG]))
#define shipY2  shipY + (int8_t)(pgm_read_byte(&shipTbl[5][HDG]))
#define ADC_TEMP (_BV(REFS0) | _BV(REFS1) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0))
#define ADC_VOLTAGE (_BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1))
ProgState simState = ProgState::Main;
void collision() {
	//   sound.tone(150, 50);
	life = life - 1;
	shipX = mp.display.width()/2;
	shipY = mp.display.height()/2;
	velocityX = 0;
	velocityY = 0;
}
void guidance() {

  //stopping gradually
  if (velocityY > 0)velocityY -= 0.01;
  if (velocityY < 0)velocityY += 0.01;

  if (velocityX > 0)velocityX -= 0.01;
  if (velocityX < 0)velocityX += 0.01;

  //stop completely if going slow enough
  if (velocityX <= 0.01 && velocityX >= -0.01) velocityX = 0;
  if (velocityY <= 0.01 && velocityY >= -0.01) velocityY = 0;

  //max speed reached?
  if (velocityY > 2)velocityY = 2;
  if (velocityY < -2)velocityY = -2;

  if (velocityX > 2)velocityX = 2;
  if (velocityX < -2)velocityX = -2;
}
void ship() {
  // if (abs(velocityX) > 0.02)
    shipX = shipX + velocityX;
  // if (abs(velocityY) > 0.02)
    shipY = shipY + velocityY;
  if (shipX < 0) shipX = shipX + mp.display.width()+10;
  if (shipX > mp.display.width()) shipX = shipX - mp.display.width()-10;
  if (shipY < 0) shipY = shipY +  mp.display.height()+10;
  if (shipY > mp.display.height()) shipY = shipY - mp.display.height()-10;
  mp.display.fillTriangle(shipX0*2, shipY0*2, shipX1*2, shipY1*2, shipX2*2, shipY2*2, TFT_LIGHTGREY);
  mp.display.drawTriangle(shipX0*2, shipY0*2, shipX1*2, shipY1*2, shipX2*2, shipY2*2, TFT_LIGHTGREY);
  // mp.display.drawTriangle(shipX0*2, shipY0*2, shipX1*2, shipY1*2, shipX2*2, shipY2*2, TFT_DARKGREY);
  mp.display.fillTriangle(shipX0, shipY0, shipX1, shipY1, shipX2, shipY2, TFT_NAVY);
}
void navigation() {
    if (mp.buttons.repeat(BTN_LEFT, 1)) HDG--;
    if (mp.buttons.repeat(BTN_RIGHT, 1)) HDG++;
    if (HDG > 23) HDG = HDG - 24;
    if (HDG < 0) HDG = HDG + 24;
    if (mp.buttons.repeat(BTN_UP, 1)) {
		
		velocityX += heading[HDG][0];
		velocityY += heading[HDG][1];

    }
    if (mp.buttons.repeat(BTN_DOWN, 1)) {
		if(velocityX!=0)
    {
		  velocityX -= heading[HDG][0];
      if((heading[HDG][0] > 0 && velocityX < 0) || (heading[HDG][0] < 0 && velocityX > 0))
        velocityX = 0;
    }
    if(velocityY!=0)
    {
	  	velocityY -= heading[HDG][1];
      if((heading[HDG][1] > 0 && velocityY < 0) || (heading[HDG][1] < 0 && velocityY > 0))
        velocityY = 0;
    }

    }
  // if (mp.buttons.pressed(BTN_DOWN)) {
  //   delay(300);
  //   if (random(0, 3) == 2) {
  //     delay(200);
  //     collision();
  //   } else {
  //     velocityX = velocityY = 0;
  //     shipX = random(0, 128);
  //     shipY = random(0, 64);
  //   }
  // }
}

void firing() {
  bullet[bulletCount][1] = (short)(round)(pgm_read_float(&heading[HDG][0]) * 20);
  bullet[bulletCount][3] = (short)(round)(pgm_read_float(&heading[HDG][1]) * 20);
  bullet[bulletCount][0] = shipX1;
  bullet[bulletCount][2] = shipY1;
  bullet[bulletCount][4] = 120;
  bulletCount = bulletCount + 1;
//   sound.tone(1100, 4);
//   delay(4);
//   sound.tone(1000, 4);
//   delay(4);
//   sound.tone(900, 4);
//   delay(4);
//   sound.tone(800, 4);
//   delay(4);
//   sound.tone(700, 4);
//   delay(4);
//   sound.tone(600, 4);
//   delay(4);
//   sound.tone(500, 4);
//   delay(4);
//   sound.tone(400, 4);
}
void trajectory() {
  for (uint8_t t = 0; t < bulletCount; t++) {
    bullet[t][0] = bullet[t][0] + bullet[t][1];
    bullet[t][2] = bullet[t][2] + bullet[t][3];
    if (bullet[t][0] < -5) bullet[t][0] = bullet[t][0] + mp.display.width()+10;
    if (bullet[t][0] > mp.display.width()+5) bullet[t][0] = bullet[t][0] - mp.display.width()-10;
    if (bullet[t][2] < -5) bullet[t][2] = bullet[t][2] + mp.display.height()+10;
    if (bullet[t][2] > mp.display.height()+5) bullet[t][2] = bullet[t][2] - mp.display.height()-10;
    if (bullet[t][4] < 0) {
      bullet[t][4] = 0;
      for (uint8_t a = 0; a < bulletCount - 1; a++)
        for (uint8_t b = 0; b < 5; b++)
          bullet[a][b] = bullet[a + 1][b];
      bullet[bulletCount][0] = 0;
      bullet[bulletCount][1] = 0;
      bullet[bulletCount][2] = 0;
      bullet[bulletCount][3] = 0;
      bullet[bulletCount][4] = 0;
      bulletCount = bulletCount - 1;
    }
    mp.display.fillRect(bullet[t][0], bullet[t][2], 2, 2, TFT_WHITE);
    bullet[t][4] = bullet[t][4] - 4;
  }
}
void asteroid() {
  for (uint8_t t = 0; t < asteroidCount; t++) {
    asteroids[t][0] = asteroids[t][0] + asteroids[t][1];
    asteroids[t][2] = asteroids[t][2] + asteroids[t][3];
    if (asteroids[t][0] < -5) asteroids[t][0] = asteroids[t][0] + mp.display.width() + 10;
    if (asteroids[t][0] > mp.display.width() + 5) asteroids[t][0] = asteroids[t][0] - mp.display.width() - 10;
    if (asteroids[t][2] < -5) asteroids[t][2] = asteroids[t][2] + mp.display.height() + 10;
    if (asteroids[t][2] > mp.display.height() + 5) asteroids[t][2] = asteroids[t][2] - mp.display.height() - 10;
    for (uint8_t b = 0; b < bulletCount; b++) {
      if ((abs(bullet[b][0] - asteroids[t][0]) <= 9 && abs(bullet[b][2] - asteroids[t][2]) <= 9) || (abs(bullet[b][0] - asteroids[t][0]) + abs(bullet[b][2] - asteroids[t][2]) < 13))
      {
        for (uint8_t a = b; a < bulletCount - 1; a++) {
          bullet[a][0] = bullet[a + 1][0];
          bullet[a][1] = bullet[a + 1][1];
          bullet[a][2] = bullet[a + 1][2];
          bullet[a][3] = bullet[a + 1][3];
          bullet[a][4] = bullet[a + 1][4];
        }
        bullet[bulletCount][0] = 0;
        bullet[bulletCount][1] = 0;
        bullet[bulletCount][2] = 0;
        bullet[bulletCount][3] = 0;
        bullet[bulletCount][4] = 0;
        bulletCount = bulletCount - 1;

        // sound.tone(150, 75);
        score = score + 2;

        rocks[rockCount][0] = asteroids[t][0];
        rocks[rockCount][2] = asteroids[t][2];
        rocks[rockCount][1] = random(-10, 10) * 0.05;
        rocks[rockCount][3] = random(-10, 10) * 0.05;
        rockDraw[rockCount] = random(0, 3);
        rockCount = rockCount + 1;
        rocks[rockCount][0] = asteroids[t][0];
        rocks[rockCount][2] = asteroids[t][2];
        rocks[rockCount][1] = random(-10, 10) * 0.05;
        rocks[rockCount][3] = random(-10, 10) * 0.05;
        rockDraw[rockCount] = random(0, 3);
        rockCount = rockCount + 1;

        for (uint8_t c = t; c < asteroidCount - 1; c++) {
          asteroidDraw[c] = asteroidDraw[c + 1];
          for (uint8_t d = 0; d < 4; d++)
            asteroids[c][d] = asteroids[c + 1][d];
        }
        asteroidCount = asteroidCount - 1;
      }
    }
    switch (asteroidDraw[t]) {
      case 0: mp.display.drawIcon(asteroid1, asteroids[t][0] - 8, asteroids[t][2] - 8, 24,24, 1, TFT_WHITE); break;
      case 1: mp.display.drawIcon(asteroid2, asteroids[t][0] - 8, asteroids[t][2] - 8, 24,24, 1, TFT_WHITE); break;
      case 2: mp.display.drawIcon(asteroid3, asteroids[t][0] - 8, asteroids[t][2] - 8, 24,24, 1, TFT_WHITE); break;
      default: mp.display.drawCircle(asteroids[t][0], asteroids[t][2], 5, TFT_WHITE);
    }
  }
}
void rock() {
  for (uint8_t r = 0; r < rockCount; r++) {
    rocks[r][0] = rocks[r][0] + rocks[r][1];
    rocks[r][2] = rocks[r][2] + rocks[r][3];
    if (rocks[r][0] < -5) rocks[r][0] = rocks[r][0] + mp.display.width()+10;
    if (rocks[r][0] > mp.display.width()+5) rocks[r][0] = rocks[r][0] - mp.display.width()-10;
    if (rocks[r][2] < -5) rocks[r][2] = rocks[r][2] + mp.display.height()+10;
    if (rocks[r][2] > mp.display.height()+5) rocks[r][2] = rocks[r][2] - mp.display.height()-10;
    for (uint8_t b = 0; b < bulletCount; b++) {
      if ((abs(bullet[b][0] - rocks[r][0]) <= 7 && abs(bullet[b][2] - rocks[r][2]) <= 7) ||
          (abs(bullet[b][0] - rocks[r][0]) + abs(bullet[b][2] - rocks[r][2]) < 11)) {
        for (uint8_t a = b; a < bulletCount - 1; a++) {
          bullet[a][0] = bullet[a + 1][0];
          bullet[a][1] = bullet[a + 1][1];
          bullet[a][2] = bullet[a + 1][2];
          bullet[a][3] = bullet[a + 1][3];
          bullet[a][4] = bullet[a + 1][4];
        }
        bullet[bulletCount][0] = 0;
        bullet[bulletCount][1] = 0;
        bullet[bulletCount][2] = 0;
        bullet[bulletCount][3] = 0;
        bullet[bulletCount][4] = 0;
        bulletCount = bulletCount - 1;

        // sound.tone(150, 75);
        score = score + 4;

        pebbles[pebbleCount][0] = rocks[r][0];
        pebbles[pebbleCount][2] = rocks[r][2];
        pebbles[pebbleCount][1] = random(-10, 10) * 0.05;
        pebbles[pebbleCount][3] = random(-10, 10) * 0.05;
        pebbleDraw[pebbleCount] = random(0, 3);
        pebbleCount = pebbleCount + 1;
        pebbles[pebbleCount][0] = rocks[r][0];
        pebbles[pebbleCount][2] = rocks[r][2];
        pebbles[pebbleCount][1] = random(-10, 10) * 0.05;
        pebbles[pebbleCount][3] = random(-10, 10) * 0.05;
        pebbleDraw[pebbleCount] = random(0, 3);
        pebbleCount = pebbleCount + 1;

        for (uint8_t c = r; c < rockCount - 1; c++) {
          rockDraw[c] = rockDraw[c + 1];
          for (uint8_t d = 0; d < 4; d++)
            rocks[c][d] = rocks[c + 1][d];
        }
        rockCount = rockCount - 1;
      }
    }
    switch (rockDraw[r]) {
      case 0: mp.display.drawIcon(rock1, rocks[r][0] - 4, rocks[r][2] - 4, 16, 16, 1, TFT_WHITE); break;
      case 1: mp.display.drawIcon(rock2, rocks[r][0] - 4, rocks[r][2] - 4, 16, 16, 1, TFT_WHITE); break;
      case 2: mp.display.drawIcon(rock3, rocks[r][0] - 4, rocks[r][2] - 4, 16, 16, 1, TFT_WHITE); break;
      default: mp.display.drawCircle(rocks[r][0], rocks[r][2], 4, TFT_WHITE);
    }
  }
}
void pebble() {
  for (uint8_t p = 0; p < pebbleCount; p++) {
    pebbles[p][0] = pebbles[p][0] + pebbles[p][1];
    pebbles[p][2] = pebbles[p][2] + pebbles[p][3];
    if (pebbles[p][0] < -5) pebbles[p][0] = pebbles[p][0] + mp.display.width()+10;
    if (pebbles[p][0] > mp.display.width()+5) pebbles[p][0] = pebbles[p][0] - mp.display.width()-10;
    if (pebbles[p][2] < -5) pebbles[p][2] = pebbles[p][2] + mp.display.height()+10;
    if (pebbles[p][2] > mp.display.height()+5) pebbles[p][2] = pebbles[p][2] - mp.display.height()-10;
    for (uint8_t b = 0; b < bulletCount; b++) {
      if (abs(bullet[b][0] - pebbles[p][0]) <= 3 && abs(bullet[b][2] - pebbles[p][2]) <= 3) {
        for (uint8_t a = b; a < bulletCount - 1; a++) {
          bullet[a][0] = bullet[a + 1][0];
          bullet[a][1] = bullet[a + 1][1];
          bullet[a][2] = bullet[a + 1][2];
          bullet[a][3] = bullet[a + 1][3];
          bullet[a][4] = bullet[a + 1][4];
        }
        bulletCount = bulletCount - 1;

        // sound.tone(150, 75);
        score = score + 8;

        for (uint8_t c = p; c < pebbleCount - 1; c++) {
          pebbleDraw[c] = pebbleDraw[c + 1];
          for (uint8_t d = 0; d < 4; d++)
            pebbles[c][d] = pebbles[c + 1][d];
        }
        pebbleCount = pebbleCount - 1;
      }
    }
    switch (pebbleDraw[p]) {
      case 0: mp.display.drawIcon(pebble1, pebbles[p][0] - 2, pebbles[p][2] - 2, 10, 10, 1, TFT_WHITE); break;
      case 1: mp.display.drawIcon(pebble2, pebbles[p][0] - 2, pebbles[p][2] - 2, 10, 10, 1, TFT_WHITE); break;
      case 2: mp.display.drawIcon(pebble3, pebbles[p][0] - 2, pebbles[p][2] - 2, 10, 10, 1, TFT_WHITE); break;
      default: mp.display.drawCircle(pebbles[p][0], pebbles[p][2], 2, TFT_WHITE);
    }
  }
}
void radar() {
  for (uint8_t a = 0; a < asteroidCount; a ++) {
    if (abs(asteroids[a][0] - shipX) < 17 && abs(asteroids[a][2] - shipY) < 17) {
    //   sound.tone(150, 50);
      collision();
      rocks[rockCount][0] = asteroids[a][0] + asteroids[a][1];
      rocks[rockCount][2] = asteroids[a][2] + asteroids[a][3];
      rocks[rockCount][1] = - asteroids[a][1];
      rocks[rockCount][3] = - asteroids[a][3];
      rockDraw[rockCount] = random(0, 3);
      rockCount = rockCount + 1;
      for (uint8_t c = a; c < asteroidCount - 1; c++)
        for (uint8_t d = 0; d < 4; d++)
          asteroids[c][d] = asteroids[c + 1][d];
      asteroidCount = asteroidCount - 1;
    }
  }
  for (uint8_t r = 0; r < rockCount; r ++) {
    if (abs(rocks[r][0] - shipX) < 13 && abs(rocks[r][2] - shipY) < 13) {
    //   sound.tone(150, 50);
      collision();
      pebbles[pebbleCount][0] = rocks[r][0] + rocks[r][1];
      pebbles[pebbleCount][2] = rocks[r][2] + rocks[r][3];
      pebbles[pebbleCount][1] = - rocks[r][1];
      pebbles[pebbleCount][3] = - rocks[r][3];
      pebbleDraw[pebbleCount] = random(0, 3);
      pebbleCount = pebbleCount + 1;
      for (uint8_t c = r; c < rockCount - 1; c++)
        for (uint8_t d = 0; d < 4; d++)
          rocks[c][d] = rocks[c + 1][d];
      rockCount = rockCount - 1;
    }
  }
  for (uint8_t p = 0; p < pebbleCount; p ++) {
    if (abs(pebbles[p][0] - shipX) < 9 && abs(pebbles[p][2] - shipY) < 9) {
      // delay(200);
      collision();
      for (uint8_t c = p; c < pebbleCount - 1; c++)
        for (uint8_t d = 0; d < 4; d++)
          pebbles[c][d] = pebbles[c + 1][d];
      pebbleCount = pebbleCount - 1;
    }
  }
  if (asteroidCount == 0 && rockCount == 0 && pebbleCount == 0) {
    tick = tick - 1;
    if (tick <= 0) {
      tick = 80;
      if (level < 8) level = level + 1;

      for (int8_t s = 0; s < level; s++) {
        asteroids[asteroidCount][0] = 112 + random(0, 106);
        asteroids[asteroidCount][2] = 80 + random(0, 96);
        asteroids[asteroidCount][1] = random(-10, 10) * 0.05;
        asteroids[asteroidCount][3] = random(-10, 10) * 0.05;
        asteroidDraw[asteroidCount] = random(0, 3);
        asteroidCount = asteroidCount + 1;
      }
    }
  }
}

void resetField() {
  level = 0;
  velocityX = velocityY = 0;
  tick = 80;
  bulletCount = 0;
  asteroidCount = rockCount = pebbleCount = 0;
}
void resetSim() {
  HDG = 0;
  shipY = mp.display.height()/2;
  shipX = mp.display.width()/2;
  resetField();
  simState = ProgState::Main;
  score = 0;
}


void enterInitials() {
  int8_t index = 0;
  initials[0] = ' ';
  initials[1] = ' ';
  initials[2] = ' ';
  while (index < 3) {
	mp.display.fillScreen(TFT_BLACK);
	mp.display.setCursor(16, 0);
	mp.display.print(F("HIGH SCORE"));
	mp.display.setCursor(88, 0);
	mp.display.print(score);

	mp.display.setCursor(56, 20);
	mp.display.print(initials[0]);
	mp.display.setCursor(64, 20);
	mp.display.print(initials[1]);
	mp.display.setCursor(72, 20);
	mp.display.print(initials[2]);
	mp.display.drawLine(56, 27, 56 + 6, 27, TFT_WHITE);
	mp.display.drawLine(64, 27, 64 + 6, 27, TFT_WHITE);
	mp.display.drawLine(72, 27, 72 + 6, 27, TFT_WHITE);
	mp.display.drawLine(56 + (index * 8), 28, 56 + (index * 8) + 6, 28, TFT_WHITE);
	// delay(150);
	if (mp.buttons.pressed(BTN_LEFT)) {
	// sound.tone(1046, 200);
	index--;
	if (index < 0) index = 0;
	}

	if (mp.buttons.pressed(BTN_RIGHT)) {
	index++;
	if (index > 2) index = 2;
	//sound.tone(1046, 200);
	}

	if (mp.buttons.pressed(BTN_DOWN)) {
	initials[index]++;
	// sound.tone(523, 150);
	// A-Z 0-9 :-? !-/ ' '
	if (initials[index] == '0') initials[index] = ' ';
	if (initials[index] == '!') initials[index] = 'A';
	if (initials[index] == '[') initials[index] = '0';
	if (initials[index] == '@') initials[index] = '!';
	}
	if (mp.buttons.pressed(BTN_UP)) {
	initials[index]--;
	// sound.tone(523, 150);
	if (initials[index] == ' ') initials[index] = '?';
	if (initials[index] == '/') initials[index] = 'Z';
	if (initials[index] == 31)  initials[index] = '/';
	if (initials[index] == '@') initials[index] = ' ';
	}
	if (mp.buttons.released(BTN_A)) {
	if (index >= 2) {
		index = index + 1;
		// sound.tone(1046, 200);
		}
	}
  }
}
uint16_t rawADC(uint8_t adc_bits) {
//   power_adc_enable(); // ADC on
//   ADMUX = adc_bits;
//   // we also need MUX5 for temperature check
//   if (adc_bits == ADC_TEMP) {
//     ADCSRB = _BV(MUX5);
//   }
//   delay(2); // Wait for ADMUX setting to settle
//   ADCSRA |= _BV(ADSC); // Start conversion
//   while (bit_is_set(ADCSRA, ADSC)); // measuring
//   return ADC;
//   power_adc_disable();
}
void setup() {
  // put your setup code here, to run once:
  mp.begin(0);
  Serial.begin(115200);
  randomSeed(millis() * millis());
  resetSim();
}

uint32_t randMillis = millis();
uint32_t shootMillis = millis();
uint32_t turnMillis = millis();
long unsigned int startMillis;

void loop() 
{
  if(mp.update())
	{
		// put your main code here, to run repeatedly:
		mp.display.fillScreen(TFT_BLACK);
		switch (simState)
		{
			case ProgState::Main: {
        mp.display.drawIcon(backdrop, 0, 0, 160, 128);
        //begin homescreen
				mp.display.setCursor(10, 10);
				mp.display.setTextSize(2);
        
        turnC = random(0, 2) == 1;
				if (millis() - turnMillis >= 300) {
					turnMillis = millis();
					if (turnC)
						HDG = HDG + 1;
					else
						HDG = HDG - 1;
				}
				if (HDG > 23) HDG = HDG - 24;
				if (HDG < 0) HDG = HDG + 24;
				velocityX = 1;
        shipY = 100;
				ship();
				if (millis()-shootMillis >= 150 && (bulletCount < 8)) {
					shootMillis = millis();
					bullet[bulletCount][1] = (short)(round)(pgm_read_float(&heading[HDG][0]) * 20);
					bullet[bulletCount][3] = (short)(round)(pgm_read_float(&heading[HDG][1]) * 20);
					bullet[bulletCount][0] = shipX1;
					bullet[bulletCount][2] = shipY1;
					bullet[bulletCount][4] = 100;
					bulletCount = bulletCount + 1;
				}
				trajectory();
        mp.display.drawIcon(title, 5, 5, 150, 75, 1, TFT_WHITE);
        mp.display.setCursor(118, 118);
        mp.display.setTextFont(1);
        mp.display.setTextSize(1);
        mp.display.setTextColor(TFT_WHITE);
        mp.display.printCenter("A: start     B: highscores");
        if (mp.buttons.released(BTN_A)) {
					while(!mp.update());
				life = 3;
        resetSim();
        resetField();
        
				simState = ProgState::Simulation;
				}
				if (mp.buttons.released(BTN_B))
				{
					while(!mp.update());
					simState = ProgState::DataDisplay;
				}
			}
			break;
			case ProgState::Simulation: {
        mp.display.drawIcon(backdrop, 0, 0, 160, 128);

        if (life > 0) {
          navigation();
          guidance();
          ship();
          if (mp.buttons.held(BTN_A, 1) && (bulletCount < 8))
            firing();
          trajectory();
          asteroid();
          rock();
          pebble();
          radar();

          mp.display.setTextSize(1);
          mp.display.setCursor(2, 10);
          mp.display.print(score);
          mp.display.setCursor(32, 10);
          mp.display.print(level);
          mp.display.setCursor(120, 10);
          mp.display.print(life);
          if (life == 0)
            resetField();
          if (mp.buttons.released(BTN_B)) 
            simState = ProgState::Pause;
				}
        else 
        {
          mp.display.drawIcon(backdrop, 0, 0, 160, 128);
          while(!mp.update());
          for (int i = 0; i < mp.display.height(); i+=4)
          {
            for (int x = 0; x < mp.display.width(); x++)
            {
              mp.display.drawPixel(x, i, TFT_DARKGREY);
              mp.display.drawPixel(x, i+1, TFT_DARKGREY);
              mp.update();
              delayMicroseconds(30);
            }
          }
          mp.display.drawBitmap(26, 15, gameover, TFT_DARKGREY);
          mp.display.drawBitmap(28, 13, gameover, TFT_BLACK);
          while(!mp.buttons.released(BTN_A) && !mp.buttons.released(BTN_B))
            mp.update();
          while(!mp.update());
          simState = ProgState::DataEntry;
				}
			}
			break;
			case ProgState::DataEntry: {
				char tmpInitials[3];
				unsigned short tmpScore;
				// Each block of EEPROM has 7 high scores, and each high score entry
				// is 5 byte long:  3 char for initials and a short for score.
				// High score processing
				for (int8_t i = 0; i < 7; i++) {
				EEPROM.get(100 + (5 * i), tmpScore);
				if (tmpScore == 0xFFFF) tmpScore = 0;
				if (score > tmpScore) {
					enterInitials();
					for (byte j = i; j < 7; j++) {
					EEPROM.get(100 + (5 * j), tmpScore);
					if (tmpScore == 0xFFFF) tmpScore = 0;

					tmpInitials[0] = (char)EEPROM.read(100 + (5 * j) + 2);
					tmpInitials[1] = (char)EEPROM.read(100 + (5 * j) + 3);
					tmpInitials[2] = (char)EEPROM.read(100 + (5 * j) + 4);

					// write score and initials to current slot
					EEPROM.put(100 + (5 * j), score);
					EEPROM.write(100 + (5 * j) + 2, initials[0]);
					EEPROM.write(100 + (5 * j) + 3, initials[1]);
					EEPROM.write(100 + (5 * j) + 4, initials[2]);

					// tmpScore and tmpInitials now hold what we want to
					//write in the next slot.
					score = tmpScore;
					initials[0] = tmpInitials[0];
					initials[1] = tmpInitials[1];
					initials[2] = tmpInitials[2];
					}
					//reset the initials
					initials[0] = ' ';
					initials[1] = ' ';
					initials[2] = ' ';
					break;
				}
				}
			}
			resetSim();
			break;
			case ProgState::DataDisplay:
			// Each block of EEPROM has 7 high scores, and each high score entry
			// is 5 int8_ts long:  3 int8_ts for initials and one int16_t for score
			mp.display.setCursor(32, 0);
			mp.display.setTextSize(1);
			mp.display.print("HIGH SCORES");

			for (int i = 0; i < 7; i++) {
				EEPROM.get(100 + (5 * i), score);
				if (score == 0xFFFF) score = 0;
				initials[0] = (char)EEPROM.read(100 + (5 * i) + 2);
				initials[1] = (char)EEPROM.read(100 + (5 * i) + 3);
				initials[2] = (char)EEPROM.read(100 + (5 * i) + 4);

				mp.display.setCursor(30, 9 + (i * 8));
				mp.display.print(i + 1);
				mp.display.print(F(" "));
				mp.display.print(initials[0]);
				mp.display.print(initials[1]);
				mp.display.print(initials[2]);
				mp.display.print(F(" "));
				mp.display.print(score);
				score = 0;
			}
			if (mp.buttons.released(BTN_B))
			{
				simState = ProgState::Main;
				while(!mp.update());
			}
			if (mp.buttons.released(BTN_LEFT)) ProgState::DataErasure;
			break;
			case ProgState::Pause:
			mp.display.setCursor(32, 30);
			mp.display.setTextSize(2);
			mp.display.printCenter(F("PAUSE"));
			if (mp.buttons.released(BTN_A)) simState = ProgState::Simulation;
			if (mp.buttons.released(BTN_B)) resetSim();
			if (mp.buttons.released(BTN_UP) || mp.buttons.released(BTN_DOWN)) {
			}
			break;
			case ProgState::DataErasure:
			mp.display.setCursor(14, 32);
			mp.display.setTextSize(1);
			mp.display.print(F("EREASE HIGHSCORE?"));
			mp.display.setCursor(14, 40);
			mp.display.print(F("START = RIGHT + B"));
			if (mp.buttons.pressed(BTN_RIGHT) || mp.buttons.pressed(BTN_B)) {
				for (int i = 0; i < 35; i ++) {
				mp.display.fillScreen(TFT_BLACK);
				mp.display.setCursor(36, mp.display.height() / 2);
				mp.display.print(F("WORKING..."));
				while(!mp.update());
				}
				delay(500);
				mp.display.setCursor(32, mp.display.height() / 2);
				mp.display.fillScreen(TFT_BLACK);
				mp.display.print(F("EREASE DONE"));
				while(!mp.update());
				
				delay(1000);
				simState = ProgState::Main;
			}
			if (mp.buttons.released(BTN_A)) simState = ProgState::Main;
			break;
		}
	}
}