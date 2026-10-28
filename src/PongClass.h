#ifndef PONGCLASS_H    // Include guard: Prevents duplicate compilation errors
#define PONGCLASS_H

#include <Arduino.h> // Allows the use of built-in Arduino functions like digitalWrite()
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PaddleClass.h>
#include <BallClass.h>

class PongClass {
  private:
    Adafruit_SSD1306& _display;

  public:
    // Constructor: Runs automatically when a class instance is created
    int _homeScore = 0;
    int _awayScore = 0;

    PongClass(Adafruit_SSD1306& display); 

    void gameStartup(Adafruit_SSD1306& display);
    void createScoreboard(Adafruit_SSD1306& display);
    void resetGame(Adafruit_SSD1306& display, PaddleClass &homePaddle, PaddleClass &awayPaddle, BallClass &ball);
};

#endif