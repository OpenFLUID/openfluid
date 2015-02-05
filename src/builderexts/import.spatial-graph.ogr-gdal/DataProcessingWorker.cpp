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
  @file DataProcessingWorker.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include "DataProcessingWorker.hpp"
#include "OGRGDALHelpers.hpp"

#include <ogrsf_frmts.h>
#include <openfluid/utils/GDALHelpers.hpp>
#include <QFileInfo>


DataProcessingWorker::DataProcessingWorker(const SourcesInfosList_t& SourcesInfos,
                                           openfluid::fluidx::AdvancedFluidXDescriptor* AdvDesc):
  m_SourcesInfos(SourcesInfos), mp_AdvDesc(AdvDesc)
{
  for (int i=0; i<m_SourcesInfos.size();i++)
    m_SourcesData.append(SourceData());
}


// =====================================================================
// =====================================================================


DataProcessingWorker::~DataProcessingWorker()
{

}


// =====================================================================
// =====================================================================


QString DataProcessingWorker::getStyledText(const QString& Text,
                                            const QString& Color, bool IsBold)
{
  QString BoldStyle;

  if (IsBold)
    BoldStyle = " font-weight: bold;";

  return QString("<font style='color: %1;%2'>%3</font>").arg(Color,BoldStyle,Text);
}


// =====================================================================
// =====================================================================


bool DataProcessingWorker::isUnitExists(const QString& Class, int ID)
{
  // is unit exist in imported units

  int i=0;
  while (i<m_SourcesInfos.size())
  {
    if (m_SourcesInfos[i].UnitsClass == Class && m_SourcesData[i].isUnitExists(ID))
      return true;
    i++;
  }

  // is unit exist in existing units
  return mp_AdvDesc->spatialDomain().isSpatialUnitExist(Class.toStdString(),ID);
}


// =====================================================================
// =====================================================================


