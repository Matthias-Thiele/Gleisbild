#include "World.hpp"
#include "Events.hpp"

#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

extern bool ledsChanged;

unsigned long dummyEventList[] = {0};
int testList[] = {FW_AH_T3, FW_WB_T2, FW_LB_T4, FW_T3_LB, FW_T2_AH, FW_LB_T1, FW_T1_WB, FW_LB_T3, FW_T4_AH, FW_WB_T4,
                  FW_T4_LB, FW_T3_AH, FW_WB_T1, FW_LB_T2, FW_AH_T4, FW_T1_LB, FW_T2_WB, FW_T4_WB, FW_AH_T2, FW_T2_LB, 
                  -1};

// Einfahrt von Bad Liebenzell
short lbefs[] = {315, 385, -1};
unsigned long lbefsev[] = {
  WAIT_FOR_BLOCK | BLOCK_LB | BLOCK_IS_REMOTE | 319ul,
  SET_SIGNAL | (11ul << 12) | 320ul,
  RESET_SIGNAL | (11ul << 12) | 335ul,
  WAIT_FOR_SIGNAL | (0ul << 12) | 383ul,
  0
};

short lbt1[] = {315, 389, 204, 155, -1};
unsigned long lbt1234ev[] = {
  WAIT_FOR_BLOCK | BLOCK_LB | 319ul,
  SET_SIGNAL | (11ul << 12) | 320ul,
  RESET_SIGNAL | (11ul << 12) | 335ul,
  WAIT_FOR_SIGNAL | (0ul << 12) | 383ul,
  SET_SIGNAL | (0ul << 12) | 370ul | ONLY_TEST,
  RESET_SIGNAL | (0ul << 12) | 190ul | ONLY_TEST,
  RESET_SIGNAL | (0ul << 12) | 56ul | ONLY_TEST,
  TRACK | TRACK_ALLOCATE | 164ul,
  STOP_TRAIN | 164ul,
  TRACK | TRACK_ALLOCATE | 94ul,
  STOP_TRAIN | 94ul,
  TRACK | TRACK_ALLOCATE | 7ul,
  STOP_TRAIN | 7ul,
  TRACK | TRACK_ALLOCATE | 441ul,
  STOP_TRAIN | 441ul,
  0
};
short lbt2[] = {315, 389, 204, 203, 61, 55, 62, 103, -1};
short lbt3[] = {315, 389, 204, 203, 61, 48, 33, 3, -1};
short lbt4[] = {315, 389, 204, 203, 61, 43, 419, 450, -1};

// Einfahrt von Althengstett
short ahefs[] = {280, 207, -1};
unsigned long ahefsev[] = {
  WAIT_FOR_BLOCK | BLOCK_AH | BLOCK_IS_REMOTE | 275ul,
  SET_SIGNAL | (13ul << 12) | 275ul,
  RESET_SIGNAL | (13ul << 12) | 260ul,
  WAIT_FOR_SIGNAL | (1ul << 12) | 212ul,
  0
};

short aht2[] = {280, 206, 391, 402, 48, 49, 33, 33, 42, 38, 73, 103, -1};
unsigned long aht234ev[] = {
  WAIT_FOR_BLOCK | BLOCK_AH | BLOCK_IS_REMOTE | 275ul,
  SET_SIGNAL | (13ul << 12) | 275ul,
  RESET_SIGNAL | (13ul << 12) | 260ul,
  WAIT_FOR_SIGNAL | (1ul << 12) | 212ul,
  SET_SIGNAL | (1ul << 12) | 220ul | ONLY_TEST,
  RESET_SIGNAL | (1ul << 12) | 392ul | ONLY_TEST,
  TRACK | TRACK_ALLOCATE | 94ul,
  STOP_TRAIN | 94ul,
  TRACK | TRACK_ALLOCATE | 7ul,
  STOP_TRAIN | 7ul,
  TRACK | TRACK_ALLOCATE | 441ul,
  STOP_TRAIN | 441ul,
  0
};
short aht3[] = {280, 206, 391, 402, 48, 49, 33, 3, -1};
short aht4[] = {280, 206, 391, 393, 403, 450, -1};

