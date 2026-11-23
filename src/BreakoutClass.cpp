#include "BreakoutClass.h" // Bind the implementation to your header file
#include "MenuClass.h" // Bind the implementation to your header file
#include "TextFunctions.h" // Bind the implementation to your header file
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BallClass.h>

// Constructor implementation
BreakoutClass::BreakoutClass(Adafruit_SSD1306& display): _display(display) {     // Store the provided pin number in the private variable
    _display = display;
    _yPos = display.height()-3;
    if (_paddleWidth % 2 != 0){
        _paddleWidth = _paddleWidth + 1;
    }
    _xRangeMin = display.width()/2 - _paddleWidth/2;
    _xRangeMax = display.width()/2 + _paddleWidth/2;
    _activeCols = (_paddleWidth > _MAX_COLUMNS) ? _MAX_COLUMNS : _paddleWidth;
    _paddlePos[_activeCols];
    int randNum = random(1,3);
    if (randNum == 1){
        _ballXVel = 1;
    }
    else{
        _ballXVel = -1;
    }
    _ballYVel = -1;
}

void BreakoutClass::gameStartup(Adafruit_SSD1306& display){
    TextFunctions txtFuncs(100, 5, 32, display);
    display.clearDisplay();
    display.setCursor(0, 0);
    txtFuncs.drawCenteredText("Are you aready to play breakout?", display);
    delay(1500);
    display.clearDisplay();
}

void BreakoutClass::createPaddle(Adafruit_SSD1306 &display) {
    drawPaddle(display, 0, 1);
}

void BreakoutClass::createBall(Adafruit_SSD1306 &display){
    _ballXPos = _xRangeMin + (_xRangeMax-_xRangeMin)/2;
    _ballYPos = _yPos-1;
    _prevBallXPos = _ballXPos;
    _prevBallYPos = _ballYPos;
    for (int x = _ballXPos-1; x <= _ballXPos; x++){
        for (int y = _ballYPos-1; y <= _ballYPos; y++){
            display.drawPixel(x, y, SSD1306_WHITE);
        }
    }
    display.display();
}

void BreakoutClass::drawBall(Adafruit_SSD1306 &display, int color){
    if (color == 1){
        for (int x = _ballXPos-1; x <= _ballXPos; x++){
            for (int y = _ballYPos-1; y <= _ballYPos; y++){
                display.drawPixel(x, y, SSD1306_WHITE);
            }
        }
    }
    else{
       for (int x = _ballXPos-1; x <= _ballXPos; x++){
            for (int y = _ballYPos-1; y <= _ballYPos; y++){
                display.drawPixel(x, y, SSD1306_BLACK);
            }
        }
    }
}

void BreakoutClass::moveBall(Adafruit_SSD1306 &display){
    drawBall(display, 0);
    _ballXPos = _prevBallXPos + _ballXVel;
    _ballYPos = _prevBallYPos + _ballYVel;
    drawBall(display, 1);
    _prevBallXPos = _ballXPos;
    _prevBallYPos = _ballYPos;

    for (int y = _yPos; y <= _yPos + 1; y++){
        for (int x = _xRangeMin; x < _xRangeMax; x++) {
            if (display.getPixel(x,y) == 0){
                display.drawPixel(x,y,SSD1306_WHITE);
            }
        }
    }

    for (int i = 0; i < _numRows; i++){
        for (int j = 0; j < _numCols; j++){
            if (_blockArr[i][j][0] != 1000 || _blockArr[i][j][1] != 1000){
                _spawnNewBoard = 0;
                break;
            }
        }
    }

    if (_spawnNewBoard == 1){
        if (_ballYPos > display.height()/2){
            createBoard(display);
            _score = _score + 10*(_numRows*_numCols);
        }
    }

    _spawnNewBoard = 1;
    display.display();
}

void BreakoutClass::movePaddleRight(Adafruit_SSD1306 &display, int xinc) {
    if (_paddlePos[_paddleWidth-1] <= display.width()-2){
        drawPaddle(display, 0, 0);
        drawPaddle(display, xinc, 1);
    }
    if (_launchBall == 0){
        for (int x = _ballXPos-1; x <= _ballXPos; x++){
            for (int y = _ballYPos-1; y <= _ballYPos; y++){
                display.drawPixel(x, y, SSD1306_BLACK);
            }
        }
        _ballXPos = _paddlePos[_paddleWidth/2];
        for (int x = _ballXPos-1; x <= _ballXPos; x++){
            for (int y = _ballYPos-1; y <= _ballYPos; y++){
                display.drawPixel(x, y, SSD1306_WHITE);
            }
        } 
        _prevBallXPos = _ballXPos;
        _prevBallYPos = _ballYPos;  
    }
    if (_launchBall == 0){
        display.display();
    }
}

