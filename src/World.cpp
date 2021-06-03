#include "World.hpp"
#include "Events.hpp"

#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

unsigned long dummyEventList[] = {0};
int testList[] = {FW_AH_T3, FW_WB_T2, FW_LB_T4, FW_T3_LB, FW_T2_AH, FW_LB_T1, FW_T1_WB, FW_LB_T3, FW_T4_AH, FW_WB_T4,
                  FW_T4_LB, FW_T3_AH, FW_WB_T1, FW_LB_T2, FW_AH_T4, FW_T1_LB, FW_T2_WB, FW_T4_WB, FW_AH_T2, FW_T2_LB, 
                  -1};

// Einfahrt von Bad Liebenzell
short lbefs[] = {315, 385, -1};
unsigned long lbefsev[] = {
  SET_SIGNAL | (8ul << 12) | 320ul,
  RESET_SIGNAL | (8ul << 12) | 335ul,
  STOP_TRAIN | 383ul,
  0
};

short lbt1[] = {315, 389, 204, 155, -1};
unsigned long lbt1234ev[] = {
  SET_SIGNAL | (8ul << 12) | 320ul,
  RESET_SIGNAL | (8ul << 12) | 335ul,
  SET_SIGNAL | (6ul << 12) | 370ul,
  RESET_SIGNAL | (6ul << 12) | 190ul,
  RESET_SIGNAL | (6ul << 12) | 56ul,
  STOP_TRAIN | 164ul,
  STOP_TRAIN | 94ul,
  STOP_TRAIN | 7ul,
  STOP_TRAIN | 441ul,
  0
};
short lbt2[] = {315, 389, 204, 203, 61, 55, 62, 103, -1};
short lbt3[] = {315, 389, 204, 203, 61, 48, 33, 3, -1};
short lbt4[] = {315, 389, 204, 203, 61, 43, 419, 450, -1};

// Einfahrt von Althengstett
short ahefs[] = {280, 207, -1};
unsigned long ahefsev[] = {
  SET_SIGNAL | (5ul << 12) | 275ul,
  RESET_SIGNAL | (5ul << 12) | 260ul,
  STOP_TRAIN | 211ul,
  0
};

short aht2[] = {280, 206, 391, 402, 48, 49, 33, 33, 42, 38, 73, 103, -1};
unsigned long aht234ev[] = {
  SET_SIGNAL | (5ul << 12) | 275ul,
  RESET_SIGNAL | (5ul << 12) | 260ul,
  SET_SIGNAL | (3ul << 12) | 220ul,
  RESET_SIGNAL | (3ul << 12) | 392ul,
  STOP_TRAIN | 94ul,
  STOP_TRAIN | 7ul,
  STOP_TRAIN | 441ul,
  0
};
short aht3[] = {280, 206, 391, 402, 48, 49, 33, 3, -1};
short aht4[] = {280, 206, 391, 393, 403, 450, -1};

// Einfahrt von Wildberg
short wbefs[] = {544, 558, 529, 473, -1};
unsigned long wbefsev[] = {
  SET_SIGNAL | (2ul << 12) | 544ul,
  RESET_SIGNAL | (2ul << 12) | 520ul,
  STOP_TRAIN | 475ul,
  0
};

short wbt1[] = {544, 558, 529, 469, 130, 180, -1};
unsigned long wbt124ev[] = {
  SET_SIGNAL | (2ul << 12) | 544ul,
  RESET_SIGNAL | (2ul << 12) | 520ul,
  SET_SIGNAL | (0ul << 12) | 490ul,
  RESET_SIGNAL | (0ul << 12) | 140ul,
  RESET_SIGNAL | (0ul << 12) | 110ul,
  RESET_SIGNAL | (0ul << 12) | 470ul,
  STOP_TRAIN | 170ul,
  STOP_TRAIN | 88ul,
  STOP_TRAIN | 435,
  0
};
short wbt2[] = {544, 558, 529, 469, 130, 125, 122, 78, -1};
short wbt4[] = {544, 558, 529, 469, 130, 125, 122, 119, 467, 425, -1};

