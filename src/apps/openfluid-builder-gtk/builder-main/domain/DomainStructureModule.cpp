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
 \file DomainStructureModule.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainStructureModule.hpp"

#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include "DomainUnitRelationAddDialog.hpp"
#include "DomainUnitAddEditDialog.hpp"
#include "BuilderListToolBoxFactory.hpp"
#include "BuilderButtonBox.hpp"
#include "BuilderFrame.hpp"

// =====================================================================
// =====================================================================

DomainStructureModule::DomainStructureModule(
    openfluid::fluidx::AdvancedFluidXDescriptor& AdvancedDesc) :
    ProjectWorkspaceModule(AdvancedDesc), m_Domain(m_AdvancedDesc.getDomain())
{
  mp_RelationAddDialog = new DomainUnitRelationAddDialog(
      AdvancedDesc.getDomain());
  mp_AddEditDialog = new DomainUnitAddEditDialog(*mp_RelationAddDialog,
                                                 AdvancedDesc.getDomain());

  mp_ToolBox = BuilderListToolBoxFactory::createDomainStructureToolBox();
  mp_ToolBox->signal_AddCommandAsked().connect(
      sigc::mem_fun(*this, &DomainStructureModule::onAddAsked));
  mp_ToolBox->signal_RemoveCommandAsked().connect(
      sigc::mem_fun(*this, &DomainStructureModule::onRemoveAsked));
  mp_ToolBox->signal_EditCommandAsked().connect(
      sigc::mem_fun(*this, &DomainStructureModule::onEditAsked));

  mp_MainPanel = 0;

  mref_ClassesStore = Gtk::ListStore::create(m_ClassColumns);

  mref_UnitsStore = Gtk::ListStore::create(m_UnitColumns);
  mref_UnitsStore->signal_sort_column_changed().connect(
      sigc::mem_fun(*this, &DomainStructureModule::onUnitsSortChanged));

  mp_ClassesView = Gtk::manage(new Gtk::TreeView(mref_ClassesStore));
  mp_ClassesView->append_column(_("Unit classes"), m_ClassColumns.m_Class);
  mp_ClassesView->get_selection()->signal_changed().connect(
      sigc::mem_fun(*this, &DomainStructureModule::onClassSelectionChanged));

  mp_UnitsView = Gtk::manage(new Gtk::TreeView(mref_UnitsStore));
  mp_UnitsView->append_column("ID", m_UnitColumns.m_Id);
  mp_UnitsView->append_column(_("Process order"), m_UnitColumns.m_PcsOrder);
  mp_UnitsView->get_column(0)->set_sort_column(m_UnitColumns.m_Id);
  mp_UnitsView->get_column(1)->set_sort_column(m_UnitColumns.m_PcsOrder);
  mp_UnitsView->signal_row_activated().connect(
      sigc::mem_fun(*this, &DomainStructureModule::onUnitRowActivated));

  mp_ClassesWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_ClassesWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_ClassesWin->add(*mp_ClassesView);
  mp_ClassesWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  mp_UnitsWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_UnitsWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_UnitsWin->add(*mp_UnitsView);
  mp_UnitsWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  mp_MainBox = Gtk::manage(new Gtk::HBox());
  mp_MainBox->pack_start(*mp_ClassesWin, Gtk::PACK_EXPAND_WIDGET, 5);
  mp_MainBox->pack_start(*mp_UnitsWin, Gtk::PACK_EXPAND_WIDGET, 5);
  mp_MainBox->set_visible(true);
  mp_MainBox->show_all_children();

  updateClasses();
}

// =====================================================================
// =====================================================================

DomainStructureModule::~DomainStructureModule()
{
  delete mp_AddEditDialog;
  delete mp_RelationAddDialog;
  delete mp_ToolBox;
}

// =====================================================================
// =====================================================================

void DomainStructureModule::onClassSelectionChanged()
{
  Gtk::TreeIter Iter = mp_ClassesView->get_selection()->get_selected();

  if (Iter)
    updateUnits(Iter);
}

