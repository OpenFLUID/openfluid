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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/utils/Process.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/ware/PluggableObserver.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/ware/WareParamsTree.hpp>

#include "GNUplotObsTools.hpp"


class GNUplotObserver : public openfluid::ware::PluggableObserver
{
  private:

    GNUplotInfo m_Plot;

    std::string m_OutputDir;
    std::string m_InputDir;


  public:

    GNUplotObserver() : PluggableObserver(), m_OutputDir("")
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
      openfluid::ware::WareParamsTree ParamsTree;

      try
      {
        ParamsTree.setParams(Params);

        parsePlotFromParamsTree(Params, m_Plot);
      }
      catch (openfluid::base::FrameworkException& E)
      {
        OPENFLUID_RaiseError(E.getMessage());
      }


      if (m_Plot.Series.empty())
      {
        OPENFLUID_RaiseError("No serie defined");
      }

      if (m_Plot.Graphs.empty())
      {
        OPENFLUID_RaiseError("No graph defined");
      }


      // Remove of indicative format in terminal
      auto Pos = m_Plot.Terminal.find("(");
      if (Pos != std::string::npos)
      {
        m_Plot.Terminal.erase(Pos - 1);
      }


      // Add of Spatial Unit pointers

      std::map<std::string, SerieInfo>::iterator Sit = m_Plot.Series.begin();
      while (Sit != m_Plot.Series.end())
      {
        std::string SerieID = Sit->first;

        // Check of attributes
        if (Sit->second.Type == SerieInfo::SerieType::SERIE_VAR)
        {
          if (!Sit->second.VarName.empty() && !Sit->second.UnitsClass.empty())
          {
            openfluid::core::SpatialUnit* TmpU;
            TmpU = mp_SpatialData->spatialUnit(Sit->second.UnitsClass, Sit->second.UnitID);
            if (TmpU != nullptr)
            {
              Sit->second.Unit = TmpU;
              Sit->second.SourceFile = SerieID + "_data.gnuplot";
            }
          }

          if (Sit->second.Unit == nullptr)
          {
            Sit->second.Type = SerieInfo::SerieType::SERIE_UNKNOWN;
          }
        }
        else if (Sit->second.Type == SerieInfo::SerieType::SERIE_FILE && Sit->second.SourceFile.empty())
        {
          Sit->second.Type = SerieInfo::SerieType::SERIE_UNKNOWN;
        }


        if (Sit->second.Type == SerieInfo::SerieType::SERIE_UNKNOWN)
        {
          OPENFLUID_LogWarning("Serie " + SerieID + " ignored");
        }

        ++Sit;
      }


      // Remove of incomplete graphs

