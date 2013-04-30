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
 \file FunctionAddGlobalParamDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "FunctionAddGlobalParamDialog.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/label.h>
#include <gtkmm/table.h>
#include <gtkmm/stock.h>
#include <gtkmm/frame.h>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include "EngineHelper.hpp"
#include "BuilderFrame.hpp"

// =====================================================================
// =====================================================================

FunctionAddGlobalParamDialog::FunctionAddGlobalParamDialog(
    openfluid::fluidx::AdvancedModelDescriptor& Model) :
    m_Model(Model)
{
  mp_Dialog = new Gtk::Dialog(_("Add of global parameter"));
  mp_Dialog->set_default_size(10, 10);

  mp_InfoBarLabel = Gtk::manage(new Gtk::Label());

  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*mp_InfoBarLabel);

  Gtk::RadioButtonGroup BtGroup;

  mp_ExistingBt = Gtk::manage(
      new Gtk::RadioButton(BtGroup, _("Function available parameters name: ")));
  mp_ExistingBt->signal_toggled().connect(
      sigc::mem_fun(*this, &FunctionAddGlobalParamDialog::onToggled));
  mp_NewBt = Gtk::manage(
      new Gtk::RadioButton(BtGroup, _("New parameter name: ")));
  mp_NewBt->signal_toggled().connect(
      sigc::mem_fun(*this, &FunctionAddGlobalParamDialog::onToggled));

  mp_Combo = Gtk::manage(new Gtk::ComboBox());
  mref_ComboModel = Gtk::ListStore::create(m_Columns);
  mref_ComboModel->set_sort_column(m_Columns.m_Name, Gtk::SORT_ASCENDING);
  mp_Combo->set_model(mref_ComboModel);
  mp_Combo->pack_start(m_Columns.m_Name);
  mp_Combo->pack_start(m_Columns.m_Unit);
  mp_Combo->signal_changed().connect(
      sigc::mem_fun(*this, &FunctionAddGlobalParamDialog::onChanged));

  mp_NameEntry = Gtk::manage(new Gtk::Entry());
  mp_NameEntry->signal_changed().connect(
      sigc::mem_fun(*this, &FunctionAddGlobalParamDialog::onChanged));
  mp_NameEntry->set_activates_default(true);

  Gtk::Table* Table = Gtk::manage(new Gtk::Table());
  Table->set_spacings(5);
  Table->set_border_width(3);
  Table->attach(*mp_ExistingBt, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::FILL);
  Table->attach(*mp_Combo, 1, 2, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::FILL);
  Table->attach(*mp_NewBt, 0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL, Gtk::FILL);
  Table->attach(*mp_NameEntry, 1, 2, 1, 2, Gtk::SHRINK | Gtk::FILL, Gtk::FILL);

  BuilderFrame* Frame = Gtk::manage(new BuilderFrame());
  Frame->setLabelText("Parameter name");
  Frame->add(*Table);

  Gtk::Label* ValueLabel = Gtk::manage(
      new Gtk::Label("", Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));
  ValueLabel->set_markup(
      Glib::ustring::compose("<b>%1</b>", _("Parameter value:")));

  mp_ValueEntry = Gtk::manage(new Gtk::Entry());
  mp_ValueEntry->set_activates_default(true);

  Gtk::Box* ValueBox = Gtk::manage(new Gtk::HBox(false, 5));
  ValueBox->set_border_width(5);
  ValueBox->pack_start(*ValueLabel, Gtk::PACK_SHRINK);
  ValueBox->pack_start(*mp_ValueEntry);

  mp_Dialog->get_vbox()->pack_start(*mp_InfoBar, Gtk::PACK_SHRINK, 5);
  mp_Dialog->get_vbox()->pack_start(*Frame, Gtk::PACK_SHRINK, 5);
  mp_Dialog->get_vbox()->pack_start(*ValueBox, Gtk::PACK_SHRINK, 5);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================

