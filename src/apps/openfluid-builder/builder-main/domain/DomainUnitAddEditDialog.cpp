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
 \file DomainUnitAddEditDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainUnitAddEditDialog.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>

#include <openfluid/fluidx/UnitDescriptor.hpp>

#include "EngineHelper.hpp"
#include "DomainUnitRelationAddDialog.hpp"
#include "DomainUnitRelationWidget.hpp"
#include <openfluid/fluidx/AdvancedDomainDescriptor.hpp>

// =====================================================================
// =====================================================================

DomainUnitAddEditDialog::DomainUnitAddEditDialog(
    DomainUnitRelationAddDialog& UnitRelationAddDialog,
    openfluid::fluidx::AdvancedDomainDescriptor& Domain) :
    mp_Domain(&Domain), m_AddDialog(UnitRelationAddDialog), mp_Unit(0)
{
  mp_InfoBarLabel = Gtk::manage(new Gtk::Label());
  mp_InfoBarLabel->set_visible(true);

  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*mp_InfoBarLabel);

  mp_ClassLabel = Gtk::manage(new Gtk::Label("", 0, 0.5));

  mp_IdLabel = Gtk::manage(new Gtk::Label("", 0, 0.5));

  mp_ClassComboEntryText = Gtk::manage(new Gtk::ComboBoxEntryText());
  mp_ClassComboEntryText->get_entry()->signal_changed().connect(
      sigc::mem_fun(*this, &DomainUnitAddEditDialog::onClassChanged));
  mp_ClassComboEntryText->get_entry()->set_activates_default(true);

  mp_IdSpin = Gtk::manage(new Gtk::SpinButton());
  mp_IdSpin->set_range(1.0, 9999.0);
  mp_IdSpin->set_increments(1, 10);
  mp_IdSpin->set_numeric(true);
  mp_IdSpin->signal_changed().connect(
      sigc::mem_fun(*this, &DomainUnitAddEditDialog::onIdChanged));
  mp_IdSpin->set_activates_default(true);

  mp_PcsOrderSpin = Gtk::manage(new Gtk::SpinButton());
  mp_PcsOrderSpin->set_range(1.0, 9999.0);
  mp_PcsOrderSpin->set_increments(1, 10);
  mp_PcsOrderSpin->set_numeric(true);
  mp_PcsOrderSpin->set_activates_default(true);

  mp_FromWidget = new DomainUnitRelationWidget("From", m_AddDialog);
  mp_ToWidget = new DomainUnitRelationWidget("To", m_AddDialog);
  mp_ParentWidget = new DomainUnitRelationWidget("Parent", m_AddDialog);
  mp_ChildWidget = new DomainUnitRelationWidget("Children", m_AddDialog);

  mp_InfoTable = Gtk::manage(new Gtk::Table());
  mp_InfoTable->attach(*Gtk::manage(new Gtk::Label(_("Unit class:"), 1, 0.5)),
                       0, 1, 0, 1, Gtk::FILL | Gtk::SHRINK, Gtk::SHRINK);
  mp_InfoTable->attach(*Gtk::manage(new Gtk::Label(_("ID:"), 1, 0.5)), 0, 1, 1,
                       2, Gtk::FILL | Gtk::SHRINK, Gtk::SHRINK);
  mp_InfoTable->attach(
      *Gtk::manage(new Gtk::Label(_("Process order:"), 1, 0.5)), 0, 1, 2, 3,
      Gtk::FILL | Gtk::SHRINK, Gtk::SHRINK);
  mp_InfoTable->attach(*mp_ClassComboEntryText, 1, 2, 0, 1,
                       Gtk::FILL | Gtk::SHRINK, Gtk::SHRINK);
  mp_InfoTable->attach(*mp_ClassLabel, 1, 2, 0, 1, Gtk::FILL | Gtk::SHRINK,
                       Gtk::SHRINK);
  mp_InfoTable->attach(*mp_IdSpin, 1, 2, 1, 2, Gtk::FILL | Gtk::SHRINK,
                       Gtk::SHRINK);
  mp_InfoTable->attach(*mp_IdLabel, 1, 2, 1, 2, Gtk::FILL | Gtk::SHRINK,
                       Gtk::SHRINK);
  mp_InfoTable->attach(*mp_PcsOrderSpin, 1, 2, 2, 3, Gtk::FILL | Gtk::SHRINK,
                       Gtk::SHRINK);
  mp_InfoTable->set_col_spacings(3);
  mp_InfoTable->set_row_spacings(5);
  mp_InfoTable->set_border_width(5);
  mp_InfoTable->show_all_children();
  mp_InfoTable->set_visible(true);

  mp_Dialog = new Gtk::Dialog();

  mp_Dialog->get_vbox()->pack_start(*mp_InfoBar, Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*mp_InfoTable, Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*mp_FromWidget->asWidget(),
                                    Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*mp_ToWidget->asWidget(), Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*mp_ParentWidget->asWidget(),
                                    Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*mp_ChildWidget->asWidget(),
                                    Gtk::PACK_SHRINK);

  mp_Dialog->set_default_size(300, 300);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  update();
}

