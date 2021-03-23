#include "Train.hpp"

void Train::init(CRGB* leds, CRGB trainColor, CRGB trackColor) {
  m_leds = leds;
  m_trackColor = trackColor;
  m_trainColor = trainColor;

  for (int i = 0; i < TRAIN_LENGTH; i++) {
    m_positions[i] = -1;
  }
}

void Train::advance(short toPosition) {
  short lastPos = m_positions[TRAIN_LENGTH - 1];
  if (lastPos != -1) {
    m_leds[lastPos] = m_trackColor;
  }

  for (int i = TRAIN_LENGTH - 2; i >= 0; i--) {
    m_positions[i + 1] = m_positions[i];
  }

  m_positions[0] = toPosition;
  if (toPosition >= 0) {
    m_leds[toPosition] = m_trainColor;
  }
}

bool Train::isEmpty() {
  for (int i = 0; i < TRAIN_LENGTH; i++) {
    if (m_positions[i] != -1) {
      return false;
    }
  }

  return true;
}