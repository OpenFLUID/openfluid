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
 \file HomeView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "HomeView.hpp"

#include <glibmm/i18n.h>

#include <gtkmm/buttonbox.h>
#include <gtkmm/separator.h>
#include <gtkmm/arrow.h>
#include <gtkmm/stock.h>

#include "BuilderHomeButton.hpp"
#include "BuilderAppActions.hpp"
#include "BuilderGraphicsHelper.hpp"

// =====================================================================
// =====================================================================

void HomeViewImpl::createHeadPanel()
{
  mp_LogoImage = BuilderGraphicsHelper::createImageFromFileName(
      "openfluid_official.png");
  mp_LogoImage->set_visible(true);

  mp_VersionLabel = Gtk::manage(new Gtk::Label());
  mp_VersionLabel->set_justify(Gtk::JUSTIFY_CENTER);
  mp_VersionLabel->set_visible(true);

  mp_LinkLabel = Gtk::manage(new Gtk::Label());
  mp_LinkLabel->set_visible(true);

  mp_HeadPanelBox = Gtk::manage(new Gtk::VBox(false, 10));
  mp_HeadPanelBox->set_visible(true);
  mp_HeadPanelBox->set_border_width(10);
  mp_HeadPanelBox->pack_start(*mp_LogoImage, Gtk::PACK_SHRINK, 10);
  mp_HeadPanelBox->pack_start(*mp_VersionLabel, Gtk::PACK_SHRINK, 10);
  mp_HeadPanelBox->pack_start(*mp_LinkLabel, Gtk::PACK_SHRINK, 10);

  mp_HeadPanel = Gtk::manage(new Gtk::EventBox());
  mp_HeadPanel->add(*mp_HeadPanelBox);
  mp_HeadPanel->set_visible(true);
  mp_HeadPanel->modify_bg(Gtk::STATE_NORMAL, Gdk::Color("#FFFFFF"));
}

// =====================================================================
// =====================================================================

void HomeViewImpl::createButtonPanel()
{
  mp_NewButton = Gtk::manage(
      new BuilderHomeButton(mp_Actions.getFileNewAction()));
  mp_NewButton->set_visible(true);

  mp_OpenButton = Gtk::manage(
      new BuilderHomeButton(mp_Actions.getFileOpenAction()));
  mp_OpenButton->set_visible(true);

  mp_DemosButton = Gtk::manage(
      new BuilderHomeButton(
          *BuilderGraphicsHelper::createEmblemedImage(Gtk::Stock::OPEN, 48,
                                                      Gtk::Stock::ABOUT, 48,
                                                      Gtk::ICON_SIZE_DIALOG),
          mp_Actions.getAppDemoOpenAction()));
  mp_DemosButton->set_visible(true);

  mp_MarketButton = Gtk::manage(
      new BuilderHomeButton(mp_Actions.getAppMarketAction()));
  mp_MarketButton->set_visible(true);

  Gtk::VButtonBox* ButtonBox = Gtk::manage(
      new Gtk::VButtonBox(Gtk::BUTTONBOX_START, 4));
  ButtonBox->set_visible(true);
  ButtonBox->pack_start(*mp_NewButton, Gtk::PACK_SHRINK, 0);
  ButtonBox->pack_start(*mp_OpenButton, Gtk::PACK_SHRINK, 0);

  Gtk::VButtonBox* ExtraButtonBox1 = Gtk::manage(
      new Gtk::VButtonBox(Gtk::BUTTONBOX_START, 4));
  ExtraButtonBox1->set_visible(true);
  ExtraButtonBox1->pack_start(*mp_DemosButton, Gtk::PACK_SHRINK, 0);

  Gtk::VButtonBox* ExtraButtonBox2 = Gtk::manage(
      new Gtk::VButtonBox(Gtk::BUTTONBOX_START, 4));
  ExtraButtonBox2->set_visible(true);
  ExtraButtonBox2->pack_start(*mp_MarketButton, Gtk::PACK_SHRINK, 0);

  mp_ButtonPanel = Gtk::manage(new Gtk::VBox(false, 0));
  mp_ButtonPanel->set_visible(true);
  mp_ButtonPanel->pack_start(*ButtonBox, Gtk::PACK_SHRINK, 10);
  mp_ButtonPanel->pack_start(*(new Gtk::HSeparator()), Gtk::PACK_SHRINK, 0);
  mp_ButtonPanel->pack_start(*ExtraButtonBox1, Gtk::PACK_SHRINK, 10);
  mp_ButtonPanel->pack_start(*(new Gtk::HSeparator()), Gtk::PACK_SHRINK, 0);
  mp_ButtonPanel->pack_start(*ExtraButtonBox2, Gtk::PACK_SHRINK, 10);
  mp_ButtonPanel->show_all_children(true);
}

// =====================================================================
// =====================================================================

void HomeViewImpl::createRecentPanel()
{
  mp_RecentTitleLabel = Gtk::manage(new Gtk::Label());
  mp_RecentTitleLabel->set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP);
  mp_RecentTitleLabel->set_padding(10, 10);
  mp_RecentTitleLabel->set_markup(_("<b>Recent projects</b>"));
  mp_RecentTitleLabel->set_visible(true);

  mp_RecentPanel = Gtk::manage(new Gtk::VBox(false, 0));
  mp_RecentPanel->set_visible(true);
  mp_RecentPanel->pack_start(*mp_RecentTitleLabel, Gtk::PACK_SHRINK, 0);
}

// =====================================================================
// =====================================================================

