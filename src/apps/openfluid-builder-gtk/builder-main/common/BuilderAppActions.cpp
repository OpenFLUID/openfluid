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
#include <gtkmm/stock.h>
#include <iostream>

#include "BuilderGraphicsHelper.hpp"

// =====================================================================
// =====================================================================

void BuilderAppActions::createAppUiXml()
{
  m_AppUiXml = "<ui>"
               "  <menubar name='MenuBar'>"
               "    <menu action='FileMenu'>"
               "      <menuitem action='FileNewEmpty'/>"
               "      <menuitem action='FileOpen'/>"
               "      <separator/>"
               "      <placeholder name='ProjectFilePlaceholder' />"
               "      <menuitem action='FileQuit'/>"
               "    </menu>"
               "    <menu action='EditMenu'>"
               "      <menuitem action='EditCut'/>"
               "      <menuitem action='EditCopy'/>"
               "      <menuitem action='EditPaste'/>"
               "      <separator/>"
               "      <menuitem action='EditPreferences'/>"
               "    </menu>"
               "    <placeholder name='ProjectDataPlaceholder' />"
               "    <placeholder name='ProjectSimulationPlaceholder' />"
               "    <placeholder name='ProjectExtensionsPlaceholder' />"
               "    <menu action='HelpMenu'>"
               "      <menu action='HelpOnlineMenu'>"
               "        <menuitem action='HelpOnlineWebsite'/>"
               "        <menuitem action='HelpOnlineCommunity'/>"
               "        <menuitem action='HelpOnlineDev'/>"
               "        <menuitem action='HelpOnlineBug'/>"
               "      </menu>"
               "      <separator/>"
               "      <menu action='HelpDemo'>"
               "        <menuitem action='HelpDemoOpen'/>"
               "        <menuitem action='HelpDemoRestore'/>"
               "      </menu>"
               "      <separator/>"
               "      <menuitem action='HelpAbout'/>"
               "    </menu>"
               "  </menubar>"

               "  <toolbar  name='ToolBar'>"
               "    <toolitem action='FileNewEmpty'/>"
               "    <toolitem action='FileOpen'/>"
               "    <placeholder name='ProjectFilePlaceholder' />"
               "    <placeholder name='SimulationPlaceholder' />"
               //    "      <separator/>"
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
                   "    <placeholder name='ProjectFilePlaceholder'>"
                   "      <menuitem action='FileSave'/>"
                   "      <menuitem action='FileSaveAs'/>"
                   "      <separator/>"
                   "      <menuitem action='FileProperties'/>"
                   "      <separator/>"
                   "      <menuitem action='FileClose'/>"
                   "    </placeholder>"
                   "  </menu>"
                   "  <placeholder name='ProjectDataPlaceholder'>"
                   "    <menu action='DataMenu'>"
                   "      <menu action='DataDomainMenu'/>"
                   "      <menu action='DataAttributesMenu'/>"
                   "      <menu action='DataEventsMenu'/>"
                   "      <menu action='DataExtraMenu'/>"
                   "      <menu action='DataMixedMenu'/>"
                   "    </menu>"
                   "  </placeholder>"
                   "  <placeholder name='ProjectSimulationPlaceholder'>"
                   "    <menu action='SimulationMenu'>"
                   "      <menuitem action='Refresh'/>"
                   "      <separator/>"
                   "      <menuitem action='SimulationRun'/>"
                   "    </menu>"
                   "  </placeholder>"
                   "  <placeholder name='ProjectExtensionsPlaceholder'>"
                   "    <menu action='ExtensionsMenu'>"
                   "    </menu>"
                   "  </placeholder>"
                   "  </menubar>"

                   "  <toolbar  name='ToolBar'>"
                   "  <placeholder name='ProjectFilePlaceholder'>"
                   "    <toolitem action='FileSave'/>"
                   "    <toolitem action='FileSaveAs'/>"
                   "    <toolitem action='FileClose'/>"
                   "  </placeholder>"
                   "  <placeholder name='SimulationPlaceholder'>"
                   "    <separator/>"
                   "    <toolitem action='MapView'/>"
                   "    <separator/>"
                   "    <toolitem action='SimulationRun'/>"
                   "  </placeholder>"
                   "  </toolbar>"
                   "</ui>";
}

// =====================================================================
// =====================================================================

