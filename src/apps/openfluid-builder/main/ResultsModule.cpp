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
  \file ResultsModule.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <iostream>

#include <glibmm/i18n.h>

#include "BuilderHelper.hpp"
#include "ResultsModule.hpp"


// =====================================================================
// =====================================================================

ResultsModule::ResultsModule()
  : ModuleInterface("Results.glade", "ViewportResults", "", "", "")
{
  m_ModuleName = _("_Results");
  m_ModuleLongName = _("Results");
  mp_StockId = BuilderHelper::createIconStockId(BUILDER_RESOURCE_PATH, "results-base.svg", "builder-results-base");

  m_TimeStepTitle = "Step";
  m_DateTimeTitle = "Date Time";

  mp_Builder->get_widget("ComboResultsSetName",mp_ComboSetName);
  mp_Builder->get_widget("EntryResultsClassName",mp_EntryClass);
  mp_Builder->get_widget("ComboResultsIDs",mp_ComboIDs);
  mp_Builder->get_widget("TreeViewResults",mp_TreeViewResults);


  Gdk::Color ColorBase("#78a91a");
  getContainer()->modify_bg(Gtk::STATE_NORMAL,BuilderHelper::applyColorAlpha(ColorBase,0.01));


  // init Combos
  mp_ComboSetName->pack_start(m_ComboColumns.m_SetName);
  mp_ComboIDs->pack_start(m_ComboColumns.m_UnitID);

  mp_ComboSetName->signal_changed().connect(sigc::mem_fun(*this,&ResultsModule::onSetNameChanged));
  mp_ComboIDs->signal_changed().connect(sigc::mem_fun(*this,&ResultsModule::onIDChanged));

  // init TreeView
  mp_TreeViewResults->append_column(m_TimeStepTitle,m_ResultsColumns.m_TimeStep);
  mp_TreeViewResults->append_column(m_DateTimeTitle,m_ResultsColumns.m_DateTime);
  mp_TreeViewResults->get_column(0)->set_sort_column(m_ResultsColumns.m_TimeStep);
  mp_TreeViewResults->get_column(1)->set_sort_column(m_ResultsColumns.m_DateTime);
  mp_TreeViewResults->get_column(0)->set_reorderable(true);
  mp_TreeViewResults->get_column(1)->set_reorderable(true);
  mp_TreeViewResults->get_column(0)->set_resizable(true);
  mp_TreeViewResults->get_column(1)->set_resizable(true);
}


// =====================================================================
// =====================================================================


ResultsModule::~ResultsModule()
{

}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::TreeStore> ResultsModule::createComboByClassTreeModel(const openfluid::base::OutputDescriptor & OutDesc)
{
  Glib::RefPtr<Gtk::TreeStore> ModelFilter = Gtk::TreeStore::create(m_ComboColumns);

  for (unsigned int i=0; i< OutDesc.getFileSets().size();i++)
  {

    for (unsigned int j=0; j< OutDesc.getFileSets()[i].getSets().size();j++)
    {
      Gtk::TreeRow RowSetName = *(ModelFilter->append());

      RowSetName[m_ComboColumns.m_SetName] = OutDesc.getFileSets()[i].getSets()[j].getName();//getUnitsClass();
      RowSetName[m_ComboColumns.m_SetDesc] = OutDesc.getFileSets()[i].getSets()[j];

      std::vector<openfluid::core::UnitID_t> IDs;

      if(OutDesc.getFileSets()[i].getSets()[j].isAllUnits())
      {
        openfluid::core::UnitsList_t * Units = mp_CoreRepos->getUnits(OutDesc.getFileSets()[i].getSets()[j].getUnitsClass())->getList();
        openfluid::core::UnitsList_t::iterator it;
        for (it=Units->begin() ; it!=Units->end() ; ++it)
          IDs.push_back(it->getID());
      }
      else
        IDs = OutDesc.getFileSets()[i].getSets()[j].getUnitsIDs();

      std::sort(IDs.begin(),IDs.end());

      for (unsigned int iUnits = 0; iUnits < IDs.size();iUnits++)
      {
        Gtk::TreeRow RowID = *(ModelFilter->append(RowSetName->children()));

        RowID[m_ComboColumns.m_UnitID] = IDs[iUnits];
      }

    }

  }

  return ModelFilter;
}