// =====================================================================
// =====================================================================

void DomainUnitAddEditDialog::onClassChanged()
{
  std::string ClassName = mp_ClassComboEntryText->get_entry()->get_text();

  bool IsValid = true;

  if (EngineHelper::isEmptyString(ClassName))
  {
    mp_InfoBarLabel->set_text(_("Class name cannot be empty"));
    IsValid = false;
  }

  mp_InfoBar->set_visible(!IsValid);
  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, IsValid);

  if (IsValid)
  {
    m_IDs = mp_Domain->getIDsOfClass(ClassName);

    if (m_IDs.empty())
      mp_IdSpin->set_value(1);
    else
      mp_IdSpin->set_value((*std::max_element(m_IDs.begin(), m_IDs.end())) + 1);
  }
}

// =====================================================================
// =====================================================================

void DomainUnitAddEditDialog::onIdChanged()
{
  mp_IdSpin->update();

  bool IsValid = true;

  if (m_IDs.count(mp_IdSpin->get_value_as_int()))
  {
    mp_InfoBarLabel->set_text(_("This unit already exists"));
    IsValid = false;
  }

  mp_InfoBar->set_visible(!IsValid);
  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, IsValid);
}

// =====================================================================
// =====================================================================

void DomainUnitAddEditDialog::update()
{
  m_Classes = mp_Domain->getClassNames();

  mp_ClassComboEntryText->clear_items();

  for (std::set<std::string>::iterator it = m_Classes.begin();
      it != m_Classes.end(); ++it)
  {
    mp_ClassComboEntryText->append_text(*it);
  }

}

// =====================================================================
// =====================================================================

openfluid::fluidx::UnitDescriptor* DomainUnitAddEditDialog::show(
    std::string SelectedClass, openfluid::fluidx::UnitDescriptor* Unit)
{
  mp_Unit = Unit;

  if (mp_Unit)
    initEditionMode();
  else
    initCreationMode(SelectedClass);

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    mp_PcsOrderSpin->update();

    if (mp_Unit)
    {
      mp_Unit->getProcessOrder() = mp_PcsOrderSpin->get_value_as_int();
    }
    else
    {
      createUnit();
    }

    clearAllRelations();

    createAllRelationsFromRelationWidgets();

    mp_Dialog->hide();

    return mp_Unit;
  }

  mp_Dialog->hide();

  return (openfluid::fluidx::UnitDescriptor*) 0;
}

// =====================================================================
// =====================================================================

void DomainUnitAddEditDialog::initEditionMode()
{
  mp_Dialog->set_title(_("Unit edition"));

  std::string ClassName = mp_Unit->getUnitClass();
  mp_ClassLabel->set_visible(true);
  mp_ClassLabel->set_text(ClassName);
  mp_ClassComboEntryText->set_visible(false);
  mp_ClassComboEntryText->get_entry()->set_text(ClassName);

  unsigned int Id = mp_Unit->getUnitID();
  mp_IdLabel->set_visible(true);
  mp_IdLabel->set_text(Glib::ustring::compose("%1", Id));
  mp_IdSpin->set_visible(false);
  mp_IdSpin->set_value(Id);

  mp_PcsOrderSpin->set_value(mp_Unit->getProcessOrder());

  mp_InfoBar->set_visible(false);
  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, true);

  mp_FromWidget->clearUnits();
  mp_ToWidget->clearUnits();
  mp_ParentWidget->clearUnits();
  mp_ChildWidget->clearUnits();

  openfluid::core::UnitClassID_t Unit = std::make_pair(mp_Unit->getUnitClass(),
                                                       mp_Unit->getUnitID());

  mp_FromWidget->appendUnits(mp_Domain->getUnitsFromOf(Unit));
  mp_ToWidget->appendUnits(mp_Domain->getUnitsToOf(Unit));
  mp_ParentWidget->appendUnits(mp_Domain->getUnitsParentsOf(Unit));
  mp_ChildWidget->appendUnits(mp_Domain->getUnitsChildrenOf(Unit));
}

// =====================================================================
// =====================================================================

