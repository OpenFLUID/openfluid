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
  @file DataTasks.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#include <chrono>
#include <ctime>

#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/fluidx/FluidXIO.hpp>
#include <openfluid/base/IOListener.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/base/ExamplesManager.hpp>
#include <openfluid/config.hpp>

#include "DataTasks.hpp"


int DataTasks::generateData(const std::string Path, bool WithSample)
{
  auto PathObj = openfluid::tools::Path(Path);

  try
  {
    if (PathObj.makeDirectory())
    {
      const std::time_t NowT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      const auto NowTM = std::gmtime(&NowT);

      openfluid::base::IOListener Listener;
      openfluid::fluidx::FluidXDescriptor FXDesc;
      openfluid::core::DateTime DT(NowTM->tm_year+1900,NowTM->tm_mon+1,NowTM->tm_mday,12,0,0);

      FXDesc.runConfiguration().setBeginDate(DT);
      DT.addSeconds(3*86400);
      FXDesc.runConfiguration().setEndDate(DT);
      FXDesc.runConfiguration().setDeltaT(600);
      FXDesc.runConfiguration().setFilled(true);

      if (WithSample)
      {
        // --- domain

        openfluid::fluidx::SpatialUnitDescriptor UnitDesc;

        //addAttribute

        // UnitsB 
        UnitDesc.setUnitsClass("UnitsB");

        UnitDesc.toSpatialUnits().clear();
        UnitDesc.setID(3);
        UnitDesc.setProcessOrder(4);
        FXDesc.spatialDomain().addUnit(UnitDesc);

        UnitDesc.toSpatialUnits().clear();
        UnitDesc.setID(2);
        UnitDesc.setProcessOrder(2);
        UnitDesc.toSpatialUnits().push_back({"UnitsB",3});
        FXDesc.spatialDomain().addUnit(UnitDesc);

        UnitDesc.toSpatialUnits().clear();
        UnitDesc.setID(1);
        UnitDesc.toSpatialUnits().push_back({"UnitsB",2});
        FXDesc.spatialDomain().addUnit(UnitDesc);

        FXDesc.spatialDomain().addAttribute("UnitsB","length","1.0");
        FXDesc.spatialDomain().addAttribute("UnitsB","slope","0.001");
        FXDesc.spatialDomain().setAttribute("UnitsB",1,"length","7.5");
        FXDesc.spatialDomain().setAttribute("UnitsB",1,"slope","0.1");
        FXDesc.spatialDomain().setAttribute("UnitsB",2,"length","25.3");
        FXDesc.spatialDomain().setAttribute("UnitsB",2,"slope","0.02");
        FXDesc.spatialDomain().setAttribute("UnitsB",3,"slope","0.01");


        // UnitsA
        UnitDesc.setUnitsClass("UnitsA");
        
        UnitDesc.setID(5);
        UnitDesc.toSpatialUnits().clear();
        UnitDesc.setProcessOrder(2);
        FXDesc.spatialDomain().addUnit(UnitDesc);

        UnitDesc.toSpatialUnits().clear();
        UnitDesc.setID(1);
        UnitDesc.setProcessOrder(1);
        UnitDesc.toSpatialUnits().push_back({"UnitsA",5});
        FXDesc.spatialDomain().addUnit(UnitDesc);
        
        UnitDesc.toSpatialUnits().clear();
        UnitDesc.setID(17);
        UnitDesc.setProcessOrder(1);
        UnitDesc.toSpatialUnits().push_back({"UnitsA",5});
        UnitDesc.toSpatialUnits().push_back({"UnitsB",2});
        FXDesc.spatialDomain().addUnit(UnitDesc);


        FXDesc.spatialDomain().addAttribute("UnitsA","area","1.0");
        FXDesc.spatialDomain().addAttribute("UnitsA","slope","0.01");
        FXDesc.spatialDomain().setAttribute("UnitsA",17,"area","789");
        FXDesc.spatialDomain().setAttribute("UnitsA",17,"slope","0.07");
        FXDesc.spatialDomain().setAttribute("UnitsA",1,"area","97.5");
        FXDesc.spatialDomain().setAttribute("UnitsA",5,"area","400");


        // --- model

        auto Gen = new openfluid::fluidx::GeneratorDescriptor(
                    "var.example","UnitsA",openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM,1
                  );
        Gen->setParameter("min","0");
        Gen->setParameter("max","1");
        FXDesc.model().appendItem(Gen);

        
        // --- monitoring
      
        auto Obs = new openfluid::fluidx::ObserverDescriptor("export.vars.files.csv");
        Obs->setEnabled(false);

        Obs->setParameter("format.auto.colsep",";");
        Obs->setParameter("format.auto.date","ISO");
        Obs->setParameter("format.auto.header","colnames-as-comment");

        for (const std::string& CName : {"UnitsA","UnitsB"})
        {
          Obs->setParameter("set.auto"+CName+".format","auto");
          Obs->setParameter("set.auto"+CName+".unitsclass",CName);
          Obs->setParameter("set.auto"+CName+".unitsIDs","*");
          Obs->setParameter("set.auto"+CName+".vars","*");
        }

        FXDesc.monitoring().appendItem(Obs);
      }


      openfluid::fluidx::FluidXIO(&Listener).writeToManyFiles(FXDesc,PathObj.toGeneric());

      return 0;
    }
  }
  catch (openfluid::base::FrameworkException&)
  { }

  return error("problem occured during sample data generation");
}


