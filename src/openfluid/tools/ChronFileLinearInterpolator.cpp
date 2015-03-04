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
  @file ChronFileLinearInterpolator.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <fstream>
#include <iomanip>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/tools/ChronFileLinearInterpolator.hpp>
#include <openfluid/scientific/Interpolators.hpp>


namespace openfluid { namespace tools {


ChronFileLinearInterpolator::ChronFileLinearInterpolator(const std::string& InFilePath, const std::string& OutFilePath,
                                                         const openfluid::core::DateTime& BeginDate,
                                                         const openfluid::core::DateTime& EndDate,
                                                         const openfluid::core::Duration_t& DeltaT,
                                                         PreProcess PrePcs)
: ChronFileInterpolator(InFilePath,OutFilePath,BeginDate,EndDate,DeltaT, PrePcs)
{

}


// =====================================================================
// =====================================================================


ChronFileLinearInterpolator::~ChronFileLinearInterpolator()
{

}


// =====================================================================
// =====================================================================


void ChronFileLinearInterpolator::runInterpolation()
{
  ChronologicalSerie Data;

  loadInFile(Data);

  openfluid::core::DateTime CurrentDateTime = m_BeginDate;
  ChronItem_t BeforeCurrentTime, AfterCurrentTime;

  long x,x1;
  double y,y0,y1;

  std::ofstream OutFile(m_OutFilePath.c_str());

  OutFile << std::setprecision(15);

  while (CurrentDateTime <= m_EndDate && Data.getSurroundingValues(CurrentDateTime,BeforeCurrentTime,AfterCurrentTime))
  {

    if (BeforeCurrentTime.first == AfterCurrentTime.first)
      y = BeforeCurrentTime.second;
    else
    {
      x = CurrentDateTime.diffInSeconds(BeforeCurrentTime.first);
      x1 = AfterCurrentTime.first.diffInSeconds(BeforeCurrentTime.first);
      y0 = BeforeCurrentTime.second;
      y1 = AfterCurrentTime.second;

      y = openfluid::scientific::linearInterpolationFromXOrigin(y0,double(x1),y1,double(x));
    }

    OutFile << CurrentDateTime.getAsString(m_OutDateFormat) << m_OutColumnSeparator << y << "\n";

    CurrentDateTime.addSeconds(m_DeltaT);
  }

  OutFile.close();
}


} } // namespaces
