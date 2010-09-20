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
 \file BuilderProject.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */


#ifndef __BUILDERPROJECT_HPP__
#define __BUILDERPROJECT_HPP__


#include <gtkmm.h>
#include <gdl/gdl.h>

#include <openfluid/base.hpp>
#include <openfluid/machine.hpp>
//#include <openfluid/config.hpp>

#include "ModuleInterface.hpp"
#include "DomainModule.hpp"
#include "ModelModule.hpp"
#include "SimulationModule.hpp"
#include "ResultsModule.hpp"


// =====================================================================
// =====================================================================


class BuilderProject// : public openfluid::machine::Engine
{
  public:

    enum LayoutType { AllTabbed, PreSimulation, PostSimulation };

    typedef std::map<Glib::ustring,ModuleInterface *> ModulesPtrByNameMap_t;

    typedef std::map<Glib::ustring,GtkWidget *> DockItemsPtrByNameMap_t;


  private:

    openfluid::base::RuntimeEnvironment * mp_RunEnv;

    openfluid::machine::SimulationBlob m_SimBlob;

//    openfluid::base::ExecutionMessages * mp_ExecMsgs;

    openfluid::machine::Engine * mp_Engine;

    openfluid::machine::MachineListener * mp_Listener;

    openfluid::io::IOListener * mp_IOListener;

    openfluid::machine::ModelInstance * mp_Model;


    ModulesPtrByNameMap_t m_Modules;

    DockItemsPtrByNameMap_t m_DockItems;


    LayoutType m_LayoutType;


    bool m_ProjectChecked;


    void addModule(ModuleInterface *, Glib::ustring ModuleName);


  public:

    BuilderProject(Glib::ustring FolderIn);

    ~BuilderProject();

    GtkWidget * getTopDockItem()
        { return m_DockItems["model"]; }

    ModulesPtrByNameMap_t getModules()
        { return m_Modules; }

    void actionDefaultLayout(LayoutType Layout);

    void actionCheckProject();

    void actionRun();

};

#endif /* __BUILDERPROJECT_HPP__ */
