#include "PaddleClass.h" // Bind the implementation to your header file
#include "MenuClass.h" // Bind the implementation to your header file
#include "TextFunctions.h" // Bind the implementation to your header file
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Constructor implementation
PaddleClass::PaddleClass(int homePaddle, Adafruit_SSD1306& display): _display(display) {
  _homePaddle = homePaddle;        // Store the provided pin number in the private variable
  _display = display;
  if (_homePaddle == 1){
    _xRangeMin = 4;
  }
  else{
    _xRangeMin = 119;
  }
  _xRangeMax = _xRangeMin + _paddleWidth;
}

void PaddleClass::createPaddle(Adafruit_SSD1306 &display) {
    int xRangeMin;
    int xRangeMax;
    int yRangeMin = (int)round(32 - _paddleLength/2);
    int yRangeMax = (int)round(32 + _paddleLength/2);
    if (_homePaddle == 1){
        xRangeMin = 4;
    }
    else{
        xRangeMin = 119;
    }
    xRangeMax = xRangeMin + _paddleWidth;
    for (int x = xRangeMin; x < xRangeMax; x++){
        for (int y = yRangeMin; y <= yRangeMax; y++) {
            display.drawPixel(x, y, SSD1306_WHITE);
        }
    }
    display.display();
    _yPos = yRangeMin + (int)round((_paddleLength)/2);
}

void PaddleClass::movePaddleDown(Adafruit_SSD1306 &display) {
    if (_yPos + _paddleLength/2 < 60){
        for (int x = _xRangeMin; x < _xRangeMax; x++){
            display.drawPixel(x, _yPos - (_paddleLength/2),SSD1306_BLACK);
            display.drawPixel(x, _yPos - (_paddleLength/2)+1,SSD1306_BLACK);
            display.drawPixel(x, _yPos + (_paddleLength/2)+1,SSD1306_WHITE);
            display.drawPixel(x, _yPos + (_paddleLength/2)+2,SSD1306_WHITE);
        }
        display.display();
        _yPos = _yPos + 2;
    }
}

void PaddleClass::movePaddleUp(Adafruit_SSD1306 &display) {
    if (_yPos - _paddleLength/2 > 3){
        for (int x = _xRangeMin; x < _xRangeMax; x++){
            display.drawPixel(x, _yPos + (_paddleLength/2),SSD1306_BLACK);
            display.drawPixel(x, _yPos + (_paddleLength/2)-1,SSD1306_BLACK);
            display.drawPixel(x, _yPos - (_paddleLength/2)-1,SSD1306_WHITE);
            display.drawPixel(x, _yPos - (_paddleLength/2)-2,SSD1306_WHITE);
        }
        display.display();
        _yPos = _yPos - 2;
    }
}