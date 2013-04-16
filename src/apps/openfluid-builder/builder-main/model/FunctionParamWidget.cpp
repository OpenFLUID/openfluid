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
 \file FunctionParamWidget
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "FunctionParamWidget.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>
#include <gtkmm/table.h>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include "EngineHelper.hpp"

// =====================================================================
// =====================================================================

FunctionParamRow::FunctionParamRow(
    openfluid::fluidx::ModelItemDescriptor& FctDesc, std::string ParamName,
    std::string ParamValue, std::string ParamUnit, std::string ParamDescription,
    bool WithRemoveBt) :
    m_FctDesc(FctDesc), m_Name(ParamName)
{
  m_ColumnCount = 5;

  std::string DescriptionTooltip =
      ParamDescription.empty() ? _("no description") : ParamDescription;

  Gtk::Label* NameLabel = Gtk::manage(new Gtk::Label(m_Name, 0, 0.5));
  NameLabel->set_tooltip_text(DescriptionTooltip);
  m_RowWidgets.push_back(NameLabel);

  mp_ValueEntry = Gtk::manage(new Gtk::Entry());
  mp_ValueEntry->set_text(ParamValue);
  mp_ValueEntry->set_tooltip_text(DescriptionTooltip);
  mp_ValueEntry->signal_changed().connect(
      sigc::mem_fun(*this, &FunctionParamRow::onValueChanged));
  m_RowWidgets.push_back(mp_ValueEntry);

  Gtk::Label* UnitLabel = Gtk::manage(new Gtk::Label(ParamUnit, 0, 0.5));
  m_RowWidgets.push_back(UnitLabel);

  mp_GlobalLabel = Gtk::manage(new Gtk::Label("", 0, 0.5));
  mp_GlobalLabel->set_sensitive(EngineHelper::isEmptyString(ParamValue));
  m_RowWidgets.push_back(mp_GlobalLabel);

  if (WithRemoveBt)
  {
    mp_RemoveButton = Gtk::manage(new Gtk::Button());
    mp_RemoveButton->set_image(
        *Gtk::manage(
            new Gtk::Image(Gtk::Stock::REMOVE, Gtk::ICON_SIZE_BUTTON)));
    mp_RemoveButton->set_relief(Gtk::RELIEF_NONE);
    mp_RemoveButton->set_tooltip_text(
        Glib::ustring::compose(_("Remove %1"), m_Name));
    mp_RemoveButton->signal_clicked().connect(
        sigc::mem_fun(*this, &FunctionParamRow::onRemoveButtonClicked));
    m_RowWidgets.push_back(mp_RemoveButton);
  }
  else
    m_RowWidgets.push_back(new Gtk::HBox());
}

// =====================================================================
// =====================================================================

void FunctionParamRow::setGlobalValue(std::string GlobalValue)
{
  if (GlobalValue.empty())
    mp_GlobalLabel->set_text("");
  else
    mp_GlobalLabel->set_text(
        Glib::ustring::compose("(%1 \"%2\")", _("global value:"), GlobalValue));
}

// =====================================================================
// =====================================================================

void FunctionParamRow::onValueChanged()
{
  std::string NewValue = mp_ValueEntry->get_text();

  m_FctDesc.setParameter(m_Name, NewValue);

  mp_GlobalLabel->set_sensitive(EngineHelper::isEmptyString(NewValue));

  m_signal_valueChangeOccured.emit();
}

// =====================================================================
// =====================================================================

void FunctionParamRow::onRemoveButtonClicked()
{
  m_FctDesc.eraseParameter(m_Name);
  m_signal_removeOccured.emit();
}

// =====================================================================
// =====================================================================

sigc::signal<void> FunctionParamRow::signal_removeOccured()
{
  return m_signal_removeOccured;
}

// =====================================================================
// =====================================================================

