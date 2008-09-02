/**
  \file DistributedEvent.h
  \brief Header of distributed event definition class

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __DISTRIBUTEDEVENT_H__
#define __DISTRIBUTEDEVENT_H__

// TODO    zieuter pour les mois dt.Set(..., wxDateTime::Month(wxDateTime::Jan + month - 1), ...);


#include <wx/wx.h>
#include <wx/hashmap.h>
#include "TypeDefs.h"
#include "DateTime.h"

namespace openfluid { namespace core {

/**
  The map associating a key to a value
*/
WX_DECLARE_STRING_HASH_MAP(wxString, EventInfosMap);


/**
  \brief Class defining  a discrete event, including hosted information

  Each event hosts informations represented as a Key-Value couple :
  \li Key is the name given to the information
  \li Value is the value of the information and can be numeric or text information

*/
class DistributedEvent
{

  private:
	  DateTime m_Date;
    EventInfosMap m_Infos;

  public:

    /**
      Default constructor
    */
    DistributedEvent();

    /**
      Constructor
      \param[in] Date the date and time of the event
    */
    DistributedEvent(DateTime Date);

    /**
      Destructor
    */
    ~DistributedEvent();

    /**
      Returns true if the information exists
      \param[in] Key the requested information key
    */
    bool isInfoExists(wxString Key);

    /**
      Returns true if the information exists and equals the given string value
      \param[in] Key the requested information key
      \param[in] Value the requested value
    */
    bool isInfoEquals(wxString Key, wxString Value);

    /**
      Returns true if the information exists and equals the given long value
      \param[in] Key the requested information key
      \param[in] Value the requested value
    */
    bool isInfoEquals(wxString Key, long Value);

    /**
      Returns true if the information exists and equals the given double value
      \param[in] Key the requested information key
      \param[in] Value the requested value
    */
    bool isInfoEquals(wxString Key, double Value);

    /**
      Returns true if the information exists and equals the given ScalarValue value
      \param[in] Key the requested information key
      \param[in] Info the requested value
    */
    bool isInfoEquals(wxString Key, ScalarValue *Info);

    /**
      Returns the number of information
    */
    int getInfosCount() { return m_Infos.size(); }

    /**
      Returns all the informations as an EventInfosMap
    */
    EventInfosMap getInfos() { return m_Infos; }

    /**
      Returns the date and time of the event
    */
    DateTime getDateTime() { return m_Date; }

    /**
      Gets an information as a string
      \param[in] Key the requested information key
      \param[out] Info the value corresponding to the requested key
      \return true if the key exists and the conversion to the requested type is correct
    */
    bool getInfoAsString(wxString Key, wxString *Info);

    /**
      Gets an information as a long integer
      \param[in] Key the requested information key
      \param[out] Info the value corresponding to the requested key
      \return true if the key exists and the conversion to the requested type is correct
    */
    bool getInfoAsLong(wxString Key, long *Info);

    /**
      Gets an information as a double
      \param[in] Key the requested information key
      \param[out] Info the value corresponding to the requested key
      \return true if the key exists and the conversion to the requested type is correct
    */
    bool getInfoAsDouble(wxString Key, double *Info);

    /**
      Gets an information as a ScalarValue
      \param[in] Key the requested information key
      \param[out] Info the value corresponding to the requested key
      \return true if the key exists and the conversion to the requested type is correct
    */
    bool getInfoAsScalarValue(wxString Key, ScalarValue *Info);

    /**
      Adds an information to the event
      \param[in] Key the key of the added information
      \param[in] Info the value of the added information
      \return true if the adding is correct (key does not already exists)
    */
    bool addInfo(wxString Key, wxString Info);

    /**
      Prints the content of the event to stdout
    */
    void println();

};

} } // namespace openfluid::core

#endif /* __DISTRIBUTEDEVENT_H__ */
