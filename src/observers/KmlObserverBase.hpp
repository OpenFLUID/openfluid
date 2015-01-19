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
  @file KmlObserverBase.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __KMLOBSERVERBASE_HPP__
#define __KMLOBSERVERBASE_HPP__


#include <ogrsf_frmts.h>
#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/utils/ExternalProgram.hpp>

#include <openfluid/ware/PluggableObserver.hpp>
#include <QProcess>
#include <QDir>


class KmlUnitInfo
{
  public:

    std::string CoordsStr;

    OGRwkbGeometryType GeometryType;

    openfluid::core::UnitID_t UnitID;

    KmlUnitInfo() : CoordsStr(""), GeometryType(wkbUnknown)
                    {};
};

template<class T>
class KmlLayerInfo
{
  public:

    std::string GroupName;

    openfluid::core::UnitClass_t UnitsClass;

    bool SourceIsDatastore;
    std::string SourceID;
    std::string SourceFilename;


    std::map<openfluid::core::UnitID_t,T> UnitsInfos;

    int LineWidth;

    KmlLayerInfo() : GroupName(""), UnitsClass(""),
                     SourceIsDatastore(false), SourceID(""), SourceFilename (""),
                     LineWidth(1)
      {};

};


class KmlObserverBase : public openfluid::ware::PluggableObserver
{
  protected:

    std::string m_TmpSubDir;
    std::string m_KmzSubDir;
    const std::string m_KmzDataSubDir;

    std::string m_Title;
    std::string m_OutputFileName;
    std::string m_InputDir;
    std::string m_OutputDir;
    std::string m_TmpDir;

    bool m_TryOpenGEarth;

    bool m_OKToGo;

    template<class T>
    bool transformVectorLayerToKmlGeometry(KmlLayerInfo<T>& LayerInfo)
    {

      // TODO manage when data are coming from datastore

      OGRDataSource *DataSource;
      OGRLayer *Layer;
      OGRFeature *Feature;


      DataSource = OGRSFDriverRegistrar::Open(LayerInfo.SourceFilename.c_str(), FALSE );
      if( DataSource == NULL )
      {
        OPENFLUID_RaiseWarning("KmlObserverBase::transformVectorLayerToKmlGeometry()",
            "Cannot open shapefile "+LayerInfo.SourceFilename+". This Kml output is ignored.");
        return false;
      }

      //std::string LayerName = boost::filesystem::path(LayerInfo.SourceFilename).stem().string();
      std::string LayerName = QFileInfo(QString::fromStdString(LayerInfo.SourceFilename)).baseName().toStdString();

      Layer = DataSource->GetLayerByName(LayerName.c_str());

      if (Layer == NULL)
      {
        OPENFLUID_RaiseWarning("KmlObserverBase::transformVectorLayerToKmlGeometry()",
            "Cannot open shapefile layer from "+LayerInfo.SourceFilename+". This Kml output is ignored.");
        return false;
      }


      int OfldIDFieldIndex = Layer->GetLayerDefn()->GetFieldIndex("OFLD_ID");

      if (OfldIDFieldIndex < 0)
      {
        OPENFLUID_RaiseWarning("KmlObserverBase::transformVectorLayerToKmlGeometry()",
            "Cannot find OFLD_ID attribute in "+LayerInfo.SourceFilename+". This Kml output is ignored.");
        return false;
      }


      Layer->ResetReading();
      while((Feature = Layer->GetNextFeature()) != NULL)
      {
        openfluid::core::UnitID_t UnitID = Feature->GetFieldAsInteger(OfldIDFieldIndex);

        if (OPENFLUID_IsUnitExist(LayerInfo.UnitsClass,UnitID))
        {

          OGRGeometry *Geometry;

          Geometry = Feature->GetGeometryRef();

          std::stringstream CoordSS;

          CoordSS << std::fixed << std::setprecision(12);

          T KUI;

          KUI.UnitID = UnitID;

          if (Geometry != NULL)
          {

            if (Geometry->getGeometryType() != wkbPolygon && Geometry->getGeometryType() != wkbLineString)
            {
              OPENFLUID_RaiseWarning("KmlObserverBase::transformVectorLayerToKmlGeometry()",
                  "Unsupported geometry type in "+LayerInfo.SourceFilename+". This Kml output is ignored.");
              return false;
            }

            KUI.GeometryType = Geometry->getGeometryType();

            // polygons
            if (Geometry->getGeometryType() == wkbPolygon)
            {
              OGRLinearRing* Ring;

              Ring = ((OGRPolygon*)(Geometry))->getExteriorRing();

              int NumPoints = Ring->getNumPoints()-1;

              for (int i=0;i<NumPoints;i++)
              {
                OGRPoint *Point = new OGRPoint();
                Ring->getPoint(i,Point);
                CoordSS << " " << Point->getX() << "," << Point->getY();
                delete Point;
              }

              // close the polygon
              if (NumPoints > 0)
              {
                OGRPoint *Point = new OGRPoint();
                Ring->getPoint(0,Point);
                CoordSS << " " << Point->getX() << "," << Point->getY();
                delete Point;
              }

            }


            // line strings
            if (Geometry->getGeometryType() == wkbLineString)
            {

              OGRLineString* LineString;
              LineString = (OGRLineString*)(Geometry);

              int NumPoints = LineString->getNumPoints();

              for (int i=0;i<NumPoints;i++)
              {
                OGRPoint *Point = new OGRPoint();
                LineString->getPoint(i,Point);
                CoordSS << " " << Point->getX() << "," << Point->getY();
                delete Point;
              }
            }

            KUI.CoordsStr = CoordSS.str();

            LayerInfo.UnitsInfos[UnitID] = KUI;

          }
          else
          {
            OPENFLUID_RaiseWarning("KmlObserverBase::transformVectorLayerToKmlGeometry()",
                "Wrong geometry reference in "+LayerInfo.SourceFilename+". This Kml output is ignored.");
            return false;
          }

        }
        OGRFeature::DestroyFeature(Feature);
      }


      OGRDataSource::DestroyDataSource(DataSource);


      return true;
    }


