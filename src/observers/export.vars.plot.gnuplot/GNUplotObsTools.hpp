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
  @file GNUplotObsTools.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __GNUPLOTOBSTOOLS_HPP__
#define __GNUPLOTOBSTOOLS_HPP__


#include <map>

#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/core/SpatialUnit.hpp>
#include <openfluid/ware/WareParamsTree.hpp>


class SerieInfo
{
  public:

    enum SerieType {SERIE_UNKNOWN, SERIE_VAR, SERIE_FILE};

    SerieType Type;

    std::string SourceFile;

    std::ofstream* FileHandle;

    openfluid::core::VariableName_t VarName;

    openfluid::core::UnitsClass_t UnitsClass;

    openfluid::core::UnitID_t UnitID;

    openfluid::core::SpatialUnit* Unit;

    std::string Label;

    std::string Style;

    std::string Color;

    SerieInfo();

};


// =====================================================================
// =====================================================================


class GraphInfo
{
  public:

    std::string Title;

    std::string Key;

    std::string YLabel;

    std::list<SerieInfo*> Series;

    GraphInfo();

};


// =====================================================================
// =====================================================================


class GNUplotInfo
{
  public:

    std::map<std::string, SerieInfo> Series;

    std::map<std::string, GraphInfo> Graphs;

    std::string Terminal;

    std::string Output;

    bool TryOpenGNUplot;

    bool Persistent;

    GNUplotInfo();
};


// =====================================================================
// =====================================================================


std::vector<std::string> parsePlotFromParamsTree(const openfluid::ware::WareParamsTree& ParamsTree,
                                                 GNUplotInfo& Plot);


#endif /* __GNUPLOTOBSTOOLS_HPP__ */
