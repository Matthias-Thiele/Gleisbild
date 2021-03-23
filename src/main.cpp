#include <Arduino.h>
#include "./FastLed/FastLED.h"
#include "Fahrweg.hpp"

void trackHandler(AsyncTimer *timer, unsigned long now);

#define LED_PIN     5
#define NUM_LEDS    300
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

void showTrack();
void setSignal();

CRGB leds[NUM_LEDS];
bool ledsChanged = false;

unsigned long start;
int pos;
int loopCount = 1;
bool selectFW = false;
CRGB trainColor = {0, 0, 0xff};
CRGB trackColor = {0x40, 0x40, 0x40};
Fahrweg fw(leds, trainColor, trackColor);
short list1[] = {0, 74, -1};
short list2[] = {75, 89, 60, 0, -1};

void testFW(bool inOut) {

  fw.set((inOut) ? list1 : list2, NULL);

  Serial.println("Fahrweg Test");
  fw.start();
}

void setup() {
  Serial.begin(115200);

  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
}


void setSignal() {
  if (pos > 10 || pos < 0) {
    leds[7] = CRGB::Red;
  } else {
    leds[7] = CRGB::Green;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (fw.done()) {
    fw.clear();
    FastLED.show();    
    delay(3000);
    selectFW = !selectFW;
    testFW(selectFW);
  }

  AsyncTimer::tick();
  if (ledsChanged) {
    FastLED.show();
    ledsChanged = false;
  }
}
