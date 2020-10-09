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
  @file MarketSrcPackage.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QProcess>

#include <openfluid/market/MarketSrcPackage.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/utils/CMakeProxy.hpp>
#include <openfluid/config.hpp>


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
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "package "+m_PackageFilename+" not initialized");
  }

  if (!m_Downloaded)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "package "+m_PackageFilename+" cannot be processed before download");
  }

  if (!openfluid::utils::CMakeProxy::isAvailable())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"CMake command not defined");
  }


  std::string BuildConfigOptions = composeFullBuildOptions(getPackageType(), m_BuildConfigOptions);

  std::string BuildDir = openfluid::tools::Filesystem::joinPath({m_TempBuildsDir,m_ID});
  std::string SrcInstallDir = openfluid::tools::Filesystem::joinPath({getInstallPath(),m_ID});

  // creating installation dir
  if (openfluid::tools::Filesystem::isDirectory(SrcInstallDir))
  {
    openfluid::tools::Filesystem::removeDirectory(SrcInstallDir);
  }

  if (!openfluid::tools::Filesystem::makeDirectory(SrcInstallDir))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "unable to create source directory for "+m_ID+" package");
  }

  // creating build dir
  if (openfluid::tools::Filesystem::isDirectory(BuildDir))
  {
    openfluid::tools::Filesystem::removeDirectory(BuildDir);
  }

  if (!openfluid::tools::Filesystem::makeDirectory(BuildDir))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "unable to create build directory for "+m_ID+" package");
  }


  // == Building commands ==

  openfluid::utils::CMakeProxy::CommandInfos UntarCommand = 
    openfluid::utils::CMakeProxy::getTarUncompressCommand(QString::fromStdString(SrcInstallDir),
                                                          QString::fromStdString(m_PackageDest),
                                                          "z");


  // converting string args as a list, without splitting spaces in quotes
  QStringList BuildConfigOptionsList = 
  openfluid::tools::convertArgsStringToList(QString::fromStdString(BuildConfigOptions));

  openfluid::utils::CMakeProxy::CommandInfos BuildConfigCommand =
      openfluid::utils::CMakeProxy::getConfigureCommand(QString::fromStdString(BuildDir),
                                                        QString::fromStdString(SrcInstallDir),
                                                        {},"",
                                                        BuildConfigOptionsList);


  openfluid::utils::CMakeProxy::CommandInfos BuildCommand = 
    openfluid::utils::CMakeProxy::getBuildCommand(QString::fromStdString(BuildDir));


  // uncompressing package
  {
    QProcess Untar;

    Untar.start(UntarCommand.Program,UntarCommand.Args);
    Untar.waitForFinished(-1);
    Untar.waitForReadyRead(-1);

    appendToLogFile(QString(Untar.readAllStandardOutput()).toStdString());

    int RetValue = Untar.exitCode();

    if (RetValue != 0)
    {
      appendToLogFile(QString(Untar.readAllStandardError()).toStdString());
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "Error uncompressing sources package using CMake");
    }
  }

  // configuring the build
  {
    QProcess Config;

    Config.start(BuildConfigCommand.Program,BuildConfigCommand.Args);
    Config.waitForFinished(-1);
    Config.waitForReadyRead(-1);

    appendToLogFile(QString(Config.readAllStandardOutput()).toStdString());

    int RetValue = Config.exitCode();

    if (RetValue != 0)
    {
      appendToLogFile(QString(Config.readAllStandardError()).toStdString());
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "Error configuring package build using CMake");
    }
  }

  // building
  {
    QProcess Build;

    Build.start(BuildCommand.Program,BuildCommand.Args);
    Build.waitForFinished(-1);
    Build.waitForReadyRead(-1);

    appendToLogFile(QString(Build.readAllStandardOutput()).toStdString());

    int RetValue = Build.exitCode();

    if (RetValue != 0)
    {
      appendToLogFile(QString(Build.readAllStandardError()).toStdString());
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Error building package using CMake");
    }
  }


  // TODO add management of plugins suffixes for builder-extensions
  std::string PackagesPluginsSuffixes= "";
  if (getPackageType() == PackageInfo::SIM)
  {
    PackagesPluginsSuffixes = openfluid::config::SIMULATORS_PLUGINS_SUFFIX;
  }
  else if (getPackageType() == PackageInfo::OBS)
  {
    PackagesPluginsSuffixes = openfluid::config::OBSERVERS_PLUGINS_SUFFIX;
  }


  std::string BuildFile = 
    openfluid::tools::Filesystem::joinPath({BuildDir,m_ID+PackagesPluginsSuffixes+openfluid::config::PLUGINS_EXT});
  std::string DestFile = 
    openfluid::tools::Filesystem::joinPath({getInstallPath(),"..",m_MarketBagBinSubDir,
                                            m_ID+PackagesPluginsSuffixes+openfluid::config::PLUGINS_EXT});

  if (!openfluid::tools::Filesystem::isFile(BuildFile))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Error finding built package");
  }

  if (openfluid::tools::Filesystem::isFile(DestFile))
  {
    openfluid::tools::Filesystem::removeFile(DestFile);
  }

  openfluid::tools::Filesystem::copyFile(BuildFile,DestFile);

  if (!m_KeepSources)
  {
    openfluid::tools::Filesystem::removeDirectory(SrcInstallDir);
  }

}


} } // namespaces
