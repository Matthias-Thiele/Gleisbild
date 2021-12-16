#include "World.hpp"
#include "Events.hpp"
#include "DRS2.h"

#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

extern bool ledsChanged;
extern bool drs2Available;

CRGB trainColor = {0, 0, 0xff};
CRGB pauseColor = {0, 0xff, 0};
CRGB trackColor = {0x20, 0x20, 0x20};
CRGB occupiedColor = {0x40, 0x40, 0x0};


unsigned long dummyEventList[] = {0};
int testList[] = {FW_AH_T3, FW_WB_T2, FW_LB_T4, FW_T3_LB, FW_T2_AH, FW_LB_T1, FW_T1_WB, FW_LB_T3, FW_T4_AH, FW_WB_T4,
                  FW_T4_LB, FW_T3_AH, FW_WB_T1, FW_LB_T2, FW_AH_T4, FW_T1_LB, FW_T2_WB, FW_T4_WB, FW_AH_T2, FW_T2_LB, 
                  -1};

// Einfahrt von Bad Liebenzell
short lbefs[] = {315, 385, -1};
unsigned long lbefsev[] = {
  FIELD_ENTER | (EXIT_LB_AUSF_GLEIS_123 << 12) | 315,
  BLOCK_FIELD | BLOCK_LB | 319ul,
  SET_SIGNAL | (11ul << 12) | 320ul,
  WAIT_FOR_SIGNAL | (11ul << 12) | 320ul,
  RESET_SIGNAL | (11ul << 12) | 335ul,
  BLOCK_FIELD | BLOCK_END_SET_LB | 336ul,
  WAIT_FOR_SIGNAL | (0ul << 12) | 383ul,
  0
};

//short lbt1[] = {315, 389, 204, 155, -1};
short lbt1[] = {315, 389, 204, 130, 469, 472, -1};
unsigned long lbt1234ev[] = {
  FIELD_ENTER | (EXIT_LB_AUSF_GLEIS_123 << 12) | 315,
  BLOCK_FIELD | BLOCK_LB | 319ul,
  SET_SIGNAL | (11ul << 12) | 320ul,
  WAIT_FOR_SIGNAL | (11ul << 12) | 320ul,
  FIELD_ENTER | (EXIT_LB_AUSF_WEICHE_2 << 12) | 321,
  FIELD_ENTER | (EXIT_LB_AUSF_WEICHE_1 << 12) | 327,
  FIELD_ENTER | (EXIT_LB_AUSF_SIG_3 << 12) | 335,
  RESET_SIGNAL | (11ul << 12) | 335ul,
  BLOCK_FIELD | BLOCK_END_SET_LB | 336ul,
  FIELD_ENTER | (EXIT_LB_AUSF_SIG_2 << 12) | 350,
  FIELD_ENTER | (EXIT_LB_AUSF_SIG_1 << 12) | 365,
  WAIT_FOR_SIGNAL | (0ul << 12) | 383ul,
  OCCUPANCY | OCCUP_LB_ON | 385,
  OCCUPANCY | OCCUP_LB_OFF | 389,
  FIELD_ENTER | (STRECKE_LB_CALW << 12) | 389,
  SET_SIGNAL | (0ul << 12) | 370ul | ONLY_TEST,
  RESET_SIGNAL | (0ul << 12) | 190ul | ONLY_TEST,
  RESET_SIGNAL | (0ul << 12) | 56ul | ONLY_TEST,
  TRACK | TRACK_ALLOCATE | 165ul,
  STOP_TRAIN_LB | 164ul,
  TRACK | TRACK_ALLOCATE | 94ul,
  STOP_TRAIN_LB | 94ul,
  OCCUPANCY | OCCUP_T3_ON | 20ul,
  OCCUPANCY | OCCUP_T3_OFF | 15ul,
  TRACK | TRACK_ALLOCATE | 7ul,
  STOP_TRAIN_LB | 7ul,
  TRACK | TRACK_ALLOCATE | 441ul,
  STOP_TRAIN_LB | 441ul,
  0
};

short lbt2[] = {315, 389, 204, 203, 61, 55, 62, 122, 125, 130, 469, 472, -1};
short lbt3[] = {315, 389, 204, 203, 61, 48, 33, 3, -1};
short lbt4[] = {315, 389, 204, 203, 61, 43, 419, 467, 119, 122, 125, 130, 469, 472, -1};

// Einfahrt von Althengstett
short ahefs[] = {280, 207, -1};
unsigned long ahefsev[] = {
  BLOCK_FIELD | BLOCK_AH | 275ul,
  SET_SIGNAL | (13ul << 12) | 275ul,
  RESET_SIGNAL | (13ul << 12) | 260ul,
  BLOCK_FIELD | BLOCK_END_SET_AH | 259ul,
  WAIT_FOR_SIGNAL | (1ul << 12) | 212ul,
  0
};

short aht2[] = {280, 206, 391, 402, 48, 49, 33, 33, 42, 38, 73, 122, 125, 130, 469, 472, -1};
unsigned long aht234ev[] = {
  BLOCK_FIELD | BLOCK_AH | 275ul,
  SET_SIGNAL | (13ul << 12) | 275ul,
  RESET_SIGNAL | (13ul << 12) | 260ul,
  BLOCK_FIELD | BLOCK_END_SET_AH | 259ul,
  OCCUPANCY | OCCUP_AH_ON | 210,
  OCCUPANCY | OCCUP_AH_OFF | 206,
  WAIT_FOR_SIGNAL | (1ul << 12) | 212ul,
  SET_SIGNAL | (1ul << 12) | 220ul | ONLY_TEST,
  RESET_SIGNAL | (1ul << 12) | 392ul | ONLY_TEST,
  TRACK | TRACK_ALLOCATE | 94ul,
  STOP_TRAIN_AH | 94ul,
  OCCUPANCY | OCCUP_T3_ON | 20ul,
  OCCUPANCY | OCCUP_T3_OFF | 15ul,
  TRACK | TRACK_ALLOCATE | 7ul,
  STOP_TRAIN_AH | 7ul,
  TRACK | TRACK_ALLOCATE | 441ul,
  STOP_TRAIN_AH | 441ul,
  0
};
short aht3[] = {280, 206, 391, 402, 48, 49, 33, 3, -1};
short aht4[] = {280, 206, 391, 393, 403, 467, 119, 122, 125, 130, 469, 472, -1};

