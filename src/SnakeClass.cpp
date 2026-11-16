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
    _cw = 0;
    display.setCursor(display.width() / 2 - _initialLength, display.height() / 2);
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _MAX_COLUMNS; j++){
            _snakeInfo[i][j][0] = 0;
            _snakeInfo[i][j][1] = 0;
            _snakeInfo[i][j][2] = 0;
            _snakeInfo[i][j][3] = 0;
            _snakeInfo[i][j][4] = -1;
        }
    }
    for (int i = 0; i < 2; i++){
        for (int j = 0; j<_initialLength; j++){
            _snakeInfo[i][j][0] = display.width()/2+j;
            _snakeInfo[i][j][1] = display.height()/2+i;
            _snakeInfo[i][j][2] = _xVel;
            _snakeInfo[i][j][3] = _yVel;
            display.drawPixel(_snakeInfo[i][j][0],_snakeInfo[i][j][1],SSD1306_WHITE);
        }
    }
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < _MAX_COLUMNS; j++){
            _wayPointInfo[i][j] = 0;
        }
    }
    display.display();
}



void SnakeClass::moveSnake(Adafruit_SSD1306 &display){
    // Erase current snake
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _initialLength; j++){
            display.drawPixel(_snakeInfo[i][j][0],_snakeInfo[i][j][1],SSD1306_BLACK);
        }
    }

    //Reassign velocities based on new waypoints
    assignVelocities();

    //Recalculate positions based on new velocities
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _initialLength; j++){
            _snakeInfo[i][j][0] += _snakeInfo[i][j][2];
            _snakeInfo[i][j][1] += _snakeInfo[i][j][3];
        }
    }
    
    //Rotate segments to align with new snake direction
    rotateSegments();

    //Display new snake
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _initialLength; j++){
            display.drawPixel(_snakeInfo[i][j][0], _snakeInfo[i][j][1], SSD1306_WHITE);
        }
    }
    display.display();
}



void SnakeClass::assignVelocities(){
    int wpInd = 0;
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _initialLength; j++){
            if (j % 2 != 0){
                wpInd = _snakeInfo[i][j][4];
                if (wpInd >= 0){
                    if (abs(_wayPointInfo[3][wpInd])>0){
                        if (_snakeInfo[i][j][0] == _wayPointInfo[0][wpInd]){
                            _snakeInfo[i][j-1][2] = _wayPointInfo[2][wpInd];
                            _snakeInfo[i][j-1][3] = _wayPointInfo[3][wpInd];
                            _snakeInfo[i][j][2] = _wayPointInfo[2][wpInd];
                            _snakeInfo[i][j][3] = _wayPointInfo[3][wpInd];
                        }
                    }
                    else{
                        if (_snakeInfo[i][j][1] == _wayPointInfo[1][wpInd]){
                            _snakeInfo[i][j-1][2] = _wayPointInfo[2][wpInd];
                            _snakeInfo[i][j-1][3] = _wayPointInfo[3][wpInd];
                            _snakeInfo[i][j][2] = _wayPointInfo[2][wpInd];
                            _snakeInfo[i][j][3] = _wayPointInfo[3][wpInd];    
                        }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
                    }
                }
            }
        }
    }
}

void SnakeClass::rotateSegments(){
    int xInd = 0;
    int yInd = 0;
    int XArr[2*_MAX_COLUMNS];
    int YArr[2*_MAX_COLUMNS];
    int k = 0;
    int trackInd = 0;
    int wpInd = 0;

    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _initialLength; j++){
            XArr[k] = _snakeInfo[i][j][0];
            YArr[k] = _snakeInfo[i][j][1];
            k++;
        }
    }

     if (_calcLastSeg){
        _counter = _counter+1;
        if (_counter == 2){
            wpInd = _snakeInfo[0][_initialLength-2][4];
            if (_wayPointInfo[4][wpInd] == 0){
                rotateCounterClockwise(_initialLength-2);
            }
            else{
                rotateClockwise(_initialLength-2);
            }

            for (int i = 0; i < 4; i++){
                _wayPointInfo[i][wpInd] = 0;
            }
            decrementSnake(_initialLength-2);

            _counter = 0;
            _calcLastSeg = 0;
        }
    }

    for (int i = 0; i < 2*_initialLength-1; i++){
        for (int j = i+1; j < 2*_initialLength; j++){
            if (XArr[i]==XArr[j] && YArr[i]==YArr[j]){
                trackInd = i;
                if (trackInd > _initialLength){
                    xInd = 1;
                    yInd = trackInd - _initialLength;
                }
                else{
                    xInd = 0;
                    yInd = trackInd;
                }
                if (yInd % 2 == 0){
                    wpInd = _snakeInfo[0][yInd-2][4];
                }
                else{
                    wpInd = _snakeInfo[0][yInd-1][4];
                }
                if (_wayPointInfo[4][wpInd] == 0){
                    rotateCounterClockwise(yInd-1);
                    decrementSnake(yInd-1);
                    if (yInd == _initialLength-3){
                        _calcLastSeg = 1;
                    }
                }
                else{
                    rotateClockwise(yInd-2);
                    decrementSnake(yInd-2);
                    if (yInd == _initialLength-2){
                        _calcLastSeg = 1;
                    }
                }
            }
        }
    }
}

