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

    char messageList[3][15];
    _menuInd = 0;
    //_mainMenu = 1;
    _menuMode = 1;
    _gameMode = 0;

    strcpy(messageList[0],"Pong");
    strcpy(messageList[1], "Snake");
    strcpy(messageList[2], "Breakout");

    dispMenuOptions(display,messageList,"Select which game you would like to play: ");

}

void MenuClass::displayGameMenu(Adafruit_SSD1306& display) {

    char messageList[3][15];
    TextFunctions textFunc(100, 5, 32, display);
    _menuInd = 0;
    //_mainMenu = 0;
    _menuMode = 2;
    _gameMenuOption = 0;

    strcpy(messageList[0],"Start New Game");
    strcpy(messageList[1], "High Scores");
    strcpy(messageList[2], "Go Back");

    dispMenuOptions(display,messageList,"What do you want to do?");

}

void MenuClass::displayEndMenu(Adafruit_SSD1306& display) {

    char messageList[3][15];
    TextFunctions textFunc(100, 5, 32, display);
    _menuInd = 0;
    //_mainMenu = 0;
    _endGameOption = 0;

    strcpy(messageList[0],"Play Again");
    strcpy(messageList[1], "Go Home");
    strcpy(messageList[2], "\0");

    dispMenuOptions(display,messageList,"What do you want to do?");

}

void MenuClass::menuDown(Adafruit_SSD1306& display) {

    int startwidths[3];
    int SCREEN_WIDTH = display.width();
    int SCREEN_HEIGHT = display.height();
    int totalSum = 0;
    int menuSize = 3;
    TextFunctions textFunc(100, 5, 32, display);

    if (_menuMode == 3){
        menuSize = 2;
    }

    for (int i = 0; i < menuSize; i++){
        startwidths[i] = (SCREEN_WIDTH - _widths[i])/2;
        totalSum += _heights[i];
    }

    int startH = (SCREEN_HEIGHT - totalSum) / 2;
    for (int i = 0; i < _menuInd+1; i++){
        startH += _heights[i];
    }

    textFunc.invertPixelRange(display, startwidths[_menuInd], startH, _widths[_menuInd], _heights[_menuInd]);

    _menuInd = _menuInd + 1;
    if (_menuInd>menuSize-1){
        _menuInd = 0;
    }

    startH = startH = (SCREEN_HEIGHT - totalSum) / 2;
    for (int i = 0; i < _menuInd+1; i++){
        startH += _heights[i];
    }

    textFunc.invertPixelRange(display, startwidths[_menuInd], startH, _widths[_menuInd], _heights[_menuInd]);

    display.display();

    if (_menuMode == 1){
        _gameMode = _menuInd;
    }
    else if (_menuMode == 2){
        _gameMenuOption = _menuInd;
    }
    else{
        _endGameOption = _menuInd;
    }

}

void MenuClass::dispMenuOptions(Adafruit_SSD1306& display, char (&messageList)[3][15], char (*dispMessage)){
    int16_t x1, y1;
    uint16_t w, h;
    int startwidths[3];
    int totalSum = 0;
    int SCREEN_WIDTH = display.width();
    int SCREEN_HEIGHT = display.height();
    int menuSize = 3;
    TextFunctions textFunc(100, 5, 32, display);

    for (int i = 0; i < 3; i++){
        if(messageList[i][0]=='\0'){
            menuSize = i;
        }
    }

    for (int i = 0; i < menuSize; i++){
        display.getTextBounds(messageList[i], 0, 0, &x1, &y1, &w, &h);
        startwidths[i] = (SCREEN_WIDTH - w)/2;
        _widths[i] = w;
        _heights[i] = h;
    }

    for (int i = 0; i < menuSize; i++){
        totalSum += _heights[i];
    }

    int startH = (SCREEN_HEIGHT - totalSum) / 2 + _heights[0];
    for (int i = 0; i < menuSize; i++){
        display.setCursor(startwidths[i], startH);
        display.print(messageList[i]);
        startH = startH + _heights[i];
    }

    display.setCursor(0,0);
    textFunc.drawAlignedText(dispMessage,display);
    startH = (SCREEN_HEIGHT - totalSum) / 2;
    int indx = 0;
    for (int i = 0; i < indx+1; i++){
        startH += _heights[i];
    }

    textFunc.invertPixelRange(display, startwidths[indx], startH, _widths[indx], _heights[indx]);
    display.display();
}