// Einfahrt von Wildberg
short wbefs[] = {544, 558, 529, 473, -1};
unsigned long wbefsev[] = {
  BLOCK_FIELD | BLOCK_WB | 549ul,
  SET_SIGNAL | (15ul << 12) | 549ul,
  RESET_SIGNAL | (15ul << 12) | 520ul,
  BLOCK_FIELD | BLOCK_END_SET_WB | 519ul,
  WAIT_FOR_SIGNAL | (2ul << 12) | 474ul,
  0
};

short wbt1[] = {544, 558, 529, 469, 130, 204, 389, 386, -1};
unsigned long wbt1ev[] = {
  BLOCK_FIELD | BLOCK_WB | 548ul,
  SET_SIGNAL | (15ul << 12) | 548ul,
  RESET_SIGNAL | (15ul << 12) | 520ul,
  BLOCK_FIELD | BLOCK_END_SET_WB | 519ul,
  SET_SIGNAL | (2ul << 12) | 490ul | ONLY_TEST,
  WAIT_FOR_SIGNAL | (2ul << 12) | 474ul,
  OCCUPANCY | OCCUP_WB_ON | 473,
  OCCUPANCY | OCCUP_WB_OFF | 469,
  RESET_SIGNAL | (2ul << 12) | 170ul,
  BLOCK_FIELD | BLOCK_END_RESET_WB | 169ul,
  TRACK | TRACK_ALLOCATE | 170ul,
  STOP_TRAIN_WB1 | 170ul,
  BEFEHLSABGABE_UNBLOCK_M1 | 170ul,
  0
};

short wbt2[] = {544, 558, 529, 469, 130, 125, 122, 78, -1};
short wbt4[] = {544, 558, 529, 469, 130, 125, 122, 119, 467, 425, -1};
unsigned long wbt24ev[] = {
  BLOCK_FIELD | BLOCK_WB | 548ul,
  SET_SIGNAL | (15ul << 12) | 548ul,
  RESET_SIGNAL | (15ul << 12) | 520ul,
  BLOCK_FIELD | BLOCK_END_SET_WB | 519ul,
  SET_SIGNAL | (2ul << 12) | 490ul | ONLY_TEST,
  WAIT_FOR_SIGNAL | (2ul << 12) | 474ul,
  OCCUPANCY | OCCUP_WB_ON | 473,
  OCCUPANCY | OCCUP_WB_OFF | 469,
  RESET_SIGNAL | (2ul << 12) | 89ul,
  RESET_SIGNAL | (2ul << 12) | 436ul,
  BLOCK_FIELD | BLOCK_END_RESET_WB | 89ul,
  BLOCK_FIELD | BLOCK_END_RESET_WB | 436ul,
  TRACK | TRACK_ALLOCATE | 88ul,
  BEFEHLSABGABE_UNBLOCK_M24 | 88ul,
  STOP_TRAIN_WB24 | 88ul,
  TRACK | TRACK_ALLOCATE | 435ul,
  BEFEHLSABGABE_UNBLOCK_M24 | 435ul,
  STOP_TRAIN_WB24 | 435ul,
  0
};

// Ausfahrt nach Wildberg
short cwbt1[] = {169, 130, 469, 543, -1};
unsigned long cwbt1ev[] = {
  BLOCK_FIELD | BLOCK_WB | BLOCK_IS_REMOTE | 165ul,
  WAIT_FOR_SIGNAL | (7ul << 12) | 165ul,
  SET_SIGNAL | (7ul << 12) | 166ul | ONLY_TEST,
  BLOCK_FIELD | BEFEHLSABGABE_UNBLOCK_H | 475,
  OCCUPANCY | OCCUP_WB_ON | 469,
  OCCUPANCY | OCCUP_WB_OFF | 473,
  RESET_SIGNAL | (7ul << 12) | 473,
  BLOCK_FIELD | BLOCK_START_SET_WB | 473ul,
  STOP_TRAIN_WBOUT | 477ul,
  SET_SIGNAL | (14ul << 12) | 500ul,
  RESET_SIGNAL | (14ul << 12) | 543ul,
  BLOCK_FIELD | BLOCK_START_RESET_WB | 542ul,
  STOP_TRAIN_ARRIVAL | 543ul,
  0
};

short cwbt2[] = {89, 122, 125, 129, 130, 130, 469, 543, -1};
unsigned long cwbt2ev[] = {
  BLOCK_FIELD | BLOCK_WB | BLOCK_IS_REMOTE | 93ul,
  WAIT_FOR_SIGNAL | (8ul << 12) | 93ul,
  SET_SIGNAL | (8ul << 12) | 91ul | ONLY_TEST,
  STOP_TRAIN_WBOUT | 477ul,
  BLOCK_FIELD | BEFEHLSABGABE_UNBLOCK_JK | 475,
  OCCUPANCY | OCCUP_WB_ON | 469,
  OCCUPANCY | OCCUP_WB_OFF | 473,
  RESET_SIGNAL | (8ul << 12) | 473,
  BLOCK_FIELD | BLOCK_START_SET_WB | 473ul,
  SET_SIGNAL | (14ul << 12) | 500ul,
  RESET_SIGNAL | (14ul << 12) | 543ul,
  BLOCK_FIELD | BLOCK_START_RESET_WB | 542ul,
  STOP_TRAIN_ARRIVAL | 543,
  0
};