bool DataProcessingWorker::loadDataFromSources(int Step)
{
  emit stepEntered(tr("Loading and checking data from sources..."));


  OGRRegisterAll();

  for (int i=0; i<m_SourcesInfos.size();i++)
  {

    QString RealURI = m_SourcesInfos[i].SourceURI;
    if (!m_SourcesInfos[i].CachedSourceURI.isEmpty())
      RealURI = m_SourcesInfos[i].CachedSourceURI;


    OGRDataSource* DS = OGRSFDriverRegistrar::Open(RealURI.toStdString().c_str(),FALSE);

    if (DS == NULL)
    {
      emit stepCompleted(Step,getStyledText(tr("[Error] Unable to open datasource for layer \"%1\"")
                                            .arg(m_SourcesInfos[i].LayerName),"red"));
      return false;
    }

    OGRLayer *Layer;
    OGRFeature *Feature;

    Layer = DS->GetLayerByName(m_SourcesInfos[i].LayerName.toStdString().c_str());

    // For cached layers as GeoJSON files
    // TODO do better for that!
    if (Layer == NULL)
      Layer = DS->GetLayer(0);

    Layer->ResetReading();
    while((Feature = Layer->GetNextFeature()) != NULL )
    {
      SourceUnit CurrentUnit;
      int CurrentUnitID;

      int FieldIndex = 0;
      QString FieldValue;

      // === Unit ID ===

      FieldIndex = Feature->GetFieldIndex(OGRGDAL_UNITID_FIELD);

      if (FieldIndex < 0)
      {
        OGRDataSource::DestroyDataSource(DS);
        emit stepCompleted(Step,getStyledText(tr("[Error] Field for unit ID not found in layer \"%1\"")
                                              .arg(m_SourcesInfos[i].LayerName),"red"));
        return false;
      }

      CurrentUnitID = Feature->GetFieldAsInteger(FieldIndex);

      if (CurrentUnitID <= 0)
      {
        OGRDataSource::DestroyDataSource(DS);
        emit stepCompleted(Step,getStyledText(tr("[Error] Wrong field format for unit ID in layer \"%1\"")
                                              .arg(m_SourcesInfos[i].LayerName),"red"));
        return false;
      }

      if (m_SourcesData[i].isUnitExists(CurrentUnitID))
      {
        OGRDataSource::DestroyDataSource(DS);
        emit stepCompleted(Step,getStyledText(tr("[Error] Unit ID %2 already exist in layer \"%1\"")
                                                 .arg(m_SourcesInfos[i].LayerName)
                                                 .arg(CurrentUnitID),
                                              "red"));
        return false;
      }


      // === Process order ===

      if (m_SourcesInfos[i].UnitsPcsOrdField.isEmpty())
      {
        CurrentUnit.ProcessOrder = 1;
      }
      else
      {
        FieldIndex = Feature->GetFieldIndex(m_SourcesInfos[i].UnitsPcsOrdField.toStdString().c_str());

        if (FieldIndex < 0)
        {
          OGRDataSource::DestroyDataSource(DS);
          emit stepCompleted(Step,getStyledText(tr("[Error] Field for process order not found in layer \"%1\"")
                                                   .arg(m_SourcesInfos[i].LayerName),
                                                "red"));
          return false;
        }

        int PcsOrd = Feature->GetFieldAsInteger(FieldIndex);

        if (PcsOrd <= 0)
        {
          OGRDataSource::DestroyDataSource(DS);
          emit stepCompleted(Step,getStyledText(tr("[Error] Wrong field format for process order in layer \"%1\"")
                                                 .arg(m_SourcesInfos[i].LayerName),
                                                "red"));
          return false;
        }

        CurrentUnit.ProcessOrder = PcsOrd;
      }


      // === To connections ===

      if (!m_SourcesInfos[i].ToConnectionsField.isEmpty())
      {
        FieldIndex = Feature->GetFieldIndex(m_SourcesInfos[i].ToConnectionsField.toStdString().c_str());

        if (FieldIndex < 0)
        {
          OGRDataSource::DestroyDataSource(DS);
          emit stepCompleted(Step,getStyledText(tr("[Error] Field for \"to\" connections not found in layer \"%1\"")
                                                .arg(m_SourcesInfos[i].LayerName),"red"));
          return false;
        }

        CurrentUnit.ToConnStr = QString(Feature->GetFieldAsString(FieldIndex));

        if (!OGRGDALHelpers::convertConnectionsStringToList(CurrentUnit.ToConnStr,CurrentUnit.ToConn))
        {
          emit stepCompleted(Step,getStyledText(tr("[Error] Wrong field \"%2\" format for \"to\" connections "
                                                   "in layer \"%1\"")
                                                   .arg(m_SourcesInfos[i].LayerName)
                                                   .arg(m_SourcesInfos[i].ToConnectionsField),
                                                "red"));
          return false;
        }
      }


      // === Childof connections ===

      if (!m_SourcesInfos[i].ChildofConnectionsField.isEmpty())
      {
        FieldIndex = Feature->GetFieldIndex(m_SourcesInfos[i].ChildofConnectionsField.toStdString().c_str());

        if (FieldIndex < 0)
        {
          OGRDataSource::DestroyDataSource(DS);
          emit stepCompleted(Step,
                             getStyledText(tr("[Error] Field for \"childof\" connections not found in layer \"%1\"")
                                           .arg(m_SourcesInfos[i].LayerName),"red"));
          return false;
        }

        CurrentUnit.ChildofConnStr = QString(Feature->GetFieldAsString(FieldIndex));

        if (!OGRGDALHelpers::convertConnectionsStringToList(CurrentUnit.ChildofConnStr,CurrentUnit.ChildofConn))
        {
          emit stepCompleted(Step,getStyledText(tr("[Error] Wrong field \"%2\" format for \"childof\" connections "
                                                   "in layer \"%1\"")
                                                   .arg(m_SourcesInfos[i].LayerName)
                                                   .arg(m_SourcesInfos[i].ChildofConnectionsField),
                                                "red"));
          return false;
        }
      }


      // === Attributes ===

      for (int j=0;j<m_SourcesInfos[i].ImportedFields.size(); j++)
      {
        FieldIndex = Feature->GetFieldIndex(m_SourcesInfos[i].ImportedFields[j].toStdString().c_str());

        if (FieldIndex <0)
        {
          OGRDataSource::DestroyDataSource(DS);
          emit stepCompleted(Step,getStyledText(tr("[Error] Field for attribute \"%2\" not found in layer \"%1\"")
                                                   .arg(m_SourcesInfos[i].LayerName)
                                                   .arg(m_SourcesInfos[i].ImportedFields[j]),
                                                "red"));
          return false;
        }

        // replacing unwanted chars (space, tab) by underscore
        QString Attr;

        if (!OGRGDALHelpers::convertFieldToAttribute(Feature,FieldIndex,Attr))
        {
          OGRDataSource::DestroyDataSource(DS);
          emit stepCompleted(Step,getStyledText(tr("[Error] Wrong field format for attribute \"%2\" in layer \"%1\"")
                                                   .arg(m_SourcesInfos[i].LayerName)
                                                   .arg(m_SourcesInfos[i].ImportedFields[j]),
                                                "red"));
          return false;
        }

        CurrentUnit.Attributes[m_SourcesInfos[i].ImportedFields[j]] = Attr.replace(" ","_")
                                                                          .replace("\t","_");
      }


      // === Area computed attribute ===

      if (m_SourcesInfos[i].IsAreaCompute)
      {
        if (m_SourcesInfos[i].ImportedFields.contains(m_SourcesInfos[i].AreaComputeAttribute))
        {
          OGRDataSource::DestroyDataSource(DS);
          emit stepCompleted(Step,getStyledText(tr("[Error] Attribute \"%2\" for computed area attribute "
                                                   "is already imported from layer \"%1\"")
                                                   .arg(m_SourcesInfos[i].LayerName)
                                                   .arg(m_SourcesInfos[i].AreaComputeAttribute),
                                                "red"));
          return false;
        }
        else
        {
          double Area = ((OGRPolygon*)(Feature->GetGeometryRef()))->get_Area();
          CurrentUnit.Attributes[m_SourcesInfos[i].AreaComputeAttribute] = QString::number(Area,'g');
        }
      }


      // === Length computed attribute ===

      if (m_SourcesInfos[i].IsLengthCompute)
      {
        if (m_SourcesInfos[i].ImportedFields.contains(m_SourcesInfos[i].LengthComputeAttribute))
        {
          OGRDataSource::DestroyDataSource(DS);
          emit stepCompleted(Step,getStyledText(tr("[Error] Attribute \"%2\" for computed length attribute "
                                                   "is already imported from layer \"%1\"")
                                                   .arg(m_SourcesInfos[i].LayerName)
                                                   .arg(m_SourcesInfos[i].LengthComputeAttribute),
                                                "red"));
          return false;
        }
        else
        {
          double Length = ((OGRLineString*)(Feature->GetGeometryRef()))->get_Length();
          CurrentUnit.Attributes[m_SourcesInfos[i].LengthComputeAttribute] = QString::number(Length,'g');
        }
      }


      // === Preprocess for computed centroid

      OGRPoint Centroid;

      if (Feature->GetGeometryRef()->Centroid(&Centroid) != OGRERR_NONE)
      {
        OGRDataSource::DestroyDataSource(DS);
        emit stepCompleted(Step,
                           getStyledText(tr("[Error] Unable to compute centroid coordinates "
                                            "for geometries in layer \"%1\"."
                                            "Computing centroid coordinates should be unchecked "
                                            "in computed attributes.")
                                         .arg(m_SourcesInfos[i].LayerName),"red"));
        return false;
      }


      // === XCentroid computed attribute ===

      if (m_SourcesInfos[i].IsXCentroidCompute)
      {
        if (m_SourcesInfos[i].ImportedFields.contains(m_SourcesInfos[i].XCentroidComputeAttribute))
        {
          OGRDataSource::DestroyDataSource(DS);
          emit stepCompleted(Step,getStyledText(tr("[Error] Attribute \"%2\" for computed centroid X attribute "
                                                   "is already imported from layer \"%1\"")
                                                   .arg(m_SourcesInfos[i].LayerName)
                                                   .arg(m_SourcesInfos[i].XCentroidComputeAttribute),
                                                "red"));
          return false;
        }
        else
        {
          CurrentUnit.Attributes[m_SourcesInfos[i].XCentroidComputeAttribute] = QString::number(Centroid.getX(),'g');
        }
      }


      // === YCentroid computed attribute ===

      if (m_SourcesInfos[i].IsYCentroidCompute)
      {
        if (m_SourcesInfos[i].ImportedFields.contains(m_SourcesInfos[i].YCentroidComputeAttribute))
        {
          OGRDataSource::DestroyDataSource(DS);
          emit stepCompleted(Step,getStyledText(tr("[Error] Attribute \"%2\" for computed centroid Y attribute "
                                                   "is already imported from layer \"%1\"")
                                                   .arg(m_SourcesInfos[i].LayerName)
                                                   .arg(m_SourcesInfos[i].YCentroidComputeAttribute),
                                                "red"));
          return false;
        }
        else
        {
          CurrentUnit.Attributes[m_SourcesInfos[i].YCentroidComputeAttribute] = QString::number(Centroid.getY(),'g');
        }
      }


      // === ZCentroid computed attribute ===

      if (m_SourcesInfos[i].IsZCentroidCompute)
      {
        if (m_SourcesInfos[i].ImportedFields.contains(m_SourcesInfos[i].ZCentroidComputeAttribute))
        {
          OGRDataSource::DestroyDataSource(DS);
          emit stepCompleted(Step,getStyledText(tr("[Error] Attribute \"%2\" for computed centroid Z attribute "
                                                   "is already imported from layer \"%1\"")
                                                   .arg(m_SourcesInfos[i].LayerName)
                                                   .arg(m_SourcesInfos[i].ZCentroidComputeAttribute),
                                                "red"));
          return false;
        }
        else
        {
          CurrentUnit.Attributes[m_SourcesInfos[i].ZCentroidComputeAttribute] = QString::number(Centroid.getZ(),'g');
        }
      }


      m_SourcesData[i].Units[CurrentUnitID] = CurrentUnit;
    }

    OGRDataSource::DestroyDataSource(DS);
    DS = NULL;
  }

  emit stepCompleted(Step,getStyledText(tr("[OK]"),"green"));
  return true;
}


