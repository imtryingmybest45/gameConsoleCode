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
TextFunctions txtFuncs(100, 5, 32, display);
MenuClass menu(100, 5, 32, display);
int menuMode = 1;
int playPongMode = 0;


void setup()  {

    pinMode(2,INPUT); //S pin
    pinMode(4,INPUT); //W pin
    pinMode(5,INPUT); //A pin
    pinMode(12,INPUT); //D pin
    pinMode(13,INPUT); //V pin

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
    display.clearDisplay();
    display.display();
    menu.displayMenu(display);

    Serial.println("Hello!");


}

void playPong(){
    Serial.println("This is the function to play pong");
}

MenuClass menuFunc(MenuClass menu, Adafruit_SSD1306 display){
    display.clearDisplay();
    display.display();
    if (menu._menuInd == 2 && menu._menuLoc == 0){
        menu.displayMenu(display);
    }
    else if (menu._menuInd == 0 && menu._menuLoc == 0){
        playPong();
    }
    else{
        menu.displayGameMenu(display);
    }
    delay(300);
}

void loop(){
    
  switchStateA = digitalRead(5);
  switchStateS = digitalRead(2);
  switchStateW = digitalRead(4);
  switchStateD = digitalRead(12);
  switchStateV = digitalRead(13);

    if (switchStateV == 1){
        if (menuMode == 1){
            //menuFunc(menu, display);
            display.clearDisplay();
            display.display();
            if (menu._menuInd == 2 && menu._menuLoc == 0){
                menu.displayMenu(display);
            }
            else if (menu._menuInd == 0 && menu._menuLoc == 0){
                //playPong();
            }
            else{
                menu.displayGameMenu(display);
            }
            delay(300);
        }
        else if(playPongMode == 1){
            playPong();
        }
    }


    
    if (switchStateA == 1){
    }

    if (switchStateW == 1){
    }

    if (switchStateS == 1){
        menu.menuDown(display);
        delay(300);
    }

    if (switchStateD == 1){
    }
}