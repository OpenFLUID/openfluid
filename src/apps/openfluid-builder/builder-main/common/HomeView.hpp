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
 \file HomeView.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __HOMEVIEW_HPP__
#define __HOMEVIEW_HPP__

#include <gtkmm/box.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>

#include <glibmm/i18n.h>
#include <sigc++/sigc++.h>

class BuilderHomeButton;
class BuilderAppActions;

class HomeView
{
  public:

    virtual sigc::signal<void, std::string> signal_OpenProjectAsked() = 0;

    virtual Gtk::Widget* asWidget() = 0;

    virtual void setVersion(std::string VersionTxt) = 0;

    virtual void setWebSite(std::string WebSite) = 0;

    virtual void setRecentProjects(std::vector<std::pair<std::string,
        std::string> > RecentProjects) = 0;
};

// =====================================================================
// =====================================================================


class HomeViewImpl: public HomeView
{
  private:
    sigc::signal<void, std::string> m_signal_OpenProjectAsked;
    BuilderAppActions& mp_Actions;
    Gtk::Box* mp_MainContainer;

    Gtk::Box* mp_HeadPanelBox;
    Gtk::EventBox* mp_HeadPanel;
    Gtk::Box* mp_BottomPanel;

    Gtk::Box* mp_ButtonPanel;
    Gtk::Box* mp_RecentPanel;

    Gtk::Image* mp_LogoImage;
    Gtk::Label* mp_VersionLabel;
    Gtk::Label* mp_LinkLabel;
    Gtk::Label* mp_RecentTitleLabel;
    Gtk::Label* mp_RecentLabel;

    BuilderHomeButton* mp_NewButton;
    BuilderHomeButton* mp_OpenButton;
    BuilderHomeButton* mp_DemosButton;
    BuilderHomeButton* mp_MarketButton;

    void createHeadPanel();

    void createButtonPanel();

    void createRecentPanel();

    void createBottomPanel();

    void addARecentProject(std::string ProjectName, std::string ProjectPath);

    bool onRecentClicked(GdkEventButton* Event, std::string RecentPath);

    bool onRecentEnter(GdkEventCrossing* Event, Gtk::Label* HoveredLabel);

    bool onRecentLeave(GdkEventCrossing* Event, Gtk::Label* HoveredLabel);

  public:

    HomeViewImpl(BuilderAppActions& Actions);

    sigc::signal<void, std::string> signal_OpenProjectAsked();

    void setVersion(std::string VersionTxt);

    void setWebSite(std::string WebSite);

    void setRecentProjects(
        std::vector<std::pair<std::string, std::string> > RecentProjects);

    Gtk::Widget* asWidget();
};

#endif /* __HOMEVIEW_HPP__ */
