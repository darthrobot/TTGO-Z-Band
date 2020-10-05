/*
 An example digital clock using a TFT LCD screen to show the time.
 Demonstrates use of the font printing routines. (Time updates but date does not.)
 
 For a more accurate clock, it would be better to use the RTClib library.
 But this is just a demo. 
 
 This examples uses the hardware SPI only. Non-hardware SPI
 is just too slow (~8 times slower!)
 
 Based on clock sketch by Gilchrist 6/2/2014 1.0
 Updated by Bodmer
A few colour codes:
 
code	color
0x0000	Black
0xFFFF	White
0xBDF7	Light Gray
0x7BEF	Dark Gray
0xF800	Red
0xFFE0	Yellow
0xFBE0	Orange
0x79E0	Brown
0x7E0	Green
0x7FF	Cyan
0x1F	Blue
0xF81F	Pink

 */
#define FS_NO_GLOBALS
#include <FS.h>

#ifdef ESP32
  #include "SPIFFS.h" // ESP32 only
#endif

#include <Arduino.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
//#include <SPI.h>
#include "NotoSansBold36.h"
#include <JPEGDecoder.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
#define AA_FONT_LARGE NotoSansBold36

uint32_t targetTime = 0;       // for next 1 second timeout

byte omm = 99;
boolean initial = 1;
byte xcolon = 0;
unsigned int colour = 0;

static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

struct Button {
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};

Button button1 = {0, 0, false};
Button button2 = {35, 0, false};
void IRAM_ATTR isr(void* arg) {
    Button* s = static_cast<Button*>(arg);
    s->numberKeyPresses += 1;
    s->pressed = true;
}


void setup(void) {
  Serial.begin(250000);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_GREEN);

    if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nInitialisation done.");

  tft.setTextColor(TFT_WHITE, TFT_GREEN); // Note: the new fonts do not draw the background colour
  tft.loadFont(AA_FONT_LARGE); 



  pinMode(button1.PIN, INPUT_PULLUP);
  pinMode(button2.PIN, INPUT_PULLUP);
  attachInterruptArg(button1.PIN, isr, &button1, FALLING);
    attachInterruptArg(button2.PIN, isr, &button2, FALLING);
}

void loop() {

  
      tft.fillScreen(TFT_GREEN);
      tft.setCursor(40, 50); // Set cursor at top left of screen
      tft.loadFont(AA_FONT_LARGE); 
      tft.setTextColor(0xFFFF);
      char buffer[10] = {'O','N','L','I','N','E'};
      tft.println(buffer); // Next size up font 2
      while(!button1.pressed&&!button2.pressed){delay(20);}

   if (button1.pressed) {
      tft.fillScreen(TFT_RED);
      tft.setCursor(35, 50); // Set cursor at top left of screen
      tft.loadFont(AA_FONT_LARGE); 
      tft.setTextColor(0xFFFF);
      char buffer1[11] = {'O','F','F','L','I','N','E'};
      tft.println(buffer1); // Next size up font 2
      delay(9999);
      button1.pressed = false;
      button1.numberKeyPresses = 0;
    }

    if (button2.pressed) {   
      tft.fillScreen(TFT_RED);
      tft.setCursor(35, 50); // Set cursor at top left of screen
      tft.loadFont(AA_FONT_LARGE); 
      tft.setTextColor(0xFFFF);
      char buffer1[11] = {'O','F','F','L','I','N','E','!'};
      tft.println(buffer1); // Next size up font 2
      delay(1000);
      for(int i=0; i<=5; i++){
      drawJpeg("/bones_25.jpg", 0, 0);
      delay(1000);
      tft.fillScreen(TFT_RED);
      tft.setCursor(35, 50); // Set cursor at top left of screen
      tft.println(buffer1); // Next size up font 2
      delay(1000);
      }
      button2.pressed = false;
      button2.numberKeyPresses = 0;
    }



}
