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
  @file

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __DATETIME_HPP__
#define __DATETIME_HPP__


#include <sys/types.h>
#include <time.h>
#include <string>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {

/**
  Type for raw time (seconds since 4713BC)
*/
typedef unsigned long long RawTime_t;

/**
  Type for time indexes (in seconds)
*/
typedef unsigned long long TimeIndex_t;


/**
  Type for durations (in seconds)
*/
typedef unsigned long long Duration_t;


/**
  @brief Class for management of date and time information.

  Class for management of date and time information. It includes arithmetics and comparisons operations.

  Sources:
  @li Fliegel, H. F. and van Flandern, T. C. (1968). Communications of the ACM, Vol. 11, No. 10 (October, 1968). http://www.decimaltime.hynes.net/index.html
  @li http://en.wikipedia.org/wiki/Julian_day

  <BR>

  <I>Example : creating a date</I>
  @code
  openfluid::core::DateTime aDate(2009,01,25,12,05,00);
  @endcode


  <I>Example : adding time span to a date</I>
  @code
  // adding 45 seconds
  aDate.addSeconds(45);

  // adding 5 minutes
  aDate.addSeconds(openfluid::core::DateTime::Minutes(5));

  // adding an hour
  aDate.addSeconds(openfluid::core::DateTime::Hour());

  // adding 60 days
  aDate.addSeconds(openfluid::core::DateTime::Days(60));
  @endcode


  <I>Example : subtracting time span to a date</I>
  @code
  // subtracting 10 seconds
  aDate.subtractSeconds(10);

  // subtracting 30 days
  aDate.subtractSeconds(openfluid::core::DateTime::Days(30));
  @endcode


  <I>Example : getting difference in seconds between two dates</I>
  @code
  openfluid::core::DateTime FirstDate(2009,01,25,12,05,00);
  openfluid::core::DateTime SecondDate(2009,02,28,00,00,00);

  openfluid::core::RawTime_t Diff;

  Diff = SecondDate - FirstDate;

  if (Diff > openfluid::core::DateTime::Days(60))
    std::cout << "The difference between the two dates is greater than 60 days" << std::end;
  else
    std::cout << "The difference between the two dates is lesser than 60 days" << std::end;
  @endcode


*/
class DLLEXPORT DateTime
{
  private:

    /**
          The tm struct contains:
        int tm_sec;
        int tm_min;
        int tm_hour;
        int tm_mday;
        int tm_mon;
        int tm_year;
        int tm_wday;
        int tm_yday;
        int tm_isdst
          */

    /**
      The date and time stored as broken down structure
    */
    struct tm m_TM;

    /**
      The date and time stored as number of seconds since 4713BC
    */
    RawTime_t m_RawTime;

    void updateYMDHMSFromRawTime();

    void updateRawTimeFromYMDHMS();


  public:

    /**
      Default constructor
    */
    DateTime();

    /**
      Constructor
    */
    DateTime(int Year, int Month, int Day, int Hour, int Minute, int Second);

    /**
      Constructor
    */
    DateTime(RawTime_t SecondsSince0000);

    /**
      Destructor
    */
    ~DateTime();

    /**
      Sets the date and time from the parameters
    */
    bool set(int Year, int Month, int Day, int Hour, int Minute, int Second);

    /**
      Sets the date and time from the number of seconds since first day of 4713BC
    */

    void set(const RawTime_t& SecondsSince0000);


    /**
      Sets the date and time from an ISO formatted string (YYYY-MM-DD hh:mm:ss)
    */
    bool setFromISOString(const std::string& DateTimeStr);


    /**
      Sets the date and time from a string using the given format
    */
    bool setFromString(const std::string& DateTimeStr, const std::string& FormatStr);


    /**
      Returns Year (4 digits)
      @return an int
    */
    int getYear() const {return (m_TM.tm_year+1900); };

    /**
      Retourns Month [1-12]
      @return an int
    */
    int getMonth() const {return (m_TM.tm_mon+1) ;};

    /**
      Returns Day [1-31]
      @return an int
    */
    int getDay() const {return m_TM.tm_mday;};

