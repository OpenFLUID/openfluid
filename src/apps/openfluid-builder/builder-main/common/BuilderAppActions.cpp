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
 \file BuilderAppActions.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "BuilderAppActions.hpp"

#include <glibmm/i18n.h>
#include <iostream>

#include "BuilderGraphicsHelper.hpp"


// =====================================================================
// =====================================================================


void BuilderAppActions::createAppUiXml()
{
  m_AppUiXml = "<ui>"
    "  <menubar name='MenuBar'>"
    "    <menu action='FileMenu'>"
    "      <menu action='FileNew'>"
    "        <menuitem action='FileNewEmpty'/>"
    "        <menuitem action='FileNewFrom'/>"
    "      </menu>"
    "      <menuitem action='FileOpen'/>"
    "      <separator/>"
    "<placeholder name='ProjectFilePlaceholder' />"
    "      <menuitem action='FileQuit'/>"
    "    </menu>"
    "    <menu action='EditMenu'>"
    "      <menuitem action='EditPreferences'/>"
    "      <menuitem action='EditExtensions'/>"
    "    </menu>"
    "<placeholder name='ProjectPlaceholder' />"
    "<placeholder name='ModulesPlaceholder' />"
    "    <menu action='ViewMenu'>"
    "    </menu>"
    "    <menu action='HelpMenu'>"
    "      <menuitem action='HelpDemo'/>"
    "      <menuitem action='HelpDoc'/>"
    "      <separator/>"
    "      <menuitem action='HelpAbout'/>"
    "    </menu>"
    "  </menubar>"
    "  <toolbar  name='ToolBar'>"
    "    <toolitem action='FileNewEmpty'/>"
    "    <toolitem action='FileNewFrom'/>"
    "    <toolitem action='FileOpen'/>"
    "    <toolitem action='FileSave'/>"
    "      <separator/>"
    "<placeholder name='ProjectPlaceholder' />"
    "      <separator/>"
    "  </toolbar>"
    "</ui>";
}

// =====================================================================
// =====================================================================


void BuilderAppActions::createProjectUiXml()
{
  m_ProjectUiXml = "<ui>"
    "  <menubar name='MenuBar'>"
    "  <menu action='FileMenu'>"
    "<placeholder name='ProjectFilePlaceholder'>"
    "      <menuitem action='FileSave'/>"
    "      <menuitem action='FileSaveAs'/>"
    "      <separator/>"
    "      <menuitem action='FileClose'/>"
    "</placeholder>"
    "</menu>"
    "<placeholder name='ProjectPlaceholder'>"
    "    <menu action='ProjectMenu'>"
    "      <menuitem action='ProjectCheck'/>"
    "      <menuitem action='ProjectRun'/>"
    "      <separator/>"
    "      <menuitem action='ProjectProperties'/>"
    "    </menu>"
    "</placeholder>"
    "  </menubar>"
    "  <toolbar  name='ToolBar'>"
    "<placeholder name='ProjectPlaceholder'>"
    "    <toolitem action='ProjectCheck'/>"
    "    <toolitem action='ProjectRun'/>"
    "</placeholder>"
    "  </toolbar>"
    "</ui>";
}


// =====================================================================
// =====================================================================


void BuilderAppActions::createAppActionGroup()
{
  mref_AppActionGroup = Gtk::ActionGroup::create();

  //File|New sub-menu
  mref_AppActionGroup->add(Gtk::Action::create("FileNewEmpty", Gtk::Stock::NEW,
      _("Empty"), _("Create new empty project")));
  mref_AppActionGroup->add(Gtk::Action::create("FileNewFrom",
      *BuilderGraphicsHelper::createBuilderIconStockId("document-new-derived.png",
          "builder-new-derived"), _("From..."),
      _("Create new project from an existing one")));
  //File menu
  mref_AppActionGroup->add(Gtk::Action::create("FileMenu", _("File")));
  mref_AppActionGroup->add(Gtk::Action::create("FileNew", Gtk::Stock::NEW));
  mref_AppActionGroup->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN,
      _("Open..."), _("Open an existing project")));
  mref_AppActionGroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT));

  //Edit menu
  mref_AppActionGroup->add(Gtk::Action::create("EditMenu", _("Edit")));
  mref_AppActionGroup->add(Gtk::Action::create("EditPreferences",
      Gtk::Stock::PREFERENCES));
  mref_AppActionGroup->add(Gtk::Action::create("EditExtensions",
      _("Extensions")));

  //View menu
  mref_AppActionGroup->add(Gtk::Action::create("ViewMenu", _("View")));

  //Help menu
  mref_AppActionGroup->add(Gtk::Action::create("HelpMenu", Gtk::Stock::HELP));
  mref_AppActionGroup->add(Gtk::Action::create("HelpDemo", _("Demo")));
  mref_AppActionGroup->add(Gtk::Action::create("HelpDoc", _("Doc")));
  mref_AppActionGroup->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT));
  mref_AppActionGroup->add(Gtk::Action::create("HelpMarket",
      *BuilderGraphicsHelper::createBuilderIconStockId("openfluid_in_cart.png",
          "openfluid_in_cart"), _("OpenFLUID Market..."),
      _("Access to OpenFLUID Market")));
}


