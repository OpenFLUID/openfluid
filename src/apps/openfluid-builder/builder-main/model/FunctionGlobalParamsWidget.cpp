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
 \file FunctionGlobalParamsWidget.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "FunctionGlobalParamsWidget.hpp"

#include <set>
#include <glibmm/i18n.h>
#include <gtkmm/stock.h>
#include <gtkmm/table.h>
#include <openfluid/fluidx/AdvancedModelDescriptor.hpp>
#include <openfluid/machine/FunctionSignatureRegistry.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>

// =====================================================================
// =====================================================================

GlobalParamRow::GlobalParamRow(
    openfluid::fluidx::AdvancedModelDescriptor& ModelDesc,
    std::string ParamName, std::string ParamValue, std::string ParamUnit) :
    m_ModelDesc(ModelDesc), m_Name(ParamName)
{
  m_ColumnCount = 4;

  Gtk::Label* NameLabel = Gtk::manage(new Gtk::Label(m_Name, 0, 0.5));
  m_RowWidgets.push_back(NameLabel);

  mp_ValueEntry = Gtk::manage(new Gtk::Entry());
  mp_ValueEntry->set_text(ParamValue);
  mp_ValueEntry->signal_changed().connect(
      sigc::mem_fun(*this, &GlobalParamRow::onValueChanged));
  m_RowWidgets.push_back(mp_ValueEntry);

  Gtk::Label* UnitLabel = Gtk::manage(new Gtk::Label(ParamUnit, 0, 0.5));
  m_RowWidgets.push_back(UnitLabel);

  mp_RemoveButton = Gtk::manage(new Gtk::Button());
  mp_RemoveButton->set_image(
      *Gtk::manage(new Gtk::Image(Gtk::Stock::REMOVE, Gtk::ICON_SIZE_BUTTON)));
  mp_RemoveButton->set_relief(Gtk::RELIEF_NONE);
  mp_RemoveButton->set_tooltip_text(_("Remove this global parameter"));
  mp_RemoveButton->signal_clicked().connect(
      sigc::mem_fun(*this, &GlobalParamRow::onRemoveButtonClicked));
  m_RowWidgets.push_back(mp_RemoveButton);
}

// =====================================================================
// =====================================================================

void GlobalParamRow::onValueChanged()
{
  m_ModelDesc.setGlobalParameter(m_Name, mp_ValueEntry->get_text());
  m_signal_valueChangeOccured.emit();
}

// =====================================================================
// =====================================================================

void GlobalParamRow::onRemoveButtonClicked()
{
  m_ModelDesc.eraseGlobalParameter(m_Name);
  m_signal_removeOccured.emit();
}

// =====================================================================
// =====================================================================

sigc::signal<void> GlobalParamRow::signal_removeOccured()
{
  return m_signal_removeOccured;
}

// =====================================================================
// =====================================================================