short cwbt4[] = {436, 467, 119, 122, 125, 129, 130, 130, 469, 543, -1};
unsigned long cwbt4ev[] = {
  BLOCK_FIELD | BLOCK_WB | BLOCK_IS_REMOTE | 440ul,
  WAIT_FOR_SIGNAL | (9ul << 12) | 440ul,
  SET_SIGNAL | (9ul << 12) | 439ul | ONLY_TEST,
  BLOCK_FIELD | BEFEHLSABGABE_UNBLOCK_JK | 475,
  OCCUPANCY | OCCUP_WB_ON | 469,
  OCCUPANCY | OCCUP_WB_OFF | 473,
  RESET_SIGNAL | (9ul << 12) | 473,
  BLOCK_FIELD | BLOCK_START_SET_WB | 473ul,
  STOP_TRAIN_WBOUT | 477ul,
  SET_SIGNAL | (14ul << 12) | 500ul,
  RESET_SIGNAL | (14ul << 12) | 543ul,
  BLOCK_FIELD | BLOCK_START_RESET_WB | 542ul,
  STOP_TRAIN_ARRIVAL | 543,
  0
};

// Ausfahrt nach Liebenzell
short clbt1[] = {165, 204, 389, 329, 300, 314, -1};
unsigned long clbt1ev[] = {
  WAIT_FOR_SIGNAL | (3ul << 12) | 165ul,
  SET_SIGNAL | (3ul << 12) | 166ul | ONLY_TEST,
  RESET_SIGNAL | (3ul << 12) | 190ul | ONLY_TEST,
  OCCUPANCY | OCCUP_LB_ON | 389,
  OCCUPANCY | OCCUP_LB_OFF | 385,
  TRACK | TRACK_RELEASE | 166ul,
  FIELD_ENTER | (ENTER_LB_EINF_SIG_1 << 12) | 389,
  FIELD_ENTER | (ENTER_LB_EINF_SIG_2 << 12) | 365,
  FIELD_ENTER | (ENTER_LB_EINF_SIG_3 << 12) | 350,
  FIELD_ENTER | (ENTER_LB_EINF_WEICHE_1 << 12) | 335,
  FIELD_ENTER | (ENTER_LB_EINF_WEICHE_2 << 12) | 305,
  FIELD_ENTER | (ENTER_LB_EINF_GLEIS_123 << 12) | 313,
  BLOCK_FIELD | BLOCK_WAIT_IF_FREE | 360,
  SET_SIGNAL | (10ul << 12) | 340ul,
  RESET_SIGNAL | (10ul << 12) | 314ul,
  BLOCK_FIELD | BLOCK_START_RESET_LB | 314ul,
  STOP_TRAIN_ARRIVAL | 314,
  0
};

short clbt2[] = {93, 62, 56, 61, 203, 204, 389, 329, 300, 314, -1};
unsigned long clbt2ev[] = {
  WAIT_FOR_SIGNAL | (4ul << 12) | 93ul,
  SET_SIGNAL | (4ul << 12) | 88ul | ONLY_TEST,
  RESET_SIGNAL | (4ul << 12) | 60ul | ONLY_TEST,
  OCCUPANCY | OCCUP_LB_ON | 389,
  OCCUPANCY | OCCUP_LB_OFF | 385,
  FIELD_ENTER | (ENTER_LB_EINF_SIG_1 << 12) | 389,
  FIELD_ENTER | (ENTER_LB_EINF_SIG_2 << 12) | 365,
  FIELD_ENTER | (ENTER_LB_EINF_SIG_3 << 12) | 350,
  FIELD_ENTER | (ENTER_LB_EINF_WEICHE_1 << 12) | 335,
  FIELD_ENTER | (ENTER_LB_EINF_WEICHE_2 << 12) | 305,
  FIELD_ENTER | (ENTER_LB_EINF_GLEIS_123 << 12) | 313,
  BLOCK_FIELD | BLOCK_WAIT_IF_FREE | 360,
  SET_SIGNAL | (10ul << 12) | 340ul,
  RESET_SIGNAL | (10ul << 12) | 314ul,
  BLOCK_FIELD | BLOCK_START_RESET_LB | 314ul,
  STOP_TRAIN_ARRIVAL | 314,
  0
};

short clbt3[] = {8, 33, 49, 61, 203, 204, 389, 329, 300, 314, -1};
unsigned long clbt3ev[] = {
  WAIT_FOR_SIGNAL | (5ul << 12) | 8ul,
  SET_SIGNAL | (5ul << 12) | 10ul | ONLY_TEST,
  RESET_SIGNAL | (5ul << 12) | 30ul | ONLY_TEST,
  OCCUPANCY | OCCUP_LB_ON | 389,
  OCCUPANCY | OCCUP_LB_OFF | 385,
  BLOCK_FIELD | BLOCK_WAIT_IF_FREE | 360,
  SET_SIGNAL | (10ul << 12) | 340ul,
  RESET_SIGNAL | (10ul << 12) | 314ul,
  BLOCK_FIELD | BLOCK_START_RESET_LB | 314ul,
  STOP_TRAIN_ARRIVAL | 314,
  0
};

short clbt4[] = {440, 420, 43, 61, 203, 204, 389, 329, 300, 314, -1};
unsigned long clbt4ev[] = {
  WAIT_FOR_SIGNAL | (6ul << 12) | 440ul,
  SET_SIGNAL | (6ul << 12) | 430ul | ONLY_TEST,
  RESET_SIGNAL | (6ul << 12) | 420ul | ONLY_TEST,
  OCCUPANCY | OCCUP_LB_ON | 389,
  OCCUPANCY | OCCUP_LB_OFF | 385,
  FIELD_ENTER | (ENTER_LB_EINF_SIG_1 << 12) | 389,
  FIELD_ENTER | (ENTER_LB_EINF_SIG_2 << 12) | 365,
  FIELD_ENTER | (ENTER_LB_EINF_SIG_3 << 12) | 350,
  FIELD_ENTER | (ENTER_LB_EINF_WEICHE_1 << 12) | 335,
  FIELD_ENTER | (ENTER_LB_EINF_WEICHE_2 << 12) | 305,
  FIELD_ENTER | (ENTER_LB_EINF_GLEIS_123 << 12) | 313,
  BLOCK_FIELD | BLOCK_WAIT_IF_FREE | 360,
  SET_SIGNAL | (10ul << 12) | 340ul,
  RESET_SIGNAL | (10ul << 12) | 314ul,
  BLOCK_FIELD | BLOCK_START_RESET_LB | 314ul,
  STOP_TRAIN_ARRIVAL | 314,
  0
};

