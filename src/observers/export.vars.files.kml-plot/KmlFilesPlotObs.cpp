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
  @file KmlFilesPlotObs.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/ware/WareParamsTree.hpp>

#include "../KmlObserverBase.hpp"


// =====================================================================
// =====================================================================


BEGIN_OBSERVER_SIGNATURE("export.vars.files.kml-plot")

  DECLARE_NAME("Exports simulation variables to kml files with charts in balloons");
  // TODO complete description with parameters usage
  DECLARE_DESCRIPTION("");
  DECLARE_VERSION(openfluid::config::VERSION_FULL);
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

END_OBSERVER_SIGNATURE


// =====================================================================
// =====================================================================


class KmlUnitInfoExtra : public KmlUnitInfo
{
  public:

    bool IsPlotted;

    std::ofstream* DataFile;

    KmlUnitInfoExtra() : KmlUnitInfo(), IsPlotted(true), DataFile(nullptr)
    { };
};


// =====================================================================
// =====================================================================


class KmlSerieInfo : public KmlLayerInfo<KmlUnitInfoExtra>
{
  public:

    std::string VarsListStr;

    std::vector<openfluid::core::VariableName_t> VarsList;

    std::string OutputFileName;

    std::string DefaultColor;

    std::string PlottedColor;

    int LineWidth;

    KmlSerieInfo() :
      VarsListStr("*"),OutputFileName(""),
      DefaultColor("ffffffff"),PlottedColor("ffffffff"),
      LineWidth(1)
    { };
};


// =====================================================================
// =====================================================================


class KmlFilesPlotObserver : public KmlObserverBase
{
  private:

    std::string m_GNUPlotSubDir;

    openfluid::utils::ExternalProgram m_PlotProgram;

    std::string m_PlotXTics;

    std::list<KmlSerieInfo> m_KmlSeriesInfos;


    // =====================================================================
    // =====================================================================


    void buildGnuplotImages(const std::string& WorkDir, const std::string& DestDir)
    {
      for (std::list<KmlSerieInfo>::iterator it=m_KmlSeriesInfos.begin();it!=m_KmlSeriesInfos.end();++it)
      {
        for (std::map<openfluid::core::UnitID_t,KmlUnitInfoExtra>::iterator it2=(*it).UnitsInfos.begin();
            it2!=(*it).UnitsInfos.end();
            ++it2)
        {

          if ((*it2).second.IsPlotted)
          {
            std::string ScriptFilename = WorkDir+"/tmpscript.gp";
            std::string DataFilename = buildFilePath(WorkDir,(*it).UnitsClass,
                                                     (*it2).second.UnitID,"","dat");
            std::string OutputFilename = buildFilePath(DestDir,(*it).UnitsClass,
                                                       (*it2).second.UnitID,"","png");

            unsigned int Columns = 1;
            unsigned int Rows = 1;

            if ((*it).VarsList.size() > 1)
            {
              Columns = (unsigned int)(std::ceil(std::sqrt((*it).VarsList.size())));
              Rows = (unsigned int)(std::ceil((*it).VarsList.size() / Columns));
            }


            std::ofstream ScriptFile(ScriptFilename.c_str());
            ScriptFile << "set terminal png size 640,480 small\n";
            ScriptFile << "set output \"" << OutputFilename << "\"\n";
            ScriptFile << "set nokey\n";
            ScriptFile << "set xdata time\n";
            ScriptFile << "set timefmt \"%Y%m%d-%H%M%S\"\n";
            ScriptFile << "set datafile separator \";\"\n";
            ScriptFile << "set datafile commentschars \"#\"\n";
            ScriptFile << "set format x \"%Y-%m-%d\\n%H:%M:%S\"\n";
            ScriptFile << "set xtics " << m_PlotXTics << " font \",7\"\n";
            ScriptFile << "set ytics autofreq font \",7\"\n";

            ScriptFile << "set origin 0,0\n";
            ScriptFile << "set multiplot layout " << Rows << "," << Columns << " rowsfirst scale 1,1\n";

            for (unsigned int i=0; i< (*it).VarsList.size();i++)
            {
              ScriptFile << "set title \"" << (*it).VarsList[i] << "\" font \",9\"\n";
              ScriptFile << "plot \""<< DataFilename << "\" using 1:"<< (i+2) <<" with lines\n";
            }

            ScriptFile << "unset multiplot\n";

    //        ScriptFile << "set title \"" << (*it).VarName << "\" font \",9\"\n";
    //        ScriptFile << "plot \""<< DataFilename << "\" using 1:2 with lines\n";
            ScriptFile.close();


            // execution of the script

            if (m_PlotProgram.isFound())
            {

              QString GNUPlotCommand = QString("\"%1\" \"%2\"").arg(m_PlotProgram.getFullProgramPath())
                                                               .arg(QString::fromStdString(ScriptFilename));
              QProcess::execute(GNUPlotCommand);
            }

          }
        }
      }

    }


