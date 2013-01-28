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
 \file ProjectExplorerAdapterModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ProjectExplorerAdapterModel.hpp"

#include <boost/foreach.hpp>
#include <glibmm/i18n.h>

#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/ware/FunctionSignature.hpp>
#include <openfluid/guicommon/BuilderDescriptor.hpp>

// =====================================================================
// =====================================================================


ProjectExplorerAdapterModelImpl::ProjectExplorerAdapterModelImpl()
{
  mref_TreeModel = BuilderTreeStore::create(m_Columns);

  Gtk::TreeRow Row;

  // Model
  Row = *(mref_TreeModel->append());
  Row[m_Columns.m_Id] = "";
  Row[m_Columns.m_Display] = _("Model");
  Row[m_Columns.m_Category] = ProjectExplorerCategories::EXPLORER_MODEL;
  Row[m_Columns.m_Weight] = Pango::WEIGHT_BOLD;
  mp_ModelRowRef = mref_TreeModel->createRowRefFromIter(*Row);

  // Domain
  Row = *(mref_TreeModel->append());
  Row[m_Columns.m_Id] = "";
  Row[m_Columns.m_Display] = _("Spatial domain");
  Row[m_Columns.m_Category] = ProjectExplorerCategories::EXPLORER_DOMAIN;
  Row[m_Columns.m_Weight] = Pango::WEIGHT_BOLD;
  mp_DomainRowRef = mref_TreeModel->createRowRefFromIter(*Row);

  // Simulation
  Row = *(mref_TreeModel->append());
  Row[m_Columns.m_Id] = "";
  Row[m_Columns.m_Display] = _("Simulation");
  Row[m_Columns.m_Category] = ProjectExplorerCategories::EXPLORER_NONE;
  Row[m_Columns.m_Weight] = Pango::WEIGHT_BOLD;

  // Simulation > Run
  Gtk::TreeRow SubRow1;
  SubRow1 = *(mref_TreeModel->append(Row->children()));
  SubRow1[m_Columns.m_Id] = "";
  SubRow1[m_Columns.m_Display] = _("Run");
  SubRow1[m_Columns.m_Category] = ProjectExplorerCategories::EXPLORER_RUN;
  SubRow1[m_Columns.m_Weight] = Pango::WEIGHT_BOLD;

  // Simulation > Run > RunInfo
  Gtk::TreeRow SubRow2 = *(mref_TreeModel->append(SubRow1->children()));
  SubRow2[m_Columns.m_Id] = "";
  SubRow2[m_Columns.m_Display] = generateRunInfoStr("", "", 1);
  SubRow2[m_Columns.m_Category] = ProjectExplorerCategories::EXPLORER_RUN;
  mp_RunInfoRowRef = mref_TreeModel->createRowRefFromIter(*SubRow2);

  // Simulation > Outputs
//  SubRow1 = *(mref_TreeModel->append(Row->children()));
//  SubRow1[m_Columns.m_Id] = "";
//  SubRow1[m_Columns.m_Display] = _("Outputs");
//  SubRow1[m_Columns.m_Category] = ProjectExplorerCategories::EXPLORER_OUTPUTS;
//  SubRow1[m_Columns.m_Weight] = Pango::WEIGHT_BOLD;

  // Results
//  Row = *(mref_TreeModel->append());
//  Row[m_Columns.m_Id] = "";
//  Row[m_Columns.m_Display] = _("Results");
//  Row[m_Columns.m_Category] = ProjectExplorerCategories::EXPLORER_NONE;
//  Row[m_Columns.m_Weight] = Pango::WEIGHT_BOLD;
//  mp_ResultsRowRef = mref_TreeModel->createRowRefFromIter(*Row);

}

// =====================================================================
// =====================================================================


std::string ProjectExplorerAdapterModelImpl::generateRunInfoStr(
    std::string Begin, std::string End, unsigned int DeltaT)
{
  return Glib::ustring::compose(_("From: %1\nTo: %2\nDelta T: %3 s"), Begin,
      End, DeltaT);
}

// =====================================================================
// =====================================================================


ProjectExplorerAdapterModelImpl::~ProjectExplorerAdapterModelImpl()
{

}

// =====================================================================
// =====================================================================


void ProjectExplorerAdapterModelImpl::initialize(
    openfluid::machine::ModelInstance* ModelInstance,
    openfluid::machine::SimulationBlob* SimBlob,
    openfluid::guicommon::BuilderDescriptor& BuilderDesc)
{
  mp_ModelInstance = ModelInstance;
  mp_SimBlob = SimBlob;
  mp_BuilderDesc = &BuilderDesc;

  updateAll();
}

// =====================================================================
// =====================================================================


void ProjectExplorerAdapterModelImpl::updateAll()
{
  updateModel();

  updateDomain();

  updateRunInfo();
}

// =====================================================================
// =====================================================================


void ProjectExplorerAdapterModelImpl::updateModel()
{
  if (mp_ModelInstance)
  {
    mref_TreeModel->clearChildrenOfRowRef(*mp_ModelRowRef);

    BOOST_FOREACH(openfluid::machine::ModelItemInstance* Fct,mp_ModelInstance->getItems())
{    Gtk::TreeRow Row = *(mref_TreeModel->appendToRowRef(*mp_ModelRowRef));

    Row[m_Columns.m_Id] = Fct->Signature->ID;
    Row[m_Columns.m_Display] = Fct->Signature->ID;
    Row[m_Columns.m_Category] = ProjectExplorerCategories::EXPLORER_MODEL;
  }
}
}

// =====================================================================
// =====================================================================


