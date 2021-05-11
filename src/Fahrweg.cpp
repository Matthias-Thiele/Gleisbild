#include "Fahrweg.hpp"
#include "Signal.hpp"
#include "Events.hpp"

extern bool ledsChanged;
Signal* g_signals;

static void trainHandler(AsyncTimer *timer, unsigned long now, void* source) {
  Fahrweg* fw = (Fahrweg*)source;
  fw->advance();
}

void Fahrweg::setSignals(Signal* signals) {
  g_signals = signals;
}

Fahrweg::Fahrweg(CRGB* leds, CRGB trainColor, CRGB trackColor) {
  m_train.init(leds, trainColor, trackColor);
  m_leds = leds;
  m_trackColor = trackColor;
  m_timer = NULL;
  m_fahrwegItems = NULL;
  m_shown = false;
}

void Fahrweg::clear() {
  if (!m_fahrwegItems) {
    return;
  }

  m_fwi.setFahrwegList(m_fahrwegItems);
  CRGB clearColor = {0, 0, 0};

Serial.print("Clear: ");
  while (m_fwi.hasMore()) {
    int pos = m_fwi.nextPos();
    if (pos < 0 || pos > 600) {
      Serial.println("+++++ Error +++++++++");
      delay(1000);
    } else {
Serial.print(pos); Serial.print(", ") ;
      m_leds[pos] = clearColor;
    }
  }

Serial.println(".");
  ledsChanged = true;
}

void Fahrweg::show() {
  m_fwi.setFahrwegList(m_fahrwegItems);

  while (m_fwi.hasMore()) {
    int pos = m_fwi.nextPos();
    m_leds[pos] = m_trackColor;
  }

  ledsChanged = true;
  m_shown = true;

  m_fwi.setFahrwegList(m_fahrwegItems);
}

void Fahrweg::set(short* fahrwegItems, unsigned long* eventList, boolean isInbound) {
  m_fahrwegItems = fahrwegItems;
  m_eventList = eventList;
  m_isInbound = isInbound;

  m_fwi.setFahrwegList(fahrwegItems);
}

void Fahrweg::start() {
  m_timer = AsyncTimer::add(50, -1, this, trainHandler);
}

void Fahrweg::advance() {
  if (m_fwi.hasMore() || (!m_isInbound && !m_train.isEmpty())) {
  //if (m_fwi.hasMore()) {
    short pos = m_fwi.peekPos();
    //Serial.print("now at pos "); Serial.println(pos);

    unsigned long* evp = m_eventList;
    while (unsigned long ev = *evp++) {
      if (((short)(ev & 0xffful)) == pos) {
      int sig = (ev >> 12) & 0xff;
      Serial.print("now at pos "); Serial.print(pos);
      Serial.print(", Signal "); Serial.println(sig);
      Serial.print(", Event "); Serial.println(ev >> 20);
        switch (ev & 0xf00000) {
          case SET_SIGNAL:
            g_signals[sig].set();
            Serial.print(" Set signal "); Serial.println(sig);
            break;
        
          case RESET_SIGNAL:
            g_signals[sig].release();
            Serial.print(" Reset signal "); Serial.println(sig);
            break;
        
          case STOP_TRAIN:
            Serial.println("Train stopped");
            m_shown = false;
            Serial.println("Clear 1");
            clear();
            if (m_isInbound) {
              m_train.redraw();
              Serial.println("Redraw");
            }
            ledsChanged = true;
            return;

          case WAIT_FOR_SIGNAL:
            if (!g_signals[sig].isSet()) {
              Serial.print(" Wait for signal "); Serial.println(sig);
              return;
            }
            break;
        }
      }
    }

    m_fwi.nextPos();
    m_train.advance(pos);
    ledsChanged = true;
  } else {
    m_shown = false;
    Serial.println("Clear 2");
    clear();
    ledsChanged = true;
  }
}

bool Fahrweg::done() {
  //Serial.print("FW done? "); Serial.println(m_shown);
  return !m_shown;
}
