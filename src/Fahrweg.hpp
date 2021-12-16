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
    void setFreeState(bool isFree);
    bool isShown();
    bool isRunning();
    void setRunning();
    void setDurchfahrt(bool isDurchfahrt);
    Train* getTrain();
    void clearTrain();
    void setTrains(Train *trackTrains);
    uint8_t getTrack();

  private:
    void send(uint8_t value);
    void sendTrainArrived(uint8_t source);
    void checkDurchfahrt();
    void processDrsTrack(uint8_t part);
    void sendDrs2(uint8_t cmd1, uint8_t cmd2);
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
    bool m_sectionIsFree;
    Train *m_trackTrains;
    uint8_t m_track;
    short m_trainRunning;
    bool m_arrivalSent = false;
    bool m_isDurchfahrt;
    unsigned long m_StartDelayTime;
};

#endif