// Ausfahrt nach Wildberg
short cwbt1[] = {169, 130, 469, 543, -1};
unsigned long cwbt1ev[] = {
  SET_SIGNAL | (9ul << 12) | 166ul,
  RESET_SIGNAL | (9ul << 12) | 135ul,
  SET_SIGNAL | (1ul << 12) | 500ul,
  RESET_SIGNAL | (1ul << 12) | 540ul,
  0
};

short cwbt2[] = {89, 122, 125, 129, 130, 130, 469, 543, -1};
unsigned long cwbt2ev[] = {
  SET_SIGNAL | (10ul << 12) | 91ul,
  RESET_SIGNAL | (10ul << 12) | 129ul,
  SET_SIGNAL | (1ul << 12) | 500ul,
  RESET_SIGNAL | (1ul << 12) | 540ul,
  0
};

short cwbt4[] = {436, 467, 119, 122, 125, 129, 130, 130, 469, 543, -1};
unsigned long cwbt4ev[] = {
  SET_SIGNAL | (11ul << 12) | 439ul,
  RESET_SIGNAL | (11ul << 12) | 460ul,
  SET_SIGNAL | (1ul << 12) | 500ul,
  RESET_SIGNAL | (1ul << 12) | 540ul,
  0
};

// Ausfahrt nach Liebenzell
short clbt1[] = {165, 204, 389, 329, 300, 314, -1};
unsigned long clbt1ev[] = {
  SET_SIGNAL | (15ul << 12) | 166ul,
  RESET_SIGNAL | (15ul << 12) | 190ul,
  SET_SIGNAL | (7ul << 12) | 350ul,
  RESET_SIGNAL | (7ul << 12) | 305ul,
  0
};

short clbt2[] = {93, 62, 56, 61, 203, 204, 389, 329, 300, 314, -1};
unsigned long clbt2ev[] = {
  SET_SIGNAL | (14ul << 12) | 88ul,
  RESET_SIGNAL | (14ul << 12) | 60ul,
  SET_SIGNAL | (7ul << 12) | 350ul,
  RESET_SIGNAL | (7ul << 12) | 305ul,
  0
};

short clbt3[] = {8, 33, 49, 61, 203, 204, 389, 329, 300, 314, -1};
unsigned long clbt3ev[] = {
  SET_SIGNAL | (13ul << 12) | 10ul,
  RESET_SIGNAL | (13ul << 12) | 30ul,
  SET_SIGNAL | (7ul << 12) | 350ul,
  RESET_SIGNAL | (7ul << 12) | 305ul,
  0
};

short clbt4[] = {440, 420, 43, 61, 203, 204, 389, 329, 300, 314, -1};
unsigned long clbt4ev[] = {
  SET_SIGNAL | (12ul << 12) | 430ul,
  RESET_SIGNAL | (12ul << 12) | 420ul,
  SET_SIGNAL | (7ul << 12) | 350ul,
  RESET_SIGNAL | (7ul << 12) | 305ul,
  0
};

// Ausfahrt nach Althengstett
short caht2[] = {93, 73, 38, 42, 33, 33, 48, 49, 402, 391, 206, 266, 295, 281, -1};
unsigned long caht2ev[] = {
  SET_SIGNAL | (14ul << 12) | 91ul,
  RESET_SIGNAL | (14ul << 12) | 73ul,
  SET_SIGNAL | (4ul << 12) | 250ul,
  RESET_SIGNAL | (4ul << 12) | 290ul,
  0
};

short caht3[] = {8, 33, 48, 49, 402, 391, 206, 266, 295, 281, -1};
unsigned long caht3ev[] = {
  SET_SIGNAL | (13ul << 12) | 10ul,
  RESET_SIGNAL | (13ul << 12) | 25ul,
  SET_SIGNAL | (4ul << 12) | 250ul,
  RESET_SIGNAL | (4ul << 12) | 290ul,
  0
};

