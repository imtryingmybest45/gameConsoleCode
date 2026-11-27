#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <TextFunctions.h>
#include <MenuClass.h>
#include <PongClass.h>
#include <PaddleClass.h>
#include <BallClass.h>
#include <SnakeClass.h>
#include <BreakoutClass.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#endif

int switchStateV = 0;
int switchStateA = 0;
int switchStateW = 0;
int switchStateS = 0;
int switchStateD = 0;
int switchStatePower = 0;
TextFunctions txtFuncs(100, 5, 32, display);
MenuClass menu(100, 5, 32, display);
PongClass pong(display);
PaddleClass awayPaddle(display, 0);
PaddleClass homePaddle(display, 1);
BallClass ball(display, -1);
SnakeClass snake(display);
BreakoutClass breakout(display);
int pongMode = 0;
int onFlag = 1;
int snakeMode = 0;
int breakoutMode = 0;
int startGameFlag = 1;
int awayWin = 0;
int physFlag = 0;


void setup()  {

    pinMode(2,INPUT); //S pin
    pinMode(4,INPUT); //W pin
    pinMode(5,INPUT); //A pin
    pinMode(12,INPUT); //D pin
    pinMode(13,INPUT); //V pin
    if (physFlag == 1){
        pinMode(11,INPUT);
    }
    else{
        pinMode(26, INPUT);
    }

    Serial.begin(9600);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display();
    delay(800);
    display.clearDisplay();
    display.display();

    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);

    txtFuncs.drawCenteredText("Welcome to my gaming console.",display);
    delay(2000);
    display.clearDisplay();
    menu.displayMenu(display);
}

void menuFunc(MenuClass &menu, Adafruit_SSD1306 &display){
   if (menu._menuMode == 1){
        display.clearDisplay();
        menu.displayGameMenu(display);
   }
   else if (menu._menuMode == 2){
        if (menu._gameMenuOption == 0){
            if (menu._gameMode == 0){
                pongMode = 1;
                menu._menuMode = 2;
            }
            else if (menu._gameMode == 1){
                snakeMode = 1;
            }
            else{
                breakoutMode = 1;
            }
        }
        else if (menu._gameMenuOption == 1){
            if (menu._gameMode == 0){
                Serial.println("You are now viewing the high scores for pong.");
            }
            else if (menu._gameMode == 1){
                Serial.println("You are now viewing the high scores for Snake.");
            }
            else{
                Serial.println("You are now viewing the high scores for Breakout.");
            }
        }
        else{
            display.clearDisplay();
            menu.displayMenu(display);
        }
   }
   else{
        if (menu._endGameOption == 0){
            if (menu._gameMode == 0){
                pongMode = 1;
            }
            else if (menu._gameMode == 1){
                snakeMode = 1;
            }
            else if (menu._gameMode == 2){
                breakoutMode = 1;
            }
        }
        else{
            display.clearDisplay();
            menu.displayMenu(display);
        }
   }
    delay(300);
}