// =====================================================================
// =====================================================================


void BuilderAppActions::createProjectActionGroup()
{
  mref_ProjectActionGroup = Gtk::ActionGroup::create();

  //File menu
  mref_ProjectActionGroup->add(Gtk::Action::create("FileSave",
      Gtk::Stock::SAVE, _("Save"), _("Save project")));
  mref_ProjectActionGroup->add(Gtk::Action::create("FileSaveAs",
      Gtk::Stock::SAVE_AS, _("Save As..."), _("Save project as...")));
  mref_ProjectActionGroup->add(Gtk::Action::create("FileClose",
      Gtk::Stock::CLOSE));

  //Project menu
  mref_ProjectActionGroup->add(Gtk::Action::create("ProjectMenu", _("Project")));
  mref_ProjectActionGroup->add(Gtk::Action::create("ProjectCheck",
      Gtk::Stock::OK, _("Check"), _("Check Project")));
  mref_ProjectActionGroup->add(Gtk::Action::create("ProjectRun",
      Gtk::Stock::EXECUTE, _("Run"), _("Run Simulation")));
  mref_ProjectActionGroup->add(Gtk::Action::create("ProjectProperties",
      Gtk::Stock::PROPERTIES));
}


// =====================================================================
// =====================================================================


BuilderAppActions::BuilderAppActions()
{
  mref_UIManager = Gtk::UIManager::create();
  createAppUiXml();
  createProjectUiXml();
  createAppActionGroup();
  createProjectActionGroup();
  mref_UIManager->insert_action_group(mref_AppActionGroup);
  mref_UIManager->insert_action_group(mref_ProjectActionGroup);
  try
  {
    mref_UIManager->add_ui_from_string(m_AppUiXml);
    mref_UIManager->add_ui_from_string(m_ProjectUiXml);
  } catch (const Glib::Error& ex)
  {
    std::cerr << "building menus failed: " << ex.what();
  }
}


// =====================================================================
// =====================================================================


Gtk::Widget* BuilderAppActions::getMenuBarWidget()
{
  return mref_UIManager->get_widget("/MenuBar");
}


// =====================================================================
// =====================================================================


Gtk::Widget* BuilderAppActions::getToolBarWidget()
{
  return mref_UIManager->get_widget("/ToolBar");
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::AccelGroup> BuilderAppActions::getAccelGroup()
{
  return mref_UIManager->get_accel_group();
}


// =====================================================================
// =====================================================================


void BuilderAppActions::setProjectActionGroupSensitive(bool Sensitive)
{
  mref_ProjectActionGroup->set_sensitive(Sensitive);
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> BuilderAppActions::getFileNewAction()
{
  return mref_AppActionGroup->get_action("FileNewEmpty");
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> BuilderAppActions::getFileNewFromAction()
{
  return mref_AppActionGroup->get_action("FileNewFrom");
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> BuilderAppActions::getFileOpenAction()
{
  return mref_AppActionGroup->get_action("FileOpen");
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> BuilderAppActions::getFileCloseAction()
{
  return mref_ProjectActionGroup->get_action("FileClose");
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> BuilderAppActions::getFileQuitAction()
{
  return mref_AppActionGroup->get_action("FileQuit");
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> BuilderAppActions::getProjectCheckAction()
{
  return mref_ProjectActionGroup->get_action("ProjectCheck");
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> BuilderAppActions::getProjectRunAction()
{
  return mref_ProjectActionGroup->get_action("ProjectRun");
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> BuilderAppActions::getAppMarketAction()
{
  return mref_AppActionGroup->get_action("HelpMarket");
}


