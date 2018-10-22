#include "Star.h"

/**
 * Randomizes the position and speed of the star.
 */
void Star::randomize(int xMin, int xMax, int yMin, int yMax, int speedMin, int speedMax)
{
    // Generate random values for the star.
    x = random(xMin, xMax);
    y = random(yMin, yMax);
    speed = random(speedMin, speedMax+1); // speedMax is +1 to make it inclusive.
}

/**
 * Updates the position of the star.
 */
void Star::update()
{
    // Update the star's Y position based on its speed.
    y += speed;
}