// Ausfahrt nach Althengstett
short caht2[] = {93, 73, 38, 42, 33, 33, 48, 49, 402, 391, 206, 266, 295, 281, -1};
unsigned long caht2ev[] = {
  WAIT_FOR_SIGNAL | (4ul << 12) | 93ul,
  SET_SIGNAL | (4ul << 12) | 91ul | ONLY_TEST,
  RESET_SIGNAL | (4ul << 12) | 392ul | ONLY_TEST,
  TRACK | TRACK_RELEASE | 392ul,
  OCCUPANCY | OCCUP_AH_ON | 206,
  OCCUPANCY | OCCUP_AH_OFF | 210,
  BLOCK_FIELD | BLOCK_WAIT_IF_FREE | 230,
  SET_SIGNAL | (12ul << 12) | 250ul,
  RESET_SIGNAL | (12ul << 12) | 290ul,
  BLOCK_FIELD | BLOCK_START_RESET_AH | 288ul,
  STOP_TRAIN_ARRIVAL | 281,
  0
};

short caht3[] = {8, 33, 48, 49, 402, 391, 206, 266, 295, 281, -1};
unsigned long caht3ev[] = {
  WAIT_FOR_SIGNAL | (5ul << 12) | 8ul,
  SET_SIGNAL | (5ul << 12) | 10ul | ONLY_TEST,
  RESET_SIGNAL | (5ul << 12) | 392ul | ONLY_TEST,
  TRACK | TRACK_RELEASE | 392ul,
  OCCUPANCY | OCCUP_AH_ON | 206,
  OCCUPANCY | OCCUP_AH_OFF | 210,
  BLOCK_FIELD | BLOCK_WAIT_IF_FREE | 230,
  SET_SIGNAL | (12ul << 12) | 250ul,
  RESET_SIGNAL | (12ul << 12) | 290ul,
  BLOCK_FIELD | BLOCK_START_RESET_AH | 288ul,
  STOP_TRAIN_ARRIVAL | 281,
  0
};

short caht4[] = {440, 403, 393, 391, 206, 266, 295, 281, -1};
unsigned long caht4ev[] = {
  WAIT_FOR_SIGNAL | (6ul << 12) | 440ul,
  SET_SIGNAL | (6ul << 12) | 438ul | ONLY_TEST,
  RESET_SIGNAL | (6ul << 12) | 392ul | ONLY_TEST,
  TRACK | TRACK_RELEASE | 392ul,
  OCCUPANCY | OCCUP_AH_ON | 206,
  OCCUPANCY | OCCUP_AH_OFF | 210,
  BLOCK_FIELD | BLOCK_WAIT_IF_FREE | 230,
  SET_SIGNAL | (12ul << 12) | 250ul,
  RESET_SIGNAL | (12ul << 12) | 290ul,
  BLOCK_FIELD | BLOCK_START_RESET_AH | 288ul,
  STOP_TRAIN_ARRIVAL | 281,
  0
};

World::World() {
  m_Pause = false;
  CRGB clearColor = {0, 0, 0};
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = clearColor;
  }

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  initSignals();
  initFahrstrassen();
}

void World::send(uint8_t value) {
  //Serial.print("Send: "); Serial.println(value, HEX);
  Serial1.write(value);
  Serial2.write(value);
}

void World::initSignals() {
  Signal::setLeds(leds);
  signals[0].setLedNo(390); // Einf. Calw von LB
  signals[0].setMelder(1);
  signals[1].setLedNo(205); // Einf. Calw von AH
  signals[1].setMelder(2);
  signals[2].setLedNo(468); // Einf. Calw von WB
  signals[2].setMelder(3);
  signals[3].setLedNo(37); // Ausf. G1 nach LB
  signals[4].setLedNo(36); // Ausf. G2 nach LB/ AH
  signals[5].setLedNo(35); // Ausf. G3 nach LB/ AH
  signals[6].setLedNo(34); // Ausf. G4 nach LB/ AH
  signals[7].setLedNo(0);   // Ausf. G1 nach WB
  signals[8].setLedNo(1);  // Ausf. G2 nach WB
  signals[9].setLedNo(2);  // Ausf. G4 nach WB
  signals[10].setLedNo(298); // Einf. LB von Calw
  signals[11].setLedNo(299); // Ausf. LB nach Calw
  signals[12].setLedNo(296); // Einf. AH von Calw
  signals[13].setLedNo(297); // Ausf. AH nach Calw
  signals[14].setLedNo(559); // Einf. WB von Calw
  signals[15].setLedNo(560); // Ausf. WB nach Calw

  for (int i = 0; i < NUM_SIGNALS; i++) {
    signals[i].release();
  }
}

