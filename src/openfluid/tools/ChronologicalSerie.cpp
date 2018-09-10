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
  @file ChronologicalSerie.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/tools/ChronologicalSerie.hpp>


namespace openfluid { namespace tools {


void ChronologicalSerie::reset()
{
  if (size() >= 2)
  {
    m_PreviousInternalIterator = (begin());
    m_InternalIterator = (++begin());
  }
}


// =====================================================================
// =====================================================================


bool ChronologicalSerie::getSurroundingValues(const openfluid::core::DateTime& DT,
                                              ChronItem_t& Before, ChronItem_t& After)
{

  if (size() <2)
    return false;

  //std::list<ChronItem_t>::iterator CurrentPos = m_InternalIterator;

  if ((*m_PreviousInternalIterator).first > DT || m_InternalIterator == end())
    reset();


  while (m_InternalIterator != end() &&
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


  if (m_InternalIterator != end())
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


} } // namespaces

