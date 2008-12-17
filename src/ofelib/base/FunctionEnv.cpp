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


bool FunctionEnvironment::setValue(wxString Key, bool Value)
{
  m_BoolValues[Key] = Value;
  return true;
}

// =====================================================================
// =====================================================================


bool FunctionEnvironment::setValue(wxString Key, wxString Value)
{
  m_StringValues[Key] = Value;
  return true;
}

// =====================================================================
// =====================================================================


bool FunctionEnvironment::getValue(wxString Key, bool *Value) const
{
  KeyBoolMap::const_iterator it = m_BoolValues.find(Key);

  if (it != m_BoolValues.end())
  {
    *Value = it->second;
  }
  else return false;
}

// =====================================================================
// =====================================================================


bool FunctionEnvironment::getValue(wxString Key, wxString *Value) const
{
  KeyStringMap::const_iterator it = m_StringValues.find(Key);

  if (it != m_StringValues.end())
  {
    *Value = it->second;
  }
  else return false;

}

// =====================================================================
// =====================================================================



} } // namespace


