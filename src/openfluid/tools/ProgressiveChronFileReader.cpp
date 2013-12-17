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
  \file ProgressiveChronFileReader.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include <openfluid/tools/ProgressiveChronFileReader.hpp>
#include <openfluid/tools/SwissTools.hpp>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace tools {


ProgressiveChronFileReader::ProgressiveChronFileReader(const std::string& FileName, const std::string& DateFormat, const std::string& ColSeparators):
  ProgressiveColumnFileReader(FileName,ColSeparators), m_DateFormat(DateFormat)
{

}


// =====================================================================
// =====================================================================


bool ProgressiveChronFileReader::getNextValue(ChronItem_t& Value)
{
  std::vector<std::string> Values;
  openfluid::core::DateTime DT;
  double Val;

  while (getNextLine(Values))
  {
    if (Values.size() == 2)
    {
      if (DT.setFromString(Values.front(),m_DateFormat) &&
          openfluid::tools::ConvertString(Values.back(),&Val))
      {
        Value.first = DT;
        Value.second = Val;
        return true;
      }
      else
        throw openfluid::base::FrameworkException("ProgressiveChronFileReader::getNextValue","wrong data in " + m_FileName);
    }
  }

  return false;
}


} } // namespaces