short caht4[] = {440, 403, 393, 391, 206, 266, 295, 281, -1};
unsigned long caht4ev[] = {
  SET_SIGNAL | (12ul << 12) | 438ul,
  RESET_SIGNAL | (12ul << 12) | 410ul,
  SET_SIGNAL | (4ul << 12) | 250ul,
  RESET_SIGNAL | (4ul << 12) | 290ul,
  0
};

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
  fahrwege[FW_LB_EFS]->set(lbefs, lbefsev, true);

  // Liebenzell auf Gleis 1
  fahrwege[FW_LB_T1]->set(lbt1, lbt1234ev, true);

  // Liebenzell auf Gleis 2
  fahrwege[FW_LB_T2]->set(lbt2, lbt1234ev, true);

  // Liebenzell auf Gleis 3
  fahrwege[FW_LB_T3]->set(lbt3, lbt1234ev, true);

  // Liebenzell auf Gleis 4
  fahrwege[FW_LB_T4]->set(lbt4, lbt1234ev, true);

  // Althengstett bis zum Einfahrtsignal
  fahrwege[FW_AH_EFS]->set(ahefs, ahefsev, true);

  // Althengstett auf Gleis 2
  fahrwege[FW_AH_T2]->set(aht2, aht234ev, true);

  // Althengstett auf Gleis 3
  fahrwege[FW_AH_T3]->set(aht3, aht234ev, true);

  // Althengstett auf Gleis 4
  fahrwege[FW_AH_T4]->set(aht4, aht234ev, true);

  // Wildberg bis zum Einfahrtsignal
  fahrwege[FW_WB_EFS]->set(wbefs, wbefsev, true);

  // Wildberg auf Gleis 1
  fahrwege[FW_WB_T1]->set(wbt1, wbt124ev, true);

  // Wildberg auf Gleis 2
  fahrwege[FW_WB_T2]->set(wbt2, wbt124ev, true);

  // Wildberg auf Gleis 4
  fahrwege[FW_WB_T4]->set(wbt4, wbt124ev, true);

  // Nach Wildberg von Gleis 1
  fahrwege[FW_T1_WB]->set(cwbt1, cwbt1ev, false);

  // Nach Wildberg von Gleis 2
  fahrwege[FW_T2_WB]->set(cwbt2, cwbt2ev, false);

  // Nach Wildberg von Gleis 4
  fahrwege[FW_T4_WB]->set(cwbt4, cwbt4ev, false);

  // Nach Liebenzell von Gleis 1
  fahrwege[FW_T1_LB]->set(clbt1, clbt1ev, false);

  // Nach Liebenzell von Gleis 2
  fahrwege[FW_T2_LB]->set(clbt2, clbt2ev, false);

  // Nach Liebenzell von Gleis 3
  fahrwege[FW_T3_LB]->set(clbt3, clbt3ev, false);

  // Nach Liebenzell von Gleis 4
  fahrwege[FW_T4_LB]->set(clbt4, clbt4ev, false);

  // Nach Althengstett von Gleis 2
  fahrwege[FW_T2_AH]->set(caht2, caht2ev, false);

  // Nach Althengstett von Gleis 3
  fahrwege[FW_T3_AH]->set(caht3, caht3ev, false);

  // Nach Althengstett von Gleis 4
  fahrwege[FW_T4_AH]->set(caht4, caht4ev, false);

 }

void World::setSignal(int num, bool value) {
  if (value) {
    signals[num].set();
  } else {
    signals[num].release();
  }
}

int activeFW = -1;
unsigned long lastStep = 0;
int activeSg = 0;

void World::test(unsigned long now) {
  if (now < lastStep) {
    return;
  }

  if (activeFW >= 0) {
    if (fahrwege[testList[activeFW]]->done()) {
      Serial.println("Fahrstrasse done.");
      delay(1000);
      //fahrwege[testList[activeFW]]->clear();
      activeFW++;
      if (testList[activeFW] < 0) {
        activeFW = 0;
      }
      Serial.print("Show Fahrstrasse "); Serial.println(testList[activeFW]);
      fahrwege[testList[activeFW]]->show();
      Serial.println("Show done.");
    }
  }

  if (activeFW < 0) {
    activeFW = 0;
    fahrwege[testList[activeFW]]->show();
  }

  //Serial.println("Advance");
  fahrwege[testList[activeFW]]->advance();
  lastStep = now + 50;
}