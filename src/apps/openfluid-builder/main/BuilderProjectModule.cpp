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
 \file BuilderProjectModule.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "BuilderProjectModule.hpp"

#include <glibmm/i18n.h>

#include "EngineProjectFactory.hpp"
#include "BuilderModuleFactory.hpp"


BuilderProjectModule::BuilderProjectModule(std::string FolderIn) :
  mp_EngineProject(0), mp_ModelModule(0), mp_DomainModule(0),
      mp_SimulationModule(0), mp_ResultsModule(0)
{
  mp_EngineProject = EngineProjectFactory::createEngineProject(FolderIn);

  if (mp_EngineProject)
  {
    mp_ModelModule = BuilderModuleFactory::createModelModule(
        *(mp_EngineProject->getModelInstance()));

    mp_DomainModule = BuilderModuleFactory::createDomainModule(
        mp_EngineProject->getCoreRepository());

    mp_SimulationModule = BuilderModuleFactory::createSimulationModule(
        mp_EngineProject->getRunDescriptor(),
        mp_EngineProject->getOutputDescriptor(),
        mp_EngineProject->getCoreRepository(),
        *mp_EngineProject->getModelInstance());

    mp_ResultsModule = BuilderModuleFactory::createResultsModule(
        mp_EngineProject->getRunDescriptor(),
        mp_EngineProject->getOutputDescriptor(),
        mp_EngineProject->getCoreRepository(),
        *mp_EngineProject->getModelInstance());
  }
}
BuilderProjectModule::~BuilderProjectModule()
{
  delete mp_ModelModule;
  delete mp_DomainModule;
  delete mp_SimulationModule;
  delete mp_ResultsModule;
  delete mp_EngineProject;
}
void BuilderProjectModule::initialize()
{
  if (mp_ModelModule)
    mp_ModelModule->initialize();
  if (mp_DomainModule)
    mp_DomainModule->initialize();
  if (mp_SimulationModule)
    mp_SimulationModule->initialize();
  if (mp_ResultsModule)
    mp_ResultsModule->initialize();
}

void BuilderProjectModule::checkAsked()
{
  mp_EngineProject->check();
}
void BuilderProjectModule::runAsked()
{
  mp_EngineProject->run();
  mp_ResultsModule->initialize();
}

BuilderProjectModulePlainGtk::BuilderProjectModulePlainGtk(std::string FolderIn) :
  BuilderProjectModule(FolderIn)
{
}
void BuilderProjectModulePlainGtk::compose()
{
  mp_Notebook = Gtk::manage(new Gtk::Notebook());
  mp_Notebook->set_visible(true);

  if (mp_ModelModule)
    mp_Notebook->append_page(*mp_ModelModule->composeAndGetAsWidget(), _(
        "Model"));
  if (mp_DomainModule)
    mp_Notebook->append_page(*mp_DomainModule->composeAndGetAsWidget(), _(
        "Domain"));
  if (mp_SimulationModule)
    mp_Notebook->append_page(*mp_SimulationModule->composeAndGetAsWidget(),
        _("Simulation"));
  if (mp_ResultsModule)
    mp_Notebook->append_page(*mp_ResultsModule->composeAndGetAsWidget(),
        _("Results"));
}
Gtk::Widget* BuilderProjectModulePlainGtk::asWidget()
{
  if (mp_Notebook)
    return mp_Notebook;
  throw std::logic_error(
      "BuilderProjectModule : you try to get a widget from a non yet composed module.");
}

