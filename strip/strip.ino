#include "FastLED.h"

#define DATA_PIN    12
//#define CLK_PIN   4
#define LED_TYPE    SK6812
#define COLOR_ORDER GRB
#define NUM_LEDS    200 // less than 200 leds in current pitlights strip (~100 leds?)
int BRIGHTNESS = 200; // starting brightness, can go to 255 but strip draws near 3A @ 200
int MODE = 0;
int set = 1;
int modevarA = 0;
int modevarB = 0;

//flags
int sertpleFade[5] = {0x8c00ff,0x000000,0x000000,0x8c00ff,0x8c00ff};
int rainbow[6] = {0xe80000,0xff6e00,0xffff00,0x084000,0x004bff,0x780089};
int trans[4] = {0x3b8ffa,0xf00bb9,0xffffff,0xf00bb9};

CRGB leds[NUM_LEDS];


void setup() {
  delay(3000); // 3 second delay for psu recovery

  pinMode(6, INPUT_PULLUP); // mode button pin, also must be set below
  Serial.begin(57600);
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


void loop()
{
  // read things, set vars, then act once vars are set   single click, double click, triple click, hold, click hold   < no; too many
  if (digitalRead(6) == 1){
    delay(100);
    if (digitalRead(6) == 0){
      delay(100);
      if (digitalRead(6) == 1){
        delay(100);
        if (digitalRead(6) == 0){ // was a double click, adjust brightness
          BRIGHTNESS = BRIGHTNESS - 40;  // step size
          if (BRIGHTNESS < 40){ // min brightness
            BRIGHTNESS = 200; // max brightness, 5 steps
          }
          FastLED.setBrightness(BRIGHTNESS);
          Serial.println(BRIGHTNESS);
//          if (digitalRead(6) == 1){
//            delay(100);
//            if (digitalRead(6) == 0){
//              Serial.println("triple click");
//            }
//          }

        }
      }
      else if (digitalRead(6) == 0){ // single click, increment mode
        MODE = MODE + 1;
        if (MODE > 4){ // amnt of modes
          MODE = 0;
        }
        Serial.println(MODE);
      }
    }
    set = 1; // button was pressed, notify modeswitcher
  }
  if (set == 1){ // begin non-anim
    if (MODE == 0){
      for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = 0x8c00ff; // purple
      }
      FastLED.show();  
    }
    else if (MODE == 1){
      for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = 0x7b7cca; // off white
        }
        FastLED.show();
      }
//    else if (MODE == 2){ // mode three is animated later
//      for( int i = 0; i < NUM_LEDS; ++i) {
//        leds[i] = sertpleFade[modevarA];
//        modevarA ++;
//        if (modevarA > 3){
//          modevarA = 0;
//          }
//        }
////        delay(100);
//        FastLED.show();
//      }
    else if (MODE == 3){
      for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = rainbow[modevarA];
        ++i; // two wide per colour
        leds[i] = rainbow[modevarA];
        modevarA ++;
        if (modevarA > 5){
          modevarA = 0;
          }
        }
//        delay(100);
        FastLED.show();
      }
    else if (MODE == 4){
      for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = trans[modevarA];
        ++i; // two wide per colour
        leds[i] = trans[modevarA];
        modevarA ++;
        if (modevarA > 3){
          modevarA = 0;
          }
        }
//        delay(100);
        FastLED.show();
      }
    set = 0; // finished updating 
    modevarA = 0;
  } // begin anim
     if (MODE == 2){
      for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = sertpleFade[modevarB];
        modevarB ++;
        if (modevarB >5){
          modevarB = 0;
           } 
        }
            delay(100);
      FastLED.show();
      }
}