void SnakeClass::changeDirection(int direction){

    if (_dirCounter == 2){
        int prevXVel = _xVel;
        int prevYVel = _yVel;

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

        if (prevXVel == 1){
            if (_yVel == 1){
                _cw = 1;
            }
            else{
                _cw = 0;
            }
        }
        else if (prevXVel == -1){
            if (_yVel == -1){
                _cw = 1;
            }
            else{
                _cw = 0;
            }
        }
        else if (prevYVel == 1){
            if (_xVel == -1){
                _cw = 1;
            }
            else{
                _cw = 0;
            }
        }
        else if (prevYVel == -1){
            if (_xVel == 1){
                _cw = 1;
            }
            else{
                _cw = 0;
            }
        }

        //Insert new waypoint
        for (int r = 0; r < 5; r++) {
            for (int c = _MAX_COLUMNS - 1; c > 0; c--) {
                _wayPointInfo[r][c] = _wayPointInfo[r][c - 1];
            }
        }

        _wayPointInfo[0][0] = _snakeInfo[0][0][0]; //1st xPos
        _wayPointInfo[1][0] = _snakeInfo[0][0][1]; //1st yPos
        _wayPointInfo[2][0] = _xVel;
        _wayPointInfo[3][0] = _yVel;
        _wayPointInfo[4][0] = _cw;

        for (int i = 0; i < 2; i++){
            for (int j = 0; j < _initialLength; j++){
                _snakeInfo[i][j][4]++;
            }
        }

        _dirCounter = 0;
        delay(200);
    }
    else{
        _dirCounter++;
    }
}