void BreakoutClass::movePaddleLeft(Adafruit_SSD1306 &display, int xinc) {
    if (_paddlePos[0] >= 2){
        drawPaddle(display, 0, 0);
        drawPaddle(display, -xinc, 1);
    }
    if (_launchBall == 0){
        for (int x = _ballXPos-1; x <= _ballXPos; x++){
            for (int y = _ballYPos-1; y <= _ballYPos; y++){
                display.drawPixel(x, y, SSD1306_BLACK);
            }
        }
        _ballXPos = _paddlePos[_paddleWidth/2];
        for (int x = _ballXPos-1; x <= _ballXPos; x++){
            for (int y = _ballYPos-1; y <= _ballYPos; y++){
                display.drawPixel(x, y, SSD1306_WHITE);
            }
        } 
        _prevBallXPos = _ballXPos;
        _prevBallYPos = _ballYPos;  
    }
    if (_launchBall == 0){
        display.display();
    }
}

void BreakoutClass::drawPaddle(Adafruit_SSD1306 &display, int xInc, int color){
    _xRangeMin = _xRangeMin + xInc;
    _xRangeMax = _xRangeMax + xInc;
    int xInd = 0;
    for (int y = _yPos; y <= _yPos + 1; y++){
        for (int x = _xRangeMin; x < _xRangeMax; x++) {
            if (color == 1){
                display.drawPixel(x, y, SSD1306_WHITE);
                _paddlePos[xInd] = x;
            }
            else{
                display.drawPixel(x, y, SSD1306_BLACK);
            }
            xInd++;
        }
        xInd = 0;
    }
}

void BreakoutClass::createBoard(Adafruit_SSD1306 &display){
    int xPos = _sideMargin+1;
    int yPos = _topMargin+1;
    int startXPos = xPos;
    for (int i = 0; i < _numRows; i++){
        for (int j = 0; j < _numCols; j++){
            _blockArr[i][j][0] = xPos;
            _blockArr[i][j][1] = yPos;
            drawBlock(display,xPos,yPos,1);
            xPos = xPos + _blockWidth + 1;
        }
        yPos = yPos + _blockHeight+1;
        xPos = startXPos;
    }
}

void BreakoutClass::drawBlock(Adafruit_SSD1306 &display, int xPos, int yPos, int color){
    if (color == 1){
        for (int i = 0; i < _blockWidth-1; i++){
            for (int j = 0; j < _blockHeight-1; j++){
                display.drawPixel(xPos+i,yPos+j,SSD1306_WHITE);
            }
        }
    }
    else{
        for (int i = 0; i < _blockWidth-1; i++){
            for (int j = 0; j < _blockHeight-1; j++){
                display.drawPixel(xPos+i,yPos+j,SSD1306_BLACK);
            }
        }
    }
}

void BreakoutClass::checkCollisions(Adafruit_SSD1306 &display){
    if (_ballXPos == 3 || _ballXPos + 1 == 3){
        _ballXVel = 1;
    }
    else if (_ballXPos == display.width()-2 || _ballXPos+1 == display.width()-2){
        _ballXVel = -1;
    }
    if (_ballYPos == 3 || _ballYPos-1 == 3){
        _ballYVel = 2;
    }

    checkPaddleCollision();
    checkBlockCollision(display);

    if (_ballYPos > display.height()){
        resetLaunch(display);
        _numLives--;
    }
}

