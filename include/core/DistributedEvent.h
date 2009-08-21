/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file DistributedEvent.h
  \brief Header of distributed event definition class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __DISTRIBUTEDEVENT_H__
#define __DISTRIBUTEDEVENT_H__


#include "TypeDefs.h"
#include "DateTime.h"

namespace openfluid { namespace core {



/**
  \brief Class defining  a discrete event, including attached information

  Each event attached information is represented by a Key-Value couple :
  \li Key is the name given to the information
  \li Value is the value of the information and can be numeric or text information
*/
class DistributedEvent
{

  private:
	  DateTime m_Date;
    typedef std::map<std::string, std::string> EventInfosMap_t;

	  EventInfosMap_t m_Infos;

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
    bool isInfoExists(std::string Key);

    /**
      Returns true if the information exists and equals the given string value
      \param[in] Key the requested information key
      \param[in] Value the requested value
    */
    bool isInfoEquals(std::string Key, std::string Value);

    /**
      Returns true if the information exists and equals the given long value
      \param[in] Key the requested information key
      \param[in] Value the requested value
    */
    bool isInfoEquals(std::string Key, long Value);

    /**
      Returns true if the information exists and equals the given double value
      \param[in] Key the requested information key
      \param[in] Value the requested value
    */
    bool isInfoEquals(std::string Key, double Value);

    /**
      Returns true if the information exists and equals the given ScalarValue value
      \param[in] Key the requested information key
      \param[in] Info the requested value
    */
    bool isInfoEquals(std::string Key, ScalarValue *Info);

    /**
      Returns the number of information
    */
    int getInfosCount() { return m_Infos.size(); }

    /**
      Returns all the informations as an EventInfosMap
    */
    EventInfosMap_t getInfos() { return m_Infos; }

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
    bool getInfoAsString(std::string Key, std::string *Info);

    /**
      Gets an information as a long integer
      \param[in] Key the requested information key
      \param[out] Info the value corresponding to the requested key
      \return true if the key exists and the conversion to the requested type is correct
    */
    bool getInfoAsLong(std::string Key, long *Info);

    /**
      Gets an information as a double
      \param[in] Key the requested information key
      \param[out] Info the value corresponding to the requested key
      \return true if the key exists and the conversion to the requested type is correct
    */
    bool getInfoAsDouble(std::string Key, double *Info);

    /**
      Gets an information as a ScalarValue
      \param[in] Key the requested information key
      \param[out] Info the value corresponding to the requested key
      \return true if the key exists and the conversion to the requested type is correct
    */
    bool getInfoAsScalarValue(std::string Key, ScalarValue *Info);

    /**
      Adds an information to the event
      \param[in] Key the key of the added information
      \param[in] Info the value of the added information
      \return true if the adding is correct (key does not already exists)
    */
    bool addInfo(std::string Key, std::string Info);

    /**
      Prints the content of the event to stdout
    */
    void println();

};

} } // namespace openfluid::core

#endif /* __DISTRIBUTEDEVENT_H__ */