// =====================================================================
// =====================================================================


bool ResultsModule::isUnitClassVisible(const Gtk::TreeModel::const_iterator & Iter)
{
  // return true only for root nodes (unit class nodes)
  Gtk::TreeRow Row = *Iter;

  if(Row.parent())
    return false;

  return true;
}


// =====================================================================
// =====================================================================


void ResultsModule::onSetNameChanged()
{
  openfluid::base::OutputSetDescriptor SetDesc = mp_ComboSetName->get_active()->get_value(m_ComboColumns.m_SetDesc);

  // update top panel widgets
  mp_EntryClass->set_text(SetDesc.getUnitsClass());

  Gtk::TreePath Path = Gtk::TreePath(mp_ComboSetName->get_active());

  Glib::RefPtr<Gtk::TreeModelFilter> refFilter = Gtk::TreeModelFilter::create(m_refUnitsModel,Path);

  mp_ComboIDs->set_model(refFilter);

  if(! refFilter->children().empty())
    mp_ComboIDs->set_active(0);

}


// =====================================================================
// =====================================================================


void ResultsModule::onIDChanged()
{
  if(mp_SimInfo)  // Simulation was run
  {
    openfluid::base::OutputSetDescriptor SetDesc = mp_ComboSetName->get_active()->get_value(m_ComboColumns.m_SetDesc);

    openfluid::core::UnitID_t ID = mp_ComboIDs->get_active()->get_value(m_ComboColumns.m_UnitID);

    std::string Class = SetDesc.getUnitsClass();
    unsigned int Precision = SetDesc.getPrecision();

    openfluid::core::Unit * Unit = mp_CoreRepos->getUnit(Class,ID);

    // get all variables names
    std::vector<std::string> AllVars;
    std::vector<std::string> VectorVars;

    if(SetDesc.isAllScalars())
      AllVars = Unit->getScalarVariables()->getVariablesNames();
    else
      AllVars = SetDesc.getScalars();

    if(SetDesc.isAllVectors())
        VectorVars = Unit->getVectorVariables()->getVariablesNames();
    else
        VectorVars = SetDesc.getVectors();

    AllVars.insert(AllVars.end(),VectorVars.begin(),VectorVars.end());


    // remove unused columns...
    std::vector<Gtk::TreeViewColumn*> TreeViewColumns = mp_TreeViewResults->get_columns();

    for(unsigned int i=0 ; i<TreeViewColumns.size() ; i++)
    {
      Glib::ustring VarName = TreeViewColumns[i]->get_title();

      if(VarName != m_TimeStepTitle && VarName != m_DateTimeTitle
          && std::find(AllVars.begin(),AllVars.end(),VarName) == AllVars.end())
      {
        // ...from our map
        m_ResultsColumns.m_VarsMap.erase(VarName);

        // ...from the TreeView
        mp_TreeViewResults->remove_column(*TreeViewColumns[i]);

        //TODO: create own TreeModelColumnRecord to have ability te remove a ColumnBase ?
      }

    }

    // add variables columns...
    for(unsigned int i=0 ; i<AllVars.size() ; i++)
    {
      std::string VarName = AllVars[i];

      // ...(if doesn't still exist)...
      if(m_ResultsColumns.m_VarsMap.find(VarName) == m_ResultsColumns.m_VarsMap.end())
      {
        Gtk::TreeModelColumn<Glib::ustring> * VarCol = new Gtk::TreeModelColumn<Glib::ustring>();

        // ...to TreeModelColumnRecord
        m_ResultsColumns.add(*VarCol);

        // ...to our map
        m_ResultsColumns.m_VarsMap[VarName] = VarCol;

        // ...to the TreeView
        int ColNb = mp_TreeViewResults->append_column(VarName,*VarCol);
        mp_TreeViewResults->get_column(ColNb-1)->set_sort_column(*VarCol);
        mp_TreeViewResults->get_column(ColNb-1)->set_reorderable(true);
        mp_TreeViewResults->get_column(ColNb-1)->set_resizable(true);
      }
    }

    Glib::RefPtr<Gtk::ListStore> ListStore = Gtk::ListStore::create(m_ResultsColumns);

    // leave this AFTER TreeModelColumn additions !
    ListStore->set_column_types(m_ResultsColumns);
    mp_TreeViewResults->set_model(ListStore);


    // create a SimulationStatus to iterate over time steps
    openfluid::base::SimulationStatus SimStatus(mp_SimInfo->getStartTime(),
        mp_SimInfo->getEndTime(),
        mp_SimInfo->getTimeStep());

    VarsMap_t VarsMap = m_ResultsColumns.m_VarsMap;
    VarsMap_t::iterator it;

    // fill TreeModel with values of variables for each time step
    do
    {
      Gtk::TreeRow Row = *(ListStore->append());

      unsigned int Step = SimStatus.getCurrentStep();

      Row[m_ResultsColumns.m_TimeStep] = Step;
      Row[m_ResultsColumns.m_DateTime] = SimStatus.getCurrentTime().getAsISOString();

      for(it=VarsMap.begin() ; it!=VarsMap.end() ; ++it)
      {
        std::string VarName = it->first;

        openfluid::core::ScalarValue ScalarValue = 0.0;
        openfluid::core::VectorValue VectorValue;

        Glib::ustring ValueStr;

        if(Unit->getScalarVariables()->getValue(VarName,Step,&ScalarValue))
        {
          ValueStr = Glib::ustring::format(std::fixed,std::setprecision(Precision),ScalarValue);

          Row[*it->second] = ValueStr;
        }
        else if(Unit->getVectorVariables()->getValue(VarName,Step,&VectorValue))
        {
          if(VectorValue.getSize()!=0)
            ValueStr = Glib::ustring::format(std::fixed,std::setprecision(Precision),VectorValue[0]);

          for(unsigned int i=1 ; i<VectorValue.getSize() ; i++)
          {
            Glib::ustring ValueStrNew = Glib::ustring::format(std::fixed,std::setprecision(Precision),VectorValue[i]);

            ValueStr = Glib::ustring::compose("%1%2%3",ValueStr,"\t ",ValueStrNew);
          }

          Row[*it->second] = ValueStr;
        }
      }

    } while (SimStatus.switchToNextStep());

    mp_TreeViewResults->expand_all();

    // delete TreeModelColumns * ?

  }
  else
  {
    Gtk::MessageDialog MsgBox(_("No value... Run the simulation before !"));
    MsgBox.run();
  }

}


// =====================================================================
// =====================================================================


void ResultsModule::init(const openfluid::base::OutputDescriptor & OutDesc,
               openfluid::core::CoreRepository * CoreRepos,
               openfluid::base::SimulationInfo * SimInfo)
{
  mp_SimInfo = SimInfo;
  mp_CoreRepos = CoreRepos;

  m_refUnitsModel = createComboByClassTreeModel(OutDesc);

  Glib::RefPtr<Gtk::TreeModelFilter> refUnitsClassFilter = Gtk::TreeModelFilter::create(m_refUnitsModel);
  refUnitsClassFilter->set_visible_func(sigc::mem_fun(*this,&ResultsModule::isUnitClassVisible));

  mp_ComboSetName->set_model(refUnitsClassFilter);

  if(! mp_ComboSetName->get_model()->children().empty())
    mp_ComboSetName->set_active(0);
}
