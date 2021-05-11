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
#define NUM_FAHRWEGE 24
#define LED_PIN     5

#define FW_LB_EFS 0
#define FW_LB_T1 1
#define FW_LB_T2 2
#define FW_LB_T3 3
#define FW_LB_T4 4
#define FW_AH_EFS 5
#define FW_AH_T2 6
#define FW_AH_T3 7
#define FW_AH_T4 8
#define FW_WB_EFS 9
#define FW_WB_T1 10
#define FW_WB_T2 11
#define FW_WB_T4 12
#define FW_T1_WB 13
#define FW_T2_WB 14
#define FW_T4_WB 15
#define FW_T1_LB 16
#define FW_T2_LB 17
#define FW_T3_LB 18
#define FW_T4_LB 19
#define FW_T2_AH 20
#define FW_T3_AH 21
#define FW_T4_AH 22


class World {
  public:
    World();
    void addTrain(int startingPoint);
    void test(unsigned long now);

  private:
    CRGB leds[NUM_LEDS];
    Signal signals[NUM_SIGNALS];
    Fahrweg *fahrwege[NUM_FAHRWEGE];

    void initSignals();
    void initFahrstrassen();
};

#endif
