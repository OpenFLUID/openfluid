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
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#ifndef __OPENFLUID_TOOLS_CHRONOLOGICALSERIE_HPP__
#define __OPENFLUID_TOOLS_CHRONOLOGICALSERIE_HPP__


#include <list>

#include <openfluid/core/DateTime.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {


template <typename DataType=double>
using ChronItem_t = std::pair<openfluid::core::DateTime,DataType>;


/**
  Container for a chronological data serie
*/
template <typename DataType=double>
class ChronologicalSerie : public std::list<ChronItem_t<DataType>>
{
  private:

    typename std::list<ChronItem_t<DataType>>::iterator m_InternalIterator;
    typename std::list<ChronItem_t<DataType>>::iterator m_PreviousInternalIterator;


  public:

    /**
      Default constructor
    */
    ChronologicalSerie() : std::list<ChronItem_t<DataType>>(), m_InternalIterator(this->begin()), 
      m_PreviousInternalIterator(this->begin())
    {
    }
    /**
      Resets the internal iterator
    */
    void reset()
    {
      if (this->size() >= 2)
      {
        m_PreviousInternalIterator = (this->begin());
        m_InternalIterator = (++this->begin());
      }
    }

    /**
      Finds the two surrending values for a given date. If the given date is exactly found in the serie,
      the two surrounding values are the same.
      @param[in] DT The date to find surrounding values
      @param[out] Before The closest date before the given date
      @param[out] After The closest date after the given date
      @return true if the closest dates before and after have been found
    */
    bool getSurroundingValues(const openfluid::core::DateTime& DT, 
                              ChronItem_t<DataType>& Before, 
                              ChronItem_t<DataType>& After)
    {
      if (this->size() <2)
      {
        return false;
      }

      if ((*m_PreviousInternalIterator).first > DT || m_InternalIterator == this->end())
      {
        reset();
      }


      while (m_InternalIterator != this->end() &&
            !((*m_InternalIterator).first >= DT &&
              (*m_PreviousInternalIterator).first <= DT))
      {

        // found exact time
        if ((*m_InternalIterator).first == DT)
        {
          Before = (*m_InternalIterator);
          After = (*m_InternalIterator);
          return true;
        }
        else if ((*m_PreviousInternalIterator).first == DT)
        {
          Before = (*m_PreviousInternalIterator);
          After = (*m_PreviousInternalIterator);
          return true;
        }

        ++m_PreviousInternalIterator;
        ++m_InternalIterator;
      }


      if (m_InternalIterator != this->end())
      {
        if ((*m_PreviousInternalIterator).first == DT)
        {
          Before = (*m_InternalIterator);
          After = (*m_InternalIterator);
          return true;
        }
        else if ((*m_InternalIterator).first == DT)
        {
          Before = (*m_InternalIterator);
          After = (*m_InternalIterator);
          return true;
        }
        else
        {
          Before = (*(m_PreviousInternalIterator));
          After = (*m_InternalIterator);
          return true;
        }
      }


      return false;
    }

};


} } // namespaces


#endif /* __OPENFLUID_TOOLS_CHRONOLOGICALSERIE_HPP__ */
