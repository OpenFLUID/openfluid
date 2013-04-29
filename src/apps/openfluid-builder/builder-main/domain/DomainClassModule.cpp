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
 \file DomainClassModule.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainClassModule.hpp"

#include <boost/algorithm/string.hpp>
#include <gtkmm/notebook.h>

#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include "DomainIDataAddDialog.hpp"
#include "DomainIDataRemoveDialog.hpp"
#include "DomainIDataEditDialog.hpp"
#include "DomainEventsComponent.hpp"

#include "BuilderListToolBoxFactory.hpp"
#include "BuilderButtonBox.hpp"

#include "BuilderFrame.hpp"
#include "EngineHelper.hpp"

// =====================================================================
// =====================================================================

DomainClassModule::DomainClassModule(
    openfluid::fluidx::AdvancedFluidXDescriptor& AdvancedDesc,
    std::string ClassName) :
    ProjectWorkspaceModule(AdvancedDesc), m_Domain(AdvancedDesc.getDomain()), m_ClassName(
        ClassName), mp_Columns(0)
{
  mp_MainPanel = Gtk::manage(new Gtk::VBox());

  // Input data

  mp_IDataAddDialog = new DomainIDataAddDialog(AdvancedDesc.getDomain(),
                                               m_ClassName);
  mp_IDataRemoveDialog = new DomainIDataRemoveDialog(AdvancedDesc.getDomain(),
                                                     m_ClassName);
  mp_IDataEditDialog = new DomainIDataEditDialog(AdvancedDesc.getDomain(),
                                                 m_ClassName);

  mp_IDataTreeView = Gtk::manage(new Gtk::TreeView());

  mp_IDataWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_IDataWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_IDataWin->add(*mp_IDataTreeView);
  mp_IDataWin->set_visible(true);
  mp_IDataWin->show_all_children();

  mp_IDataListToolBox = BuilderListToolBoxFactory::createDomainIDataToolBox();
  mp_IDataListToolBox->signal_AddCommandAsked().connect(
      sigc::mem_fun(*this, &DomainClassModule::whenAddIDataAsked));
  mp_IDataListToolBox->signal_RemoveCommandAsked().connect(
      sigc::mem_fun(*this, &DomainClassModule::whenRemoveIDataAsked));
  mp_IDataListToolBox->signal_EditCommandAsked().connect(
      sigc::mem_fun(*this, &DomainClassModule::whenEditIDataAsked));

  updateIData();

  // Events

  mp_DomainEventsMVP = new DomainEventsComponent(AdvancedDesc.getDomain());
  mp_EventsListToolBox = BuilderListToolBoxFactory::createDomainEventsToolBox();
}

// =====================================================================
// =====================================================================

DomainClassModule::~DomainClassModule()
{
  delete mp_IDataAddDialog;
  delete mp_IDataRemoveDialog;
  delete mp_IDataEditDialog;
  delete mp_IDataListToolBox;
}

// =====================================================================
// =====================================================================

void DomainClassModule::updateIData()
{
  std::set<std::string> IDataNames = m_Domain.getInputDataNames(m_ClassName);

  mp_IDataListToolBox->setRemoveCommandAvailable(!IDataNames.empty());
  mp_IDataListToolBox->setEditCommandAvailable(!IDataNames.empty());

  // store existing sort and selection

  int SortColumnId = 0;
  Gtk::SortType SortType = Gtk::SORT_ASCENDING;
  std::string SortColumnName = "";
  int SelectedID = -1;

  if (mref_ListStore)
  {
    mref_ListStore->get_sort_column_id(SortColumnId, SortType);
    SortColumnName = mp_IDataTreeView->get_column(SortColumnId)->get_title();

    Gtk::TreeIter Selected = mp_IDataTreeView->get_selection()->get_selected();
    if (Selected)
      SelectedID = Selected->get_value(mp_Columns->m_Id);
  }

// create liststore

  delete mp_Columns;
  mp_Columns = new IDataColumns(IDataNames);

  mref_ListStore.clear();
  mref_ListStore = Gtk::ListStore::create(*mp_Columns);

  mp_IDataTreeView->set_model(mref_ListStore);

  std::set<int> IDs = m_Domain.getIDsOfClass(m_ClassName);
  for (std::set<int>::iterator it = IDs.begin(); it != IDs.end(); ++it)
  {
    Gtk::TreeRow Row = *(mref_ListStore->append());

    Row[mp_Columns->m_Id] = *it;

    for (std::set<std::string>::iterator itData = IDataNames.begin();
        itData != IDataNames.end(); ++itData)
    {
      std::string IDataName = *itData;
      std::string IDataValue = m_Domain.getInputData(m_ClassName, *it,
                                                     IDataName);

      Row.set_value(mp_Columns->getColumn(IDataName), IDataValue);

      if (*it == SelectedID)
        mp_IDataTreeView->get_selection()->select(Row);
    }
  }

  // adapt treeview

  mp_IDataTreeView->remove_all_columns();

  mp_IDataTreeView->append_column("ID", mp_Columns->m_Id);
  mp_IDataTreeView->get_column(0)->set_sort_column(mp_Columns->m_Id);
  if (SortColumnId == 0)
  {
    mref_ListStore->set_sort_column(SortColumnId, SortType);
    SortColumnId = -1;
  }

  for (std::set<std::string>::iterator it = IDataNames.begin();
      it != IDataNames.end(); ++it)
  {
    Gtk::TreeModelColumn<std::string>& Col = mp_Columns->getColumn(*it);
    int Index = mp_IDataTreeView->append_column_editable(escapeUnderscores(*it),
                                                         Col)
                - 1;
    ((Gtk::CellRendererText *) mp_IDataTreeView->get_column_cell_renderer(Index))->signal_edited().connect(
        sigc::bind<std::string>(
            sigc::mem_fun(*this, &DomainClassModule::onDataEdited), *it));
    mp_IDataTreeView->get_column(Index)->set_sort_column(Col);

    if (SortColumnId != -1 && escapeUnderscores(*it) == SortColumnName)
    {
      mref_ListStore->set_sort_column(Col, SortType);
      SortColumnId = -1;
    }
  }

  // apply default sort and selection if needed

  if (SortColumnId != -1)
    mref_ListStore->set_sort_column(0, Gtk::SORT_ASCENDING);

  if (!mp_IDataTreeView->get_selection()->get_selected() && !mref_ListStore->children().empty())
    mp_IDataTreeView->get_selection()->select(mref_ListStore->children()[0]);
}