void World::initFahrstrassen() {
  Fahrweg::setSignals(signals);
  for (int i = 0; i < 4; i++) {
    trackTrains[i].init(leds, trainColor, trackColor, occupiedColor);
  }

  for (int i = 0; i < NUM_FAHRWEGE; i++) {
    fahrwege[i] = new Fahrweg(leds, trainColor, trackColor, occupiedColor);
    fahrwege[i]->setTrains(trackTrains);
  }

  // Liebenzell bis zum Einfahrtsignal
  fahrwege[FW_LB_EFS]->set(lbefs, lbefsev, 0xff, true);

  // Liebenzell auf Gleis 1
  fahrwege[FW_LB_T1]->set(lbt1, lbt1234ev, 0, true);

  // Liebenzell auf Gleis 2
  fahrwege[FW_LB_T2]->set(lbt2, lbt1234ev, 1, true);

  // Liebenzell auf Gleis 3
  fahrwege[FW_LB_T3]->set(lbt3, lbt1234ev, 2, true);

  // Liebenzell auf Gleis 4
  fahrwege[FW_LB_T4]->set(lbt4, lbt1234ev, 3, true);

  // Althengstett bis zum Einfahrtsignal
  fahrwege[FW_AH_EFS]->set(ahefs, ahefsev, 0xff, true);

  // Althengstett auf Gleis 2
  fahrwege[FW_AH_T2]->set(aht2, aht234ev, 1, true);

  // Althengstett auf Gleis 3
  fahrwege[FW_AH_T3]->set(aht3, aht234ev, 2, true);

  // Althengstett auf Gleis 4
  fahrwege[FW_AH_T4]->set(aht4, aht234ev, 3, true);

  // Wildberg bis zum Einfahrtsignal
  fahrwege[FW_WB_EFS]->set(wbefs, wbefsev, 0xff, true);

  // Wildberg auf Gleis 1
  fahrwege[FW_WB_T1]->set(wbt1, wbt1ev, 0, true);

  // Wildberg auf Gleis 2
  fahrwege[FW_WB_T2]->set(wbt2, wbt24ev, 1, true);

  // Wildberg auf Gleis 4
  fahrwege[FW_WB_T4]->set(wbt4, wbt24ev, 3, true);

  // Nach Wildberg von Gleis 1
  fahrwege[FW_T1_WB]->set(cwbt1, cwbt1ev, 0, false);

  // Nach Wildberg von Gleis 2
  fahrwege[FW_T2_WB]->set(cwbt2, cwbt2ev, 1, false);

  // Nach Wildberg von Gleis 4
  fahrwege[FW_T4_WB]->set(cwbt4, cwbt4ev, 3, false);

  // Nach Liebenzell von Gleis 1
  fahrwege[FW_T1_LB]->set(clbt1, clbt1ev, 0, false);

  // Nach Liebenzell von Gleis 2
  fahrwege[FW_T2_LB]->set(clbt2, clbt2ev, 1, false);

  // Nach Liebenzell von Gleis 3
  fahrwege[FW_T3_LB]->set(clbt3, clbt3ev, 2, false);

  // Nach Liebenzell von Gleis 4
  fahrwege[FW_T4_LB]->set(clbt4, clbt4ev, 3, false);

  // Nach Althengstett von Gleis 2
  fahrwege[FW_T2_AH]->set(caht2, caht2ev, 1, false);

  // Nach Althengstett von Gleis 3
  fahrwege[FW_T3_AH]->set(caht3, caht3ev, 2,false);

  // Nach Althengstett von Gleis 4
  fahrwege[FW_T4_AH]->set(caht4, caht4ev, 3, false);

 }

void World::processDrs2Command(unsigned long now, uint8_t cmd) {
  Serial.print("DRS2 command "); Serial.println(cmd);
  
  switch (cmd) {
    case DRS2_IS_ALIVE:
      Serial3.write(DRS2HB);
      drs2Available = true;
      m_DecativateDrs2 = now + 10000;
      break;

    case DRS2_ANF_ENTBLOCKEN:
      Serial1.write(0xd2);
      break;

    case DRS2_ERL_ENTBLOCKEN:
      Serial1.write(0xd1);
      break;

    case DRS2_END_ENTBLOCKEN:
      Serial1.write(0xd3);
      break;

    case DRS2_SIGA_FAHRT:
    case DRS2_ERSATZA:
      signals[SIG_EINF_LB].set();
      break;

    case DRS2_SIGA_HALT:
      signals[SIG_EINF_LB].release();
      break;

    case DRS2_SIGBCD_FAHRT:
    case DRS2_ERSATZBCD:
      signals[SIG_AUSF_LB].set();
      break;

    case DRS2_SIGBCD_HALT:
      signals[SIG_AUSF_LB].release();
      break;

    case DRS2_AUSFAHRT_GESETZT:
      startTrain(0);
      break;
  }
}

void World::processCommand(uint8_t cmd) {
  if (m_isTestMode) {
    return;
  }

  bool actVal;
  uint8_t pk = (cmd >> 4) & 0xf;
  //Serial.print("From Mega2 "); Serial.print(cmd, HEX); Serial.print(", "); Serial.println(pk);
  switch (pk) {
    case 0:
      // Befehlsabgaben
      //Serial.print("From Mega2 "); Serial.print(cmd, HEX); Serial.print(", "); Serial.println(pk);
      if (m_befehlNothing) {
        actVal = cmd & 1;
        //Serial.print("actval: "); Serial.print(actVal); Serial.print(", mBefAusf: "); Serial.println(m_befehlAusfahrtWB);
        if (m_befehlAusfahrtWB != actVal) {
          Serial.println("Activate Ausfahrt WB.");
          m_ActivateAusfahrt = millis() + 5000ul;
          m_befehlNothing = false;
        }
        m_befehlAusfahrtWB = actVal;

        actVal = (cmd & 2) == 2;
        //Serial.print("actval: "); Serial.print(actVal); Serial.print(", mBefEinf: "); Serial.println(m_befehlEinfahrtWB);
        if (actVal && !m_befehlEinfahrtWB) {
          Serial.println("Activate Einfahrt WB.");
          m_ActivateEinfahrt = millis() + 5000ul;
          m_befehlNothing = false;
        }
        m_befehlEinfahrtWB = actVal;
      } else {
        if ((cmd & 3) == 0) {
          Serial.print("REset Befehlsabgabe: "); Serial.println(cmd, HEX);
          m_befehlNothing = true;
        }
      }
      break;

    case 1:
      // Gruppensignalhebel (Signal wird aus Fahrstraße ermittelt)
      m_signalLBActive = cmd & 1;
      m_signalAHActive = (cmd & 2) == 2;
      //Serial.print("LB - AH: "); Serial.print(m_signalLBActive); Serial.print(" - "); Serial.println(m_signalAHActive);
      break;

    case 3:
      // Zugmeldung
      checkStartTrain(cmd & 0x7);
      break;

    case 4:
      // Streckenblock
      updateStreckenblock(cmd & 0xf);
      break;

    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
      setFahrstrasse(cmd);
      break;

    case 13:
      if ((cmd & 0xd) == 0xd) {
        m_Pause = !m_Pause;
        Serial.print("Pause: "); Serial.println(m_Pause);
        leds[315] = (m_Pause) ? pauseColor : trackColor;
      }
  }

  ledsChanged = true;
}

