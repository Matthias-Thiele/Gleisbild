#ifndef _ASYNC_TIMER_H__
#define _ASYNC_TIMER_H__

#include <Arduino.h>

#define MAX_TIMER 20

typedef void (*handler_t)(class AsyncTimer*, unsigned long, void*);


class AsyncTimer {

  private:
    unsigned long m_delayMillis;
    unsigned long m_nextShot;
    void* m_param;
    int m_repeatCount;
    int m_activated;
    bool m_isOdd;
    handler_t m_handler;

    AsyncTimer() {

    }

    AsyncTimer(unsigned long delayMillis, int repeatCount, void* param, handler_t handler);
    void tick(unsigned long now);

  public:
    static AsyncTimer* add(unsigned long delayMillis, int repeatCount, void* param, handler_t handler);
    static void tick();
    static void kill(AsyncTimer *timer);

    bool isOdd() {
      return m_isOdd;
    }

    int getRepeatCount() {
      return m_repeatCount;
    }

    int getActivated() {
      return m_activated;
    }

};


#endif