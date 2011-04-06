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
 \file ProjectWorkspace.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ProjectWorkspace.hpp"

#include <iostream>

// =====================================================================
// =====================================================================


ProjectWorkspace::ProjectWorkspace()
{
  mp_Notebook = Gtk::manage(new Gtk::Notebook());
  mp_Notebook->set_visible(true);
}

// =====================================================================
// =====================================================================


sigc::signal<void, std::string> ProjectWorkspace::signal_PageRemoved()
{
  return m_signal_PageRemoved;
}

// =====================================================================
// =====================================================================


bool ProjectWorkspace::existsPageName(std::string PageName)
{
  return (m_ByNamePagesMap.find(PageName) != m_ByNamePagesMap.end());
}

// =====================================================================
// =====================================================================


void ProjectWorkspace::setCurrentPage(std::string PageName)
{
  if (existsPageName(PageName))
    mp_Notebook->set_current_page(mp_Notebook->page_num(
        *m_ByNamePagesMap[PageName]));
}

// =====================================================================
// =====================================================================


void ProjectWorkspace::appendPage(std::string PageName,
    Gtk::Widget* PageContent)
{
  Gtk::Widget* TabLabel = createClosableTabLabel(PageName);

  mp_Notebook->append_page(*PageContent, *TabLabel, *TabLabel);

  m_ByNamePagesMap[PageName] = PageContent;
}

// =====================================================================
// =====================================================================


void ProjectWorkspace::removePage(std::string PageName)
{
  if (existsPageName(PageName))
  {
    mp_Notebook->remove_page(*m_ByNamePagesMap[PageName]);

    m_ByNamePagesMap.erase(m_ByNamePagesMap.find(PageName));

    m_signal_PageRemoved.emit(PageName);
  }
}

// =====================================================================
// =====================================================================


std::vector<std::string> ProjectWorkspace::getPageNames()
{
  std::vector<std::string> PageNames;

  for (std::map<std::string, Gtk::Widget*>::iterator it =
      m_ByNamePagesMap.begin(); it != m_ByNamePagesMap.end(); ++it)
  {
    PageNames.push_back(it->first);
  }

  return PageNames;
}

// =====================================================================
// =====================================================================


Gtk::Widget* ProjectWorkspace::createClosableTabLabel(std::string LabelText)
{
  Gtk::Label* TabLabel = Gtk::manage(new Gtk::Label(LabelText));

  Gtk::Image* TabImage = Gtk::manage(new Gtk::Image(Gtk::Stock::CLOSE,
      Gtk::ICON_SIZE_BUTTON));

  Gtk::EventBox* EventBox = Gtk::manage(new Gtk::EventBox());
  EventBox->set_events(Gdk::BUTTON_PRESS_MASK);
  EventBox->signal_button_press_event().connect(sigc::bind<std::string>(
      sigc::mem_fun(*this, &ProjectWorkspace::onTabCloseImageClicked),
      LabelText));
  EventBox->set_tooltip_text(_("Close this page"));
  EventBox->add(*TabImage);

  Gtk::HBox* TabLabelBox = Gtk::manage(new Gtk::HBox());
  TabLabelBox->pack_start(*TabLabel, Gtk::SHRINK, 0);
  TabLabelBox->pack_end(*EventBox, Gtk::SHRINK, 0);

  TabLabelBox->show_all_children();

  return TabLabelBox;
}

// =====================================================================
// =====================================================================


bool ProjectWorkspace::onTabCloseImageClicked(
    GdkEventButton* /*ClickedButton*/, std::string LabelText)
{
  if (existsPageName(LabelText))
  {
    mp_Notebook->remove(*m_ByNamePagesMap[LabelText]);

    m_ByNamePagesMap.erase(LabelText);

    m_signal_PageRemoved.emit(LabelText);

    return true;
  }
  return false;
}

// =====================================================================
// =====================================================================


Gtk::Widget* ProjectWorkspace::asWidget()
{
  //  Gtk::Label* NewLabel = new Gtk::Label("ici nouveau");
  //  NewLabel->set_visible(true);
  //
  //  Gtk::Label* OtherLabel = new Gtk::Label("ici other");
  //  OtherLabel->set_visible(true);
  //
  //  appendPage("new tab", NewLabel);
  //  appendPage("another tab", OtherLabel);
  //
  //  setCurrentPage("another tab");

  return mp_Notebook;
}
