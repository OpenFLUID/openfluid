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
  @file GNUplotObs.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */



#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/ware/PluggableObserver.hpp>
#include <openfluid/tools/DataHelpers.hpp>

#include <QDir>
#include <QProcess>


// =====================================================================
// =====================================================================


DECLARE_OBSERVER_PLUGIN


// =====================================================================
// =====================================================================


BEGIN_OBSERVER_SIGNATURE("export.vars.plot.gnuplot")

  DECLARE_NAME("Plots simulation variables using GNUplot");
  DECLARE_DESCRIPTION("This observer plots simulation variables using GNUplot formalism and tools\n"
      "It uses series of values (simulated or from existing data files) that can be combined into one or many graphs.\n"
      "Plotted variables can only be scalar variables (boolean, integer, double)\n"
      "Parameters for declaring series can be:\n"
      "  serie.<seriename>.var : the name of the variable to plot (e.g. var.foo)\n"
      "  serie.<seriename>.unitclass : the unit class (e.g. TU)\n"
      "  serie.<seriename>.unitid : the unit id (e.g. 17)\n"
      "  serie.<seriename>.sourcefile : the file to plot (e.g. measured_data.dat), if this serie is not based on simulation variables\n"
      "  serie.<seriename>.style : the GNUplot style to use to plot this serie (e.g. linespoint)\n"
      "  serie.<seriename>.label : the label to use for this serie (e.g. foo) instead of the automatic label\n"
      "Parameters for declaring graphs can be:\n"
      "  graph.<graphname>.series : a semicolon seprated list of <seriename> (e.g. serie1;serie2)\n"
      "  graph.<graphname>.title : the title to use for this graph (e.g. my graph) instead of the automatic title\n"
      "  graph.<graphname>.key : the mode for key display\n"
      "  graph.<graphname>.ylabel : the ylabel for the graph\n"
      "Global parameters are:\n"
      "  terminal : the terminal mode to replace the default wxt mode (e.g. pdfcairo size 11.7,8.3)\n"
      "  output : the output file name when terminal exports graphs to file (e.g. mygraph.pdf)\n"
      "  tryopengnuplot : set to 1 to run GNUplot after simulation ends\n"
      "  persistent : set to 1 to set GNUplot persistent if run after simulation ends\n"
      );

  DECLARE_VERSION(openfluid::config::FULL_VERSION);
  DECLARE_STATUS(openfluid::ware::STABLE);

END_OBSERVER_SIGNATURE


// =====================================================================
// =====================================================================

class SerieInfo
{
  public:

    enum SerieType {SERIE_UNKNOWN, SERIE_VAR, SERIE_FILE};

    SerieType Type;

    std::string SourceFile;

    std::ofstream* FileHandle;

    openfluid::core::VariableName_t VarName;

    openfluid::core::UnitClass_t UnitClass;

    openfluid::core::UnitID_t UnitID;

    openfluid::core::SpatialUnit* Unit;

    std::string Label;

    std::string Style;

    std::string Color;

    SerieInfo() : Type(SERIE_UNKNOWN),
        SourceFile(""), FileHandle(NULL), VarName(""), UnitClass(""), UnitID(1), Unit(NULL),
        Label(""), Style("line"), Color("")
    { }

};


class GraphInfo
{
  public:

    std::string Title;

    std::string Key;

    std::string YLabel;

    std::list<SerieInfo*> Series;

    GraphInfo() : Title(""), Key("default"), YLabel("")
    { }

};


// =====================================================================
// =====================================================================


class GNUplotObserver : public openfluid::ware::PluggableObserver
{
  private:

    std::map<std::string, SerieInfo> m_Series;

    std::map<std::string,GraphInfo> m_Graphs;

    std::string m_OutputDir;
    std::string m_InputDir;

    bool m_TryOpenGNUplot;

    bool m_Persistent;

    std::string m_Terminal;
    std::string m_Output;


  public:

    GNUplotObserver() : PluggableObserver(), m_OutputDir(""),
      m_TryOpenGNUplot(false), m_Persistent(false), m_Terminal(""), m_Output("")
    {

    }

    // =====================================================================
    // =====================================================================