// Einfahrt von Wildberg
short wbefs[] = {544, 558, 529, 473, -1};
unsigned long wbefsev[] = {
  WAIT_FOR_BLOCK | BLOCK_WB | BLOCK_IS_REMOTE | 548ul,
  SET_SIGNAL | (15ul << 12) | 544ul,
  RESET_SIGNAL | (15ul << 12) | 520ul,
  WAIT_FOR_SIGNAL | (2ul << 12) | 474ul,
  0
};

short wbt1[] = {544, 558, 529, 469, 130, 180, -1};
unsigned long wbt124ev[] = {
  WAIT_FOR_BLOCK | BLOCK_WB | BLOCK_IS_REMOTE | 548ul,
  SET_SIGNAL | (15ul << 12) | 544ul,
  RESET_SIGNAL | (15ul << 12) | 520ul,
  WAIT_FOR_SIGNAL | (2ul << 12) | 474ul,
  SET_SIGNAL | (2ul << 12) | 490ul | ONLY_TEST,
  RESET_SIGNAL | (2ul << 12) | 140ul | ONLY_TEST,
  RESET_SIGNAL | (2ul << 12) | 110ul | ONLY_TEST,
  RESET_SIGNAL | (2ul << 12) | 470ul | ONLY_TEST,
  TRACK | TRACK_ALLOCATE | 170ul,
  STOP_TRAIN | 170ul,
  TRACK | TRACK_ALLOCATE | 88ul,
  STOP_TRAIN | 88ul,
  TRACK | TRACK_ALLOCATE | 435ul,
  STOP_TRAIN | 435,
  0
};
short wbt2[] = {544, 558, 529, 469, 130, 125, 122, 78, -1};
short wbt4[] = {544, 558, 529, 469, 130, 125, 122, 119, 467, 425, -1};

// Ausfahrt nach Wildberg
short cwbt1[] = {169, 130, 469, 543, -1};
unsigned long cwbt1ev[] = {
  SET_SIGNAL | (7ul << 12) | 166ul,
  RESET_SIGNAL | (7ul << 12) | 135ul,
  SET_SIGNAL | (14ul << 12) | 500ul,
  RESET_SIGNAL | (14ul << 12) | 540ul,
  0
};

short cwbt2[] = {89, 122, 125, 129, 130, 130, 469, 543, -1};
unsigned long cwbt2ev[] = {
  SET_SIGNAL | (8ul << 12) | 91ul,
  RESET_SIGNAL | (8ul << 12) | 129ul,
  SET_SIGNAL | (14ul << 12) | 500ul,
  RESET_SIGNAL | (14ul << 12) | 540ul,
  0
};

short cwbt4[] = {436, 467, 119, 122, 125, 129, 130, 130, 469, 543, -1};
unsigned long cwbt4ev[] = {
  SET_SIGNAL | (9ul << 12) | 439ul,
  RESET_SIGNAL | (9ul << 12) | 460ul,
  SET_SIGNAL | (14ul << 12) | 500ul,
  RESET_SIGNAL | (14ul << 12) | 540ul,
  0
};

// Ausfahrt nach Liebenzell
short clbt1[] = {165, 204, 389, 329, 300, 314, -1};
unsigned long clbt1ev[] = {
  SET_SIGNAL | (3ul << 12) | 166ul,
  RESET_SIGNAL | (3ul << 12) | 190ul,
  SET_SIGNAL | (10ul << 12) | 350ul,
  RESET_SIGNAL | (10ul << 12) | 305ul,
  0
};

short clbt2[] = {93, 62, 56, 61, 203, 204, 389, 329, 300, 314, -1};
unsigned long clbt2ev[] = {
  SET_SIGNAL | (4ul << 12) | 88ul,
  RESET_SIGNAL | (4ul << 12) | 60ul,
  SET_SIGNAL | (10ul << 12) | 350ul,
  RESET_SIGNAL | (10ul << 12) | 305ul,
  0
};