sigc::signal<void> FunctionParamRow::signal_valueChangeOccured()
{
  return m_signal_valueChangeOccured;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

FunctionParamWidget::FunctionParamWidget(
    openfluid::fluidx::ModelItemDescriptor& FctDesc,
    openfluid::machine::ModelItemSignatureInstance* Sign,
    FunctionAddParamDialog& AddParamDialog) :
    m_FctDesc(FctDesc), mp_Sign(Sign), m_AddParamDialog(AddParamDialog)
{
  Gtk::Button* AddButton = Gtk::manage(new Gtk::Button());
  AddButton->set_image(
      *Gtk::manage(new Gtk::Image(Gtk::Stock::ADD, Gtk::ICON_SIZE_BUTTON)));
  AddButton->set_tooltip_text(_("Add a parameter"));
  AddButton->signal_clicked().connect(
      sigc::mem_fun(*this, &FunctionParamWidget::onAddButtonClicked));

  Gtk::HBox* ButtonBox = Gtk::manage(new Gtk::HBox());
  ButtonBox->pack_start(*AddButton, Gtk::PACK_SHRINK, 0);

  mp_Table = Gtk::manage(new Gtk::Table());
  mp_Table->set_col_spacings(10);
  mp_Table->set_border_width(5);

  pack_start(*ButtonBox, Gtk::PACK_SHRINK);
  pack_start(*mp_Table, Gtk::PACK_EXPAND_WIDGET);

  updateRows();

  set_visible(true);
  show_all_children();
}

// =====================================================================
// =====================================================================

FunctionParamWidget::~FunctionParamWidget()
{

}

// =====================================================================
// =====================================================================

void FunctionParamWidget::updateRows()
{
  int TableWidgetCount = mp_Table->children().size();
  for (int i = 0; i < TableWidgetCount; i++)
    mp_Table->remove(*mp_Table->children().begin()->get_widget());

  m_Rows.clear();

  m_CurrentTableBottom = 0;

  std::map<std::string, std::string> Params = m_FctDesc.getParametersAsMap();

  if (mp_Sign)
  {
    std::vector<openfluid::ware::SignatureHandledDataItem> Items =
        mp_Sign->Signature->HandledData.FunctionParams;
    for (std::vector<openfluid::ware::SignatureHandledDataItem>::iterator it =
        Items.begin(); it != Items.end(); ++it)
    {
      attachRow(
          new FunctionParamRow(m_FctDesc, it->DataName, Params[it->DataName],
                               it->DataUnit, it->Description, false),
          it->DataName);

      Params.erase(it->DataName);
    }
  }

  // add any parameter that was set but not present in signature
  for (std::map<std::string, std::string>::iterator it = Params.begin();
      it != Params.end(); ++it)
  {
    attachRow(
        new FunctionParamRow(m_FctDesc, it->first, it->second, "", "", true),
        it->first);
  }

  if (m_Rows.empty())
  {
    Gtk::Label* EmptyLabel = Gtk::manage(new Gtk::Label("", 0.5, 0));
    EmptyLabel->set_markup(_("<i>No parameters</i>"));
    mp_Table->attach(*EmptyLabel, 0, 1, 0, 1);
  }

  mp_Table->show_all_children();
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::attachRow(FunctionParamRow* Row,
                                    std::string ParamName)
{
  for (unsigned int i = 0; i < Row->getColumnCount(); i++)
  {
    mp_Table->attach(*Row->getWidgets()[i], i, i + 1, m_CurrentTableBottom,
                     m_CurrentTableBottom + 1,
                     i == 1 ? Gtk::EXPAND | Gtk::FILL : Gtk::SHRINK | Gtk::FILL,
                     Gtk::FILL, 0, 0);
  }

  Row->signal_removeOccured().connect(
      sigc::mem_fun(*this, &FunctionParamWidget::onStructureChangeOccured));
  Row->signal_valueChangeOccured().connect(
      sigc::mem_fun(*this, &FunctionParamWidget::onValueChangeOccured));

  m_CurrentTableBottom++;

  m_Rows[ParamName] = Row;
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::onAddButtonClicked()
{
  if (m_AddParamDialog.show(&m_FctDesc, mp_Sign))
    onStructureChangeOccured();
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::updateGlobals(
    const std::map<std::string, std::string>& GlobalParams)
{
  m_Globals = GlobalParams;

  updateGlobals();
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::updateGlobals()
{
  for (std::map<std::string, FunctionParamRow*>::iterator it = m_Rows.begin();
      it != m_Rows.end(); ++it)
  {
    std::map<std::string, std::string>::const_iterator Found = m_Globals.find(
        it->first);

    it->second->setGlobalValue(Found != m_Globals.end() ? Found->second : "");
  }
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::onStructureChangeOccured()
{
  updateRows();
  updateGlobals();
  m_signal_changeOccured.emit();
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::onValueChangeOccured()
{
  m_signal_changeOccured.emit();
}

// =====================================================================
// =====================================================================

sigc::signal<void> FunctionParamWidget::signal_changeOccured()
{
  return m_signal_changeOccured;
}

// =====================================================================
// =====================================================================

