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

#include <openfluid/io.hpp>

#include "BuilderProject.hpp"


// =====================================================================
// =====================================================================


BuilderProject::BuilderProject()
{
      mp_ExecMsgs = openfluid::base::ExecutionMessages::getInstance();
      mp_RunEnv = openfluid::base::RuntimeEnvironment::getInstance();
      mp_Engine = new openfluid::machine::Engine(new openfluid::machine::MachineListener(),
          new openfluid::io::IOListener());

      addModule(new ModelModule(),"model");
      addModule(new DomainModule(),"domain");
      addModule(new SimulationModule(),"simulation");
      addModule(new ResultsModule(),"results");

}


// =====================================================================
// =====================================================================


BuilderProject::~BuilderProject()
{
  for(DockItemsPtrByNameMap_t::const_iterator it = m_DockItems.begin() ; it != m_DockItems.end() ; ++it)
  {
    it->second->hide_item();
    //  it->second->unbind();
    delete it->second;
  }

  for(ModulesPtrByNameMap_t::const_iterator it = m_Modules.begin() ; it != m_Modules.end() ; ++it)
  {
    delete it->second;
  }
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


void BuilderProject::reorderDockItems()
{
  m_DockItems["simulation"]->dock_to(*(m_DockItems["model"]), Gdl::DOCK_BOTTOM);
  m_DockItems["results"]->dock_to(*(m_DockItems["simulation"]), Gdl::DOCK_BOTTOM);
  m_DockItems["domain"]->dock_to(*(m_DockItems["model"]), Gdl::DOCK_RIGHT);

  m_DockItems["simulation"]->iconify_item();
  m_DockItems["results"]->iconify_item();
}


// =====================================================================
// =====================================================================


void BuilderProject::actionDefaultLayout()
{
  std::cout << "ici default layout de Project" << std::endl;
}
