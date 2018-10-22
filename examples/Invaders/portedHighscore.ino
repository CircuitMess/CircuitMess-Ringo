//#include <Gamebuino-Meta.h>
//#include "declare.h"
//
//#define NUM_HIGHSCORE 5
//#define NAMELENGTH 12
//
//int highscore[NUM_HIGHSCORE];
//char name[NUM_HIGHSCORE][NAMELENGTH + 1];
//
//const uint16_t highscore_sound[] PROGMEM = {
//  0x0005,0x140,0x150,0x15C,0x170,0x180,0x16C,0x154,0x160,0x174,0x184,0x14C,0x15C,0x168,0x17C,0x18C,0x0000 };
//
//void initHighscore() {
//	for (byte thisScore = 0; thisScore < NUM_HIGHSCORE; thisScore++) {
//		highscore[thisScore] = gb.save.get(2 * thisScore);
//		gb.save.get(2 * thisScore + 1, name[thisScore], NAMELENGTH + 1);
//	}
//}
//
//void saveHighscore(unsigned int test_score) {
//	if (test_score > highscore[NUM_HIGHSCORE - 1]) {//if it's a highscore
//		if (drawNewHighscore(test_score)) {
//			gb.getDefaultName(name[NUM_HIGHSCORE - 1]);
//			gb.gui.keyboard("Name", name[NUM_HIGHSCORE - 1]);
//			highscore[NUM_HIGHSCORE - 1] = test_score;
//			for (byte i = NUM_HIGHSCORE - 1; i > 0; i--) { //bubble sorting FTW
//				if (highscore[i - 1] < highscore[i]) {
//					char tempName[NAMELENGTH];
//					strcpy(tempName, name[i - 1]);
//					strcpy(name[i - 1], name[i]);
//					strcpy(name[i], tempName);
//					unsigned int tempScore;
//					tempScore = highscore[i - 1];
//					highscore[i - 1] = highscore[i];
//					highscore[i] = tempScore;
//				}
//				else {
//					break;
//				}
//			}
//			for (byte thisScore = 0; thisScore < NUM_HIGHSCORE; thisScore++) {
//				gb.save.set(2 * thisScore, highscore[thisScore]);
//				gb.save.set(2 * thisScore + 1, name[thisScore]);
//			}
//		}
//	}
//	drawHighScores();
//}
//
//void drawHighScores() {
//	while (gamestatus != "title") {
//		if (gb.update()) {
//			gb.display.clear();
//			// gb.display.setFont(font5x7);
//
//			if (gamestatus == "gameover") {
//				gb.display.setColor(RED);
//				gb.display.cursor_x = 20;
//				gb.display.cursorY = 3;
//				gb.display.print("GAME OVER");
//			}
//			gb.display.setColor(BLUE);
//			gb.display.cursor_x = 15 + random(0, 2);
//			gb.display.cursorY = 20 + random(0, 2);
//			gb.display.println("BEST SCORES");
//			gb.display.textWrap = false;
//			gb.display.cursor_x = 0;
//			gb.display.cursorY = gb.display.fontHeight * 2 + 20;
//			for (byte thisScore = 0; thisScore < NUM_HIGHSCORE; thisScore++) {
//				// gb.display.setColor(int(255 - (12*thisScore)), int(20*thisScore), int(255 - (15 *thisScore)) );
//				gb.display.setColor(YELLOW);
//				if (highscore[thisScore] == 0)
//					gb.display.print('-');
//				else {
//					gb.display.print(name[thisScore]);
//					gb.display.cursor_x = gb.display.width() - 4 * gb.display.fontWidth;
//					gb.display.cursorY = gb.display.fontHeight * 2 + gb.display.fontHeight*thisScore + 20;
//					gb.display.println(highscore[thisScore]);
//				}
//				if (buttonsPressed(BTN_A) || buttonsPressed(BTN_B) || buttonsPressed(BUTTON_MENU)) {
//					//gb.sound.playOK();
//					gamestatus = "title";
//					break;
//				}
//			}
//		}
//	}
//}
//
//boolean drawNewHighscore(unsigned int newscore) {
//	//gb.sound.playPattern(highscore_sound, 0);
//	//gb.sound.playOK();
//	while (1) {
//		if (gb.update()) {
//			gb.display.clear();
//			if (gamestatus == "gameover") {
//				gb.display.setColor(RED);
//				gb.display.cursor_x = 20;
//				gb.display.cursorY = 3;
//				gb.display.print("GAME OVER");
//			}
//			gb.display.setColor(BLUE);
//			gb.display.cursor_x = 10 + random(0, 2);
//			gb.display.cursorY = 15 + random(0, 2);
//			gb.display.print("NEW HIGHSCORE");
//			gb.display.setColor(YELLOW);
//			gb.display.cursor_x = 10;
//			gb.display.cursorY = 32;
//			gb.display.print("You      ");
//			gb.display.print(newscore);
//			gb.display.cursor_x = 10;
//			gb.display.cursorY = 38;
//			gb.display.print("Best     ");
//			gb.display.print(highscore[0]);
//			gb.display.cursor_x = 10;
//			gb.display.cursorY = 44;
//			gb.display.print("Worst    ");
//			gb.display.print(highscore[NUM_HIGHSCORE - 1]);
//			gb.display.setColor(BLUE);
//			gb.display.cursor_x = 10;
//			gb.display.cursorY = 56;
//			gb.display.print("\25:Save \26:Exit");
//			if (buttonsPressed(BTN_A)) {
//				//gb.sound.playOK();
//				gamestatus = "title";
//				return true;
//			}
//			if (buttonsPressed(BTN_B)) {
//				//gb.sound.playCancel();
//				gamestatus = "title";
//				return false;
//			}
//		}
//	}
//}
//
//