// =====================================================================
// =====================================================================

void DomainStructureModule::onUnitRowActivated(
    const Gtk::TreeModel::Path& /*Path*/, Gtk::TreeViewColumn* /*Column*/)
{
  onEditAsked();
}

// =====================================================================
// =====================================================================

void DomainStructureModule::onUnitsSortChanged()
{
  Gtk::TreeIter ClassIter = mp_ClassesView->get_selection()->get_selected();

  if (ClassIter)
  {
    int SortId;
    Gtk::SortType SortType;

    mref_UnitsStore->get_sort_column_id(SortId, SortType);

    ClassIter->set_value(m_ClassColumns.m_UnitSortColumnId, SortId);
    ClassIter->set_value(m_ClassColumns.m_UnitSortType, SortType);
  }
}

// =====================================================================
// =====================================================================

void DomainStructureModule::updateClasses(std::string ClassToForceSelection)
{
  std::set<std::string> Classes = m_Domain.getClassNames();

  Gtk::TreeModel::Children Children = mref_ClassesStore->children();
  Gtk::TreeModel::Children::iterator it = Children.begin();
  while (it != Children.end())
  {
    std::string Class = it->get_value(m_ClassColumns.m_Class);
    if (Classes.find(Class) != Classes.end())
    {
      if (Class == ClassToForceSelection)
        mp_ClassesView->get_selection()->select(it);

      Classes.erase(Class);
      it++;
    }
    else
      it = mref_ClassesStore->erase(it);
  }

  for (std::set<std::string>::iterator it = Classes.begin();
      it != Classes.end(); ++it)
  {
    Gtk::TreeRow Row = *(mref_ClassesStore->append());
    Row[m_ClassColumns.m_Class] = *it;
    Row[m_ClassColumns.m_UnitSortColumnId] = 0;
    Row[m_ClassColumns.m_UnitSortType] = Gtk::SORT_ASCENDING;

    if (*it == ClassToForceSelection)
      mp_ClassesView->get_selection()->select(Row);
  }

  bool IsDomainEmpty = mref_ClassesStore->children().empty();

  if (!IsDomainEmpty)
  {
    if (!mp_ClassesView->get_selection()->get_selected())
      mp_ClassesView->get_selection()->select(mref_ClassesStore->children()[0]);
  }
  else
    mref_UnitsStore->clear();

  mp_ToolBox->setRemoveCommandAvailable(!IsDomainEmpty);
  mp_ToolBox->setEditCommandAvailable(!IsDomainEmpty);
}

// =====================================================================
// =====================================================================

void DomainStructureModule::updateUnits(Gtk::TreeIter& ClassIter)
{
  mref_UnitsStore->clear();

  std::string ClassName = ClassIter->get_value(m_ClassColumns.m_Class);

  std::set<int> IDs = m_Domain.getIDsOfClass(ClassName);

  for (std::set<int>::iterator it = IDs.begin(); it != IDs.end(); ++it)
  {
    const openfluid::fluidx::UnitDescriptor& Desc = m_Domain.getUnitDescriptor(
        ClassName, *it);

    Gtk::TreeRow Row = *(mref_UnitsStore->append());
    Row[m_UnitColumns.m_Id] = Desc.getUnitID();
    Row[m_UnitColumns.m_PcsOrder] = Desc.getProcessOrder();
  }

  mref_UnitsStore->set_sort_column(
      ClassIter->get_value(m_ClassColumns.m_UnitSortColumnId),
      ClassIter->get_value(m_ClassColumns.m_UnitSortType));

  if (!IDs.empty())
    mp_UnitsView->get_selection()->select(mref_UnitsStore->children()[0]);

//  mp_ClassesView->scroll_to_row(mp_ClassesView->get_model()->get_path(Iter));
}

// =====================================================================
// =====================================================================

