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
  \file KmlFilesPlotObs.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <boost/foreach.hpp>

#include "../KmlObserverBase.hpp"



// =====================================================================
// =====================================================================


DECLARE_OBSERVER_PLUGIN


// =====================================================================
// =====================================================================


BEGIN_OBSERVER_SIGNATURE("export.vars.files.kml-plot")

  DECLARE_NAME("Exports simulation variables to kml files with charts in balloons");
  // TODO complete description with parameters usage
  DECLARE_DESCRIPTION("");
  DECLARE_VERSION("1.0");
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================

class KmlUnitInfoExtra : public KmlUnitInfo
{
  public:


    bool IsPlotted;

    std::ofstream* DataFile;

    KmlUnitInfoExtra() : KmlUnitInfo(), IsPlotted(true), DataFile(NULL)
    { };
};


// =====================================================================
// =====================================================================


class KmlSerieInfo : public KmlLayerInfo<KmlUnitInfoExtra>
{
  public:

    std::string VarNamesStr;

    std::vector<openfluid::core::VariableName_t> VarNames;

    std::string OutputFileName;

    std::string DefaultColor;

    std::string PlottedColor;

    int LineWidth;

    KmlSerieInfo() : VarNamesStr("*"),OutputFileName(""), DefaultColor("ffffffff"),
                     PlottedColor("ffffffff")
    { };
};


// =====================================================================
// =====================================================================


class KmlFilesPlotObserver : public KmlObserverBase
{
  private:

    const std::string m_GNUPlotSubDir;

    std::string m_PlotCommand;

    std::string m_PlotXTics;

    std::list<KmlSerieInfo> m_KmlSeriesInfos;


    // =====================================================================
    // =====================================================================


