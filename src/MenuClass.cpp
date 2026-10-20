#include "MenuClass.h" // Bind the implementation to your header file
#include "TextFunctions.h" // Bind the implementation to your header file
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Constructor implementation
MenuClass::MenuClass(int maxBuffSize, int maxRowSize, int maxLineSize, Adafruit_SSD1306& display): _display(display) {
  _maxBuffSize = maxBuffSize;        // Store the provided pin number in the private variable
  _maxRowSize = maxRowSize;
  _maxLineSize = maxLineSize;
  _display = display;

}


void MenuClass::displayMenu(Adafruit_SSD1306& display) {
    int16_t x1, y1;
    uint16_t w, h;
    char messageList[3][15];
    int startwidths[3];
    int totalSum = 0;
    int SCREEN_WIDTH = display.width();
    int SCREEN_HEIGHT = display.height();
    TextFunctions textFunc(100, 5, 32, display);
    _menuInd = 0;
    _mainMenu = 1;
    _gameMode = 0;

    strcpy(messageList[0],"Pong");
    strcpy(messageList[1], "Snake");
    strcpy(messageList[2], "Other Game");

    for (int i = 0; i < 3; i++){
        display.getTextBounds(messageList[i], 0, 0, &x1, &y1, &w, &h);
        startwidths[i] = (SCREEN_WIDTH - w)/2;
        _widths[i] = w;
        _heights[i] = h;
    }

    for (int i = 0; i < 3; i++){
        totalSum += _heights[i];
    }

    int startH = (SCREEN_HEIGHT - totalSum) / 2 + _heights[0];
    for (int i = 0; i < 3; i++){
        //display.setCursor(widths[i], startH);
        display.setCursor(startwidths[i], startH);
        display.print(messageList[i]);
        startH = startH + _heights[i];
    }

    display.setCursor(0,0);
    textFunc.drawAlignedText("Select which game you would like to play: ", display);
    startH = (SCREEN_HEIGHT - totalSum) / 2;
    int indx = 0;
    for (int i = 0; i < indx+1; i++){
        startH += _heights[i];
    }

    textFunc.invertPixelRange(display, startwidths[indx], startH, _widths[indx], _heights[indx]);

    display.display();

}

void MenuClass::displayGameMenu(Adafruit_SSD1306& display) {
    int16_t x1, y1;
    uint16_t w, h;
    char messageList[3][15];
    int startwidths[3];
    int totalSum = 0;
    int SCREEN_WIDTH = display.width();
    int SCREEN_HEIGHT = display.height();
    TextFunctions textFunc(100, 5, 32, display);
    _menuInd = 0;
    _mainMenu = 0;
    _gameMenuOption = 0;

    strcpy(messageList[0],"Start New Game");
    strcpy(messageList[1], "High Scores");
    strcpy(messageList[2], "Go Back");

    for (int i = 0; i < 3; i++){
        display.getTextBounds(messageList[i], 0, 0, &x1, &y1, &w, &h);
        startwidths[i] = (SCREEN_WIDTH - w)/2;
        _widths[i] = w;
        _heights[i] = h;
    }

    for (int i = 0; i < 3; i++){
        totalSum += _heights[i];
    }

    int startH = (SCREEN_HEIGHT - totalSum) / 2 + _heights[0];
    for (int i = 0; i < 3; i++){
        //display.setCursor(widths[i], startH);
        display.setCursor(startwidths[i], startH);
        display.print(messageList[i]);
        startH = startH + _heights[i];
    }

    display.setCursor(0,0);
    textFunc.drawAlignedText("What do you want to do? ", display);
    startH = (SCREEN_HEIGHT - totalSum) / 2;
    int indx = 0;
    for (int i = 0; i < indx+1; i++){
        startH += _heights[i];
    }

    textFunc.invertPixelRange(display, startwidths[indx], startH, _widths[indx], _heights[indx]);
    display.display();

}

void MenuClass::menuDown(Adafruit_SSD1306& display) {

    int startwidths[3];
    int SCREEN_WIDTH = display.width();
    int SCREEN_HEIGHT = display.height();
    int totalSum = 0;
    TextFunctions textFunc(100, 5, 32, display);

    for (int i = 0; i < 3; i++){
        startwidths[i] = (SCREEN_WIDTH - _widths[i])/2;
        totalSum += _heights[i];
    }

    int startH = (SCREEN_HEIGHT - totalSum) / 2;
    for (int i = 0; i < _menuInd+1; i++){
        startH += _heights[i];
    }

    textFunc.invertPixelRange(display, startwidths[_menuInd], startH, _widths[_menuInd], _heights[_menuInd]);

    _menuInd = _menuInd + 1;
    if (_menuInd>2){
        _menuInd = 0;
    }

    startH = startH = (SCREEN_HEIGHT - totalSum) / 2;
    for (int i = 0; i < _menuInd+1; i++){
        startH += _heights[i];
    }

    textFunc.invertPixelRange(display, startwidths[_menuInd], startH, _widths[_menuInd], _heights[_menuInd]);

    display.display();

    if (_mainMenu == 1){
        _gameMode = _menuInd;
    }
    else{
        _gameMenuOption = _menuInd;
    };

}