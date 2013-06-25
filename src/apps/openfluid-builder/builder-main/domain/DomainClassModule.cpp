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
#include "DomainAttributesAddDialog.hpp"
#include "DomainAttributesRemoveDialog.hpp"
#include "DomainAttributesEditDialog.hpp"
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
        ClassName), mp_AttrsColumns(0)
{
  mp_MainPanel = Gtk::manage(new Gtk::VBox());

  // attributes

  mp_AttrAddDialog = new DomainAttributesAddDialog(AdvancedDesc.getDomain(),
                                               m_ClassName);
  mp_AttrRemoveDialog = new DomainAttributesRemoveDialog(AdvancedDesc.getDomain(),
                                                     m_ClassName);
  mp_AttrEditDialog = new DomainAttributesEditDialog(AdvancedDesc.getDomain(),
                                                 m_ClassName);

  mp_AttrsTreeView = Gtk::manage(new Gtk::TreeView());

  mp_AttrsWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_AttrsWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_AttrsWin->add(*mp_AttrsTreeView);
  mp_AttrsWin->set_visible(true);
  mp_AttrsWin->show_all_children();

  mp_AttrListToolBox = BuilderListToolBoxFactory::createDomainAttributeToolBox();
  mp_AttrListToolBox->signal_AddCommandAsked().connect(
      sigc::mem_fun(*this, &DomainClassModule::whenAddAttributeAsked));
  mp_AttrListToolBox->signal_RemoveCommandAsked().connect(
      sigc::mem_fun(*this, &DomainClassModule::whenRemoveAttributeAsked));
  mp_AttrListToolBox->signal_EditCommandAsked().connect(
      sigc::mem_fun(*this, &DomainClassModule::whenEditAttributeAsked));

  // Events

  mref_EventsTreeStore = Gtk::TreeStore::create(m_EventsColumns);

  mp_EventsTreeView = Gtk::manage(new Gtk::TreeView(mref_EventsTreeStore));
  mp_EventsTreeView->append_column(_("ID - Date - Information"),
                                   m_EventsColumns.m_Id_Date_Info);

  mp_EventsWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_EventsWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_EventsWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
  mp_EventsWin->add(*mp_EventsTreeView);
  mp_EventsWin->set_visible(true);
  mp_EventsWin->show_all_children();

  mp_EventsListToolBox = BuilderListToolBoxFactory::createDomainEventsToolBox();
  mp_EventsListToolBox->signal_AddCommandAsked().connect(
      sigc::mem_fun(*this, &DomainClassModule::whenAddEventAsked));
  mp_EventsListToolBox->signal_RemoveCommandAsked().connect(
      sigc::mem_fun(*this, &DomainClassModule::whenRemoveEventAsked));

  update();
}

// =====================================================================
// =====================================================================

DomainClassModule::~DomainClassModule()
{
  delete mp_AttrAddDialog;
  delete mp_AttrRemoveDialog;
  delete mp_AttrEditDialog;
  delete mp_AttrListToolBox;
  delete mp_EventsListToolBox;
}

// =====================================================================
// =====================================================================

