#ifndef TEXTFUNCTIONS_H    // Include guard: Prevents duplicate compilation errors
#define TEXTFUNCTIONS_H

#include <Arduino.h> // Allows the use of built-in Arduino functions like digitalWrite()
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class TextFunctions {
  private:
    int _maxBuffSize;        // Private variable accessible only within this class
    int _maxRowSize;
    int _maxLineSize;
    Adafruit_SSD1306& _display;

  public:
    // Constructor: Runs automatically when a class instance is created
    TextFunctions(int maxBuffSize, int maxRowSize, int maxLineSize, Adafruit_SSD1306& display); 

    int findLastIndex(char *target, char *substr);
    void getSubstring(char* target, int start, int len, size_t maxBuffSize);
    void drawCenteredText(char* text, Adafruit_SSD1306& display);
    void drawAlignedText(char* text, Adafruit_SSD1306& display);
    //void displayMenu(Adafruit_SSD1306& display);
    void invertPixelRange(Adafruit_SSD1306 &display, int16_t x, int16_t y, int16_t width, int16_t height);
};

#endif