void World::checkStartTrain(uint8_t source) {
  uint8_t shiftedSource = source;
  uint8_t shiftedLast = m_lastStart;
  //Serial.print("Check start train last: "); Serial.print(m_lastStart, HEX); Serial.print(", source: "); Serial.println(source);

  for (uint8_t i = 0; i < 3; i++) {
    if ((shiftedLast & 1) && !(shiftedSource & 1)) {
      startTrain(i);
    }

    shiftedSource >>= 1;
    shiftedLast >>= 1;
  }

  m_lastStart = source;
}

/**
 * Streckenblock-Info an DRS2 durchgeben
 **/
void World::updateDrs2(uint8_t source) {
  bool isErlaubnis = (source & 8) == 0;
  uint8_t val;

  if (isErlaubnis) {
    m_DRS_IS_FROM_LB = (source & 1);
     val =  (m_DRS_IS_FROM_LB) ? DRS2_LB_UNBLOCKED: DRS2_LB_BLOCKED;
  } else {
    bool isFree = source & 1;
    if (m_DRS_IS_FROM_LB) {
      val = (isFree) ? DRS2_LB_END_BLOCKED : DRS2_LB_END_UNBLOCKED;
    } else {
      val = (isFree) ? DRS2_LB_ANF_BLOCKED : DRS2_LB_ANF_UNBLOCKED;
    }
  }

  Serial3.write(val);
  //Serial.print("Send DRS2: "); Serial.println(val);
}

/**
 * Section block, lower nibble: <Erl 0/Free 1> <Wildberg> <Althengstett> <Liebenzell>
 * 1: is remote, 0: is local
 **/
void World::updateStreckenblock(uint8_t source) {
  //Serial.print("Update Streckenblock "); Serial.println(source, HEX);
  updateDrs2(source);
  bool isErlaubnis = (source & 8) == 0;

  for (uint8_t i = 0; i < 3; i++) {
    if (isErlaubnis) {
      if (!(source & 1) != m_streckeIsRemote[i]) {
        Serial.print("Switch block "); Serial.print(i); Serial.print(" to "); Serial.println(!(source & 1));
      }

      m_streckeIsRemote[i] = !(source & 1);
    } else {
      if ((source & 1) != m_streckeIsFree[i]) {
        Serial.print("Switch free state "); Serial.print(i); Serial.print(" to "); Serial.println(!(source & 1));
      }

      m_streckeIsFree[i] = source & 1;
    }
    source >>= 1;
  }

  fahrwege[FW_LB_EFS]->setBlock(m_streckeIsRemote[0]);
  fahrwege[FW_LB_EFS]->setFreeState(m_streckeIsFree[0]);

  for (uint8_t fwn = FW_LB_T1; fwn <= FW_LB_T4; fwn++) {
    fahrwege[fwn]->setBlock(m_streckeIsRemote[0]);
    fahrwege[fwn]->setFreeState(m_streckeIsFree[0]);
  }

  for (uint8_t fwn = FW_T1_LB; fwn <= FW_T4_LB; fwn++) {
    fahrwege[fwn]->setBlock(m_streckeIsRemote[0]);
    fahrwege[fwn]->setFreeState(m_streckeIsFree[0]);
  }

  fahrwege[FW_AH_EFS]->setBlock(m_streckeIsRemote[1]);
  fahrwege[FW_AH_EFS]->setFreeState(m_streckeIsFree[1]);

  for (uint8_t fwn = FW_AH_T2; fwn <= FW_AH_T4; fwn++) {
    fahrwege[fwn]->setBlock(m_streckeIsRemote[1]);
    fahrwege[fwn]->setFreeState(m_streckeIsFree[1]);
  }

  for (uint8_t fwn = FW_T2_AH; fwn <= FW_T4_AH; fwn++) {
    fahrwege[fwn]->setBlock(m_streckeIsRemote[1]);
    fahrwege[fwn]->setFreeState(m_streckeIsFree[1]);
  }

  fahrwege[FW_WB_EFS]->setBlock(m_streckeIsRemote[2]);
  fahrwege[FW_WB_EFS]->setFreeState(m_streckeIsFree[2]);
  
  for (uint8_t fwn = FW_WB_T1; fwn <= FW_WB_T4; fwn++) {
    fahrwege[fwn]->setBlock(m_streckeIsRemote[2]);
    fahrwege[fwn]->setFreeState(m_streckeIsFree[2]);
  }
  
  for (uint8_t fwn = FW_T1_WB; fwn <= FW_T4_WB; fwn++) {
    fahrwege[fwn]->setBlock(m_streckeIsRemote[2]);
    fahrwege[fwn]->setFreeState(m_streckeIsFree[2]);
  }
}

void World::startTrain(uint8_t source) {
  Serial.print("Start train "); Serial.println(source);
  switch (source) {
    case 0:
      m_fromLB = selectFW(FW_LB_EFS, FW_LB_T1, FW_LB_T4);
      m_fromLB->show(NULL);
      m_fromLB->start();
      break;
    case 1:
      if (!m_fromAH) {
        m_fromAH = selectFW(FW_AH_EFS, FW_AH_T2, FW_AH_T4);
        m_fromAH->show(NULL);
      }
      m_fromAH->start();
      break;
    case 2:
      m_fromWB = selectFW(FW_WB_EFS, FW_WB_T1, FW_WB_T4);
      m_fromWB->show(NULL);
      m_fromWB->start();
      break;
  }
}

Fahrweg* World::selectFW(uint8_t noSelection, uint8_t firstNum, uint8_t lastNum) {
  Fahrweg* result = fahrwege[noSelection];
  for (uint8_t fw = firstNum; fw <= lastNum; fw++) {
    if (fahrwege[fw]->isShown()) {
      result = fahrwege[fw];
      break;
    }
  }

  return result;
}

void World::changeFW(uint8_t fwNum, bool setClear, Train* sourceTrain) {
  Fahrweg *fw = fahrwege[fwNum];
  if (fw->isShown() != setClear) {
    if (setClear) {
      fw->show(sourceTrain);
      if (sourceTrain != NULL) {
        Serial.println("Redraw souce train.");
        sourceTrain->redraw();
      }

      checkSetDurchfahrt();
     } else {
      Serial.print("changeFW, clear "); Serial.println(fwNum);
      fw->clear();
    }
  }
}

