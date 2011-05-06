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
  \file ViewWithGNUplot.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include <openfluid/tools.hpp>
#include <openfluid/base.hpp>
#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <glibmm/i18n.h>

#include <fstream>

#include "ViewWithGNUplot.hpp"

std::string ViewWithGNUplot::m_GNUplotProgram = "";


// =====================================================================
// =====================================================================


ViewWithGNUplot::ViewWithGNUplot()
{

}


// =====================================================================
// =====================================================================


void ViewWithGNUplot::FindGNUplot()
{
  std::string GNUplotFile = "";

#if defined __unix__ || defined __APPLE__
  GNUplotFile = "gnuplot";
#endif

#if WIN32
  GNUplotFile = "gnuplot.exe";
#endif

  if (!GNUplotFile.empty())
  {
    std::vector<std::string> GNUplotPaths = openfluid::tools::GetFileLocationsUsingPATHEnvVar(GNUplotFile);

    if (!GNUplotPaths.empty())
      m_GNUplotProgram = boost::filesystem::path(GNUplotPaths[0]).string();
    else
      m_GNUplotProgram = "";
  }
}


// =====================================================================
// =====================================================================


bool ViewWithGNUplot::IsGNUplotAvailable()
{
  return !m_GNUplotProgram.empty();
};

// =====================================================================
// =====================================================================

std::vector<std::string> ViewWithGNUplot::getOrderedScalarVariablesNames(const std::string& Data)
{
  std::string BeginStr("scalar variables order (after date and time columns): ");

  std::size_t FoundBegin = Data.find(BeginStr);

  if (FoundBegin != std::string::npos)
  {
    std::size_t FoundEnd = Data.find("\n",FoundBegin+1);
    if (FoundEnd != std::string::npos)
    {
      FoundBegin = FoundBegin +BeginStr.size();
      std::string VarsStr = Data.substr(FoundBegin,FoundEnd-FoundBegin);

      return openfluid::tools::SplitString(VarsStr," ",false);
    }
  }


  return std::vector<std::string>();
}


// =====================================================================
// =====================================================================


std::string ViewWithGNUplot::getVectorVariableName(const std::string& Data)
{
  std::string BeginStr("vector variable: ");

  std::size_t FoundBegin = Data.find(BeginStr);

  if (FoundBegin != std::string::npos)
  {
    std::size_t FoundEnd = Data.find("\n",FoundBegin+1);
    if (FoundEnd != std::string::npos)
    {
      FoundBegin = FoundBegin +BeginStr.size();
      std::string VarStr = Data.substr(FoundBegin,FoundEnd-FoundBegin);

      return VarStr;
    }
  }

  return "";
}


// =====================================================================
// =====================================================================


void ViewWithGNUplot::run(const std::string& Data,
                          const std::string& DateFormat,
                          const std::string& ColSeparator,
                          const std::string& CommentChar,
                          bool SingleWindow)
{

  if (!getVectorVariableName(Data).empty())
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(_("Unable to plot file with GNUplot:\nVector variables are not currently supported."));
    return;
  }

  if (DateFormat.find(ColSeparator) != std::string::npos)
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(_("Unable to plot file with GNUplot:\nColumn separator is present in date format."));
    return;
  }

  boost::filesystem::create_directories(boost::filesystem::path(openfluid::base::RuntimeEnvironment::getInstance()->getTempDir()));

  std::string DataFilename = boost::filesystem::path(openfluid::base::RuntimeEnvironment::getInstance()->getTempDir()+"/gprun.data").string();
  std::string ScriptFilename = boost::filesystem::path(openfluid::base::RuntimeEnvironment::getInstance()->getTempDir()+"/gprun.gp").string();

  std::ofstream DataFile(DataFilename.c_str());
  DataFile << Data;
  DataFile.close();

  std::vector<std::string> VarNames = getOrderedScalarVariablesNames(Data);


  if (VarNames.size() <1 )
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(_("No data found for plotting"));
    return;
  }

  // determine columns and rows

  if (SingleWindow)
  {

    unsigned int Columns = 1;
    unsigned int Rows = 1;

    if (VarNames.size() > 1)
    {
      Columns = (unsigned int)(std::ceil(std::sqrt(VarNames.size())));
      Rows = (unsigned int)(std::ceil(VarNames.size() / Columns));
    }


    // generate script
    std::ofstream ScriptFile(ScriptFilename.c_str());
    ScriptFile << "set nokey\n";
    ScriptFile << "set xdata time\n";
    ScriptFile << "set timefmt \"" << DateFormat << "\"\n";
    ScriptFile << "set datafile separator \"" << ColSeparator << "\n";
    ScriptFile << "set datafile commentschars \"" << CommentChar << "\n";
    ScriptFile << "set format x \"%Y-%m-%d\\n%H:%M:%S\"\n";
    ScriptFile << "set xtics autofreq font \",7\"\n";
    ScriptFile << "set ytics autofreq font \",7\"\n";

    ScriptFile << "set origin 0,0\n";
    ScriptFile << "set multiplot layout " << Rows << "," << Columns << " rowsfirst scale 1,1\n";

    for (unsigned int i=0; i< VarNames.size();i++)
    {
      ScriptFile << "set title \"" << VarNames[i] << "\" font \",9\"\n";
      ScriptFile << "plot \""<< DataFilename << "\" using 1:"<< (i+2) <<" with lines\n";
    }

    ScriptFile << "unset multiplot\n";

    ScriptFile.close();

    if(system(std::string(m_GNUplotProgram + " -persist " + ScriptFilename).c_str()) != 0)
      openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(_("GNUplot error"));
  }
  else
  {
    for (unsigned int i=0; i< VarNames.size();i++)
    {
      // generate script
      std::ofstream ScriptFile(ScriptFilename.c_str());
      ScriptFile << "set nokey\n";
      ScriptFile << "set xdata time\n";
      ScriptFile << "set timefmt \"" << DateFormat << "\"\n";
      ScriptFile << "set datafile separator \"" << ColSeparator << "\n";
      ScriptFile << "set datafile commentschars \"" << CommentChar << "\n";
      ScriptFile << "set format x \"%Y-%m-%d\\n%H:%M:%S\"\n";
      ScriptFile << "set xtics autofreq font \",7\"\n";
      ScriptFile << "set ytics autofreq font \",7\"\n";
      ScriptFile << "set title \"" << VarNames[i] << "\" font \",9\"\n";
      ScriptFile << "plot \""<< DataFilename << "\" using 1:"<< (i+2) <<" with lines\n";
      ScriptFile.close();

      if(system(std::string(m_GNUplotProgram + " -persist " + ScriptFilename).c_str()) != 0)
        openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(_("GNUplot error"));

    }
  }
}

