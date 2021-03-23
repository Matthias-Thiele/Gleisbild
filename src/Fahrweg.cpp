#include "Fahrweg.hpp"
extern bool ledsChanged;

static void trainHandler(AsyncTimer *timer, unsigned long now, void* source) {
  Serial.print("Train Handler: ");Serial.println((long)source);
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

  Serial.print("mfwi: "); Serial.print(m_fahrwegItems[0]); Serial.print(" _ "); Serial.print(m_fahrwegItems[1]); Serial.println(".");
  m_fwi.setFahrwegList(m_fahrwegItems);
  CRGB clearColor = {0, 0, 0};

  while (m_fwi.hasMore()) {
    int pos = m_fwi.nextPos();
    Serial.print("Clear: "); Serial.println(pos);
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
  Serial.println("Print FW");
  m_fahrwegItems = fahrwegItems;
  Serial.print("mfwi2: "); Serial.print(m_fahrwegItems[0]); Serial.print(" _ "); Serial.print(m_fahrwegItems[1]); Serial.println(".");
  m_eventList = eventList;
  m_fwi.setFahrwegList(fahrwegItems);

  while (m_fwi.hasMore()) {
    int pos = m_fwi.nextPos();
    Serial.print("Set TrackColor to "); Serial.println(pos);
    m_leds[pos] = m_trackColor;
  }

  m_fwi.setFahrwegList(fahrwegItems);
  ledsChanged = true;
  Serial.print("mfwi3: "); Serial.print(m_fahrwegItems[0]); Serial.print(" _ "); Serial.print(m_fahrwegItems[1]); Serial.println(".");
  Serial.println("Print FW done.");
}

void Fahrweg::start() {
  Serial.println("FW start");
  Serial.print("mfwi7: "); Serial.print(m_fahrwegItems[0]); Serial.print(" _ "); Serial.print(m_fahrwegItems[1]); Serial.println(".");
  m_timer = AsyncTimer::add(500, -1, this, trainHandler);
  Serial.print("mfwi8: "); Serial.print(m_fahrwegItems[0]); Serial.print(" _ "); Serial.print(m_fahrwegItems[1]); Serial.println(".");
}

void Fahrweg::advance() {
  Serial.print("mfwi5: "); Serial.print(m_fahrwegItems[0]); Serial.print(" _ "); Serial.print(m_fahrwegItems[1]); Serial.println(".");
  if (m_fwi.hasMore() || !m_train.isEmpty()) {
    short pos = m_fwi.nextPos();
  Serial.print("mfwi5A: "); Serial.print(m_fahrwegItems[0]); Serial.print(" _ "); Serial.print(m_fahrwegItems[1]); Serial.println(".");
    m_train.advance(pos);
  Serial.print("mfwi5B: "); Serial.print(m_fahrwegItems[0]); Serial.print(" _ "); Serial.print(m_fahrwegItems[1]); Serial.println(".");
    ledsChanged = true;
  } else {
    AsyncTimer::kill(m_timer);
    m_timer = NULL;
  Serial.print("mfwi4: "); Serial.print(m_fahrwegItems[0]); Serial.print(" _ "); Serial.print(m_fahrwegItems[1]); Serial.println(".");
  }
}

bool Fahrweg::done() {
  //Serial.print("FW done? "); Serial.println((long)m_timer);
  return m_timer == NULL;
}
