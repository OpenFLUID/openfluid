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
 @file MarketDatasetPackage.cpp

 @author Manuel Chataigner <manuel.chataigner@supagro.inra.fr>
*/


#include <QProcess>

#include <openfluid/market/MarketDatasetPackage.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/utils/CMakeProxy.hpp>


namespace openfluid { namespace market {


MarketDatasetPackage::MarketDatasetPackage(const openfluid::ware::WareID_t& ID, const std::string& PackageURL)
                    : MarketPackage(ID, PackageURL)
{

}


// =====================================================================
// =====================================================================


std::string MarketDatasetPackage::getInstallPath() const
{
  return m_MarketBagDatasetDir;
}


// =====================================================================
// =====================================================================


PackageInfo::PackageType MarketDatasetPackage::getPackageType() const
{
  return PackageInfo::DATA;
}


// =====================================================================
// =====================================================================


void MarketDatasetPackage::process()
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


  std::string DatasetInstallDir = getInstallPath() + "/" + m_ID;

  if (openfluid::tools::Filesystem::isDirectory(DatasetInstallDir))
  {
    openfluid::tools::Filesystem::removeDirectory(DatasetInstallDir);
  }

  if (!openfluid::tools::Filesystem::makeDirectory(DatasetInstallDir))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "unable to create dataset directory for "+m_ID+" package");
  }

  QString ProcessCommand =
      openfluid::utils::CMakeProxy::getTarUncompressCommand(QString::fromStdString(DatasetInstallDir),
                                                            QString::fromStdString(m_PackageDest),"z");

  // uncompressing package
  appendToLogFile(m_PackageFilename,getPackageType(),"uncompressing datasets",ProcessCommand.toStdString());

  QProcess Uncompress;

  Uncompress.start(ProcessCommand);
  Uncompress.waitForFinished(-1);
  Uncompress.waitForReadyRead(-1);

  appendToLogFile(QString(Uncompress.readAllStandardOutput()).toStdString());

  int RetValue = Uncompress.exitCode();

  if (RetValue != 0)
  {
    appendToLogFile(QString(Uncompress.readAllStandardError()).toStdString());
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Error uncompressing package using CMake");
  }

}


} } // namespaces
