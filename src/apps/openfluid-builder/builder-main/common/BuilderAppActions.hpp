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
 \file BuilderAppActions.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __BUILDERAPPACTIONS_HPP__
#define __BUILDERAPPACTIONS_HPP__

#include <gtkmm/uimanager.h>
#include <gtkmm/actiongroup.h>

class BuilderAppActions
{
  private:

    Glib::RefPtr<Gtk::UIManager> mref_UIManager;
    Glib::RefPtr<Gtk::ActionGroup> mref_AppActionGroup;
    Glib::RefPtr<Gtk::ActionGroup> mref_ProjectActionGroup;
    Glib::ustring m_AppUiXml;
    Glib::ustring m_ProjectUiXml;

    void createAppUiXml();

    void createProjectUiXml();

    void createAppActionGroup();

    void createProjectActionGroup();

  public:

    BuilderAppActions();

    Gtk::Widget* getMenuBarWidget();

    Gtk::Widget* getToolBarWidget();

    Glib::RefPtr<Gtk::AccelGroup> getAccelGroup();

    void setProjectActionGroupSensitive(bool Sensitive);

    void setProjectActionGroupVisible(bool Visible);

    Glib::RefPtr<Gtk::Action> getFileNewAction();

    Glib::RefPtr<Gtk::Action> getFileOpenAction();

    Glib::RefPtr<Gtk::Action> getFilePropertiesAction();

    Glib::RefPtr<Gtk::Action> getFileCloseAction();

    Glib::RefPtr<Gtk::Action> getFileQuitAction();

    Glib::RefPtr<Gtk::Action> getSimulationRunAction();

    Glib::RefPtr<Gtk::Action> getAppDemoOpenAction();

    Glib::RefPtr<Gtk::Action> getEditPreferencesAction();

    Glib::RefPtr<Gtk::Action> getAppAboutAction();

    Glib::RefPtr<Gtk::Action> getAppOnlineWebsiteAction();

    Glib::RefPtr<Gtk::Action> getAppOnlineCommunityAction();

    Glib::RefPtr<Gtk::Action> getAppOnlineDevAction();

    Glib::RefPtr<Gtk::Action> getAppOnlineBugAction();

    Glib::RefPtr<Gtk::Action> getAppDemoRestoreAction();

    Glib::RefPtr<Gtk::Action> getSaveAction();

    Glib::RefPtr<Gtk::Action> getSaveAsAction();

    Glib::RefPtr<Gtk::Action> getMapViewAction();

    Glib::RefPtr<Gtk::Action> getAppMarketAction();

    Glib::RefPtr<Gtk::Action> getRefreshAction();

    Glib::RefPtr<Gtk::Action> addProjectExtensionAction(
        std::string ActionName, std::string ActionLabel,
        std::string ActionTooltip = "");

    Glib::RefPtr<Gtk::Action> addProjectDataAction(
        Glib::RefPtr<Gtk::Action> ParentMenuAction, std::string ActionName,
        std::string ActionLabel, std::string ActionTooltip = "");

    Glib::RefPtr<Gtk::Action> getDataDomainMenuAction();
    Glib::RefPtr<Gtk::Action> getDataInputdataMenuAction();
    Glib::RefPtr<Gtk::Action> getDataEventsMenuAction();
    Glib::RefPtr<Gtk::Action> getDataExtraMenuAction();
    Glib::RefPtr<Gtk::Action> getDataMixedMenuAction();

    Glib::RefPtr<Gtk::Action> getExtensionAction(const std::string ExtensionID);
};

#endif /* __BUILDERAPPACTIONS_HPP__ */

