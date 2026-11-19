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
    int _scoreTracker = 0;
    const static int _MAX_COLUMNS = 210;
    const static int _MAX_WP_COLUMNS = 20;
    int _snakeInfo[2][_MAX_COLUMNS][5];
    int _wayPointInfo[5][_MAX_WP_COLUMNS];
    int _initialLength = 10;
    int _currentLength = _initialLength;
    int _xVel = -1;
    int _yVel = 0;
    int _cw = 0;
    int _calcLastSeg = 0;
    int _counter = 0;
    int _spawnFlag = 1;
    int _eatFood = 0;
    int _dirCounter = 2;
    int _gameOver = 0;
    int _numWayPoints = 0;
    const static int _foodSize = 2;
    int _foodPos[_foodSize][_foodSize][2];

    SnakeClass(Adafruit_SSD1306& display); 

    void gameStartup(Adafruit_SSD1306& display);
    void createSnake(Adafruit_SSD1306& display);
    void moveSnake(Adafruit_SSD1306& display);
    void changeDirection(int direction);
    void rotateClockwise(int c);
    void rotateCounterClockwise(int c);
    void assignVelocities();
    void rotateSegments();
    void decrementSnake(int c);
    void spawnFood(Adafruit_SSD1306 &display);
    void checkFood(Adafruit_SSD1306 &display);
    void increaseLength();
    void checkCollision(Adafruit_SSD1306 &display);
    void restartGame(Adafruit_SSD1306 &display);
    void displayScore(Adafruit_SSD1306 &display);
    void findNumWayPoints();
}; 

#endif