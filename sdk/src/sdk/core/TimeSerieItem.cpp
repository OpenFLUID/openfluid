/**
  \file TimeSerieItem.cpp
  \brief


  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include "TimeSerieItem.h"


namespace mhydasdk { namespace core {



TimeSerieItem::TimeSerieItem()
{
  // m_IsValidRawTime = false;
}


// =====================================================================
// =====================================================================


TimeSerieItem::TimeSerieItem(int Year, int Month, int Day, int Hour, int Minute, int Second, float Value)
{

  m_Value = Value;

  m_DateTime = DateTime(Year,Month,Day,Hour,Minute,Second);
}


// =====================================================================
// =====================================================================


int TimeSerieItem::getYear() const
{
  return m_DateTime.getYear();
}


// =====================================================================
// =====================================================================


int TimeSerieItem::getMonth() const
{
  return m_DateTime.getMonth();
}


// =====================================================================
// =====================================================================


int TimeSerieItem::getDay() const
{
  return m_DateTime.getDay();
}


// =====================================================================
// =====================================================================


int TimeSerieItem::getHour() const
{
  return m_DateTime.getHour();
}


// =====================================================================
// =====================================================================


int TimeSerieItem::getMinute() const
{
  return m_DateTime.getMinute();
}


// =====================================================================
// =====================================================================


int TimeSerieItem::getSecond() const
{
  return m_DateTime.getSecond();
}


// =====================================================================
// =====================================================================


rawtime_t TimeSerieItem::getRawTime()
{

  return m_DateTime.getRawTime();
}


// =====================================================================
// =====================================================================


wxString TimeSerieItem::getTimeAsString()
{
  return wxString(m_DateTime.asString().c_str(),wxConvUTF8);
}


// =====================================================================
// =====================================================================


float TimeSerieItem::getValue()
{
  return m_Value;
}


// =====================================================================
// =====================================================================


void TimeSerieItem::setValue(float Value)
{
  m_Value = Value;
}


// =====================================================================
// =====================================================================


} } //namespace mhydasdk::core

