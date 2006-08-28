/**
  \file ChronDataSource.cpp
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#include "ChronDataSource.h"


// =====================================================================
// =====================================================================

namespace mhydasdk { namespace core {


ChronDataSource::ChronDataSource(cdsid_t ID)
{
  m_ID = ID;
  m_Name = "";

}


// =====================================================================
// =====================================================================


ChronDataSource::~ChronDataSource()
{

};


// =====================================================================
// =====================================================================


void ChronDataSource::setTimeSerie(TimeSerie *Serie)
{
  m_TimeSerie = Serie;
}


// =====================================================================
// =====================================================================


TimeSerie *ChronDataSource::getTimeSerie()
{

  return m_TimeSerie;
}


// =====================================================================
// =====================================================================


cdsid_t ChronDataSource::getID()
{
  return m_ID;
}


// =====================================================================
// =====================================================================


std::string ChronDataSource::getIDName() const
{
  char Buf[10];

  int Length = snprintf(Buf, 10, "%d", m_ID);

  return std::string(Buf, Length)+" ("+m_Name+")";
}



} } // namespace mhydasdk::core