void HomeViewImpl::createBottomPanel()
{
  createButtonPanel();
  createRecentPanel();

  Gtk::VSeparator* VSep = Gtk::manage(new Gtk::VSeparator());
  VSep->set_visible(true);

  mp_BottomPanel = Gtk::manage(new Gtk::HBox(false, 0));
  mp_BottomPanel->set_visible(true);
  mp_BottomPanel->pack_start(*mp_ButtonPanel, Gtk::PACK_SHRINK, 10);
  mp_BottomPanel->pack_start(*VSep, Gtk::PACK_SHRINK, 0);
  mp_BottomPanel->pack_start(*mp_RecentPanel, Gtk::PACK_EXPAND_WIDGET, 0);
}

// =====================================================================
// =====================================================================

HomeViewImpl::HomeViewImpl(BuilderAppActions& Actions) :
    mp_Actions(Actions)
{
  createHeadPanel();
  createBottomPanel();

  Gtk::HSeparator* HSep = Gtk::manage(new Gtk::HSeparator());
  HSep->set_visible(true);

  mp_MainContainer = Gtk::manage(new Gtk::VBox(false, 0));
  mp_MainContainer->set_border_width(0);
  mp_MainContainer->set_visible(true);
  mp_MainContainer->pack_start(*mp_HeadPanel, Gtk::PACK_SHRINK);
  mp_MainContainer->pack_start(*HSep, Gtk::PACK_SHRINK, 0);
  mp_MainContainer->pack_start(*mp_BottomPanel, Gtk::PACK_EXPAND_WIDGET, 0);
}

// =====================================================================
// =====================================================================

sigc::signal<void, std::string> HomeViewImpl::signal_OpenProjectAsked()
{
  return m_signal_OpenProjectAsked;
}

// =====================================================================
// =====================================================================

void HomeViewImpl::setVersion(std::string VersionTxt)
{
  mp_VersionLabel->set_markup(VersionTxt);
}

// =====================================================================
// =====================================================================

void HomeViewImpl::setWebSite(std::string WebSite)
{
  mp_LinkLabel->set_markup("<a href=\"" + WebSite + "\">" + WebSite + "</a>");
}

// =====================================================================
// =====================================================================

void HomeViewImpl::setRecentProjects(
    std::vector<std::pair<std::string, std::string> > RecentProjects)
{
  if (RecentProjects.empty())
  {
    mp_RecentLabel = Gtk::manage(new Gtk::Label());
    mp_RecentLabel->set_markup(
        Glib::ustring::compose("<i>%1</i>", _("no recent project")));
    mp_RecentLabel->set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP);
    mp_RecentLabel->set_padding(30, 10);
    mp_RecentLabel->set_visible(true);
    mp_RecentPanel->pack_start(*mp_RecentLabel, Gtk::PACK_SHRINK, 0);
  }
  else
  {
    for (unsigned int i = 0; i < RecentProjects.size(); i++)
    {
      addARecentProject(RecentProjects[i].second, RecentProjects[i].first);
    }
  }

}

// =====================================================================
// =====================================================================

void HomeViewImpl::addARecentProject(std::string ProjectName,
                                     std::string ProjectPath)
{
  Gtk::Label* RecentLabel = Gtk::manage(new Gtk::Label());
  RecentLabel->set_markup(ProjectName == "" ? _("unnamed") : ProjectName);
  RecentLabel->set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER);
  RecentLabel->set_padding(5, 0);
  RecentLabel->set_visible(true);

  Gtk::Arrow* Arrow = Gtk::manage(
      new Gtk::Arrow(Gtk::ARROW_RIGHT, Gtk::SHADOW_NONE));
  Arrow->set_padding(20, 0);
  Arrow->set_visible(true);

  Gtk::HBox* Box = Gtk::manage(new Gtk::HBox());
  Box->pack_start(*Arrow, Gtk::PACK_SHRINK);
  Box->pack_start(*RecentLabel);
  Box->set_visible(true);

  Gtk::EventBox* RecentBox = Gtk::manage(new Gtk::EventBox());
  RecentBox->set_tooltip_text(ProjectPath);
  RecentBox->signal_button_press_event().connect(
      sigc::bind<std::string>(
          sigc::mem_fun(*this, &HomeViewImpl::onRecentClicked), ProjectPath));
  RecentBox->signal_enter_notify_event().connect(
      sigc::bind<Gtk::Label*>(
          sigc::mem_fun(*this, &HomeViewImpl::onRecentEnter), RecentLabel));
  RecentBox->signal_leave_notify_event().connect(
      sigc::bind<Gtk::Label*>(
          sigc::mem_fun(*this, &HomeViewImpl::onRecentLeave), RecentLabel));
  RecentBox->add(*Box);
  RecentBox->set_visible(true);

  mp_RecentPanel->pack_start(*RecentBox, Gtk::PACK_SHRINK, 5);
}

// =====================================================================
// =====================================================================

bool HomeViewImpl::onRecentClicked(GdkEventButton* /*Event*/,
                                   std::string RecentPath)
{
  m_signal_OpenProjectAsked.emit(RecentPath);
  return false;
}

// =====================================================================
// =====================================================================

bool HomeViewImpl::onRecentEnter(GdkEventCrossing* /*Event*/,
                                 Gtk::Label* HoveredLabel)
{
  HoveredLabel->set_markup("<u>" + HoveredLabel->get_text() + "</u>");
  return false;
}

// =====================================================================
// =====================================================================

bool HomeViewImpl::onRecentLeave(GdkEventCrossing* /*Event*/,
                                 Gtk::Label* HoveredLabel)
{
  HoveredLabel->set_text(HoveredLabel->get_text());
  return false;
}

// =====================================================================
// =====================================================================

Gtk::Widget* HomeViewImpl::asWidget()
{
  return mp_MainContainer;
}

