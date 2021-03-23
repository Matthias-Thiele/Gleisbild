#include <Arduino.h>
#include "./FastLed/FastLED.h"
#include "AsyncTmer.hpp"
#include "FahrwegIterator.hpp"

void trackHandler(AsyncTimer *timer, unsigned long now);

#define LED_PIN     5
#define NUM_LEDS    300
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

void showTrack();
void setTrain(int pos);
void setSignal();

CRGB leds[NUM_LEDS];
unsigned long start;
int pos;
int loopCount = 1;


void testFW() {
  FahrwegIterator fwi;
  int list1[] = {0, 3, 10, 13, 23, 20, 30, 30, -1};
  fwi.setFahrwegList(list1);

  Serial.println("Fahrweg Test");
  while (fwi.hasMore()) {
    int pos = fwi.nextPos();
    Serial.print(pos);
    Serial.print(" - ");
  }

  Serial.println("done.");
}

void setup() {
  Serial.begin(115200);
  testFW();

  Serial.println("Gleisbild started.");
  // put your setup code here, to run once:
  AsyncTimer::add(500, -1, 12345, trackHandler);
  start = millis();
  pos = -10;

  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  showTrack();
  setSignal();
  setTrain(1);

  FastLED.show();
}

void showTrack() {
  for (int i = 0; i < 75; i++) {
    leds[i].b = 0x80;
    leds[i].r = 0x80;
    leds[i].g = 0x80;
  }
}

void setTrain(int pos) {
  if (pos < 0) {
    pos = 0;
  }

  for (int i = 0; i < 5; i++) {
    if (i + pos > 74) break;
    leds[i + pos] = CRGB::Blue;
  }
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
  AsyncTimer::tick();
}

void trackHandler(AsyncTimer *timer, unsigned long now) {
    Serial.print("Show Track ");
    Serial.print(timer->getActivated());
    Serial.print(" ");
    Serial.print(timer->getParam());
    Serial.print(" ");
    Serial.println(timer->isOdd());

    unsigned long start = millis();
    showTrack();
    setSignal();
    setTrain(pos++);
    FastLED.show();
    timer->setParam(millis() - start);

    if (pos > 75) {
      pos = -10;
      start += 1000;

      loopCount++;
      if (loopCount > 3) {
        loopCount = 0;
      }
      FastLED.setBrightness(  BRIGHTNESS * loopCount - 1 );
    }
}

