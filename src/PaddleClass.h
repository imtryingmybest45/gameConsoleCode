#ifndef PADDLECLASS_H    // Include guard: Prevents duplicate compilation errors
#define PADDLECLASS_H

#include <Arduino.h> // Allows the use of built-in Arduino functions like digitalWrite()
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class PaddleClass {
  private:
    int _homePaddle;
    Adafruit_SSD1306& _display;

  public:
    // Constructor: Runs automatically when a class instance is created
    int _paddleLength = 12;
    int _xPos = 0;
    int _yRangeMin;
    int _yRangeMax;
    const static int _MAX_COLUMNS = 24;
    int _paddlePos[_MAX_COLUMNS];
    int _activeRows;
    int _activeCols;

    PaddleClass(Adafruit_SSD1306& display, int homePaddle); 

    void createPaddle(Adafruit_SSD1306& display);
    void movePaddleDown(Adafruit_SSD1306& display, int yInc);
    void movePaddleUp(Adafruit_SSD1306& display, int yInc);
    void drawPaddle(Adafruit_SSD1306& display, int yInc, int color);
};

#endif