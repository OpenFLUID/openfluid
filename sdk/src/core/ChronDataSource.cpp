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
  mp_ProcessedData = NULL;
  m_TimeSerie = NULL;

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


// =====================================================================
// =====================================================================

void ChronDataSource::Process()
{

  int Count = m_TimeSerie->getItemsCollection()->size();
  
  mp_ProcessedData = new RainValue[Count];
  
  for (int i=0; i<Count; i++)
  {
    mp_ProcessedData[i] = m_TimeSerie->getItemsCollection()->at(i)->getValue();
  }  

  
}


} } // namespace mhydasdk::core




