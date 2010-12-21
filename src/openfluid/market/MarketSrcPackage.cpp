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
  \file MarketSrcPackage.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <openfluid/market/MarketSrcPackage.hpp>
#include <openfluid/config.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>



namespace openfluid { namespace market {


MarketSrcPackage::MarketSrcPackage(openfluid::base::FuncID_t ID, std::string PackageURL)
                : MarketPackage(ID,PackageURL)
{

}


// =====================================================================
// =====================================================================


void MarketSrcPackage::setAdditionalBuildConfigOptions(const std::string& Options)
{
  m_BuildConfigOptions = Options;
}


// =====================================================================
// =====================================================================


void MarketSrcPackage::process()
{

  if (m_CMakeCommand.empty())
    throw openfluid::base::OFException("OpenFLUID framework","MarketSrcPackage::process()","CMake command not defined");

  if (!m_BuildConfigOptions.empty()) m_BuildConfigOptions = " " + m_BuildConfigOptions;

  std::string BuildDir = m_TempBuildsDir + "/" + m_ID;

  if (!boost::filesystem::create_directories(boost::filesystem::path(BuildDir)))
    throw openfluid::base::OFException("OpenFLUID framework","MarketSrcPackage::process()","unable to create build directory for "+m_ID+" package");

  std::string UntarCommand = m_CMakeCommand +" -E chdir " + BuildDir+ " " + m_CMakeCommand + " -E tar xfz " + m_PackageDest;

  std::string BuildConfigCommand = m_CMakeCommand +" -E chdir " + BuildDir+ " " + m_CMakeCommand + " ." + m_BuildConfigOptions;

  std::string BuildCommand = m_CMakeCommand +" -E chdir " + BuildDir+ " " + m_CMakeCommand + " --build . --target "+m_ID;


  // uncompressing package
  if (std::system(UntarCommand.c_str()) != 0)
    throw openfluid::base::OFException("OpenFLUID framework","MarketBinPackage::process()","Error uncompressing package using CMake");

  // configuring build
  if (std::system(BuildConfigCommand.c_str()) != 0)
    throw openfluid::base::OFException("OpenFLUID framework","MarketBinPackage::process()","Error configuring package build using CMake");

  // building
  if (std::system(BuildCommand.c_str()) != 0)
    throw openfluid::base::OFException("OpenFLUID framework","MarketBinPackage::process()","Error building package using CMake");

  boost::filesystem::copy_file(boost::filesystem::path(BuildDir+"/"+m_ID+openfluid::config::PLUGINS_EXT),
                                    boost::filesystem::path(m_MarketBagDir+"/"+m_ID+openfluid::config::PLUGINS_EXT));
}


} } // namespaces




