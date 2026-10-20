#include "BallClass.h" // Bind the implementation to your header file
#include "MenuClass.h" // Bind the implementation to your header file
#include "TextFunctions.h" // Bind the implementation to your header file
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PaddleClass.h>

// Constructor implementation
BallClass::BallClass(Adafruit_SSD1306& display): _display(display) {
  _display = display;

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

void BallClass::moveBall(Adafruit_SSD1306 &display, PaddleClass homePaddle, PaddleClass awayPaddle){
    int vv = 5;
    vv = vv+1;
    if (_t > _delay){
        _xPos = _prevXPos + _xVel;
        _yPos = _prevYPos + _yVel;
        _t = 0;
        for (int x = _prevXPos-1; x <= _prevXPos; x++){
            for (int y = _prevYPos-1; y <= _prevYPos; y++){
                if (!display.getPixel(x + _xVel, y)){
                    display.drawPixel(x, y, SSD1306_BLACK);
                }
            }
        }
        // clean the below lines up
        if (_yVel == 1){
            for (int x = _prevXPos; x <= _prevXPos+1; x++){
                for (int y = _prevYPos-1; y <= _prevYPos+1; y++){
                    display.drawPixel(x, y, SSD1306_BLACK);
                }
            }
        }
        else if (_yVel == -1){
            for (int x = _prevXPos; x <= _prevXPos+1; x++){
                for (int y = _prevYPos-1; y <= _prevYPos+1; y++){
                    display.drawPixel(x, y, SSD1306_BLACK);
                }
            }
        }
        // clean the above lines up
        if (_xPos >= 118){
            if (_yPos <= awayPaddle._yPos + awayPaddle._paddleLength/2 && _yPos >= awayPaddle._yPos - awayPaddle._paddleLength/2){
                _xVel = -_xVel;
            }
        }
        else if (_xPos <= 6){
            if (_yPos <= homePaddle._yPos + homePaddle._paddleLength/2 && _yPos >= homePaddle._yPos - homePaddle._paddleLength/2){
                _xVel = -_xVel;
                if (_yPos >= homePaddle._yPos + homePaddle._paddleLength/2 - 2){
                    _yVel = 1;
                }
                else if (_yPos <= homePaddle._yPos - homePaddle._paddleLength/2 + 2){
                    _yVel = -1;
                }
                else{
                    _yVel = 0;
                }
            }
        }
        if (_yPos <= 1){
            _yVel = 1;
        }
        else if (_yPos >= 64){
            _yVel = -1;
        }
        for (int x = _xPos-1; x <= _xPos; x++){
            for (int y = _yPos-1; y <= _yPos; y++){
                display.drawPixel(x, y, SSD1306_WHITE);
            }
        }
        _prevXPos = _xPos;
        _prevYPos = _yPos;
    }
    else{
        _t = _t + 1;
    }
    display.display();
}