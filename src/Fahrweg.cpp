#include "Fahrweg.hpp"
extern bool ledsChanged;

static void trainHandler(AsyncTimer *timer, unsigned long now, void* source) {
  Fahrweg* fw = (Fahrweg*)source;
  fw->advance();
}


Fahrweg::Fahrweg(CRGB* leds, CRGB trainColor, CRGB trackColor) {
  m_train.init(leds, trainColor, trackColor);
  m_leds = leds;
  m_trackColor = trackColor;
  m_timer = NULL;
  m_fahrwegItems = NULL;
}

void Fahrweg::clear() {
  if (!m_fahrwegItems) {
    return;
  }

  m_fwi.setFahrwegList(m_fahrwegItems);
  CRGB clearColor = {0, 0, 0};

  while (m_fwi.hasMore()) {
    int pos = m_fwi.nextPos();
    if (pos < 0 || pos > 300) {
      Serial.println("+++++ Error +++++++++");
      delay(1000);
    } else {
      m_leds[pos] = clearColor;
    }
  }

  ledsChanged = true;
}

void Fahrweg::set(short* fahrwegItems, unsigned int* eventList) {
  m_fahrwegItems = fahrwegItems;
  m_eventList = eventList;
  m_fwi.setFahrwegList(fahrwegItems);

  while (m_fwi.hasMore()) {
    int pos = m_fwi.nextPos();
    m_leds[pos] = m_trackColor;
  }

  m_fwi.setFahrwegList(fahrwegItems);
  ledsChanged = true;
}

void Fahrweg::start() {
  m_timer = AsyncTimer::add(500, -1, this, trainHandler);
}

void Fahrweg::advance() {
  if (m_fwi.hasMore() || !m_train.isEmpty()) {
    short pos = m_fwi.nextPos();
    m_train.advance(pos);
    ledsChanged = true;
  } else {
    AsyncTimer::kill(m_timer);
    m_timer = NULL;
  }
}

bool Fahrweg::done() {
  //Serial.print("FW done? "); Serial.println((long)m_timer);
  return m_timer == NULL;
}
