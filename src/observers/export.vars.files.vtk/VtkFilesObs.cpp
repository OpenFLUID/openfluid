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
  \file VtkFilesObs.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include <ogrsf_frmts.h>





/* If SINGLE is defined when triangle.o is compiled, it should also be       */
/*   defined here.  If not, it should not be defined here.                   */

/* #define SINGLE */

#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */

#define ANSI_DECLARATORS

extern "C"
{
#include "triangle.h"
}


#include "RasterMan.hpp"



#include <openfluid/ware/PluggableObserver.hpp>


// =====================================================================
// =====================================================================


DECLARE_OBSERVER_PLUGIN


// =====================================================================
// =====================================================================


BEGIN_OBSERVER_SIGNATURE("export.vars.files.vtk")

  DECLARE_NAME("Exports simulation variables to time indexed vtk files for animation");
  // TODO complete description with parameters usage
  DECLARE_DESCRIPTION("");
  DECLARE_VERSION(openfluid::config::FULL_VERSION);
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class VtkSerieInfo
{
  public:

    openfluid::core::UnitClass_t UnitsClass;

    std::string VectorSourceFilename;

    std::string DEMSourceFilename;

    openfluid::core::VariableName_t VarName;

    std::string OutputFileBaseName;

    std::string OutputDir;

    std::string CellsNbrStr;

    std::string PointsNbrStr;

    std::string PointsStr;

    std::string CellsCntyStr;

    std::string CellsTypesStr;

    std::string CellsOffsetsStr;


    std::list< std::pair<openfluid::core::UnitID_t,unsigned int> > OrderedListOfUnits;

    openfluid::core::DoubleValue MaxValue;

    openfluid::core::DoubleValue MinValue;

    bool IsMinMaxInitialized;


    VtkSerieInfo() : UnitsClass(""),
                     VectorSourceFilename (""), DEMSourceFilename (""), VarName(""),
                     OutputFileBaseName(""), OutputDir(""),
                     CellsNbrStr("0"), PointsNbrStr("0"),
                     PointsStr(""), CellsCntyStr(""), CellsTypesStr(""), CellsOffsetsStr(""),
                     MaxValue(0.0),MinValue(0.0),IsMinMaxInitialized(false)
                     {};
};


// =====================================================================
// =====================================================================


class VtkFilesObserver : public openfluid::ware::PluggableObserver
{
  private:

    std::list<VtkSerieInfo> m_VtkSeriesInfos;

    std::string m_DEMFilename;

    SimpleRasterManager m_RasterMan;

    void writeVtkFile(VtkSerieInfo& VtkSerie, const openfluid::core::TimeIndex_t& Index)
    {
      std::ostringstream oss;
        std::string CellDataStr;
        openfluid::core::DoubleValue Value;
        openfluid::core::Unit* UU;

        // preparing current data
        std::list< std::pair<openfluid::core::UnitID_t,unsigned int> >::const_iterator UnitsIt;

        for (UnitsIt = VtkSerie.OrderedListOfUnits.begin(); UnitsIt != VtkSerie.OrderedListOfUnits.end(); ++UnitsIt)
        {
          UU = OPENFLUID_GetUnit(VtkSerie.UnitsClass,(*UnitsIt).first);

          if (UU != NULL)
          {
            OPENFLUID_GetVariable(UU,VtkSerie.VarName,Index,Value);
          }
          else
          {
            Value = 0.0;
          }

          for (unsigned int i=0;i<(*UnitsIt).second;i++)
            oss << Value << "\n";

          if (!VtkSerie.IsMinMaxInitialized)
          {
            VtkSerie.MaxValue = Value;
            VtkSerie.MinValue = Value;
            VtkSerie.IsMinMaxInitialized = true;
          }
          else
          {
            VtkSerie.MinValue = std::min(VtkSerie.MinValue,Value);
            VtkSerie.MaxValue = std::max(VtkSerie.MaxValue,Value);
          }
        }
        CellDataStr = oss.str();




        // writing file
        std::string TimeIndexStr;
        openfluid::tools::ConvertValue(Index,&TimeIndexStr);

        std::string FilenameStr = boost::filesystem::path(VtkSerie.OutputDir+"/"+VtkSerie.OutputFileBaseName+TimeIndexStr+".vtu").string();

        std::ofstream VtkFile(FilenameStr.c_str());


        VtkFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
        VtkFile << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"BigEndian\">\n";
        VtkFile << "  <UnstructuredGrid>\n";
        VtkFile << "    <Piece NumberOfPoints=\"" << VtkSerie.PointsNbrStr << "\" NumberOfCells=\"" << VtkSerie.CellsNbrStr << "\">\n";
        VtkFile << "      <CellData Scalars=\"scalars\">\n";
        VtkFile << "        <DataArray type=\"Float64\" Name=\"" << VtkSerie.VarName << "\" format=\"ascii\">\n";
        VtkFile << CellDataStr << "\n";
        VtkFile << "        </DataArray>\n";
        VtkFile << "      </CellData>\n";
        VtkFile << "      <Points>\n";
        VtkFile << "        <DataArray type=\"Float32\" NumberOfComponents=\"3\" Format=\"ascii\">\n";
        VtkFile << VtkSerie.PointsStr << "\n";
        VtkFile << "        </DataArray>\n";
        VtkFile << "      </Points>\n";
        VtkFile << "      <Cells>\n";
        VtkFile << "        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">\n";
        VtkFile << VtkSerie.CellsCntyStr << "\n";
        VtkFile << "        </DataArray>\n";
        VtkFile << "        <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">\n";
        VtkFile << VtkSerie.CellsOffsetsStr << "\n";
        VtkFile << "        </DataArray>\n";
        VtkFile << "        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n";
        VtkFile << VtkSerie.CellsTypesStr << "\n";
        VtkFile << "        </DataArray>\n";
        VtkFile << "      </Cells>\n";
        VtkFile << "    </Piece>\n";
        VtkFile << "  </UnstructuredGrid>\n";
        VtkFile << "</VTKFile>\n";
        VtkFile.close();
    }


