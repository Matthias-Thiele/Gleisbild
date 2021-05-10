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
}

void Signal::setLedNo(int ledPos) {
  m_ledPos = ledPos;
}

bool Signal::isSet() {
  return m_set;
}

void Signal::set() {
  Serial.print("Set to green "); Serial.println(m_ledPos);
  m_set = true;
  m_leds[m_ledPos] = CRGB::Green;
  ledsChanged = true;
}

void Signal::release() {
  m_set = false;
  m_leds[m_ledPos] = CRGB::Red;
  ledsChanged = true;
}

void Signal::blank() {
  m_set = false;
  m_leds[m_ledPos] = CRGB::Black;
  ledsChanged = true;
}