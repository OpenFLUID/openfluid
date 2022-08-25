/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file DateTime.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <openfluid/core/DateTime.hpp>


namespace openfluid { namespace core {


DateTime::DateTime()
{
  set(1900, 1, 1, 0, 0, 0);
}


// =====================================================================
// =====================================================================


DateTime::DateTime(int Year, int Month, int Day, int Hour, int Minute, int Second)
{
  set(Year, Month, Day, Hour, Minute, Second);
}


// =====================================================================
// =====================================================================


DateTime::DateTime(RawTime_t SecondsSince0000)
{
  set(SecondsSince0000);
}


// =====================================================================
// =====================================================================


DateTime DateTime::now()
{
  const std::time_t NowT = std::time(nullptr);
  const std::tm* NowTM = std::localtime(&NowT);

  openfluid::core::DateTime DT(NowTM->tm_year+1900,NowTM->tm_mon+1,NowTM->tm_mday,
                               NowTM->tm_hour,NowTM->tm_min,NowTM->tm_sec);

  return DT;
}


// =====================================================================
// =====================================================================


DateTime DateTime::fromString(const std::string& DateTimeStr, const std::string& FormatStr)
{
  openfluid::core::DateTime DT;
  DT.setFromString(DateTimeStr,FormatStr);
  return DT;
}


// =====================================================================
// =====================================================================


DateTime DateTime::fromISOString(const std::string& DateTimeStr)
{
  openfluid::core::DateTime DT;
  DT.setFromISOString(DateTimeStr);
  return DT;
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
  else
  {
    return false;
  }
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
  return (sscanf(DateTimeStr.c_str(),"%4d-%2d-%2d %2d:%2d:%2d",&Year,&Month,&Day,&Hour,&Min,&Sec) == 6 &&
          set(Year, Month, Day, Hour, Min, Sec));
}


// =====================================================================
// =====================================================================


bool DateTime::setFromString(const std::string& DateTimeStr, const std::string& FormatStr)
{
  // TODO Should be replaced by C++11 features when available. Example:
  /*
  std::tm CTime;
  std::istringstream StrS(DateTimeStr);
  StrS.imbue(std::locale(std::locale::classic()));
  StrS >> std::get_time(&CTime,FormatStr.c_str());

  return (set(CTime.tm_year+1900, CTime.tm_mon+1,CTime.tm_mday,
              CTime.tm_hour,CTime.tm_min,CTime.tm_sec));
  */

  boost::posix_time::time_input_facet* Facet = new  boost::posix_time::time_input_facet(FormatStr);

  std::istringstream StrS(DateTimeStr);
  StrS.imbue(std::locale(std::locale::classic(),Facet));
  boost::posix_time::ptime Time(boost::posix_time::not_a_date_time);
  StrS >> Time;

  return (Time != boost::posix_time::not_a_date_time &&
          set(Time.date().year(), Time.date().month(),Time.date().day(),Time.time_of_day().hours(),
              Time.time_of_day().minutes(), Time.time_of_day().seconds()));
}


// =====================================================================
// =====================================================================


void DateTime::set(const RawTime_t& SecondsSince0000)
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

  RawTime_t n, c, y, m, a, Year, Month, Day;

  RawTime_t JDN = (RawTime_t)(m_RawTime / 86400) + 1721059;  // added to set refererence to JDN base
  RawTime_t SecsLeft = (RawTime_t)(m_RawTime % 86400);

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

  m_TM.tm_hour = (RawTime_t)(SecsLeft / 3600);
  SecsLeft = (RawTime_t)(SecsLeft % 3600);

  m_TM.tm_min = (RawTime_t)(SecsLeft / 60);
  SecsLeft = (RawTime_t)(SecsLeft % 60);

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

  RawTime_t JDN;

  int Day,Year,Month;

  Day = m_TM.tm_mday;
  Year = m_TM.tm_year+1900;
  Month = m_TM.tm_mon+1;

  JDN = (RawTime_t)((Day-32075+1461*(Year+4800+(Month-14)/12)/4+367*
                                    (Month-2-(Month-14)/12*12)/12-3*((Year+4900+(Month-14)/12)/100)/4)-1721059);

  m_RawTime = (RawTime_t)((JDN*86400) + (m_TM.tm_hour*3600) + (m_TM.tm_min*60) + m_TM.tm_sec);
}


// =====================================================================
// =====================================================================


RawTime_t DateTime::getRawTime() const
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

  return std::string(pCh,strlen(pCh));
}


// =====================================================================
// =====================================================================


std::string  DateTime::getAsString(const std::string& Format) const
{
  char pCh[80];
  std::string Str;

  strftime(pCh,80,Format.c_str(),&m_TM);

  return std::string(pCh,strlen(pCh));
}


// =====================================================================
// =====================================================================


std::string DateTime::getDateAsISOString() const
{
  char pCh[80];
  std::string Str;

  strftime(pCh,80,"%Y-%m-%d",&m_TM);

  return std::string(pCh,strlen(pCh));
}


// =====================================================================
// =====================================================================


std::string DateTime::getTimeAsISOString() const
{
  char pCh[80];
  std::string Str;

  strftime(pCh,80,"%H:%M:%S",&m_TM);

  return std::string(pCh,strlen(pCh));

}


// =====================================================================
// =====================================================================


void DateTime::addSeconds(const RawTime_t& Seconds)
{
  m_RawTime = m_RawTime + Seconds;
  updateYMDHMSFromRawTime();
}


// =====================================================================
// =====================================================================


void DateTime::subtractSeconds(const RawTime_t& Seconds)
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


RawTime_t DateTime::diffInSeconds(const DateTime& DT) const
{
  return (m_RawTime - DT.getRawTime());
}


// =====================================================================
// =====================================================================


bool DateTime::operator==(const DateTime &Right) const
{
  return (m_RawTime == Right.m_RawTime);
}


// =====================================================================
// =====================================================================


bool DateTime::operator!=(const DateTime &Right) const
{
  return (m_RawTime != Right.m_RawTime);
}


// =====================================================================
// =====================================================================


bool DateTime::operator>(const DateTime &Right) const
{
  return (m_RawTime > Right.m_RawTime);
}


// =====================================================================
// =====================================================================


bool DateTime::operator>=(const DateTime &Right) const
{
  return (m_RawTime >= Right.m_RawTime);
}


// =====================================================================
// =====================================================================


bool DateTime::operator<(const DateTime &Right) const
{
  return (m_RawTime < Right.m_RawTime);
}


// =====================================================================
// =====================================================================


bool DateTime::operator<=(const DateTime &Right) const
{
  return (m_RawTime <= Right.m_RawTime);
}


// =====================================================================
// =====================================================================


DateTime DateTime::operator+(const RawTime_t& Seconds) const
{
  DateTime DT(m_RawTime+Seconds);

  return DT;
}


// =====================================================================
// =====================================================================


DateTime DateTime::operator-(const RawTime_t& Seconds) const
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


} } // namespaces

