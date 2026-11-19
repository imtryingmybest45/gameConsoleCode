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
    display.setCursor(display.width() / 2 - _currentLength, display.height() / 2);
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
        for (int j = 0; j<_currentLength; j++){
            _snakeInfo[i][j][0] = display.width()/2+j;
            _snakeInfo[i][j][1] = display.height()/2+i;
            _snakeInfo[i][j][2] = _xVel;
            _snakeInfo[i][j][3] = _yVel;
            display.drawPixel(_snakeInfo[i][j][0],_snakeInfo[i][j][1],SSD1306_WHITE);
        }
    }
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < _MAX_WP_COLUMNS; j++){
            _wayPointInfo[i][j] = 0;
        }
    }
    display.display();

}



void SnakeClass::moveSnake(Adafruit_SSD1306 &display){

    // Erase current snake
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _currentLength; j++){
            display.drawPixel(_snakeInfo[i][j][0],_snakeInfo[i][j][1],SSD1306_BLACK);
        }
    }

    //Reassign velocities based on new waypoints
    assignVelocities();

    //Recalculate positions based on new velocities
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _currentLength; j++){
            _snakeInfo[i][j][0] += _snakeInfo[i][j][2];
            _snakeInfo[i][j][1] += _snakeInfo[i][j][3];
        }
    }
    
    //Rotate segments to align with new snake direction
    rotateSegments();

    //Display new snake
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _currentLength; j++){
            display.drawPixel(_snakeInfo[i][j][0], _snakeInfo[i][j][1], SSD1306_WHITE);
        }
    }

    checkCollision(display);

    if (_gameOver == 0){
        display.display();
    }
}



