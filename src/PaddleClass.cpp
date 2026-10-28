#include "PaddleClass.h" // Bind the implementation to your header file
#include "MenuClass.h" // Bind the implementation to your header file
#include "TextFunctions.h" // Bind the implementation to your header file
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
    display.display();
}

void PaddleClass::movePaddleDown(Adafruit_SSD1306 &display, int yinc) {
    if (_paddlePos[_paddleLength] <= display.height()-2){
        drawPaddle(display, 0, 0);
        drawPaddle(display, yinc, 1);
        display.display();
    }
}

void PaddleClass::movePaddleUp(Adafruit_SSD1306 &display, int yinc) {
    if (_paddlePos[0] >= 3){
        drawPaddle(display, 0, 0);
        drawPaddle(display, yinc, 1);
        display.display();
    }
}

void PaddleClass::drawPaddle(Adafruit_SSD1306 &display, int yInc, int color){
    _yRangeMin = _yRangeMin + yInc;
    _yRangeMax = _yRangeMax + yInc;
    int yInd = 0;
    for (int x = _xPos; x <= _xPos + 1; x++){
        for (int y = _yRangeMin; y <= _yRangeMax; y++) {
            if (color == 1){
                display.drawPixel(x, y, SSD1306_WHITE);
                _paddlePos[yInd] = y;
            }
            else{
                display.drawPixel(x, y, SSD1306_BLACK);
            }
            yInd++;
        }
    }
}