#include "TextFunctions.h" // Bind the implementation to your header file
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Constructor implementation
TextFunctions::TextFunctions(int maxBuffSize, int maxRowSize, int maxLineSize, Adafruit_SSD1306& display): _display(display) {
  _maxBuffSize = maxBuffSize;        // Store the provided pin number in the private variable
  _maxRowSize = maxRowSize;
  _maxLineSize = maxLineSize;
  _display = display;
}

int TextFunctions::findLastIndex(char *target, char *substr){
    char *haystack = {target};
    char *needle = {substr};

    char *lastMatch = NULL;
    char *currentMatch = strstr(haystack, needle); // Find first occurrence

    int niter = 0;

    while (currentMatch != NULL) {
      lastMatch = currentMatch; // Update the last known match
      currentMatch = strstr(currentMatch + 1, needle); // Search again from the next character
      niter = niter+1;
      if (niter>500){
        break;
      }
    }

    if (lastMatch != NULL) {
      int index = lastMatch - haystack; // Calculate the 0-based index position
      return index;
    } else {
      return -1;
    }
  }

  void TextFunctions::getSubstring(char* target, int start, int len, size_t maxBuffSize) {
    char src[maxBuffSize];
    for (int i = 0; i < len; i++) {
        src[i] = target[start + i];
    }
    src[len] = '\0';
    for (int i = 0; i< len; i++){
        target[i] = src[i];
    }
    target[len] = '\0';
}

void TextFunctions::drawCenteredText(char* text, Adafruit_SSD1306& display) {
    int16_t x1, y1;
    uint16_t w, h;
    size_t maxBuffSize = 100;
    size_t maxRowSize = 6;
    size_t maxLineSize = 32;
    int totalSum = 0;
    int widths[maxRowSize];
    int heights[maxRowSize];
    int SCREEN_WIDTH = display.width();
    int SCREEN_HEIGHT = display.height();

    char targtext[maxBuffSize];
    for (int i = 0; i < maxBuffSize; i++){
        targtext[i] = '\0';
    }

    int lastIndex = findLastIndex(targtext, " ");
    int lastTargInd;

    for (int i = 0; i < strlen(text); i++){
        targtext[i] = text[i];
    }

    int s = 0;
    char messageList[maxRowSize][maxLineSize];
    for (int i = 0; i < maxRowSize; i++){
        messageList[i][0] = '\0';
        widths[i] = 0;
        heights[i] = 0;
    }

    display.getTextBounds(targtext, 0, 0, &x1, &y1, &w, &h);
    if (*&w>=126){
        while (strchr(targtext, ' ') != nullptr){
            while (*&w >= 126){
                getSubstring(targtext, 0, lastIndex, maxBuffSize);
                display.getTextBounds(targtext, 0, 0, &x1, &y1, &w, &h);

                lastIndex = findLastIndex(targtext," ");
            }

            for (int i = 0; i < strlen(targtext); i++) {
                messageList[s][i] = targtext[i];
            };
            messageList[s][strlen(targtext)] = '\0';
            widths[s] = (SCREEN_WIDTH - w)/2;;
            heights[s] = h;

            int len = strlen(targtext);
            lastTargInd = findLastIndex(text, targtext);
            for (int i = 0; i < strlen(text); i++){
                targtext[i] = text[i];
            }
            getSubstring(targtext, lastTargInd+len+1, strlen(text)-len-1, maxBuffSize);
            lastIndex = findLastIndex(targtext," ");

            display.getTextBounds(targtext, 0, 0, &x1, &y1, &w, &h);
            s=s+1;
        }
    }

    for (int i = 0; i < strlen(targtext); i++) {
        messageList[s][i] = targtext[i];
    };
    messageList[s][strlen(targtext)] = '\0';
    widths[s] = (SCREEN_WIDTH - w)/2;;
    heights[s] = h;

    for (int i = 0; i < maxRowSize; i++) {
        totalSum += heights[i]; 
    }

    int startH = (SCREEN_HEIGHT - totalSum) / 2;
    for (int i = 0; i < maxRowSize; i++){
        display.setCursor(widths[i], startH);
        display.print(messageList[i]);
        startH = startH + heights[i];
    }

    display.display();
    //delay(2000);

    /*display.clearDisplay();
    display.display();*/

}