void SnakeClass::rotateClockwise(int c){
    int r = 0;
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

void SnakeClass::rotateCounterClockwise(int c){
    int r = 0;
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

void SnakeClass::decrementSnake(int c){
    _snakeInfo[0][c][4]--;
    _snakeInfo[0][c+1][4]--;
    _snakeInfo[1][c][4]--;
    _snakeInfo[1][c+1][4]--;
}

void SnakeClass::spawnFood(Adafruit_SSD1306 &display){

    int XArr[2*_MAX_COLUMNS];
    int YArr[2*_MAX_COLUMNS];
    int k  = 0;
    int maxX;
    int maxY;
    int minX;
    int minY;
    int xPos;
    int yPos;
    int yRand;
    int xRand;

    if (_flag == 1){
        for (int i = 0; i < 2; i++){
            for (int j = 0; j < 2; j++){
                display.drawPixel(_foodPos[i][j][0],_foodPos[i][j][1],SSD1306_BLACK);
            }
        }

        for (int i = 0; i < 2; i++){
            for (int j = 0; j < _initialLength; j++){
                XArr[k] = _snakeInfo[i][j][0];
                YArr[k] = _snakeInfo[i][j][1];
                k++;
            }
        }

        maxX = XArr[0];
        minX = XArr[0];

        for (int i = 1; i < 2*_initialLength; i++){
            if (XArr[i]>maxX){
                maxX = XArr[i];
            }
            else if (XArr[i]<minX){
                minX = XArr[i];
            }
        }

        maxY = YArr[0];
        minY = YArr[0];

        for (int i = 1; i < 2*_initialLength; i++){
            if (YArr[i]>maxY){
                maxY = YArr[i];
            }
            else if (YArr[i]<minY){
                minY = YArr[i];
            }
        }

        maxX = maxX + 6;
        minX = minX - 6;
        maxY = maxY + 6;
        minY = minY - 6;

        if (minY <= 5){
            minY = 5;
        }
        if (maxY >= display.height()-3){
            maxY = display.height()-5;
        }

        if (minX <= 5){
            minX = 5;
        }
        if (maxX >= display.width()-3){
            maxX = display.width()-5;
        }

        xRand = random(0,2);
        xPos = xRand*random(3,minX) + (1-xRand)*random(maxX,display.width()-3);
        if (xPos > 125){
            xPos = 125;
        }
        else if (xPos < 3){
            xPos = 3;
        }

        yRand = random(0,2);
        yPos = yRand*random(3,minY) + (1-yRand)*random(maxY,display.height()-3);
        if (yPos > 61){
            yPos = 61;
        }
        else if (yPos < 3){
            yPos = 3;
        }

        _foodPos[0][0][0] = xPos;
        _foodPos[0][1][0] = xPos-1;
        _foodPos[1][0][0] = xPos;
        _foodPos[1][1][0] = xPos-1;

        _foodPos[0][0][1] = yPos;
        _foodPos[0][1][1] = yPos;
        _foodPos[1][0][1] = yPos-1;
        _foodPos[1][1][1] = yPos-1;

        for (int i = 0; i < 2; i++){
            for (int j = 0; j < 2; j++){
                display.drawPixel(_foodPos[i][j][0],_foodPos[i][j][1],SSD1306_WHITE);
            }
        }
        _flag = 0;
    }

}

void SnakeClass::checkFood(Adafruit_SSD1306 &display){
    _eatFood = 0;
    for (int k = 0; k < 2; k++){
        for (int i = 0; i < 2; i++){
            for (int j = 0; j < 2; j++){
                if (_foodPos[i][j][0] == _snakeInfo[k][0][0] && _foodPos[i][j][1] == _snakeInfo[k][0][1]){
                    _eatFood = 1;
                    break;
                }
            }
        }
    }
    if (_eatFood == 1){
        if (_calcLastSeg == 0 && _initialLength < _MAX_COLUMNS-4){
            increaseLength();
        }
        _flag = 1;
    }
}

void SnakeClass::increaseLength(){

    int desSeg = 2;

    if (_snakeInfo[0][_initialLength-1][2] < 0){
        _snakeInfo[0][_initialLength][0] = _snakeInfo[0][_initialLength-1][0]+1;
        _snakeInfo[0][_initialLength+1][0] = _snakeInfo[0][_initialLength-1][0]+2;
        _snakeInfo[1][_initialLength][0] = _snakeInfo[1][_initialLength-1][0]+1;
        _snakeInfo[1][_initialLength+1][0] = _snakeInfo[1][_initialLength-1][0]+2;

        _snakeInfo[0][_initialLength][1] = _snakeInfo[0][_initialLength-1][1];
        _snakeInfo[0][_initialLength+1][1] = _snakeInfo[0][_initialLength-1][1];
        _snakeInfo[1][_initialLength][1] = _snakeInfo[1][_initialLength-1][1];
        _snakeInfo[1][_initialLength+1][1] = _snakeInfo[1][_initialLength-1][1];
    }
    else if (_snakeInfo[0][_initialLength-1][2] > 0){
        _snakeInfo[0][_initialLength][0] = _snakeInfo[0][_initialLength-1][0]-1;
        _snakeInfo[0][_initialLength+1][0] = _snakeInfo[0][_initialLength-1][0]-2;
        _snakeInfo[1][_initialLength][0] = _snakeInfo[1][_initialLength-1][0]-1;
        _snakeInfo[1][_initialLength+1][0] = _snakeInfo[1][_initialLength-1][0]-2;

        _snakeInfo[0][_initialLength][1] = _snakeInfo[0][_initialLength-1][1];
        _snakeInfo[0][_initialLength+1][1] = _snakeInfo[0][_initialLength-1][1];
        _snakeInfo[1][_initialLength][1] = _snakeInfo[1][_initialLength-1][1];
        _snakeInfo[1][_initialLength+1][1] = _snakeInfo[1][_initialLength-1][1];
    }
    else if (_snakeInfo[0][_initialLength-1][3] < 0){
        _snakeInfo[0][_initialLength][1] = _snakeInfo[0][_initialLength-1][1]+1;
        _snakeInfo[0][_initialLength+1][1] = _snakeInfo[0][_initialLength-1][1]+2;
        _snakeInfo[1][_initialLength][1] = _snakeInfo[1][_initialLength-1][1]+1;
        _snakeInfo[1][_initialLength+1][1] = _snakeInfo[1][_initialLength-1][1]+2;

        _snakeInfo[0][_initialLength][0] = _snakeInfo[0][_initialLength-1][0];
        _snakeInfo[0][_initialLength+1][0] = _snakeInfo[0][_initialLength-1][0];
        _snakeInfo[1][_initialLength][0] = _snakeInfo[1][_initialLength-1][0];
        _snakeInfo[1][_initialLength+1][0] = _snakeInfo[1][_initialLength-1][0];
    }
    else{
        _snakeInfo[0][_initialLength][1] = _snakeInfo[0][_initialLength-1][1]-1;
        _snakeInfo[0][_initialLength+1][1] = _snakeInfo[0][_initialLength-1][1]-2;
        _snakeInfo[1][_initialLength][1] = _snakeInfo[1][_initialLength-1][1]-1;
        _snakeInfo[1][_initialLength+1][1] = _snakeInfo[1][_initialLength-1][1]-2;

        _snakeInfo[0][_initialLength][0] = _snakeInfo[0][_initialLength-1][0];
        _snakeInfo[0][_initialLength+1][0] = _snakeInfo[0][_initialLength-1][0];
        _snakeInfo[1][_initialLength][0] = _snakeInfo[1][_initialLength-1][0];
        _snakeInfo[1][_initialLength+1][0] = _snakeInfo[1][_initialLength-1][0];
    }

    for (int i = 0; i < 2; i++){
        for (int j = 2; j < 5; j++){
            _snakeInfo[i][_initialLength][j] = _snakeInfo[i][_initialLength-desSeg][j];
            _snakeInfo[i][_initialLength+1][j] = _snakeInfo[i][_initialLength-desSeg][j];
        }
    }

    _initialLength = _initialLength + 2;
}