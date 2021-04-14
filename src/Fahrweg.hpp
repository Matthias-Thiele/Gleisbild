#ifndef _FAHRWEG__
#define _FAHRWEG__

#include "Train.hpp"
#include "FahrwegIterator.hpp"
#include "AsyncTimer.hpp"

class Fahrweg {
  public:
    Fahrweg(CRGB* leds, CRGB trainColor, CRGB trackColor);
    void set(short* fahrwegItems, unsigned long* eventList);
    void start();
    void advance();
    bool done();
    void clear();

  private:
    short* m_fahrwegItems;
    unsigned long* m_eventList;
    CRGB* m_leds;
    CRGB  m_trackColor;
    Train m_train;
    FahrwegIterator m_fwi;
    AsyncTimer* m_timer;
};

#endif