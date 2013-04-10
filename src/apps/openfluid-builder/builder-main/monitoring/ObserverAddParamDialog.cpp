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
 \file ObserverAddParamDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "ObserverAddParamDialog.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/label.h>
#include <gtkmm/table.h>
#include <gtkmm/stock.h>
#include <openfluid/machine/ObserverInstance.hpp>
#include "EngineHelper.hpp"

// =====================================================================
// =====================================================================

ObserverAddParamDialog::ObserverAddParamDialog()
{
  mp_Dialog = new Gtk::Dialog(_("Add of observer parameter"));
  mp_Dialog->set_default_size(10, 10);

  mp_InfoBarLabel = Gtk::manage(new Gtk::Label());

  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*mp_InfoBarLabel);

  Gtk::Label* NameLabel = Gtk::manage(
      new Gtk::Label(_("Parameter name:"), Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));
  Gtk::Label* ValueLabel = Gtk::manage(
      new Gtk::Label(_("Parameter value:"), Gtk::ALIGN_LEFT,
                     Gtk::ALIGN_CENTER));

  mp_NameEntry = Gtk::manage(new Gtk::Entry());
  mp_NameEntry->signal_changed().connect(
      sigc::mem_fun(*this, &ObserverAddParamDialog::onChanged));
  mp_NameEntry->set_activates_default(true);

  mp_ValueEntry = Gtk::manage(new Gtk::Entry());
  mp_ValueEntry->set_activates_default(true);

  Gtk::Table* Table = Gtk::manage(new Gtk::Table());
  Table->set_col_spacings(3);
  Table->attach(*NameLabel, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL);
  Table->attach(*mp_NameEntry, 1, 2, 0, 1);
  Table->attach(*ValueLabel, 0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL);
  Table->attach(*mp_ValueEntry, 1, 2, 1, 2);

  mp_Dialog->get_vbox()->pack_start(*mp_InfoBar, Gtk::PACK_SHRINK, 5);
  mp_Dialog->get_vbox()->pack_start(*Table, Gtk::PACK_SHRINK, 5);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================

ObserverAddParamDialog::~ObserverAddParamDialog()
{
}

// =====================================================================
// =====================================================================

void ObserverAddParamDialog::onChanged()
{
  std::string Name = mp_NameEntry->get_text();

  bool IsValid = false;

  if (EngineHelper::isEmptyString(Name))
  {
    mp_InfoBarLabel->set_text(_("Parameter name can not be empty"));
  }
  else if (mp_Obs->hasParameter(Name))
  {
    mp_InfoBarLabel->set_text(_("Parameter name already exists"));
  }
  else if (!mp_Obs->isInsertable(Name))
  {
    mp_InfoBarLabel->set_text(_("Parameter name doesn't respect xml rules"));
  }
  else
  {
    IsValid = true;
  }

  mp_InfoBar->set_visible(!IsValid);
  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, IsValid);
  mp_Dialog->resize(10, 10);
}

// =====================================================================
// =====================================================================

bool ObserverAddParamDialog::show(openfluid::fluidx::ObserverDescriptor* Obs)
{
  if (!Obs)
  {
    mp_Dialog->hide();
    return false;
  }

  mp_Obs = Obs;

  mp_NameEntry->set_text("");
  onChanged();

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    mp_Obs->setParameter(mp_NameEntry->get_text(), mp_ValueEntry->get_text());

    mp_Dialog->hide();
    return true;
  }

  mp_Dialog->hide();
  return false;
}

// =====================================================================
// =====================================================================