    // =====================================================================
    // =====================================================================


    bool prepareVtkFile(VtkSerieInfo& VtkSerie)
    {
      OGRRegisterAll();

      OGRDataSource *DataSource;
      OGRLayer *Layer;
      OGRFeature *Feature;

      struct triangulateio TriIn, TriMid, TriOut, VorOut;
      SimpleRasterManager LocalRasterMan;

      std::ostringstream ossCellsCnty, ossPoints, ossCellsTypes, ossCellsOffsets;
      ossPoints << std::fixed << std::setprecision(5);



      // open local DEM for this serie if exists
      if (!VtkSerie.DEMSourceFilename.empty())
        LocalRasterMan.open(VtkSerie.DEMSourceFilename);


      // open and check shapefile
      DataSource = OGRSFDriverRegistrar::Open(VtkSerie.VectorSourceFilename.c_str(), FALSE );
      if( DataSource == NULL )
      {
        OPENFLUID_RaiseWarning("ExportVtkFunction::prepareVtkFile",
            "Cannot open shapefile "+VtkSerie.VectorSourceFilename+". This Vtk output is ignored.");
        return false;
      }

      std::string LayerName = boost::filesystem::path(VtkSerie.VectorSourceFilename).stem().string();

      Layer = DataSource->GetLayerByName(LayerName.c_str());

      if (Layer == NULL)
      {
        OPENFLUID_RaiseWarning("ExportVtkFunction::prepareVtkFile",
            "Cannot open shapefile layer from "+VtkSerie.VectorSourceFilename+". This Vtk output is ignored.");
        return false;
      }


      int SelfIDFieldIndex = Layer->GetLayerDefn()->GetFieldIndex("SELF_ID");

      if (SelfIDFieldIndex < 0)
      {
        OPENFLUID_RaiseWarning("ExportVtkFunction::prepareVtkFile",
            "Cannot find SELF_ID attribute in "+VtkSerie.VectorSourceFilename+". This Vtk output is ignored.");
        return false;
      }




      unsigned int PointsIndex = 0;
      unsigned int CellsIndex = 0;


      // parse shapefile

      Layer->ResetReading();
      while((Feature = Layer->GetNextFeature()) != NULL)
      {
        openfluid::core::UnitID_t UnitID = Feature->GetFieldAsInteger(SelfIDFieldIndex);

        OGRGeometry *Geometry;

        Geometry = Feature->GetGeometryRef();

        std::string CellTypeStr = "";

        if (Geometry != NULL)
        {

          if (Geometry->getGeometryType() != wkbPolygon && Geometry->getGeometryType() != wkbLineString)
          {
            OPENFLUID_RaiseWarning("ExportVtkFunction::prepareVtkFile()",
                  "Unsupported geometry type in "+VtkSerie.VectorSourceFilename+". This Vtk output is ignored.");
            return false;
          }

          // polygons
          if (Geometry->getGeometryType() == wkbPolygon)
          {
            OGRLinearRing* Ring;

            Ring = ((OGRPolygon*)(Geometry))->getExteriorRing();

            int NumPoints = Ring->getNumPoints()-1;

            TriIn.numberofpoints = NumPoints;
            TriIn.pointlist = (REAL *) malloc(TriIn.numberofpoints * 2 * sizeof(REAL));
            TriIn.numberofpointattributes = 0;
            TriIn.pointattributelist = (REAL *) NULL;
            TriIn.pointmarkerlist = (int *) malloc(TriIn.numberofpoints * sizeof(int));
            TriIn.numberoftriangles = 0;
            TriIn.trianglelist = (int *) NULL;
            TriIn.triangleattributelist = (REAL *) NULL;
            TriIn.neighborlist = (int *) NULL;
            TriIn.numberofsegments = NumPoints;
            TriIn.segmentlist = (int *) malloc(TriIn.numberofsegments * 2 * sizeof(int));
            TriIn.segmentmarkerlist = (int *) malloc(TriIn.numberofsegments * sizeof(int));
            TriIn.numberofedges = 0;
            TriIn.edgelist = (int *) NULL;
            TriIn.edgemarkerlist = (int *) NULL;
            TriIn.numberofholes = 0;
            TriIn.numberofregions = 0;

            TriMid.pointlist = (REAL *) NULL;
            TriMid.pointattributelist = (REAL *) NULL;
            TriMid.pointmarkerlist = (int *) NULL;
            TriMid.trianglelist = (int *) NULL;
            TriMid.triangleattributelist = (REAL *) NULL;
            TriMid.neighborlist = (int *) NULL;
            TriMid.segmentlist = (int *) NULL;
            TriMid.segmentmarkerlist = (int *) NULL;
            TriMid.edgelist = (int *) NULL;
            TriMid.edgemarkerlist = (int *) NULL;

            TriOut.pointlist = (REAL *) NULL;
            TriOut.pointattributelist = (REAL *) NULL;
            TriOut.pointmarkerlist = (int *) NULL;
            TriOut.trianglelist = (int *) NULL;
            TriOut.triangleattributelist = (REAL *) NULL;
            TriOut.neighborlist = (int *) NULL;
            TriOut.segmentlist = (int *) NULL;
            TriOut.segmentmarkerlist = (int *) NULL;
            TriOut.edgelist = (int *) NULL;
            TriOut.edgemarkerlist = (int *) NULL;


            for (int i=0;i<NumPoints;i++)
            {
               OGRPoint *Point = new OGRPoint();
               Ring->getPoint(i,Point);
               TriIn.pointlist[i*2] = Point->getX();
               TriIn.pointlist[(i*2)+1] = Point->getY();
               TriIn.pointmarkerlist[i] = 1;
               if (i != (NumPoints-1))
               {
                 TriIn.segmentlist[i*2] = i;
                 TriIn.segmentlist[(i*2)+1] = i+1;
               }
               else
               {
                 TriIn.segmentlist[i*2] = i;
                 TriIn.segmentlist[(i*2)+1] = 0;
               }
               TriIn.segmentmarkerlist[i] = 1;

               delete Point;
            }

            // triangulation
            triangulate((char*)"pzQ", &TriIn, &TriMid,&VorOut);
            // triangulation refinement
            triangulate((char*)"prq30zQ", &TriMid, &TriOut,&VorOut);



            unsigned int BasePointIndex = PointsIndex;

            double ZValue = 0.0;


            for (int i=0;i<TriOut.numberofpoints;i++)
            {
              if (LocalRasterMan.isOpened())
              {
                if (!LocalRasterMan.getRasterValue(TriOut.pointlist[i*2],TriOut.pointlist[(i*2)+1],ZValue))
                  ZValue = 0.0;
              }
              else
              {
                if (m_RasterMan.isOpened())
                {
                  if (!m_RasterMan.getRasterValue(TriOut.pointlist[i*2],TriOut.pointlist[(i*2)+1],ZValue))
                    ZValue = 0.0;
                }
              }
              ossPoints << TriOut.pointlist[i*2] << " " << TriOut.pointlist[(i*2)+1] << " " << ZValue << "\n";
              PointsIndex++;
            }

            CellTypeStr = "";

            VtkSerie.OrderedListOfUnits.push_back(std::make_pair(UnitID,TriOut.numberoftriangles));

            for (int i=0; i<TriOut.numberoftriangles;i++)
            {
              CellTypeStr += "7\n";
              ossCellsCnty << BasePointIndex + TriOut.trianglelist[i*3] << " ";
              ossCellsCnty << BasePointIndex + TriOut.trianglelist[(i*3)+1] << " ";
              ossCellsCnty << BasePointIndex + TriOut.trianglelist[(i*3)+2] << " ";
              ossCellsCnty << BasePointIndex + TriOut.trianglelist[i*3] << "\n";
              CellsIndex++;
              ossCellsOffsets << (CellsIndex * 4) << "\n";
            }

            free(TriIn.pointlist);
            free(TriIn.segmentlist);
            free(TriIn.segmentmarkerlist);
            free(TriIn.pointmarkerlist);

            free(TriMid.pointlist);
            free(TriMid.pointattributelist);
            free(TriMid.pointmarkerlist);
            free(TriMid.trianglelist);
            free(TriMid.triangleattributelist);
            free(TriMid.neighborlist);
            free(TriMid.segmentlist);
            free(TriMid.segmentmarkerlist);
            free(TriMid.edgelist);
            free(TriMid.edgemarkerlist);

            free(TriOut.pointlist);
            free(TriOut.pointattributelist);
            free(TriOut.pointmarkerlist);
            free(TriOut.trianglelist);
            free(TriOut.triangleattributelist);
            free(TriOut.neighborlist);
            free(TriOut.segmentlist);
            free(TriOut.segmentmarkerlist);
            free(TriOut.edgelist);
            free(TriOut.edgemarkerlist);
          }


          // line strings
          if (Geometry->getGeometryType() == wkbLineString)
          {
            CellTypeStr = "4";
            VtkSerie.OrderedListOfUnits.push_back(std::make_pair(UnitID,1));

            OGRLineString* LineString;
            LineString = (OGRLineString*)(Geometry);

            int NumPoints = LineString->getNumPoints();

            for (int i=0;i<NumPoints;i++)
            {
               OGRPoint *Point = new OGRPoint();
               LineString->getPoint(i,Point);

               double ZValue = 0.0;

               if (m_RasterMan.isOpened())
               {
                 if (!m_RasterMan.getRasterValue(Point->getX(),Point->getY(),ZValue))
                   ZValue = 0.0;
               }


               ossPoints << Point->getX() << " " << Point->getY() << " " << ZValue << "\n";
               delete Point;

               ossCellsCnty << PointsIndex << " ";
               PointsIndex++;
            }
            ossCellsOffsets << PointsIndex << "\n";
            ossCellsCnty << "\n";
            CellsIndex++;
          }


        }
        else
        {
          OPENFLUID_RaiseWarning("ExportVtkFunction::prepareVtkFile()",
                "Wrong geometry reference in "+VtkSerie.VectorSourceFilename+". This Vtk output is ignored.");
          return false;
        }

        ossCellsTypes << CellTypeStr << "\n";
        OGRFeature::DestroyFeature(Feature);
      }


      openfluid::tools::ConvertValue(PointsIndex,&VtkSerie.PointsNbrStr);
      openfluid::tools::ConvertValue(CellsIndex,&VtkSerie.CellsNbrStr);

      VtkSerie.PointsStr = ossPoints.str();
      VtkSerie.CellsCntyStr = ossCellsCnty.str();
      VtkSerie.CellsTypesStr = ossCellsTypes.str();
      VtkSerie.CellsOffsetsStr = ossCellsOffsets.str();


      OGRDataSource::DestroyDataSource(DataSource);

      return true;
    }


