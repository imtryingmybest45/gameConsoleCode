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
    int _paddleWidth = 2;
    int _paddleLength = 12;
    int _yPos = 0;
    int _xRangeMin;
    int _xRangeMax;
    int _paddlePos[2][12];

    PaddleClass(int homePaddle, Adafruit_SSD1306& display); 

    void createPaddle(Adafruit_SSD1306& display);
    void movePaddleDown(Adafruit_SSD1306& display);
    void movePaddleUp(Adafruit_SSD1306& display);
};

#endif