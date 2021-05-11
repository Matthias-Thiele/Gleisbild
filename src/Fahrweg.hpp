#ifndef _FAHRWEG__
#define _FAHRWEG__

#include "Train.hpp"
#include "FahrwegIterator.hpp"
#include "AsyncTimer.hpp"
#include "Signal.hpp"

class Fahrweg {
  public:
    static void setSignals(Signal* signals);
    Fahrweg(CRGB* leds, CRGB trainColor, CRGB trackColor);
    void set(short* fahrwegItems, unsigned long* eventList, boolean isInbound);
    void show();
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
    bool m_isInbound;
    FahrwegIterator m_fwi;
    bool m_shown;
    AsyncTimer* m_timer;
};

#endif