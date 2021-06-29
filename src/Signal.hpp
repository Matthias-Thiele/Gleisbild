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
    void setMelder(uint8_t melderNo);

  private:
    void send(bool setReset);
    bool m_set;
    int m_ledPos;
    uint8_t m_melderNo;
};

extern Signal signals[10];

#endif