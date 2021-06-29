#ifndef _TRAIN__
#define _TRAIN__
#include "./FastLed/FastLED.h"

#define TRAIN_LENGTH 5

class Train {
  public:
    Train();
    void init(CRGB* leds, CRGB trainColor, CRGB trackColor, CRGB occupiedColor);
    void advance(short toPosition);
    void adjust(short startPosition);
    bool isEmpty();
    void redraw();
    short* getPositions();
    void setPositions(short* pos);
    void clear();
    void occupancy(bool isOccupied);
    
  private:
    CRGB* m_leds;
    CRGB  m_trackColor;
    CRGB  m_trainColor;
    CRGB  m_occupiedColor;
    short m_positions[TRAIN_LENGTH];
    bool  m_isOccupied;
};

#endif
