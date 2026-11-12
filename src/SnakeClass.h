#ifndef SNAKECLASS_H    // Include guard: Prevents duplicate compilation errors
#define SNAKECLASS_H

#include <Arduino.h> // Allows the use of built-in Arduino functions like digitalWrite()
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PaddleClass.h>
#include <BallClass.h>

class SnakeClass {
  private:
    Adafruit_SSD1306& _display;

  public:
    // Constructor: Runs automatically when a class instance is created
    int _score = 0;
    const static int _MAX_COLUMNS = 24;
    int _snakeInfo[2][_MAX_COLUMNS][5];
    int _wayPointInfo[4];
    int _initialLength = 16;
    /*int _xVel = 0;
    int _yVel = 1;*/
    int _xVel = -1;
    int _yVel = 0;
    int _counter = 0;
    int _t = 0;
    int _calcFlag = 0;
    int _cw = 1;

    SnakeClass(Adafruit_SSD1306& display); 

    void gameStartup(Adafruit_SSD1306& display);
    void createSnake(Adafruit_SSD1306& display);
    void moveSnake(Adafruit_SSD1306& display);
    void changeDirection(int direction);

};

#endif