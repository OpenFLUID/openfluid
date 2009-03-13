/**
  \file DistributedEvent.cpp
  \brief implements distributed event definition class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "DistributedEvent.h"
#include "openfluid-tools.h"
#include <iostream>

namespace openfluid { namespace core {

DistributedEvent::DistributedEvent()
{
  m_Infos.clear();
  m_Date = DateTime();
}

// =====================================================================
// =====================================================================


DistributedEvent::DistributedEvent(DateTime Date)
{
  m_Infos.clear();
  m_Date = Date;
}

// =====================================================================
// =====================================================================


DistributedEvent::~DistributedEvent()
{

}


// =====================================================================
// =====================================================================


bool DistributedEvent::isInfoExists(std::string Key)
{
  return !(m_Infos.find(Key) == m_Infos.end());
}

// =====================================================================
// =====================================================================


bool DistributedEvent::isInfoEquals(std::string Key, std::string Value)
{
  std::string TmpValue;

  return (getInfoAsString(Key,&TmpValue) && (TmpValue == Value));

}

// =====================================================================
// =====================================================================


bool DistributedEvent::isInfoEquals(std::string Key, long Value)
{
 long TmpValue;

  return (getInfoAsLong(Key,&TmpValue) && (TmpValue == Value));

}

// =====================================================================
// =====================================================================


bool DistributedEvent::isInfoEquals(std::string Key, double Value)
{
  double TmpValue;

  return (getInfoAsDouble(Key,&TmpValue) && (TmpValue == Value));

}

// =====================================================================
// =====================================================================


bool DistributedEvent::isInfoEquals(std::string Key, ScalarValue *Value)
{
  ScalarValue TmpValue;

  return (getInfoAsScalarValue(Key,&TmpValue) && (TmpValue == (*Value)));

}


// =====================================================================
// =====================================================================


bool DistributedEvent::getInfoAsString(std::string Key, std::string *Info)
{

  if (m_Infos.find(Key) == m_Infos.end())
  {
    return false;
  }
  else *Info = m_Infos[Key];

  return true;
}

// =====================================================================
// =====================================================================


bool DistributedEvent::getInfoAsLong(std::string Key, long *Info)
{

  std::string InfoStr;

  return ((getInfoAsString(Key,&InfoStr)) && (openfluid::tools::ConvertString(InfoStr,Info)));
}

// =====================================================================
// =====================================================================


bool DistributedEvent::getInfoAsDouble(std::string Key, double *Info)
{
  std::string InfoStr;

  return ((getInfoAsString(Key,&InfoStr)) && (openfluid::tools::ConvertString(InfoStr,Info)));
}

// =====================================================================
// =====================================================================


bool DistributedEvent::getInfoAsScalarValue(std::string Key, ScalarValue *Info)
{
  return getInfoAsDouble(Key,Info);
}


// =====================================================================
// =====================================================================

bool DistributedEvent::addInfo(std::string Key, std::string Info)
{
  if (m_Infos.find(Key) != m_Infos.end())
  {
    return false;
  }
  else m_Infos[Key] = Info;

  return true;
}

// =====================================================================
// =====================================================================


void DistributedEvent::println()
{
  std::cerr << "Date : " << m_Date.getAsISOString() << std::endl;
  std::cerr << "Infos : " <<std::endl;

  EventInfosMap_t::iterator it;
  for (it = m_Infos.begin();it != m_Infos.end();++it)
  {
    std::cerr << " - " << it->first << " = " << it->second << std::endl;
  }
}



} }