void TextFunctions::drawAlignedText(char* text, Adafruit_SSD1306& display) {
    int16_t x1, y1;
    uint16_t w, h;
    size_t maxBuffSize = 100;
    size_t maxRowSize = 6;
    size_t maxLineSize = 32;
    int heights[maxRowSize];

    char targtext[maxBuffSize];
    for (int i = 0; i < maxBuffSize; i++){
        targtext[i] = '\0';
    }

    int lastIndex = findLastIndex(targtext, " ");
    int lastTargInd;

    for (int i = 0; i < strlen(text); i++){
        targtext[i] = text[i];
    }

    int s = 0;
    char messageList[maxRowSize][maxLineSize];
    for (int i = 0; i < maxRowSize; i++){
        messageList[i][0] = '\0';
        heights[i] = 0;
    }

    display.getTextBounds(targtext, 0, 0, &x1, &y1, &w, &h);
    if (*&w>=126){
        while (strchr(targtext, ' ') != nullptr){
            while (*&w >= 126){
                getSubstring(targtext, 0, lastIndex, maxBuffSize);
                display.getTextBounds(targtext, 0, 0, &x1, &y1, &w, &h);

                lastIndex = findLastIndex(targtext," ");
            }

            for (int i = 0; i < strlen(targtext); i++) {
                messageList[s][i] = targtext[i];
            };
            messageList[s][strlen(targtext)] = '\0';
            heights[s] = h;

            int len = strlen(targtext);
            lastTargInd = findLastIndex(text, targtext);
            for (int i = 0; i < strlen(text); i++){
                targtext[i] = text[i];
            }
            getSubstring(targtext, lastTargInd+len+1, strlen(text)-len-1, maxBuffSize);
            lastIndex = findLastIndex(targtext," ");

            display.getTextBounds(targtext, 0, 0, &x1, &y1, &w, &h);
            s=s+1;
        }
    }

    for (int i = 0; i < strlen(targtext); i++) {
        messageList[s][i] = targtext[i];
    };
    messageList[s][strlen(targtext)] = '\0';
    heights[s] = h;

    int startH = 0;
    for (int i = 0; i < maxRowSize; i++){
        display.setCursor(0, startH);
        display.print(messageList[i]);
        startH = startH + heights[i];
    }

    display.display();
    /*delay(2000);

    display.clearDisplay();
    display.display();*/

}

/*void TextFunctions::displayMenu(Adafruit_SSD1306& display) {
    int16_t x1, y1;
    uint16_t w, h;
    char messageList[3][15];
    int widths[3];
    int heights[3];
    int totalSum = 0;
    int SCREEN_WIDTH = display.width();
    int SCREEN_HEIGHT = display.height();

    strcpy(messageList[0],"Pong");
    strcpy(messageList[1], "Snake");
    strcpy(messageList[2], "Other Game");

    for (int i = 0; i < 3; i++){
        display.getTextBounds(messageList[i], 0, 0, &x1, &y1, &w, &h);
        widths[i] = (SCREEN_WIDTH - w)/2;
        heights[i] = h;
    }

    for (int i = 0; i < 3; i++){
        totalSum += heights[i];
    }

    int startH = (SCREEN_HEIGHT - totalSum) / 2 + heights[1];
    for (int i = 0; i < 3; i++){
        display.setCursor(widths[i], startH);
        display.print(messageList[i]);
        startH = startH + heights[i];
    }

    display.setCursor(0,0);
    drawAlignedText("Select which game you would like to play: ", display);
    invertPixelRange(display, (SCREEN_WIDTH-widths[2])/2, (SCREEN_HEIGHT - totalSum) / 2 + heights[1] + heights[2], widths[2], heights[2]);

    display.display();

}*/

void TextFunctions::invertPixelRange(Adafruit_SSD1306 &display, int16_t x, int16_t y, int16_t width, int16_t height) {
    // Loop through every pixel in the specified rectangular range
    for (int16_t i = x; i < x + width; i++) {
        for (int16_t j = y; j < y + height; j++) {
            
            // Check boundaries to prevent memory corruption/crashing
            if (i >= 0 && i < display.width() && j >= 0 && j < display.height()) {
                
                // If the pixel is ON (SSD1306_WHITE), turn it OFF (SSD1306_BLACK)
                // If it is OFF, turn it ON
                if (display.getPixel(i, j) == SSD1306_WHITE) {
                    display.drawPixel(i, j, SSD1306_BLACK);
                } else {
                    display.drawPixel(i, j, SSD1306_WHITE);
                }
            }
        }
    }
}