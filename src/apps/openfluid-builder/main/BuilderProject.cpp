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
  \file BuilderProject.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */
#include <boost/date_time/posix_time/posix_time.hpp>

#include <openfluid/io.hpp>

#include "BuilderProject.hpp"


// =====================================================================
// =====================================================================


BuilderProject::BuilderProject(Glib::ustring FolderIn)
//  : openfluid::machine::Engine(new openfluid::machine::MachineListener(),
//        new openfluid::io::IOListener())
{
  if(FolderIn!="")
  {
//    mp_ExecMsgs = openfluid::base::ExecutionMessages::getInstance();

//    mp_RunEnv = openfluid::base::RuntimeEnvironment::getInstance();
//    mp_Listener = new openfluid::machine::MachineListener();
//
//    mp_Engine = new openfluid::machine::Engine(mp_Listener,
//        new openfluid::io::IOListener());
//
//    mp_RunEnv->setInputDir(FolderIn);
//    mp_RunEnv->setOutputDir(Glib::ustring::compose("%1/OUT",FolderIn));
//
//
//    std::cout << mp_RunEnv->getInputDir() << std::endl;
//    std::cout << mp_RunEnv->getOutputDir() << std::endl;


    //  std::vector<std::string> FunctionsPaths = openfluid::base::RuntimeEnvironment::getInstance()->getPluginsPaths();
    //  unsigned int i;
    //
    //  std::cout << "Input dir: " << openfluid::base::RuntimeEnvironment::getInstance()->getInputDir() << std::endl;
    //  if (openfluid::base::RuntimeEnvironment::getInstance()->isWriteResults() || openfluid::base::RuntimeEnvironment::getInstance()->isWriteSimReport()) std::cout << "Output dir: " << openfluid::base::RuntimeEnvironment::getInstance()->getOutputDir() << std::endl;
    //  std::cout << "Functions search path(s):" << std::endl;
    //  for (i=0;i<FunctionsPaths.size();i++) std::cout << " #" << (i+1) << " " << FunctionsPaths[i] << std::endl;
    //  std::cout << "Temp dir: " << openfluid::base::RuntimeEnvironment::getInstance()->getTempDir() << std::endl;

//    std::cout << "* Loading data... " << std::endl;
//    mp_Engine->loadData();
//
//    std::cout << "* Processing run configuration... " << std::endl;
//    mp_Engine->processRunConfiguration();
//
//    std::cout << "* Building spatial domain... " << std::endl;
//    mp_Engine->buildSpatialDomain();
//
//    std::cout << "* Building model... " << std::endl;
//    mp_Engine->buildModel();

  }
    addModule(new ModelModule(),"model");
    addModule(new DomainModule(),"domain");
    addModule(new SimulationModule(),"simulation");
    addModule(new ResultsModule(),"results");

    m_ProjectChecked = false;
}


// =====================================================================
// =====================================================================


BuilderProject::~BuilderProject()
{
  for(DockItemsPtrByNameMap_t::const_iterator it = m_DockItems.begin() ; it != m_DockItems.end() ; ++it)
  {
    it->second->hide_item();
    delete it->second;
  }

  for(ModulesPtrByNameMap_t::const_iterator it = m_Modules.begin() ; it != m_Modules.end() ; ++it)
  {
    delete it->second;
  }

//  delete mp_RunEnv;
//  delete mp_Listener;
//  delete mp_Engine;
//  delete mp_ExecMsgs;

}


// =====================================================================
// =====================================================================


void BuilderProject::addModule(ModuleInterface * Module, Glib::ustring ModuleName)
{
  // create DockItem
  Gtk::Container * Container = Module->getContainer();
  const Glib::ustring & RefModuleName = Module->getModuleName();
  const Glib::ustring & RefModuleLongName = "  " + Module->getModuleLongName();
  const Gtk::StockID & RefStockId = *(Module->getStockId());
  Gdl::DockItem * DockItem = new Gdl::DockItem(RefModuleName,
                                  RefModuleLongName,
                                  RefStockId,
                                  Gdl::DOCK_ITEM_BEH_NORMAL | Gdl::DOCK_ITEM_BEH_CANT_CLOSE);
  DockItem->add(*Container);


   //TODO: check if moduleName exists before insert
  m_DockItems[ModuleName] = DockItem;

  m_Modules[ModuleName] = Module;

}


// =====================================================================
// =====================================================================