sigc::signal<void> GlobalParamRow::signal_valueChangeOccured()
{
  return m_signal_valueChangeOccured;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

FunctionGlobalParamsWidget::FunctionGlobalParamsWidget(
    openfluid::fluidx::AdvancedModelDescriptor& ModelDesc) :
    m_Model(ModelDesc)
{
  Gtk::Label* ParamsLabel = Gtk::manage(
      new Gtk::Label(_("Available parameters:")));

  mp_Combo = Gtk::manage(new Gtk::ComboBox());
  mref_ComboModel = Gtk::ListStore::create(m_Columns);
  mref_ComboModel->set_sort_column(m_Columns.m_Name, Gtk::SORT_ASCENDING);
  mp_Combo->set_model(mref_ComboModel);
  mp_Combo->pack_start(m_Columns.m_Name);
  mp_Combo->pack_start(m_Columns.m_Unit);

  mp_AddButton = Gtk::manage(new Gtk::Button(_("Set global")));
  mp_AddButton->set_tooltip_text(_("Set this parameter as global"));
  mp_AddButton->signal_clicked().connect(
      sigc::mem_fun(*this, &FunctionGlobalParamsWidget::onAddButtonClicked));

  Gtk::HBox* TopBox = Gtk::manage(new Gtk::HBox(false, 10));
  TopBox->pack_start(*ParamsLabel, Gtk::PACK_SHRINK);
  TopBox->pack_start(*mp_Combo, Gtk::PACK_SHRINK);
  TopBox->pack_start(*mp_AddButton, Gtk::PACK_SHRINK);

  mp_Table = Gtk::manage(new Gtk::Table());
  mp_Table->set_col_spacings(10);
  mp_Table->set_border_width(5);

  pack_start(*TopBox, Gtk::PACK_SHRINK);
  pack_start(*mp_Table, Gtk::PACK_EXPAND_WIDGET);

  update();

  set_visible(true);
  show_all_children();
}

// =====================================================================
// =====================================================================

FunctionGlobalParamsWidget::~FunctionGlobalParamsWidget()
{
}

// =====================================================================
// =====================================================================

void FunctionGlobalParamsWidget::update()
{
  mref_ComboModel->clear();

  int TableWidgetCount = mp_Table->children().size();
  for (int i = 0; i < TableWidgetCount; i++)
    mp_Table->remove(*mp_Table->children().begin()->get_widget());

  m_CurrentTableBottom = 0;

  openfluid::machine::FunctionSignatureRegistry* Reg =
      openfluid::machine::FunctionSignatureRegistry::getInstance();

  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      m_Model.getItems();

  std::map<std::string, std::string> ModelGlobalParams =
      openfluid::fluidx::WareDescriptor::getParamsAsMap(
          m_Model.getGlobalParameters());

  std::set<std::string> TreatedParams;

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    openfluid::machine::ModelItemSignatureInstance* Sign =
        Reg->getSignatureItemInstance(*it);

    if (Sign)
    {
      std::vector<openfluid::ware::SignatureHandledDataItem> Items =
          Sign->Signature->HandledData.FunctionParams;

      for (std::vector<openfluid::ware::SignatureHandledDataItem>::iterator it =
          Items.begin(); it != Items.end(); ++it)
      {
        if (TreatedParams.count(it->DataName))
          continue;

        std::map<std::string, std::string>::iterator Found =
            ModelGlobalParams.find(it->DataName);

        if (Found == ModelGlobalParams.end())
        {
          Gtk::TreeModel::Row Row = *(mref_ComboModel->append());
          Row[m_Columns.m_Name] = it->DataName;
//          Row[m_Columns.m_Unit] = "(" + it->DataUnit + ")";
        }
        else
          attachRow(
              new GlobalParamRow(m_Model, it->DataName, Found->second,
                                 it->DataUnit));

        TreatedParams.insert(it->DataName);
      }
    }

  }

  mp_Combo->set_active(0);

  mp_AddButton->set_sensitive(!mp_Combo->get_model()->children().empty());

  mp_Table->show_all_children();
}

// =====================================================================
// =====================================================================

void FunctionGlobalParamsWidget::attachRow(GlobalParamRow* Row)
{
  for (unsigned int i = 0; i < Row->getColumnCount(); i++)
  {
    mp_Table->attach(*Row->getWidgets()[i], i, i + 1, m_CurrentTableBottom,
                     m_CurrentTableBottom + 1,
                     i == 1 ? Gtk::EXPAND | Gtk::FILL : Gtk::SHRINK | Gtk::FILL,
                     Gtk::FILL, 0, 0);
  }

  Row->signal_removeOccured().connect(
      sigc::mem_fun(*this,
                    &FunctionGlobalParamsWidget::onStructureChangeOccured));
  Row->signal_valueChangeOccured().connect(
      sigc::mem_fun(*this, &FunctionGlobalParamsWidget::onValueChangeOccured));

  m_CurrentTableBottom++;
}

// =====================================================================
// =====================================================================

void FunctionGlobalParamsWidget::onAddButtonClicked()
{
  Gtk::TreeModel::iterator Iter = mp_Combo->get_active();
  if (Iter)
  {
    m_Model.setGlobalParameter(Iter->get_value(m_Columns.m_Name), "");
    onStructureChangeOccured();
  }
}

// =====================================================================
// =====================================================================

void FunctionGlobalParamsWidget::onStructureChangeOccured()
{
  update();
  m_signal_changeOccured.emit();
}

// =====================================================================
// =====================================================================

void FunctionGlobalParamsWidget::onValueChangeOccured()
{
  m_signal_changeOccured.emit();
}

// =====================================================================
// =====================================================================

sigc::signal<void> FunctionGlobalParamsWidget::signal_changeOccured()
{
  return m_signal_changeOccured;
}

// =====================================================================
// =====================================================================

