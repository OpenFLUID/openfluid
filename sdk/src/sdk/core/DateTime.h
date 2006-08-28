/**
  \file DateTime.h
  \brief header of date-time managment class

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __DATETIME_H__
#define __DATETIME_H__


#include <sys/types.h>
#include <wx/string.h>


namespace mhydasdk { namespace core {


typedef unsigned long long rawtime_t;

/**
  Thanks To Fliegel, H. F. and van Flandern, T. C. (1968). Communications of the ACM, Vol. 11, No. 10 (October, 1968).
  and http://www.decimaltime.hynes.net/index.html

*/
class DateTime
{
  private:
    int m_Year;
    int m_Month;
    int m_Day;
    int m_Hour;
    int m_Min;
    int m_Sec;

    rawtime_t m_RawTime;
    bool m_CalculatedRawTime;

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
      Returns Year (4 digits)
      \return an int
    */
    int getYear() const {return m_Year;};

    /**
      Retourns Month [1-12]
      \return an int
    */
    int getMonth() const {return m_Month;};

    /**
      Returns Day [1-31]
      \return an int
    */
    int getDay() const {return m_Day;};

    /**
      Returns Hour  [0-23]
      \return an int
    */
    int getHour() const {return m_Hour;};

    /**
      Returns Minute [0-59]
      \return an int
    */
    int getMinute() const {return m_Min;};

    /**
      Returns Second [0-59]
      \return an int
    */
    int getSecond() const {return m_Sec;};

    /**
      Returns date-time in raw format (number of seconds since first day of 4713 BC)
      \return a rawtime_t
    */
    rawtime_t getRawTime();

    /**
      Returns date-time as string, using format YYYT-MM-DD hh:mm:ss
      \return a string
    */
    wxString asString();

    /**
      Returns date as string, using format YYYT-MM-DD
      \return a string
    */
    wxString getDateAsString();

    /**
      Returns time as string, using format hh:mm:ss
      \return a string
    */
    wxString getTimeAsString();

};



} } // namespace mhydasdk::core

#endif











