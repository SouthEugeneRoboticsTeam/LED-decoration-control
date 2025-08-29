#include "FastLED.h"

#define DATA_PIN    5
//#define CLK_PIN   4
#define LED_TYPE    SK6812
#define COLOR_ORDER GRB
#define NUM_LEDS    17
int BRIGHTNESS = 240; // starting brightness, can go to 255
int MODE = 0;
int set = 1;
int modevarA = 0;
int modevarB = 0;

//flags
long int sertpleFade[5] = {0x8c00ff,0x000000,0x000000,0x8c00ff,0x8c00ff};
long int rainbow[18] = {0xe80000,0xe80000,0xe80000,0xff6e00,0xff6e00,0xff6e00,0xffff00,0xffff00,0xffff00,0x084000,0x084000,0x084000,0x004bff,0x004bff,0x004bff,0x780089,0x780089,0x780089};
long int trans[17] = {0x3b8ffa,0x3b8ffa,0x3b8ffa,0x3b8ffa,0xf00bb9,0xf00bb9,0xf00bb9,0xffffff,0xffffff,0xffffff,0xf00bb9,0xf00bb9,0xf00bb9,0x3b8ffa,0x3b8ffa,0x3b8ffa, 0x3b8ffa};

CRGB leds[NUM_LEDS];


void setup() {
  delay(3000); // 3 second delay for psu recovery

  pinMode(3, INPUT_PULLUP); // mode button pin, also must be set below
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
  if (digitalRead(3) == 1){
    delay(100);
    if (digitalRead(3) == 0){
      delay(100);
      if (digitalRead(3) == 1){
        delay(100);
        if (digitalRead(3) == 0){ // was a double click, adjust brightness
          BRIGHTNESS = BRIGHTNESS - 40; // step size
          if (BRIGHTNESS < 40){ // min brightness
            BRIGHTNESS = 240; // max brightness, six steps
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
      else if (digitalRead(3) == 0){ // single click, increment mode
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
    else if (MODE == 3){ // both flags are synced to sign size
      for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = rainbow[modevarA];
        modevarA ++;
        }
//        delay(100);
        FastLED.show();
      }
    else if (MODE == 4){
      for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = trans[modevarA];
        modevarA ++;
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
        if (modevarB >4){
          modevarB = 0;
           } 
        }
            delay(65);
      FastLED.show();
      }
}
