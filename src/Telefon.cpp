#include "Telefon.hpp"

/**
 * Klasse zur Abfrage der drei Telefonlinien zur
 * Ankündigung (und damit Start) eines neuen Zugs.
 **/
Telefon::Telefon(uint8_t pinLB, uint8_t pinAH, uint8_t pinWB) {
  m_pinLB.init(pinLB, 50);
  m_pinAH.init(pinAH, 50);
  m_pinWB.init(pinWB, 50);
  m_lastOutput = 0xaa;
}

/**
 * Liefert den aktuellen Stand der drei Telefonlinien
 * zurück, LB in Bit 0, AH in Bit 1 und WB in Bit 2
 * Input ist active low, im Programm jedoch active high
 **/
uint8_t Telefon::getState() {
  uint8_t result = 0;
  if (!m_pinLB.read()) {
    result |= 1;
  }
  if (!m_pinAH.read()) {
    result |= 2;
  }
  if (!m_pinWB.read()) {
    result |= 4;
  }

  if (result != m_lastOutput) {
    Serial.print("Telefon: "); Serial.println(result);
    m_lastOutput = result;
  }
  
  return result;
}

void Telefon::tick(unsigned long now) {
  m_pinLB.tick(now);
  m_pinAH.tick(now);
  m_pinWB.tick(now);
}