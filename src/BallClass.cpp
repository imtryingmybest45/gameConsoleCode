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
  _xPos = display.width()/2;
  _yPos = display.height()/2;
  _prevXPos = _xPos;
  _prevYPos = _yPos;

}

void BallClass::drawBall(Adafruit_SSD1306 &display, int color, PaddleClass &homePaddle, PaddleClass &awayPaddle){
    int badPixel = 0;
    for (int x = _xPos-1; x <= _xPos; x++){
        for (int y = _yPos-1; y <= _yPos; y++){
            if (color == 0){
                if (_xVel > 0){
                    for (int iy = 0; iy < awayPaddle._paddleLength; iy++){
                        if (x == awayPaddle._xRangeMin && y == awayPaddle._paddlePos[iy]){
                            badPixel = 1;
                            break;
                        }
                    }
                    if (badPixel){
                        break;
                    }
                    else{
                        display.drawPixel(x,y,SSD1306_BLACK);
                        }
                    }
                else{
                    for (int iy = 0; iy < homePaddle._paddleLength; iy++){
                        if (x == homePaddle._xRangeMax-1 && y == homePaddle._paddlePos[iy]){
                            badPixel = 1;
                            break;
                        }
                    }
                    if (badPixel){
                        break;
                    }
                    else{
                        display.drawPixel(x,y,SSD1306_BLACK);
                    }
                }
            }
            else{
                display.drawPixel(x, y, SSD1306_WHITE);
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
        drawBall(display, 0, homePaddle, awayPaddle);
        _xPos = _prevXPos + _xVel;
        _yPos = _prevYPos + _yVel;
       drawBall(display, 1, homePaddle, awayPaddle);
        _prevXPos = _xPos;
        _prevYPos = _yPos;
        if (_xPos == awayPaddle._xRangeMin-1){
            /*for (int iy = 0; iy <= awayPaddle._paddleLength; iy++){
                //if (_yPos == awayPaddle._paddlePos[iy][1]){
                if (_yPos == awayPaddle._paddlePos[iy]){
                    _xVel = -_xVel;
                    break;
                }
            }*/
           _xVel = -_xVel;
        }
        else if (_xPos == homePaddle._xRangeMax+1){
            for (int iy = 0; iy <= homePaddle._paddleLength; iy++){
                if (_yPos == homePaddle._paddlePos[iy]){
                    if (iy < homePaddle._paddleLength/2 - 1){
                        _yVel = -1;
                    }
                    else if (iy > awayPaddle._paddleLength/2 + 1){
                        _yVel = 1;
                    }
                    else{
                        _yVel = 0;
                    }
                    _xVel = -_xVel;
                    break;
                }
            }
        }
        if (_yPos <= 1){
            _yVel = 1;
        }
        else if (_yPos >= display.height()){
            _yVel = -1;
        }
    }
    else{
        _t = _t + 1;
    }
    display.display();
}