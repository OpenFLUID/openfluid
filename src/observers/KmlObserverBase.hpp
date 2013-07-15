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
  \file KmlObserverBase.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __KMLOBSERVERBASE_HPP__
#define __KMLOBSERVERBASE_HPP__

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <ogrsf_frmts.h>

#include <glibmm/fileutils.h>
#include <glibmm/spawn.h>

#include <ogrsf_frmts.h>

#include <openfluid/ware/PluggableObserver.hpp>


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

    std::string m_ZipCommand;

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

      std::string LayerName = boost::filesystem::path(LayerInfo.SourceFilename).stem().string();

      Layer = DataSource->GetLayerByName(LayerName.c_str());

      if (Layer == NULL)
      {
        OPENFLUID_RaiseWarning("KmlObserverBase::transformVectorLayerToKmlGeometry()",
            "Cannot open shapefile layer from "+LayerInfo.SourceFilename+". This Kml output is ignored.");
        return false;
      }


      int SelfIDFieldIndex = Layer->GetLayerDefn()->GetFieldIndex("SELF_ID");

      if (SelfIDFieldIndex < 0)
      {
        OPENFLUID_RaiseWarning("KmlObserverBase::transformVectorLayerToKmlGeometry()",
            "Cannot find SELF_ID attribute in "+LayerInfo.SourceFilename+". This Kml output is ignored.");
        return false;
      }


      Layer->ResetReading();
      while((Feature = Layer->GetNextFeature()) != NULL)
      {
        openfluid::core::UnitID_t UnitID = Feature->GetFieldAsInteger(SelfIDFieldIndex);

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

      std::string KmzFilePath = boost::filesystem::path(m_OutputDir + "/"+ m_OutputFileName).string();

      boost::filesystem::remove_all(boost::filesystem::path(KmzFilePath));

      std::string FullZipCommand = "\"" + m_ZipCommand +"\" a -tzip \"" + KmzFilePath +"\"  \"" + boost::filesystem::path(m_TmpDir+"/"+m_KmzSubDir+"/").string() + "*\"";

      std::string StrOut = "";
      std::string StrErr = "";
      int RetValue = 0;
      Glib::spawn_command_line_sync(FullZipCommand,&StrOut,&StrErr,&RetValue);
    }


    // =====================================================================
    // =====================================================================


    void tryOpenGEarth()
    {
      if (m_TryOpenGEarth)
      {
        // search for Google Earth (google-earth)
        std::string GEarthProgram = "";

#if defined __unix__ || defined __APPLE__
        GEarthProgram = "google-earth";
#endif

#if WIN32
        // TODO check for win32
        GEarthProgram = "google-earth";
#endif


        std::vector<std::string> GEarthPaths = openfluid::tools::GetFileLocationsUsingPATHEnvVar(GEarthProgram);

        if (!GEarthPaths.empty())
        {
          std::string GEarthCommand = boost::filesystem::path(GEarthPaths[0]).string()+" "+boost::filesystem::path(m_OutputDir + "/"+ m_OutputFileName).string();
          Glib::spawn_command_line_async(GEarthCommand);
        }
        else
        {
          OPENFLUID_RaiseWarning("KmlObserverBase::tryOpenGEarth()",
                                 "Cannot find Google Earth");
          return;
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void searchForZipTool()
    {
      // search for zip tool (7zip)
      std::string ZipProgram = "";

      #if defined __unix__ || defined __APPLE__
        ZipProgram = "7z";
      #endif

      #if WIN32
        ZipProgram = "7";
      #endif

      if (ZipProgram.empty())
      {
        OPENFLUID_RaiseWarning("KmlObserverBase::searchForZipTool()",
                    "Unsupported system platform");
        m_OKToGo = false;
        return;
      }

      std::vector<std::string> ZipPaths = openfluid::tools::GetFileLocationsUsingPATHEnvVar(ZipProgram);

      if (!ZipPaths.empty())
      {
        m_ZipCommand = boost::filesystem::path(ZipPaths[0]).string();
      }
      else
      {
        OPENFLUID_RaiseWarning("KmlObserverBase::searchForZipTool()",
            "Required Zip program ("+ ZipProgram +") not found");
        m_OKToGo = false;
        return;
      }
    }


    // =====================================================================
    // =====================================================================


    void prepareTempDirectory()
    {

      std::string TmpDir;

      OPENFLUID_GetRunEnvironment("dir.temp",TmpDir);

      m_TmpDir = boost::filesystem::path(TmpDir+"/"+m_TmpSubDir).string();

      boost::filesystem::create_directories(boost::filesystem::path(m_TmpDir));

      if (!boost::filesystem::is_directory(boost::filesystem::path(m_TmpDir)))
      {
        OPENFLUID_RaiseWarning("KmlObserverBase::prepareTempDirectory()",
                  "Cannot initialize temporary directory");
        m_OKToGo = false;
        return;
      }

      boost::filesystem::remove_all(boost::filesystem::path(m_TmpDir+"/"+m_KmzSubDir));
      boost::filesystem::create_directories(boost::filesystem::path(m_TmpDir+"/"+m_KmzSubDir));

      if (!boost::filesystem::is_directory(boost::filesystem::path(m_TmpDir+"/"+m_KmzSubDir)))
      {
        OPENFLUID_RaiseWarning("KmlObserverBase::prepareTempDirectory()",
                  "Cannot initialize kmz temporary directory");
        m_OKToGo = false;
        return;
      }


      boost::filesystem::create_directories(boost::filesystem::path(m_TmpDir+"/"+m_KmzSubDir+"/"+m_KmzDataSubDir));

      if (!boost::filesystem::is_directory(boost::filesystem::path(m_TmpDir+"/"+m_KmzSubDir+"/"+m_KmzDataSubDir)))
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
      m_ZipCommand(""),
      m_TryOpenGEarth(false),
      m_OKToGo(false)
    {

    }


};


#endif /* __KMLOBSERVERBASE_HPP__ */
