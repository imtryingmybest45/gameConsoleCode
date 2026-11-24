#ifndef BREAKOUTCLASS_H    // Include guard: Prevents duplicate compilation errors
#define BREAKOUTCLASS_H

#include <Arduino.h> // Allows the use of built-in Arduino functions like digitalWrite()
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <BallClass.h>

class BallClass;

class BreakoutClass {
  private:
    int _homePaddle;
    Adafruit_SSD1306& _display;

  public:
    // Constructor: Runs automatically when a class instance is created
    int _paddleWidth = 18;
    int _yPos = 54;
    int _xRangeMin;
    int _xRangeMax;
    int _score = 0;
    const static int _maxLives = 5;
    int _numLives = _maxLives;
    const static int _MAX_COLUMNS = 24;
    const static int _MAX_BLOCK_ROWS = 5;
    const static int _MAX_BLOCK_COLUMNS = 30;
    int _paddlePos[_MAX_COLUMNS];
    int _activeRows;
    int _activeCols;
    int _prevBallXPos;
    int _prevBallYPos;
    int _ballXPos;
    int _ballYPos;
    int _ballXVel = 0;
    int _ballYVel = 0;
    int _launchBall = 0;
    int _blockHeight = 4;
    int _blockWidth = 9;
    int _topMargin = 4;
    int _botMargin = 8;
    int _sideMargin = 4;
    int _numCols = (_display.width()-(2*_sideMargin))/(_blockWidth+1);
    int _numRows = 4;
    int _blockArr[_MAX_BLOCK_ROWS][_MAX_BLOCK_COLUMNS][2];
    int _numBlocks = _numRows*_numCols;
    int _livesArr[_maxLives-1][2];


    BreakoutClass(Adafruit_SSD1306& display); 

    void gameStartup(Adafruit_SSD1306& display);
    void createPaddle(Adafruit_SSD1306& display);
    void movePaddleRight(Adafruit_SSD1306& display, int xInc);
    void movePaddleLeft(Adafruit_SSD1306& display, int xInc);
    void drawPaddle(Adafruit_SSD1306& display, int xInc, int color); 
    void createBall(Adafruit_SSD1306& display);
    void moveBall(Adafruit_SSD1306& display);
    void drawBall(Adafruit_SSD1306& display, int color);
    void createBoard(Adafruit_SSD1306& display);
    void checkCollisions(Adafruit_SSD1306& display);
    void checkPaddleCollision();
    void checkBlockCollision(Adafruit_SSD1306& display);
    void drawBlock(Adafruit_SSD1306& display, int xPos, int yPos, int color);
    void resetLaunch(Adafruit_SSD1306& display);
    void displayScore(Adafruit_SSD1306& display);
    void restartGame(Adafruit_SSD1306& display);
    void updateLives(Adafruit_SSD1306& display);
    void populateLives();
};

#endif