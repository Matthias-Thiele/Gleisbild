#ifndef _FAHRWEG__
#define _FAHRWEG__

#include "Train.hpp"
#include "FahrwegIterator.hpp"
#include "AsyncTimer.hpp"
#include "Signal.hpp"

class Fahrweg {
  public:
    static void setSignals(Signal* signals);
    Fahrweg(CRGB* leds, CRGB trainColor, CRGB trackColor, CRGB occupiedColor);
    void set(short* fahrwegItems, unsigned long* eventList, uint8_t track, boolean isInbound);
    void show(Train* train);
    void start();
    void stop();
    void advance(bool testMode);
    bool done();
    void clear();
    void setBlock(bool isRemote);
    bool isShown();
    Train* getTrain();
    void setTrains(Train *trackTrains);
    uint8_t getTrack();

  private:
    void send(uint8_t value);
    short* m_fahrwegItems;
    unsigned long* m_eventList;
    CRGB* m_leds;
    CRGB  m_trackColor;
    Train m_train;
    bool m_isInbound;
    FahrwegIterator m_fwi;
    bool m_shown;
    AsyncTimer* m_timer;
    bool m_sectionBlockIsRemote;
    Train *m_trackTrains;
    uint8_t m_track;
    bool m_trainRunning;
};

#endif