#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include "Star.h"

// LCD Pins.
#define LCD_CS A3     // LCD CS pin.
#define LCD_CD A2     // LCD CD pin. Sometimes also labeled as the LCD RS pin.
#define LCD_WR A1     // LCD WR pin.
#define LCD_RD A0     // LCD RD pin.
#define LCD_RESET A4  // LCD RST pin.

// Screen constatns.
#define SCREEN_IDENTIFIER 0x9341  // Set this to your screen identifier. 0x9341 is the identifier for my generic ILI9341 compatible screen.
#define SCREEN_WIDTH 240          // Screen width in pixels.
#define SCREEN_HEIGHT 320         // Screen height in pixels.

// Program constants.
#define STAR_COUNT 100            // Number of stars on the screen. Arduino UNO maxes out around 250.
#define BACKGROUND_COLOR 0x0000   // Background color in hex. 0x0000 is black.
#define STAR_SPEED_MIN 3          // Minimum movement in pixels per update. (value is inclusive)
#define STAR_SPEED_MAX 20         // Maximum movement in pixels per update. (value is inclusive)
#define STAR_COLOR 0xffff         // Star color in hex. 0xffff is white.

// Global variables.
Adafruit_TFTLCD tftlcd(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);  // The screen instance.
Star stars[STAR_COUNT];                                             // An array of star instances.

void setup(void)
{
    screenSetup();
    starsSetup();
}

/**
 * Prepare the screen for displaying.
 */
void screenSetup()
{
    tftlcd.reset();
    tftlcd.begin(SCREEN_IDENTIFIER);
    tftlcd.fillScreen(BACKGROUND_COLOR);  
}

/**
 * Prepare the stars for their first display.
 */
void starsSetup()
{
    // Loop through each star.
    for(int i = 0; i < STAR_COUNT; i++)
    {
        // Randomize its position and speed.
        stars[i].randomize(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, STAR_SPEED_MIN, STAR_SPEED_MAX);
    }
}

void loop(void)
{
    starsDraw();
}

/**
 * Update the positions of the stars and redraw them on the screen.
 */
void starsDraw()
{
    // Loop through each star.
    for(int i = 0; i < STAR_COUNT; i++)
    {
        // Remove the star from the screen by changing its pixel to the background color.
        tftlcd.drawPixel(stars[i].x, stars[i].y, BACKGROUND_COLOR);

        // Update the position of the star.
        stars[i].update();

        // If the star's Y position is off the screen after the update.
        if(stars[i].y >= SCREEN_HEIGHT)
        {
            // Randomize its position.
            stars[i].randomize(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, STAR_SPEED_MIN, STAR_SPEED_MAX);
            // Set its Y position back to the top.
            stars[i].y = 0;
        }

        // Draw the star at its new coordinate.
        tftlcd.drawPixel(stars[i].x, stars[i].y, STAR_COLOR);
    }
}