    // =====================================================================
    // =====================================================================


    void writeKmlFile(const std::string& DocFilePath)
    {
      std::ofstream KmlFile(DocFilePath.c_str());

      KmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
      KmlFile << "<kml xmlns=\"http://www.opengis.net/kml/2.2\" "
                 "xmlns:gx=\"http://www.google.com/kml/ext/2.2\" "
                 "xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n";

      KmlFile << "<Document>\n";

      KmlFile << "  <name>" << m_Title << "</name>\n";
      KmlFile << "  <open>1</open>\n";

      for (std::list<KmlSerieInfo>::iterator it=m_KmlSeriesInfos.begin();it!=m_KmlSeriesInfos.end();++it)
      {

        std::string TmpStyleID = (*it).UnitsClass +"_style";

        if ((*((*it).UnitsInfos.begin())).second.GeometryType == wkbPolygon)
        {
          KmlFile << "    <Style id=\"" << TmpStyleID << "\"><PolyStyle><color>"<< (*it).DefaultColor
                  << "</color><outline>1</outline></PolyStyle></Style>\n";
          KmlFile << "    <Style id=\"" << TmpStyleID << "_plotted\"><PolyStyle><color>"<< (*it).PlottedColor
                  << "</color></PolyStyle><outline>1</outline></Style>\n";
        }
        else if ((*((*it).UnitsInfos.begin())).second.GeometryType == wkbLineString)
        {
          KmlFile << "    <Style id=\"" << TmpStyleID << "\"><LineStyle><color>"<< (*it).DefaultColor
                  << "</color><width>" << (*it).LineWidth
                  << "</width></LineStyle><PolyStyle><fill>0</fill></PolyStyle></Style>\n";
          KmlFile << "    <Style id=\"" << TmpStyleID << "_plotted\"><LineStyle><color>"
                  << (*it).PlottedColor << "</color><width>" << (*it).LineWidth
                  << "</width></LineStyle><PolyStyle><fill>0</fill></PolyStyle></Style>\n";
        }
        else
        {
          OPENFLUID_LogWarning("Unsupported geometry format in source geometry file");
          m_OKToGo = false;
          return;
        }

        KmlFile << "    <Folder>\n";
        KmlFile << "      <name>" << (*it).UnitsClass << "</name>\n";

        for (std::map<openfluid::core::UnitID_t,KmlUnitInfoExtra>::iterator it2=(*it).UnitsInfos.begin();
            it2!=(*it).UnitsInfos.end();
            ++it2)
        {
          KmlFile << "    <Placemark>\n";


          KmlFile << "      <name>" << (*it).UnitsClass << " " << (*it2).second.UnitID << "</name>\n";

          KmlFile << "      <description>\n<![CDATA[\n";
          KmlFile << "Unit class: " << (*it).UnitsClass << "<br/>\n";
          KmlFile << "Unit ID: " << (*it2).second.UnitID << "<br/>\n";

          KmlFile << "<br/>\n";

          if ((*it2).second.IsPlotted)
          {
            KmlFile << "<img src=\"" << buildFilePath(m_KmzDataSubDir,(*it).UnitsClass,
                                                      (*it2).second.UnitID,"","png") << "\"/>\n";
          }

          KmlFile << "\n]]>\n      </description>\n";


          if ((*it2).second.IsPlotted)
          {
            KmlFile << "      <styleUrl>#" << TmpStyleID << "_plotted</styleUrl>\n";
          }
          else
          {
            KmlFile << "      <styleUrl>#" << TmpStyleID << "</styleUrl>\n";
          }


          if ((*it2).second.GeometryType == wkbPolygon)
          {
            KmlFile << "<Polygon><tessellate>1</tessellate><outerBoundaryIs><LinearRing><coordinates>"
                    << (*it2).second.CoordsStr << "</coordinates></LinearRing></outerBoundaryIs></Polygon>\n";
          }
          else if ((*it2).second.GeometryType == wkbLineString)
          {
            KmlFile << "<LineString><tessellate>1</tessellate><coordinates>" << (*it2).second.CoordsStr
                    << "</coordinates></LineString>\n";
          }
          else
          {
            OPENFLUID_LogWarning("Unsupported geometry format in source geometry file");
            m_OKToGo = false;
            return;
          }


          KmlFile << "    </Placemark>\n";
        }

        KmlFile << "    </Folder>\n";
      }


      KmlFile << "</Document>\n";
      KmlFile << "</kml>\n";
      KmlFile.close();
    }


