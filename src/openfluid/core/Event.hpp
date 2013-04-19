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

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __EVENT_HPP__
#define __EVENT_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/deprecation.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/core/DateTime.hpp>


namespace openfluid { namespace core {



/**
  @brief Class defining  a discrete event, including attached information

  Each event attached information is represented by a Key-Value couple :
  @li Key is the name given to the information
  @li Value is the value of the information and can be numeric or text information
*/
class DLLEXPORT Event
{
  public:
    typedef std::map<std::string, openfluid::core::StringValue> EventInfosMap_t;

  private:
	  DateTime m_Date;

	  EventInfosMap_t m_Infos;

  public:

    /**
      Default constructor
    */
    Event();

    /**
      Constructor
      @param[in] Date the date and time of the event
    */
    Event(DateTime Date);

    /**
      Destructor
    */
    ~Event();

    /**
      Returns true if the information exists
      @param[in] Key the requested information key
    */
    bool isInfoExist(const std::string Key) const;

    /**
      Returns true if the information exists and equals the given string value
      @param[in] Key the requested information key
      @param[in] Value the requested value
    */
    bool isInfoEqual(const std::string Key, const std::string Value) const;

    /**
      Returns true if the information exists and equals the given long value
      @param[in] Key the requested information key
      @param[in] Value the requested value
    */
    bool isInfoEqual(const std::string Key, long Value) const;

    /**
      Returns true if the information exists and equals the given double value
      @param[in] Key the requested information key
      @param[in] Value the requested value
    */
    bool isInfoEqual(const std::string Key, const double Value) const;

    /**
      Returns true if the information exists and equals the given double value
      @param[in] Key the requested information key
      @param[in] Info the requested value
      @deprecated
    */
    bool isInfoEqual(const std::string Key, const double* Info) const OPENFLUID_DEPRECATED;

    /**
      Returns true if the information exists and equals the given DoubleValue value
      @param[in] Key the requested information key
      @param[in] Info the requested value
    */
    bool isInfoEqual(const std::string Key, const DoubleValue& Info) const;

    /**
      Returns the number of information
    */
    inline int getInfosCount() const { return m_Infos.size(); }

    /**
      Returns all the informations as an EventInfosMap
    */
    inline EventInfosMap_t getInfos() const { return m_Infos; }

    /**
      Returns the date and time of the event
    */
    inline DateTime getDateTime() const { return m_Date; }

    /**
      Gets an information as a string
      @param[in] Key the requested information key
      @param[out] Info the value corresponding to the requested key
      @return true if the key exists and the conversion to the requested type is correct
      @deprecated
    */
    bool getInfoAsString(const std::string Key, std::string *Info) const OPENFLUID_DEPRECATED;

    /**
      Gets an information as a string
      @param[in] Key the requested information key
      @param[out] Info the value corresponding to the requested key
      @return true if the key exists and the conversion to the requested type is correct
    */
    bool getInfoAsString(const std::string Key, std::string& Info) const;

    /**
      Gets an information as a long integer
      @param[in] Key the requested information key
      @param[out] Info the value corresponding to the requested key
      @return true if the key exists and the conversion to the requested type is correct
      @deprecated
    */
    bool getInfoAsLong(const std::string Key, long *Info) const OPENFLUID_DEPRECATED;

    /**
      Gets an information as a long integer
      @param[in] Key the requested information key
      @param[out] Info the value corresponding to the requested key
      @return true if the key exists and the conversion to the requested type is correct
    */
    bool getInfoAsLong(const std::string Key, long& Info) const;

    /**
      Gets an information as a double
      @param[in] Key the requested information key
      @param[out] Info the value corresponding to the requested key
      @return true if the key exists and the conversion to the requested type is correct
      @deprecated
    */
    bool getInfoAsDouble(const std::string Key, double *Info) const OPENFLUID_DEPRECATED;

    /**
      Gets an information as a double
      @param[in] Key the requested information key
      @param[out] Info the value corresponding to the requested key
      @return true if the key exists and the conversion to the requested type is correct
    */
    bool getInfoAsDouble(const std::string Key, double& Info) const;

    /**
      Gets an information as a DoubleValue
      @param[in] Key the requested information key
      @param[out] Info the value corresponding to the requested key
      @return true if the key exists and the conversion to the requested type is correct
    */
    bool getInfoAsDoubleValue(const std::string Key, DoubleValue& Info) const;

    /**
      Adds an information to the event
      @param[in] Key the key of the added information
      @param[in] Info the value of the added information
      @return true if the adding is correct (key does not already exists)
    */
    bool addInfo(const std::string Key, const std::string Info);

    /**
      Prints the content of the event to stdout
    */
    void println() const;

};

} } // namespace openfluid::core

#endif /* __EVENT_HPP__ */