void BuilderProject::actionDefaultLayout(LayoutType Layout)
{
    // Default layout, base for others

      m_DockItems["simulation"]->dock_to(*(m_DockItems["model"]), Gdl::DOCK_BOTTOM);
      m_DockItems["results"]->dock_to(*(m_DockItems["simulation"]), Gdl::DOCK_BOTTOM);
      m_DockItems["domain"]->dock_to(*(m_DockItems["model"]), Gdl::DOCK_RIGHT);

  switch(Layout)
  {
    case PreSimulation:
      m_DockItems["simulation"]->iconify_item();
      m_DockItems["results"]->iconify_item();
      break;
    case PostSimulation:
      m_DockItems["model"]->iconify_item();
      m_DockItems["domain"]->iconify_item();
      m_DockItems["simulation"]->iconify_item();
      break;
    case AllTabbed:
      m_DockItems["domain"]->dock_to(*(m_DockItems["model"]), Gdl::DOCK_CENTER);
      m_DockItems["simulation"]->dock_to(*(m_DockItems["model"]), Gdl::DOCK_CENTER);
      m_DockItems["results"]->dock_to(*(m_DockItems["model"]), Gdl::DOCK_CENTER);
      break;
    default:;
  }
}


// =====================================================================
// =====================================================================


void BuilderProject::actionCheckProject()
{
  // !! delete existing variables from previous checkConsistency !
//  try
//  {
//    std::cout << "* Initializing parameters... " << std::endl;
//    mp_Engine->initParams();
//
//    std::cout << "* Preparing data... " << std::endl;
//    mp_Engine->prepareData();
//
//    std::cout << "* Checking consistency... " << std::endl;
//    mp_Engine->checkConsistency();
//
//    m_ProjectChecked = true;
//  }
//  catch(openfluid::base::OFException& E)
//  {
//    std::cerr << E.what() << std::endl;
//  }
}


// =====================================================================
// =====================================================================


void BuilderProject::actionRun()
{
//  if(!m_ProjectChecked)
//    actionCheckProject();
//
//
//  // !! delete existing variable values from previous run !
//
//
//  std::cout << "Simulation from " << mp_Engine->getSimulationInfo()->getStartTime().getAsISOString()
//                    << " to " << mp_Engine->getSimulationInfo()->getEndTime().getAsISOString() << std::endl
//                    << "         -> " <<  (mp_Engine->getSimulationInfo()->getStepsCount()) << " time steps of " << mp_Engine->getSimulationInfo()->getTimeStep() << " seconds" << std::endl;
//
//  std::cout << std::endl;
//
//  if (openfluid::base::RuntimeEnvironment::getInstance()->isUserValuesBufferSize())
//    std::cout << "Buffers for variables set to " << openfluid::base::RuntimeEnvironment::getInstance()->getValuesBufferSize() << " time steps" <<  std::endl;
//  else std::cout << "Buffers for variables set to full simulation" << std::endl;
//  std::cout << "Buffers for output files set to " << openfluid::base::RuntimeEnvironment::getInstance()->getFilesBufferSize() << " bytes" << std::endl;
//
//  std::cout << std::endl;
//
//  std::cout.flush();
//
//  std::cout << std::endl << "**** Running simulation ****" << std::endl;
//  std::cout.flush();
//
//  boost::posix_time::ptime m_FullStartTime = boost::posix_time::microsec_clock::local_time();
//
//  boost::posix_time::ptime m_EffectiveStartTime = boost::posix_time::microsec_clock::local_time();
//  mp_Engine->run();
//  boost::posix_time::ptime m_EffectiveEndTime = boost::posix_time::microsec_clock::local_time();
//  std::cout << "**** Simulation completed ****" << std::endl << std::endl;std::cout << std::endl;
//  std::cout.flush();
//
//  openfluid::base::RuntimeEnvironment::getInstance()->setEffectiveSimulationDuration(m_EffectiveEndTime-m_EffectiveStartTime);
//
//
//  if (openfluid::base::RuntimeEnvironment::getInstance()->isWriteSimReport())
//  {
//    std::cout << "* Saving simulation report... "; std::cout.flush();
//    mp_Engine->saveReports();
//    std::cout << "[Done]" << std::endl; std::cout.flush();
////    mp_ExecMsgs->resetWarningFlag();
//  }
//
//
//  boost::posix_time::ptime m_FullEndTime = boost::posix_time::microsec_clock::local_time();
//
//  if (openfluid::base::RuntimeEnvironment::getInstance()->isWriteResults() || openfluid::base::RuntimeEnvironment::getInstance()->isWriteSimReport()) std::cout << std::endl;
//
//  boost::posix_time::time_duration FullSimDuration = m_FullEndTime - m_FullStartTime;
//
//  std::cout << std::endl;
//
//  std::cout << "Simulation run time: " << boost::posix_time::to_simple_string(openfluid::base::RuntimeEnvironment::getInstance()->getEffectiveSimulationDuration()) << std::endl;
//  std::cout << "     Total run time: " << boost::posix_time::to_simple_string(FullSimDuration) << std::endl;
//  std::cout << std::endl;
}

