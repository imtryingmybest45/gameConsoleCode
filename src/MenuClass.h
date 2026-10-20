#ifndef MENUCLASS_H    // Include guard: Prevents duplicate compilation errors
#define MENUCLASS_H

#include <Arduino.h> // Allows the use of built-in Arduino functions like digitalWrite()
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class MenuClass {
  private:
    int _maxBuffSize;        // Private variable accessible only within this class
    int _maxRowSize;
    int _maxLineSize;
    Adafruit_SSD1306& _display;

  public:
    // Constructor: Runs automatically when a class instance is created
    int _widths[3];
    int _heights[3];
    int _menuInd = 0;
    int _mainMenu = 1;
    int _gameMode = 0;
    int _gameMenuOption = 0;

    MenuClass(int maxBuffSize, int maxRowSize, int maxLineSize, Adafruit_SSD1306& display); 

    void displayMenu(Adafruit_SSD1306& display);
    void displayGameMenu(Adafruit_SSD1306& display);
    void menuDown(Adafruit_SSD1306& display);
};

#endif