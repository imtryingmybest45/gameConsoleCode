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
                display.drawPixel(x, y, SSD1306_WHITE);
            }
        }
    }
    else{
        for (int x = _xPos; x <= _xPos+1; x++){
            for (int y = _yPos-1; y <= _yPos; y++){
                display.drawPixel(x, y, SSD1306_BLACK);
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

//This is a new comment to check if my workingbranch is working

void BallClass::moveBall(Adafruit_SSD1306 &display, PaddleClass &homePaddle, PaddleClass &awayPaddle){
    if (_t >= _delay){
        _t = 0;
        if (_xPos == homePaddle._xPos+2 && _yPos >= homePaddle._paddlePos[0] && _yPos <= homePaddle._paddlePos[homePaddle._paddleLength]){
            _xVel = -_xVel;
            if(_yPos <= homePaddle._paddlePos[homePaddle._paddleLength/2 - 2]){
                _yVel = -abs(_xVel);
            }
            else if(_yPos >= homePaddle._paddlePos[homePaddle._paddleLength/2 + 2]){
                _yVel = abs(_xVel);
            }
            else{
                _yVel = 0;
            }
        }
        else if( _xPos+1 == awayPaddle._xPos-1 && _yPos >= awayPaddle._paddlePos[0] && _yPos <= awayPaddle._paddlePos[awayPaddle._paddleLength]){
            _xVel = -_xVel;
            if(_yPos <= awayPaddle._paddlePos[awayPaddle._paddleLength/2 - 2]){
                _yVel = -abs(_xVel);
            }
            else if(_yPos >= awayPaddle._paddlePos[awayPaddle._paddleLength/2 + 2]){
                _yVel = abs(_xVel);
            }
            else{
                _yVel = 0;
            }
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