void loop(){
  switchStateA = digitalRead(5);
  switchStateS = digitalRead(2);
  switchStateW = digitalRead(4);
  switchStateD = digitalRead(12);
  switchStateV = digitalRead(13);

  if (physFlag == 1){
    switchStatePower = digitalRead(11);
  }
  else{
    switchStatePower = digitalRead(26);
  }

    if (switchStateV == 1){
       menuFunc(menu, display);
       if (pongMode == 1 && startGameFlag == 1){
            pong.gameStartup(display);
            homePaddle.createPaddle(display);
            awayPaddle.createPaddle(display);
            ball.createBall(display);
            startGameFlag = 0;
       }
       else if(snakeMode == 1 && startGameFlag == 1){
            snake.gameStartup(display);
            snake.createSnake(display);
            snake.spawnFood(display);
            startGameFlag = 0;
       }
       else if(breakoutMode == 1 && startGameFlag == 1){
            breakout.gameStartup(display);
            breakout.createBoard(display);
            breakout.populateLives();
            breakout.updateLives(display);
            breakout._xRangeMin = display.width()/2 - breakout._paddleWidth/2;
            breakout._xRangeMax = display.width()/2 + breakout._paddleWidth/2;
            breakout._launchBall = 0;
            if (random(1,3) == 1){
                breakout._ballXVel = 1;
            }
            else{
                breakout._ballXVel = -1;
            }
            breakout._ballYVel = -1;
            breakout.createPaddle(display);
            breakout.createBall(display);
            startGameFlag = 0;
       }
       else if (breakoutMode == 1){
            if (breakout._launchBall == 0){
                breakout._launchBall = 1;
            }
       }
    }

    if (switchStateA == 1){
        if (snakeMode == 1){
            if (snake._snakeInfo[0][0][2]==0){
                snake.changeDirection(1);
            }
        }
        else if(breakoutMode == 1){
            if (abs(breakout._ballXVel)==2){
                breakout.movePaddleLeft(display,4);
            }
            else{
                breakout.movePaddleLeft(display,3);
            }
        }
    }

    if (switchStateW == 1){
        if (pongMode == 1){
            homePaddle.movePaddleUp(display, 2);
        }
        else if (snakeMode == 1){
            if (snake._snakeInfo[0][0][3] == 0){
                snake.changeDirection(2);
            }
        }
    }

    if (switchStateS == 1){
        if (pongMode == 1){
            homePaddle.movePaddleDown(display, 2);
        }
        else if (snakeMode == 1){
            if (snake._snakeInfo[0][0][3]==0){
                snake.changeDirection(4);
            }
        }
        else{
            menu.menuDown(display);
            delay(300);
        }
    }

    if (switchStateD == 1){
        if (snakeMode == 1){
            if (snake._snakeInfo[0][0][2]==0){
                snake.changeDirection(3);
            }
        }
        else if(breakoutMode == 1){
            if (abs(breakout._ballXVel) == 2){
                breakout.movePaddleRight(display,4);
            }
            else{
                breakout.movePaddleRight(display,3);
            }
        }
    }

    if (switchStatePower == 1){
        if (onFlag == 1){
            display.clearDisplay();
            display.display();
            onFlag = 0;
            pongMode = 0;
            snakeMode = 0;
            breakoutMode = 0;
            pong.resetGame(display, homePaddle, awayPaddle, ball);
            pong._homeScore = 0;
            pong._awayScore = 0;
            delay(500);
            startGameFlag = 1;
            snake._spawnFlag = 0;
            snake._currentLength = snake._initialLength;
            breakout._launchBall = 0;
            breakout._numLives = breakout._maxLives;
            breakout._numBlocks = breakout._numRows*breakout._numCols;
            if (random(1,3) == 1){
                breakout._ballXVel = 1;
            }
            else{
                breakout._ballXVel = -1;
            }
            breakout._ballYVel = -1;
            breakout._xRangeMin = display.width()/2 - breakout._paddleWidth/2;
            breakout._xRangeMax = display.width()/2 + breakout._paddleWidth/2;
            breakout.populateLives();
        }
        else{
            txtFuncs.drawCenteredText("Welcome to my gaming console.",display);
            delay(2000);
            display.clearDisplay();
            menu.displayMenu(display);
            onFlag = 1;
        }
    }

    if (pongMode == 1){
        ball.moveBall(display, homePaddle, awayPaddle);
        awayPaddle.moveAwayPaddle(display, ball);
        if (ball._xPos < 0 || ball._xPos > SCREEN_WIDTH){
            if (ball._xPos < 0){
                pong._awayScore = pong._awayScore+1;
                awayWin = 1;
            }
            else{
                pong._homeScore = pong._homeScore+1;
                awayWin = 0;
            }
            pong.createScoreboard(display);
            display.display();
            delay(1500);

            if(pong._homeScore == 7 || pong._awayScore == 7){
                pong.resetGame(display, homePaddle, awayPaddle, ball);
                if (pong._homeScore == 7){
                    txtFuncs.drawCenteredText("You win!",display);
                    delay(1500);
                }
                else{
                    txtFuncs.drawCenteredText("You lose!",display);
                    delay(1500);
                }
                pong._homeScore = 0;
                pong._awayScore = 0;
                display.clearDisplay();
                pongMode = 0;
                startGameFlag = 1;
                menu._menuMode = 3;
                display.clearDisplay();
                menu.displayEndMenu(display);
            }
            else{
                pong.resetGame(display, homePaddle, awayPaddle, ball);
                if (awayWin == 0){
                    ball._xVel = 1;
                }
                display.clearDisplay();
                display.display();
                homePaddle.createPaddle(display);
                awayPaddle.createPaddle(display);
                ball.createBall(display);
            }
        }
    }

    else if (snakeMode == 1){
        snake.moveSnake(display);
        snake.checkFood(display);
        snake.spawnFood(display);
        if (snake._gameOver == 1){
            snakeMode = 0;
            menu._menuMode = 3;
            startGameFlag = 1;
            snake.displayScore(display);
            snake.restartGame(display);
            menu.displayEndMenu(display);
        }
    }
    else if (breakoutMode == 1){
        if (breakout._launchBall == 1){
            breakout.moveBall(display);
            breakout.checkCollisions(display);
        }
        if (breakout._numLives == 0){
            breakoutMode = 0;
            menu._menuMode = 3;
            startGameFlag = 1;
            breakout.displayScore(display);
            breakout.restartGame(display);
            breakout.populateLives();
            menu.displayEndMenu(display);
        }
    }
}