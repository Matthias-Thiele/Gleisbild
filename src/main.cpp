#include <Arduino.h>
#include "./FastLed/FastLED.h"
#include "Fahrweg.hpp"
#include "Signal.hpp"
#include "Events.hpp"

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
bool selectAlthLieb = false;
CRGB trainColor = {0, 0, 0xff};
CRGB trackColor = {0x40, 0x40, 0x40};
Fahrweg fw(leds, trainColor, trackColor);

short list1[] = {1, 75, -1};
unsigned long events1[] = {
  SET_SIGNAL | (1 << 12) | 50,
  RESET_SIGNAL | (1 << 12) | 66,
  0
};

short list2[] = {76, 90, 61, 1, -1};
unsigned long events2[] = {
  WAIT_FOR_SIGNAL | (2 << 12) | 82,
  RESET_SIGNAL | (2 << 12) | 90,
  SET_SIGNAL | (0 << 12) | 20,
  RESET_SIGNAL| (0 << 12) | 1,
  0
};

short calwAlth[] = {1, 61, 90, 76, -1};
unsigned long evCalwAlth[] = {
  SET_SIGNAL | (5 << 12) | 50,
  RESET_SIGNAL | (5 << 12) | 86,
  0
};

short althCalw[] = {74, 1, -1};
unsigned long evAlthCalw[] = {
  WAIT_FOR_SIGNAL | (4 << 12) | 63,
  RESET_SIGNAL | (4 << 12) | 55,
  SET_SIGNAL | (3 << 12) | 20,
  RESET_SIGNAL | (3 << 12) | 1,
  0
};

short calwLieb[] = {184, 124, 95, 109, -1};
unsigned long evCalwLieb[] = {
  SET_SIGNAL | (8l << 12) | 160,
  RESET_SIGNAL | (8l << 12) | 100,
  0
};

short liebCalw[] = { 110, 184, -1};
unsigned long evLiebCalw[] = {
  WAIT_FOR_SIGNAL | (7 << 12) | 122,
  RESET_SIGNAL | (7 << 12) | 130,
  SET_SIGNAL | (6 << 12) | 164,
  RESET_SIGNAL | (6 << 12) | 183,
  0
};

Signal signale[9];
bool lastFWState = false;

void testFW(bool inOut) {

  if (inOut) {
    Serial.println("Nach Wildberg");
    fw.set(list1, events1);
  } else {
    Serial.println("Von Wildberg");
    fw.set(list2, events2);
  }

  if (inOut) {
    signale[0].blank();
    signale[1].release();
    signale[2].release();
  } else {
    signale[0].release();
    signale[1].blank();
    signale[2].release();
  }

  Serial.println("Fahrweg Test");
  fw.start();
}

void testFW2(bool inOut, bool althlbz) {

  if (althlbz) {
    if (inOut) {
      Serial.println("Nach Althengstett");
      fw.set(calwAlth, evCalwAlth);
    } else {
      Serial.println("Von Althengstett");
      fw.set(althCalw, evAlthCalw);
    }

    if (inOut) {
      signale[3].blank();
      signale[4].release();
      signale[5].release();
    } else {
      signale[3].release();
      signale[5].blank();
      signale[4].release();
    }
  } else {
    if (inOut) {
      Serial.println("Nach Liebenzell");
      fw.set(calwLieb, evCalwLieb);
    } else {
      Serial.println("Von Liebenzell");
      fw.set(liebCalw, evLiebCalw);
    }

    if (inOut) {
      signale[6].blank();
      signale[7].release();
      signale[8].release();
    } else {
      signale[6].release();
      signale[8].blank();
      signale[7].release();
    }

  }

  Serial.println("Fahrweg 2 Test");
  fw.start();
}

void setup() {
  Serial.begin(115200);
  pinMode(FWBLOCK_PIN, INPUT);

  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  Signal::setLeds(leds);
  signale[0].setLedNo(0);
  signale[1].setLedNo(91);
  signale[2].setLedNo(92);
  signale[3].setLedNo(0);
  signale[4].setLedNo(92);
  signale[5].setLedNo(91);
  signale[6].setLedNo(185);
  signale[7].setLedNo(94);
  signale[8].setLedNo(93);

  for (int i = 0; i < 6; i++) {
    signale[i].release();
  }
 
  
}


void loop() {
  // put your main code here, to run repeatedly:
  if (!selectFW && !lastFWState && digitalRead(FWBLOCK_PIN) == 1) {
    Serial.println("Ausfahrt aus AHLB erlaubt, set LastFW.");
    signale[selectAlthLieb ? 4 : 7].set();
    lastFWState = true;
  }

  if (lastFWState && digitalRead(FWBLOCK_PIN) == 0) {
    Serial.println("Reset LastFW.");
    lastFWState = false;
  }

  if (fw.done()) {
    fw.clear();
    for (int i = 0; i < 9; i++) {
      signale[i].blank();
    }

    FastLED.show();    
    delay(1000);
    if (!selectFW && digitalRead(FWBLOCK_PIN) == 1) {
      return;
    }

    selectFW = !selectFW;
    if (selectFW) {
      selectAlthLieb = !selectAlthLieb;
    }

    testFW2(selectFW, selectAlthLieb);
  }

  AsyncTimer::tick();
  if (ledsChanged) {
    FastLED.show();
    ledsChanged = false;
  }
}