void BuilderAppActions::createAppActionGroup()
{
  mref_AppActionGroup = Gtk::ActionGroup::create();

  //File menu
  mref_AppActionGroup->add(Gtk::Action::create("FileMenu", _("_Project")));
  mref_AppActionGroup->add(
      Gtk::Action::create("FileNewEmpty", Gtk::Stock::NEW, _("_New"),
                          _("Create a project")));
  mref_AppActionGroup->add(
      Gtk::Action::create("FileOpen", Gtk::Stock::OPEN, _("_Open..."),
                          _("Open a project")));
  mref_AppActionGroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT));

  //Edit menu
  mref_AppActionGroup->add(Gtk::Action::create("EditMenu", _("E_dit")));
  mref_AppActionGroup->add(Gtk::Action::create("EditCut", Gtk::Stock::CUT));
  mref_AppActionGroup->add(Gtk::Action::create("EditCopy", Gtk::Stock::COPY));
  mref_AppActionGroup->add(Gtk::Action::create("EditPaste", Gtk::Stock::PASTE));
  mref_AppActionGroup->add(
      Gtk::Action::create("EditPreferences", Gtk::Stock::PREFERENCES));

  //Help menu
  mref_AppActionGroup->add(Gtk::Action::create("HelpMenu", Gtk::Stock::HELP));
  mref_AppActionGroup->add(
      Gtk::Action::create("HelpOnlineMenu", _("OpenFLUID online")));
  mref_AppActionGroup->add(
      Gtk::Action::create("HelpOnlineWebsite", _("Official website")));
  mref_AppActionGroup->add(
      Gtk::Action::create("HelpOnlineCommunity", _("Community website")));
  mref_AppActionGroup->add(
      Gtk::Action::create("HelpOnlineDev", _("Development website")));
  mref_AppActionGroup->add(
      Gtk::Action::create("HelpOnlineBug", _("Bug tracking")));
  mref_AppActionGroup->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT));
  mref_AppActionGroup->add(Gtk::Action::create("HelpDemo", _("Examples")));
  mref_AppActionGroup->add(
      Gtk::Action::create("HelpDemoOpen", _("Open an example project..."),
                          _("Open an example project")));
  mref_AppActionGroup->add(
      Gtk::Action::create("HelpDemoRestore",
                          _("Restore default example projects")));

  //Home menu
  mref_AppActionGroup->add(
      Gtk::Action::create(
          "HomeMarket",
          *BuilderGraphicsHelper::createBuilderIconStockId(
              "openfluid_in_cart.png", "openfluid_in_cart"),
          _("OpenFLUID Market..."), _("Access to OpenFLUID Market")));
}

// =====================================================================
// =====================================================================

void BuilderAppActions::createProjectActionGroup()
{
  mref_ProjectActionGroup = Gtk::ActionGroup::create();

  //File menu
  mref_ProjectActionGroup->add(
      Gtk::Action::create("FileSave", Gtk::Stock::SAVE, _("_Save"), _("Save")));
  mref_ProjectActionGroup->add(
      Gtk::Action::create("FileSaveAs", Gtk::Stock::SAVE_AS, _("Save _as..."),
                          _("Save as...")));
  mref_ProjectActionGroup->add(
      Gtk::Action::create("FileProperties", Gtk::Stock::PROPERTIES,
                          _("Properties"), _("Project properties")));
  mref_ProjectActionGroup->add(
      Gtk::Action::create("FileClose", Gtk::Stock::CLOSE));

  //Data menu
  mref_ProjectActionGroup->add(Gtk::Action::create("DataMenu", _("Data")));
  mref_ProjectActionGroup->add(
      Gtk::Action::create("DataDomainMenu", _("Import spatial domain")));
  mref_ProjectActionGroup->add(
      Gtk::Action::create("DataAttributesMenu", _("Import attributes")));
  mref_ProjectActionGroup->add(
      Gtk::Action::create("DataEventsMenu", _("Import events")));
  mref_ProjectActionGroup->add(
      Gtk::Action::create("DataExtraMenu", _("Import extra file")));
  mref_ProjectActionGroup->add(
      Gtk::Action::create("DataMixedMenu", _("Other imports")));

  //Simulation menu
  mref_ProjectActionGroup->add(
      Gtk::Action::create("SimulationMenu", _("_Simulation")));
  mref_ProjectActionGroup->add(
      Gtk::Action::create("Refresh", Gtk::Stock::REFRESH,
                          _("Reload simulators"),
                          _("Reload simulators")));
  mref_ProjectActionGroup->add(
      Gtk::Action::create("SimulationRun", Gtk::Stock::MEDIA_PLAY,
                          _("Run...")));

  mref_ProjectActionGroup->add(
      Gtk::Action::create(
          "MapView",
          *BuilderGraphicsHelper::createBuilderIconStockId("mapview.png",
                                                           "mapview"),
          _("Map View"), _("Map View")));

  //Extensions menu
  mref_ProjectActionGroup->add(
      Gtk::Action::create("ExtensionsMenu", _("Extensions")));

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
  }
  catch (const Glib::Error& ex)
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

