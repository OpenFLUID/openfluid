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
  \file DomainModule.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <iostream>

#include <glibmm/i18n.h>

#include "BuilderHelper.hpp"
#include "DomainModule.hpp"


// =====================================================================
// =====================================================================

DomainModule::DomainModule(openfluid::core::CoreRepository & CoreRepos)
  : ModuleInterface("Domain.glade", "ViewportDomain", "MenuDomain", "ToolBarDomain","")
{
  m_ModuleName = _("_Domain");
  m_ModuleLongName = _("Domain");
  mp_StockId = BuilderHelper::createIconStockId(BUILDER_RESOURCE_PATH, "domain-base.svg", "builder-domain-base");

  mp_Builder->get_widget("VBoxInfos",mp_VBoxInfos);
  mp_Builder->get_widget("TreeViewUnits",mp_TreeViewUnits);
  mp_Builder->get_widget("TreeViewInputData",mp_TreeViewInputData);

  createActions();

  mp_MainTreeModel = createMainTreeModel(CoreRepos);

  mp_TreeViewUnits->set_model(mp_MainTreeModel);
  mp_TreeViewInputData->set_model(mp_MainTreeModel);

  initTreeViewUnits();
  initTreeViewInputData();



//  Gtk::Label * Label = 0;
//  mp_Builder->get_widget("label1",Label);
//
//  // List units
//  openfluid::core::UnitsListByClassMap_t::const_iterator UnitsIt;
//
//  unsigned int UnitsCount = 0;
//  for (UnitsIt = CoreRepos.getUnitsByClass()->begin(); UnitsIt != CoreRepos.getUnitsByClass()->end();++UnitsIt )
//  {
//    UnitsCount = UnitsCount + (*UnitsIt).second.getList()->size();
//  }
//
//  Label->set_text(Glib::ustring::compose("%1 units : ",UnitsCount));
//
//  for (UnitsIt = CoreRepos.getUnitsByClass()->begin(); UnitsIt != CoreRepos.getUnitsByClass()->end();++UnitsIt )
//  {
//    Label->set_text(Label->get_text() + "\n" + Glib::ustring::compose(" - %1, %2 units",UnitsIt->first,UnitsIt->second.getList()->size()));
//  }

}


// =====================================================================
// =====================================================================


DomainModule::~DomainModule()
{
//  delete mp_CoreData;
}


// =====================================================================
// =====================================================================