      std::map<std::string, GraphInfo>::iterator Git = m_Plot.Graphs.begin();
      while (Git != m_Plot.Graphs.end())
      {
        std::string GraphID = Git->first;

        if (Git->second.Series.empty())
        {
          Git = m_Plot.Graphs.erase(Git);
          OPENFLUID_LogWarning("Graph " + GraphID + " ignored");
        }
        else
        {
          ++Git;
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void WriteDataToFiles()
    {
      std::map<std::string, SerieInfo>::iterator Sit;
      std::map<std::string, SerieInfo>::iterator Sitb = m_Plot.Series.begin();
      std::map<std::string, SerieInfo>::iterator Site = m_Plot.Series.end();

      for (Sit = Sitb; Sit != Site; ++Sit)
      {
        if ((*Sit).second.Type == SerieInfo::SerieType::SERIE_VAR)
        {
          openfluid::core::Value* Val =
              (*Sit).second.Unit->variables()->currentValueIfIndex((*Sit).second.VarName,
                                                                   OPENFLUID_GetCurrentTimeIndex());

          if (Val!= nullptr)
          {
            *((*Sit).second.FileHandle) << OPENFLUID_GetCurrentDate().getAsString("%Y-%m-%dT%H:%M:%S");
            *((*Sit).second.FileHandle) << " ";
            if (Val->isSimple() && !Val->isStringValue())
            {
              Val->writeToStream(*((*Sit).second.FileHandle));
            }
            else
            {
              *((*Sit).second.FileHandle) << "NaN";
            }
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
      std::map<std::string, SerieInfo>::iterator Sitb = m_Plot.Series.begin();
      std::map<std::string, SerieInfo>::iterator Site = m_Plot.Series.end();

      for (Sit = Sitb; Sit != Site; ++Sit)
      {
        if ((*Sit).second.Type == SerieInfo::SerieType::SERIE_VAR)
        {
          if ((*Sit).second.FileHandle != nullptr && (*Sit).second.FileHandle->is_open())
          {
            (*Sit).second.FileHandle->close();

            delete (*Sit).second.FileHandle;
            (*Sit).second.FileHandle = nullptr;
          }
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
      std::map<std::string, SerieInfo>::iterator Sitb = m_Plot.Series.begin();
      std::map<std::string, SerieInfo>::iterator Site = m_Plot.Series.end();

      for (Sit = Sitb; Sit != Site; ++Sit)
      {
        if ((*Sit).second.Type == SerieInfo::SerieType::SERIE_VAR)
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
      if (m_Plot.TryOpenGNUplot)
      {
        openfluid::utils::ExternalProgram GNUPlotProgram =
            openfluid::utils::ExternalProgram::getRegisteredProgram(
              openfluid::utils::ExternalProgram::RegisteredPrograms::Gnuplot);

        if (GNUPlotProgram.isFound())
        {          
          std::vector<std::string> Args;

          if (m_Plot.Persistent)
          {
            Args << "-persist";
          }

          Args << openfluid::tools::Path({m_OutputDir,"script.gnuplot"}).toGeneric();

          openfluid::utils::Process::execute(GNUPlotProgram.getFullProgramPath(),Args);
        }
        else
        {
          OPENFLUID_LogWarning("Cannot find GNUPlot");
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void onFinalizedRun()
    {
      CloseFiles();

      std::ofstream ScriptFile(std::string(m_OutputDir+"/script.gnuplot").c_str());

      if (!m_Plot.Terminal.empty())
      {
        ScriptFile << "set terminal " << m_Plot.Terminal << "\n";
        if (!m_Plot.Output.empty())
        {
          ScriptFile << "set output \"" << m_OutputDir << "/" << m_Plot.Output << "\"\n";
        }
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

      if (m_Plot.Graphs.size() > 1)
      {
        Columns = (unsigned int)(std::ceil(std::sqrt(m_Plot.Graphs.size())));
        Rows = (unsigned int)(std::ceil(m_Plot.Graphs.size() / Columns));

        if (Columns*Rows < m_Plot.Graphs.size())
        {
          Rows++;
        }
      }

      ScriptFile << "set multiplot layout " << Rows << "," << Columns << " rowsfirst scale 1,1\n";

      std::map<std::string, GraphInfo>::iterator Git;
      std::map<std::string, GraphInfo>::iterator Gitb = m_Plot.Graphs.begin();
      std::map<std::string, GraphInfo>::iterator Gite = m_Plot.Graphs.end();


      for (Git = Gitb; Git != Gite; ++Git)
      {
        ScriptFile << "set title \"" << (*Git).second.Title << "\" font \",10\"\n";
        ScriptFile << "set key " << (*Git).second.Key << "\n";

        if ((*Git).second.YLabel.empty())
        {
          ScriptFile << "unset ylabel\n";
        }
        else
        {
          ScriptFile << "set ylabel \"" << (*Git).second.YLabel << "\"\n";
        }

        ScriptFile << "plot ";

        std::list<SerieInfo*>::iterator Sit;
        std::list<SerieInfo*>::iterator Sitb = (*Git).second.Series.begin();
        std::list<SerieInfo*>::iterator Site = (*Git).second.Series.end();

        for (Sit = Sitb; Sit != Site; ++Sit)
        {
          std::string SourceDir = m_OutputDir;
          if ((*Sit)->Type == SerieInfo::SerieType::SERIE_FILE)
          {
            SourceDir = m_InputDir;
          }

          std::string Label = (*Sit)->Label;
          if (Label.empty())
          {
            if ((*Sit)->Type == SerieInfo::SerieType::SERIE_FILE)
            {
              Label = (*Sit)->SourceFile;
            }
            else
            {
              std::string UnitIDStr = std::to_string((*Sit)->UnitID);
              Label= (*Sit)->VarName + " (" + (*Sit)->UnitsClass + "#" + UnitIDStr + ")";
            }
          }


          if (Sit != Sitb)
          {
            ScriptFile << ", ";
          }

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