    /**
      Returns Hour  [0-23]
      @return an int
    */
    int getHour() const {return m_TM.tm_hour;};

    /**
      Returns Minute [0-59]
      @return an int
    */
    int getMinute() const {return m_TM.tm_min;};

    /**
      Returns Second [0-59]
      @return an int
    */
    int getSecond() const {return m_TM.tm_sec;};

    /**
      Returns date-time in raw format (number of seconds since first day of 4713 BC)
      @return a rawtime_t
    */
    RawTime_t getRawTime() const;


    /**
      Returns date-time as string, using format YYYT-MM-DD hh:mm:ss
      @return a string
    */
    std::string getAsISOString() const;

    /**
      Returns date-time as string, using strftime() format string
      @param[in] Format strftime()-like format string
      @return a string
    */
    std::string getAsString(std::string Format) const;


    /**
      Returns date as string, using format YYYY-MM-DD
      @return a string
    */
    std::string getDateAsISOString() const;

    /**
      Returns time as string, using format hh:mm:ss
      @return a string
    */
    std::string getTimeAsISOString() const;


    /**
      Adds the given seconds to the current date and time
    */
    void addSeconds(const RawTime_t& Seconds);

    /**
      Subtracts the given seconds from the current date and time
    */
    void subtractSeconds(const RawTime_t& Seconds);

    /**
      Returns the difference in seconds between the date-time and the given date-time (Self - Given)
    */
    RawTime_t diffInSeconds(const DateTime& DT) const;

    /**
      Returns true if the date-time is between the two given date-time
    */
    bool isBetween(const DateTime& FirstDT, const DateTime& SecondDT);

    /**
      Returns true if the date-time is strictly between the two given date-time
    */
    bool isStrictlyBetween(const DateTime& FirstDT, const DateTime& SecondDT);


    /**
      Assignment operator
    */
    DateTime& operator =(const DateTime &Right);

    /**
      Equal operator
    */
    bool operator ==(const DateTime &Right) const;

    /**
      Difference operator
    */
    bool operator !=(const DateTime &Right) const;

    /**
      Greater than operator
    */
    bool operator >(const DateTime &Right) const;

    /**
      Greater than or equal operator
    */
    bool operator >=(const DateTime &Right) const;

    /**
      Lower than operator
    */
    bool operator <(const DateTime &Right) const;

    /**
      Lower than or equal operator
    */
    bool operator <=(const DateTime &Right) const;

    /**
      Add operator
    */
    DateTime operator +(const RawTime_t& Seconds) const;

    /**
      Subtract operator
    */
    DateTime operator -(const RawTime_t& Seconds) const;

    /**
      Returns the number of seconds for one minute
    */
    static inline RawTime_t Minute() { return 60; };

    /**
      Returns the number of seconds for N minutes
    */
    static inline RawTime_t Minutes(int N) { return (60*N); };

    /**
      Returns the number of seconds for one hour
    */
    static inline RawTime_t Hour() { return 3600; };

    /**
      Returns the number of seconds for N hours
    */
    static inline RawTime_t Hours(int N) { return (3600*N); };

    /**
      Returns the number of seconds for one day
    */
    static inline RawTime_t Day() { return 86400; };

    /**
      Returns the number of seconds for N days
    */
    static inline RawTime_t Days(int N) { return (86400*N); };

    /**
      Returns the number of seconds for one week
    */
    static inline RawTime_t Week() { return 604800; };

    /**
      Returns the number of seconds for N weeks
    */
    static inline RawTime_t Weeks(int N) { return (604800*N); };

    /**
      Returns true if the given year is a leap year
    */
    static bool isLeapYear(int Year);

    /**
      Returns the number of days in the given month for the given year
    */
    static int getNumOfDaysInMonth(int Year, int Month);

    /**
      Returns true if the given date time is valid
    */
    static bool isValidDateTime(int Year, int Month, int Day, int Hour, int Minute, int Second);

    /**
      Returns true if the given date is the same, without comparing time parts
    */
    bool isSameDate(const DateTime& DT) const;

    /**
      Returns true if the given time is the same, without comparing dates
    */
    bool isSameTime(const DateTime& DT) const;

};



} } // namespaces

#endif











