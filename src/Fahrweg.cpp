#include "Fahrweg.hpp"
#include "Signal.hpp"
#include "Events.hpp"

extern bool ledsChanged;
Signal* g_signals;
bool g_isTestMode;

//static void trainHandler(AsyncTimer *timer, unsigned long now, void* source) {
//  Fahrweg* fw = (Fahrweg*)source;
//  fw->advance(g_isTestMode);
//}

void Fahrweg::send(uint8_t value) {
  Serial.print("Send: "); Serial.println(value, HEX);
  Serial1.write(value);
  Serial2.write(value);
}

void Fahrweg::setSignals(Signal* signals) {
  g_signals = signals;
}

void Fahrweg::setTrains(Train *trackTrains) {
  m_trackTrains = trackTrains;
}

Fahrweg::Fahrweg(CRGB* leds, CRGB trainColor, CRGB trackColor, CRGB occupiedColor) {
  m_train.init(leds, trainColor, trackColor, occupiedColor);
  m_leds = leds;
  m_trackColor = trackColor;
  m_timer = NULL;
  m_fahrwegItems = NULL;
  m_shown = false;
  m_sectionBlockIsRemote = false;
  m_trainRunning = -1;
}

void Fahrweg::clear() {
  Serial.println("Clear fahrweg");
  m_trainRunning = -1;
  if (!m_fahrwegItems) {
    return;
  }

  m_fwi.setFahrwegList(m_fahrwegItems);
  CRGB clearColor = {0, 0, 0};

  while (m_fwi.hasMore()) {
    int pos = m_fwi.nextPos();
    if (pos < 0 || pos > 600) {
      Serial.println("+++++ Error +++++++++");
      delay(1000);
    } else {
      m_leds[pos] = clearColor;
    }
  }

  Serial.print("Is inbound? "); Serial.println(m_isInbound);
  if (m_isInbound) {
    m_train.redraw();
  }

  ledsChanged = true;
  m_shown = false;
}

bool Fahrweg::isShown() {
  return m_shown;
}

void Fahrweg::show(Train* train) {
  Serial.print("Show fw, train: "); Serial.println((long) train, HEX);
  m_fwi.setFahrwegList(m_fahrwegItems);

  while (m_fwi.hasMore()) {
    int pos = m_fwi.nextPos();
    m_leds[pos] = m_trackColor;
  }

  ledsChanged = true;
  m_shown = true;

  m_fwi.setFahrwegList(m_fahrwegItems);

  if (train) {
    bool found = false;
    short tpos = train->getPositions()[TRAIN_LENGTH - 1];
    Serial.print("look for pos "); Serial.print(tpos);
    if (tpos >= 0) {
      while (m_fwi.hasMore()) {
        int pos = m_fwi.peekPos();
        if (pos == tpos) {
          Serial.print(", found ");
          m_train.setPositions(train->getPositions());
          found = true;
          break;
        } else {
          m_fwi.nextPos();
        }
      }

      if (!found) {
        Serial.print("reset ");
        m_fwi.setFahrwegList(m_fahrwegItems);
      }

      start();
    }
  } else {
    Serial.print("Track: "); Serial.print(m_track); Serial.print(", isEmpty: "); Serial.println(m_trackTrains[m_track].isEmpty());
    if (!m_isInbound && !m_trackTrains[m_track].isEmpty()) {
      m_train.setPositions(m_trackTrains[m_track].getPositions());
      m_train.redraw();
      Serial.println("Start train");
      start();
    }
  }

  Serial.print("Show done. ");Serial.println(isShown());
}

void Fahrweg::set(short* fahrwegItems, unsigned long* eventList, uint8_t track, boolean isInbound) {
  m_fahrwegItems = fahrwegItems;
  m_eventList = eventList;
  m_isInbound = isInbound;
  m_track = track;

  m_fwi.setFahrwegList(fahrwegItems);
}

void Fahrweg::start() {
  Serial.print("start() "); Serial.print(m_fwi.peekPos()); Serial.print(", train ");Serial.println(m_train.getPositions()[0]);
  m_train.adjust(m_fwi.peekPos());
  m_trainRunning = 0;
  m_arrivalSent = false;
}