void World::setFahrstrasse(uint8_t source) {
  uint8_t bits = ~(source & 0xf);
  uint8_t baseValue = (((source >> 4) & 0xf) - 8) << 2;
  for (uint8_t mask = 1, offset = 0; offset < 4; mask <<= 1, offset++) {
    uint8_t fsNum = baseValue + offset;
    //Serial.print("Check fw: "); Serial.println(fsNum);
    Train *sourceTrain = NULL;
    if (bits & mask) {
      Fahrweg *sourceFW = NULL;
      if (fsNum <= FW_LB_T4 ) {
        sourceTrain = fahrwege[FW_LB_EFS]->getTrain();
        sourceFW = fahrwege[FW_LB_EFS];
        changeFW(FW_LB_EFS, false, sourceTrain);
      } else if (fsNum <= FW_AH_T4 ) {
        sourceTrain = fahrwege[FW_AH_EFS]->getTrain();
        sourceFW = fahrwege[FW_AH_EFS];
        changeFW(FW_AH_EFS, false, sourceTrain);
      } else if (fsNum <= FW_WB_T4 ) {
        sourceTrain = fahrwege[FW_WB_EFS]->getTrain();
        sourceFW = fahrwege[FW_WB_EFS];
        changeFW(FW_WB_EFS, false, sourceTrain);
      }

      if (!fahrwege[fsNum]->isShown()) {
        Serial.print("Show fw: "); Serial.println(fsNum);
        fahrwege[fsNum]->show(sourceTrain);
        if (sourceTrain) {
          if (fsNum <= FW_LB_T4 ) {
            m_fromLB = fahrwege[fsNum];
            fahrwege[FW_LB_EFS]->clearTrain();
          } else if (fsNum <= FW_AH_T4 ) {
            m_fromAH = fahrwege[fsNum];
            fahrwege[FW_AH_EFS]->clearTrain();
          } else if (fsNum <= FW_WB_T4 ) {
            m_fromWB = fahrwege[fsNum];
            fahrwege[FW_WB_EFS]->clearTrain();
          }
        } else {
          if (fsNum >= FW_T1_LB && fsNum <= FW_T4_LB) {
            m_fromLB = fahrwege[fsNum];
          } else if (fsNum >= FW_T2_AH && fsNum <= FW_T4_AH) {
            m_fromAH = fahrwege[fsNum];
          } else if (fsNum >= FW_T1_WB && fsNum <= FW_T4_WB) {
            m_fromWB = fahrwege[fsNum];
          }
        }
      }

      if (sourceFW) {
        //Serial.print("Stop FW "); Serial.println((unsigned long) sourceFW);
        sourceFW->stop();
      }     
    } else {
      sourceTrain = fahrwege[fsNum]->getTrain();

      if (fahrwege[fsNum]->isShown() && !fahrwege[fsNum]->isRunning()) {
        Serial.print("Clear fw: "); Serial.println(fsNum);

        fahrwege[fsNum]->clear();
        switch (fsNum) {
          case FW_T1_WB: signals[7].release(); send(0xe1); break;
          case FW_T2_WB: signals[8].release(); break;
          case FW_T4_WB: signals[9].release(); break;

        }
      }

      switch (fsNum) {
          // Zug an Einfahrts-Fahrweg zurückgeben
          case FW_LB_T1:
          case FW_LB_T2:
          case FW_LB_T3:
          case FW_LB_T4:
            if ((sourceTrain != NULL) && !sourceTrain->isEmpty() && !fahrwege[FW_LB_EFS]->isShown() && (!fahrwege[fsNum]->isRunning())) {
              Serial.print("Return FW LB "); Serial.println((sourceTrain == NULL) ? "kein Zug" : (sourceTrain->isEmpty() ? "leer" : "laufender Zug"));
              fahrwege[FW_LB_EFS]->show(sourceTrain);
              fahrwege[FW_LB_EFS]->setRunning();
              sourceTrain->redraw();
            }
            break;

          case FW_AH_T2:
          case FW_AH_T3:
          case FW_AH_T4:
            if ((sourceTrain != NULL) && !sourceTrain->isEmpty() && !fahrwege[FW_AH_EFS]->isShown() && (!fahrwege[fsNum]->isRunning())) {
              Serial.print("Return FW AH "); Serial.println((sourceTrain == NULL) ? "kein Zug" : (sourceTrain->isEmpty() ? "leer" : "laufender Zug"));
              fahrwege[FW_AH_EFS]->show(sourceTrain);
              fahrwege[FW_AH_EFS]->setRunning();
              sourceTrain->redraw();
            }
            break;
      }
    }

    checkSetDurchfahrt();
    checkSignal(fsNum);
  }
}

void World::checkSetDurchfahrt() {
    // Sonderbehandlung für Durchfahrten auf Gleis 1 (nur hier möglich).
    bool isDurchfahrt = (fahrwege[FW_LB_T1]->isShown() && fahrwege[FW_T1_WB]->isShown()) || (fahrwege[FW_WB_T1]->isShown() && fahrwege[FW_T1_LB]->isShown());
    fahrwege[FW_LB_T1]->setDurchfahrt(isDurchfahrt);
    fahrwege[FW_WB_T1]->setDurchfahrt(isDurchfahrt);
    fahrwege[FW_T1_LB]->setDurchfahrt(isDurchfahrt);
    fahrwege[FW_T1_WB]->setDurchfahrt(isDurchfahrt);
}

