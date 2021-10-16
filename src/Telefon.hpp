#ifndef _TELEFON__
#define _TELEFON__

#include "Arduino.h"
#include "DebouncedPin.hpp"

class Telefon {
public:
  Telefon(uint8_t pinLB, uint8_t pinAH, uint8_t pinWB);
  uint8_t getState();
  void tick(unsigned long now);

private:
  DebouncedPin m_pinLB;
  DebouncedPin m_pinAH;
  DebouncedPin m_pinWB;

  uint8_t m_lastOutput;
  
};

#endif