// =====================================================================
// =====================================================================


int DataTasks::processCreateData()
{
  if (!m_Cmd.isOptionActive("name"))
  {
    return error("missing data name");
  }

  if (!m_Cmd.isOptionActive("type"))
  {
    return error("missing data type");
  }

  const auto& Name = m_Cmd.getOptionValue("name");

  if (!openfluid::tools::isValidDatasetName(Name))
  {
    return error("invalid data name");
  }

  const auto& TypeStr = m_Cmd.getOptionValue("type");
  
  const auto& ParentPath = (m_Cmd.getOptionValue("parent-path").empty() ? openfluid::tools::Filesystem::currentPath() : 
                                                                          m_Cmd.getOptionValue("parent-path"));

  if (TypeStr == "dataset")
  {
    return generateData(openfluid::tools::Filesystem::joinPath({ParentPath,Name}),
                        m_Cmd.isOptionActive("with-sample-data"));
  }
  else if (TypeStr == "project")
  {
    auto PrjMan = openfluid::base::RunContextManager::instance();
    if (PrjMan->createProject(openfluid::tools::Filesystem::joinPath({ParentPath,Name}), Name,"","",false))
    {
      return generateData(PrjMan->getInputDir(),m_Cmd.isOptionActive("with-sample-data"));
    }

    return error("cannot create project");
  }
  else
  {
    return error("unknown data type");
  }

  return error();
}


// =====================================================================
// =====================================================================


int DataTasks::processInstallExamples()
{
  std::string ResPath = m_Cmd.getOptionValue("resources-path");
  std::string InstPath = m_Cmd.getOptionValue("install-path");

  if (m_Cmd.isOptionActive("simulators-only"))
  {
    auto OptStr = m_Cmd.getOptionValue("simulators-only");
    if (!OptStr.empty() && OptStr != "*")
    {
      auto Selection = openfluid::tools::splitString(OptStr,",");
      bool AllIsOK = true;
      for (const auto& S : Selection)
      {
        AllIsOK += openfluid::base::ExamplesManager::installSimulator(S,ResPath,InstPath,m_Cmd.isOptionActive("force"));
      }
      return (AllIsOK ? 0 : error("problems occurred during installation"));
    }
    else
    {
      return (openfluid::base::ExamplesManager::installAllSimulators(ResPath,InstPath,m_Cmd.isOptionActive("force")) ?
              0 : error("problems occurred during installation"));
    }
  }
  
  if (m_Cmd.isOptionActive("projects-only"))
  {
    auto OptStr = m_Cmd.getOptionValue("projects-only");
    if (!OptStr.empty() && OptStr != "*")
    {
      auto Selection = openfluid::tools::splitString(OptStr,",");
      bool AllIsOK = true;
      for (const auto& S : Selection)
      {
        AllIsOK += openfluid::base::ExamplesManager::installProject(S,ResPath,InstPath,m_Cmd.isOptionActive("force"));
      }
      return (AllIsOK ? 0 : error("problems occurred during installation"));
    }
    else
    {
      return (openfluid::base::ExamplesManager::installAllProjects(ResPath,InstPath,m_Cmd.isOptionActive("force")) ?
              0 : error("problems occurred during installation"));
    }
  }
  
  if (!m_Cmd.isOptionActive("simulators-only") && !m_Cmd.isOptionActive("projects-only"))
  {
    return (openfluid::base::ExamplesManager::installAll(ResPath,InstPath,m_Cmd.isOptionActive("force")) ?
            0 : error("problems occurred during installation"));
  }

  return error();
}


// =====================================================================
// =====================================================================


int DataTasks::process()
{
  if (m_Cmd.getName() == "create-data")
  {
    return processCreateData();
  }
  else if (m_Cmd.getName() == "install-examples")
  {
    return processInstallExamples();
  }
  
  return unknownCommand();
}
