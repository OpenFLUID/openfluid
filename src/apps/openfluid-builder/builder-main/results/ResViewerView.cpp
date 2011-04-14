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
 \file ResViewerView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ResViewerView.hpp"

#include <glibmm/i18n.h>

#include <iostream>

// =====================================================================
// =====================================================================


void ResViewerViewImpl::onSelectionChanged()
{
  m_signal_FileSelectionChanged.emit();
}

// =====================================================================
// =====================================================================


ResViewerViewImpl::ResViewerViewImpl()
{
  mp_MainBox = Gtk::manage(new Gtk::VBox());

  mp_TitleLabel = Gtk::manage(new Gtk::Label());

  mp_TreeView = Gtk::manage(new Gtk::TreeView());

  Gtk::ScrolledWindow* Win = Gtk::manage(new Gtk::ScrolledWindow());
  Win->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  Win->add(*mp_TreeView);

  mp_MainBox->pack_start(*Win);

  mp_MainBox->show_all_children();

  mp_Notebook = Gtk::manage(new Gtk::Notebook());
  mp_Notebook->set_scrollable(true);
  mp_Notebook->popup_enable();
  mp_Notebook->append_page(*mp_MainBox, _("tabular"));
}

// =====================================================================
// =====================================================================


sigc::signal<void> ResViewerViewImpl::signal_FileSelectionChanged()
{
  return m_signal_FileSelectionChanged;
}

// =====================================================================
// =====================================================================


void ResViewerViewImpl::setColumns(ResViewerColumns* Columns)
{
  mp_TreeView->remove_all_columns();

  if (Columns)
  {
    mp_TreeView->append_column(_("Time Step"), *Columns->getStepColumn());
    mp_TreeView->get_column(0)->set_sort_column(*Columns->getStepColumn());

    mp_TreeView->append_column(_("Date Time"), *Columns->getDateColumn());
    mp_TreeView->get_column(1)->set_sort_column(*Columns->getDateColumn());

    std::map<std::string, Gtk::TreeModelColumn<std::string>*> ColumnsByTitle =
        Columns->getByTitleColumns();

    for (std::map<std::string, Gtk::TreeModelColumn<std::string>*>::iterator
        it = ColumnsByTitle.begin(); it != ColumnsByTitle.end(); ++it)
    {
      int ColNb = mp_TreeView->append_column(it->first, *it->second);
      mp_TreeView->get_column(ColNb - 1)->set_sort_column(*it->second);
    }
  }
}

// =====================================================================
// =====================================================================


void ResViewerViewImpl::setModel(Glib::RefPtr<Gtk::TreeModel> Model)
{
  mp_TreeView->set_model(Model);
}

// =====================================================================
// =====================================================================


void ResViewerViewImpl::setTitle(std::string Text)
{
  mp_TitleLabel->set_text(Text);
}

// =====================================================================
// =====================================================================


void ResViewerViewImpl::setFileContentsByName(std::map<std::string,
    Glib::RefPtr<Gtk::TextBuffer> > FileContents)
{
  while (mp_Notebook->get_n_pages() > 1)
    mp_Notebook->remove_page(1);

  for (std::map<std::string, Glib::RefPtr<Gtk::TextBuffer> >::iterator it =
      FileContents.begin(); it != FileContents.end(); ++it)
  {
    Gtk::TextView* TextView = Gtk::manage(new Gtk::TextView(it->second));
    TextView->set_editable(false);
    TextView->set_visible(true);

    Gtk::ScrolledWindow* Win = Gtk::manage(new Gtk::ScrolledWindow());
    Win->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    Win->set_visible(true);
    Win->add(*TextView);

    Gtk::Label* TabLabel = Gtk::manage(new Gtk::Label(it->first));
    Gtk::Label* MenuLabel = Gtk::manage(new Gtk::Label(it->first,
        Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));

    mp_Notebook->append_page(*Win, *TabLabel, *MenuLabel);
    mp_Notebook->set_tab_reorderable(*Win, true);
  }
}

// =====================================================================
// =====================================================================


Gtk::Widget* ResViewerViewImpl::asWidget()
{
  return mp_Notebook;
}