    ~GNUplotObserver()
    {
      CloseFiles();
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

      m_TryOpenGNUplot = ParamsPT.get<bool>("tryopengnuplot",m_TryOpenGNUplot);
      m_Persistent = ParamsPT.get<bool>("persistent",m_Persistent);
      m_Terminal = ParamsPT.get("terminal",m_Terminal);
      m_Output = ParamsPT.get("output",m_Output);


      if (!ParamsPT.get_child_optional("serie"))
        OPENFLUID_RaiseError("No serie defined");

      if (!ParamsPT.get_child_optional("graph"))
              OPENFLUID_RaiseError("No graph defined");


      foreach (const boost::property_tree::ptree::value_type &v,ParamsPT.get_child("serie"))
      {
        std::string SerieID = v.first;

        SerieInfo SInfo;
        SInfo.VarName = ParamsPT.get("serie."+SerieID+".var","");
        SInfo.UnitClass = ParamsPT.get("serie."+SerieID+".unitclass","");
        std::string UnitIDStr = ParamsPT.get("serie."+SerieID+".unitID","");
        SInfo.SourceFile = ParamsPT.get("serie."+SerieID+".sourcefile","");
        SInfo.Label = ParamsPT.get("serie."+SerieID+".label","");
        SInfo.Style = ParamsPT.get("serie."+SerieID+".style","line");
        SInfo.Color = ParamsPT.get("serie."+SerieID+".color","");

        if (!SInfo.VarName.empty() && !SInfo.UnitClass.empty() && openfluid::tools::convertString(UnitIDStr,&SInfo.UnitID))
        {
          openfluid::core::SpatialUnit* TmpU;
          TmpU = mp_SpatialData->spatialUnit(SInfo.UnitClass,SInfo.UnitID);
          if (TmpU != NULL)
          {
            SInfo.Type = SerieInfo::SERIE_VAR;
            SInfo.Unit = TmpU;
            SInfo.SourceFile = SerieID + "_data.gnuplot";
          }
        }
        else if (!SInfo.SourceFile.empty())
          SInfo.Type = SerieInfo::SERIE_FILE;

        if (SInfo.Type!=SerieInfo::SERIE_UNKNOWN) m_Series[SerieID] = SInfo;
        else OPENFLUID_RaiseWarning("Serie " + SerieID + "ignored");

      }


      foreach (const boost::property_tree::ptree::value_type &v,ParamsPT.get_child("graph"))
      {
        std::string GraphID = v.first;

         GraphInfo GInfo;

         GInfo.Title = ParamsPT.get("graph."+GraphID+".title",v.first);
         GInfo.Key = ParamsPT.get("graph."+GraphID+".key","default");
         GInfo.YLabel = ParamsPT.get("graph."+GraphID+".ylabel","");

         std::vector<std::string> SeriesStr = openfluid::tools::splitString(ParamsPT.get("graph."+GraphID+".series",""),
                                                                            ";",false);

         for (std::vector<std::string>::const_iterator it = SeriesStr.begin();it != SeriesStr.end();++it)
         {
           if (m_Series.find(*it) != m_Series.end())
             GInfo.Series.push_back(&(m_Series[*it]));
         }

         if (!GInfo.Series.empty())
           m_Graphs[v.first] = GInfo;
         else
           OPENFLUID_RaiseWarning("Graph " + v.first + "ignored");
      }
    }


    // =====================================================================
    // =====================================================================


    void WriteDataToFiles()
    {
      std::map<std::string, SerieInfo>::iterator Sit;
      std::map<std::string, SerieInfo>::iterator Sitb = m_Series.begin();
      std::map<std::string, SerieInfo>::iterator Site = m_Series.end();

      for (Sit = Sitb; Sit != Site; ++Sit)
      {
        if ((*Sit).second.Type == SerieInfo::SERIE_VAR)
        {
          openfluid::core::Value* Val = (*Sit).second.Unit->variables()->currentValueIfIndex((*Sit).second.VarName,OPENFLUID_GetCurrentTimeIndex());

          if (Val!= NULL)
          {
            *((*Sit).second.FileHandle) << OPENFLUID_GetCurrentDate().getAsString("%Y-%m-%dT%H:%M:%S");
            *((*Sit).second.FileHandle) << " ";
            if (Val->isSimple() && !Val->isStringValue())
              Val->writeToStream(*((*Sit).second.FileHandle));
            else
              *((*Sit).second.FileHandle) << "NaN";
            *((*Sit).second.FileHandle) << "\n";
          }
        }
      }
    }

    // =====================================================================
    // =====================================================================


    void CloseFiles()
    {
      std::map<std::string, SerieInfo>::iterator Sit;
      std::map<std::string, SerieInfo>::iterator Sitb = m_Series.begin();
      std::map<std::string, SerieInfo>::iterator Site = m_Series.end();

      for (Sit = Sitb; Sit != Site; ++Sit)
      {
        if ((*Sit).second.Type == SerieInfo::SERIE_VAR)
        {
          if ((*Sit).second.FileHandle!=NULL && (*Sit).second.FileHandle->is_open())
            (*Sit).second.FileHandle->close();
        }
      }

    }


    // =====================================================================
    // =====================================================================


    void onPrepared()
    {
      OPENFLUID_GetRunEnvironment("dir.output",m_OutputDir);
      OPENFLUID_GetRunEnvironment("dir.input",m_InputDir);

      std::map<std::string, SerieInfo>::iterator Sit;
      std::map<std::string, SerieInfo>::iterator Sitb = m_Series.begin();
      std::map<std::string, SerieInfo>::iterator Site = m_Series.end();

      for (Sit = Sitb; Sit != Site; ++Sit)
      {
        if ((*Sit).second.Type == SerieInfo::SERIE_VAR)
        {
          (*Sit).second.FileHandle = new std::ofstream(std::string(m_OutputDir+"/"+(*Sit).second.SourceFile).c_str());
        }
      }

    }


