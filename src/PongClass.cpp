#include "PongClass.h" // Bind the implementation to your header file
#include "MenuClass.h" // Bind the implementation to your header file
#include "TextFunctions.h" // Bind the implementation to your header file
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
  createScoreboard(display);
}

void PongClass::createScoreboard(Adafruit_SSD1306 &display) {
  display.setCursor(20,5);
  display.print(_homeScore);
  display.setCursor(102,5);
  display.print(_awayScore);
  display.display();
}
