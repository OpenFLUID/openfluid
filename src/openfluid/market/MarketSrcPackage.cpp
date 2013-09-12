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

#include <QProcess>


namespace openfluid { namespace market {


MarketSrcPackage::MarketSrcPackage(const openfluid::ware::WareID_t& ID, const std::string& PackageURL)
                : MarketPackage(ID,PackageURL),
                  m_KeepSources(true)
{

}


// =====================================================================
// =====================================================================


void MarketSrcPackage::process()
{
  if (!m_Initialized)
    throw openfluid::base::FrameworkException("MarketSrcPackage::download()","package "+m_PackageFilename+" not initialized");

  if (!m_Downloaded)
    throw openfluid::base::FrameworkException("MarketSrcPackage::process()","package "+m_PackageFilename+" cannot be processed before download");

  if (m_CMakeCommand.empty())
    throw openfluid::base::FrameworkException("MarketSrcPackage::process()","CMake command not defined");


  std::string BuildConfigOptions = composeFullBuildOptions(getPackageType(), m_BuildConfigOptions);

  std::string BuildDir = m_TempBuildsDir + "/" + m_ID;
  std::string SrcInstallDir = getInstallPath() + "/" + m_ID;

  // creating installation dir
  if (boost::filesystem::is_directory(boost::filesystem::path(SrcInstallDir)))
    boost::filesystem::remove_all(boost::filesystem::path(SrcInstallDir));

  if (!boost::filesystem::create_directories(boost::filesystem::path(SrcInstallDir)))
    throw openfluid::base::FrameworkException("MarketSrcPackage::process()","unable to create source directory for "+m_ID+" package");

  // creating build dir
  if (boost::filesystem::is_directory(boost::filesystem::path(BuildDir)))
    boost::filesystem::remove_all(boost::filesystem::path(BuildDir));

  if (!boost::filesystem::create_directories(boost::filesystem::path(BuildDir)))
    throw openfluid::base::FrameworkException("MarketSrcPackage::process()","unable to create build directory for "+m_ID+" package");

  // == Building commands ==

  std::string UntarCommand = "\"" + m_CMakeCommand +"\" -E chdir \"" + SrcInstallDir+ "\" \"" + m_CMakeCommand + "\" -E tar xfvz \"" + m_PackageDest + "\"";

  std::string BuildConfigCommand = "\"" + m_CMakeCommand +"\" -E chdir \"" + BuildDir+ "\" \"" + m_CMakeCommand + "\" \"" + SrcInstallDir + "\"" + BuildConfigOptions;

  std::string BuildCommand = "\"" + m_CMakeCommand +"\" -E chdir \"" + BuildDir+ "\" \"" + m_CMakeCommand + "\" --build .";



  // uncompressing package
  {
    QProcess Untar;

    Untar.start(QString::fromStdString(UntarCommand));
    Untar.waitForFinished(-1);
    Untar.waitForReadyRead(-1);

    appendToLogFile(QString(Untar.readAllStandardOutput()).toStdString());

    int RetValue = Untar.exitCode();

    if (RetValue != 0)
    {
      appendToLogFile(QString(Untar.readAllStandardError()).toStdString());
      throw openfluid::base::FrameworkException("MarketSrcPackage::process()","Error uncompressing sources package using CMake");
    }
  }

  // configuring the build
  {
    QProcess Config;

    Config.start(QString::fromStdString(BuildConfigCommand));
    Config.waitForFinished(-1);
    Config.waitForReadyRead(-1);

    appendToLogFile(QString(Config.readAllStandardOutput()).toStdString());

    int RetValue = Config.exitCode();

    if (RetValue != 0)
    {
      appendToLogFile(QString(Config.readAllStandardError()).toStdString());
      throw openfluid::base::FrameworkException("MarketSrcPackage::process()","Error configuring package build using CMake");
    }
  }

  // building
  {
    QProcess Build;

    Build.start(QString::fromStdString(BuildCommand));
    Build.waitForFinished(-1);
    Build.waitForReadyRead(-1);

    appendToLogFile(QString(Build.readAllStandardOutput()).toStdString());

    int RetValue = Build.exitCode();

    if (RetValue != 0)
    {
      appendToLogFile(QString(Build.readAllStandardError()).toStdString());
      throw openfluid::base::FrameworkException("MarketSrcPackage::process()","Error building package using CMake");
    }
  }


  // TODO add management of plugins suffixes for builder-extensions
  std::string PackagesPluginsSuffixes= "";
  if (getPackageType() == PackageInfo::SIM)
    PackagesPluginsSuffixes = openfluid::config::SIMULATORS_PLUGINS_SUFFIX;
  else if (getPackageType() == PackageInfo::OBS)
    PackagesPluginsSuffixes = openfluid::config::OBSERVERS_PLUGINS_SUFFIX;


  if (!boost::filesystem::exists(boost::filesystem::path(BuildDir+"/"+m_ID+PackagesPluginsSuffixes+openfluid::config::PLUGINS_EXT)))
    throw openfluid::base::FrameworkException("MarketSrcPackage::process()","Error finding built package");

  std::string BinInstallDir = getInstallPath() + "/../" + m_MarketBagBinSubDir;
  if (boost::filesystem::exists(boost::filesystem::path(BinInstallDir+"/"+m_ID+PackagesPluginsSuffixes +openfluid::config::PLUGINS_EXT)))
    boost::filesystem::remove(boost::filesystem::path(BinInstallDir+"/"+m_ID+PackagesPluginsSuffixes+openfluid::config::PLUGINS_EXT));

  boost::filesystem::copy_file(boost::filesystem::path(BuildDir+"/"+m_ID+PackagesPluginsSuffixes+openfluid::config::PLUGINS_EXT),
                               boost::filesystem::path(BinInstallDir+"/"+m_ID+PackagesPluginsSuffixes+openfluid::config::PLUGINS_EXT));

  if (!m_KeepSources) boost::filesystem::remove_all(boost::filesystem::path(SrcInstallDir));

}


} } // namespaces




