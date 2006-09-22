/**
  \file DateTime.cpp
  \brief implements date-time managment class

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "DateTime.h"
#include <iostream>

#include <wx/datetime.h>

namespace mhydasdk { namespace core {


DateTime::DateTime()
{
  DateTime(1900, 1, 1, 0, 0, 0);
}

// =====================================================================
// =====================================================================


DateTime::DateTime(int Year, int Month, int Day, int Hour, int Minute, int Second)
{
  m_Year = Year;
  m_Month = Month;
  m_Day = Day;
  m_Hour = Hour;
  m_Min = Minute;
  m_Sec = Second;

  m_CalculatedRawTime = false;

}


// =====================================================================
// =====================================================================


DateTime::DateTime(rawtime_t SecondsSince0000)
{

  /** \internal

    http://en.wikipedia.org/wiki/Talk:Julian_day

  */

  rawtime_t n, c, y, m, a, Year, Month, Day;

  rawtime_t JDN = (rawtime_t)(SecondsSince0000 / 86400) + 1721059;  // added to set refererence to JDN base
  rawtime_t SecsLeft = (rawtime_t)(SecondsSince0000 % 86400);

  //n = JDN + 32082;


  n = JDN + 32044;
  c = (4*n + 3)/146097;
  n = n + (3*c + 3)/4;

  y = (4*n + 3)/1461;
  n = n - (1461*y)/4;
  m = (5*n + 2)/153;
  a = (m + 2)/12;
  Day = n - (153*m + 2)/5 + 1;
  Month = m + 3 - (12*a);
  Year = y + a - 4800;

  m_Day = (int)Day;
  m_Month = (int)Month;
  m_Year = (int)Year;

  m_Hour = (rawtime_t)(SecsLeft / 3600);
  SecsLeft = (rawtime_t)(SecsLeft % 3600);

  m_Min = (rawtime_t)(SecsLeft / 60);
  SecsLeft = (rawtime_t)(SecsLeft % 60);

  m_Sec = SecsLeft;

  m_RawTime = SecondsSince0000;

  m_CalculatedRawTime = false;


}

// =====================================================================
// =====================================================================

DateTime::~DateTime()
{

}


// =====================================================================
// =====================================================================


rawtime_t DateTime::getRawTime()
{

  /** \internal

    Uses the julian day formula from
    Fliegel, H. F. and van Flandern, T. C. (1968). Communications of the ACM, Vol. 11, No. 10 (October, 1968).

    D-32075+1461*(Y+4800+(M-14)/12)/4+367*(M-2-(M-14)/12*12)/12-3*((Y+4900+(M-14)/12)/100)/4

    This formula gives the number of days since the reference day which is the first day of 4713BC.
    We choose the 1-1-0000 as our reference day, so this formula is also used to copute the numbers of
    days since 1-1-0000

    So we applied the following formula:
       (number of days between the date to process and first day of 4713BC)
       minus (number of days between 1-1-0000 and first day of 4713BC)
    using (number of days between 1-1-0000 and first day of 4713BC) is 1721059

  */

  if (!m_CalculatedRawTime)
  {

    rawtime_t JDN;

    JDN = (rawtime_t)((m_Day-32075+1461*(m_Year+4800+(m_Month-14)/12)/4+367*(m_Month-2-(m_Month-14)/12*12)/12-3*((m_Year+4900+(m_Month-14)/12)/100)/4)-1721059);

    m_RawTime = (rawtime_t)((JDN*86400) + (m_Hour*3600) + (m_Min*60) + m_Sec);

    #if 0
    std::cerr << "JDN: " << JDN << "    RawTime: " << m_RawTime << std::endl;
    #endif
    m_CalculatedRawTime = true;
  }

  return m_RawTime;

}


// =====================================================================
// =====================================================================


wxString  DateTime::asString()
{

  return (wxDateTime((unsigned short)(m_Day),(wxDateTime::Month)(m_Month-1),m_Year,
                    (unsigned short)(m_Hour),(unsigned short)(m_Min),(unsigned short)(m_Sec),0)).Format(wxT("%Y-%m-%d %H:%M:%S"));
}


wxString  DateTime::getDateAsString()
{
  return (wxDateTime((unsigned short)(m_Day),(wxDateTime::Month)(m_Month-1),m_Year,
                    (unsigned short)(m_Hour),(unsigned short)(m_Min),(unsigned short)(m_Sec),0)).Format(wxT("%Y-%m-%d"));
}


wxString  DateTime::getTimeAsString()
{
  return (wxDateTime((unsigned short)(m_Day),(wxDateTime::Month)(m_Month-1),m_Year,
                    (unsigned short)(m_Hour),(unsigned short)(m_Min),(unsigned short)(m_Sec),0)).Format(wxT("%H:%M:%S"));

}




} } // namespace mhydasdk::core