// =====================================================================
// =====================================================================


bool DataProcessingWorker::checkConnectivity(int Step)
{
  // Check connectivity against both existing and imported units
  emit stepEntered(tr("Checking connectivity consistency..."));

  for (int i=0; i<m_SourcesData.size();i++)
  {
    QMapIterator<int, SourceUnit> It(m_SourcesData[i].Units);
    while (It.hasNext())
    {
      It.next();

      QList<SourceConnection> Conns = It.value().ToConn;

      for (int j=0; j<Conns.size(); j++)
      {
        if (!isUnitExists(Conns[j].DestClass,Conns[j].DestID))
        {
          emit stepCompleted(Step,getStyledText(tr("[Error] Destination unit of \"to\" connection "
                                                   "does not exist for unit %2 of layer \"%1\"")
                                                   .arg(m_SourcesInfos[i].LayerName)
                                                   .arg(It.key()),
                                                "red"));
          return false;
        }
      }


      Conns = It.value().ChildofConn;

      for (int j=0; j<Conns.size(); j++)
      {
        if (!isUnitExists(Conns[j].DestClass,Conns[j].DestID))
        {
          emit stepCompleted(Step,getStyledText(tr("[Error] Parent unit of \"childof\" connection "
                                                   "does not exist for unit %2 of layer \"%1\"")
                                                   .arg(m_SourcesInfos[i].LayerName)
                                                   .arg(It.key()),
                                                "red"));
          return false;
        }
      }
    }
  }

  emit stepCompleted(Step,getStyledText(tr("[OK]"),"green"));
  return true;
}


