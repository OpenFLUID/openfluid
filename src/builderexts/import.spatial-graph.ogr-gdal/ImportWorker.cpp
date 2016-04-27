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
  @file ImportWorker.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include "ImportWorker.hpp"
#include "OGRGDALHelpers.hpp"

#include <QFileInfo>
#include <QDir>

#include <ogrsf_frmts.h>


ImportWorker::ImportWorker(const SourcesInfosList_t& SourcesInfos,
                           openfluid::fluidx::AdvancedFluidXDescriptor* AdvDesc,
                           const QString& InputDir):
  DataProcessingWorker(SourcesInfos,AdvDesc), m_InputDir(InputDir)
{

}


// =====================================================================
// =====================================================================


ImportWorker::~ImportWorker()
{

}


// =====================================================================
// =====================================================================


bool ImportWorker::importLayer(int Step,int Index)
{
  emit stepEntered(tr("Importing layer \"%1\"...").arg(m_SourcesInfos[Index].LayerName));

  QString UnitsClass = m_SourcesInfos[Index].UnitsClass;
  bool AtttrsCreated = false;

  QMapIterator<int, SourceUnit> It(m_SourcesData[Index].Units);

  while (It.hasNext())
  {
    It.next();

    openfluid::fluidx::SpatialUnitDescriptor* UDesc = new openfluid::fluidx::SpatialUnitDescriptor();

    UDesc->setUnitsClass(UnitsClass.toStdString());
    UDesc->setID(It.key());
    UDesc->setProcessOrder(It.value().ProcessOrder);

    mp_AdvDesc->spatialDomain().addUnit(UDesc);


    QMapIterator<QString,QString> AttrsIt(It.value().Attributes);

    // create attributes on first unit of the units class
    if (!AtttrsCreated)
    {
      while (AttrsIt.hasNext())
      {
        AttrsIt.next();
        mp_AdvDesc->spatialDomain().addAttribute(UnitsClass.toStdString(),
                                             AttrsIt.key().toStdString(),"-");
      }

      AttrsIt.toFront();
      AtttrsCreated = true;
    }

    // set attributes values for the current imported unit
    while (AttrsIt.hasNext())
    {
      AttrsIt.next();

      mp_AdvDesc->spatialDomain().attribute(UnitsClass.toStdString(),
                                            It.key(),
                                            AttrsIt.key().toStdString()) = AttrsIt.value().toStdString();
    }

  }

  emit stepCompleted(Step,getStyledText(tr("[OK]"),"green"));

  return true;
}


// =====================================================================
// =====================================================================


bool ImportWorker::buildConnections(int Step)
{
  emit stepEntered(tr("Building connections..."));

  int i=0;
  bool OKToContinue = true;

  while (i<m_SourcesInfos.size() && OKToContinue)
  {
    QMapIterator<int, SourceUnit> It(m_SourcesData[i].Units);

    while (It.hasNext())
    {
      It.next();

      // "to" connections
      for (int j=0;j<It.value().ToConn.size();j++)
      {
        mp_AdvDesc->spatialDomain().addFromToRelation(std::make_pair(m_SourcesInfos[i].UnitsClass.toStdString(),
                                                                 It.key()),
                                                  std::make_pair(It.value().ToConn[j].DestClass.toStdString(),
                                                                 It.value().ToConn[j].DestID));
      }

      // "childof" connections
      for (int j=0;j<It.value().ChildofConn.size();j++)
      {
        mp_AdvDesc->spatialDomain().addParentChildRelation(
            std::make_pair(It.value().ChildofConn[j].DestClass.toStdString(),
                           It.value().ChildofConn[j].DestID),
            std::make_pair(m_SourcesInfos[i].UnitsClass.toStdString(),It.key()));
      }
    }
    i++;
  }

  emit stepCompleted(Step,getStyledText(tr("[OK]"),"green"));

  return OKToContinue;

}


// =====================================================================
// =====================================================================


