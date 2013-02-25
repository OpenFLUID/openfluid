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
 \file VectorDataset.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "VectorDataset.hpp"

#include <boost/filesystem/operations.hpp>
#include <openfluid/base/OFException.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/base/RuntimeEnv.hpp>

namespace openfluid {
namespace landr {

VectorDataset::VectorDataset(std::string FileName, std::string DriverName)
{
  OGRRegisterAll();

  OGRSFDriver* Driver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
      DriverName.c_str());

  if (!Driver)
    throw openfluid::base::OFException(
        "OpenFLUID framework", "VectorDataset::VectorDataset",
        "\"" + DriverName + "\" driver not available.");

  std::string Path = openfluid::core::GeoValue::computeAbsolutePath(
      getInitializedTmpPath(), FileName);

  if (isAlreadyExisting(Path))
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "VectorDataset::VectorDataset",
        "Error while creating " + Path
        + " : "
        + "A VectorDataset with this name already exists, try with another name.");

  mp_DataSource = Driver->CreateDataSource(Path.c_str(), NULL);

  if (!mp_DataSource)
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "VectorDataset::VectorDataset",
        "Error while creating " + Path + " : "
        + "Creation of OGRDataSource failed.");

  mp_DataSource->SetDriver(Driver);

  //TODO maybe do not create a layer (needed to open the datasource) nor write on disk now ?
  mp_DataSource->CreateLayer("", NULL, wkbUnknown, NULL);

  // necessary to ensure headers are written out in an orderly way and all resources are recovered
  OGRDataSource::DestroyDataSource(mp_DataSource);

  mp_DataSource = OGRSFDriverRegistrar::Open(Path.c_str(), true);

  if (!mp_DataSource)
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "VectorDataset::VectorDataset",
        "Error while opening " + Path + " : "
        + "Creation of OGRDataSource failed.");
}

// =====================================================================
// =====================================================================

VectorDataset::VectorDataset(openfluid::core::GeoVectorValue& Value,
                             std::string NewFileName)
{
  OGRRegisterAll();

  OGRDataSource* DS = Value.get(false);
  OGRSFDriver* Driver = DS->GetDriver();

  std::string FileName = NewFileName;

  if (FileName.empty())
  {
    std::string OriginalPath = DS->GetName();
    FileName = boost::filesystem::path(OriginalPath).filename().string();
  }

  std::string Path = openfluid::core::GeoValue::computeAbsolutePath(
      getInitializedTmpPath(), FileName);

  if (isAlreadyExisting(Path))
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "VectorDataset::VectorDataset",
        "Error while creating " + Path
        + " : "
        + "A VectorDataset with this name already exists, try with another name.");

  mp_DataSource = Driver->CopyDataSource(DS, Path.c_str(), NULL);

  if (!mp_DataSource)
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "VectorDataset::VectorDataset",
        "Error while creating " + Path + " : "
        + "Creation of OGRDataSource failed.");

  mp_DataSource->SetDriver(Driver);

  // necessary to ensure headers are written out in an orderly way and all resources are recovered
  OGRDataSource::DestroyDataSource(mp_DataSource);

  mp_DataSource = OGRSFDriverRegistrar::Open(Path.c_str(), true);

  if (!mp_DataSource)
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "VectorDataset::VectorDataset",
        "Error while opening " + Path + " : "
        + "Creation of OGRDataSource failed.");
}

// =====================================================================
// =====================================================================

std::string VectorDataset::getInitializedTmpPath()
{
  std::string TmpPath =
      openfluid::base::RuntimeEnvironment::getInstance()->getTempDir();

  if (!boost::filesystem::exists(TmpPath))
    boost::filesystem::create_directory(TmpPath);

  return TmpPath;
}

// =====================================================================
// =====================================================================

bool VectorDataset::isAlreadyExisting(std::string Path)
{
  OGRDataSource* DS = OGRSFDriverRegistrar::Open(Path.c_str(), false);

  if (DS)
  {
    OGRDataSource::DestroyDataSource(DS);
    return true;
  }

  return false;
}

// =====================================================================
// =====================================================================

VectorDataset::~VectorDataset()
{
  OGRSFDriver* Driver = mp_DataSource->GetDriver();
  std::string Path = mp_DataSource->GetName();

  OGRDataSource::DestroyDataSource(mp_DataSource);
  Driver->DeleteDataSource(Path.c_str());
}

// =====================================================================
// =====================================================================

OGRDataSource* VectorDataset::getDataSource()
{
  return mp_DataSource;
}

// =====================================================================
// =====================================================================

void VectorDataset::copyToDisk(std::string FilePath, std::string FileName,
                               bool ReplaceIfExists)
{
  OGRSFDriver* Driver = mp_DataSource->GetDriver();

  if (!boost::filesystem::exists(FilePath))
    boost::filesystem::create_directory(FilePath);

  std::string Path = openfluid::core::GeoValue::computeAbsolutePath(FilePath,
                                                                    FileName);

  OGRDataSource* DS = OGRSFDriverRegistrar::Open(Path.c_str(), false);

  if (DS)
  {
    if (ReplaceIfExists)
    {
      OGRSFDriver* Driver = DS->GetDriver();
      OGRDataSource::DestroyDataSource(DS);
      Driver->DeleteDataSource(Path.c_str());
    }
    else
    {
      OGRDataSource::DestroyDataSource(DS);
      throw openfluid::base::OFException(
          "OpenFLUID framework",
          "VectorDataset::copyToDisk",
          "Error while creating " + Path + " : "
          + "This VectorDataset already exists.");
    }
  }

  OGRDataSource* NewDS = Driver->CopyDataSource(mp_DataSource, Path.c_str(),
                                                NULL);

  if (!NewDS)
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "VectorDataset::copyToDisk",
        "Error while creating " + Path + " : "
        + "Copying of OGRDataSource failed.");

  // necessary to ensure headers are written out in an orderly way and all resources are recovered
  OGRDataSource::DestroyDataSource(NewDS);
}

// =====================================================================
// =====================================================================

}
} // namespaces