void DomainUnitAddEditDialog::initCreationMode(std::string SelectedClass)
{
  mp_Dialog->set_title(_("Unit creation"));

  mp_ClassLabel->set_visible(false);
  mp_ClassComboEntryText->set_visible(true);
  mp_ClassComboEntryText->get_entry()->set_text(SelectedClass);

  mp_IdLabel->set_visible(false);
  mp_IdSpin->set_visible(true);

  mp_PcsOrderSpin->set_value(1);

  mp_FromWidget->clearUnits();
  mp_ToWidget->clearUnits();
  mp_ParentWidget->clearUnits();
  mp_ChildWidget->clearUnits();

  onClassChanged();
}

// =====================================================================
// =====================================================================

void DomainUnitAddEditDialog::createUnit()
{
  std::string ClassName = mp_ClassComboEntryText->get_entry()->get_text();
  int ID = mp_IdSpin->get_value_as_int();
  int PcsOrder = mp_PcsOrderSpin->get_value_as_int();

  mp_Unit = new openfluid::fluidx::UnitDescriptor;
  mp_Unit->getUnitClass() = ClassName;
  mp_Unit->getUnitID() = ID;
  mp_Unit->getProcessOrder() = PcsOrder;
}

// =====================================================================
// =====================================================================

void DomainUnitAddEditDialog::clearAllRelations()
{
  if (!mp_Unit)
    return;

  openfluid::core::UnitClassID_t Unit = std::make_pair(mp_Unit->getUnitClass(),
                                                       mp_Unit->getUnitID());

  std::list<openfluid::core::UnitClassID_t> Froms = mp_Domain->getUnitsFromOf(
      Unit);
  for (std::list<openfluid::core::UnitClassID_t>::iterator it = Froms.begin();
      it != Froms.end(); ++it)
  {
    const_cast<openfluid::fluidx::UnitDescriptor&>(mp_Domain->getUnitDescriptor(
        it->first, it->second)).getUnitsTos().remove(Unit);
  }
  std::list<openfluid::core::UnitClassID_t> Children =
      mp_Domain->getUnitsChildrenOf(Unit);
  for (std::list<openfluid::core::UnitClassID_t>::iterator it =
      Children.begin(); it != Children.end(); ++it)
  {
    const_cast<openfluid::fluidx::UnitDescriptor&>(mp_Domain->getUnitDescriptor(
        it->first, it->second)).getUnitsParents().remove(Unit);
  }

  mp_Unit->getUnitsTos().clear();
  mp_Unit->getUnitsParents().clear();
}

// =====================================================================
// =====================================================================

void DomainUnitAddEditDialog::createAllRelationsFromRelationWidgets()
{
  openfluid::core::UnitClassID_t Unit = std::make_pair(mp_Unit->getUnitClass(),
                                                       mp_Unit->getUnitID());

  std::list<openfluid::core::UnitClassID_t> Tos = mp_ToWidget->getUnits();
  for (std::list<openfluid::core::UnitClassID_t>::iterator it = Tos.begin();
        it != Tos.end(); ++it)
  {
    if(!std::count(mp_Unit->getUnitsTos().begin(),mp_Unit->getUnitsTos().end(),*it))
      mp_Unit->getUnitsTos().push_back(*it);
  }

  std::list<openfluid::core::UnitClassID_t> Parents =
      mp_ParentWidget->getUnits();
  for (std::list<openfluid::core::UnitClassID_t>::iterator it = Parents.begin();
          it != Parents.end(); ++it)
    {
      if(!std::count(mp_Unit->getUnitsParents().begin(),mp_Unit->getUnitsParents().end(),*it))
        mp_Unit->getUnitsParents().push_back(*it);
    }

  std::list<openfluid::core::UnitClassID_t> Froms = mp_FromWidget->getUnits();
  for (std::list<openfluid::core::UnitClassID_t>::iterator it = Froms.begin();
      it != Froms.end(); ++it)
  {
    std::list<openfluid::core::UnitClassID_t>& it_Tos = mp_Domain->getUnitsToOf(*it);
    if(!std::count(it_Tos.begin(),it_Tos.end(),Unit))
      it_Tos.push_back(Unit);
  }

  std::list<openfluid::core::UnitClassID_t> Children =
      mp_ChildWidget->getUnits();
  for (std::list<openfluid::core::UnitClassID_t>::iterator it =
      Children.begin(); it != Children.end(); ++it)
  {
    std::list<openfluid::core::UnitClassID_t>& it_Parents = mp_Domain->getUnitsParentsOf(*it);
    if(!std::count(it_Parents.begin(),it_Parents.end(),Unit))
      it_Parents.push_back(Unit);
  }
}

// =====================================================================
// =====================================================================

