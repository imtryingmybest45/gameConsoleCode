#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


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

int switchStateA = LOW;
int switchStateS = LOW;
int switchStateW = LOW;
int switchStateD = LOW;
int switchStateV = LOW;


void setup()  {

  pinMode(23,INPUT);
  pinMode(2,INPUT);
  pinMode(18,INPUT);
  pinMode(16,INPUT);
  pinMode(4,INPUT);

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(800);
  display.clearDisplay();
  display.display();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(2, 2);

}

void loop(){

  switchStateA = digitalRead(4);
  switchStateS = digitalRead(2);
  switchStateW = digitalRead(23);
  switchStateD = digitalRead(16);
  switchStateV = digitalRead(18);

  if (switchStateA == HIGH){
    display.setCursor(0,0);
    display.print("A");
    display.display();
    delay(800);
    display.clearDisplay();
    display.display();
  }

  if (switchStateW == HIGH){
    display.setCursor(0,0);
    display.print("W");
    display.display();
    delay(800);
    display.clearDisplay();
    display.display();
  }

  if (switchStateS == HIGH){
    display.setCursor(0,0);
    display.print("S");
    display.display();
    delay(800);
    display.clearDisplay();
    display.display();
  }

  if (switchStateD == HIGH){
    display.setCursor(0,0);
    display.print("D");
    display.display();
    delay(800);
    display.clearDisplay();
    display.display();
  }

  if (switchStateV == HIGH){
    display.setCursor(0,0);
    display.print("Select");
    display.display();
    delay(800);
    display.clearDisplay();
    display.display();
  }
    
}
