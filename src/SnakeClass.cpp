#include "SnakeClass.h"    // Bind the implementation to your header file
#include "MenuClass.h"     // Bind the implementation to your header file
#include "TextFunctions.h" // Bind the implementation to your header file
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PaddleClass.h>
#include <BallClass.h>
#include <iostream>
#include <vector>

// Constructor implementation
SnakeClass::SnakeClass(Adafruit_SSD1306 &display) : _display(display){
    _display = display;
}

void SnakeClass::gameStartup(Adafruit_SSD1306 &display){
    TextFunctions txtFuncs(100, 5, 32, display);
    display.clearDisplay();
    display.setCursor(0, 0);
    txtFuncs.drawCenteredText("Are you aready to play snake?", display);
    delay(1500);
    display.clearDisplay();
}

void SnakeClass::createSnake(Adafruit_SSD1306 &display){
    _xVel = -1;
    _yVel = 0;
    _counter = 0;
    _t = 0;
    _calcFlag = 0;
    _cw = 0;
    display.setCursor(display.width() / 2 - _initialLength, display.height() / 2);
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _MAX_COLUMNS; j++){
            _snakeInfo[i][j][0] = 0;
            _snakeInfo[i][j][1] = 0;
            _snakeInfo[i][j][2] = 0;
            _snakeInfo[i][j][3] = 0;
            _snakeInfo[i][j][4] = 0;
        }
    }
    for (int i = 0; i < 2; i++){
        for (int j = 0; j<_initialLength; j++){
            //This is for going down
            /*_snakeInfo[i][j][0] = display.width()/2+i;
            _snakeInfo[i][j][1] = display.height()/2-j;*/

            //This is for going left
            _snakeInfo[i][j][0] = display.width()/2+j;
            _snakeInfo[i][j][1] = display.height()/2+i;
            _snakeInfo[i][j][2] = _xVel;
            _snakeInfo[i][j][3] = _yVel;
            display.drawPixel(_snakeInfo[i][j][0],_snakeInfo[i][j][1],SSD1306_WHITE);
        }
    }
    for (int i = 0; i < 4; i++){
        _wayPointInfo[i] = 0;
    }
    display.display();
}

