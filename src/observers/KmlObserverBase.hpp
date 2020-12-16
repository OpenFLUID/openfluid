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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __KMLOBSERVERBASE_HPP__
#define __KMLOBSERVERBASE_HPP__


#include <ogrsf_frmts.h>

#include <QProcess>
#include <QDir>


#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/ware/PluggableObserver.hpp>
#include <openfluid/utils/GDALCompatibility.hpp>


// =====================================================================
// =====================================================================


class KmlUnitInfo
{
  public:

    std::string CoordsStr;

    OGRwkbGeometryType GeometryType;

    openfluid::core::UnitID_t UnitID;

    KmlUnitInfo() : CoordsStr(""), GeometryType(wkbUnknown)
                    {};
};


// =====================================================================
// =====================================================================


template<class T>
class KmlLayerInfo
{
  public:

    openfluid::core::UnitsClass_t UnitsClass;

    bool SourceIsDatastore;
    std::string SourceID;
    std::string SourceFilename;


    std::map<openfluid::core::UnitID_t,T> UnitsInfos;

    int LineWidth;

    KmlLayerInfo() : UnitsClass(""),
                     SourceIsDatastore(false), SourceID(""), SourceFilename (""),
                     LineWidth(1)
      {};

};


// =====================================================================
// =====================================================================


class KmlObserverBase : public openfluid::ware::PluggableObserver
{
  protected:

    std::string m_TmpSubDirRoot;
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

      GDALDataset_COMPAT* DataSource;
      OGRLayer *Layer;
      OGRFeature *Feature;


      DataSource = GDALOpenRO_COMPAT(LayerInfo.SourceFilename.c_str());
      if( DataSource == nullptr )
      {
        OPENFLUID_LogWarning("Cannot open shapefile "+LayerInfo.SourceFilename+". This Kml output is ignored.");
        return false;
      }

      std::string LayerName = openfluid::tools::Filesystem::basename(LayerInfo.SourceFilename);

      Layer = DataSource->GetLayerByName(LayerName.c_str());

      if (Layer == nullptr)
      {
        OPENFLUID_LogWarning("Cannot open shapefile layer from " + LayerInfo.SourceFilename +
                               ". This Kml output is ignored.");
        return false;
      }


      int OfldIDFieldIndex = Layer->GetLayerDefn()->GetFieldIndex("OFLD_ID");

      if (OfldIDFieldIndex < 0)
      {
        OPENFLUID_LogWarning("Cannot find OFLD_ID attribute in " + LayerInfo.SourceFilename +
                               ". This Kml output is ignored.");
        return false;
      }


      Layer->ResetReading();
      while((Feature = Layer->GetNextFeature()) != nullptr)
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

          if (Geometry != nullptr)
          {

            if (Geometry->getGeometryType() != wkbPolygon && Geometry->getGeometryType() != wkbLineString)
            {
              OPENFLUID_LogWarning("Unsupported geometry type in " + LayerInfo.SourceFilename +
                                     ". This Kml output is ignored.");
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
            OPENFLUID_LogWarning("Wrong geometry reference in " + LayerInfo.SourceFilename +
                                   ". This Kml output is ignored.");
            return false;
          }

        }
        OGRFeature::DestroyFeature(Feature);
      }


      GDALClose_COMPAT(DataSource);


      return true;
    }


    // =====================================================================
    // =====================================================================


    void buildKmzFile()
    {
      std::string InputDir = m_TmpDir+"/"+m_KmzSubDir+"/";
      std::string KmzFilePath = m_OutputDir + "/"+ m_OutputFileName;

      openfluid::tools::Filesystem::removeDirectory(KmzFilePath);

      openfluid::utils::ExternalProgram SevenZProgram =
          openfluid::utils::ExternalProgram::getRegisteredProgram(openfluid::utils::ExternalProgram::SevenZipProgram);

      if (SevenZProgram.isFound())
      {
        QStringList Args;
        Args << "a";
        Args << "-tzip";
        Args << QString::fromStdString(KmzFilePath);
        Args << QString::fromStdString(InputDir)+"/*";

        QProcess::execute(SevenZProgram.getFullProgramPath(),Args);
      }
    }


    // =====================================================================
    // =====================================================================


    void tryOpenGEarth()
    {
      if (m_TryOpenGEarth)
      {
        openfluid::utils::ExternalProgram GEarthProgram =
            openfluid::utils::ExternalProgram::getRegisteredProgram(
                openfluid::utils::ExternalProgram::GoogleEarthProgram);

        if (GEarthProgram.isFound())
        {
          QProcess::execute(GEarthProgram.getFullProgramPath(),
                            {  QDir(QString::fromStdString(m_OutputDir))
                               .absoluteFilePath(QString::fromStdString(m_OutputFileName))
                            });
        }
        else
        {
          OPENFLUID_LogWarning("Cannot find Google Earth");
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void prepareTempDirectory()
    {

      std::string TmpDir;

      OPENFLUID_GetRunEnvironment("dir.temp",TmpDir);

      m_TmpDir = openfluid::tools::Filesystem::makeUniqueSubdirectory(TmpDir,m_TmpSubDirRoot);

      openfluid::tools::Filesystem::makeDirectory(m_TmpDir);

      if (!openfluid::tools::Filesystem::isDirectory(m_TmpDir))
      {
        OPENFLUID_LogWarning("Cannot initialize temporary directory");
        m_OKToGo = false;
        return;
      }

      openfluid::tools::Filesystem::removeDirectory(m_TmpDir+"/"+m_KmzSubDir);
      openfluid::tools::Filesystem::makeDirectory(m_TmpDir+"/"+m_KmzSubDir);

      if (!openfluid::tools::Filesystem::isDirectory(m_TmpDir+"/"+m_KmzSubDir))
      {
        OPENFLUID_LogWarning("Cannot initialize kmz temporary directory");
        m_OKToGo = false;
        return;
      }


      openfluid::tools::Filesystem::makeDirectory(m_TmpDir+"/"+m_KmzSubDir+"/"+m_KmzDataSubDir);

      if (!openfluid::tools::Filesystem::isDirectory(m_TmpDir+"/"+m_KmzSubDir+"/"+m_KmzDataSubDir))
      {
        OPENFLUID_LogWarning("Cannot initialize kmz data temporary directory");
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

