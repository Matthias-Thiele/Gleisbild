#ifndef _WORLD__
#define _WORLD__

#include "FastLed/FastLED.h"
#include "Signal.hpp"
#include "Fahrweg.hpp"

#define FROM_LIEBENZELL 1
#define FROM_ALTHENGSTETT 2
#define FROM_WILDBERG 3
#define FROM_CALW 4

#define NUM_LEDS 580
#define NUM_SIGNALS 16
#define NUM_FAHRWEGE 23
#define LED_PIN     5

#define SIG_EINF_LB 10
#define SIG_AUSF_LB 11

#define FW_LB_EFS 20
#define FW_LB_T1 0
#define FW_LB_T2 1
#define FW_LB_T3 2
#define FW_LB_T4 3
#define FW_AH_EFS 21
#define FW_AH_T2 4
#define FW_AH_T3 5
#define FW_AH_T4 6
#define FW_WB_EFS 22
#define FW_WB_T1 7
#define FW_WB_T2 8
#define FW_WB_T4 9
#define FW_T1_LB 10
#define FW_T2_LB 11
#define FW_T3_LB 12
#define FW_T4_LB 13
#define FW_T2_AH 14
#define FW_T3_AH 15
#define FW_T4_AH 16
#define FW_T1_WB 17
#define FW_T2_WB 18
#define FW_T4_WB 19


class World {
  public:
    World();
    void addTrain(int startingPoint);
    void test(unsigned long now);
    void process(unsigned long now);
    void setSignal(int num, bool value);
    void processCommand(uint8_t cmd);
    void processDrs2Command(unsigned long now, uint8_t cmd);

  private:
    CRGB leds[NUM_LEDS];
    Signal signals[NUM_SIGNALS];
    Train trackTrains[4];
    Fahrweg *fahrwege[NUM_FAHRWEGE];
    bool m_isTestMode;
    uint8_t m_lastStart = 0;
    unsigned long lastStep = 0;

    Fahrweg *m_fromLB = NULL;
    Fahrweg *m_fromAH = NULL;
    Fahrweg *m_fromWB = NULL;
    bool m_streckeIsRemote[3];
    bool m_streckeIsFree[3];
    bool m_signalLBActive = false;
    bool m_signalAHActive = false;

    bool m_befehlNothing = true;
    bool m_befehlAusfahrtWB = false;
    bool m_befehlEinfahrtWB = false;
    unsigned long m_ActivateEinfahrt = UINT32_MAX;
    unsigned long m_ActivateAusfahrt = UINT32_MAX;
    unsigned long m_DecativateDrs2 = 0;

    bool m_Pause;

    bool m_DRS_IS_FROM_LB;
    
    void send(uint8_t value);
    void initSignals();
    void initFahrstrassen();
    void checkStartTrain(uint8_t source);
    void startTrain(uint8_t source);
    void updateStreckenblock(uint8_t source);
    void setFahrstrasse(uint8_t source);
    void changeFW(uint8_t fwNum, bool setClear, Train* sourceTrai);
    void checkSignal(uint8_t fsNum);
    void checkSetDurchfahrt();
    void updateDrs2(uint8_t source);
    Fahrweg* selectFW(uint8_t noSelection, uint8_t firstNum, uint8_t lastNum);
};

#endif
