
#ifndef __VARIABLES_H__
#define __VARIABLES_H__


#include "TypeDefs.h"
#include "SSerieValues.h"

namespace openfluid { namespace core {


template <class T>
class Variables
{
  private :

    typedef std::map<VariableName_t,StepSerieOfValues<T> > VariablesMap_t;
    VariablesMap_t m_Data;


  public :

    Variables();

    ~Variables();

    bool createVariable(const VariableName_t aName);

    bool modifyValue(const VariableName_t aName, const TimeStep_t aStep, const T aValue);

    bool appendValue(const VariableName_t aName, const T aValue);

    bool getValue(const VariableName_t aName, const TimeStep_t aStep, T *aValue) const;

    bool isVariableExists(const VariableName_t aName) const;

    std::vector<VariableName_t> getVariablesNames() const;

    unsigned int getVariableValuesCount(const VariableName_t aName) const;

    bool releaseMemory(TimeStep_t Step);



};


// =====================================================================
// =====================================================================


template <class T>
Variables<T>::Variables()
{

}


// =====================================================================
// =====================================================================


template <class T>
Variables<T>::~Variables()
{

}

// =====================================================================
// =====================================================================


template <class T>
bool Variables<T>::createVariable(const VariableName_t aName)
{
  if (!isVariableExists(aName))
  {
    m_Data[aName];
    return true;
  }
  return false;
}

// =====================================================================
// =====================================================================

template <class T>
bool Variables<T>::modifyValue(const VariableName_t aName, const TimeStep_t aStep, const T aValue)
{
  if (isVariableExists(aName))
  {
    return m_Data[aName].modifyValue(aStep,aValue);
  }
  return false;
}


// =====================================================================
// =====================================================================


template <class T>
bool Variables<T>::appendValue(const VariableName_t aName, const T aValue)
{
  if (isVariableExists(aName))
  {
     return m_Data[aName].appendValue(aValue);
  }
  return false;
}

// =====================================================================
// =====================================================================


template <class T>
bool Variables<T>::getValue(const VariableName_t aName, const TimeStep_t aStep, T* aValue) const
{

  typename VariablesMap_t::const_iterator it = m_Data.find(aName);

//  std::cout << "Variables::getValue-a " << aStep << std::endl; std::cout.flush();

  if (it != m_Data.end())
  {
//    std::cout << "Variables::getValue-b " << aStep << std::endl; std::cout.flush();
    return it->second.getValue(aStep,aValue);
  }
//  std::cout << "Variables::getValue-d " << aStep << std::endl; std::cout.flush();
  return false;
}

// =====================================================================
// =====================================================================

template <class T>
bool Variables<T>::isVariableExists(const VariableName_t aName) const
{

  return m_Data.find(aName) != m_Data.end();
}


// =====================================================================
// =====================================================================

template <class T>
std::vector<VariableName_t> Variables<T>::getVariablesNames() const
{
  std::vector<VariableName_t> TheNames;

  typename VariablesMap_t::const_iterator it;

  for (it = m_Data.begin() ; it != m_Data.end() ; ++it)
  {
    TheNames.push_back(it->first);
  }

  return TheNames;

}


// =====================================================================
// =====================================================================

template <class T>
bool Variables<T>::releaseMemory(TimeStep_t Step)
{
  typename VariablesMap_t::iterator it;

  for (it = m_Data.begin();it != m_Data.end();++it)
  {
    it->second.deleteValues(Step);
  }
  return true;
}

// =====================================================================
// =====================================================================

template <class T>
unsigned int Variables<T>::getVariableValuesCount(const VariableName_t aName) const
{
  // TODO check this

  typename VariablesMap_t::const_iterator it;

  it = m_Data.find(aName);

  if (it = m_Data.end()) return -1;

  else return it->second.getNextStep();
}


} } // namespaces




#endif /* __VARIABLES_H_ */
