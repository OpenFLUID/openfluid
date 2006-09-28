/**
  \file TimeSerieItem.h
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __TIMESERIEITEM_H__
#define __TIMESERIEITEM_H__

#include "time.h"
#include "DateTime.h"
#include <wx/string.h>



namespace mhydasdk { namespace core {


/**
  \brief Classe de gestion d'un enregistrement de chronique temporelle

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class TimeSerieItem
{
  private:

    //tm m_Time;
    //time_t m_RawTime;
    float m_Value;
    //bool m_IsValidRawTime;
    DateTime m_DateTime;

  public:

    /**
      Constructeur par défaut
    */
    TimeSerieItem();


    /**
      Constructeur
    */
    TimeSerieItem(int Year, int Month, int Day, int Hour, int Minute, int Second, float Value);


    /**
      Returns date and time as a DateTime object
      \return a DateTime object
    */
    DateTime getDateTime() const {return m_DateTime;};


    /**
      Returns year (4 digits)
      \return an unsigned char
    */
    int getYear() const;

    /**
      Returns Month [1-12]
      \return an int
    */
    int getMonth() const;

    /**
      Returns Day [1-31]
      \return an int
    */
    int getDay() const;

    /**
      Returns Hour [0-23]
      \return an int
    */
    int getHour() const;

    /**
      Returns Minute [0-59]
      \return an int
    */
    int getMinute() const;

    /**
      Returns Minute [0-59]
      \return an int
    */
    int getSecond() const;

    /**
      Returns number of seconds since 1-1-0000
      \return a rawtime_t
    */
    rawtime_t getRawTime();


    wxString getTimeAsString();


    /**
      Returns value of the record
      \return an unsigned char
    */
    float getValue();

    /**
      Sets value of the record
      \param[in] Value Value of the record
    */
    void setValue(float Value);
};


} } //namespace mhydasdk::core

#endif
