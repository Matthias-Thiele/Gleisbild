#include "FahrwegIterator.hpp"

void FahrwegIterator::setFahrwegList(short* positionList) {
  m_Items = positionList;
  nextItem();
}

bool FahrwegIterator::hasMore() {
  return m_ActPos != END_OF_FW;
}

int FahrwegIterator::peekPos() {
  return m_ActPos;
}

int FahrwegIterator::nextPos() {
  int result = m_ActPos;
  if (result != END_OF_FW) {
    if (m_ActPos == m_End) {
      nextItem();
    } else {
      m_ActPos += m_Direction;
    }
  }

  return result;
}

void FahrwegIterator::nextItem() {
  if (!m_Items) {
    m_Start = END_OF_FW;
    return;
  }

  m_Start = *m_Items++;
  if (m_Start != END_OF_FW) {
    m_End = *m_Items++;
    m_ActPos = m_Start;
    m_Direction = (m_End > m_Start) ? 1 : -1;
  } else {
    m_ActPos = END_OF_FW;
  }
}