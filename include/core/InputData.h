
#ifndef __INPUTDATA_H__
#define __INPUTDATA_H__

#include "TypeDefs.h"

namespace openfluid { namespace core {


template <class T>
class InputData
{
  private :

    typedef std::map<InputDataName_t,T> InputDataMap_t;
    InputDataMap_t m_Data;

  public :

    InputData();

    ~InputData();

    bool setValue(const InputDataName_t aName,const T aValue);

    bool getValue(const InputDataName_t aName, T *aValue);

    bool isDataExist(const InputDataName_t aName) const;

    std::vector<InputDataName_t> getInputDataNames() const;


};


// =====================================================================
// =====================================================================


template <class T>
InputData<T>::InputData()
{

}


// =====================================================================
// =====================================================================


template <class T>
InputData<T>::~InputData()
{

}


// =====================================================================
// =====================================================================

template <class T>
bool InputData<T>::setValue(const InputDataName_t aName,const T aValue)
{

  if (isDataExist(aName)) return false;
  else
  {
    m_Data[aName] = aValue;
  }
  return true;
}


// =====================================================================
// =====================================================================


template <class T>
bool InputData<T>::getValue(const InputDataName_t aName, T *aValue)
{
  typename InputDataMap_t::iterator it = m_Data.find(aName);

  if (it != m_Data.end())
  {
    // TODO Check if by reference is possible
    *aValue = it->second;
    return true;
  }
  else return false;
}

// =====================================================================
// =====================================================================

template <class T>
bool InputData<T>::isDataExist(const InputDataName_t aName) const
{
  return m_Data.find(aName) != m_Data.end();
}

// =====================================================================
// =====================================================================

template <class T>
std::vector<InputDataName_t> InputData<T>::getInputDataNames() const
{
  std::vector<InputDataName_t> TheNames;

  typename InputDataMap_t::const_iterator it;

  for (it = m_Data.begin() ; it != m_Data.end() ; ++it)
  {
    TheNames.push_back(it->first);
  }

  return TheNames;

}

} } // namespaces



#endif /* __INPUTDATA_H__ */