    void buildGnuplotImages(std::string WorkDir, std::string DestDir)
    {
      for (std::list<KmlSerieInfo>::iterator it=m_KmlSeriesInfos.begin();it!=m_KmlSeriesInfos.end();++it)
      {
        for (std::map<openfluid::core::UnitID_t,KmlUnitInfoExtra>::iterator it2=(*it).UnitsInfos.begin();it2!=(*it).UnitsInfos.end();++it2)
        {

          if ((*it2).second.IsPlotted)
          {
            std::string ScriptFilename = boost::filesystem::path(WorkDir+"/tmpscript.gp").string();
            std::string DataFilename = buildFilePath(WorkDir,(*it).UnitsClass,(*it2).second.UnitID,(*it).GroupName,"dat");
            std::string OutputFilename = buildFilePath(DestDir,(*it).UnitsClass,(*it2).second.UnitID,(*it).GroupName,"png");


            unsigned int Columns = 1;
            unsigned int Rows = 1;

            if ((*it).VarNames.size() > 1)
            {
              Columns = (unsigned int)(std::ceil(std::sqrt((*it).VarNames.size())));
              Rows = (unsigned int)(std::ceil((*it).VarNames.size() / Columns));
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

            for (unsigned int i=0; i< (*it).VarNames.size();i++)
            {
              ScriptFile << "set title \"" << (*it).VarNames[i] << "\" font \",9\"\n";
              ScriptFile << "plot \""<< DataFilename << "\" using 1:"<< (i+2) <<" with lines\n";
            }

            ScriptFile << "unset multiplot\n";

    //        ScriptFile << "set title \"" << (*it).VarName << "\" font \",9\"\n";
    //        ScriptFile << "plot \""<< DataFilename << "\" using 1:2 with lines\n";
            ScriptFile.close();

            std::string FullPlotCommand = "\"" + m_PlotCommand +"\" \"" + ScriptFilename +"\"";

            std::string StrOut = "";
            std::string StrErr = "";
            int RetValue = 0;
            Glib::spawn_command_line_sync(FullPlotCommand,&StrOut,&StrErr,&RetValue);


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
      KmlFile << "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n";

      KmlFile << "<Document>\n";

      KmlFile << "  <name>" << m_Title << "</name>\n";
      KmlFile << "  <open>1</open>\n";

      for (std::list<KmlSerieInfo>::iterator it=m_KmlSeriesInfos.begin();it!=m_KmlSeriesInfos.end();++it)
      {

        std::string TmpStyleID = (*it).UnitsClass +"_style";

        KmlFile << "    <Style id=\"" << TmpStyleID << "\"><LineStyle><color>"<< (*it).DefaultColor << "</color><width>" << (*it).LineWidth << "</width></LineStyle><PolyStyle><fill>0</fill></PolyStyle></Style>\n";

        KmlFile << "    <Style id=\"" << TmpStyleID << "_plotted\"><LineStyle><color>"<< (*it).PlottedColor << "</color><width>" << (*it).LineWidth << "</width></LineStyle><PolyStyle><fill>0</fill></PolyStyle></Style>\n";

        KmlFile << "    <Folder>\n";
        KmlFile << "      <name>" << (*it).UnitsClass << "</name>\n";

        for (std::map<openfluid::core::UnitID_t,KmlUnitInfoExtra>::iterator it2=(*it).UnitsInfos.begin();it2!=(*it).UnitsInfos.end();++it2)
        {
          KmlFile << "    <Placemark>\n";


          KmlFile << "      <name>" << (*it).UnitsClass << " " << (*it2).second.UnitID << "</name>\n";

          KmlFile << "      <description>\n<![CDATA[\n";
          KmlFile << "Unit class: " << (*it).UnitsClass << "<br/>\n";
          KmlFile << "Unit ID: " << (*it2).second.UnitID << "<br/>\n";

          KmlFile << "<br/>\n";

          if ((*it2).second.IsPlotted)
          {
            KmlFile << "<img src=\"" << buildFilePath(m_KmzDataSubDir,(*it).UnitsClass,(*it2).second.UnitID,(*it).GroupName,"png") << "\"/>\n";
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
            KmlFile << "<Polygon><tessellate>1</tessellate><outerBoundaryIs><LinearRing><coordinates>" << (*it2).second.CoordsStr << "</coordinates></LinearRing></outerBoundaryIs></Polygon>\n";
          }

          if ((*it2).second.GeometryType == wkbLineString)
          {
            KmlFile << "<LineString><tessellate>1</tessellate><coordinates>" << (*it2).second.CoordsStr << "</coordinates></LineString>\n";
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


    static std::string buildFilePath(std::string Dir,std::string UnitClass,
                                     openfluid::core::UnitID_t ID, std::string VarName,
                                     std::string Ext)
    {
      std::ostringstream oss;

      oss << Dir << "/" << UnitClass << "_" << ID << "_" << VarName << "." << Ext;

      return boost::filesystem::path(oss.str()).string();
    }



  public:

    KmlFilesPlotObserver() : KmlObserverBase(), m_GNUPlotSubDir("gnuplot")
    {
      m_TmpSubDir = "export.vars.files.kml-plot";
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
      boost::property_tree::ptree Params_pt = openfluid::ware::PluggableWare::getParamsAsPropertyTree(Params);

      OGRRegisterAll();

      OPENFLUID_GetRunEnvironment("dir.input",m_InputDir);
      OPENFLUID_GetRunEnvironment("dir.output",m_OutputDir);

      // general

      m_Title = Params_pt.get("title",m_Title);
      m_OutputFileName = Params_pt.get("kmzfilename",m_OutputFileName);
      m_TryOpenGEarth = Params_pt.get<bool>("tryopengearth",m_TryOpenGEarth);


      BOOST_FOREACH(const boost::property_tree::ptree::value_type &v,Params_pt.get_child("layers"))
      {
        std::string LayerID = v.first;

        KmlSerieInfo KSI;

        KSI.UnitsClass = Params_pt.get("layers."+LayerID+".unitclass","");


        KSI.VarNamesStr = Params_pt.get("layers."+LayerID+".varnames","*");

        // TODO Manage selection of plotted spatial units

        KSI.LineWidth = Params_pt.get<int>("layers."+LayerID+".linewidth",1);
        KSI.DefaultColor = Params_pt.get("layers."+LayerID+".defaultcolor","ffffffff");
        KSI.PlottedColor = Params_pt.get("layers."+LayerID+".plottedcolor","ff0000ff");

        KSI.SourceIsDatastore = (Params_pt.get("layers."+LayerID+".source","file") == "datastore");
        if (KSI.SourceIsDatastore)
          return;
        else
        {
          KSI.SourceFilename = Params_pt.get("layers."+LayerID+".sourcefile","");
          if (KSI.SourceFilename.empty())
          {
            OPENFLUID_RaiseWarning(OPENFLUID_GetWareID(),"KmlFilesPlotObserver::initParams()",
                                   "wrong sourcefile format");
            return;
          }
          KSI.SourceFilename = m_InputDir + "/" + KSI.SourceFilename;
        }

        if (transformVectorLayerToKmlGeometry(KSI))
        {
          m_KmlSeriesInfos.push_back(KSI);
        }

      }

      m_OKToGo = true;


    }


    // =====================================================================
    // =====================================================================


    void onPrepared()
    {
      if (!m_OKToGo) return;

      searchForZipTool();

      if (!m_OKToGo) return;

      std::string PlotProgram = "";

      #if defined __unix__ || defined __APPLE__
        PlotProgram = "gnuplot";
      #endif

      #if WIN32
        PlotProgram = "wgnuplot.exe";
      #endif

      if (PlotProgram.empty())
      {
        OPENFLUID_RaiseWarning("utils.export.spatialdomain-vars.kml-plot","ExportKmlPlotFunction::initializeRun()",
                    "Unsupported system platform");
        m_OKToGo = false;
        return;
      }

      std::vector<std::string> PlotPaths = openfluid::tools::GetFileLocationsUsingPATHEnvVar(PlotProgram);

      if (!PlotPaths.empty())
      {
        m_PlotCommand = boost::filesystem::path(PlotPaths[0]).string();
      }
      else
      {
        OPENFLUID_RaiseWarning("utils.export.spatialdomain-vars.kml-plot","ExportKmlPlotFunction::initializeRun()",
            "Required Plot program ("+ PlotProgram +") not found");
        m_OKToGo = false;
        return;
      }


      std::ostringstream TmpOSS;

      TmpOSS << ((OPENFLUID_GetEndDate().diffInSeconds(OPENFLUID_GetBeginDate())) / 2);

      m_PlotXTics = TmpOSS.str();


      prepareTempDirectory();

      if (!m_OKToGo) return;


      boost::filesystem::create_directories(boost::filesystem::path(m_TmpDir+"/"+m_GNUPlotSubDir));

      if (!boost::filesystem::is_directory(boost::filesystem::path(m_TmpDir+"/"+m_GNUPlotSubDir)))
      {
        OPENFLUID_RaiseWarning("utils.export.spatialdomain-vars.kml-plot","ExportKmlPlotFunction::initializeRun()",
            "Cannot initialize gnuplot temporary directory");
        m_OKToGo = false;
        return;
      }



      // initialize data files

      for (std::list<KmlSerieInfo>::iterator it=m_KmlSeriesInfos.begin();it!=m_KmlSeriesInfos.end();++it)
      {
        if ((*it).VarNamesStr == "*")
        {
          (*it).VarNames = mp_CoreData->getUnits((*it).UnitsClass)->getList()->front().getVariables()->getVariablesNames();
        }
        else
        {
          openfluid::tools::TokenizeString((*it).VarNamesStr,(*it).VarNames,";");
        }
        if ((*it).VarNames.empty())
        {
          m_OKToGo = false;
          return;
        }


        for (std::map<openfluid::core::UnitID_t,KmlUnitInfoExtra>::iterator it2=(*it).UnitsInfos.begin();it2!=(*it).UnitsInfos.end();++it2)
        {

          if ((*it2).second.IsPlotted)
          {
            std::string DataFilename = buildFilePath(boost::filesystem::path(m_TmpDir+"/"+m_GNUPlotSubDir).string(),(*it).UnitsClass,(*it2).second.UnitID,(*it).GroupName,"dat");

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

      if (!m_OKToGo) return;

      openfluid::core::Unit* UU;


      // write data files
      for (std::list<KmlSerieInfo>::iterator it=m_KmlSeriesInfos.begin();it!=m_KmlSeriesInfos.end();++it)
      {
        for (std::map<openfluid::core::UnitID_t,KmlUnitInfoExtra>::iterator it2=(*it).UnitsInfos.begin();it2!=(*it).UnitsInfos.end();++it2)
        {
          if ((*it2).second.IsPlotted)
          {

            UU = OPENFLUID_GetUnit((*it).UnitsClass,(*it2).second.UnitID);

            if (UU != NULL)
            {
              (*((*it2).second.DataFile)) << OPENFLUID_GetCurrentDate().getAsString("%Y%m%d-%H%M%S");


              for (std::vector<openfluid::core::VariableName_t>::iterator it3 = (*it).VarNames.begin();it3 != (*it).VarNames.end();++it3)
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
      if (!m_OKToGo) return;

      // close data files

      for (std::list<KmlSerieInfo>::iterator it=m_KmlSeriesInfos.begin();it!=m_KmlSeriesInfos.end();++it)
      {
        for (std::map<openfluid::core::UnitID_t,KmlUnitInfoExtra>::iterator it2=(*it).UnitsInfos.begin();it2!=(*it).UnitsInfos.end();++it2)
        {
          if ((*it2).second.IsPlotted)
          {
            (*it2).second.DataFile->close();
            delete (*it2).second.DataFile;
          }
        }
      }


      buildGnuplotImages(m_TmpDir+"/"+m_GNUPlotSubDir,m_TmpDir+"/"+m_KmzSubDir+"/"+m_KmzDataSubDir);


      writeKmlFile(boost::filesystem::path(m_TmpDir+"/"+m_KmzSubDir+"/doc.kml").string());

      buildKmzFile();

      tryOpenGEarth();

    }


};



// =====================================================================
// =====================================================================


DEFINE_OBSERVER_CLASS(KmlFilesPlotObserver)