short clbt3[] = {8, 33, 49, 61, 203, 204, 389, 329, 300, 314, -1};
unsigned long clbt3ev[] = {
  SET_SIGNAL | (5ul << 12) | 10ul,
  RESET_SIGNAL | (5ul << 12) | 30ul,
  SET_SIGNAL | (10ul << 12) | 350ul,
  RESET_SIGNAL | (10ul << 12) | 305ul,
  0
};

short clbt4[] = {440, 420, 43, 61, 203, 204, 389, 329, 300, 314, -1};
unsigned long clbt4ev[] = {
  SET_SIGNAL | (6ul << 12) | 430ul,
  RESET_SIGNAL | (6ul << 12) | 420ul,
  SET_SIGNAL | (10ul << 12) | 350ul,
  RESET_SIGNAL | (10ul << 12) | 305ul,
  0
};

// Ausfahrt nach Althengstett
short caht2[] = {93, 73, 38, 42, 33, 33, 48, 49, 402, 391, 206, 266, 295, 281, -1};
unsigned long caht2ev[] = {
  SET_SIGNAL | (4ul << 12) | 91ul,
  RESET_SIGNAL | (4ul << 12) | 73ul,
  SET_SIGNAL | (12ul << 12) | 250ul,
  RESET_SIGNAL | (12ul << 12) | 290ul,
  0
};

short caht3[] = {8, 33, 48, 49, 402, 391, 206, 266, 295, 281, -1};
unsigned long caht3ev[] = {
  SET_SIGNAL | (5ul << 12) | 10ul,
  RESET_SIGNAL | (5ul << 12) | 25ul,
  SET_SIGNAL | (12ul << 12) | 250ul,
  RESET_SIGNAL | (12ul << 12) | 290ul,
  0
};

