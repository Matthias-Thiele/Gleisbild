#include "Train.hpp"

Train::Train() {
  clear();
}

void Train::init(CRGB* leds, CRGB trainColor, CRGB trackColor, CRGB occupiedColor) {
  m_leds = leds;
  m_trackColor = trackColor;
  m_trainColor = trainColor;
  m_occupiedColor = occupiedColor;
  clear();
}

void Train::clear() {
  m_isOccupied = false;
  for (int i = 0; i < TRAIN_LENGTH; i++) {
    m_positions[i] = -1;
  }
}

void Train::occupancy(bool isOccupied) {
  Serial.print("Is occupied: "); Serial.println(isOccupied);
  m_isOccupied = isOccupied;
  redraw();
}

void Train::adjust(short firstPosition) {
  if ((m_positions[0] == firstPosition) && (m_positions[TRAIN_LENGTH - 1] != firstPosition)) {
    short tmp = m_positions[0];
    m_positions[0] = m_positions[TRAIN_LENGTH - 1];
    m_positions[TRAIN_LENGTH - 1] = tmp;
    tmp = m_positions[1];
    m_positions[1] = m_positions[TRAIN_LENGTH - 2];
    m_positions[TRAIN_LENGTH - 2] = tmp;
  }
}

void Train::advance(short toPosition) {
  //Serial.print("Train advance to "); Serial.println(toPosition);
  short lastPos = m_positions[TRAIN_LENGTH - 1];
  if (lastPos != -1) {
    m_leds[lastPos] = m_trackColor;
  }

  for (int i = TRAIN_LENGTH - 2; i >= 0; i--) {
    m_positions[i + 1] = m_positions[i];
  }

  m_positions[0] = toPosition;
  if (toPosition >= 0) {
    m_leds[toPosition] = m_isOccupied ? m_occupiedColor : m_trainColor;
  }
}

short* Train::getPositions() {
  Serial.print("getPositions "); for (uint8_t i = 0; i < TRAIN_LENGTH; i++) {Serial.print(m_positions[i]); Serial.print(" - ");} Serial.println();
  return m_positions;
}

void Train::setPositions(short* pos) {
  Serial.print("setPositions "); Serial.print(*pos); Serial.print(" ");
  for (uint8_t i = 0; i < TRAIN_LENGTH; i++) {
    m_positions[i] = *pos++;
    Serial.print(m_positions[i]);
  }
  Serial.println();
}

void Train::redraw() {
  Serial.print("Train redraw "); Serial.print((long)this, HEX); Serial.print(", col: "); Serial.print(m_trainColor, HEX);
  for (uint8_t i = 0; i < TRAIN_LENGTH; i++) {
    int toPosition = m_positions[i];
    Serial.print(" "); Serial.print(toPosition);
    if (toPosition != -1) {
      m_leds[toPosition] = m_isOccupied ? m_occupiedColor : m_trainColor;
    }
  }
  Serial.println();
}

bool Train::isEmpty() {
  for (uint8_t i = 0; i < TRAIN_LENGTH; i++) {
    if (m_positions[i] != -1) {
      return false;
    }
  }

  return true;
}