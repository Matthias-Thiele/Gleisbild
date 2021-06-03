#include <Arduino.h>
#include "./FastLed/FastLED.h"
#include "World.hpp"


#define FWBLOCK_PIN 4

World *world;


bool ledsChanged = false;


void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  pinMode(FWBLOCK_PIN, INPUT);

  delay( 1000 ); // power-up safety delay
  world = new World();
}

void loop() {
  if(Serial1.available()) {
    int val = Serial1.read();
    uint8_t pk = (val >> 2) & 0x3C;
    Serial.print("From Mega2 "); Serial.print(val, HEX); Serial.print(", "); Serial.println(pk);
    for (int i = 0; i < 4; i++) {
      world->setSignal(pk + i, !(val & (1 << (i & 0xf))));
    }

    FastLED.show();
  }

  if (digitalRead(FWBLOCK_PIN) == 0) {
    return;
  }
  
  unsigned long now = millis();
  world->test(now);

  if (ledsChanged) {
    FastLED.show();
    ledsChanged = false;
  }
}
