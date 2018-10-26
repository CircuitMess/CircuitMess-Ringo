//----------------------------------------------------------------------    
//                             I N V A D E R S
//                    coded by Yoda Zhang on Gamebuino
//                       ported on META by Jicehel
//                                 V 1.0w
//----------------------------------------------------------------------    

//----------------------------------------------------------------------------    
// define variables and constants
//----------------------------------------------------------------------------    
#include <MAKERphone.h>
#include "Star.h"
//#include "portedHighscore.ino"
//#include "portedImages.ino"
//#include "portedNonstandard.ino"
//#include "portedSpecific.ino"
MAKERphone mp;
# define Maxwhidth 74
uint32_t pixelsTimer=0;
bool pixelsState=0;
String gamestatus;
int score;
int lives;
int gamelevel;
int shipx;
int invaderx[55];
int invadery[55];
int invaders[55];
int invaderframe[55];
int invaderanz;
int invaderctr;
int invadersound;
int checkdir;
int nextxdir;
int nextydir;
int invaderxr;
int invaderyr;
int invadershotx[4];
int invadershoty[4];
int invadershotframe;
int invadershots;
int invadershottimer;
int bunkers[4];
int shotx;
int shoty;
int checkl;
int checkr;
int checkt;
int checkb;
int yeahtimer;
int infoshow;
int deadcounter;
int saucerx;
int saucerdir;
int saucers;
int saucertimer;
int saucerwait;
int delayBip;

//----------------------------------------------------------------------------    
// define images & sounds
//----------------------------------------------------------------------------    
extern const byte gamelogo[];
extern const byte invader[8][7];
extern const byte playership[3][6];
extern const byte bunker[5][7];
extern const byte bomb[2][6];
extern const byte saucer[2][10];
extern const int soundfx[8][8];

/*u
//----------------------------------------------------------------------------
// setup
//----------------------------------------------------------------------------
*/
void setup() {
  Serial.begin(115200);
	 mp.begin();
	 mp.display.fillScreen(TFT_BLACK);
	 mp.display.setFreeFont(TT1);
	 //mp.update();
	 starsSetup();
	 gamestatus = "title";
}

//----------------------------------------------------------------------------    
// loop
//----------------------------------------------------------------------------    
void loop() {

	if (mp.update()) {

		if (gamestatus == "newgame") { newgame(); } // new game

		if (gamestatus == "newlevel") { newlevel(); } // new level

		if (gamestatus == "running") { // game running loop
			mp.display.fillScreen(TFT_BLACK);
      mp.update();
      if (mp.buttons.released(BTN_B))
      {
        gamestatus = "title";
        while(!mp.update());
      }
			showscore();
			checkbuttons(); // check buttons and move playership
			drawplayership(); // draw player ship
			drawplayershot(); // move + draw player shoot
			invaderlogic(); // invader logic
			drawinvaders(); // draw invaders
			invadershot(); // invaders shoot
			nextlevelcheck(); // next level?
			drawbunkers(); // draw bunkers & check collission with player shot
			saucerappears(); // saucer appears?
			movesaucer(); // move saucer
			drawsaucer(); // draw saucer & remove if hit
			showscore(); // show lives, score, level
       // end of: gamestatus = running
		}
   
		if (gamestatus == "title") { 
		  showtitle();
		}

		if (gamestatus == "gameover") { // game over
			mp.display.fillScreen(TFT_BLACK);
      for (int i = 0; i < 40; i++) {
        if (invaders[i] != -1) {
          mp.display.drawBitmap(invaderx[i], invadery[i], invader[invaders[i] + invaderframe[i]], TFT_WHITE);
        }
      }
		  mp.display.fillRect(6, 25, 68, 14, TFT_WHITE);
		  mp.display.setCursor(7, 37);
		  mp.display.setTextSize(2);
		  mp.display.setTextColor(TFT_BLACK);
		  mp.display.print("GAME OVER");
		  mp.display.setTextSize(1);
      if(mp.buttons.released(BTN_B) || mp.buttons.released(BTN_A))
      {
        while(!mp.update());
        gamestatus="title";
      }
        
 
		
		  
			
			//saveHighscore(score);
		}
	} // end of update
}
