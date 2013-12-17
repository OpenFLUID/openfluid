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
  \file CompoundValue.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/core/CompoundValue.hpp>
#include <boost/assign/list_of.hpp>

namespace openfluid { namespace core {

CompoundValue::SeparatorsByLevels_t CompoundValue::m_StreamSeparators = boost::assign::list_of(";")("|");


// =====================================================================
// =====================================================================


std::string CompoundValue::getStreamSeparator(const unsigned int& Level)
{
  if (Level > m_StreamSeparators.size()) return "";
  return m_StreamSeparators[Level-1];
}


// =====================================================================
// =====================================================================


void CompoundValue::setStreamSeparator(const unsigned int& Level, const std::string& Sep)
{
  if (Level <= m_StreamSeparators.size()) m_StreamSeparators[Level-1] = Sep;
}


} }  // namespaces


