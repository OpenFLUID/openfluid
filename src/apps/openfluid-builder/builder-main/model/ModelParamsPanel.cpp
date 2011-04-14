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
 \file ModelParamsPanel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelParamsPanel.hpp"

#include "ModelFctParamsComponent.hpp"

// =====================================================================
// =====================================================================


ModelParamsPanel::ModelParamsPanel()
{
  mp_Notebook = Gtk::manage(new Gtk::Notebook());
  mp_Notebook->set_scrollable(true);
  mp_Notebook->popup_enable();
  mp_Notebook->set_visible(true);
}

// =====================================================================
// =====================================================================


ModelParamsPanel::~ModelParamsPanel()
{
  // TODO Auto-generated destructor stub
}

// =====================================================================
// =====================================================================


void ModelParamsPanel::addAStaticPage(Gtk::Widget* Page, std::string Label,
    int Position)
{
  Gtk::Label* TabLabel = Gtk::manage(new Gtk::Label(Label));
  Gtk::Label* MenuLabel = Gtk::manage(new Gtk::Label(Label, Gtk::ALIGN_LEFT,
      Gtk::ALIGN_CENTER));

  mp_Notebook->insert_page(*Page, *TabLabel, *MenuLabel, Position);
  mp_Notebook->set_tab_reorderable(*Page, true);
}

// =====================================================================
// =====================================================================


void ModelParamsPanel::addAFctParamsPage(Gtk::Widget* FctParamsPage,
    std::string PageLabel)
{
  Gtk::Label* TabLabel = Gtk::manage(new Gtk::Label(PageLabel));
  Gtk::Label* MenuLabel = Gtk::manage(new Gtk::Label(PageLabel,
      Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));

  mp_Notebook->append_page(*FctParamsPage, *TabLabel, *MenuLabel);
  mp_Notebook->set_tab_reorderable(*FctParamsPage, true);

  m_ByFctNamePages[PageLabel] = FctParamsPage;
}

// =====================================================================
// =====================================================================


void ModelParamsPanel::removeAPage(std::string PageLabel)
{
  if (m_ByFctNamePages.find(PageLabel) != m_ByFctNamePages.end())
  {
    mp_Notebook->remove_page(*m_ByFctNamePages[PageLabel]);
    m_ByFctNamePages.erase(PageLabel);
  }
}

// =====================================================================
// =====================================================================


void ModelParamsPanel::setCurrentPage(std::string PageLabel)
{
  if (m_ByFctNamePages.find(PageLabel) != m_ByFctNamePages.end())
  {
    mp_Notebook->set_current_page(mp_Notebook->page_num(
        *m_ByFctNamePages[PageLabel]));
  }
}

// =====================================================================
// =====================================================================


Gtk::Widget* ModelParamsPanel::asWidget()
{
  return mp_Notebook;
}
