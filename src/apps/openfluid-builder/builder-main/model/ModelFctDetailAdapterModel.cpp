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
 \file ModelFctDetailAdapterModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelFctDetailAdapterModel.hpp"

#include <glibmm/i18n.h>
#include <boost/filesystem.hpp>

#include <openfluid/machine/ModelItemInstance.hpp>

#include "ModelFctDetailParamsTreeStore.hpp"
#include "ModelFctDetailVarsTreeStore.hpp"
#include "ModelFctDetailIDataTreeStore.hpp"
#include "ModelFctDetailEventsTreeStore.hpp"
#include "ModelFctDetailExtraFilesTreeStore.hpp"

// =====================================================================
// =====================================================================


Glib::ustring ModelFctDetailAdapterModelImpl::replaceEmpty(
    Glib::ustring TextToCheck)
{
  if (TextToCheck.empty())
    TextToCheck = _("(unknown)");
  return TextToCheck;
}

// =====================================================================
// =====================================================================


void ModelFctDetailAdapterModelImpl::extractInfos()
{
  std::string val;
  m_Infos["id"] = replaceEmpty(mp_Signature->Signature->ID);
  m_Infos["name"] = replaceEmpty(mp_Signature->Signature->Name);
  m_Infos["description"] = replaceEmpty(mp_Signature->Signature->Description);
  m_Infos["version"] = replaceEmpty(mp_Signature->Signature->Version);
  m_Infos["domain"] = replaceEmpty(mp_Signature->Signature->Domain);
  m_Infos["process"] = replaceEmpty(mp_Signature->Signature->Process);
  m_Infos["method"] = replaceEmpty(mp_Signature->Signature->Method);
  m_Infos["author"] = replaceEmpty(mp_Signature->Signature->Author);
  m_Infos["authormail"] = replaceEmpty(mp_Signature->Signature->AuthorEmail);

  m_Infos["path"]
      = ((boost::filesystem::path) mp_Signature->Filename).parent_path().string();

  switch (mp_Signature->Signature->Status)
  {
    case openfluid::ware::EXPERIMENTAL:
      m_Infos["status"] = "Experimental";
      break;
    case openfluid::ware::BETA:
      m_Infos["status"] = "Beta";
      break;
    case openfluid::ware::STABLE:
      m_Infos["status"] = "Stable";
      break;
    default:
      m_Infos["status"] = replaceEmpty("");
  }
}

// =====================================================================
// =====================================================================


void ModelFctDetailAdapterModelImpl::updateTreeModel(Glib::RefPtr<
    ModelFctDetailTreeStore> TreeModel)
{
  TreeModel->clear();
  TreeModel->createTitleRows();
  TreeModel->extractItems(mp_Signature->Signature);
}

// =====================================================================
// =====================================================================


ModelFctDetailAdapterModelImpl::ModelFctDetailAdapterModelImpl(
    ModelFctDetailColumns& Columns) :
  m_Columns(Columns)
{
  mref_ParamsModel = ModelFctDetailParamsTreeStore::create(m_Columns);
  mref_VarsModel = ModelFctDetailVarsTreeStore::create(m_Columns);
  mref_IDataModel = ModelFctDetailIDataTreeStore::create(m_Columns);
  mref_EventsModel = ModelFctDetailEventsTreeStore::create(m_Columns);
  mref_XFilesModel = ModelFctDetailExtraFilesTreeStore::create(m_Columns);

  mref_ParamsModel->setTitleColumn(m_Columns.m_Name);
}

// =====================================================================
// =====================================================================


ModelFctDetailAdapterModelImpl::~ModelFctDetailAdapterModelImpl()
{
}

// =====================================================================
// =====================================================================

void ModelFctDetailAdapterModelImpl::setFctToDisplay(
    openfluid::machine::ModelItemSignatureInstance* Signature)
{
  mp_Signature = Signature;

  m_Infos.clear();
  extractInfos();

  updateTreeModel(mref_ParamsModel);
  updateTreeModel(mref_VarsModel);
  updateTreeModel(mref_IDataModel);
  updateTreeModel(mref_EventsModel);
  updateTreeModel(mref_XFilesModel);
}

// =====================================================================
// =====================================================================


bool ModelFctDetailAdapterModelImpl::isAPluggableFct()
{
  return !(mp_Signature && mp_Signature->ItemType
      == openfluid::base::ModelItemDescriptor::Generator);
}

// =====================================================================
// =====================================================================


std::map<std::string, std::string> ModelFctDetailAdapterModelImpl::getInfos()
{
  return m_Infos;
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::TreeModel> ModelFctDetailAdapterModelImpl::getParamsModel()
{
  return mref_ParamsModel;
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::TreeModel> ModelFctDetailAdapterModelImpl::getVarsModel()
{
  return mref_VarsModel;
}


// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::TreeModel> ModelFctDetailAdapterModelImpl::getIDataModel()
{
  return mref_IDataModel;
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::TreeModel> ModelFctDetailAdapterModelImpl::getExtraFilesModel()
{
  return mref_XFilesModel;
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::TreeModel> ModelFctDetailAdapterModelImpl::getEventsModel()
{
  return mref_EventsModel;
}
