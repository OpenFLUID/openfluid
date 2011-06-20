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
 \file ModelGeneratorCreationDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelGeneratorCreationDialog.hpp"

#include <gtkmm/radiobuttongroup.h>
#include <gtkmm/table.h>
#include <gtkmm/box.h>
#include <gtkmm/stock.h>

#include <glibmm/i18n.h>

#include <openfluid/guicommon/DialogBoxFactory.hpp>

// =====================================================================
// =====================================================================


ModelGeneratorCreationDialog::ModelGeneratorCreationDialog(
    openfluid::core::CoreRepository& CoreRepos,
    openfluid::machine::ModelInstance* ModelInstance) :
  mp_CoreRepos(&CoreRepos), mp_ModelInstance(ModelInstance)
{
  mp_VarNameEntry = Gtk::manage(new Gtk::Entry());
  mp_VarNameEntry->set_activates_default(true);
  mp_VarNameEntry->signal_changed().connect(sigc::mem_fun(*this,
      &ModelGeneratorCreationDialog::onVarNameEntryChanged));

  mp_ClassCombo = Gtk::manage(new Gtk::ComboBoxText());

  Gtk::RadioButton::Group RadioGrp;

  mp_ScalarRadio = Gtk::manage(
      new Gtk::RadioButton(RadioGrp, _("Scalar Value")));
  mp_VectorRadio = Gtk::manage(new Gtk::RadioButton(RadioGrp,
      _("Vector Value:") + std::string(" ")));

  mp_VarSizeSpin = Gtk::manage(new Gtk::SpinButton());
  mp_VarSizeSpin->set_numeric(true);
  mp_VarSizeSpin->set_increments(1, 1);
  mp_VarSizeSpin->set_range(2.0, 9.0);
  mp_VarSizeSpin->set_activates_default(true);

  Gtk::Table* GenInfoTable = Gtk::manage(new Gtk::Table());
  GenInfoTable->set_row_spacings(10);
  GenInfoTable->set_col_spacings(3);
  GenInfoTable->attach(
      *Gtk::manage(new Gtk::Label(_("Variable name"), 1, 0.5)), 0, 1, 0, 1,
      Gtk::FILL, Gtk::SHRINK);
  GenInfoTable->attach(*mp_VarNameEntry, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
  GenInfoTable->attach(*Gtk::manage(new Gtk::Label(_("Unit class"), 1, 0.5)),
      0, 1, 1, 2, Gtk::FILL, Gtk::SHRINK);
  GenInfoTable->attach(*mp_ClassCombo, 1, 2, 1, 2, Gtk::FILL, Gtk::SHRINK);

  Gtk::HBox* VectorBox = Gtk::manage(new Gtk::HBox());
  VectorBox->pack_start(*mp_VectorRadio, Gtk::PACK_SHRINK);
  VectorBox->pack_start(*Gtk::manage(new Gtk::Label(_("Size"))),
      Gtk::PACK_SHRINK);
  VectorBox->pack_start(*mp_VarSizeSpin, Gtk::PACK_SHRINK);

  mp_Dialog = new Gtk::Dialog(_("Generator creation"));

  mp_InfoBarLabel = Gtk::manage(new Gtk::Label(""));

  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*mp_InfoBarLabel);

  mp_Dialog->get_vbox()->pack_start(*mp_InfoBar, Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*GenInfoTable, Gtk::PACK_SHRINK, 10);
  mp_Dialog->get_vbox()->pack_start(*mp_ScalarRadio, Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*VectorBox, Gtk::PACK_SHRINK, 10);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->show_all_children();

}

// =====================================================================
// =====================================================================


ModelGeneratorCreationDialog::~ModelGeneratorCreationDialog()
{
  delete mp_Dialog;
}

// =====================================================================
// =====================================================================


std::map<std::string, std::string> ModelGeneratorCreationDialog::show()
{
  std::map<std::string, std::string> GenInfo;

  if (mp_CoreRepos->getUnitsGlobally()->empty())
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(
        "You can't create a generator now :\n Domain is empty");
    return GenInfo;
  }

  init();

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    GenInfo["varname"] = mp_VarNameEntry->get_text();
    GenInfo["classname"] = mp_ClassCombo->get_active_text();
    if (mp_ScalarRadio->get_active())
      GenInfo["varsize"] = Glib::ustring::compose("%1", 1);
    else
      GenInfo["varsize"] = Glib::ustring::compose("%1",
          mp_VarSizeSpin->get_value_as_int());
  }

  mp_Dialog->hide();

  return GenInfo;
}

// =====================================================================
// =====================================================================


void ModelGeneratorCreationDialog::init()
{
  mp_ClassCombo->clear();

  for (openfluid::core::UnitsListByClassMap_t::const_iterator it =
      mp_CoreRepos->getUnitsByClass()->begin(); it
      != mp_CoreRepos->getUnitsByClass()->end(); ++it)
  {
    if (it->second.getList()->size() != 0)
      mp_ClassCombo->append_text(it->first);
  }

  mp_ClassCombo->set_active(0);

  mp_ScalarRadio->set_active(true);
  mp_VarNameEntry->set_text("");
  mp_VarSizeSpin->set_value(2);

  m_ExistingVars.clear();

  for (std::list<openfluid::machine::ModelItemInstance*>::const_iterator it =
      mp_ModelInstance->getItems().begin(); it
      != mp_ModelInstance->getItems().end(); ++it)
  {
    std::vector<openfluid::base::SignatureHandledDataItem> Vars =
        (*it)->Signature->HandledData.ProducedVars;

    for (unsigned int i = 0; i < Vars.size(); i++)
      m_ExistingVars.insert(Vars[i].DataName);
  }

  onVarNameEntryChanged();
}

// =====================================================================
// =====================================================================


void ModelGeneratorCreationDialog::onVarNameEntryChanged()
{
  if (mp_VarNameEntry->get_text().empty())
  {
    mp_InfoBarLabel->set_text(_("Variable name can not be empty"));
    mp_InfoBar->set_visible(true);
    mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, false);
    return;
  }

  if (m_ExistingVars.find(mp_VarNameEntry->get_text()) != m_ExistingVars.end())
  {
    mp_InfoBarLabel->set_text(Glib::ustring::compose(
        _("Variable %1 already exists"), mp_VarNameEntry->get_text()));
    mp_InfoBar->set_visible(true);
    mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, false);
    return;
  }

  mp_InfoBarLabel->set_text("");
  mp_InfoBar->set_visible(false);
  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, true);

}
