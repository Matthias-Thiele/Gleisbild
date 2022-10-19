#include <Arduino.h>
#include "./FastLed/FastLED.h"
#include "World.hpp"
#include "Telefon.hpp"
#include "DRS2.h"


#define FWBLOCK_PIN 4
#define TEST_PIN 8
#define TRAIN_START_LB 9
#define TRAIN_START_AH 10
#define TRAIN_START_WB 11

#define HEARTBEAT 0xd0


World *world;
Telefon *telefon;
bool drs2Available;

bool ledsChanged = false;


void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  pinMode(FWBLOCK_PIN, INPUT_PULLUP);
  pinMode(TEST_PIN, INPUT_PULLUP);

  delay( 1000 ); // power-up safety delay
  world = new World();
  telefon = new Telefon(TRAIN_START_LB, TRAIN_START_AH, TRAIN_START_WB);
  drs2Available = false;

  // init Signalmelder H und M
  Serial1.write(0xe0);
  Serial1.write(0xe1);
}

void loop() {
  static unsigned long nextCheckTrain = 0ul;
  unsigned long now = millis();
  telefon->tick(now);

  if (Serial3.available()) {
    uint8_t val = Serial3.read();
    //if (val != 0) {
      //Serial.print(" "); Serial.print(val);
      if (val == 80) {
        Serial.println();
      }
    //}

    world->processDrs2Command(now, val);
  }

  if (now > nextCheckTrain) {
    //Serial.print("Train start: "); Serial.println(telefon->getState());
    nextCheckTrain = now + 500ul;
    world->processCommand(0x30 | telefon->getState());

    Serial1.write(HEARTBEAT);
    Serial2.write(HEARTBEAT);
  }

  if(Serial1.available()) {
    uint8_t val = Serial1.read();
    world->processCommand(val);
  }

  if (Serial2.available()) {
    uint8_t val = Serial2.read();
    //Serial.print("Ser 2: "); Serial.println(val, HEX);
    world->processCommand(0x10 | val);
  }

   if (digitalRead(FWBLOCK_PIN) == 0) {
    return;
  }
  
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
