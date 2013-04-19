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
 \file DatastoreView.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "DatastoreView.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/scrolledwindow.h>
#include <openfluid/fluidx/AdvancedDatastoreDescriptor.hpp>
#include <openfluid/fluidx/DatastoreItemDescriptor.hpp>
#include "BuilderListToolBoxFactory.hpp"
#include "BuilderButtonBox.hpp"
#include "DatasoreAddItemDialog.hpp"

// =====================================================================
// =====================================================================

DatastoreView::DatastoreView(
    openfluid::fluidx::AdvancedDatastoreDescriptor& Datastore) :
    mp_Datastore(&Datastore)
{
  mp_MainBox = Gtk::manage(new Gtk::HBox());

  mref_ListStore = Gtk::ListStore::create(m_Columns);

  mp_TreeView = Gtk::manage(new Gtk::TreeView());

  mp_TreeView->append_column("ID", m_Columns.m_ID);
  mp_TreeView->append_column(_("Type"), m_Columns.m_Type);
  mp_TreeView->append_column(_("Unit Class"), m_Columns.m_Class);
  mp_TreeView->append_column(_("Source"), m_Columns.m_Source);
  mp_TreeView->append_column(_("Test"), m_Columns.m_Test);

  mp_TreeView->get_column(4)->add_attribute(
      ((Gtk::CellRendererText*) mp_TreeView->get_column_cell_renderer(4))->property_mode(),
      m_Columns.m_True);

  mp_TreeView->set_visible(true);

  Gtk::ScrolledWindow* mp_ModelWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_ModelWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_ModelWin->set_visible(true);
  mp_ModelWin->add(*mp_TreeView);
  mp_ModelWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  mp_TreeView->set_model(mref_ListStore);

  mp_DSListToolBox = BuilderListToolBoxFactory::createDatastoreToolBox();
  mp_DSListToolBox->signal_AddCommandAsked().connect(
      sigc::mem_fun(*this, &DatastoreView::whenAddAsked));
  mp_DSListToolBox->signal_RemoveCommandAsked().connect(
      sigc::mem_fun(*this, &DatastoreView::whenRemoveAsked));
  mp_DSListToolBox->signal_UpCommandAsked().connect(
      sigc::mem_fun(*this, &DatastoreView::whenUpAsked));
  mp_DSListToolBox->signal_DownCommandAsked().connect(
      sigc::mem_fun(*this, &DatastoreView::whenDownAsked));

  Gtk::VBox* ButtonsPanel = Gtk::manage(new Gtk::VBox());
  ButtonsPanel->pack_start(*mp_DSListToolBox->asWidget(), Gtk::PACK_SHRINK);
  ButtonsPanel->set_visible(true);

  mp_MainBox->pack_start(*mp_ModelWin);
  mp_MainBox->pack_start(*ButtonsPanel, Gtk::PACK_SHRINK);
  mp_MainBox->set_border_width(5);
  mp_MainBox->set_visible(true);

  update();
  requestSelectionAt(0);

  mp_AddDialog = new DatasoreAddItemDialog(Datastore);
}

// =====================================================================
// =====================================================================

DatastoreView::~DatastoreView()
{
  delete mp_DSListToolBox;
}

// =====================================================================
// =====================================================================

void DatastoreView::update()
{
  mref_ListStore->clear();

  std::list<openfluid::fluidx::DatastoreItemDescriptor*> Items =
      mp_Datastore->getItems();

  bool Test = false;
  for (std::list<openfluid::fluidx::DatastoreItemDescriptor*>::iterator it =
      Items.begin(); it != Items.end(); ++it)
  {

    Gtk::TreeRow Row = *(mref_ListStore->append());

    Row[m_Columns.m_ID] = (*it)->getID();

    Row[m_Columns.m_Type] =
        openfluid::core::UnstructuredValue::getStringFromValueType(
            (*it)->getType());
    Row[m_Columns.m_Class] = (*it)->getUnitClass();
    Row[m_Columns.m_Source] = (*it)->getRelativePath();
    Row[m_Columns.m_Test] = Test;
    Test = !Test;
    Row[m_Columns.m_True] = true;
  }

  updateListToolBox();
}

// =====================================================================
// =====================================================================

Gtk::Widget* DatastoreView::asWidget()
{
  return mp_MainBox;
}

// =====================================================================
// =====================================================================

void DatastoreView::updateListToolBox()
{
  bool AtLeast1Item = !mp_Datastore->getItems().empty();
  bool AtLeast2Item = mp_Datastore->getItems().size() > 1;
  mp_DSListToolBox->setRemoveCommandAvailable(AtLeast1Item);
  mp_DSListToolBox->setUpCommandAvailable(AtLeast2Item);
  mp_DSListToolBox->setDownCommandAvailable(AtLeast2Item);
}

// =====================================================================
// =====================================================================

void DatastoreView::whenAddAsked()
{
  openfluid::fluidx::DatastoreItemDescriptor* NewItem = mp_AddDialog->show();
  if (NewItem)
  {
    mp_Datastore->appendItem(NewItem);
    update();
    m_signal_DatastoreChanged.emit();
  }
}

// =====================================================================
// =====================================================================

void DatastoreView::whenRemoveAsked()
{
  Gtk::TreeIter Iter = mp_TreeView->get_selection()->get_selected();

  if (!Iter)
    return;

  int Position = *mref_ListStore->get_path(Iter).begin();

  mp_Datastore->removeItem(Position);

  update();

  // set new selection
  int Size = mref_ListStore->children().size();
  if (Position < Size)
    requestSelectionAt(Position);
  else if (Position == Size)
    requestSelectionAt(Position - 1);

  signal_DatastoreChanged().emit();
}

// =====================================================================
// =====================================================================

void DatastoreView::whenUpAsked()
{
  Gtk::TreeIter Iter = mp_TreeView->get_selection()->get_selected();

  if (!Iter)
    return;

  int From = *mref_ListStore->get_path(Iter).begin();
  int To = (From == 0) ? getLastPosition() : (From - 1);

  mp_Datastore->moveItem(From, To);

  update();
  requestSelectionAt(To);

  signal_DatastoreChanged().emit();
}

// =====================================================================
// =====================================================================

void DatastoreView::whenDownAsked()
{
  Gtk::TreeIter Iter = mp_TreeView->get_selection()->get_selected();

  if (!Iter)
    return;

  int From = *mref_ListStore->get_path(Iter).begin();
  int To = (From == getLastPosition()) ? 0 : (From + 1);

  mp_Datastore->moveItem(From, To);

  update();
  requestSelectionAt(To);

  signal_DatastoreChanged().emit();
}

// =====================================================================
// =====================================================================

int DatastoreView::getLastPosition()
{
  int Size = mref_ListStore->children().size();
  return (Size == 0) ? -1 : (Size - 1);
}
// =====================================================================
// =====================================================================

sigc::signal<void> DatastoreView::signal_DatastoreChanged()
{
  return m_signal_DatastoreChanged;
}

// =====================================================================
// =====================================================================

void DatastoreView::requestSelectionAt(int Position)
{
  if (Position > -1 && Position < (int) mref_ListStore->children().size())
  {
    Gtk::TreeIter Iter = mref_ListStore->children().begin();
    std::advance(Iter, Position);
    mp_TreeView->get_selection()->select(Iter);
  }
  else
    mp_TreeView->get_selection()->unselect_all();
}
