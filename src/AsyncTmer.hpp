#ifndef _ASYNC_TIMER_H__
#define _ASYNC_TIMER_H__

#include <Arduino.h>

#define MAX_TIMER 20

typedef void (*handler_t)(class AsyncTimer*, unsigned long);
static AsyncTimer* timerList[MAX_TIMER];

class AsyncTimer {

  private:
    unsigned long m_delayMillis;
    unsigned long m_nextShot;
    unsigned long m_param;
    int m_repeatCount;
    int m_activated;
    bool m_isOdd;
    handler_t m_handler;

    AsyncTimer() {

    }

    AsyncTimer(unsigned long delayMillis, int repeatCount, unsigned long param, handler_t handler) {
      m_delayMillis = delayMillis;
      m_repeatCount = repeatCount;
      m_handler = handler;
      m_nextShot = 0ul;
      m_isOdd = false;
      m_activated = 0;
      m_param = param;
    }

    void tick(unsigned long now) {
      if (now >= m_nextShot) {
        m_activated++;
        Serial.print("Tick: "); Serial.print(now); Serial.print(" - "); Serial.println(m_nextShot);
        m_handler(this, now);
        if (m_repeatCount > 0) {
          m_repeatCount--;
        }

        if (m_repeatCount == 0) {
          kill(this);
        }

        m_nextShot += m_delayMillis;
        m_isOdd = !m_isOdd;
      }
    }

  public:
    static AsyncTimer* add(unsigned long delayMillis, int repeatCount, unsigned long param, handler_t handler) {
      Serial.println("add Timer");
      for (int t = 0; t < MAX_TIMER; t++) {
        if (timerList[t] == NULL) {
          Serial.println("free slot found");
          AsyncTimer *newTimer = new AsyncTimer(delayMillis, repeatCount, param, handler);
          timerList[t] = newTimer;
          return newTimer;
        }
      }

      return NULL;
    }

    static void tick() {
      unsigned long now = millis();
      for (int t = 0; t < MAX_TIMER; t++) {
        if (timerList[t] != NULL) {
          timerList[t]->tick(now);
        }
      }
    }

    static void kill(AsyncTimer *timer) {
      for (int t = 0; t < MAX_TIMER; t++) {
        if (timerList[t] == timer) {
          free(timerList[t]);
          timerList[t] = NULL;
        }
      }
    }

    bool isOdd() {
      return m_isOdd;
    }

    int getRepeatCount() {
      return m_repeatCount;
    }

    int getActivated() {
      return m_activated;
    }

    unsigned long getParam() {
      return m_param;
    }

    void setParam(unsigned long param) {
      m_param = param;
    }

};


#endif