    // =====================================================================
    // =====================================================================


    void writeSerieReport(const VtkSerieInfo& VtkSerie)
    {
      std::string FilenameStr = boost::filesystem::path(VtkSerie.OutputDir+"/report.txt").string();

      std::ofstream ReportFile(FilenameStr.c_str());

      ReportFile << "Unit class : " << VtkSerie.UnitsClass << "\n";
      ReportFile << "Variable : " << VtkSerie.VarName << "\n";
      ReportFile << "Minimum value : " << VtkSerie.MinValue << "\n";
      ReportFile << "Maximum value : " << VtkSerie.MaxValue << "\n";

      ReportFile.close();
    }



  public:

    VtkFilesObserver() : PluggableObserver()
    {

    }


    // =====================================================================
    // =====================================================================


    ~VtkFilesObserver()
    {

    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      boost::property_tree::ptree ParamsPT;

      try
      {
        ParamsPT = openfluid::ware::PluggableWare::getParamsAsPropertyTree(Params);
      }
      catch (openfluid::base::FrameworkException& E)
      {
        OPENFLUID_RaiseError(E.getMessage());
      }


      std::string InputDir, OutputDir;

      OPENFLUID_GetRunEnvironment("dir.input",InputDir);
      OPENFLUID_GetRunEnvironment("dir.output",OutputDir);

      m_DEMFilename = ParamsPT.get("DEMfile","");
      if (!m_DEMFilename.empty()) m_DEMFilename = boost::filesystem::path(InputDir+"/"+m_DEMFilename).string();

      // vtk series

      if (!ParamsPT.get_child_optional("vtkserie"))
        OPENFLUID_RaiseError("No vtkserie defined");

      BOOST_FOREACH(const boost::property_tree::ptree::value_type &v,ParamsPT.get_child("vtkserie"))
      {
        std::string SerieID = v.first;

        VtkSerieInfo VtkSerie;

        VtkSerie.UnitsClass = ParamsPT.get("vtkserie."+SerieID+".unitclass","");
        VtkSerie.VarName = ParamsPT.get("vtkserie."+SerieID+".varname","");
        VtkSerie.VectorSourceFilename = ParamsPT.get("vtkserie."+SerieID+".sourcefile","");
        VtkSerie.DEMSourceFilename = ParamsPT.get("vtkserie."+SerieID+".DEMfile","");

        if (!(VtkSerie.UnitsClass.empty() || VtkSerie.VarName.empty() || VtkSerie.VectorSourceFilename.empty()))
        {
          VtkSerie.VectorSourceFilename = boost::filesystem::path(InputDir+"/"+VtkSerie.VectorSourceFilename).string();
          VtkSerie.OutputDir = boost::filesystem::path(OutputDir+"/VTKseries/"+VtkSerie.UnitsClass+"_"+VtkSerie.VarName).string();
          VtkSerie.OutputFileBaseName = VtkSerie.UnitsClass+"_"+VtkSerie.VarName+"_t";

          if (!VtkSerie.DEMSourceFilename.empty())
            VtkSerie.DEMSourceFilename = boost::filesystem::path(InputDir+"/"+VtkSerie.DEMSourceFilename).string();

          m_VtkSeriesInfos.push_back(VtkSerie);
        }
      }

    }


