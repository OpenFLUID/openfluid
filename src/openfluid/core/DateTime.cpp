/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  @file
  @brief Implements date-time management class

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include <openfluid/core/DateTime.hpp>
#include <iostream>
#include <cstring>
#include <cstdio>



namespace openfluid { namespace core {


DateTime::DateTime()
{
  DateTime(1900, 1, 1, 0, 0, 0);
}

// =====================================================================
// =====================================================================


DateTime::DateTime(int Year, int Month, int Day, int Hour, int Minute, int Second)
{
  set(Year, Month, Day, Hour, Minute, Second);
}


// =====================================================================
// =====================================================================


DateTime::DateTime(rawtime_t SecondsSince0000)
{
  set(SecondsSince0000);
}

// =====================================================================
// =====================================================================

DateTime::~DateTime()
{

}

// =====================================================================
// =====================================================================


bool DateTime::set(int Year, int Month, int Day, int Hour, int Minute, int Second)
{
  if (isValidDateTime(Year,Month,Day,Hour,Minute,Second))
  {
    m_TM.tm_year = Year-1900;
    m_TM.tm_mon = Month-1;
    m_TM.tm_mday = Day;
    m_TM.tm_hour = Hour;
    m_TM.tm_min = Minute;
    m_TM.tm_sec = Second;
    m_TM.tm_wday = -1;
    m_TM.tm_yday = -1;
    m_TM.tm_isdst = 0;

    updateRawTimeFromYMDHMS();

    return true;
  }
  else return false;
}

// =====================================================================
// =====================================================================


bool DateTime::setFromISOString(const std::string& DateTimeStr)
{
  int Year;
  int Month;
  int Day;
  int Hour;
  int Min;
  int Sec;


  // scan of the input string to break it down
  sscanf(DateTimeStr.c_str(),"%4d-%2d-%2d %2d:%2d:%2d",&Year,&Month,&Day,&Hour,&Min,&Sec);

  return set(Year, Month, Day, Hour, Min, Sec);

}

// =====================================================================
// =====================================================================


void DateTime::set(const rawtime_t& SecondsSince0000)
{
  m_RawTime = SecondsSince0000;

  updateYMDHMSFromRawTime();


}

// =====================================================================
// =====================================================================



void DateTime::updateYMDHMSFromRawTime()
{


  /** @internal

    http://en.wikipedia.org/wiki/Talk:Julian_day

  */


  rawtime_t n, c, y, m, a, Year, Month, Day;

  rawtime_t JDN = (rawtime_t)(m_RawTime / 86400) + 1721059;  // added to set refererence to JDN base
  rawtime_t SecsLeft = (rawtime_t)(m_RawTime % 86400);

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

  m_TM.tm_mday = (int)Day;
  m_TM.tm_mon = (int)Month - 1;
  m_TM.tm_year = (int)Year - 1900;

  m_TM.tm_hour = (rawtime_t)(SecsLeft / 3600);
  SecsLeft = (rawtime_t)(SecsLeft % 3600);

  m_TM.tm_min = (rawtime_t)(SecsLeft / 60);
  SecsLeft = (rawtime_t)(SecsLeft % 60);

  m_TM.tm_sec = SecsLeft;



}

// =====================================================================
// =====================================================================


void DateTime::updateRawTimeFromYMDHMS()
{
  /** @internal

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


  rawtime_t JDN;

  int Day,Year,Month;

  Day = m_TM.tm_mday;
  Year = m_TM.tm_year+1900;
  Month = m_TM.tm_mon+1;

  JDN = (rawtime_t)((Day-32075+1461*(Year+4800+(Month-14)/12)/4+367*(Month-2-(Month-14)/12*12)/12-3*((Year+4900+(Month-14)/12)/100)/4)-1721059);

  m_RawTime = (rawtime_t)((JDN*86400) + (m_TM.tm_hour*3600) + (m_TM.tm_min*60) + m_TM.tm_sec);

  #if 0
  std::cerr << "JDN: " << JDN << "    RawTime: " << m_RawTime << std::endl;
  #endif



}

// =====================================================================
// =====================================================================


rawtime_t DateTime::getRawTime() const
{


  return m_RawTime;

}


// =====================================================================
// =====================================================================


std::string DateTime::getAsISOString() const
{

  char pCh[80];
  std::string Str;

  strftime(pCh,80,"%Y-%m-%d %H:%M:%S",&m_TM);

  Str = std::string(pCh,strlen(pCh));

  return Str;

}


// =====================================================================
// =====================================================================

std::string  DateTime::getAsString(std::string Format) const
{

  char pCh[80];
  std::string Str;


  strftime(pCh,80,Format.c_str(),&m_TM);

  Str = std::string(pCh,strlen(pCh));

  return Str;


}


// =====================================================================
// =====================================================================


std::string DateTime::getDateAsISOString() const
{

  char pCh[80];
  std::string Str;


  strftime(pCh,80,"%Y-%m-%d",&m_TM);

  Str = std::string(pCh,strlen(pCh));

  return Str;


}



// =====================================================================
// =====================================================================


std::string DateTime::getTimeAsISOString() const
{

  char pCh[80];
  std::string Str;

  strftime(pCh,80,"%H:%M:%S",&m_TM);

  Str = std::string(pCh,strlen(pCh));

  return Str;

}

// =====================================================================
// =====================================================================


void DateTime::addSeconds(const rawtime_t& Seconds)
{
  m_RawTime = m_RawTime + Seconds;
  updateYMDHMSFromRawTime();
}

// =====================================================================
// =====================================================================


void DateTime::subtractSeconds(const rawtime_t& Seconds)
{
  m_RawTime = m_RawTime - Seconds;
  updateYMDHMSFromRawTime();
}

// =====================================================================
// =====================================================================


bool DateTime::isBetween(const DateTime& FirstDT, const DateTime& SecondDT)
{
  return ((*this >= FirstDT) && (*this <= SecondDT));
}

// =====================================================================
// =====================================================================


bool DateTime::isStrictlyBetween(const DateTime& FirstDT, const DateTime& SecondDT)
{
  return ((*this > FirstDT) && (*this < SecondDT));
}


// =====================================================================
// =====================================================================


rawtime_t DateTime::diffInSeconds(const DateTime& DT) const
{
  return (m_RawTime - DT.getRawTime());
}

// =====================================================================
// =====================================================================


DateTime& DateTime::operator =(const DateTime &Right)
{
  if (&Right != this) set(Right.m_RawTime);

  return *this;
}


// =====================================================================
// =====================================================================

bool DateTime::operator ==(const DateTime &Right) const
{
  return (m_RawTime == Right.m_RawTime);
}

// =====================================================================
// =====================================================================

bool DateTime::operator !=(const DateTime &Right) const
{
  return (m_RawTime != Right.m_RawTime);
}

// =====================================================================
// =====================================================================

bool DateTime::operator >(const DateTime &Right) const
{
  return (m_RawTime > Right.m_RawTime);
}

// =====================================================================
// =====================================================================

bool DateTime::operator >=(const DateTime &Right) const
{
  return (m_RawTime >= Right.m_RawTime);
}

// =====================================================================
// =====================================================================

bool DateTime::operator <(const DateTime &Right) const
{
  return (m_RawTime < Right.m_RawTime);
}

// =====================================================================
// =====================================================================

bool DateTime::operator <=(const DateTime &Right) const
{
  return (m_RawTime <= Right.m_RawTime);
}


// =====================================================================
// =====================================================================


DateTime DateTime::operator +(const rawtime_t& Seconds) const
{
  DateTime DT(m_RawTime+Seconds);

  return DT;
}

// =====================================================================
// =====================================================================


DateTime DateTime::operator -(const rawtime_t& Seconds) const
{
  DateTime DT(m_RawTime-Seconds);

  return DT;
}

// =====================================================================
// =====================================================================

/* static */
int DateTime::getNumOfDaysInMonth(int Year, int Month)
{
  // the number of days in month in Julian/Gregorian calendar: the first line
  // is for normal years, the second one is for the leap ones
  static int DaysInMonth[2][12] =
  {
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
  };

  return DaysInMonth[DateTime::isLeapYear(Year)][Month-1];
}

// =====================================================================
// =====================================================================

/* static */
bool DateTime::isLeapYear(int Year)
{
  return (Year % 4 == 0) && ((Year % 100 != 0) || (Year % 400 == 0));
}

// =====================================================================
// =====================================================================


/* static */
bool DateTime::isValidDateTime(int Year, int Month, int Day, int Hour, int Minute, int Second)
{
  return ((Month >= 1) && (Month <= 12)  && (Day >= 1) && (Day <= getNumOfDaysInMonth(Year,Month)) &&
          (Hour >=0) && (Hour <= 23) && (Minute >=0) && (Minute <= 59) && (Second >=0) && (Second <= 59));
}

// =====================================================================
// =====================================================================

bool DateTime::isSameDate(const DateTime& DT) const
{
  return (((m_TM.tm_year+1900) ==  DT.getYear()) &&
          ((m_TM.tm_mon+1) == DT.getMonth()) &&
          ((m_TM.tm_mday) == DT.getDay()));
}

// =====================================================================
// =====================================================================

bool DateTime::isSameTime(const DateTime& DT) const
{
  return (((m_TM.tm_hour) ==  DT.getHour()) &&
          ((m_TM.tm_min) == DT.getMinute()) &&
          ((m_TM.tm_sec) == DT.getSecond()));
}




} } // namespace mhydasdk::core




