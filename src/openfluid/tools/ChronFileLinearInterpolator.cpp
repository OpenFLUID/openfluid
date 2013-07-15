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
  \file ChronFileLinearInterpolator.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <fstream>
#include <iomanip>

#include <boost/filesystem/path.hpp>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/tools/ChronFileLinearInterpolator.hpp>


namespace openfluid { namespace tools {


ChronFileLinearInterpolator::ChronFileLinearInterpolator(const std::string& InFilePath, const std::string& OutFilePath,
                                                          const openfluid::core::DateTime& BeginDate, const openfluid::core::DateTime& EndDate,
                                                          const openfluid::core::Duration_t& DeltaT, PreProcess PrePcs)
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

  std::ofstream OutFile(boost::filesystem::path(m_OutFilePath).string().c_str());

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

      y = y0 + ( x * ( double(y1-y0)/double(x1)));
    }

    OutFile << CurrentDateTime.getAsString(m_OutDateFormat) << m_OutColumnSeparator << y << "\n";

    CurrentDateTime.addSeconds(m_DeltaT);
  }

  OutFile.close();

}


} } // namespaces
