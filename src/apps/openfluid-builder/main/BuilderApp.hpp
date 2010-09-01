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
 \file BuilderApp.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */


#ifndef __BUILDERAPP_HPP__
#define __BUILDERAPP_HPP__


#include <gtkmm.h>
#include <gdlmm.h>
#include <gdl/gdl.h>

#include "BuilderProject.hpp"
#include "ModuleInterface.hpp"


#define MAINWINDOW_UI_FILE "MainWindow.glade"
#define OPENFLUID_LOGO_FILE "openfluid_official.svg"


// =====================================================================
// =====================================================================


class BuilderApp
{
  private:

    Glib::RefPtr<Gtk::Builder> mp_Builder;


    // App top container elements

    Gtk::Window * mp_MainWindow;

    Gtk::MenuBar * mp_MainMenuBar;

    Gtk::Toolbar * mp_MainToolBar;

    Gtk::HBox * mp_MainToolBarContainer;

    Gtk::Box * mp_ProjectContainer;

    Gtk::Container * mp_HomeContainer;


    // Docking elements

    Gdl::Dock * mp_MainDock;

    Gdl::DockItem * mp_DockItemHome;

    Glib::RefPtr<Gdl::DockLayout> mp_LayoutManager;


    std::vector<Glib::RefPtr<Gtk::Action> > m_ProjectActions;

    BuilderProject * mp_Project;


    void createActions();

    void createDock();

    void createDockItemHome();

    void setMainWindowProperties();

    void setAboutBoxProperties();

    void setHomePanelProperties();


    void actionNewEmpty();

    void actionNewFrom();

    void actionOpen();

    void actionSave();

    void actionSaveAs();

    void actionClose();

    void actionQuit();

    void actionCheckProject();

    void actionRun();

    void actionProperties();

    void actionPreferences();

    void actionDefaultLayout(BuilderProject::LayoutType Layout);

    void actionLayoutManager();

    void actionPlugins();

    void actionDoc();

    void actionDemo();

    void actionAbout();


    void createProject(Glib::ustring FolderIn="");

    void deleteProject();

    void onDialogNewFromCbToggled(Gtk::CheckButton* Cb,std::vector<Gtk::CheckButton*> CbDepends);


  public:

    BuilderApp(int argc, char** argv);

    ~BuilderApp();

};


#endif /* __BUILDERAPP_HPP__ */
