#ifndef _FAHRWEG_ITERATOR__
#define _FAHRWEG_ITERATOR__

#define END_OF_FW -1

class FahrwegIterator {
  public:
    void setFahrwegList(int* positionList);
    int nextPos();
    bool hasMore();

  private:
    int *m_Items;
    int m_Start;
    int m_End;
    int m_Direction;
    int m_ActPos;

    void nextItem();
};

#endif