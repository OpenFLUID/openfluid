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
  @file ChronologicalSerie.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_TOOLS_CHRONOLOGICALSERIE_HPP__
#define __OPENFLUID_TOOLS_CHRONOLOGICALSERIE_HPP__


#include <list>

#include <openfluid/core/DateTime.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {


typedef std::pair<openfluid::core::DateTime,double> ChronItem_t;


/**
  Container for a chronological data serie
*/
class OPENFLUID_API ChronologicalSerie : public std::list<ChronItem_t>
{
  private:

    std::list<ChronItem_t>::iterator m_InternalIterator;
    std::list<ChronItem_t>::iterator m_PreviousInternalIterator;


  public:

    /**
      Default constructor
    */
    ChronologicalSerie() : std::list<ChronItem_t>(), m_InternalIterator(begin()), m_PreviousInternalIterator(begin())
    { }

    /**
      Resets the internal iterator
    */
    void reset();

    /**
      Finds the two surrending values for a given date. If the given date is exactly found in the serie,
      the two surrounding values are the same.
      @param[in] DT The date to find surrounding values
      @param[out] Before The closest date before the given date
      @param[out] After The closest date after the given date
      @return true if the closest dates before and after have been found
    */
    bool getSurroundingValues(const openfluid::core::DateTime& DT, ChronItem_t& Before, ChronItem_t& After);

};


} } // namespaces


#endif /* __OPENFLUID_TOOLS_CHRONOLOGICALSERIE_HPP__ */
