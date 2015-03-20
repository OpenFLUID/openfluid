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
  @file ProgressiveColumnFileReader.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <boost/algorithm/string.hpp>
#include <openfluid/tools/DataHelpers.hpp>

#include <openfluid/tools/ProgressiveColumnFileReader.hpp>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace tools {


ProgressiveColumnFileReader::ProgressiveColumnFileReader(const std::string& FileName,
                                                         const std::string& ColSeparators):
    m_File(FileName.c_str()), m_ColSeparators(ColSeparators), m_FileName(FileName)
{
  if (!m_File.is_open())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Can not open file " + FileName);
}


// =====================================================================
// =====================================================================


bool ProgressiveColumnFileReader::getNextLine(std::string& Line)
{
  if (!m_File.eof())
  {
    std::getline(m_File,Line);
    boost::algorithm::trim(Line);
    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool ProgressiveColumnFileReader::getNextLine(std::vector<std::string>& Values)
{
  std::string TmpStr;

  if (getNextLine(TmpStr))
  {
    Values = openfluid::tools::splitString(TmpStr,m_ColSeparators,false);
    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


void ProgressiveColumnFileReader::reset()
{
  if (m_File.is_open())
  {
    m_File.clear();
    m_File.seekg(0,std::ios::beg);
  }
}



} } // namespaces