void Fahrweg::stop() {
  if (m_trainRunning > 0) {
    Serial.println("stop fahrweg.");
  
    m_trainRunning = -1;
    m_train.clear();
  }
}

void Fahrweg::setBlock(bool isRemote) {
  if (m_sectionBlockIsRemote != isRemote) {
    Serial.print("section block of track "); Serial.print(m_track); Serial.print(" set to "); Serial.print(isRemote);Serial.print(", was "); Serial.println(m_sectionBlockIsRemote);
  }
  m_sectionBlockIsRemote = isRemote;
}

void Fahrweg::setFreeState(bool isFree) {
  m_sectionIsFree = isFree;
}

Train* Fahrweg::getTrain() {
  return &m_train;
}

uint8_t Fahrweg::getTrack() {
  return m_track;
}

void Fahrweg::sendTrainArrived(uint8_t source) {
  if (m_arrivalSent) {
    return;
  } else {
    m_arrivalSent = true;
  }

  switch (source) {
    case 0:
      // Einfahrt aus Liebenzell
      send(0xf4);
      break;

    case 1:
      // Einfahrt aus Althengstett
      send(0xf5);
      break;

    case 2:
      // Einfahrt aus Wildberg auf Gleis 1
      Serial.println("Rückblocken WB auf Gleis 1");
      send(0xd8);
      break;

    case 3:
      // Einfahrt aus Wildberg auf Gleis 2 oder 4
      Serial.println("Rückblocken WB auf Gleis 2 oder 4");
      send(0xd9);
      break;

    case 4:
      // Ausfahrt nach Wildberg von Gleis 1-4
      Serial.println("Rückblocken WB Ausfahrt.");
      send(0xda);
      break;

    case 5:
      // Ausfahrender Zug hat das Gleisbild verlassen
      Serial.println("Ausgehende Zugfahrt beendet.");
      m_trainRunning = -1;
      break;
  }
}