void SnakeClass::assignVelocities(){
    int wpInd = 0;
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _currentLength; j++){
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
    int yInd = 0;
    /*int XArr[2*_MAX_COLUMNS];
    int YArr[2*_MAX_COLUMNS];*/
    int XArr[2*(_currentLength+1)];
    int YArr[2*(_currentLength+1)];
    int k = 0;
    int trackInd = 0;
    int wpInd = 0;
    int tPos = 0;

    for (int i = 0; i < 2; i++){
        for (int j = 0; j < _currentLength; j++){
            XArr[k] = _snakeInfo[i][j][0];
            YArr[k] = _snakeInfo[i][j][1];
            k++;
        }
    }

    /*if (_xVel == 0){
        tPos = _wayPointInfo[0][0];
        Serial.println(tPos);
        for (int i = 0; i < _currentLength; i++){
            Serial.println(_snakeInfo[0][i][0]);
            Serial.println(i);
            if (_snakeInfo[0][i][0] == tPos){
                break;
            }
        }
    }*/

     if (_calcLastSeg){
        _counter = _counter+1;
        if (_counter == 2){
            wpInd = _snakeInfo[0][_currentLength-2][4];
            if (_wayPointInfo[4][wpInd] == 0){
                rotateCounterClockwise(_currentLength-2);
            }
            else{
                rotateClockwise(_currentLength-2);
            }

            for (int i = 0; i < 4; i++){
                _wayPointInfo[i][wpInd] = 0;
            }
            decrementSnake(_currentLength-2);

            _counter = 0;
            _calcLastSeg = 0;
        }
    }

   for (int i = 0; i < 2*_currentLength-1; i++){
        for (int j = i+1; j < 2*_currentLength; j++){
            if (XArr[i]==XArr[j] && YArr[i]==YArr[j]){
                trackInd = i;
                if (trackInd > _currentLength){
                    yInd = trackInd - _currentLength;
                }
                else{
                    yInd = trackInd;
                }
                if (yInd % 2 == 0){
                    if (yInd < 2){
                        yInd = 2;
                    }
                    wpInd = _snakeInfo[0][yInd-2][4];
                }
                else{
                    if (yInd < 1){
                        yInd = 1;
                    }
                    wpInd = _snakeInfo[0][yInd-1][4];
                }
                if (_wayPointInfo[4][wpInd] == 0){
                    rotateCounterClockwise(yInd-1);
                    decrementSnake(yInd-1);
                    if (yInd == _currentLength-3){
                        _calcLastSeg = 1;
                    }
                }
                else{
                    rotateClockwise(yInd-2);
                    decrementSnake(yInd-2);
                    if (yInd == _currentLength-2){
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
            for (int c = _MAX_WP_COLUMNS - 1; c > 0; c--) {
                _wayPointInfo[r][c] = _wayPointInfo[r][c - 1];
            }
        }

        _wayPointInfo[0][0] = _snakeInfo[0][0][0]; //1st xPos
        _wayPointInfo[1][0] = _snakeInfo[0][0][1]; //1st yPos
        _wayPointInfo[2][0] = _xVel;
        _wayPointInfo[3][0] = _yVel;
        _wayPointInfo[4][0] = _cw;

        for (int i = 0; i < 2; i++){
            for (int j = 0; j < _currentLength; j++){
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

    /*int XArr[2*_MAX_COLUMNS];
    int YArr[2*_MAX_COLUMNS];*/
    int XArr[2*(_currentLength+1)];
    int YArr[2*(_currentLength+1)];
    int k  = 0;
    int maxX; int maxY;
    int minX; int minY;
    int xPos; int yPos;

    if (_spawnFlag == 1){
        for (int i = 0; i < 2; i++){
            for (int j = 0; j < 2; j++){
                display.drawPixel(_foodPos[i][j][0],_foodPos[i][j][1],SSD1306_BLACK);
            }
        }

        for (int i = 0; i < 2; i++){
            for (int j = 0; j < _currentLength; j++){
                XArr[k] = _snakeInfo[i][j][0];
                YArr[k] = _snakeInfo[i][j][1];
                k++;
            }
        }

        xPos = random(3,display.width()-2);
        yPos = random(3,display.height()-2);
        int flag = 1;

        minX = min(XArr[_currentLength-1],XArr[_currentLength]);
        maxX = max(XArr[_currentLength-1],XArr[_currentLength]);
        minY = min(YArr[_currentLength-1],YArr[_currentLength]);
        maxY = max(YArr[_currentLength-1],YArr[_currentLength]);

        while (flag == 1){
            if (xPos >= minX && xPos <= maxX){
                xPos = random(3,display.width()-3);
                break; 
            }
            if (yPos >= minY && yPos <= maxY){
                yPos = random(3,display.height()-3);
                break;
            }
            else{
                flag = 0;
            }
        }

        for (int i = 0; i < 2; i++){
            for (int j = 0; j < 2; j++){
                _foodPos[i][j][0] = xPos-j;
                _foodPos[i][j][1] = yPos-i;
            }
        }

        for (int i = 0; i < 2; i++){
            for (int j = 0; j < 2; j++){
                display.drawPixel(_foodPos[i][j][0],_foodPos[i][j][1],SSD1306_WHITE);
            }
        }
        _spawnFlag = 0;
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
        if (_calcLastSeg == 0 && _currentLength < _MAX_COLUMNS-4){
            increaseLength();
        }
        _score++;
        _spawnFlag = 1;
    }
}

void SnakeClass::increaseLength(){

    int xVel = _snakeInfo[0][_currentLength-1][2];
    int yVel = _snakeInfo[0][_currentLength-1][3];
    int sgnX;
    int sgnY;

    if (xVel > 0) {
        sgnX = 1;
    } else if (xVel < 0) {
        sgnX = -1;
    } else {
        sgnX = 0;
    }

    if (yVel > 0) {
        sgnY = 1;
    } else if (yVel < 0) {
        sgnY = -1;
    } else {
        sgnY = 0;
    }

    if (abs(xVel) > 0){
        for (int i = 0; i < 2; i++){
            _snakeInfo[i][_currentLength][0] = _snakeInfo[i][_currentLength-1][0]-(sgnX*1);
            _snakeInfo[i][_currentLength+1][0] = _snakeInfo[i][_currentLength-1][0]-(sgnX*2);
            _snakeInfo[i][_currentLength][1] = _snakeInfo[i][_currentLength-1][1];
            _snakeInfo[i][_currentLength+1][1] = _snakeInfo[i][_currentLength-1][1];
        }
    }
    else if (abs(yVel) > 0){
        for (int i = 0; i < 2; i++){
            _snakeInfo[i][_currentLength][1] = _snakeInfo[i][_currentLength-1][1]-(sgnY*1);
            _snakeInfo[i][_currentLength+1][1] = _snakeInfo[i][_currentLength-1][1]-(sgnY*2);
            _snakeInfo[i][_currentLength][0] = _snakeInfo[i][_currentLength-1][0];
            _snakeInfo[i][_currentLength+1][0] = _snakeInfo[i][_currentLength-1][0];
        }
    }

    for (int i = 0; i < 2; i++){
        for (int j = 2; j < 5; j++){
            _snakeInfo[i][_currentLength][j] = _snakeInfo[i][_currentLength-2][j];
            _snakeInfo[i][_currentLength+1][j] = _snakeInfo[i][_currentLength-2][j];
        }
    }

    _currentLength = _currentLength + 2;
}

void SnakeClass::checkCollision(Adafruit_SSD1306 &display){
    int XArr[2*_MAX_COLUMNS];
    int YArr[2*_MAX_COLUMNS];
    int k = 0;

    for (int i = 0; i < 2; i++){
        for (int j = 1; j < _currentLength; j++){
            XArr[k] = _snakeInfo[i][j][0];
            YArr[k] = _snakeInfo[i][j][1];
            k++;
        }
    }

    for (int i = 0; i < 2; i++){
        for (int j = i; j < 2*(_currentLength-1); j++){
            if (_snakeInfo[i][0][0] == XArr[j] && _snakeInfo[i][0][1] == YArr[j]){
                _gameOver = 1;
                break;
            }
        }
    }

    for (int i = 0; i < 2; i++){
        if (_snakeInfo[i][0][0] > display.width()+1 || _snakeInfo[i][0][0] < -1){
            _gameOver = 1;
            break;
        }
        if (_snakeInfo[i][0][1] > display.height()+1 || _snakeInfo[i][0][1] < -1){
            _gameOver = 1;
            break;
        }
    }
}

void SnakeClass::displayScore(Adafruit_SSD1306 &display){
    
}

void SnakeClass::restartGame(Adafruit_SSD1306 &display){
    _currentLength = _initialLength;
    _calcLastSeg = 0;
    _counter = 0;
    _spawnFlag = 1;
    _eatFood = 0;
    _dirCounter = 2;
    _gameOver = 0;
    _score = 0;
    _scoreTracker = 0;
}

void SnakeClass::findNumWayPoints(){
    for (int i = 0; i < _MAX_WP_COLUMNS; i++){
        if (_wayPointInfo[2][i] == _wayPointInfo[3][i]){
            _numWayPoints = i;
            break;
        }
    }
}