bool ImportWorker::processFilesAndDatastore(int Step)
{
  emit stepEntered(tr("Copying files and populating datastore..."));

  int i=0;
  bool OKToContinue = true;

  while (i<m_SourcesInfos.size() && OKToContinue)
  {

    if (m_SourcesInfos[i].IsDatasetImport)
    {
      // local copy of files in dataset

      QString FullSrcFilePath = m_SourcesInfos[i].SourceURI;
      if (!m_SourcesInfos[i].CachedSourceURI.isEmpty())
        FullSrcFilePath = m_SourcesInfos[i].CachedSourceURI;

      QString FullDestFilePath = m_InputDir + "/" + m_SourcesInfos[i].RelativeDatasetPath;
      QString FullDestPath = QFileInfo(FullDestFilePath).absolutePath();
      QString DestExtension = QFileInfo(FullDestFilePath).suffix();

      QDir().mkpath(FullDestPath);

      OGRRegisterAll();

      OGRDataSource* SrcDS = OGRSFDriverRegistrar::Open(FullSrcFilePath.toStdString().c_str(),FALSE);

      if (SrcDS != nullptr)
      {
        OGRSFDriver *CopyDriver;

        QString DriverName = OGRGDALHelpers::getDriverFromFileExt(DestExtension);

        CopyDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(DriverName.toStdString().c_str());

        if (CopyDriver != nullptr)
        {
          OGRDataSource* DestDS = CopyDriver->CopyDataSource(SrcDS,
                                                             FullDestFilePath.toStdString().c_str());

          OGRDataSource::DestroyDataSource(DestDS);
        }

        OGRDataSource::DestroyDataSource(SrcDS);

      }
    }

    if (m_SourcesInfos[i].IsDatastore)
    {

      QString RelativePath = m_SourcesInfos[i].RelativeDatasetPath;

      if (m_SourcesInfos[i].IsAlreadyInDataset)
        RelativePath = QDir(m_InputDir).relativeFilePath(m_SourcesInfos[i].SourceURI);


      // populate datastore

      openfluid::fluidx::DatastoreItemDescriptor* DSItem =
          new openfluid::fluidx::DatastoreItemDescriptor(m_SourcesInfos[i].DatastoreID.toStdString(),
                                                         m_InputDir.toStdString(),
                                                         RelativePath.toStdString(),
                                                         openfluid::core::UnstructuredValue::GeoVectorValue);
      DSItem->setUnitsClass(m_SourcesInfos[i].UnitsClass.toStdString());

      mp_AdvDesc->datastore().appendItem(DSItem);
    }

    i++;
  }

  emit stepCompleted(Step,getStyledText(tr("[OK]"),"green"));

  return true;
}


// =====================================================================
// =====================================================================


bool ImportWorker::runImport(int StartStep)
{
  int i=0;

  bool OKToContinue = true;

  while (i<m_SourcesInfos.size() && OKToContinue)
  {
    OKToContinue = importLayer(StartStep+i,i);
    i++;
  }

  if (OKToContinue)
    OKToContinue = buildConnections(StartStep+m_SourcesInfos.size()+1);

  if (OKToContinue)
    OKToContinue = processFilesAndDatastore(StartStep+m_SourcesInfos.size()+2);

  return OKToContinue;
}


// =====================================================================
// =====================================================================


void ImportWorker::run()
{
  if (!runCheck(1))
    emit completed(getStyledText(tr("Consistency error, "
                                    "import could not be performed with current import configuration"),
                                 "red",true));
  else
  {
    if (runImport(OGRGDALEXT_PRECHECK_STEPS+1))
    {
      emit completed(getStyledText(tr("Import sucessful"),
                                       "green",true));
    }
    else
    {
      emit completed(getStyledText(tr("Import error. "
                                       "Import may be uncompleted or inconsistent"),
                                       "red",true));
    }

    emit closeRequired();
  }

  emit finished();
}