void DomainStructureModule::onEditAsked()
{
  Gtk::TreeIter ClassIter = mp_ClassesView->get_selection()->get_selected();
  Gtk::TreeIter UnitIter = mp_UnitsView->get_selection()->get_selected();

  if (ClassIter && UnitIter)
  {
    std::string Class = ClassIter->get_value(m_ClassColumns.m_Class);
    int ID = UnitIter->get_value(m_UnitColumns.m_Id);

    openfluid::fluidx::UnitDescriptor* Unit = mp_AddEditDialog->show(Class, ID);

    if (Unit)
    {
      UnitIter->set_value(m_UnitColumns.m_PcsOrder, Unit->getProcessOrder());

      m_signal_DomainStructureChanged.emit();
    }
  }
}

// =====================================================================
// =====================================================================

void DomainStructureModule::onAddAsked()
{
  std::string CurrentClass = "";

  Gtk::TreeIter ClassIter = mp_ClassesView->get_selection()->get_selected();
  if (ClassIter)
    CurrentClass = ClassIter->get_value(m_ClassColumns.m_Class);

  openfluid::fluidx::UnitDescriptor* Unit = mp_AddEditDialog->show(
      CurrentClass);

  if (!Unit)
    return;

  std::string NewUnitClass = Unit->getUnitClass();

  if (CurrentClass != NewUnitClass)
    updateClasses(NewUnitClass);
  else
    updateUnits(ClassIter);

  Gtk::TreeModel::Children Children = mref_UnitsStore->children();
  for (Gtk::TreeModel::Children::iterator it = Children.begin();
      it != Children.end(); ++it)
  {
    if (it->get_value(m_UnitColumns.m_Id) == (int) Unit->getUnitID())
    {
      mp_UnitsView->get_selection()->select(it);
      break;
    }
  }

  m_signal_DomainStructureChanged.emit();
}

// =====================================================================
// =====================================================================

void DomainStructureModule::onRemoveAsked()
{
  Gtk::TreeIter ClassIter = mp_ClassesView->get_selection()->get_selected();
  Gtk::TreeIter UnitIter = mp_UnitsView->get_selection()->get_selected();

  if (ClassIter && UnitIter)
  {
    std::string Class = ClassIter->get_value(m_ClassColumns.m_Class);

    m_Domain.deleteUnit(Class, UnitIter->get_value(m_UnitColumns.m_Id));

    if (m_Domain.isClassNameExists(Class))
      updateUnits(ClassIter);
    else
      updateClasses();

    m_signal_DomainStructureChanged.emit();
  }
}

// =====================================================================
// =====================================================================

void DomainStructureModule::compose()
{
  mp_MainPanel = Gtk::manage(new Gtk::VBox());

  Gtk::HBox* TopPanel = Gtk::manage(new Gtk::HBox());
  TopPanel->set_border_width(5);
  TopPanel->pack_start(*mp_MainBox, Gtk::PACK_EXPAND_WIDGET, 5);

  Gtk::VBox* ButtonsPanel = Gtk::manage(new Gtk::VBox());
  ButtonsPanel->pack_start(*mp_ToolBox->asWidget(), Gtk::PACK_SHRINK);
  ButtonsPanel->set_visible(true);

  TopPanel->pack_start(*ButtonsPanel, Gtk::PACK_SHRINK, 5);
  TopPanel->set_visible(true);

  mp_MainPanel->pack_start(*TopPanel, Gtk::PACK_EXPAND_WIDGET, 12);

  mp_MainPanel->set_visible(true);
}

// =====================================================================
// =====================================================================

Gtk::Widget* DomainStructureModule::asWidget()
{
  if (mp_MainPanel)
    return mp_MainPanel;
  throw std::logic_error(
      "DomainStructureModule : you try to get a widget from a non yet composed module.");
}

// =====================================================================
// =====================================================================

sigc::signal<void> DomainStructureModule::signal_ModuleChanged()
{
  return m_signal_DomainStructureChanged;
}

// =====================================================================
// =====================================================================

void DomainStructureModule::whenDomainChanged()
{
  m_signal_DomainStructureChanged.emit();
}

// =====================================================================
// =====================================================================

void DomainStructureModule::update()
{
  updateClasses();
}

// =====================================================================
// =====================================================================

