#ifndef _TRAIN__
#define _TRAIN__
#include "./FastLed/FastLED.h"

#define TRAIN_LENGTH 5

class Train {
  public:
    void init(CRGB* leds, CRGB trainColor, CRGB trackColor);
    void advance(short toPosition);
    bool isEmpty();
    void redraw();
    short* getPositions();
    void setPositions(short* pos);
    void clear();
    
  private:
    CRGB* m_leds;
    CRGB  m_trackColor;
    CRGB  m_trainColor;
    short m_positions[TRAIN_LENGTH];
};

#endif
