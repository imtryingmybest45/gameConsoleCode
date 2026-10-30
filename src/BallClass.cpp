#include "BallClass.h" // Bind the implementation to your header file
#include "MenuClass.h" // Bind the implementation to your header file
#include "TextFunctions.h" // Bind the implementation to your header file
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PaddleClass.h>

// Constructor implementation
BallClass::BallClass(Adafruit_SSD1306& display, int xVel): _display(display) {
  _xVel = xVel;
  _display = display;
  _xPos = display.width()/2;
  _yPos = display.height()/2;
  _prevXPos = _xPos;
  _prevYPos = _yPos;

}

void BallClass::drawBall(Adafruit_SSD1306 &display, int color, PaddleClass &homePaddle, PaddleClass &awayPaddle){
    if (color == 1){
        for (int x = _xPos; x <= _xPos+1; x++){
            for (int y = _yPos-1; y <= _yPos; y++){
                if (x >= homePaddle._xPos+2 && x < awayPaddle._xPos){
                    display.drawPixel(x, y, SSD1306_WHITE);
                }
            }
        }
    }
    else{
        for (int x = _xPos; x <= _xPos+1; x++){
            for (int y = _yPos-1; y <= _yPos; y++){
                if (x >= homePaddle._xPos+2 && x < awayPaddle._xPos){
                    display.drawPixel(x, y, SSD1306_BLACK);
                }
            }
        }
    }
}

void BallClass::createBall(Adafruit_SSD1306 &display) {
    for (int x = _xPos-1; x <= _xPos; x++){
        for (int y = _yPos-1; y <= _yPos; y++){
            display.drawPixel(x, y, SSD1306_WHITE);
        }
    }
    display.display();
}

void BallClass::calcYVel(PaddleClass &homePaddle, PaddleClass &awayPaddle, int homepaddle){
    int delt;
    int left;
    int inc;
    if (homepaddle == 1){
        delt = floor(homePaddle._paddleLength/5);
        left = homePaddle._paddleLength - 5*delt;
        inc = ceil(left/2);
        if (_yPos >= homePaddle._paddlePos[0] && _yPos <= homePaddle._paddlePos[delt+inc-1]){
            _yVel = -2;
        }
        else if (_yPos >= homePaddle._paddlePos[delt+inc] && _yPos <= homePaddle._paddlePos[2*delt+inc-1]){
            _yVel = -1;
        }
        else if (_yPos >= homePaddle._paddlePos[2*delt+inc] && _yPos <= homePaddle._paddlePos[3*delt+inc-1]){
            _yVel = 0;
        }
        else if (_yPos >= homePaddle._paddlePos[3*delt+inc] && _yPos <= homePaddle._paddlePos[4*delt+inc-1]){
            _yVel = 1;
        }
        else{
            _yVel = 2;
        }
    }
    else{
        delt = floor(awayPaddle._paddleLength/5);
        left = awayPaddle._paddleLength - 5*delt;
        inc = ceil(left/2);
        if (_yPos >= awayPaddle._paddlePos[0] && _yPos <= awayPaddle._paddlePos[delt+inc-1]){
            _yVel = -2;
        }
        else if (_yPos >= awayPaddle._paddlePos[delt+inc] && _yPos <= awayPaddle._paddlePos[2*delt+inc-1]){
            _yVel = -1;
        }
        else if (_yPos >= awayPaddle._paddlePos[2*delt+inc] && _yPos <= awayPaddle._paddlePos[3*delt+inc-1]){
            _yVel = 0;
        }
        else if (_yPos >= awayPaddle._paddlePos[3*delt+inc] && _yPos <= awayPaddle._paddlePos[4*delt+inc-1]){
            _yVel = 1;
        }
        else{
            _yVel = 2;
        }
    }

}

void BallClass::moveBall(Adafruit_SSD1306 &display, PaddleClass &homePaddle, PaddleClass &awayPaddle){
    if (_t >= _delay){
        _t = 0;
        if (_xPos >= homePaddle._xPos+1 && _xPos <= homePaddle._xPos+2 && _yPos >= homePaddle._paddlePos[0] && _yPos <= homePaddle._paddlePos[homePaddle._paddleLength-1]){
            _xVel = -_xVel;
           calcYVel(homePaddle, awayPaddle, 1);

        }
        else if (_xPos+1 >= awayPaddle._xPos-1 && _xPos+1 <= awayPaddle._xPos && _yPos >= awayPaddle._paddlePos[0] && _yPos <= awayPaddle._paddlePos[awayPaddle._paddleLength-1]){
            _xVel = -_xVel;
           calcYVel(homePaddle, awayPaddle, 0);
        }
        if (_yPos <= 3 || _yPos >= 62){
            _yVel = -_yVel;
        }
    }
    drawBall(display, 0, homePaddle, awayPaddle);
    _xPos = _prevXPos + _xVel;
    _yPos = _prevYPos + _yVel;
    drawBall(display, 1, homePaddle, awayPaddle);
    _prevXPos = _xPos;
    _prevYPos = _yPos;
    display.display();
}