FunctionAddGlobalParamDialog::~FunctionAddGlobalParamDialog()
{
  delete mp_Dialog;
}

// =====================================================================
// =====================================================================

void FunctionAddGlobalParamDialog::onToggled()
{
  mp_Combo->set_sensitive(mp_ExistingBt->get_active());
  mp_NameEntry->set_sensitive(mp_NewBt->get_active());
  onChanged();
}

// =====================================================================
// =====================================================================

void FunctionAddGlobalParamDialog::onChanged()
{
  std::string Name;

  if (mp_ExistingBt->get_active())
  {
    Gtk::TreeModel::iterator Iter = mp_Combo->get_active();
    if (Iter)
      Name = Iter->get_value(m_Columns.m_Name);
  }
  else
    Name = mp_NameEntry->get_text();

  bool IsValid = false;

  if (EngineHelper::isEmptyString(Name))
  {
    mp_InfoBarLabel->set_text(_("Parameter name can not be empty"));
  }
  else if (mp_DummyItem->hasParameter(Name))
  {
    mp_InfoBarLabel->set_text(_("Parameter name already exists"));
  }
  else if (!openfluid::ware::PluggableWare::isWellFormated(Name))
  {
    mp_InfoBarLabel->set_text(_("Parameter name doesn't respect naming rules"));
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

bool FunctionAddGlobalParamDialog::show()
{
  // create a dummy item to store global parameters (only for "onChange" tests)
  mp_DummyItem = new openfluid::fluidx::WareDescriptor();
  mp_DummyItem->setParameters(m_Model.getGlobalParameters());

  updateCombo();
  mp_NameEntry->set_text("");
  mp_ValueEntry->set_text("");
  if (!mref_ComboModel->children().empty())
    mp_ExistingBt->set_active(true);
  else
    mp_NewBt->set_active(true);
  onToggled();
  onChanged();

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    std::string Name;

    if (mp_ExistingBt->get_active())
    {
      Gtk::TreeModel::iterator Iter = mp_Combo->get_active();
      if (Iter)
        Name = Iter->get_value(m_Columns.m_Name);
    }
    else
      Name = mp_NameEntry->get_text();

    m_Model.setGlobalParameter(Name, mp_ValueEntry->get_text());

    mp_Dialog->hide();
    return true;
  }

  mp_Dialog->hide();
  return false;
}

// =====================================================================
// =====================================================================

void FunctionAddGlobalParamDialog::updateCombo()
{
  mref_ComboModel->clear();

  openfluid::machine::SimulatorSignatureRegistry* Reg =
      openfluid::machine::SimulatorSignatureRegistry::getInstance();

  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      m_Model.getItems();

  openfluid::ware::WareParams_t ModelGlobalParams =
      m_Model.getGlobalParameters();

  std::set<std::string> TreatedParams;

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    openfluid::machine::ModelItemSignatureInstance* Sign =
        Reg->getSignatureItemInstance(*it);

    if (Sign)
    {
      std::vector<openfluid::ware::SignatureHandledDataItem> Items =
          Sign->Signature->HandledData.SimulatorParams;

      for (std::vector<openfluid::ware::SignatureHandledDataItem>::iterator it =
          Items.begin(); it != Items.end(); ++it)
      {
        if (TreatedParams.count(it->DataName))
          continue;

        openfluid::ware::WareParams_t::iterator Found = ModelGlobalParams.find(
            it->DataName);

        if (Found == ModelGlobalParams.end())
        {
          Gtk::TreeModel::Row Row = *(mref_ComboModel->append());
          Row[m_Columns.m_Name] = it->DataName;
//          Row[m_Columns.m_Unit] = "(" + it->DataUnit + ")";
        }

        TreatedParams.insert(it->DataName);
      }
    }

  }

  mp_Combo->set_active(0);
}

// =====================================================================
// =====================================================================

