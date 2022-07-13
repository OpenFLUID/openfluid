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
  @file ReportTasks.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/machine/ObserverRegistry.hpp>
#include <openfluid/machine/WareRegistrySerializer.hpp>
#include <openfluid/base/Environment.hpp>

#include "ReportTasks.hpp"


int ReportTasks::process()
{

    std::string Waretype;

    if (m_ExtraArgs.empty() || (m_ExtraArgs.at(0) != "simulators" && m_ExtraArgs.at(0) != "observers"))
    {
      return error("type of wares is missing for reporting");
    }
    else
    {
      Waretype = m_ExtraArgs.at(0);
    }

    std::string MatchStr;
    if (m_ExtraArgs.size() > 1)
    {
      MatchStr = m_ExtraArgs.at(1);
    }

    bool Detailed = !m_Cmd.isOptionActive("list");
    bool WithErrors = m_Cmd.isOptionActive("with-errors");
    std::string Format = "text";

    if (m_Cmd.isOptionActive("format"))
    {
      Format = m_Cmd.getOptionValue("format");
    }

    if (Format != "text" && Format != "json")
    {
      return error("unknown format");
    }

    if (Waretype == "simulators")
    {
      if (m_Cmd.isOptionActive("simulators-paths"))
      {
        openfluid::base::Environment::addExtraSimulatorsDirs(m_Cmd.getOptionValue("simulators-paths"));
      }

      auto Reg = openfluid::machine::SimulatorRegistry::instance();
      Reg->clear();
      Reg->discoverWares(false);

      const openfluid::machine::WareRegistrySerializer<openfluid::ware::SimulatorSignature> RegSzr(Reg);
      RegSzr.writeToStream(std::cout,Format,Detailed,WithErrors);
      std::cout.flush();

      return 0;
    }
    else if (Waretype == "observers")
    {
      if (m_Cmd.isOptionActive("observers-paths"))
      {
        openfluid::base::Environment::addExtraObserversDirs(m_Cmd.getOptionValue("observers-paths"));
      }

      auto Reg = openfluid::machine::ObserverRegistry::instance();
      Reg->clearWares();
      Reg->discoverWares();

      const openfluid::machine::WareRegistrySerializer<openfluid::ware::ObserverSignature> RegSzr(Reg);
      RegSzr.writeToStream(std::cout,Format,Detailed,WithErrors);
      std::cout.flush();

      return 0;
    }

    return error();
}