    // =====================================================================
    // =====================================================================


    void onPrepared()
    {
      if (!m_DEMFilename.empty())
      {

        if (!m_RasterMan.open(m_DEMFilename))
        {
            OPENFLUID_RaiseWarning("ExportVtkFunction::initializeRun",
              "Cannot open DEM "+m_DEMFilename+". Z Values are set to 0.0");
        }
      }


      std::list<VtkSerieInfo>::iterator VtkSerieIt;
      std::vector<std::list<VtkSerieInfo>::iterator> VtkSeriesToDelete;

      for (VtkSerieIt = m_VtkSeriesInfos.begin();VtkSerieIt != m_VtkSeriesInfos.end();++VtkSerieIt)
      {
        if (!prepareVtkFile(*VtkSerieIt))
        {
          VtkSeriesToDelete.push_back(VtkSerieIt);
        }
        else
        {
          if (boost::filesystem::is_directory(boost::filesystem::path((*VtkSerieIt).OutputDir)))
            boost::filesystem::remove_all(boost::filesystem::path((*VtkSerieIt).OutputDir));

          boost::filesystem::create_directories(boost::filesystem::path((*VtkSerieIt).OutputDir));
        }
      }

      for (unsigned int i=0; i < VtkSeriesToDelete.size();i++) m_VtkSeriesInfos.erase(VtkSeriesToDelete[i]);

      m_RasterMan.close();
    }