void BreakoutClass::checkPaddleCollision(){
    int delt;
    int left;
    int inc;
    if (_ballYPos <= _yPos+1 && _ballYPos >= _yPos){
        delt = floor(_paddleWidth/5);
        left = _paddleWidth - 5*delt;
        inc = ceil(left/2);
        if (_ballXPos >= _paddlePos[0]-1 && _ballXPos <= _paddlePos[delt+inc-1]){
            _ballXVel = -1;
            _ballYVel = -2;
        }
        else if (_ballXPos >= _paddlePos[delt+inc] && _ballXPos <= _paddlePos[2*delt+inc-1]){
            _ballXVel = -1;
            _ballYVel = -2;
        }
        else if (_ballXPos >= _paddlePos[2*delt+inc] && _ballXPos <= _paddlePos[3*delt+inc-1]){
            _ballXVel = 0;
            _ballYVel = -2;
        }
        else if (_ballXPos >= _paddlePos[3*delt+inc] && _ballXPos <= _paddlePos[4*delt+inc-1]){
            _ballXVel = 1;
            _ballYVel = -2;
        }
        else if (_ballXPos >= _paddlePos[4*delt+inc] && _ballXPos <= _paddlePos[_paddleWidth-1]+2){
            _ballXVel = 1;
            _ballYVel = -2;
        }
    }
}

void BreakoutClass::checkBlockCollision(Adafruit_SSD1306& display){
    for (int i = _numRows; i >= 0; i--){
        for (int j = 0; j < _numCols; j++){
            if (_ballXPos >= _blockArr[i][j][0] && _ballXPos <= _blockArr[i][j][0]+_blockWidth){
                if (_ballYPos <= _blockArr[i][j][1]+1 && _ballYPos >= _blockArr[i][j][1]-1){
                    _ballYVel = -2;
                    drawBlock(display,_blockArr[i][j][0],_blockArr[i][j][1],0);
                    _blockArr[i][j][0] = 1000;
                    _blockArr[i][j][1] = 1000;
                }
                else if (_ballYPos <= _blockArr[i][j][1]+_blockHeight+1 && _ballYPos >= _blockArr[i][j][1]+_blockHeight){
                    _ballYVel = 2;
                    drawBlock(display,_blockArr[i][j][0],_blockArr[i][j][1],0);
                    _blockArr[i][j][0] = 1000;
                    _blockArr[i][j][1] = 1000;
                }
            }

            if (_ballYPos <= _blockArr[i][j][1]+_blockHeight && _ballYPos >= _blockArr[i][j][1]){
                if (_ballXPos >= _blockArr[i][j][0]-1 && _ballXPos <= _blockArr[i][j][0]+1){
                    _ballXVel = -1;
                    drawBlock(display,_blockArr[i][j][0],_blockArr[i][j][1],0);
                    _blockArr[i][j][0] = 1000;
                    _blockArr[i][j][1] = 1000;
                }
                else if (_ballXPos-1 <= _blockArr[i][j][0]+_blockWidth && _ballXPos-1 >= _blockArr[i][j][0]+_blockWidth-2){
                    _ballXVel = 1;
                    drawBlock(display,_blockArr[i][j][0],_blockArr[i][j][1],0);
                    _blockArr[i][j][0] = 1000;
                    _blockArr[i][j][1] = 1000;
                }
            }

        }
    }
}

void BreakoutClass::resetLaunch(Adafruit_SSD1306& display){
    int randNum = random(1,3);
    drawPaddle(display,0,0);
    _xRangeMin = display.width()/2 - _paddleWidth/2;
    _xRangeMax = display.width()/2 + _paddleWidth/2;
    delay(1500);
    _launchBall = 0;
    if (randNum == 1){
        _ballXVel = 1;
    }
    else{
        _ballXVel = -1;
    }
    _ballYVel = -1;
    createPaddle(display);
    createBall(display);
}

void BreakoutClass::displayScore(Adafruit_SSD1306 &display){
    for (int i = 0; i < _numRows; i++){
        for (int j = 0; j < _numCols; j++){
            if (_blockArr[i][j][0] == 1000 && _blockArr[i][j][1] == 1000){
                _score = _score+10;
            }
        }
    }
    TextFunctions txtFuncs(100, 5, 32, display);
    display.clearDisplay();
    char result[30];
    char arr1[] = "You scored ";
    char buffer[9];
    itoa(_score,buffer,10);
    strcpy(result,arr1);
    strcat(result,buffer);
    txtFuncs.drawCenteredText(result,display);
    delay(1500);
    display.clearDisplay();
    txtFuncs.drawCenteredText("Game Over",display);
    delay(1500);
    display.clearDisplay();
}

void BreakoutClass::restartGame(Adafruit_SSD1306 & display){
    _numLives = _maxLives;
    _launchBall = 0;
    _score = 0;
}
