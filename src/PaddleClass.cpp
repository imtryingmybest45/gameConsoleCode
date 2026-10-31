#include "PaddleClass.h" // Bind the implementation to your header file
#include "MenuClass.h" // Bind the implementation to your header file
#include "TextFunctions.h" // Bind the implementation to your header file
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BallClass.h>

// Constructor implementation
PaddleClass::PaddleClass(Adafruit_SSD1306& display, int homePaddle): _display(display) {
    _homePaddle = homePaddle;        // Store the provided pin number in the private variable
    _display = display;
    if (_homePaddle == 1){
        _xPos = 5;
    }
    else{
        _xPos = display.width()-9;
    }
    if (_paddleLength % 2 != 0){
        _paddleLength = _paddleLength + 1;
    }
    _yRangeMin = display.height()/2 - _paddleLength/2;
    _yRangeMax = display.height()/2 + _paddleLength/2;
    _activeCols = (_paddleLength > _MAX_COLUMNS) ? _MAX_COLUMNS : _paddleLength;
    _paddlePos[_activeCols];

}

void PaddleClass::createPaddle(Adafruit_SSD1306 &display) {
    drawPaddle(display, 0, 1);
    //display.display();
}

void PaddleClass::movePaddleDown(Adafruit_SSD1306 &display, int yinc) {
    if (_paddlePos[_paddleLength-1] <= display.height()-2){
        drawPaddle(display, 0, 0);
        drawPaddle(display, yinc, 1);
    }
}

void PaddleClass::movePaddleUp(Adafruit_SSD1306 &display, int yinc) {
    if (_paddlePos[0] >= 3){
        drawPaddle(display, 0, 0);
        drawPaddle(display, -yinc, 1);
    }
}

void PaddleClass::drawPaddle(Adafruit_SSD1306 &display, int yInc, int color){
    _yRangeMin = _yRangeMin + yInc;
    _yRangeMax = _yRangeMax + yInc;
    int yInd = 0;
    for (int x = _xPos; x <= _xPos + 1; x++){
        for (int y = _yRangeMin; y < _yRangeMax; y++) {
            if (color == 1){
                display.drawPixel(x, y, SSD1306_WHITE);
                _paddlePos[yInd] = y;
            }
            else{
                display.drawPixel(x, y, SSD1306_BLACK);
            }
            yInd++;
        }
        yInd = 0;
    }
}

void PaddleClass::moveAwayPaddle(Adafruit_SSD1306 &display, BallClass &ball){
    int startTracking = display.width()/2 - 50;
    int endTracking = display.width()/2 + 25;
    if(ball._xVel > 0 && ball._xPos > startTracking && ball._xPos < endTracking){
        if ((ball._yPos - _paddlePos[_paddleLength/2])>0){
            movePaddleDown(display, 1);
        }
        else if ((ball._yPos - _paddlePos[_paddleLength/2]<0)){
            movePaddleUp(display, 1);
        }
    }
    else if (ball._xVel > 0 && (ball._xPos == endTracking || ball._xPos-1 == endTracking) && _calcFlag){
        _counter = 0;
        _t = 0;
        int t = floor((_xPos - endTracking)/abs(ball._xVel));
        int y0 = ball._yPos;
        int y;
        int vy = ball._yVel;
        int randInt = random(1,5);
        for (int ii = 0; ii <= t; ii++){
            y = y0 + vy;
            if (y <= 1 || y >= display.height()-1){
                vy = -vy;
            }
            y0 = y;
        }
        /*if (y < _paddlePos[0]){
            _delt = _paddlePos[0] - y;
        }
        else if (y > _paddlePos[_paddleLength-1]){
            _delt = _paddlePos[_paddleLength-1] - y;
        }
        else{
            _delt = _paddlePos[_paddleLength/2] - y;
        }*/
        _delt = _paddlePos[_paddleLength/2] - y;
        _paddleVel = 2;
        _counter = floor(abs(_delt)/_paddleVel)-randInt;
        //_counter = floor(abs(_delt/_paddleVel));
        _calcFlag = 0;
    }
    else if (ball._xVel > 0 && ball._xPos >= endTracking){
        _calcFlag = 1;
        if (_t < _counter){
            if (_delt > 0){
                movePaddleUp(display, _paddleVel);
            }
            else if (_delt < 0){
                movePaddleDown(display, _paddleVel);
            }
            _t++;
        }
    }
}