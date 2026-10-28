#ifndef BALLCLASS_H    // Include guard: Prevents duplicate compilation errors
#define BALLCLASS_H

#include <Arduino.h> // Allows the use of built-in Arduino functions like digitalWrite()
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PaddleClass.h>

class BallClass {
  private:
    Adafruit_SSD1306& _display;

  public:
    // Constructor: Runs automatically when a class instance is created
    int _xPos;
    int _yPos;
    int _prevXPos;
    int _prevYPos;
    int _xVel;
    int _yVel = 0;
    int _t = 0;
    int _delay = 0;

    BallClass(Adafruit_SSD1306& display, int xVel); 

    void createBall(Adafruit_SSD1306& display);
    void moveBall(Adafruit_SSD1306& display, PaddleClass &homePaddle, PaddleClass &awayPaddle);
    void drawBall(Adafruit_SSD1306& display, int color, PaddleClass &homePaddle, PaddleClass &awayPaddle);
};

#endif