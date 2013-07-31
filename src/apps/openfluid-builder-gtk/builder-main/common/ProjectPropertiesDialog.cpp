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
 \file ProjectPropertiesDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ProjectPropertiesDialog.hpp"

#include <glibmm/i18n.h>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/table.h>
#include <gtkmm/stock.h>

#include <openfluid/base/ProjectManager.hpp>

// =====================================================================
// =====================================================================


ProjectPropertiesDialog::ProjectPropertiesDialog()
{
  mp_NameValueLabel = Gtk::manage(new Gtk::Label("", 0, 0.5));
  mp_PathValueLabel = Gtk::manage(new Gtk::Label("", 0, 0.5));

  mp_DescriptionTextView = Gtk::manage(new Gtk::TextView());
  mp_DescriptionTextView->get_buffer()->signal_changed().connect(sigc::mem_fun(
      *this, &ProjectPropertiesDialog::onDescriptionChanged));

  Gtk::ScrolledWindow* DescriptionWin = Gtk::manage(new Gtk::ScrolledWindow());
  DescriptionWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  DescriptionWin->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
  DescriptionWin->add(*mp_DescriptionTextView);
  DescriptionWin->set_visible(true);

  mp_AuthorsEntry = Gtk::manage(new Gtk::Entry());
  mp_AuthorsEntry->set_activates_default(true);
  mp_AuthorsEntry->signal_changed().connect(sigc::mem_fun(*this,
      &ProjectPropertiesDialog::onAuthorsChanged));

  mp_CreationDateValueLabel = Gtk::manage(new Gtk::Label("", 0, 0.5));
  mp_ModificationDateValueLabel = Gtk::manage(new Gtk::Label("", 0, 0.5));

  mp_IncrementalOutputCheckButton = Gtk::manage(new Gtk::CheckButton());
  mp_IncrementalOutputCheckButton->signal_clicked().connect(sigc::mem_fun(
      *this, &ProjectPropertiesDialog::onIncrementalOutputChanged));

  Gtk::Table* MainTable = Gtk::manage(new Gtk::Table());
  MainTable->set_spacings(5);
  MainTable->set_border_width(5);
  MainTable->attach(*Gtk::manage(new Gtk::Label(_("Project name:"), 1, 0.5)),
      0, 1, 0, 1, Gtk::FILL, Gtk::FILL);
  MainTable->attach(*mp_NameValueLabel, 1, 2, 0, 1);
  MainTable->attach(*Gtk::manage(new Gtk::Label(_("Project path:"), 1, 0.5)),
      0, 1, 1, 2, Gtk::FILL, Gtk::FILL);
  MainTable->attach(*mp_PathValueLabel, 1, 2, 1, 2);
  MainTable->attach(*Gtk::manage(
      new Gtk::Label(_("Project description:"), 1, 0)), 0, 1, 2, 3, Gtk::FILL,
      Gtk::FILL);
  MainTable->attach(*DescriptionWin, 1, 2, 2, 3);
  MainTable->attach(
      *Gtk::manage(new Gtk::Label(_("Project authors:"), 1, 0.5)), 0, 1, 3, 4,
      Gtk::FILL, Gtk::SHRINK);
  MainTable->attach(*mp_AuthorsEntry, 1, 2, 3, 4);
  MainTable->attach(*Gtk::manage(new Gtk::Label(_("Creation date:"), 1, 0.5)),
      0, 1, 4, 5, Gtk::FILL, Gtk::SHRINK);
  MainTable->attach(*mp_CreationDateValueLabel, 1, 2, 4, 5);
  MainTable->attach(*Gtk::manage(new Gtk::Label(_("Last modification date:"),
      1, 0.5)), 0, 1, 5, 6, Gtk::FILL, Gtk::SHRINK);
  MainTable->attach(*mp_ModificationDateValueLabel, 1, 2, 5, 6);
  MainTable->attach(*Gtk::manage(new Gtk::Label(_("Enable incremental outputs:"),
      1, 0.5)), 0, 1, 6, 7, Gtk::FILL, Gtk::SHRINK);
  MainTable->attach(*mp_IncrementalOutputCheckButton, 1, 2, 6, 7);
  MainTable->set_border_width(8);

  mp_Dialog = new Gtk::Dialog(_("Project properties"));
  mp_Dialog->set_has_separator(true);
  mp_Dialog->get_vbox()->pack_start(*MainTable, Gtk::PACK_SHRINK, 5);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->show_all_children();

}

// =====================================================================
// =====================================================================


ProjectPropertiesDialog::~ProjectPropertiesDialog()
{
  delete mp_Dialog;
}

// =====================================================================
// =====================================================================


void ProjectPropertiesDialog::onDescriptionChanged()
{
  m_Description = mp_DescriptionTextView->get_buffer()->get_text();
}

// =====================================================================
// =====================================================================


void ProjectPropertiesDialog::onIncrementalOutputChanged()
{
  m_IsIncrementalOutput = mp_IncrementalOutputCheckButton->get_active();
}

// =====================================================================
// =====================================================================


void ProjectPropertiesDialog::onAuthorsChanged()
{
  m_Authors = mp_AuthorsEntry->get_text();
}

// =====================================================================
// =====================================================================


void ProjectPropertiesDialog::show()
{
  openfluid::base::ProjectManager* PjMgr =
      openfluid::base::ProjectManager::getInstance();

  m_Description = PjMgr->getDescription();
  m_Authors = PjMgr->getAuthors();
  m_IsIncrementalOutput = PjMgr->isIncrementalOutputDir();

  mp_NameValueLabel->set_text(PjMgr->getName());
  mp_PathValueLabel->set_text(PjMgr->getPath());
  mp_DescriptionTextView->get_buffer()->set_text(m_Description);
  mp_AuthorsEntry->set_text(m_Authors);
  mp_CreationDateValueLabel->set_text(boost::posix_time::to_simple_string(
      boost::posix_time::from_iso_string(PjMgr->getCreationDate())));
  mp_ModificationDateValueLabel->set_text(boost::posix_time::to_simple_string(
      boost::posix_time::from_iso_string(PjMgr->getLastModDate())));
  mp_IncrementalOutputCheckButton->set_active(m_IsIncrementalOutput);

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    PjMgr->setDescription(m_Description);
    PjMgr->setAuthors(m_Authors);
    PjMgr->setIncrementalOutputDir(m_IsIncrementalOutput);

    PjMgr->save();
  }

  mp_Dialog->hide();
}