    // =====================================================================
    // =====================================================================


    static std::string buildFilePath(const std::string& Dir,const openfluid::core::UnitsClass_t& UnitsClass,
                                     openfluid::core::UnitID_t ID, const openfluid::core::VariableName_t& VarName,
                                     const std::string& Ext)
    {
      std::ostringstream oss;

      oss << Dir << "/" << UnitsClass << "_" << ID << "_" << VarName << "." << Ext;

      return oss.str();
    }


  public:

    KmlFilesPlotObserver() :
      KmlObserverBase(), m_GNUPlotSubDir("gnuplot"),
      m_PlotProgram(openfluid::utils::ExternalProgram::getRegisteredProgram(
          openfluid::utils::ExternalProgram::GnuplotProgram))
    {
      m_TmpSubDirRoot = "export.vars.files.kml-plot";
      m_OutputFileName = "kmlplot.kmz";
    }


    // =====================================================================
    // =====================================================================


    ~KmlFilesPlotObserver()
    {

    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      openfluid::ware::WareParamsTree ParamsTree;

      try
      {
        ParamsTree.setParams(Params);
      }
      catch (openfluid::base::FrameworkException& E)
      {
        OPENFLUID_LogWarning(E.getMessage());
        m_OKToGo = false;
        return;
      }


      OGRRegisterAll();

      OPENFLUID_GetRunEnvironment("dir.input",m_InputDir);
      OPENFLUID_GetRunEnvironment("dir.output",m_OutputDir);

      // general

      try
      {
        m_Title = ParamsTree.root().getChildValue("title",m_Title);

        m_OutputFileName = ParamsTree.root().getChildValue("kmzfilename",m_OutputFileName);
        ParamsTree.root().getChildValue("tryopengearth",m_TryOpenGEarth).toBoolean(m_TryOpenGEarth);


        if (!ParamsTree.root().hasChild("layers"))
        {
          OPENFLUID_LogWarning("No layer defined");
          m_OKToGo = false;
          return;
        }

        for (auto& Layer : ParamsTree.root().child("layers"))
        {
          KmlSerieInfo KSI;

          KSI.UnitsClass = Layer.second.getChildValue("unitsclass","");
          if (KSI.UnitsClass == "")
          {
            // search for deprecated "unitclass" parameter
            KSI.UnitsClass = Layer.second.getChildValue("unitclass","");
          }


          KSI.VarsListStr = Layer.second.getChildValue("varslist","*");

          // TODO Manage selection of plotted spatial units

          Layer.second.getChildValue("linewidth",1).toInteger(KSI.LineWidth);
          KSI.DefaultColor = Layer.second.getChildValue("defaultcolor","ffffffff");
          KSI.PlottedColor = Layer.second.getChildValue("plottedcolor","ff0000ff");

          KSI.SourceIsDatastore = (Layer.second.getChildValue("source","file").get() == "datastore");
          if (KSI.SourceIsDatastore)
            return;
          else
          {
            KSI.SourceFilename = Layer.second.getChildValue("sourcefile","");
            if (KSI.SourceFilename.empty())
            {
              OPENFLUID_LogWarning("wrong sourcefile format");
              m_OKToGo = false;
              return;
            }
            KSI.SourceFilename = m_InputDir + "/" + KSI.SourceFilename;
          }

          if (transformVectorLayerToKmlGeometry(KSI))
          {
            m_KmlSeriesInfos.push_back(KSI);
          }

        }
      }
      catch (openfluid::base::FrameworkException& E)
      {
        OPENFLUID_LogWarning(E.getMessage());
        m_OKToGo = false;
        return;
      }

      m_OKToGo = true;

      m_KmzSubDir = m_OutputFileName + "_kmz-dir";
      m_GNUPlotSubDir = m_OutputFileName + "_gnuplot-dir";
    }


    // =====================================================================
    // =====================================================================