void ProjectExplorerAdapterModelImpl::updateDomain()
{
  if (mp_SimBlob)
  {
    mref_TreeModel->clearChildrenOfRowRef(*mp_DomainRowRef);

    for (openfluid::core::UnitsListByClassMap_t::const_iterator it =
        mp_SimBlob->getCoreRepository().getUnitsByClass()->begin(); it
        != mp_SimBlob->getCoreRepository().getUnitsByClass()->end(); ++it)
    {
      if (!it->second.getList()->empty())
      {
        Gtk::TreeRow Row = *(mref_TreeModel->appendToRowRef(*mp_DomainRowRef));

        Row[m_Columns.m_Id] = it->first;
        Row[m_Columns.m_Display] = generateClassInfoStr(it->first,
            it->second.getList()->size());
        Row[m_Columns.m_Category] = ProjectExplorerCategories::EXPLORER_CLASS;
      }
    }
  }
}

// =====================================================================
// =====================================================================


std::string ProjectExplorerAdapterModelImpl::generateClassInfoStr(
    std::string ClassName, unsigned int UnitsCount)
{
  return Glib::ustring::compose(_("%1 (%2 units)"), ClassName, UnitsCount);
}

// =====================================================================
// =====================================================================


void ProjectExplorerAdapterModelImpl::updateRunInfo()
{
//  if (mp_FXDesc)
//  {
    Gtk::TreeRow Row = mref_TreeModel->getRowFromRowRef(*mp_RunInfoRowRef);

    Row[m_Columns.m_Display] = generateRunInfoStr(
        mp_BuilderDesc->getRunDescriptor().getBeginDate().getAsISOString(),
        mp_BuilderDesc->getRunDescriptor().getEndDate().getAsISOString(),
        mp_BuilderDesc->getRunDescriptor().getDeltaT());
//  }
}

// =====================================================================
// =====================================================================


//void ProjectExplorerAdapterModelImpl::updateResults(bool WithWarningState)
//{
//  if (WithWarningState)
//  {
//    Gtk::TreeModel::Children ChildrenRows = mref_TreeModel->getRowFromRowRef(
//        *mp_ResultsRowRef).children();
//
//    for (unsigned int i = 0; i < ChildrenRows.size(); i++)
//    {
//      ChildrenRows[i][m_Columns.m_Color] = "red";
//    }
//  } else
//  {
//    if (mp_SimBlob)
//    {
//      mref_TreeModel->clearChildrenOfRowRef(*mp_ResultsRowRef);
//
//      BOOST_FOREACH(openfluid::base::OutputFilesDescriptor FileDesc,mp_SimBlob->getOutputDescriptor().getFileSets())
//{      BOOST_FOREACH(openfluid::base::OutputSetDescriptor SetDesc,FileDesc.getSets())
//      {
//        std::string SetName = SetDesc.getName();
//        std::string ClassName = SetDesc.getUnitsClass();
//        int UnitsCount = 0;
//
//        if(mp_SimBlob->getCoreRepository().getUnits(ClassName))
//        {
//          Gtk::TreeRow Row = *(mref_TreeModel->appendToRowRef(*mp_ResultsRowRef));
//
//          if(SetDesc.isAllUnits())
//          UnitsCount = mp_SimBlob->getCoreRepository().getUnits(ClassName)->getList()->size();
//          else
//          UnitsCount = SetDesc.getUnitsIDs().size();
//
//          Row[m_Columns.m_Id] = SetName;
//          Row[m_Columns.m_Display] = generateSetInfoStr(SetName,ClassName,UnitsCount);
//          Row[m_Columns.m_Category] = ProjectExplorerCategories::EXPLORER_SET;
//        }
//      }
//    }
//  }
//}
//}

// =====================================================================
// =====================================================================


//std::string ProjectExplorerAdapterModelImpl::generateSetInfoStr(
//    std::string SetName, std::string ClassName, unsigned int UnitsCount)
//{
//  return Glib::ustring::compose(_("%1 (%2 - %3 units)"), SetName, ClassName,
//      UnitsCount);
//}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::TreeModel> ProjectExplorerAdapterModelImpl::getTreeModel()
{
  return mref_TreeModel;
}

// =====================================================================
// =====================================================================


void ProjectExplorerAdapterModelImpl::setActivated(Gtk::TreePath Path)
{
  Gtk::TreeIter it = mref_TreeModel->get_iter(Path);

  if (it)
  {
    Gtk::TreeRow Row = *it;

    m_ActivatedElements = std::make_pair(Row[m_Columns.m_Category],
        Row[m_Columns.m_Id]);
  }
}

// =====================================================================
// =====================================================================


std::pair<ProjectExplorerCategories::ProjectExplorerCategory, std::string> ProjectExplorerAdapterModelImpl::getActivatedElements()
{
  return m_ActivatedElements;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


Gtk::TreeRowReference* ProjectExplorerAdapterModelSub::getRunInfoRowRef()
{
  return mp_RunInfoRowRef;
}

// =====================================================================
// =====================================================================


void ProjectExplorerAdapterModelSub::setModelInstance(
    openfluid::machine::ModelInstance* ModelInstance)
{
  mp_ModelInstance = ModelInstance;
}

// =====================================================================
// =====================================================================


void ProjectExplorerAdapterModelSub::setSimulationBlob(
    openfluid::machine::SimulationBlob* SimBlob)
{
  mp_SimBlob = SimBlob;
}

// =====================================================================
// =====================================================================

void ProjectExplorerAdapterModelSub::setFluidXDescriptor(openfluid::guicommon::BuilderDescriptor& BuilderDesc)
{
  mp_BuilderDesc = &BuilderDesc;
}

// =====================================================================
// =====================================================================


std::string ProjectExplorerAdapterModelSub::generateRunInfoStr(
    std::string Begin, std::string End, unsigned int DeltaT)
{
  return ProjectExplorerAdapterModelImpl::generateRunInfoStr(Begin, End, DeltaT);
}
