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
  \file MarketPackage.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <openfluid/market/MarketPackage.hpp>

namespace openfluid { namespace market {


std::string MarketPackage::m_TempBuildsDir = "";
std::string MarketPackage::m_TempDownloadsDir = "";
std::string MarketPackage::m_MarketBagDir = "";

std::string MarketPackage::m_CMakeCommand = "";



MarketPackage::MarketPackage(openfluid::base::FuncID_t ID, std::string PackageURL)
              : m_ID(ID), m_PackageURL(PackageURL)
{

}


// =====================================================================
// =====================================================================


void MarketPackage::initialize()
{
  std::string CMakeProgram = "";

#if defined __unix__ || defined __APPLE__
  CMakeProgram = "cmake";
#endif

#if WIN32
  CMakeProgram = "cmake.exe";
#endif

  if (CMakeProgram.empty())
    throw openfluid::base::OFException("OpenFLUID framework","MarketPackage::initialize()","Unsupported system platform");


  std::vector<std::string> CMakePaths = openfluid::tools::GetFileLocationsUsingPATHEnvVar(CMakeProgram);

  if (!CMakePaths.empty())
  {
    m_CMakeCommand = boost::filesystem::path(CMakePaths[0]).string();
  }
  else
    throw openfluid::base::OFException("OpenFLUID framework","MarketPackage::initialize()","Required CMake program not found");


}


// =====================================================================
// =====================================================================


void MarketPackage::setWorksDirs(std::string TempBuildsDir, std::string TempDownloadsDir, std::string MarketBagDir)
{
  m_TempBuildsDir = TempBuildsDir;
  m_TempDownloadsDir = TempDownloadsDir;
  m_MarketBagDir = MarketBagDir;
}


// =====================================================================
// =====================================================================


void MarketPackage::download()
{

  m_PackageDest = boost::filesystem::path(m_TempDownloadsDir+"/"+m_ID+".tar.gz").string();

  openfluid::tools::CURLDownloader::downloadToFile(m_PackageURL, m_PackageDest);
}


} } // namespaces




