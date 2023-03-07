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
  @file GNUplotObsTools.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/tools/StringHelpers.hpp>

#include "GNUplotObsTools.hpp"


SerieInfo::SerieInfo() :
  Type(SerieType::SERIE_UNKNOWN), SourceFile(""), FileHandle(nullptr),
  VarName(""), UnitsClass(""), UnitID(1), Unit(nullptr),
  Label(""), Style("line"), Color("")
{

}


// =====================================================================
// =====================================================================


GraphInfo::GraphInfo() :
  Title(""), Key("default"), YLabel("")
{

}


// =====================================================================
// =====================================================================


GNUplotInfo::GNUplotInfo() :
  Terminal(""), Output(""),
  TryOpenGNUplot(false), Persistent(false)
{

}


// =====================================================================
// =====================================================================


std::vector<std::string> parsePlotFromParamsTree(const openfluid::ware::WareParamsTree& ParamsTree,
                                                 GNUplotInfo& Plot)
{
  std::vector<std::string> Messages;

  Plot.Series.clear();
  Plot.Graphs.clear();


  ParamsTree.root().getChildValue("tryopengnuplot", Plot.TryOpenGNUplot).toBoolean(Plot.TryOpenGNUplot);
  ParamsTree.root().getChildValue("persistent", Plot.Persistent).toBoolean(Plot.Persistent);
  Plot.Terminal = ParamsTree.root().getChildValue("terminal", Plot.Terminal);
  Plot.Output = ParamsTree.root().getChildValue("output", Plot.Output);


  if (ParamsTree.root().hasChild("serie"))
  {
    for (auto& Serie : ParamsTree.root().child("serie"))
    {
      std::string SerieID = Serie.first;

      SerieInfo SInfo;

      SInfo.Label = Serie.second.getChildValue("label","");
      SInfo.Style = Serie.second.getChildValue("style","line");
      SInfo.Color = Serie.second.getChildValue("color","");

      if (Serie.second.hasChild("var"))
      {
        SInfo.VarName = Serie.second.getChildValue("var","");
        SInfo.UnitsClass = Serie.second.getChildValue("unitsclass","");
        if (SInfo.UnitsClass == "")
        {
          // search for deprecated "unitclass" parameter
          SInfo.UnitsClass = Serie.second.getChildValue("unitclass","");
        }

        std::string UnitIDStr = Serie.second.getChildValue("unitID","");
        openfluid::tools::toNumeric(UnitIDStr,SInfo.UnitID);

        SInfo.Type = SerieInfo::SerieType::SERIE_VAR;
      }
      else if (Serie.second.hasChild("sourcefile"))
      {
        SInfo.SourceFile = Serie.second.getChildValue("sourcefile","");
        SInfo.Type = SerieInfo::SerieType::SERIE_FILE;
      }

      Plot.Series[SerieID] = SInfo;
    }
  }

  if (ParamsTree.root().hasChild("graph"))
  {
    for (auto& Graph : ParamsTree.root().child("graph"))
    {
      std::string GraphID = Graph.first;

      GraphInfo GInfo;

      GInfo.Title = Graph.second.getChildValue("title", GraphID);
      GInfo.Key = Graph.second.getChildValue("key", "default");
      GInfo.YLabel = Graph.second.getChildValue("ylabel", "");

      std::vector<std::string> SeriesStr =
          openfluid::tools::split(Graph.second.getChildValue("series",""), ";");

      for (std::vector<std::string>::const_iterator it = SeriesStr.begin(); it != SeriesStr.end(); ++it)
      {
        if (Plot.Series.find(*it) != Plot.Series.end())
        {
          GInfo.Series.push_back(&(Plot.Series[*it]));
        }
      }

      Plot.Graphs[GraphID] = GInfo;
    }
  }

  return Messages;
}
