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
 \file SignatureDetailWidget.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "SignatureDetailWidget.hpp"

#include <boost/filesystem/path.hpp>
#include <glibmm/i18n.h>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/ware/FunctionSignature.hpp>
#include <openfluid/core/Value.hpp>
#include "WareItemInfoWidget.hpp"

// =====================================================================
// =====================================================================

SignatureDetailWidget::SignatureDetailWidget()
{
  mref_ParamsModel = Gtk::TreeStore::create(m_Columns);
  mref_VarsModel = Gtk::TreeStore::create(m_Columns);
  mref_IDataModel = Gtk::TreeStore::create(m_Columns);
  mref_EventsModel = Gtk::TreeStore::create(m_Columns);
  mref_XFilesModel = Gtk::TreeStore::create(m_Columns);

  mp_ParamsTreeView = new Gtk::TreeView();
  mp_ParamsTreeView->append_column(_("Name"), m_Columns.m_Name);
  mp_ParamsTreeView->append_column(_("SI Unit"), m_Columns.m_Unit);
  mp_ParamsTreeView->append_column(_("Description"), m_Columns.m_Description);
  mp_ParamsTreeView->set_model(mref_ParamsModel);

  mp_VarsTreeView = new Gtk::TreeView();
  mp_VarsTreeView->append_column(_("Name"), m_Columns.m_Name);
  mp_VarsTreeView->append_column(_("Type"), m_Columns.m_Type);
  mp_VarsTreeView->append_column(_("SI Unit"), m_Columns.m_Unit);
  mp_VarsTreeView->append_column(_("Unit class"), m_Columns.m_Class);
  mp_VarsTreeView->append_column(_("Description"), m_Columns.m_Description);
  mp_VarsTreeView->set_model(mref_VarsModel);

  mp_IDataTreeView = new Gtk::TreeView();
  mp_IDataTreeView->append_column(_("Name"), m_Columns.m_Name);
  mp_IDataTreeView->append_column(_("Unit class"), m_Columns.m_Class);
  mp_IDataTreeView->set_model(mref_IDataModel);

  mp_EventsTreeView = new Gtk::TreeView();
  mp_EventsTreeView->append_column(_("Unit class"), m_Columns.m_Class);
  mp_EventsTreeView->set_model(mref_EventsModel);

  mp_ExtraFilesTreeView = new Gtk::TreeView();
  mp_ExtraFilesTreeView->append_column(_("Name"), m_Columns.m_Name);
  mp_ExtraFilesTreeView->set_model(mref_XFilesModel);

  mp_ParamsWin = new Gtk::ScrolledWindow();
  mp_ParamsWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_ParamsWin->add(*mp_ParamsTreeView);

  mp_VarsWin = new Gtk::ScrolledWindow();
  mp_VarsWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_VarsWin->add(*mp_VarsTreeView);

  mp_IDataWin = new Gtk::ScrolledWindow();
  mp_IDataWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_IDataWin->add(*mp_IDataTreeView);

  mp_EventsWin = new Gtk::ScrolledWindow();
  mp_EventsWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_EventsWin->add(*mp_EventsTreeView);

  mp_ExtraFilesWin = new Gtk::ScrolledWindow();
  mp_ExtraFilesWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_ExtraFilesWin->add(*mp_ExtraFilesTreeView);

  set_visible(true);
}

// =====================================================================
// =====================================================================

SignatureDetailWidget::~SignatureDetailWidget()
{
  // widgets are not managed but explicitly deleted to avoid their destruction when removing notebook pages
  delete mp_ParamsWin;
  delete mp_VarsWin;
  delete mp_IDataWin;
  delete mp_EventsWin;
  delete mp_ExtraFilesWin;

  delete mp_ParamsTreeView;
  delete mp_VarsTreeView;
  delete mp_IDataTreeView;
  delete mp_EventsTreeView;
  delete mp_ExtraFilesTreeView;
}

// =====================================================================
// =====================================================================

