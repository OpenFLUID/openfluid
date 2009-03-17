

#ifndef __SSERIEVALUES_H__
#define __SSERIEVALUES_H__


#include <vector>
#include <iostream>

namespace openfluid { namespace core {

class StepsReservation
{
  private :
    static unsigned int Reserved;

  public :
    static unsigned int getReservation() { return Reserved; };

    static void setReservation(unsigned int aReservation) { Reserved = aReservation; };


};


template <class T>
class StepSerieOfValues : public StepsReservation
{
  //TODO static values for packet size and keep???


  private:



    unsigned int m_BaseStepIndex;
    std::vector<T> m_Data;

    unsigned int m_NextStep;

  public:


    StepSerieOfValues();

    ~StepSerieOfValues();

    bool deleteValues(const unsigned int UntilStepNbr);

    bool getValue(const unsigned int StepNbr,T* Value) const;

    bool modifyValue(const unsigned int StepNbr, T Value);

    bool appendValue(const T Value);

    unsigned int getNextStep() const;

    void displayStatus();

};


// =====================================================================
// =====================================================================

template <class T>
StepSerieOfValues<T>::StepSerieOfValues()
{

  m_BaseStepIndex = 0;
  m_NextStep = 0;

  m_Data.clear();
  m_Data.reserve(Reserved);
  m_Data.clear();

}

// =====================================================================
// =====================================================================

template <class T>
StepSerieOfValues<T>::~StepSerieOfValues()
{

}

// =====================================================================
// =====================================================================


template <class T>
bool StepSerieOfValues<T>::deleteValues(const unsigned int UntilStepNbr)
{
  // TODO clean comments
  // compute the real index of UntilStepNbr in m_Data
  unsigned int RealEnd = UntilStepNbr - m_BaseStepIndex +1;

  if (RealEnd < 0) return false;

//  std::cerr << "Erasing from 0 to " << RealEnd << " | ";

  m_Data.erase(m_Data.begin(),m_Data.begin()+RealEnd);

  m_Data.reserve(Reserved);

//  std::cerr << UntilStepNbr << ", " << m_BaseStepIndex << " -> ";

  // compute the new BaseIndex
  m_BaseStepIndex = UntilStepNbr+1;

//  std::cerr << " " << m_BaseStepIndex << std::endl;

  return true;
}


// =====================================================================
// =====================================================================

template <class T>
bool StepSerieOfValues<T>::getValue(const unsigned int StepNbr,T* Value) const
{

  if (StepNbr < m_BaseStepIndex || StepNbr-m_BaseStepIndex >= m_Data.size()) return false;

  *Value = m_Data[StepNbr-m_BaseStepIndex];

  return true;
}


// =====================================================================
// =====================================================================

template <class T>
bool StepSerieOfValues<T>::modifyValue(const unsigned int StepNbr, T Value)
{
  if (StepNbr < m_BaseStepIndex) return false;

  m_Data[StepNbr-m_BaseStepIndex] = Value;

  return true;
}


// =====================================================================
// =====================================================================

template <class T>
bool StepSerieOfValues<T>::appendValue(const T Value)
{

  m_Data.push_back(Value);
  m_NextStep++;

  return true;
}


// =====================================================================
// =====================================================================


template <class T>
unsigned int StepSerieOfValues<T>::getNextStep() const
{
  return m_NextStep;
}


// =====================================================================
// =====================================================================


template <class T>
void StepSerieOfValues<T>::displayStatus()
{
  std::cout << "-- StepSerieOfValues status --"<< std::endl;
  std::cout << "   Reserved : " << Reserved << std::endl;
  std::cout << "   Size : " << m_Data.size() << std::endl;
  std::cout << "   Element size : " << sizeof(T) << std::endl;
  std::cout << "   BaseStepIndex : " << m_BaseStepIndex << std::endl;
  std::cout << "   Current storage step : " << m_NextStep-1 << std::endl;
  std::cout << "------------------------------"<< std::endl;
}


// =====================================================================
// =====================================================================


} } // namespaces



#endif /* __SSERIEVALUES_H__ */
