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
    int _xPos = 64;
    int _yPos = 32;
    int _prevXPos = _xPos;
    int _prevYPos = _yPos;
    int _xVel = 2;
    int _yVel = 0;
    int _t = 0;
    int _delay = 0;

    BallClass(Adafruit_SSD1306& display); 

    void createBall(Adafruit_SSD1306& display);
    void moveBall(Adafruit_SSD1306& display, PaddleClass homePaddle, PaddleClass awayPaddle);
};

#endif