void SignatureDetailWidget::update(
    openfluid::machine::ModelItemSignatureInstance* Signature)
{
  int Nb = get_n_pages();
  for (int i = 0; i < Nb; i++)
    remove_page(-1);

  if (!Signature)
    return;

  append_page(
      *Gtk::manage(new WareItemInfoWidget(*Signature, *Signature->Signature)),
      _("Information"));

  mref_ParamsModel->clear();
  updateParamsModel(Signature->Signature->HandledData.FunctionParams);
  if (!mref_ParamsModel->children().empty())
    append_page(*mp_ParamsWin, _("Parameters"));

  mref_VarsModel->clear();
  updateVarsModel(Signature->Signature->HandledData.ProducedVars,
                  _("Produced"));
  updateVarsModel(Signature->Signature->HandledData.UpdatedVars, _("Updated"));
  updateVarsModel(Signature->Signature->HandledData.RequiredVars,
                  _("Required"));
  updateVarsModel(Signature->Signature->HandledData.UsedVars, _("Used"));
  updateVarsModel(Signature->Signature->HandledData.RequiredPrevVars,
                  _("Required t-1"));
  updateVarsModel(Signature->Signature->HandledData.UsedPrevVars,
                  _("Used t-1"));
  if (!mref_VarsModel->children().empty())
    append_page(*mp_VarsWin, _("Variables"));
  mp_VarsTreeView->expand_all();

  mref_IDataModel->clear();
  updateIDataModel(Signature->Signature->HandledData.ProducedInputdata,
                   _("Produced"));
  updateIDataModel(Signature->Signature->HandledData.RequiredInputdata,
                   _("Required"));
  updateIDataModel(Signature->Signature->HandledData.UsedInputdata, _("Used"));
  if (!mref_IDataModel->children().empty())
    append_page(*mp_IDataWin, _("Inputdata"));
  mp_IDataTreeView->expand_all();

  mref_EventsModel->clear();
  updateEventsModel(Signature->Signature->HandledData.UsedEventsOnUnits);
  if (!mref_EventsModel->children().empty())
    append_page(*mp_EventsWin, _("Events"));

  mref_XFilesModel->clear();
  updateXFilesModel(Signature->Signature->HandledData.RequiredExtraFiles,
                    _("Required"));
  updateXFilesModel(Signature->Signature->HandledData.UsedExtraFiles,
                    _("Used"));
  if (!mref_XFilesModel->children().empty())
    append_page(*mp_ExtraFilesWin, _("Extra Files"));
  mp_ExtraFilesTreeView->expand_all();

  set_current_page(0);
  show_all_children(true);
}

// =====================================================================
// =====================================================================

void SignatureDetailWidget::updateParamsModel(
    const std::vector<openfluid::ware::SignatureHandledDataItem>& Items)
{
  for (unsigned int i = 0; i < Items.size(); i++)
  {
    Gtk::TreeRow Row = *mref_ParamsModel->append();

    Row[m_Columns.m_Name] = Items[i].DataName;
    Row[m_Columns.m_Unit] = Items[i].DataUnit;
    Row[m_Columns.m_Description] = Items[i].Description;
  }
}

// =====================================================================
// =====================================================================

void SignatureDetailWidget::updateVarsModel(
    const std::vector<openfluid::ware::SignatureHandledTypedDataItem>& Items,
    std::string SubTitle)
{
  if (!Items.empty())
  {
    Gtk::TreeRow Row = *mref_VarsModel->append();
    Row[m_Columns.m_Name] = SubTitle;

    for (unsigned int i = 0; i < Items.size(); i++)
    {
      Gtk::TreeRow SubRow = *mref_VarsModel->append(Row->children());
      SubRow[m_Columns.m_Name] = Items[i].DataName;
      SubRow[m_Columns.m_Type] = openfluid::core::Value::getStringFromValueType(
          Items[i].DataType);
      SubRow[m_Columns.m_Unit] = Items[i].DataUnit;
      SubRow[m_Columns.m_Class] = Items[i].UnitClass;
      SubRow[m_Columns.m_Description] = Items[i].Description;
    }
  }
}

// =====================================================================
// =====================================================================

void SignatureDetailWidget::updateIDataModel(
    const std::vector<openfluid::ware::SignatureHandledDataItem>& Items,
    std::string SubTitle)
{
  if (!Items.empty())
  {
    Gtk::TreeRow Row = *mref_IDataModel->append();
    Row[m_Columns.m_Name] = SubTitle;

    for (unsigned int i = 0; i < Items.size(); i++)
    {
      Gtk::TreeRow SubRow = *mref_IDataModel->append(Row->children());
      SubRow[m_Columns.m_Name] = Items[i].DataName;
      SubRow[m_Columns.m_Class] = Items[i].UnitClass;
    }
  }
}

// =====================================================================
// =====================================================================

void SignatureDetailWidget::updateEventsModel(
    const std::vector<std::string>& Items)
{
  for (unsigned int i = 0; i < Items.size(); i++)
  {
    Gtk::TreeRow Row = *mref_EventsModel->append();
    Row[m_Columns.m_Class] = Items[i];
  }
}

// =====================================================================
// =====================================================================

void SignatureDetailWidget::updateXFilesModel(
    const std::vector<std::string>& Items, std::string SubTitle)
{
  if (!Items.empty())
  {
    Gtk::TreeRow Row = *mref_XFilesModel->append();
    Row[m_Columns.m_Name] = SubTitle;

    for (unsigned int i = 0; i < Items.size(); i++)
    {
      Gtk::TreeRow SubRow = *mref_XFilesModel->append(Row->children());
      SubRow[m_Columns.m_Name] = Items[i];
    }
  }
}

// =====================================================================
// =====================================================================
