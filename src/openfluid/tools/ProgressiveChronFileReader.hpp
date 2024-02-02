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
  @file ProgressiveChronFileReader.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#ifndef __OPENFLUID_TOOLS_PROGRESSIVECHRONFILEREADER_HPP__
#define __OPENFLUID_TOOLS_PROGRESSIVECHRONFILEREADER_HPP__


#include <openfluid/tools/ProgressiveColumnFileReader.hpp>
#include <openfluid/tools/ChronologicalSerie.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {


template<class DataType>
class ProgressiveChronFileReader : public ProgressiveColumnFileReader
{
  private:

    std::string m_DateFormat;


  public:

    ProgressiveChronFileReader(const std::string& FileName,
                               const std::string& DateFormat = "%Y-%m-%dT%H:%M:%S",
                               const std::string& ColSeparators = " \t\r\n") :
        ProgressiveColumnFileReader(FileName,ColSeparators), m_DateFormat(DateFormat)
    {
    }

    virtual ~ProgressiveChronFileReader()
    {
    }


    bool getNextValue(ChronItem_t<DataType>& Value);

};


// =====================================================================
// =====================================================================


template<>
inline bool OPENFLUID_API ProgressiveChronFileReader<double>::getNextValue(ChronItem_t<double>& Value)
{
  std::vector<std::string> Values;
  openfluid::core::DateTime DT;
  double Val;

  while (getNextLine(Values))
  {
    if (Values.size() == 2)
    {
      if (DT.setFromString(Values.front(),m_DateFormat) &&
          openfluid::tools::toNumeric(Values.back(),Val))
      {
        Value.first = DT;
        Value.second = Val;
        return true;
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "wrong data in " + m_FileName);
      }
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


template<>
inline bool OPENFLUID_API ProgressiveChronFileReader<std::vector<std::string>>::getNextValue(
  ChronItem_t<std::vector<std::string>>& Value)
{
  std::vector<std::string> Values;
  openfluid::core::DateTime DT;

  while (getNextLine(Values))
  {
    if (Values.size() >= 2)
    {
      if (DT.setFromString(Values.front(),m_DateFormat) )
      {
        Value.first = DT;
        Value.second.clear();
        for (std::size_t i=1;i<Values.size();i++)
        {
          Value.second.push_back(Values[i]);
        }
        return true;
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "wrong data in " + m_FileName);
      }
    }
  }

  return false;
}


} } // namespaces


#endif /* __OPENFLUID_TOOLS_PROGRESSIVECHRONFILEREADER_HPP__ */