void Fahrweg::advance(bool testMode) {
  /*Serial.print("Advance pos: "); Serial.print(m_fwi.peekPos()); 
  Serial.print(", test: "); Serial.print(testMode); 
  Serial.print(", empty: "); Serial.print(m_train.isEmpty()); 
  Serial.print(", track: "); Serial.print(m_track); 
  Serial.print(", shown: "); Serial.print(m_shown); 
  Serial.print(", shown and empty: "); Serial.print(m_shown && m_train.isEmpty()); 
  Serial.print(", mtt empty: "); Serial.print(m_trackTrains[m_track].isEmpty()); 
  Serial.print(", this: "); Serial.println((long)this, HEX);*/
  g_isTestMode = testMode;

  if (!testMode && m_shown && m_train.isEmpty()) {
    if (!m_isInbound && (m_track != 0xff) && m_train.isEmpty() && !m_trackTrains[m_track].isEmpty()) {
      Serial.print("Start train "); Serial.println((long)this, HEX);
      m_train.setPositions(m_trackTrains[m_track].getPositions());
      m_trackTrains[m_track].clear();
      start();
    }
  }

  if (!testMode && (m_trainRunning < 0)) {
    return;
  }

  if (m_fwi.hasMore() || (!m_isInbound && !m_train.isEmpty())) {
    short pos = m_fwi.peekPos();

    unsigned long* evp = m_eventList;
    while (unsigned long ev = *evp++) {
      if (((short)(ev & 0xffful)) == pos) {
        int sig = (ev >> 12) & 0xff;
        bool isRemote = ev & BLOCK_IS_REMOTE;
        bool onlyTest = ev & ONLY_TEST;
        uint8_t occ = 0xf0 | sig;
        uint8_t blkf = 0xd0 | ((sig >> 4) & 0xf);

        Serial.print("now at pos "); Serial.print(pos);
        Serial.print(", Test "); Serial.print(testMode);
        Serial.print(", Signal "); Serial.print(sig);
        Serial.print(", Remote "); Serial.print(isRemote);
        Serial.print(", Section "); Serial.print(m_sectionBlockIsRemote);
        Serial.print(", Event "); Serial.println(ev >> 20, HEX);
        switch (ev & 0xff00000) {
          case OCCUPANCY:
            Serial.print("Occupancy: "); Serial.println(occ, HEX);
            send(occ);
            m_train.occupancy((occ & 0xf) < 8);
            break;
            
          case BLOCK_FIELD:
            Serial.print("Block field at pos "); Serial.print(pos); Serial.print(" - "); Serial.print(sig, HEX); Serial.print(", isFree: "); Serial.print(m_sectionIsFree); Serial.print(", isRemote: "); Serial.print(isRemote); Serial.print(", m_sBIR: "); Serial.println(m_sectionBlockIsRemote);
            if (ev & BLOCK_WAIT_IF_FREE) {
              if (m_sectionIsFree) {
                // dem FDL Zeit geben den Anfangsblock zu setzen, solange nicht weiterlaufen
                Serial.println("Wait for Anfangsfeld.");
                return;
              }

              break;
            }
            
            if (sig < 0x10) {
              if (!testMode && ((m_sectionBlockIsRemote != isRemote) || !m_sectionIsFree)) {
                Serial.println("Wait for section block.");
                return;
              }
            } else {
              Serial.print("Change block field "); Serial.println(blkf, HEX);
              send(blkf);
            }
            break;

          case SET_SIGNAL:
            if (!onlyTest || testMode) {
              g_signals[sig].set();
              Serial.print(" Set signal "); Serial.print(sig); Serial.print(" at pos "); Serial.println(pos);
              if (sig == 2) {
                send(0xe8); // Signalmelder M ein
              }
            }
            break;
        
          case RESET_SIGNAL:
            if (!onlyTest || testMode) {
              g_signals[sig].release();
              Serial.print(" Reset signal "); Serial.print(sig); Serial.print(" at pos "); Serial.println(pos);
              switch (sig) {
                case 2:
                  send(0xe0); // Signalmelder M aus
                  break;

                case 7:
                  send(0xe1); // Signalmelder H aus
                  m_trackTrains[m_track].clear();
                  break;

                case 3:
                case 4:
                case 5:
                case 6:
                case 8:
                case 9:
                  // alle Ausfahrten: Gleis als Frei melden.
                  m_trackTrains[m_track].clear();
                  break;

              }
            }
            break;
        
          case STOP_TRAIN_LB:
          case STOP_TRAIN_AH:
          case STOP_TRAIN_WB1:
          case STOP_TRAIN_WB24:
          case STOP_TRAIN_WBOUT:

            //Serial.println("Train stopped");
            if (testMode) {
              m_shown = false;
              Serial.println("Clear 1");
              clear();
              if (m_isInbound) {
                m_train.redraw();
                Serial.println("Redraw");
              }
              ledsChanged = true;
            } else {
              if (m_trainRunning > 10) {
                Serial.print("Stop train, distance = "); Serial.println(m_trainRunning);
                sendTrainArrived(sig);
                m_trainRunning = -1;
                //stop();
              }
            }

            return;

          case WAIT_FOR_SIGNAL:
            if (!testMode && !g_signals[sig].isSet()) {
              //Serial.print(" Wait for signal "); Serial.println(sig);
              return;
            }
            break;

          case TRACK:
            if (m_track != 0xff) {
              if (ev & TRACK_ALLOCATE) {
                if (m_trackTrains[m_track].isEmpty() && !m_train.isEmpty()) {
                  Serial.print("Track allocated "); Serial.println(m_track);
                  m_trackTrains[m_track].setPositions(m_train.getPositions());
                  //m_train.clear();
                }
              }
              if (ev & TRACK_RELEASE) {
                if (!m_trackTrains[m_track].isEmpty()) {
                  Serial.print("Track released "); Serial.println(m_track);
                }

                m_trackTrains[m_track].clear();
              }
            }
            break;
        }
      }
    }

    m_fwi.nextPos();
    m_train.advance(pos);
    m_trainRunning++;
    ledsChanged = true;
  } else {
    if (testMode) {
      m_shown = false;
      Serial.println("Clear 2");
      clear();
      if (m_isInbound) {
        m_train.redraw();
      }
      ledsChanged = true;
    }
  }
}

bool Fahrweg::done() {
  //Serial.print("FW done? "); Serial.println(m_shown);
  return !m_shown;
}

bool Fahrweg::isRunning() {
  return m_trainRunning > 10;
}