void BuilderAppActions::setProjectActionGroupVisible(bool Visible)
{
  mref_ProjectActionGroup->set_visible(Visible);
  getAppDemoRestoreAction()->set_visible(!Visible);
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getFileNewAction()
{
  return mref_AppActionGroup->get_action("FileNewEmpty");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getFileOpenAction()
{
  return mref_AppActionGroup->get_action("FileOpen");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getFilePropertiesAction()
{
  return mref_ProjectActionGroup->get_action("FileProperties");
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

Glib::RefPtr<Gtk::Action> BuilderAppActions::getRefreshAction()
{
  return mref_ProjectActionGroup->get_action("Refresh");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getSimulationRunAction()
{
  return mref_ProjectActionGroup->get_action("SimulationRun");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getAppMarketAction()
{
  return mref_AppActionGroup->get_action("HomeMarket");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getEditPreferencesAction()
{
  return mref_AppActionGroup->get_action("EditPreferences");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getAppAboutAction()
{
  return mref_AppActionGroup->get_action("HelpAbout");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getAppOnlineWebsiteAction()
{
  return mref_AppActionGroup->get_action("HelpOnlineWebsite");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getAppOnlineCommunityAction()
{
  return mref_AppActionGroup->get_action("HelpOnlineCommunity");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getAppOnlineDevAction()
{
  return mref_AppActionGroup->get_action("HelpOnlineDev");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getAppOnlineBugAction()
{
  return mref_AppActionGroup->get_action("HelpOnlineBug");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getAppDemoOpenAction()
{
  return mref_AppActionGroup->get_action("HelpDemoOpen");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getAppDemoRestoreAction()
{
  return mref_AppActionGroup->get_action("HelpDemoRestore");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getSaveAction()
{
  return mref_ProjectActionGroup->get_action("FileSave");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getSaveAsAction()
{
  return mref_ProjectActionGroup->get_action("FileSaveAs");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getMapViewAction()
{
  return mref_ProjectActionGroup->get_action("MapView");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::addProjectExtensionAction(
    std::string ActionName, std::string ActionLabel, std::string ActionTooltip)
{
  Glib::RefPtr<Gtk::Action> NewAction = Gtk::Action::create(ActionName,
                                                            ActionLabel,
                                                            ActionTooltip);

  mref_ProjectActionGroup->add(NewAction);

  std::string MenuPath = "/MenuBar/ProjectExtensionsPlaceholder/ExtensionsMenu/";
  std::string MenuItemName = ActionName + "MenuItem";

  mref_UIManager->add_ui(gtk_ui_manager_new_merge_id(mref_UIManager->gobj()),
                         MenuPath, MenuItemName, ActionName,
                         Gtk::UI_MANAGER_MENUITEM, false);

  mref_UIManager->get_widget(MenuPath + MenuItemName)->set_tooltip_markup(
      ActionTooltip);

  return NewAction;
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::addProjectDataAction(
    Glib::RefPtr<Gtk::Action> ParentMenuAction, std::string ActionName,
    std::string ActionLabel, std::string ActionTooltip)
{
  Glib::RefPtr<Gtk::Action> NewAction = Gtk::Action::create(ActionName,
                                                            ActionLabel,
                                                            ActionTooltip);

  mref_ProjectActionGroup->add(NewAction);

  std::string MenuPath = "/MenuBar/ProjectDataPlaceholder/DataMenu/"
      + ParentMenuAction->get_name() + "/";
  std::string MenuItemName = ActionName + "MenuItem";

  mref_UIManager->add_ui(gtk_ui_manager_new_merge_id(mref_UIManager->gobj()),
                         MenuPath, MenuItemName, ActionName,
                         Gtk::UI_MANAGER_MENUITEM, false);

  mref_UIManager->get_widget(MenuPath + MenuItemName)->set_tooltip_markup(
      ActionTooltip);

  return NewAction;
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getDataDomainMenuAction()
{
  return mref_ProjectActionGroup->get_action("DataDomainMenu");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getDataAttributesMenuAction()
{
  return mref_ProjectActionGroup->get_action("DataAttributesMenu");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getDataEventsMenuAction()
{
  return mref_ProjectActionGroup->get_action("DataEventsMenu");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getDataExtraMenuAction()
{
  return mref_ProjectActionGroup->get_action("DataExtraMenu");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getDataMixedMenuAction()
{
  return mref_ProjectActionGroup->get_action("DataMixedMenu");
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> BuilderAppActions::getExtensionAction(
    const std::string ExtensionID)
{
  return mref_ProjectActionGroup->get_action(ExtensionID);
}
