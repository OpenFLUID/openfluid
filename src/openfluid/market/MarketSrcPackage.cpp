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

#include <glibmm/error.h>
#include <glibmm/spawn.h>



namespace openfluid { namespace market {


MarketSrcPackage::MarketSrcPackage(openfluid::ware::WareID_t ID, std::string PackageURL)
                : MarketPackage(ID,PackageURL),
                  m_KeepSources(true)
{

}


// =====================================================================
// =====================================================================


void MarketSrcPackage::process()
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","MarketSrcPackage::download()","package "+m_PackageFilename+" not initialized");

  if (!m_Downloaded)
    throw openfluid::base::OFException("OpenFLUID framework","MarketSrcPackage::process()","package "+m_PackageFilename+" cannot be processed before download");

  if (m_CMakeCommand.empty())
    throw openfluid::base::OFException("OpenFLUID framework","MarketSrcPackage::process()","CMake command not defined");

  std::string StrOut;
  std::string StrErr;
  int RetValue;

  std::string BuildConfigOptions = composeFullBuildOptions(getTypePackage(), m_BuildConfigOptions);

  std::string BuildDir = m_TempBuildsDir + "/" + m_ID;
  std::string SrcInstallDir = getInstallPath() + "/" + m_ID;

  if (boost::filesystem::is_directory(boost::filesystem::path(SrcInstallDir)))
    boost::filesystem::remove_all(boost::filesystem::path(SrcInstallDir));

  if (!boost::filesystem::create_directories(boost::filesystem::path(SrcInstallDir)))
    throw openfluid::base::OFException("OpenFLUID framework","MarketSrcPackage::process()","unable to create source directory for "+m_ID+" package");

  if (boost::filesystem::is_directory(boost::filesystem::path(BuildDir)))
    boost::filesystem::remove_all(boost::filesystem::path(BuildDir));

  if (!boost::filesystem::create_directories(boost::filesystem::path(BuildDir)))
    throw openfluid::base::OFException("OpenFLUID framework","MarketSrcPackage::process()","unable to create build directory for "+m_ID+" package");

  std::string UntarCommand = "\"" + m_CMakeCommand +"\" -E chdir \"" + SrcInstallDir+ "\" \"" + m_CMakeCommand + "\" -E tar xfvz \"" + m_PackageDest + "\"";

  std::string BuildConfigCommand = "\"" + m_CMakeCommand +"\" -E chdir \"" + BuildDir+ "\" \"" + m_CMakeCommand + "\" \"" + SrcInstallDir + "\"" + BuildConfigOptions;

  std::string BuildCommand = "\"" + m_CMakeCommand +"\" -E chdir \"" + BuildDir+ "\" \"" + m_CMakeCommand + "\" --build .";


  // uncompressing package
  try
  {
    appendToLogFile(m_PackageFilename,"uncompressing sources",UntarCommand);

    StrOut.clear();
    StrErr.clear();
    RetValue = 0;
    Glib::spawn_command_line_sync(UntarCommand,&StrOut,&StrErr,&RetValue);

    appendToLogFile(StrOut);

    if (RetValue != 0)
    {
      appendToLogFile(StrErr);
      throw openfluid::base::OFException("OpenFLUID framework","MarketSrcPackage::process()","Error uncompressing package using CMake");
    }
  }
  catch (Glib::Error& E)
  {
    throw openfluid::base::OFException("OpenFLUID framework","MarketSrcPackage::process()","Glib error uncompressing package using CMake");
  }


  // configuring build
  try
  {
    appendToLogFile(m_PackageFilename,"configuring sources build",BuildConfigCommand);

    StrOut.clear();
    StrErr.clear();
    RetValue = 0;
    Glib::spawn_command_line_sync(BuildConfigCommand,&StrOut,&StrErr,&RetValue);

    appendToLogFile(StrOut);

    if (RetValue != 0)
    {
      appendToLogFile(StrErr);
      throw openfluid::base::OFException("OpenFLUID framework","MarketSrcPackage::process()","Error configuring package build using CMake");
    }

  }
  catch (Glib::Error& E)
  {
    throw openfluid::base::OFException("OpenFLUID framework","MarketSrcPackage::process()","Glib error configuring package build using CMake");
  }


  // building
  try
  {
    appendToLogFile(m_PackageFilename,"building sources",BuildCommand);

    StrOut.clear();
    StrErr.clear();
    RetValue = 0;
    Glib::spawn_command_line_sync(BuildCommand,&StrOut,&StrErr,&RetValue);

    appendToLogFile(StrOut);

    if (RetValue != 0)
    {
      appendToLogFile(StrErr);
      throw openfluid::base::OFException("OpenFLUID framework","MarketSrcPackage::process()","Error building package using CMake");

    }

  }
  catch (Glib::Error& E)
  {
    throw openfluid::base::OFException("OpenFLUID framework","MarketSrcPackage::process()","Glib error building package using CMake");
  }

  if (!boost::filesystem::exists(boost::filesystem::path(BuildDir+"/"+m_ID+openfluid::config::FUNCTIONS_PLUGINS_SUFFIX+openfluid::config::PLUGINS_EXT)))
    throw openfluid::base::OFException("OpenFLUID framework","MarketSrcPackage::process()","Error finding built package");

  std::string BinInstallDir = getInstallPath() + "/../" + getMarketBagBinSubDir();
  if (boost::filesystem::exists(boost::filesystem::path(BinInstallDir+"/"+m_ID+openfluid::config::FUNCTIONS_PLUGINS_SUFFIX +openfluid::config::PLUGINS_EXT)))
    boost::filesystem::remove(boost::filesystem::path(BinInstallDir+"/"+m_ID+openfluid::config::FUNCTIONS_PLUGINS_SUFFIX+openfluid::config::PLUGINS_EXT));

  boost::filesystem::copy_file(boost::filesystem::path(BuildDir+"/"+m_ID+openfluid::config::FUNCTIONS_PLUGINS_SUFFIX+openfluid::config::PLUGINS_EXT),
                               boost::filesystem::path(BinInstallDir+"/"+m_ID+openfluid::config::FUNCTIONS_PLUGINS_SUFFIX+openfluid::config::PLUGINS_EXT));

  if (!m_KeepSources) boost::filesystem::remove_all(boost::filesystem::path(SrcInstallDir));

}


} } // namespaces