void SnakeClass::moveSnake(Adafruit_SSD1306 &display){
    int r;
    int c;
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _initialLength; j++){
            display.drawPixel(_snakeInfo[i][j][0],_snakeInfo[i][j][1],SSD1306_BLACK);
        }
    }
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _initialLength; j++){
            if (j % 2 != 0){
                if (abs(_yVel)>0){
                    if (_snakeInfo[i][j][0] == _wayPointInfo[0]){
                        _snakeInfo[i][j-1][2] = _wayPointInfo[2];
                        _snakeInfo[i][j-1][3] = _wayPointInfo[3];
                        _snakeInfo[i][j][2] = _wayPointInfo[2];
                        _snakeInfo[i][j][3] = _wayPointInfo[3];
                    }
                }
                else{
                    if (_snakeInfo[i][j][1] == _wayPointInfo[1]){
                        _snakeInfo[i][j-1][2] = _wayPointInfo[2];
                        _snakeInfo[i][j-1][3] = _wayPointInfo[3];
                        _snakeInfo[i][j][2] = _wayPointInfo[2];
                        _snakeInfo[i][j][3] = _wayPointInfo[3];
                    }
                }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
            }
        }
    }
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _initialLength; j++){
            _snakeInfo[i][j][0] += _snakeInfo[i][j][2];
            _snakeInfo[i][j][1] += _snakeInfo[i][j][3];
        }
    }
    for (int j = 0; j < _initialLength-1; j++){
        if (j % 2 != 0){
            if (_cw == 0){
                if (_xVel == 0 && _snakeInfo[1][j][2] != _snakeInfo[1][j+1][2] && _snakeInfo[0][j][1] != _snakeInfo[1][j+1][1]){
                    r = 0;
                    c = j-1;
                    int tempX = _snakeInfo[r][c][0];                         // Save Top-Left
                    _snakeInfo[r][c][0] = _snakeInfo[r+1][c][0];             // Move Bottom-Left to Top-Left
                    _snakeInfo[r+1][c][0] = _snakeInfo[r+1][c+1][0];         // Move Bottom-Right to Bottom-Left
                    _snakeInfo[r+1][c+1][0] = _snakeInfo[r][c+1][0];         // Move Top-Right to Bottom-Right
                    _snakeInfo[r][c+1][0] = tempX;                           // Move saved Top-Left to Top-Right

                    // 2. Handle Y Coordinates [1]
                    int tempY = _snakeInfo[r][c][1];                         // Save Top-Left
                    _snakeInfo[r][c][1] = _snakeInfo[r+1][c][1];             // Move Bottom-Left to Top-Left
                    _snakeInfo[r+1][c][1] = _snakeInfo[r+1][c+1][1];         // Move Bottom-Right to Bottom-Left
                    _snakeInfo[r+1][c+1][1] = _snakeInfo[r][c+1][1];         // Move Top-Right to Bottom-Right
                    _snakeInfo[r][c+1][1] = tempY;
                    if (j == _initialLength-3){
                        _calcFlag = 1;
                    }  
                }
                else if (_yVel == 0 && _snakeInfo[1][j][2] != _snakeInfo[1][j+1][2] && _snakeInfo[0][j][0] != _snakeInfo[1][j+1][0]){
                    r = 0;
                    c = j-1;
                    int tempX = _snakeInfo[r][c][0];                         // Save Top-Left
                    _snakeInfo[r][c][0] = _snakeInfo[r+1][c][0];             // Move Bottom-Left to Top-Left
                    _snakeInfo[r+1][c][0] = _snakeInfo[r+1][c+1][0];         // Move Bottom-Right to Bottom-Left
                    _snakeInfo[r+1][c+1][0] = _snakeInfo[r][c+1][0];         // Move Top-Right to Bottom-Right
                    _snakeInfo[r][c+1][0] = tempX;                           // Move saved Top-Left to Top-Right

                    // 2. Handle Y Coordinates [1]
                    int tempY = _snakeInfo[r][c][1];                         // Save Top-Left
                    _snakeInfo[r][c][1] = _snakeInfo[r+1][c][1];             // Move Bottom-Left to Top-Left
                    _snakeInfo[r+1][c][1] = _snakeInfo[r+1][c+1][1];         // Move Bottom-Right to Bottom-Left
                    _snakeInfo[r+1][c+1][1] = _snakeInfo[r][c+1][1];         // Move Top-Right to Bottom-Right
                    _snakeInfo[r][c+1][1] = tempY;
                    // 1. Handle X Coordinates [0]
                    if (j == _initialLength-3){
                        _calcFlag = 1;
                    }   
                }
            }
            else{
                if (_xVel == 0 && _snakeInfo[1][j][2] != _snakeInfo[1][j+1][2] && _snakeInfo[0][j+1][1] != _snakeInfo[1][j][1]){
                    r = 0;
                    c = j-1;
                    // 1. Handle X Coordinates [0]
                    int tempX = _snakeInfo[r][c][0];                         // Save Top-Left
                    _snakeInfo[r][c][0] = _snakeInfo[r][c+1][0];             // Move Top-Right to Top-Left
                    _snakeInfo[r][c+1][0] = _snakeInfo[r+1][c+1][0];         // Move Bottom-Right to Top-Right
                    _snakeInfo[r+1][c+1][0] = _snakeInfo[r+1][c][0];         // Move Bottom-Left to Bottom-Right
                    _snakeInfo[r+1][c][0] = tempX;                           // Move saved Top-Left to Bottom-Left

                    // 2. Handle Y Coordinates [1]
                    int tempY = _snakeInfo[r][c][1];                         // Save Top-Left
                    _snakeInfo[r][c][1] = _snakeInfo[r][c+1][1];             // Move Top-Right to Top-Left
                    _snakeInfo[r][c+1][1] = _snakeInfo[r+1][c+1][1];         // Move Bottom-Right to Top-Right
                    _snakeInfo[r+1][c+1][1] = _snakeInfo[r+1][c][1];         // Move Bottom-Left to Bottom-Right
                    _snakeInfo[r+1][c][1] = tempY;                           // Move saved Top-Left to Bottom-Left*/
                    if (j == _initialLength-3){
                        _calcFlag = 1;
                    }  
                }
                else if (_yVel == 0 && _snakeInfo[1][j][2] != _snakeInfo[1][j+1][2] && _snakeInfo[0][j+1][0] != _snakeInfo[1][j][0]){
                    r = 0;
                    c = j-1;
                    // 1. Handle X Coordinates [0]
                    int tempX = _snakeInfo[r][c][0];                         // Save Top-Left
                    _snakeInfo[r][c][0] = _snakeInfo[r][c+1][0];             // Move Top-Right to Top-Left
                    _snakeInfo[r][c+1][0] = _snakeInfo[r+1][c+1][0];         // Move Bottom-Right to Top-Right
                    _snakeInfo[r+1][c+1][0] = _snakeInfo[r+1][c][0];         // Move Bottom-Left to Bottom-Right
                    _snakeInfo[r+1][c][0] = tempX;                           // Move saved Top-Left to Bottom-Left

                    // 2. Handle Y Coordinates [1]
                    int tempY = _snakeInfo[r][c][1];                         // Save Top-Left
                    _snakeInfo[r][c][1] = _snakeInfo[r][c+1][1];             // Move Top-Right to Top-Left
                    _snakeInfo[r][c+1][1] = _snakeInfo[r+1][c+1][1];         // Move Bottom-Right to Top-Right
                    _snakeInfo[r+1][c+1][1] = _snakeInfo[r+1][c][1];         // Move Bottom-Left to Bottom-Right
                    _snakeInfo[r+1][c][1] = tempY;                           // Move saved Top-Left to Bottom-Left*/
                    if (j == _initialLength-3){
                        _calcFlag = 1;
                    }  
                }
            }
        }
    }
    if (_calcFlag == 1){
        _t = _t + 1;
    }
    if (_t == 2){
        r = 0;
        c = _initialLength-2;
        if (_cw == 0){
            int tempX = _snakeInfo[r][c][0];                         // Save Top-Left
            _snakeInfo[r][c][0] = _snakeInfo[r+1][c][0];             // Move Bottom-Left to Top-Left
            _snakeInfo[r+1][c][0] = _snakeInfo[r+1][c+1][0];         // Move Bottom-Right to Bottom-Left
            _snakeInfo[r+1][c+1][0] = _snakeInfo[r][c+1][0];         // Move Top-Right to Bottom-Right
            _snakeInfo[r][c+1][0] = tempX;                           // Move saved Top-Left to Top-Right

            // 2. Handle Y Coordinates [1]
            int tempY = _snakeInfo[r][c][1];                         // Save Top-Left
            _snakeInfo[r][c][1] = _snakeInfo[r+1][c][1];             // Move Bottom-Left to Top-Left
            _snakeInfo[r+1][c][1] = _snakeInfo[r+1][c+1][1];         // Move Bottom-Right to Bottom-Left
            _snakeInfo[r+1][c+1][1] = _snakeInfo[r][c+1][1];         // Move Top-Right to Bottom-Right
            _snakeInfo[r][c+1][1] = tempY;
        }
        else{
            // 1. Handle X Coordinates [0]
            int tempX = _snakeInfo[r][c][0];                         // Save Top-Left
            _snakeInfo[r][c][0] = _snakeInfo[r][c+1][0];             // Move Top-Right to Top-Left
            _snakeInfo[r][c+1][0] = _snakeInfo[r+1][c+1][0];         // Move Bottom-Right to Top-Right
            _snakeInfo[r+1][c+1][0] = _snakeInfo[r+1][c][0];         // Move Bottom-Left to Bottom-Right
            _snakeInfo[r+1][c][0] = tempX;                           // Move saved Top-Left to Bottom-Left

            // 2. Handle Y Coordinates [1]
            int tempY = _snakeInfo[r][c][1];                         // Save Top-Left
            _snakeInfo[r][c][1] = _snakeInfo[r][c+1][1];             // Move Top-Right to Top-Left
            _snakeInfo[r][c+1][1] = _snakeInfo[r+1][c+1][1];         // Move Bottom-Right to Top-Right
            _snakeInfo[r+1][c+1][1] = _snakeInfo[r+1][c][1];         // Move Bottom-Left to Bottom-Right
            _snakeInfo[r+1][c][1] = tempY;                           // Move saved Top-Left to Bottom-Left*/
        }
        _calcFlag = 0;
        _t = 0;
    }
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _initialLength; j++){
            /*if (i == 0 && j % 2 == 0){
                display.drawPixel(_snakeInfo[i][j][0], _snakeInfo[i][j][1], SSD1306_BLACK);
            }
            else{
                display.drawPixel(_snakeInfo[i][j][0], _snakeInfo[i][j][1], SSD1306_WHITE);
            }*/
           display.drawPixel(_snakeInfo[i][j][0], _snakeInfo[i][j][1], SSD1306_WHITE);
        }
    }
    display.display();
}

void SnakeClass::changeDirection(int direction){
    if ((_xVel == -1 && direction == 2)|| (_xVel == 1 && direction == 4)){
        _cw = 1;
    }
    else if ((_xVel == 1 && direction == 2) || (_xVel == -1 && direction == 4)){
        _cw = 0;
    }
    else if ((_yVel == 1 && direction == 1) || (_yVel == -1 && direction == 3)){
        _cw = 1;
    }
    else if ((_yVel == -1 && direction == 1) || (_yVel == 1 && direction == 3)){
        _cw = 0;
    }
    if (direction == 1){
        _xVel = -1;
        _yVel = 0;
    }
    else if (direction == 2){
        _xVel = 0;
        _yVel = -1;
    }
    else if (direction == 3){
        _xVel = 1;
        _yVel = 0;
    }
    else{
        _xVel = 0;
        _yVel = 1;
    }
    _wayPointInfo[0] = _snakeInfo[0][0][0]; //1st xPos
    _wayPointInfo[1] = _snakeInfo[0][0][1]; //1st yPos
    _wayPointInfo[2] = _xVel;
    _wayPointInfo[3] = _yVel;
    delay(100);
}