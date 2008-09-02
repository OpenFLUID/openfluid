/**
  \file DateTime.h
  \brief header of date-time management class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __DATETIME_H__
#define __DATETIME_H__


#include <sys/types.h>
#include <wx/string.h>
#include <time.h>
#include <wx/datetime.h>

namespace openfluid { namespace core {

/**
  Type for raw time (seconds since 4713BC)
*/
typedef long long rawtime_t;

/**
  \brief Class for management of date and time information.

  Class for management of date and time information. It includes arithmetics and comparisons operations.

  Sources:
  \li Fliegel, H. F. and van Flandern, T. C. (1968). Communications of the ACM, Vol. 11, No. 10 (October, 1968). http://www.decimaltime.hynes.net/index.html
  \li http://en.wikipedia.org/wiki/Julian_day
  \li wxDateTime documentation http://docs.wxwidgets.org/
*/
class DateTime
{
  private:

    /**
      \if DOCFORDEV
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
        \endif
    */

    /**
      The date and time stored as broken down structure
    */
    struct tm m_TM;

    /**
      The date and time stored as number of seconds since 4713BC
    */
    rawtime_t m_RawTime;

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
    DateTime(rawtime_t SecondsSince0000);

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

    void set(rawtime_t SecondsSince0000);


    /**
      Sets the date and time from an ISO formatted string (YYYY-MM-DD hh:mm:ss)
    */
    bool setFromISOString(wxString DateTimeStr);

    /**
      Sets the date and time from a formatted string (using strptime formatting)
    */
    bool setFromString(wxString DateTimeStr, wxString Format);


    /**
      Sets the date and time from a wxDateTime object
    */
    bool setFromwxDateTime(wxDateTime DT);


    /**
      Returns Year (4 digits)
      \return an int
    */
    int getYear() const {return (m_TM.tm_year+1900); };

    /**
      Retourns Month [1-12]
      \return an int
    */
    int getMonth() const {return (m_TM.tm_mon+1) ;};

    /**
      Returns Day [1-31]
      \return an int
    */
    int getDay() const {return m_TM.tm_mday;};

    /**
      Returns Hour  [0-23]
      \return an int
    */
    int getHour() const {return m_TM.tm_hour;};

    /**
      Returns Minute [0-59]
      \return an int
    */
    int getMinute() const {return m_TM.tm_min;};

    /**
      Returns Second [0-59]
      \return an int
    */
    int getSecond() const {return m_TM.tm_sec;};

    /**
      Returns date-time in raw format (number of seconds since first day of 4713 BC)
      \return a rawtime_t
    */
    rawtime_t getRawTime();


    /**
      Returns date-time as string, using format YYYT-MM-DD hh:mm:ss
      \return a string
    */
    wxString getAsISOString();

    /**
      Returns date-time as string, using strftime() format string
      \param[in] strftime()-like format string
      \return a string
    */
    wxString getAsString(wxString Format);


    /**
      Returns date as string, using format YYYY-MM-DD
      \return a string
    */
    wxString getDateAsISOString();

    /**
      Returns time as string, using format hh:mm:ss
      \return a string
    */
    wxString getTimeAsISOString();

    /**
      Returns date-time as a wxdateTime object
    */
    wxDateTime getAswxDateTime();


    /**
      Adds the given seconds to the current date and time
    */
    void addSeconds(rawtime_t Seconds);

    /**
      Subtracts the given seconds from the current date and time
    */
    void subtractSeconds(rawtime_t Seconds);

    /**
      Returns the difference in seconds between the date-time and the given date-time (Self - Given)
    */
    rawtime_t diffInSeconds(DateTime DT);

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
    bool operator ==(const DateTime &Right);

    /**
      Difference operator
    */
    bool operator !=(const DateTime &Right);

    /**
      Greater than operator
    */
    bool operator >(const DateTime &Right);

    /**
      Greater than or equal operator
    */
    bool operator >=(const DateTime &Right);

    /**
      Lower than operator
    */
    bool operator <(const DateTime &Right);

    /**
      Lower than or equal operator
    */
    bool operator <=(const DateTime &Right);

    /**
      Add operator
    */
    DateTime operator +(const rawtime_t Seconds) const;

    /**
      Subtract operator
    */
    DateTime operator -(const rawtime_t Seconds) const;

    /**
      Returns the number of seconds for one minute
    */
    static inline rawtime_t Minute() { return 60; };

    /**
      Returns the number of seconds for N minutes
    */
    static inline rawtime_t Minutes(int N) { return (60*N); };

    /**
      Returns the number of seconds for one hour
    */
    static inline rawtime_t Hour() { return 3600; };

    /**
      Returns the number of seconds for N hours
    */
    static inline rawtime_t Hours(int N) { return (3600*N); };

    /**
      Returns the number of seconds for one day
    */
    static inline rawtime_t Day() { return 86400; };

    /**
      Returns the number of seconds for N days
    */
    static inline rawtime_t Days(int N) { return (86400*N); };

    /**
      Returns the number of seconds for one week
    */
    static inline rawtime_t Week() { return 604800; };

    /**
      Returns the number of seconds for N weeks
    */
    static inline rawtime_t Weeks(int N) { return (604800*N); };

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
    bool isSameDate(DateTime DT);

    /**
      Returns true if the given time is the same, without comparing dates
    */
    bool isSameTime(DateTime DT);

};



} } // namespace mhydasdk::core

#endif











