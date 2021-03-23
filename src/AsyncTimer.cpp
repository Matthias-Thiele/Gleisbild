#include "AsyncTimer.hpp"

AsyncTimer* timerList[MAX_TIMER];

AsyncTimer::AsyncTimer(unsigned long delayMillis, int repeatCount, void* param, handler_t handler) {
  m_delayMillis = delayMillis;
  m_repeatCount = repeatCount;
  m_handler = handler;
  m_nextShot = 0ul;
  m_isOdd = false;
  m_activated = 0;
  m_param = param;
}

void AsyncTimer::tick(unsigned long now) {
  if (now >= m_nextShot) {
    Serial.print("Tick - now: "); Serial.print(now); Serial.print(", next: "); Serial.println(m_nextShot); delay(100);
    m_activated++;
    m_handler(this, now, m_param);
    if (m_repeatCount > 0) {
      m_repeatCount--;
    }

    if (m_repeatCount == 0) {
      kill(this);
    }

    if (m_nextShot == 0) {
      m_nextShot = now;
    }
    
    m_nextShot += m_delayMillis;
    m_isOdd = !m_isOdd;
  }
}

AsyncTimer* AsyncTimer::add(unsigned long delayMillis, int repeatCount, void* param, handler_t handler) {
  Serial.print("add Timer to list "); Serial.println((long)timerList);
  for (int t = 0; t < MAX_TIMER; t++) {
    if (timerList[t] == NULL) {
      Serial.println("free slot found");
      AsyncTimer *newTimer = new AsyncTimer(delayMillis, repeatCount, param, handler);
      timerList[t] = newTimer;
      Serial.print("New Timer added at slot "); Serial.println(t);
      for (int i = 0; i < MAX_TIMER; i++) {Serial.print((long)timerList[i]); Serial.print(" : ");}
      return newTimer;
    }
  }

  return NULL;
}

void AsyncTimer::tick() {
  unsigned long now = millis();
  for (int t = 0; t < MAX_TIMER; t++) {
    if (timerList[t] != NULL) {
      timerList[t]->tick(now);
    }
  }
}

void AsyncTimer::kill(AsyncTimer *timer) {
  Serial.println("Kill Timer");
  for (int t = 0; t < MAX_TIMER; t++) {
    if (timerList[t] == timer) {
      free(timerList[t]);
      timerList[t] = NULL;
    }
  }
}
