#include "World.hpp"
#include "Events.hpp"

#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

unsigned long dummyEventList[] = {0};

// Einfahrt von Bad Liebenzell
short lbefs[] = {315, 385, -1};
unsigned long lbefsev[] = {
  SET_SIGNAL | (8ul << 12) | 320ul,
  RESET_SIGNAL | (8ul << 12) | 335ul,
  0
};

short lbt1[] = {315, 389, 204, 165, -1};
unsigned long lbt1234ev[] = {
  SET_SIGNAL | (8ul << 12) | 320ul,
  RESET_SIGNAL | (8ul << 12) | 335ul,
  SET_SIGNAL | (6ul << 12) | 370ul,
  RESET_SIGNAL | (6ul << 12) | 190ul,
  RESET_SIGNAL | (6ul << 12) | 56ul,
  0
};
short lbt2[] = {315, 389, 204, 203, 61, 55, 62, 93, -1};
short lbt3[] = {315, 389, 204, 203, 61, 48, 33, 8, -1};
short lbt4[] = {315, 389, 204, 203, 61, 43, 420, 441, -1};

// Einfahrt von Althengstett
short ahefs[] = {280, 211, -1};
unsigned long ahefsev[] = {
  SET_SIGNAL | (5ul << 12) | 275ul,
  RESET_SIGNAL | (5ul << 12) | 260ul,
  0
};
short aht2[] = {280, 206, 391, 402, 48, 49, 33, 33, 42, 38, 73, 93, -1};
unsigned long aht234ev[] = {
  SET_SIGNAL | (5ul << 12) | 275ul,
  RESET_SIGNAL | (5ul << 12) | 260ul,
  SET_SIGNAL | (3ul << 12) | 220ul,
  RESET_SIGNAL | (3ul << 12) | 392ul,
  0
};
short aht3[] = {280, 206, 391, 402, 48, 49, 33, 8, -1};
short aht4[] = {280, 206, 391, 393, 403, 441, -1};

World::World() {
  CRGB clearColor = {0, 0, 0};
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = clearColor;
  }

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  initSignals();
  initFahrstrassen();
}


void World::initSignals() {
  Signal::setLeds(leds);
  signals[0].setLedNo(468); // Einf. Calw von WB
  signals[1].setLedNo(559); // Einf. WB von Calw
  signals[2].setLedNo(560); // Ausf. WB nach Calw
  signals[3].setLedNo(205); // Einf. Calw von AH
  signals[4].setLedNo(296); // Einf. AH von Calw
  signals[5].setLedNo(297); // Ausf. AH nach Calw
  signals[6].setLedNo(390); // Einf. Calw von LB
  signals[7].setLedNo(298); // Einf. LB von Calw
  signals[8].setLedNo(299); // Ausf. LB nach Calw
  signals[9].setLedNo(0);   // Ausf. G1 nach WB
  signals[10].setLedNo(1);  // Ausf. G2 nach WB
  signals[11].setLedNo(2);  // Ausf. G4 nach WB
  signals[12].setLedNo(34); // Ausf. G4 nach LB/ AH
  signals[13].setLedNo(35); // Ausf. G3 nach LB/ AH
  signals[14].setLedNo(36); // Ausf. G2 nach LB/ AH
  signals[15].setLedNo(37); // Ausf. G1 nach LB

  for (int i = 0; i < NUM_SIGNALS; i++) {
    signals[i].release();
  }
}

void World::initFahrstrassen() {
  Fahrweg::setSignals(signals);

  CRGB trainColor = {0, 0, 0xff};
  CRGB trackColor = {0x20, 0x20, 0x20};
  for (int i = 0; i < NUM_FAHRWEGE; i++) {
    fahrwege[i] = new Fahrweg(leds, trainColor, trackColor);
  }

  // Liebenzell bis zum Einfahrtsignal
  fahrwege[FW_LB_EFS]->set(lbefs, lbefsev);

  // Liebenzell auf Gleis 1
  fahrwege[FW_LB_T1]->set(lbt1, lbt1234ev);

  // Liebenzell auf Gleis 2
  fahrwege[FW_LB_T2]->set(lbt2, lbt1234ev);

  // Liebenzell auf Gleis 3
  fahrwege[FW_LB_T3]->set(lbt3, lbt1234ev);

  // Liebenzell auf Gleis 4
  fahrwege[FW_LB_T4]->set(lbt4, lbt1234ev);

  // Althengstett bis zum Einfahrtsignal
  fahrwege[FW_AH_EFS]->set(ahefs, ahefsev);

  // Althengstett auf Gleis 2
  fahrwege[FW_AH_T2]->set(aht2, aht234ev);

  // Althengstett auf Gleis 3
  fahrwege[FW_AH_T3]->set(aht3, aht234ev);

  // Althengstett auf Gleis 4
  fahrwege[FW_AH_T4]->set(aht4, aht234ev);
}

int activeFW = -1;
unsigned long lastStep = 0;
int activeSg = 0;

void World::test(unsigned long now) {
  if (now < lastStep) {
    return;
  }

  if (activeFW >= 0) {
    if (fahrwege[activeFW]->done()) {
      Serial.println("Fahrstrasse done.");
      delay(1000);
      fahrwege[activeFW]->clear();
      activeFW++;
      if (activeFW > 8) {
        activeFW = 0;
      }
      Serial.print("Show Fahrstrasse "); Serial.println(activeFW);
      fahrwege[activeFW]->show();
    }
  }

  if (activeFW < 0) {
    activeFW = 5;
    fahrwege[activeFW]->show();
  }

  //Serial.println("Advance");
  fahrwege[activeFW]->advance();
  lastStep = now + 60;
}