    // =====================================================================
    // =====================================================================


    void onInitializedRun()
    {
      std::list<VtkSerieInfo>::iterator VtkSerieIt;

      for (VtkSerieIt = m_VtkSeriesInfos.begin();VtkSerieIt != m_VtkSeriesInfos.end();++VtkSerieIt)
      {
        writeVtkFile((*VtkSerieIt),OPENFLUID_GetCurrentTimeIndex());
      }

    }


    // =====================================================================
    // =====================================================================


    void onStepCompleted()
    {
      std::list<VtkSerieInfo>::iterator VtkSerieIt;

      for (VtkSerieIt = m_VtkSeriesInfos.begin();VtkSerieIt != m_VtkSeriesInfos.end();++VtkSerieIt)
      {
        writeVtkFile((*VtkSerieIt),OPENFLUID_GetCurrentTimeIndex());
      }

    }


    // =====================================================================
    // =====================================================================


    void onFinalizedRun()
    {
      std::list<VtkSerieInfo>::iterator VtkSerieIt;

        for (VtkSerieIt = m_VtkSeriesInfos.begin();VtkSerieIt != m_VtkSeriesInfos.end();++VtkSerieIt)
          writeSerieReport(*VtkSerieIt);
    }


};


// =====================================================================
// =====================================================================


DEFINE_OBSERVER_CLASS(VtkFilesObserver)