short caht4[] = {440, 403, 393, 391, 206, 266, 295, 281, -1};
unsigned long caht4ev[] = {
  SET_SIGNAL | (6ul << 12) | 438ul,
  RESET_SIGNAL | (6ul << 12) | 410ul,
  SET_SIGNAL | (12ul << 12) | 250ul,
  RESET_SIGNAL | (12ul << 12) | 290ul,
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
  signals[0].setLedNo(390); // Einf. Calw von LB
  signals[1].setLedNo(205); // Einf. Calw von AH
  signals[2].setLedNo(468); // Einf. Calw von WB
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

  CRGB trainColor = {0, 0, 0xff};
  CRGB trackColor = {0x20, 0x20, 0x20};
  for (int i = 0; i < NUM_FAHRWEGE; i++) {
    fahrwege[i] = new Fahrweg(leds, trainColor, trackColor);
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
  fahrwege[FW_WB_T1]->set(wbt1, wbt124ev, 0, true);

  // Wildberg auf Gleis 2
  fahrwege[FW_WB_T2]->set(wbt2, wbt124ev, 1, true);

  // Wildberg auf Gleis 4
  fahrwege[FW_WB_T4]->set(wbt4, wbt124ev, 3, true);

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

void World::processCommand(uint8_t cmd) {
  if (m_isTestMode) {
    return;
  }

  uint8_t pk = (cmd >> 4) & 0xf;
  //Serial.print("From Mega2 "); Serial.print(cmd, HEX); Serial.print(", "); Serial.println(pk);
  switch (pk) {
    case 0:
    case 1:
      // Signalhebel
      for (int i = 0; i < 4; i++) {
        setSignal((pk << 2) + i, !(cmd & (8 >> i)));
      }

      break;

    case 2:
      // Zugmeldung
      checkStartTrain(cmd & 0x7);
      break;

    case 3:
      // Streckenblock
      updateStreckenblock(cmd & 0x7);
      break;

    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      setFahrstrasse(cmd);
      break;
  }

  ledsChanged = true;
}

void World::checkStartTrain(uint8_t source) {
  uint8_t shiftedSource = source;
  uint8_t shiftedLast = m_lastStart;

  for (uint8_t i = 0; i < 3; i++) {
    if (!(shiftedLast & 1) && (shiftedSource & 1)) {
      Serial.print("Start train "); Serial.println(i);
      startTrain(i);
    }

    shiftedSource >>= 1;
    shiftedLast >>= 1;
  }

  m_lastStart = source;
}

/**
 * Section block, lower nibble: 0 <Wildberg> <Althengstett> <Liebenzell>
 * 1: is remote, 0: is local
 **/
void World::updateStreckenblock(uint8_t source) {
  for (uint8_t i = 0; i < 3; i++) {
    if (!(source & 1) != m_streckeIsRemote[i]) {
      Serial.print("Switch block "); Serial.print(i); Serial.print(" to "); Serial.println(!(source & 1));
    }

    m_streckeIsRemote[i] = !(source & 1);
    source >>= 1;
  }

  fahrwege[FW_LB_EFS]->setBlock(m_streckeIsRemote[0]);
  for (uint8_t fwn = FW_LB_T1; fwn <= FW_LB_T4; fwn++) {
    fahrwege[fwn]->setBlock(m_streckeIsRemote[0]);
  }

  fahrwege[FW_AH_EFS]->setBlock(m_streckeIsRemote[1]);
  for (uint8_t fwn = FW_AH_T2; fwn <= FW_AH_T4; fwn++) {
    fahrwege[fwn]->setBlock(m_streckeIsRemote[1]);
  }

  fahrwege[FW_WB_EFS]->setBlock(m_streckeIsRemote[2]);
  for (uint8_t fwn = FW_WB_T1; fwn <= FW_WB_T4; fwn++) {
    fahrwege[fwn]->setBlock(m_streckeIsRemote[2]);
  }
}

void World::startTrain(uint8_t source) {
  switch (source) {
    case 0:
      m_fromLB = fahrwege[FW_LB_EFS];
      m_fromLB->show(NULL);
      break;
    case 1:
      m_fromAH = fahrwege[FW_AH_EFS];
      m_fromAH->show(NULL);
      break;
    case 2:
      m_fromWB = fahrwege[FW_WB_EFS];
      m_fromWB->show(NULL);
      break;
  }
}

void World::changeFW(uint8_t fwNum, bool setClear) {
  if (fahrwege[fwNum]->isShown() != setClear) {
    if (setClear) {
      fahrwege[fwNum]->show(NULL);
    } else {
      fahrwege[fwNum]->clear();
    }
  }
}

void World::setFahrstrasse(uint8_t source) {
  uint8_t bits = ~(source & 0xf);
  uint8_t baseValue = (((source >> 4) & 0xf) - 4) << 2;
  for (uint8_t mask = 8, offset = 0; mask > 0; mask >>= 1, offset++) {
    uint8_t fsNum = baseValue + offset;
    Train *sourceTrain = NULL;
    if (bits & mask) {
      if (fsNum <= FW_LB_T4 ) {
        sourceTrain = fahrwege[FW_LB_EFS]->getTrain();
        changeFW(FW_LB_EFS, false);
      } else if (fsNum <= FW_AH_T4 ) {
        sourceTrain = fahrwege[FW_AH_EFS]->getTrain();
        changeFW(FW_AH_EFS, false);
      } else if (fsNum <= FW_WB_T4 ) {
        sourceTrain = fahrwege[FW_WB_EFS]->getTrain();
        changeFW(FW_WB_EFS, false);
      }

      if (!fahrwege[fsNum]->isShown()) {
        Serial.print("Show fw: "); Serial.println(fsNum);
        fahrwege[fsNum]->show(sourceTrain);
        if (sourceTrain) {
          if (fsNum <= FW_LB_T4 ) {
            m_fromLB->done();
            m_fromLB = fahrwege[fsNum];
          } else if (fsNum <= FW_AH_T4 ) {
            m_fromAH->done();
            m_fromAH = fahrwege[fsNum];
          } else if (fsNum <= FW_WB_T4 ) {
            m_fromWB->done();
            m_fromWB = fahrwege[fsNum];
          }
        }
      }     
    } else {
      if (fahrwege[fsNum]->isShown()) {
        Serial.print("Clear fw: "); Serial.println(fsNum);
        fahrwege[fsNum]->clear();
      }
    }
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
  if (now < lastStep) {
    return;
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

  lastStep = now + 100;
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