#include "PongClass.h" // Bind the implementation to your header file
#include "MenuClass.h" // Bind the implementation to your header file
#include "TextFunctions.h" // Bind the implementation to your header file
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PaddleClass.h>
#include <BallClass.h>

// Constructor implementation
PongClass::PongClass(Adafruit_SSD1306& display): _display(display) {
  _display = display;

}

void PongClass::gameStartup(Adafruit_SSD1306 &display) {
  TextFunctions txtFuncs(100, 5, 32, display);
  display.clearDisplay();
  display.setCursor(0,0);
  txtFuncs.drawCenteredText("Are you aready to play pong?",display);
  delay(1500);
  display.clearDisplay();
  //createScoreboard(display);
  display.display();
}

void PongClass::createScoreboard(Adafruit_SSD1306 &display) {
  display.setCursor(20,5);
  display.print(_homeScore);
  display.setCursor(102,5);
  display.print(_awayScore);
  display.display();
}

void PongClass::resetGame(Adafruit_SSD1306 &display, PaddleClass &homePaddle, PaddleClass &awayPaddle, BallClass &ball){
  homePaddle._yRangeMin = display.height()/2 - homePaddle._paddleLength/2;
  homePaddle._yRangeMax = display.height()/2 + homePaddle._paddleLength/2;
  awayPaddle._yRangeMin = display.height()/2 - awayPaddle._paddleLength/2;
  awayPaddle._yRangeMax = display.height()/2 + awayPaddle._paddleLength/2;
  ball._xPos = display.width()/2;
  ball._yPos = display.height()/2;
  ball._prevXPos = ball._xPos;
  ball._prevYPos = ball._yPos;
  ball._xVel = -1;
  ball._yVel = 0;
}
