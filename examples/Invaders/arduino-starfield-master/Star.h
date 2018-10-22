#ifndef _STAR_H_
#define _STAR_H_

#include "Arduino.h"

/**
 * Class that represents a single star on the screen.
 */
class Star
{
	public:
    void randomize(int xMin, int xMax, int yMin, int yMax, int speedMin, int speedMax);
    void update();
    int x, y, speed;
};

#endif

