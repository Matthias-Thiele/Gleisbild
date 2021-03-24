#include <Arduino.h>
#include "./FastLed/FastLED.h"
#include "Fahrweg.hpp"

void trackHandler(AsyncTimer *timer, unsigned long now);

#define LED_PIN     5
#define FWBLOCK_PIN 4
#define NUM_LEDS    300
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

void showTrack();
void setSignal(int go);

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

  if (!inOut) {
    setSignal(2);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(FWBLOCK_PIN, INPUT);

  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
}

int signalState = 0;

void setSignal(int go) {
  signalState = go;
  switch (go) {
    case 0:
      leds[83] = CRGB::Black;
      break;

    case 1:
      leds[83] = CRGB::Green;
      Serial.println("Set signal green");
      break;

    case 2:
    case 3:
      leds[83] = CRGB::Red;
      Serial.println("Set signal red");
      break;
  }

  ledsChanged = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Signal ");Serial.print(signalState); Serial.print(", fw "); Serial.print(selectFW); Serial.print(", pin "); Serial.println(digitalRead(FWBLOCK_PIN)); delay(100);
  if (!selectFW && ! fw.done() && digitalRead(FWBLOCK_PIN) == 1) {
    if (signalState == 2) {
      setSignal(1);
    }
  }

  if (fw.done()) {
    fw.clear();
    setSignal(0);
    FastLED.show();    
    delay(3000);
    if (!selectFW && digitalRead(FWBLOCK_PIN) == 1) {
      return;
    }
    selectFW = !selectFW;
    testFW(selectFW);
  }

  AsyncTimer::tick();
  if (ledsChanged) {
    FastLED.show();
    ledsChanged = false;
  }
}