    void onPrepared()
    {
      if (!m_OKToGo)
        return;

      if (!m_PlotProgram.isFound())
      {
        OPENFLUID_LogWarning("Required GNUplot program not found");
        m_OKToGo = false;
        return;
      }


      std::ostringstream TmpOSS;

      TmpOSS << ((OPENFLUID_GetEndDate().diffInSeconds(OPENFLUID_GetBeginDate())) / 2);

      m_PlotXTics = TmpOSS.str();


      prepareTempDirectory();

      if (!m_OKToGo)
        return;


      openfluid::tools::Filesystem::removeDirectory(m_TmpDir+"/"+m_GNUPlotSubDir);
      openfluid::tools::Filesystem::makeDirectory(m_TmpDir+"/"+m_GNUPlotSubDir);

      if (!openfluid::tools::Filesystem::isDirectory(m_TmpDir+"/"+m_GNUPlotSubDir))
      {
        OPENFLUID_LogWarning("Cannot initialize gnuplot temporary directory");
        m_OKToGo = false;
        return;
      }


      // initialize data files

      for (std::list<KmlSerieInfo>::iterator it=m_KmlSeriesInfos.begin();it!=m_KmlSeriesInfos.end();++it)
      {
        if ((*it).VarsListStr == "*")
        {
          (*it).VarsList =
              mp_SpatialData->spatialUnits((*it).UnitsClass)->list()->front().variables()->getVariablesNames();
        }
        else
        {
          openfluid::tools::tokenizeString((*it).VarsListStr,(*it).VarsList,";");
        }

        if ((*it).VarsList.empty())
        {
          OPENFLUID_LogWarning("Variable list is empty");
          m_OKToGo = false;
          return;
        }


        for (std::map<openfluid::core::UnitID_t,KmlUnitInfoExtra>::iterator it2=(*it).UnitsInfos.begin();
            it2!=(*it).UnitsInfos.end();
            ++it2)
        {

          if ((*it2).second.IsPlotted)
          {
            std::string DataFilename =
                buildFilePath(m_TmpDir+"/"+m_GNUPlotSubDir,
                              (*it).UnitsClass,(*it2).second.UnitID,
                              "","dat");

            (*it2).second.DataFile = new std::ofstream(DataFilename.c_str());
            (*((*it2).second.DataFile)) << "#" << DataFilename << "\n";
          }
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void appendToDataFiles()
    {

      if (!m_OKToGo)
        return;

      openfluid::core::SpatialUnit* UU;


      // write data files
      for (std::list<KmlSerieInfo>::iterator it=m_KmlSeriesInfos.begin();it!=m_KmlSeriesInfos.end();++it)
      {
        for (std::map<openfluid::core::UnitID_t,KmlUnitInfoExtra>::iterator it2=(*it).UnitsInfos.begin();
             it2!=(*it).UnitsInfos.end();
             ++it2)
        {
          if ((*it2).second.IsPlotted)
          {

            UU = OPENFLUID_GetUnit((*it).UnitsClass,(*it2).second.UnitID);

            if (UU != nullptr)
            {
              (*((*it2).second.DataFile)) << OPENFLUID_GetCurrentDate().getAsString("%Y%m%d-%H%M%S");


              for (std::vector<openfluid::core::VariableName_t>::iterator it3 = (*it).VarsList.begin();
                  it3 != (*it).VarsList.end();
                  ++it3)
              {
                if (OPENFLUID_IsVariableExist(UU,*it3,OPENFLUID_GetCurrentTimeIndex(),openfluid::core::Value::DOUBLE))
                {

                  double Val;
                  OPENFLUID_GetVariable(UU,*it3,OPENFLUID_GetCurrentTimeIndex(),Val);
                  (*((*it2).second.DataFile)) << ";" << Val;
                }
              }

              (*((*it2).second.DataFile)) << "\n";
            }
            else
            {
              (*((*it2).second.DataFile)) << OPENFLUID_GetCurrentDate().getAsString("%Y%m%d-%H%M%S") << ";n/a\n";
            }
          }
        }
      }

    }


    // =====================================================================
    // =====================================================================


    void onInitializedRun()
    {
      appendToDataFiles();
    }


    // =====================================================================
    // =====================================================================


    void onStepCompleted()
    {
      appendToDataFiles();
    }


    // =====================================================================
    // =====================================================================


    void onFinalizedRun()
    {
      if (!m_OKToGo)
        return;

      // close data files

      for (std::list<KmlSerieInfo>::iterator it=m_KmlSeriesInfos.begin();it!=m_KmlSeriesInfos.end();++it)
      {
        for (std::map<openfluid::core::UnitID_t,KmlUnitInfoExtra>::iterator it2=(*it).UnitsInfos.begin();
            it2!=(*it).UnitsInfos.end();
            ++it2)
        {
          if ((*it2).second.IsPlotted)
          {
            if ((*it2).second.DataFile!=nullptr && (*it2).second.DataFile->is_open())
              (*it2).second.DataFile->close();
            delete (*it2).second.DataFile;
          }
        }
      }


      buildGnuplotImages(m_TmpDir+"/"+m_GNUPlotSubDir,m_TmpDir+"/"+m_KmzSubDir+"/"+m_KmzDataSubDir);


      writeKmlFile(m_TmpDir+"/"+m_KmzSubDir+"/doc.kml");

      buildKmzFile();

      tryOpenGEarth();

    }


};


// =====================================================================
// =====================================================================


DEFINE_OBSERVER_CLASS(KmlFilesPlotObserver)