void DomainModule::createActions()
{
  // create action
  Glib::RefPtr<Gtk::Action> ActionCheckDomain;
  ActionCheckDomain = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionCheckDomain"));
  ActionCheckDomain->set_label(_("Check Domain"));
  ActionCheckDomain->set_tooltip(_("Check Domain"));
  ActionCheckDomain->signal_activate().connect(sigc::mem_fun(*this,&DomainModule::actionCheckDomain));

  // add customized icons
  Gtk::StockID * CheckStockId = BuilderHelper::createIconStockId(BUILDER_RESOURCE_PATH, "check-domain.svg", "builder-domain-check");
  ActionCheckDomain->set_stock_id(*CheckStockId);

  m_Actions.push_back(ActionCheckDomain);
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::TreeStore> DomainModule::createMainTreeModel(openfluid::core::CoreRepository & CoreRepos)
{
  initInputDataMap(CoreRepos);

  Glib::RefPtr<Gtk::TreeStore> MainTreeModel = Gtk::TreeStore::create(m_DomainColumns);

  openfluid::core::UnitsListByClassMap_t Units = *CoreRepos.getUnitsByClass();
  openfluid::core::UnitsListByClassMap_t::iterator ItUnits;

  for(ItUnits=Units.begin() ; ItUnits!=Units.end() ; ++ItUnits)
  {
    Gtk::TreeRow RowUnitClass = *(MainTreeModel->append());

    RowUnitClass[m_DomainColumns.m_UnitClass] = ItUnits->first;

    openfluid::core::UnitsList_t * UnitsOfClass = CoreRepos.getUnits(ItUnits->first)->getList();
    openfluid::core::UnitsList_t::iterator ItUnitsOfClass;

    for(ItUnitsOfClass=UnitsOfClass->begin() ; ItUnitsOfClass!=UnitsOfClass->end() ; ++ItUnitsOfClass)
    {
      Gtk::TreeRow RowUnit = *(MainTreeModel->append(RowUnitClass->children()));

      // Unit
      RowUnit[m_DomainColumns.m_Id] = ItUnitsOfClass->getID();
      RowUnit[m_DomainColumns.m_PcsOrder] = ItUnitsOfClass->getProcessOrder();


      // Parents & Tos
      std::list<openfluid::core::UnitClassID_t> Parents;
      std::list<openfluid::core::UnitClassID_t> Tos;

      openfluid::core::UnitsListByClassMap_t::iterator ItClasses;
      for(ItClasses=Units.begin() ; ItClasses!=Units.end() ; ++ItClasses)
      {
        openfluid::core::UnitsPtrList_t * ParentUnits = ItUnitsOfClass->getParentUnits(ItClasses->first);
        openfluid::core::UnitsPtrList_t * ToUnits = ItUnitsOfClass->getToUnits(ItClasses->first);

        openfluid::core::UnitsPtrList_t::iterator ItParentsUnits;
        openfluid::core::UnitsPtrList_t::iterator ItTosUnits;

        if(ParentUnits)
        {
          for(ItParentsUnits=ParentUnits->begin() ; ItParentsUnits!=ParentUnits->end() ; ++ItParentsUnits)
          {
            openfluid::core::UnitClass_t Class = (*ItParentsUnits)->getClass();
            openfluid::core::UnitID_t Id = (*ItParentsUnits)->getID();
            Parents.push_back(std::make_pair(Class,Id));
          }
        }

        if(ToUnits)
        {
          for(ItTosUnits=ToUnits->begin() ; ItTosUnits!=ToUnits->end() ; ++ItTosUnits)
          {
            openfluid::core::UnitClass_t Class = (*ItTosUnits)->getClass();
            openfluid::core::UnitID_t Id = (*ItTosUnits)->getID();
            Tos.push_back(std::make_pair(Class,Id));
          }
        }
      }

      RowUnit[m_DomainColumns.m_UnitsParents] = Parents;
      RowUnit[m_DomainColumns.m_UnitsTos] = Tos;


      // Input Data
      openfluid::core::InputData<openfluid::core::InputDataValue> IData = *ItUnitsOfClass->getInputData();

      InputDataMap_t::iterator ItDataMap;

      for(ItDataMap=m_DomainColumns.m_InputDataMap.begin() ; ItDataMap!=m_DomainColumns.m_InputDataMap.end() ; ++ItDataMap)
      {
        Glib::ustring DataName = ItDataMap->first;

        if(IData.isDataExist(DataName))
        {
          std::string Val;

          IData.getValue(DataName,&Val);

          RowUnit.set_value(*(ItDataMap->second),Val);
        }
      }
    }

  }

  MainTreeModel->set_sort_column(m_DomainColumns.m_Id,Gtk::SORT_ASCENDING);

  return MainTreeModel;
}


// =====================================================================
// =====================================================================


void DomainModule::initInputDataMap(openfluid::core::CoreRepository & CoreRepos)
{
  // 1. Get all InputDataNames

  openfluid::core::UnitsPtrList_t UnitsPtr = *CoreRepos.getUnitsGlobally();
  openfluid::core::UnitsPtrList_t::iterator ItUnitsPtr;

  for(ItUnitsPtr=UnitsPtr.begin() ; ItUnitsPtr!=UnitsPtr.end() ; ++ItUnitsPtr)
  {
    std::vector<openfluid::core::InputDataName_t> DataNames = (*ItUnitsPtr)->getInputData()->getInputDataNames();

    for(unsigned int i=0 ; i<DataNames.size() ; i++)
    {
      m_DomainColumns.m_InputDataMap[DataNames[i]];
    }
  }


  // 2. Create one TreeModelColumn for each InputDataName

  InputDataMap_t::iterator ItDataMap;

  for(ItDataMap=m_DomainColumns.m_InputDataMap.begin() ; ItDataMap!=m_DomainColumns.m_InputDataMap.end() ; ++ItDataMap)
  {
    Gtk::TreeModelColumn<std::string> * Col = new Gtk::TreeModelColumn<std::string>();

    m_DomainColumns.add(*Col);

    ItDataMap->second = Col;
  }

}


// =====================================================================
// =====================================================================


void DomainModule::initTreeViewUnits()
{
  mp_TreeViewUnits->append_column("Unit class",m_DomainColumns.m_UnitClass);
  mp_TreeViewUnits->set_search_column(m_DomainColumns.m_UnitClass);
  mp_TreeViewUnits->get_column(0)->set_sort_column(m_DomainColumns.m_UnitClass);

  mp_TreeViewUnits->append_column("Id",m_DomainColumns.m_Id);
  mp_TreeViewUnits->get_column(1)->set_sort_column(m_DomainColumns.m_Id);

  mp_TreeViewUnits->append_column_editable("Process order",m_DomainColumns.m_PcsOrder/*,"%i"*/);
  mp_TreeViewUnits->get_column(2)->set_sort_column(m_DomainColumns.m_PcsOrder);

  mp_TreeViewUnits->expand_all();
}


// =====================================================================
// =====================================================================


void DomainModule::initTreeViewInputData()
{

  mp_TreeViewInputData->append_column("Unit class",m_DomainColumns.m_UnitClass);
  mp_TreeViewInputData->set_search_column(m_DomainColumns.m_UnitClass);
  mp_TreeViewInputData->get_column(0)->set_sort_column(m_DomainColumns.m_UnitClass);

  mp_TreeViewInputData->append_column("Id",m_DomainColumns.m_Id);
  mp_TreeViewInputData->get_column(1)->set_sort_column(m_DomainColumns.m_Id);


  InputDataMap_t::iterator ItDataMap;

  for(ItDataMap=m_DomainColumns.m_InputDataMap.begin() ; ItDataMap!=m_DomainColumns.m_InputDataMap.end() ; ++ItDataMap)
  {
    int ColNb = mp_TreeViewInputData->append_column_editable(ItDataMap->first,*ItDataMap->second);
    mp_TreeViewInputData->get_column(ColNb-1)->set_sort_column(*ItDataMap->second);
  }

  mp_TreeViewInputData->expand_all();
}


// =====================================================================
// =====================================================================


openfluid::base::DomainDescriptor * DomainModule::getDomainDescriptorWOIData()
{
  openfluid::base::DomainDescriptor * DomainDesc = new openfluid::base::DomainDescriptor();

  std::list<openfluid::base::UnitDescriptor> DefDescriptor;

  Gtk::TreeModel::Children UnitClasses = mp_MainTreeModel->children();
  Gtk::TreeModel::Children::iterator ItUnitClasses;

  for(ItUnitClasses=UnitClasses.begin() ; ItUnitClasses!=UnitClasses.end() ; ++ItUnitClasses)
  {
    openfluid::core::UnitClass_t UnitClass = (*ItUnitClasses)[m_DomainColumns.m_UnitClass];

    Gtk::TreeModel::Children Units = ItUnitClasses->children();
    Gtk::TreeModel::Children::iterator ItUnits;

    for(ItUnits=Units.begin() ; ItUnits!=Units.end() ; ++ItUnits)
    {
      openfluid::base::UnitDescriptor UnitDesc;

      UnitDesc.getUnitClass() = UnitClass;

      UnitDesc.getUnitID() = (*ItUnits)[m_DomainColumns.m_Id];

      UnitDesc.getProcessOrder() = (*ItUnits)[m_DomainColumns.m_PcsOrder];

      UnitDesc.getUnitsParents() = (*ItUnits)[m_DomainColumns.m_UnitsParents];

      UnitDesc.getUnitsTos() = (*ItUnits)[m_DomainColumns.m_UnitsTos];

      DefDescriptor.push_back(UnitDesc);
    }

  }

  DomainDesc->getUnits() = DefDescriptor;

  return DomainDesc;
}


// =====================================================================
// =====================================================================


void DomainModule::setInputData(openfluid::core::CoreRepository & CoreRepos)
{
  /* Temporary array of InputData
   *     | IDataName1 | IDataName2
   * id1 | IDataValue | IDataValue
   * id2 | IDataValue | IDataValue
   * id3 | ...
   */
  std::valarray<std::string> IDataArray;

  int IDataNamesNb = m_DomainColumns.m_InputDataMap.size();
  unsigned int IDataArrayJ = IDataNamesNb + 1;

  Gtk::TreeModel::Children UnitClasses = mp_MainTreeModel->children();
  Gtk::TreeModel::Children::iterator ItUnitClasses;

  InputDataMap_t::iterator ItDataMap;

  for(ItUnitClasses=UnitClasses.begin() ; ItUnitClasses!=UnitClasses.end() ; ++ItUnitClasses)
  {
    openfluid::core::UnitClass_t UnitClass = (*ItUnitClasses)[m_DomainColumns.m_UnitClass];

    Gtk::TreeModel::Children Units = ItUnitClasses->children();
    Gtk::TreeModel::Children::iterator ItUnits;

    unsigned int IDataArrayI = Units.size() + 1;

    IDataArray.resize(IDataArrayJ * IDataArrayI);

    unsigned int i = 1;
    unsigned j;

    for(ItUnits=Units.begin() ; ItUnits!=Units.end() ; ++ItUnits)
    {
      // Add Input Data to temporary array
      std::ostringstream oss;
      oss << (*ItUnits)[m_DomainColumns.m_Id];
      std::string IdStr = oss.str();

      IDataArray[i*IDataArrayJ] = IdStr; // set Unit ID on first column

      j = 1;
      for(ItDataMap=m_DomainColumns.m_InputDataMap.begin() ; ItDataMap!=m_DomainColumns.m_InputDataMap.end() ; ++ItDataMap)
      {
        IDataArray[j] = ItDataMap->first; // set IData Name on first line

        std::string Val = ItUnits->get_value(*ItDataMap->second);

        IDataArray[(i*IDataArrayJ)+j] = Val;

        j++;
      }

      i++;
    }


    // Check if there is Data for each Data Name

    std::vector<std::string> IDataNamesToKeep;

    for(j=1 ; j<IDataArrayJ ; j++)
    {
      // get Input Data by column (by DataName)
      std::valarray<std::string> Col = IDataArray[std::slice(j+IDataArrayJ,IDataArrayI-1,IDataArrayJ)];

      std::ostringstream oss;

      for(i=0 ; i<Col.size() ; i++)
      {
        oss << Col[i];
      }

      if(!oss.str().empty()) // if there is at least one Data for this Data Name
      {
        IDataNamesToKeep.push_back(IDataArray[j]);
      }
    }


    // Add used Input Data to the Unit

    for(ItUnits=Units.begin() ; ItUnits!=Units.end() ; ++ItUnits)
    {
      openfluid::core::Unit * Unit = CoreRepos.getUnit(UnitClass,(*ItUnits)[m_DomainColumns.m_Id]);

      for(i=0 ; i<IDataNamesToKeep.size() ; i++)
      {
        std::string DataName = IDataNamesToKeep[i];

        Unit->getInputData()->setValue(DataName,ItUnits->get_value(*m_DomainColumns.m_InputDataMap[DataName]));
      }
    }

  }

}


// =====================================================================
// =====================================================================


void DomainModule::actionCheckDomain()
{
  Gtk::TreeModel::Children Units = mp_MainTreeModel->children();
  Gtk::TreeModel::Children::iterator ItUnits;

  for(ItUnits=Units.begin() ; ItUnits!=Units.end() ; ++ItUnits)
  {
    std::cout << "***Class " << Glib::ustring((*ItUnits)[m_DomainColumns.m_UnitClass]) << std::endl;

    Gtk::TreeModel::Children UnitsOfClass = ItUnits->children();
    Gtk::TreeModel::Children::iterator ItUUnits;

    for(ItUUnits=UnitsOfClass.begin() ; ItUUnits!=UnitsOfClass.end() ; ++ItUUnits)
    {
      // Unit
      std::cout << "**Id " << (*ItUUnits)[m_DomainColumns.m_Id] << " - Pcs order " << (*ItUUnits)[m_DomainColumns.m_PcsOrder] << std::endl;

      // Parents
      std::cout << "**Parents :" << std::endl;
      std::list<openfluid::core::UnitClassID_t> Parents = (*ItUUnits)[m_DomainColumns.m_UnitsParents];
      std::list<openfluid::core::UnitClassID_t>::iterator ItParents;

      for(ItParents=Parents.begin() ; ItParents!=Parents.end() ; ++ItParents)
      {
        std::cout << "  " << ItParents->first << " - " << ItParents->second << std::endl;
      }

      // Input Data
      std::cout << "**Input Data :" << std::endl;
      InputDataMap_t::iterator ItData;
      for(ItData=m_DomainColumns.m_InputDataMap.begin() ; ItData!=m_DomainColumns.m_InputDataMap.end() ; ++ItData)
      {
        std::cout << "  " << ItData->first << " : " << ItUUnits->get_value(*ItData->second) <<std::endl;
      }
    }

  }

}
