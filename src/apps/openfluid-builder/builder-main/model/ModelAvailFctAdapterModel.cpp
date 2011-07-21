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
 \file ModelAvailFctAdapterModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelAvailFctAdapterModel.hpp"
#include "BuilderGraphicsHelper.hpp"



sigc::signal<void> ModelAvailFctAdapterModelImpl::signal_SelectionChanged()
{
  return m_signal_SelectionChanged;
}


// =====================================================================
// =====================================================================


sigc::signal<void> ModelAvailFctAdapterModelImpl::signal_FunctionsChanged()
{
  return m_signal_FunctionsChanged;
}


// =====================================================================
// =====================================================================


void ModelAvailFctAdapterModelImpl::createTitleRows()
{
  mp_PluggableTitleRowRef = mref_TreeModel->appendATitleRow(_(
      "Simulation functions"));
  mp_GeneratorTitleRowRef = mref_TreeModel->appendATitleRow(_("Generators"));
}


// =====================================================================
// =====================================================================


Glib::ustring ModelAvailFctAdapterModelImpl::replaceEmpty(
    Glib::ustring TextToCheck)
{
  if (TextToCheck.empty())
    TextToCheck = _("(unknown)");
  return TextToCheck;
}


// =====================================================================
// =====================================================================


void ModelAvailFctAdapterModelImpl::setAPluggableFunction(
    openfluid::machine::SignatureItemInstance* Function)
{
  Gtk::TreeRow TitleRow = mref_TreeModel->appendToRowRef(
      *mp_PluggableTitleRowRef);
  TitleRow[m_Columns.m_Status] = BuilderGraphicsHelper::createPixbufFromFile(
      "fct_status_stable.png");
  TitleRow[m_Columns.m_Id] = Function->Signature->ID;
  TitleRow[m_Columns.m_Domain] = replaceEmpty(Function->Signature->Domain);

  switch (Function->Signature->Status)
  {
    case openfluid::base::EXPERIMENTAL:
      TitleRow[m_Columns.m_Status] = BuilderGraphicsHelper::createPixbufFromFile(
          "fct_status_experimental.png");
      break;
    case openfluid::base::BETA:
      TitleRow[m_Columns.m_Status] = BuilderGraphicsHelper::createPixbufFromFile(
          "fct_status_beta.png");
      break;
    case openfluid::base::STABLE:
      TitleRow[m_Columns.m_Status] = BuilderGraphicsHelper::createPixbufFromFile(
          "fct_status_stable.png");
      break;
  }

  m_SignaturesById[Function->Signature->ID] = Function;
}


// =====================================================================
// =====================================================================


void ModelAvailFctAdapterModelImpl::setAGeneratorFunction(
    openfluid::machine::SignatureItemInstance* Function)
{
  Gtk::TreeRow TitleRow = mref_TreeModel->appendToRowRef(
      *mp_GeneratorTitleRowRef);
  TitleRow[m_Columns.m_Id] = Function->Signature->ID;
  m_SignaturesById[Function->Signature->ID] = Function;
}


// =====================================================================
// =====================================================================


void ModelAvailFctAdapterModelImpl::defineFirstAvailableRow()
{
  if (!mref_TreeModel->children()[0]->children().empty())
    mp_FirstAvailableRowRef = mref_TreeModel->createRowRefFromPathString("0:0");
  else if (!mref_TreeModel->children()[1]->children().empty())
    mp_FirstAvailableRowRef = mref_TreeModel->createRowRefFromPathString("1:0");
  else
    mp_FirstAvailableRowRef = new Gtk::TreeRowReference();
}


// =====================================================================
// =====================================================================


ModelAvailFctAdapterModelImpl::ModelAvailFctAdapterModelImpl(
    ModelAvailFctColumns& Columns) :
  m_Columns(Columns)
{
  mref_TreeModel = BuilderTreeStore::create(m_Columns);
  mref_TreeModel->setTitleColumn(m_Columns.m_Id);
  createTitleRows();
  mp_SelectedSignature = 0;
}


// =====================================================================
// =====================================================================


ModelAvailFctAdapterModelImpl::~ModelAvailFctAdapterModelImpl()
{
  delete mp_GeneratorTitleRowRef;
  delete mp_PluggableTitleRowRef;
  delete mp_FirstAvailableRowRef;
}


// =====================================================================
// =====================================================================


void ModelAvailFctAdapterModelImpl::setSignatures(
    FunctionSignatureRegistry::FctSignaturesByType_t Signatures)
{
  mref_TreeModel->clear();
  m_SignaturesById.clear();
  createTitleRows();
  for (unsigned int i = 0; i
      < Signatures[openfluid::base::ModelItemDescriptor::PluggedFunction].size(); i++)
  {
    setAPluggableFunction(
        Signatures[openfluid::base::ModelItemDescriptor::PluggedFunction][i]);
  }
  for (unsigned int i = 0; i
      < Signatures[openfluid::base::ModelItemDescriptor::Generator].size(); i++)
  {
    setAGeneratorFunction(
        Signatures[openfluid::base::ModelItemDescriptor::Generator][i]);
  }
  defineFirstAvailableRow();
  m_signal_FunctionsChanged.emit();
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::TreeModel> ModelAvailFctAdapterModelImpl::getTreeModel()
{
  return mref_TreeModel;
}


// =====================================================================
// =====================================================================


void ModelAvailFctAdapterModelImpl::setSelectedRow(Gtk::TreeRow Row)
{
  if (Row)
  {
    std::string SelectedId = Row.get_value(m_Columns.m_Id);
    if (Row && m_SignaturesById.find(SelectedId) != m_SignaturesById.end())
      mp_SelectedSignature = m_SignaturesById[SelectedId];
    else
      mp_SelectedSignature = 0;
  }
}


// =====================================================================
// =====================================================================


openfluid::machine::SignatureItemInstance* ModelAvailFctAdapterModelImpl::getSelectedSignature()
{
  return mp_SelectedSignature;
}


// =====================================================================
// =====================================================================


Gtk::TreeRow ModelAvailFctAdapterModelImpl::getFirstAvailableRow()
{
  return mref_TreeModel->getRowFromRowRef(*mp_FirstAvailableRowRef);
}
