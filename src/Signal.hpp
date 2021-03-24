#ifndef _SIGNAL__
#define _SIGNAL__

#include "FastLed/FastLED.h"

class Signal {
  public:
    static void setLeds(CRGB* leds);
    Signal();
    void setLedNo(int pos);
    bool isSet();
    void set();
    void release();
    void blank();

  private:
    bool m_set;
    int m_ledPos;
};

extern Signal signals[10];

#endif