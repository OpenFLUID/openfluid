/**
  \file DistributedEvent.cpp
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "DistributedEvent.h"

#include <iostream>

namespace mhydasdk { namespace core {

DistributedEvent::DistributedEvent()
{
  m_Infos.clear(); 
  m_Date = wxInvalidDateTime;
  //m_Date.ParseFormat(wxT("2002-05-25 12:25:32"),wxT("%Y-%m-%d %H:%M:%S"));
}

// =====================================================================
// =====================================================================


DistributedEvent::DistributedEvent(wxDateTime Date)
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


bool DistributedEvent::getInfoAsString(wxString Key, wxString *Info)
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


bool DistributedEvent::getInfoAsLong(wxString Key, long *Info)
{

  wxString InfoStr;
  
  return ((getInfoAsString(Key,&InfoStr)) && (InfoStr.ToLong(Info)));  
}

// =====================================================================
// =====================================================================


bool DistributedEvent::getInfoAsDouble(wxString Key, double *Info)
{
  wxString InfoStr;

  return ((getInfoAsString(Key,&InfoStr)) && (InfoStr.ToDouble(Info)));
}

// =====================================================================
// =====================================================================


bool DistributedEvent::getInfoAsMHYDASScalarValue(wxString Key, MHYDASScalarValue *Info)
{
  return getInfoAsDouble(Key,Info);
}


// =====================================================================
// =====================================================================

bool DistributedEvent::addInfo(wxString Key, wxString Info)
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
  std::cerr << "Date : " << m_Date.Format(wxT("%Y-%m-%d %H:%M:%S")).mb_str(wxConvUTF8) << std::endl;
  std::cerr << "Infos : " <<std::endl;
  
  EventInfosMap::iterator it;
  for (it = m_Infos.begin();it != m_Infos.end();++it)
  {
    std::cerr << " - " << it->first.mb_str(wxConvUTF8) << " = " << it->second.mb_str(wxConvUTF8) << std::endl;
  }
}



} }
