#include <Arduino.h>
#include "./FastLed/FastLED.h"
#include "World.hpp"


#define FWBLOCK_PIN 4
#define TEST_PIN 8

World *world;


bool ledsChanged = false;


void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  pinMode(FWBLOCK_PIN, INPUT);
  pinMode(TEST_PIN, INPUT_PULLUP);

  delay( 1000 ); // power-up safety delay
  world = new World();
}

void loop() {
  if(Serial1.available()) {
    int val = Serial1.read();
    world->processCommand(val);
  }

  if (digitalRead(FWBLOCK_PIN) == 0) {
    return;
  }
  
  unsigned long now = millis();
  if (digitalRead(TEST_PIN)) {
    world->test(now);
  } else {
    world->process(now);
  }

  if (ledsChanged) {
    FastLED.show();
    ledsChanged = false;
  }
}
