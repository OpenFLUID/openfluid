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
  @file ChronFileInterpolator.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <cmath>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/tools/ChronFileInterpolator.hpp>
#include <openfluid/tools/ColumnTextParser.hpp>


namespace openfluid { namespace tools {


ChronFileInterpolator::ChronFileInterpolator(const std::string& InFilePath, const std::string& OutFilePath,
                                             const openfluid::core::DateTime& BeginDate,
                                             const openfluid::core::DateTime& EndDate,
                                             const openfluid::core::Duration_t& DeltaT,
                                             PreProcess PrePcs)
: m_InFilePath(InFilePath), m_InDateFormat("%Y-%m-%dT%H:%M:%S"), m_InColumnSeparators(" \t\r\n"), m_InCommentChar("#"),
  m_OutFilePath(OutFilePath), m_OutDateFormat("%Y-%m-%dT%H:%M:%S"), m_OutColumnSeparator(" "), m_OutCommentChar("#"),
  m_BeginDate(BeginDate), m_EndDate(EndDate), m_DeltaT(DeltaT), m_PreProcess(PrePcs)
{

}


// =====================================================================
// =====================================================================


ChronFileInterpolator::~ChronFileInterpolator()
{

}


// =====================================================================
// =====================================================================


void ChronFileInterpolator::checkPreload()
{
  std::size_t ColSepFound;

  for (unsigned int i=0; i<m_InColumnSeparators.size(); i++)
  {
    ColSepFound = m_InDateFormat.find(m_InColumnSeparators[i]);
    if (ColSepFound != std::string::npos)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "column separator has been found in input file date format");
    }
  }


  ColSepFound = m_OutDateFormat.find(m_OutColumnSeparator);
  if (ColSepFound != std::string::npos)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "column separator has been found in output file date format");
  }


  if (m_BeginDate >= m_EndDate)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Begin date is greater or equal to end date");
  }
}


// =====================================================================
// =====================================================================


void ChronFileInterpolator::checkPostload()
{

}


// =====================================================================
// =====================================================================


void ChronFileInterpolator::loadInFile(ChronologicalSerie<double>& Data)
{
  checkPreload();

  ColumnTextParser FileParser(m_InCommentChar, m_InColumnSeparators);

  double Value;
  openfluid::core::DateTime ZeDT;

  Data.clear();


  if (!FileParser.loadFromFile(m_InFilePath))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "unable to load file "+m_InFilePath);
  }


  if (FileParser.getColsCount() == 2)
  {
    unsigned int i = 0;
    Value = 0;

    std::string DateStr;


    while (i<FileParser.getLinesCount())
    {
      if (FileParser.getStringValue(i,0,&DateStr) &&
          FileParser.getDoubleValue(i,1,&Value) &&
          ZeDT.setFromString(DateStr,m_InDateFormat))
      {

        if (!std::isnan(Value) && !std::isinf(Value))
        {

          if (!Data.empty() && Data.back().first > ZeDT )
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "wrong time chronology in "+m_InFilePath);
          }

          Data.push_back(std::make_pair(ZeDT,Value));

        }
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "wrong value read from "+m_InFilePath);
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "wrong file format in "+m_InFilePath);
      }

      i++;
    }
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "wrong global file format in "+m_InFilePath);
  }

  // checking of the loaded file
  if (Data.size() < 2)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "file "+
                                              m_InFilePath+
                                              " contains unsufficient values (at least 2 values needed)");
  }

  // checking of the covered period
  if (Data.front().first > m_BeginDate || Data.back().first < m_EndDate)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "serie in file "+m_InFilePath+" does not cover the requested period");
  }


  // clean unwanted values before begin date
  bool Continue = true;
  while (Continue && Data.size() >= 2)
  {
    if (Data.front().first < m_BeginDate && (*(++(Data.begin()))).first < m_BeginDate)
    {
      Data.pop_front();
    }
    else
    {
      Continue = false;
    }
  }


  // clean unwanted values after end date
  Continue = true;
  while (Continue && Data.size() >= 2)
  {
    if (Data.back().first > m_EndDate && (*(++(Data.rbegin()))).first > m_EndDate)
    {
      Data.pop_back();
    }
    else
    {
      Continue = false;
    }
  }


  // cumulate if necessary
  if (m_PreProcess == CUMULATE)
  {
    double CumulatedValue = 0;

    for (auto& LocalValue : Data)
    {
      CumulatedValue +=  LocalValue.second;
      LocalValue.second = CumulatedValue;
    }
  }

  checkPostload();
}


} } // namespaces