    // =====================================================================
    // =====================================================================


    void onInitializedRun()
    {
      WriteDataToFiles();
    }


    // =====================================================================
    // =====================================================================


    void onStepCompleted()
    {
      WriteDataToFiles();
    }


    // =====================================================================
    // =====================================================================


    void tryOpenGNUplot()
    {
      if (m_TryOpenGNUplot)
      {
        openfluid::utils::ExternalProgram GNUPlotProgram =
            openfluid::utils::ExternalProgram::getRegisteredProgram(openfluid::utils::ExternalProgram::GnuplotProgram);

        if (GNUPlotProgram.isFound())
        {
          QString PersistOption = " ";
          if (m_Persistent) PersistOption = " -persist ";

          QString GNUPlotCommand = QString("\"%1\"%2\"%3\"").arg(GNUPlotProgram.getFullProgramPath())
                                                    .arg(PersistOption)
                                                    .arg(QDir(QString::fromStdString(m_OutputDir))
                                                         .absoluteFilePath("script.gnuplot"));

          QProcess::execute(GNUPlotCommand);
        }
        else
        {
          OPENFLUID_RaiseWarning("GNUplotObserver::tryOpenGNUplot()",
                                 "Cannot find GNUPlot");
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void onFinalizedRun()
    {
      CloseFiles();

      std::ofstream ScriptFile(std::string(m_OutputDir+"/script.gnuplot").c_str());

      if (!m_Terminal.empty())
      {
        ScriptFile << "set terminal " << m_Terminal << "\n";
        if (!m_Output.empty())
          ScriptFile << "set output \"" << m_OutputDir << "/" << m_Output << "\"\n";
      }

      ScriptFile << "set xtics rotate font \",5\"\n";
      ScriptFile << "set ytics font \",7\"\n";
      ScriptFile << "set xdata time\n";
      ScriptFile << "set timefmt \"%Y-%m-%dT%H:%M:%S\"\n";
      ScriptFile << "set datafile separator \" \"\n";
      ScriptFile << "set datafile commentschars \"#\"\n";
      ScriptFile << "set format x \"%Y-%m-%d\\n%H:%M:%S\"\n";
      ScriptFile << "set datafile missing \"NaN\"\n";
      ScriptFile << "set xlabel \"Time\"\n";

      unsigned int Columns = 1;
      unsigned int Rows = 1;

      if (m_Graphs.size() > 1)
      {
        Columns = (unsigned int)(std::ceil(std::sqrt(m_Graphs.size())));
        Rows = (unsigned int)(std::ceil(m_Graphs.size() / Columns));

        if (Columns*Rows < m_Graphs.size()) Rows++;
      }

      ScriptFile << "set multiplot layout " << Rows << "," << Columns << " rowsfirst scale 1,1\n";

      std::map<std::string,GraphInfo>::iterator Git;
      std::map<std::string,GraphInfo>::iterator Gitb = m_Graphs.begin();
      std::map<std::string,GraphInfo>::iterator Gite = m_Graphs.end();


      for (Git = Gitb; Git != Gite; ++Git)
      {

        ScriptFile << "set title \"" << (*Git).second.Title << "\" font \",10\"\n";
        ScriptFile << "set key " << (*Git).second.Key << "\n";

        if ((*Git).second.YLabel.empty())
          ScriptFile << "unset ylabel\n";
        else
          ScriptFile << "set ylabel \"" << (*Git).second.YLabel << "\"\n";

        ScriptFile << "plot ";

        std::list<SerieInfo*>::iterator Sit;
        std::list<SerieInfo*>::iterator Sitb = (*Git).second.Series.begin();
        std::list<SerieInfo*>::iterator Site = (*Git).second.Series.end();

        for (Sit = Sitb; Sit != Site; ++Sit)
        {
          std::string SourceDir = m_OutputDir;
          if ((*Sit)->Type == SerieInfo::SERIE_FILE) SourceDir = m_InputDir;

          std::string Label = (*Sit)->Label;
          if (Label.empty())
          {
            if ((*Sit)->Type == SerieInfo::SERIE_FILE) Label = (*Sit)->SourceFile;
            else
            {
              std::string UnitIDStr;
              openfluid::tools::convertValue((*Sit)->UnitID,&UnitIDStr);
              Label= (*Sit)->VarName + " (" + (*Sit)->UnitClass + "#" + UnitIDStr + ")";
            }
          }


          if (Sit != Sitb) ScriptFile << ", ";

          ScriptFile << "\"" << SourceDir << "/" << (*Sit)->SourceFile
                     << "\" using 1:2 with " << (*Sit)->Style << " title \"" << Label << "\"";
        }

        ScriptFile << "\n";

      }

      ScriptFile << "unset multiplot\n";

      ScriptFile.close();

      tryOpenGNUplot();
    }


};


// =====================================================================
// =====================================================================


DEFINE_OBSERVER_CLASS(GNUplotObserver)