// =====================================================================
// =====================================================================

std::string DomainClassModule::escapeUnderscores(std::string Str)
{
  boost::replace_all(Str, "_", "__");
  return Str;
}

// =====================================================================
// =====================================================================

void DomainClassModule::whenAddIDataAsked()
{
  if (mp_IDataAddDialog->show())
  {
    updateIData();
    m_signal_DomainClassChanged.emit();
  }
}

// =====================================================================
// =====================================================================

void DomainClassModule::whenRemoveIDataAsked()
{
  if (mp_IDataRemoveDialog->show())
  {
    updateIData();
    m_signal_DomainClassChanged.emit();
  }
}

// =====================================================================
// =====================================================================

void DomainClassModule::whenEditIDataAsked()
{
  if (mp_IDataEditDialog->show())
  {
    updateIData();
    m_signal_DomainClassChanged.emit();
  }
}

// =====================================================================
// =====================================================================

void DomainClassModule::onDataEdited(const Glib::ustring& Path,
                                     const Glib::ustring& NewText,
                                     std::string DataName)
{
  Gtk::TreeIter Iter = mref_ListStore->get_iter(Path);

  if (!Iter)
    return;

  std::string DataVal = NewText;
  if (EngineHelper::isEmptyString(NewText) && openfluid::guicommon::DialogBoxFactory::showSimpleOkCancelQuestionDialog(
      _("You can't set an empty input data value.\n\n"
          "Do you want to set this input data value as the default one (\"-\") instead?")))
  {
    DataVal = "-";
    Iter->set_value(mp_Columns->getColumn(DataName), DataVal);
  }

  m_Domain.getInputData(m_ClassName, Iter->get_value(mp_Columns->m_Id),
                        DataName) = DataVal;

  m_signal_DomainClassChanged.emit();
}

// =====================================================================
// =====================================================================

void DomainClassModule::compose()
{
// input data

  Gtk::VBox* IDataButtonsPanel = Gtk::manage(new Gtk::VBox());
  IDataButtonsPanel->pack_start(*mp_IDataListToolBox->asWidget(),
                                Gtk::PACK_SHRINK);
  IDataButtonsPanel->set_visible(true);

  Gtk::HBox* FirstPanel = Gtk::manage(new Gtk::HBox());
  FirstPanel->set_border_width(5);
  FirstPanel->pack_start(*mp_IDataWin);
  FirstPanel->pack_start(*IDataButtonsPanel, Gtk::PACK_SHRINK, 5);
  FirstPanel->set_visible(true);

// events

  Gtk::VBox* EventsButtonsPanel = Gtk::manage(new Gtk::VBox());
  EventsButtonsPanel->pack_start(*mp_EventsListToolBox->asWidget(),
                                 Gtk::PACK_SHRINK);
  EventsButtonsPanel->set_visible(true);

  Gtk::HBox* SecondPanel = Gtk::manage(new Gtk::HBox());
  SecondPanel->set_border_width(5);
  SecondPanel->pack_start(*mp_DomainEventsMVP->asWidget());
  SecondPanel->pack_start(*EventsButtonsPanel, Gtk::PACK_SHRINK, 5);
  SecondPanel->set_visible(true);

  Gtk::Notebook* Notebook = Gtk::manage(new Gtk::Notebook());
  Notebook->append_page(*FirstPanel, _("Inputdata"));
  Notebook->append_page(*SecondPanel, _("Events"));
  Notebook->set_border_width(6);
  Notebook->set_visible(true);

  mp_MainPanel->pack_start(*Notebook);

  mp_MainPanel->set_visible(true);
}

// =====================================================================
// =====================================================================

Gtk::Widget* DomainClassModule::asWidget()
{
  if (mp_MainPanel)
    return mp_MainPanel;
  throw std::logic_error(
      "DomainClassModule : you try to get a widget from a non yet composed module.");
}

// =====================================================================
// =====================================================================

sigc::signal<void> DomainClassModule::signal_ModuleChanged()
{
  return m_signal_DomainClassChanged;
}

// =====================================================================
// =====================================================================

void DomainClassModule::update()
{
  updateIData();

  // TODO update Events
}

// =====================================================================
// =====================================================================

