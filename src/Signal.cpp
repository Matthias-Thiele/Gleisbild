#include "Signal.hpp"

#define MAX_SIGNAL 10
Signal signals[10];
CRGB* m_leds;
extern bool ledsChanged;

void Signal::setLeds(CRGB* leds) {
  m_leds = leds;
}

Signal::Signal() {
  m_set = false;
  m_melderNo = 0xff;
}

void Signal::setLedNo(int ledPos) {
  m_ledPos = ledPos;
}

bool Signal::isSet() {
  return m_set;
}

void Signal::setMelder(uint8_t melderNo) {
  m_melderNo = melderNo;
}

void Signal::send(bool setReset) {
  if (m_melderNo != 0xff) {
    uint8_t value = 0xe0 | (m_melderNo << 1) | (!setReset & 1);
    Serial.print("Send: "); Serial.println(value, HEX);
    Serial1.write(value);
  }
}
void Signal::set() {
  m_set = true;
  m_leds[m_ledPos] = CRGB::Green;
  ledsChanged = true;
  send(true);
}

void Signal::release() {
  m_set = false;
  m_leds[m_ledPos] = CRGB::Red;
  ledsChanged = true;
  send(false);
}

void Signal::blank() {
  m_set = false;
  m_leds[m_ledPos] = CRGB::Black;
  ledsChanged = true;
}