    // =====================================================================
    // =====================================================================


    void buildKmzFile()
    {
      std::string InputDir = boost::filesystem::path(m_TmpDir+"/"+m_KmzSubDir+"/").string();
      std::string KmzFilePath = boost::filesystem::path(m_OutputDir + "/"+ m_OutputFileName).string();

      openfluid::tools::removeDirectoryRecursively(QString::fromStdString(KmzFilePath));

      openfluid::utils::ExternalProgram SevenZProgram =
          openfluid::utils::ExternalProgram::getRegisteredProgram(openfluid::utils::ExternalProgram::SevenZipProgram);

      if (SevenZProgram.isFound())
      {
        QString SevenZCommand = QString("\"%1\" a -tzip \"%2\" \"%3\"*").arg(SevenZProgram.getFullProgramPath())
                                                                        .arg(QString::fromStdString(KmzFilePath))
                                                                        .arg(QString::fromStdString(InputDir));

        QProcess::execute(SevenZCommand);
      }
    }


    // =====================================================================
    // =====================================================================


    void tryOpenGEarth()
    {
      if (m_TryOpenGEarth)
      {
        openfluid::utils::ExternalProgram GEarthProgram =
            openfluid::utils::ExternalProgram::getRegisteredProgram(openfluid::utils::ExternalProgram::GoogleEarthProgram);

        if (GEarthProgram.isFound())
        {
          QString GEarthCommand = QString("%1 %2").arg(GEarthProgram.getFullProgramPath())
                                                  .arg(QDir(QString::fromStdString(m_OutputDir))
                                                       .absoluteFilePath(QString::fromStdString(m_OutputFileName)));
          QProcess::execute(GEarthCommand);
        }
        else
        {
          OPENFLUID_RaiseWarning("KmlObserverBase::tryOpenGEarth()",
                                 "Cannot find Google Earth");
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void prepareTempDirectory()
    {

      std::string TmpDir;

      OPENFLUID_GetRunEnvironment("dir.temp",TmpDir);

      m_TmpDir = boost::filesystem::path(TmpDir+"/"+m_TmpSubDir).string();

      QDir().mkpath(QString::fromStdString(m_TmpDir));

      if (!QFileInfo(QString::fromStdString(m_TmpDir)).isDir())
      {
        OPENFLUID_RaiseWarning("KmlObserverBase::prepareTempDirectory()",
                  "Cannot initialize temporary directory");
        m_OKToGo = false;
        return;
      }

      openfluid::tools::removeDirectoryRecursively(QString::fromStdString(m_TmpDir+"/"+m_KmzSubDir));
      QDir().mkpath(QString::fromStdString(m_TmpDir+"/"+m_KmzSubDir));

      if (!QFileInfo(QString::fromStdString(m_TmpDir+"/"+m_KmzSubDir)).isDir())
      {
        OPENFLUID_RaiseWarning("KmlObserverBase::prepareTempDirectory()",
                  "Cannot initialize kmz temporary directory");
        m_OKToGo = false;
        return;
      }


      QDir().mkpath(QString::fromStdString(m_TmpDir+"/"+m_KmzSubDir+"/"+m_KmzDataSubDir));

      if (!QFileInfo(QString::fromStdString(m_TmpDir+"/"+m_KmzSubDir+"/"+m_KmzDataSubDir)).isDir())
      {
        OPENFLUID_RaiseWarning("KmlObserverBase::prepareTempDirectory()",
                  "Cannot initialize kmz data temporary directory");
        m_OKToGo = false;
        return;
      }
    }


  public:

    KmlObserverBase() : openfluid::ware::PluggableObserver(),
      m_KmzSubDir("kmz"),m_KmzDataSubDir("data"),
      m_Title("OpenFLUID simulation with time animation"),
      m_OutputFileName(""),
      m_InputDir(""), m_OutputDir(""), m_TmpDir(""),
      m_TryOpenGEarth(false),
      m_OKToGo(false)
    {

    }


};


#endif /* __KMLOBSERVERBASE_HPP__ */
