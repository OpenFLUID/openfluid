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
 \file AvailSimulatorsWidget.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "AvailSimulatorsWidget.hpp"

#include <glibmm/i18n.h>
#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include "BuilderGraphicsHelper.hpp"

// =====================================================================
// =====================================================================

AvailSimulatorsWidget::AvailSimulatorsWidget(
    const openfluid::fluidx::AdvancedModelDescriptor& Model) :
    m_Model(Model)
{
  mref_TreeModel = Gtk::TreeStore::create(m_Columns);

  mp_TreeView = Gtk::manage(new Gtk::TreeView());
  mp_TreeView->set_model(mref_TreeModel);

  Gtk::TreeView::Column * idColumn = Gtk::manage(
      new Gtk::TreeView::Column(_("Available simulators")));

  idColumn->pack_start(m_Columns.m_Status, false);
  idColumn->pack_start(m_Columns.m_Id);
  mp_TreeView->append_column(*idColumn);
  std::list<Gtk::CellRenderer*> CR = idColumn->get_cell_renderers();
  for (std::list<Gtk::CellRenderer*>::iterator it = CR.begin(); it != CR.end();
      ++it)
  {
    mp_TreeView->get_column(0)->add_attribute(
        ((Gtk::CellRendererText*) *it)->property_sensitive(),
        m_Columns.m_Sensitive);
  }

  mp_TreeView->append_column(_("Domain"), m_Columns.m_Domain);

  mp_TreeView->get_column(1)->add_attribute(
      ((Gtk::CellRendererText*) mp_TreeView->get_column_cell_renderer(1))->property_sensitive(),
      m_Columns.m_Sensitive);

  mp_TreeView->set_search_column(m_Columns.m_Id);
  mp_TreeView->get_column(0)->set_sort_column(m_Columns.m_Id);
  mp_TreeView->get_column(1)->set_sort_column(m_Columns.m_Domain);

  for (unsigned int i = 0; i < mp_TreeView->get_columns().size(); i++)
  {
    mp_TreeView->get_column(i)->set_resizable();
    mp_TreeView->get_column(i)->set_reorderable();
  }

  mp_TreeView->get_selection()->signal_changed().connect(
      sigc::mem_fun(*this, &AvailSimulatorsWidget::onSelectionChanged));
  mp_TreeView->signal_row_activated().connect(
      sigc::mem_fun(*this, &AvailSimulatorsWidget::onRowActivated));

  mp_MainWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_MainWin->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  mp_MainWin->add(*mp_TreeView);

  pack_start(*mp_MainWin);

  set_visible(true);
  show_all_children();
}

// =====================================================================
// =====================================================================

void AvailSimulatorsWidget::update()
{
  mref_TreeModel->clear();

  openfluid::machine::SimulatorSignatureRegistry* Reg =
      openfluid::machine::SimulatorSignatureRegistry::getInstance();

  // Pluggable simulators

  Gtk::TreeRow PlugRow = *mref_TreeModel->append();
  PlugRow[m_Columns.m_Id] = _("Simulators");
  PlugRow[m_Columns.mp_Sign] = 0;
  PlugRow[m_Columns.m_Sensitive] = true;

  openfluid::machine::SimulatorSignatureRegistry::SimSignaturesByName_t Plugs =
      Reg->getPluggableSignatures();

  if (Plugs.empty())
  {
    Gtk::TreeRow SubRow = *mref_TreeModel->append(PlugRow.children());
    SubRow[m_Columns.m_Id] = _("no simulator signature available");
    SubRow[m_Columns.mp_Sign] = 0;
  }
  else
  {
    for (openfluid::machine::SimulatorSignatureRegistry::SimSignaturesByName_t::iterator it =
        Plugs.begin(); it != Plugs.end(); ++it)
    {
      Gtk::TreeRow SubRow = *mref_TreeModel->append(PlugRow.children());

      SubRow[m_Columns.m_Id] = it->second->Signature->ID;
      SubRow[m_Columns.m_Domain] = replaceEmpty(it->second->Signature->Domain);
      SubRow[m_Columns.mp_Sign] = it->second;

      std::string ImageFile;
      switch (it->second->Signature->Status)
      {
        case openfluid::ware::EXPERIMENTAL:
          ImageFile = "sim_status_experimental.png";
          break;
        case openfluid::ware::BETA:
          ImageFile = "sim_status_beta.png";
          break;
        case openfluid::ware::STABLE:
          ImageFile = "sim_status_stable.png";
          break;
      }
      SubRow[m_Columns.m_Status] = BuilderGraphicsHelper::createPixbufFromFile(
          ImageFile);

      SubRow[m_Columns.m_Sensitive] = m_Model.getFirstItemIndex(
          it->second->Signature->ID)
                                      == -1;
    }
  }

// Generators

  openfluid::machine::SimulatorSignatureRegistry::SimSignaturesByName_t Gens =
      Reg->getGeneratorSignatures();

  Gtk::TreeRow GenRow = *mref_TreeModel->append();
  GenRow[m_Columns.m_Id] = _("Generators");
  GenRow[m_Columns.mp_Sign] = 0;
  GenRow[m_Columns.m_Sensitive] = true;

  for (openfluid::machine::SimulatorSignatureRegistry::SimSignaturesByName_t::iterator it =
      Gens.begin(); it != Gens.end(); ++it)
  {
    Gtk::TreeRow SubRow = *mref_TreeModel->append(GenRow.children());
    SubRow[m_Columns.m_Id] = it->second->Signature->ID;
    SubRow[m_Columns.mp_Sign] = it->second;
    SubRow[m_Columns.m_Sensitive] = true;
  }

  mp_TreeView->expand_all();

  if (!Plugs.empty())
    mp_TreeView->get_selection()->select(PlugRow->children().begin());
  else
    mp_TreeView->get_selection()->select(GenRow->children().begin());

  show_all_children();
}

// =====================================================================
// =====================================================================

Glib::ustring AvailSimulatorsWidget::replaceEmpty(Glib::ustring TextToCheck)
{
  if (TextToCheck.empty())
    TextToCheck = _("(unknown)");
  return TextToCheck;
}

// =====================================================================
// =====================================================================

void AvailSimulatorsWidget::onSelectionChanged()
{
  Gtk::TreeIter Iter = mp_TreeView->get_selection()->get_selected();

  if (Iter)
    m_signal_SelectionChanged.emit(Iter->get_value(m_Columns.mp_Sign));
}

// =====================================================================
// =====================================================================

sigc::signal<void, openfluid::machine::ModelItemSignatureInstance*> AvailSimulatorsWidget::signal_SelectionChanged()
{
  return m_signal_SelectionChanged;
}

// =====================================================================
// =====================================================================

void AvailSimulatorsWidget::onRowActivated(const Gtk::TreeModel::Path& Path,
                                          Gtk::TreeViewColumn* /*Column*/)
{
  Gtk::TreeIter Iter = mref_TreeModel->get_iter(Path);

  if (Iter)
  {
    openfluid::machine::ModelItemSignatureInstance* Sign = Iter->get_value(
        m_Columns.mp_Sign);
    if (Sign && m_Model.getFirstItemIndex(Sign->Signature->ID) == -1)
      m_signal_Activated.emit(Sign);
  }
}

// =====================================================================
// =====================================================================

sigc::signal<void, openfluid::machine::ModelItemSignatureInstance*> AvailSimulatorsWidget::signal_Activated()
{
  return m_signal_Activated;
}

// =====================================================================
// =====================================================================