// =====================================================================
// =====================================================================


bool DataProcessingWorker::runCheck(int StartStep)
{
  // Check unit class does not exist
  emit stepEntered(tr("Checking units classes..."));

  for (int i=0; i<m_SourcesInfos.size();i++)
  {
    if (m_SourcesInfos[i].UnitsClass.isEmpty())
    {
      emit stepCompleted(StartStep,getStyledText(tr("[Error] Class name for layer \"%1\" is empty")
                                                 .arg(m_SourcesInfos[i].LayerName),
                                                 "red"));
      return false;
    }

    if (mp_AdvDesc->spatialDomain().isClassNameExists(m_SourcesInfos[i].UnitsClass.toStdString()))
    {
      emit stepCompleted(StartStep,getStyledText(tr("[Error] Class name for layer \"%1\" already exists")
                                                 .arg(m_SourcesInfos[i].LayerName),
                                                 "red"));
      return false;
    }

    for (int j=0; j<m_SourcesInfos.size();j++)
    {
      if (i!=j && m_SourcesInfos[j].UnitsClass == m_SourcesInfos[i].UnitsClass)
      {
        emit stepCompleted(StartStep,getStyledText(tr("[Error] Units class \"%1\" is imported twice")
                                                   .arg(m_SourcesInfos[i].UnitsClass),
                                                   "red"));
        return false;
      }
    }
  }

  emit stepCompleted(StartStep,getStyledText(tr("[OK]"),"green"));


  emit stepEntered(tr("Checking configuration of files copies and datastore..."));



  for (int i=0; i<m_SourcesInfos.size();i++)
  {
    // check if dataset import file is not empty
    if (m_SourcesInfos[i].IsDatasetImport && m_SourcesInfos[i].RelativeDatasetPath.isEmpty())
    {
      emit stepCompleted(StartStep,
                         getStyledText(tr("[Error] Missing file name for copy of layer \"%1\" in project dataset")
                                       .arg(m_SourcesInfos[i].LayerName),"red"));
      return false;
    }

    QFileInfo DatasetFile(m_SourcesInfos[i].RelativeDatasetPath);
    QString Ext = DatasetFile.suffix();
    std::set<std::string> RegisteredExtsList = openfluid::utils::getOGRFilesExtensionsForOpenFLUID();

    // check if dataset import file contains an extension
    if (m_SourcesInfos[i].IsDatasetImport && Ext.isEmpty())
    {
      emit stepCompleted(StartStep,getStyledText(tr("[Error] Missing file extension for layer \"%1\"")
                                                  .arg(m_SourcesInfos[i].LayerName),
                                                  "red"));
      return false;
    }

    // check if dataset import file extension exists in drivers
    if (m_SourcesInfos[i].IsDatasetImport && RegisteredExtsList.find(Ext.toStdString()) == RegisteredExtsList.end())
    {
      emit stepCompleted(StartStep,getStyledText(tr("[Error] Wrong file extension for layer \"%1\"")
                                                  .arg(m_SourcesInfos[i].LayerName),
                                                  "red"));
      return false;
    }

    // check if datastore ID is not empty
    if (m_SourcesInfos[i].IsDatastore && m_SourcesInfos[i].DatastoreID.isEmpty())
    {
      emit stepCompleted(StartStep,getStyledText(tr("[Error] Missing datastore ID for layer \"%1\"")
                                                 .arg(m_SourcesInfos[i].LayerName),
                                                 "red"));
      return false;
    }

    // check if datastore ID already exists
    if (m_SourcesInfos[i].IsDatastore &&
        mp_AdvDesc->datastore().isItemAlreadyExist(m_SourcesInfos[i].DatastoreID.toStdString()))
    {
      emit stepCompleted(StartStep,getStyledText(tr("[Error] Datastore ID for layer \"%1\" already exists")
                                                       .arg(m_SourcesInfos[i].LayerName),
                                                       "red"));
      return false;
    }

  }

  emit stepCompleted(StartStep+1,getStyledText(tr("[OK]"),"green"));


  // load data from source
  if (!loadDataFromSources(StartStep+2))
    return false;


  if (!checkConnectivity(StartStep+3))
    return false;

  return true;
}