void DomainClassModule::updateAttributes()
{
  std::set<std::string> AttrsNames = m_Domain.getAttributesNames(m_ClassName);

  mp_AttrListToolBox->setRemoveCommandAvailable(!AttrsNames.empty());
  mp_AttrListToolBox->setEditCommandAvailable(!AttrsNames.empty());

  // store existing sort and selection

  int SortColumnId = 0;
  Gtk::SortType SortType = Gtk::SORT_ASCENDING;
  std::string SortColumnName = "";
  int SelectedID = -1;

  if (mref_AttrsListStore)
  {
    mref_AttrsListStore->get_sort_column_id(SortColumnId, SortType);
    SortColumnName = mp_AttrsTreeView->get_column(SortColumnId)->get_title();

    Gtk::TreeIter Selected = mp_AttrsTreeView->get_selection()->get_selected();
    if (Selected)
      SelectedID = Selected->get_value(mp_AttrsColumns->m_Id);
  }

// create liststore

  delete mp_AttrsColumns;
  mp_AttrsColumns = new AttrsColumns(AttrsNames);

  mref_AttrsListStore.clear();
  mref_AttrsListStore = Gtk::ListStore::create(*mp_AttrsColumns);

  mp_AttrsTreeView->set_model(mref_AttrsListStore);

  std::set<int> IDs = m_Domain.getIDsOfClass(m_ClassName);
  for (std::set<int>::iterator it = IDs.begin(); it != IDs.end(); ++it)
  {
    Gtk::TreeRow Row = *(mref_AttrsListStore->append());

    Row[mp_AttrsColumns->m_Id] = *it;

    for (std::set<std::string>::iterator itData = AttrsNames.begin();
        itData != AttrsNames.end(); ++itData)
    {
      std::string AttrName = *itData;
      std::string AttrValue = m_Domain.getAttribute(m_ClassName, *it,
                                                     AttrName);

      Row.set_value(mp_AttrsColumns->getColumn(AttrName), AttrValue);

      if (*it == SelectedID)
        mp_AttrsTreeView->get_selection()->select(Row);
    }
  }

  // adapt treeview

  mp_AttrsTreeView->remove_all_columns();

  mp_AttrsTreeView->append_column("ID", mp_AttrsColumns->m_Id);
  mp_AttrsTreeView->get_column(0)->set_sort_column(mp_AttrsColumns->m_Id);
  if (SortColumnId == 0)
  {
    mref_AttrsListStore->set_sort_column(SortColumnId, SortType);
    SortColumnId = -1;
  }

  for (std::set<std::string>::iterator it = AttrsNames.begin();
      it != AttrsNames.end(); ++it)
  {
    Gtk::TreeModelColumn<std::string>& Col = mp_AttrsColumns->getColumn(*it);
    int Index = mp_AttrsTreeView->append_column_editable(escapeUnderscores(*it),
                                                         Col)
                - 1;
    ((Gtk::CellRendererText *) mp_AttrsTreeView->get_column_cell_renderer(Index))->signal_edited().connect(
        sigc::bind<std::string>(
            sigc::mem_fun(*this, &DomainClassModule::onAttributeEdited), *it));
    mp_AttrsTreeView->get_column(Index)->set_sort_column(Col);

    if (SortColumnId != -1 && escapeUnderscores(*it) == SortColumnName)
    {
      mref_AttrsListStore->set_sort_column(Col, SortType);
      SortColumnId = -1;
    }
  }

  // apply default sort and selection if needed

  if (SortColumnId != -1)
    mref_AttrsListStore->set_sort_column(0, Gtk::SORT_ASCENDING);

  if (!mp_AttrsTreeView->get_selection()->get_selected() && !mref_AttrsListStore->children().empty())
    mp_AttrsTreeView->get_selection()->select(
        mref_AttrsListStore->children()[0]);
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

void DomainClassModule::whenAddAttributeAsked()
{
  if (mp_AttrAddDialog->show())
  {
    updateAttributes();
    m_signal_DomainClassChanged.emit();
  }
}

// =====================================================================
// =====================================================================

void DomainClassModule::whenRemoveAttributeAsked()
{
  if (mp_AttrRemoveDialog->show())
  {
    updateAttributes();
    m_signal_DomainClassChanged.emit();
  }
}

// =====================================================================
// =====================================================================

void DomainClassModule::whenEditAttributeAsked()
{
  if (mp_AttrEditDialog->show())
  {
    updateAttributes();
    m_signal_DomainClassChanged.emit();
  }
}

// =====================================================================
// =====================================================================

void DomainClassModule::onAttributeEdited(const Glib::ustring& Path,
                                     const Glib::ustring& NewText,
                                     std::string DataName)
{
  Gtk::TreeIter Iter = mref_AttrsListStore->get_iter(Path);

  if (!Iter)
    return;

  std::string DataVal = NewText;
  if (EngineHelper::isEmptyString(NewText) && openfluid::guicommon::DialogBoxFactory::showSimpleOkCancelQuestionDialog(
      _("You can't set an empty attribute value.\n\n"
          "Do you want to set this attribute value as the default one (\"-\") instead?")))
  {
    DataVal = "-";
    Iter->set_value(mp_AttrsColumns->getColumn(DataName), DataVal);
  }

  m_Domain.getAttribute(m_ClassName, Iter->get_value(mp_AttrsColumns->m_Id),
                        DataName) = DataVal;

  m_signal_DomainClassChanged.emit();
}

// =====================================================================
// =====================================================================

void DomainClassModule::updateEvents()
{
  const std::map<int, openfluid::fluidx::BuilderUnit>& Units =
      m_Domain.getUnitsByIdByClass().at(m_ClassName);

  mref_EventsTreeStore->clear();

  for (std::map<int, openfluid::fluidx::BuilderUnit>::const_iterator it =
      Units.begin(); it != Units.end(); ++it)
  {
    std::list<openfluid::core::Event*> Events = it->second.Events;

    if (Events.empty())
      continue;

    Gtk::TreeRow UnitRow = *mref_EventsTreeStore->append();
    UnitRow[m_EventsColumns.m_Id_Date_Info] = Glib::ustring::compose("%1",
                                                                     it->first);

    EngineHelper::sortEventsListByDateTime(Events);

    for (std::list<openfluid::core::Event*>::iterator itEvents = Events.begin();
        itEvents != Events.end(); ++itEvents)
    {
      Gtk::TreeRow EventRow = *mref_EventsTreeStore->append(
          UnitRow->children());

      EventRow[m_EventsColumns.m_Id_Date_Info] =
          (*itEvents)->getDateTime().getAsISOString();

      openfluid::core::Event::EventInfosMap_t Infos = (*itEvents)->getInfos();

      for (openfluid::core::Event::EventInfosMap_t::iterator itInfos =
          Infos.begin(); itInfos != Infos.end(); ++itInfos)
      {
        Gtk::TreeRow InfoRow = *mref_EventsTreeStore->append(
            EventRow->children());

        InfoRow[m_EventsColumns.m_Id_Date_Info] = Glib::ustring::compose(
            "%1 : %2", itInfos->first, itInfos->second.get());
      }
    }

  }

  for (unsigned int i = 0; i < mref_EventsTreeStore->children().size(); i++)
    mp_EventsTreeView->expand_row(
        Gtk::TreePath(Glib::ustring::compose("%1", i)), false);
}

// =====================================================================
// =====================================================================

void DomainClassModule::whenAddEventAsked()
{
  openfluid::guicommon::DialogBoxFactory::showDisabledFeatureMessage();
}

// =====================================================================
// =====================================================================

void DomainClassModule::whenRemoveEventAsked()
{
  openfluid::guicommon::DialogBoxFactory::showDisabledFeatureMessage();
}

// =====================================================================
// =====================================================================

void DomainClassModule::compose()
{
// attributes

  Gtk::VBox* AttrsButtonsPanel = Gtk::manage(new Gtk::VBox());
  AttrsButtonsPanel->pack_start(*mp_AttrListToolBox->asWidget(),
                                Gtk::PACK_SHRINK);
  AttrsButtonsPanel->set_visible(true);

  Gtk::HBox* FirstPanel = Gtk::manage(new Gtk::HBox());
  FirstPanel->set_border_width(5);
  FirstPanel->pack_start(*mp_AttrsWin);
  FirstPanel->pack_start(*AttrsButtonsPanel, Gtk::PACK_SHRINK, 5);
  FirstPanel->set_visible(true);

// events

  Gtk::VBox* EventsButtonsPanel = Gtk::manage(new Gtk::VBox());
  EventsButtonsPanel->pack_start(*mp_EventsListToolBox->asWidget(),
                                 Gtk::PACK_SHRINK);
  EventsButtonsPanel->set_visible(true);

  Gtk::HBox* SecondPanel = Gtk::manage(new Gtk::HBox());
  SecondPanel->set_border_width(5);
  SecondPanel->pack_start(*mp_EventsWin);
  SecondPanel->pack_start(*EventsButtonsPanel, Gtk::PACK_SHRINK, 5);
  SecondPanel->set_visible(true);

  Gtk::Notebook* Notebook = Gtk::manage(new Gtk::Notebook());
  Notebook->append_page(*FirstPanel, _("Attributes"));
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
  updateAttributes();
  updateEvents();
}

// =====================================================================
// =====================================================================