void World::checkSignal(uint8_t fsNum) {
  unsigned long now = millis();
  bool isActive = fahrwege[fsNum]->isShown();
  uint8_t signalNo = 0;
  //if (fsNum == FW_LB_T4) {Serial.print(" Fahrweg active: "); Serial.print(fsNum); Serial.print(", "); Serial.print(isActive); Serial.print(", AH active: "); Serial.print(m_signalAHActive); Serial.print(", LB active: "); Serial.println(m_signalLBActive);}
  switch (fsNum) {
    case FW_LB_T1:
    case FW_LB_T2:
    case FW_LB_T3:
    case FW_LB_T4:
      isActive = fahrwege[FW_LB_T1]->isShown() || fahrwege[FW_LB_T2]->isShown() || fahrwege[FW_LB_T3]->isShown() || fahrwege[FW_LB_T4]->isShown();
      setSignal(0, m_signalLBActive && isActive);
      break;

    case FW_AH_T2:
    case FW_AH_T3:
    case FW_AH_T4:
      isActive = fahrwege[FW_AH_T2]->isShown() || fahrwege[FW_AH_T3]->isShown() || fahrwege[FW_AH_T4]->isShown();
      setSignal(1, m_signalAHActive && isActive);
      break;

    case FW_T1_LB:
      setSignal(3, m_signalLBActive && isActive);
      break;

    case FW_T2_LB:
    case FW_T2_AH:
      isActive = fahrwege[FW_T2_LB]->isShown() || fahrwege[FW_T2_AH]->isShown();
      setSignal(4, (m_signalLBActive || m_signalAHActive) && isActive);
      break;

    case FW_T3_LB:
    case FW_T3_AH:
      isActive = fahrwege[FW_T3_LB]->isShown() || fahrwege[FW_T3_AH]->isShown();
      setSignal(5, (m_signalLBActive || m_signalAHActive) && isActive);
      break;
      
    case FW_T4_LB:
    case FW_T4_AH:
      isActive = fahrwege[FW_T4_LB]->isShown() || fahrwege[FW_T4_AH]->isShown();
      setSignal(6, (m_signalLBActive || m_signalAHActive) && isActive);
      break;

    case FW_T1_WB:
    case FW_T2_WB:
    case FW_T4_WB:
      if (!m_streckeIsFree[2] || !m_streckeIsRemote[2]) {
        return;
      }

      isActive = fahrwege[FW_T1_WB]->isShown() || fahrwege[FW_T2_WB]->isShown() || fahrwege[FW_T4_WB]->isShown();
      if (now < m_ActivateAusfahrt) {
        //Serial.print("Wait for activation out at "); Serial.print(m_ActivateAusfahrt); Serial.print(", now is "); Serial.println(now);
        //isActive = false;
      } else {
        //Serial.print("fsnum "); Serial.print(fsNum); Serial.print(", isShown "); Serial.print(fahrwege[fsNum]->isShown()); Serial.print(", isActive "); Serial.println(isActive);
        switch(fsNum) {
          case FW_T1_WB: if (fahrwege[FW_T1_WB]->isShown()) signalNo = 7; break;
          case FW_T2_WB: if (fahrwege[FW_T2_WB]->isShown()) signalNo = 8; break;
          case FW_T4_WB: if (fahrwege[FW_T4_WB]->isShown()) signalNo = 9; break;
        }
        //Serial.print("Activate out "); Serial.print(isActive); Serial.print(" - Befehl Ausfahrt "); Serial.print(m_befehlAusfahrtWB); Serial.print(", Signal "); Serial.print(signalNo); Serial.print(", free "); Serial.println(m_streckeIsFree[2]);
        if (isActive && m_befehlAusfahrtWB) {
          if (signalNo != 0) {
            m_ActivateAusfahrt = UINT32_MAX;
            Serial.print("Is active out: "); Serial.println(isActive);
            setSignal(signalNo, isActive);
            // Signalmelder nur bei Ausfahrt vom Gleis 1
            send(0xe1 | ((fsNum == FW_T1_WB) ? 8 : 0));
            }
        } else {
          m_ActivateAusfahrt = now + 2000ul;
          //Serial.println("Wait for activation.");
          setSignal(signalNo, false);
          send(0xe1);
        }
      }
      break;

    case FW_WB_T1:
    case FW_WB_T2:
    case FW_WB_T4:
      isActive = fahrwege[FW_WB_T1]->isShown() || fahrwege[FW_WB_T2]->isShown() || fahrwege[FW_WB_T4]->isShown();
      if (now < m_ActivateEinfahrt) {
        //Serial.print("Wait for activation in at "); Serial.print(m_ActivateEinfahrt); Serial.print(", now is "); Serial.println(now);
        isActive = false;
      } else {
        Serial.println("Activate in");
        if (isActive) {
          m_ActivateEinfahrt = UINT32_MAX;
          Serial.print("Is active in: "); Serial.println(isActive);
          setSignal(2, isActive);
          send(0xe8); // Signalmelder M ein
        } else {
          setSignal(2, false);
          send(0xe0);
        }
      }

      break;
  }
}

void World::setSignal(int num, bool value) {
  if (value) {
    signals[num].set();
  } else {
    signals[num].release();
  }
}

void World::process(unsigned long now) {
  m_isTestMode = false;
  if ((now < lastStep) || m_Pause) {
    return;
  }

  if (now > m_DecativateDrs2) {
    //Serial.println("Deactivate drs2");
    drs2Available = false;
  }

 if (m_fromLB && !m_fromLB->done()) {
    m_fromLB->advance(false);
  } else {
    m_fromLB = NULL;
  }

  if (m_fromAH && !m_fromAH->done()) {
    m_fromAH->advance(false);
  } else {
    m_fromAH = NULL;
  }

  if (m_fromWB && !m_fromWB->done()) {
    m_fromWB->advance(false);
  } else {
    m_fromWB = NULL;
  }

  lastStep = now + 600;

  for (int i = 0; i < 4; i++) {
    /*if (!trackTrains[i].isEmpty()) {
      trackTrains[i].redraw();
    }
    ledsChanged = true;*/

    //CRGB state = (trackTrains[i].isEmpty()) ? CRGB::Green : CRGB::Red;
    //leds[281 + i] = state;
  }
}


int activeFW = -1;
int activeSg = 0;

void World::test(unsigned long now) {
  m_isTestMode = true;
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
      fahrwege[testList[activeFW]]->show(NULL);
      Serial.println("Show done.");
    }
  }

  if (activeFW < 0) {
    activeFW = 0;
    fahrwege[testList[activeFW]]->show(NULL);
  }

  //Serial.println("Advance");
  fahrwege[testList[activeFW]]->advance(true);
  lastStep = now + 50;
}