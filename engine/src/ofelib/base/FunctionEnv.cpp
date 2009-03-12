/**
  \file PlugFunction.cpp
  \brief implementation file for function environment

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "FunctionEnv.h"

namespace openfluid { namespace base {



FunctionEnvironment::FunctionEnvironment()
{


}

// =====================================================================
// =====================================================================

FunctionEnvironment::~FunctionEnvironment()
{

}

// =====================================================================
// =====================================================================


bool FunctionEnvironment::setValue(std::string Key, bool Value)
{
  m_BoolValues[Key] = Value;
  return true;
}

// =====================================================================
// =====================================================================


bool FunctionEnvironment::setValue(std::string Key, std::string Value)
{
  m_StringValues[Key] = Value;
  return true;
}

// =====================================================================
// =====================================================================


bool FunctionEnvironment::getValue(std::string Key, bool *Value) const
{
  KeyBoolMap_t::const_iterator it = m_BoolValues.find(Key);

  if (it != m_BoolValues.end())
  {
    *Value = it->second;
  }
  else return false;
}

// =====================================================================
// =====================================================================


bool FunctionEnvironment::getValue(std::string Key, std::string *Value) const
{
  KeyStringMap_t::const_iterator it = m_StringValues.find(Key);

  if (it != m_StringValues.end())
  {
    *Value = it->second;
  }
  else return false;

}

// =====================================================================
// =====================================================================



} } // namespace


