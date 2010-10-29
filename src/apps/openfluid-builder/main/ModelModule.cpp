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
  \file ModelModule.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <iostream>

#include <glibmm/i18n.h>

#include "BuilderHelper.hpp"
#include "ModelModule.hpp"


// =====================================================================
// =====================================================================


ModelModule::ModelModule(openfluid::machine::ModelInstance & Model)
: ModuleInterface("Model.glade", "ViewportModel", "MenuModel", "ToolBarModel", /*"StatusBarWidgetModel"*/"")
{
  m_ModuleName = _("_Model");
  m_ModuleLongName = _("Model");
  mp_StockId = BuilderHelper::createIconStockId(BUILDER_RESOURCE_PATH, "model-base.svg", "builder-model-base");

  createActions();

  openfluid::machine::ArrayOfModelItemInstance PlugContainers = openfluid::machine::PluginManager::getInstance()->getAvailableFunctions();

  for(unsigned int i=0 ; i<PlugContainers.size() ; i++)
      PlugContainers[i]->ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::ArrayOfModelItemInstance GeneratorContainers = createGeneratorContainers();

  // create Main TreeModel
  mp_MainTreeModel = createMainTreeModel(PlugContainers,GeneratorContainers);

  // create Available functions panel
  mp_ModelAvailFct = new ModelAvailFct(mp_Builder,mp_MainTreeModel);

  openfluid::machine::ArrayOfModelItemInstance AllFctContainers = PlugContainers;
  AllFctContainers.insert(AllFctContainers.end(),GeneratorContainers.begin(),GeneratorContainers.end());

  // create Used functions panel
  mp_ModelUsedFct = new ModelUsedFct(mp_Builder,AllFctContainers,Model);

  // create StatusBar
  mp_StatusBarWidget = new StatusInterface(_("Model Status"));

  mp_StatusParamsValues = new StatusItemInterface(_("Params values"));
  StatusItemInterface * Statustest = new StatusItemInterface(_("Test"));

  static_cast<StatusInterface*>(mp_StatusBarWidget)->addAStatusItem(mp_StatusParamsValues);
  static_cast<StatusInterface*>(mp_StatusBarWidget)->addAStatusItem(Statustest, true);

}


// =====================================================================
// =====================================================================


ModelModule::~ModelModule()
{
  /*TODO: delete custom objects */

  delete mp_ModelAvailFct;
  delete mp_ModelUsedFct;
  delete mp_StatusBarWidget;

  mp_ModelAvailFct = 0;
  mp_ModelUsedFct = 0;
  mp_StatusBarWidget = 0;
}


// =====================================================================
// =====================================================================


void ModelModule::createActions()
{
//  // create action
//  Glib::RefPtr<Gtk::Action> ActionCheckModel;
//  ActionCheckModel = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionCheckModel"));
//  ActionCheckModel->set_label(_("Check Model"));
//  ActionCheckModel->set_tooltip(_("Check Model"));
//  ActionCheckModel->signal_activate().connect(sigc::mem_fun(*this,&ModelModule::actionCheckModel));
//
//  // add customized icons
//  Gtk::StockID * CheckStockId = BuilderHelper::createIconStockId(BUILDER_RESOURCE_PATH, "check-model.svg", "builder-model-check");
//  ActionCheckModel->set_stock_id(*CheckStockId);
//
//  m_Actions.push_back(ActionCheckModel);

}


// =====================================================================
// =====================================================================


openfluid::machine::ArrayOfModelItemInstance ModelModule::createGeneratorContainers()
{
  openfluid::machine::ArrayOfModelItemInstance GeneratorContainers;

  GeneratorContainers.push_back(createGeneratorInstance(openfluid::base::GeneratorDescriptor::Fixed));
  GeneratorContainers.push_back(createGeneratorInstance(openfluid::base::GeneratorDescriptor::Random));
  GeneratorContainers.push_back(createGeneratorInstance(openfluid::base::GeneratorDescriptor::Interp));

  return GeneratorContainers;
}


// =====================================================================
// =====================================================================


openfluid::machine::ModelItemInstance * ModelModule::createGeneratorInstance(openfluid::base::GeneratorDescriptor::GeneratorMethod GeneratorMethod)
{
  openfluid::base::SignatureHandledDataItem * VarName = new openfluid::base::SignatureHandledDataItem(
      "Varname","","Name of the produced variable","-");
  openfluid::base::SignatureHandledDataItem * UnitClass = new openfluid::base::SignatureHandledDataItem(
      "Unitclass","","Unit class of the produced variable","-");
  openfluid::base::SignatureHandledDataItem * VarSize = new openfluid::base::SignatureHandledDataItem(
      "Varsize","","Optionnal : to produce a vector variable instead of a scalar variable","-");


  openfluid::machine::ModelItemInstance * IInstance = new openfluid::machine::ModelItemInstance();

  openfluid::base::FunctionSignature* Signature = new openfluid::base::FunctionSignature();

  openfluid::machine::Generator * GenInstance;

  IInstance->SDKCompatible = true;
  IInstance->ItemType = openfluid::base::ModelItemDescriptor::Generator;

  Signature->HandledData.FunctionParams.push_back(*VarName);
  Signature->HandledData.FunctionParams.push_back(*UnitClass);
  Signature->HandledData.FunctionParams.push_back(*VarSize);

  switch(GeneratorMethod)
  {
    case openfluid::base::GeneratorDescriptor::Fixed:
      Signature->ID = "(generator) Fixed";
      Signature->Name = "Fixed Generator";
      Signature->Description = "Generates a constant value";

      Signature->HandledData.FunctionParams.push_back(
          openfluid::base::SignatureHandledDataItem(
              "fixedvalue","","Value to produce","-"));

      GenInstance = new openfluid::machine::FixedGenerator();
      break;

    case openfluid::base::GeneratorDescriptor::Random:
      Signature->ID = "(generator) Random";
      Signature->Name = "Random Generator";
      Signature->Description = "Generates a random value in a range";

      Signature->HandledData.FunctionParams.push_back(
          openfluid::base::SignatureHandledDataItem(
              "min","","Lower bound of the random range for the value to produce","-"));

      Signature->HandledData.FunctionParams.push_back(
          openfluid::base::SignatureHandledDataItem(
              "max","","Upper bound of the random range for the value to produce","-"));

      GenInstance = new openfluid::machine::RandomGenerator();
      break;

    case openfluid::base::GeneratorDescriptor::Interp:
      Signature->ID = "(generator) Interp";
      Signature->Name = "Interpolation Generator";
      Signature->Description = "Generates an interpolated value from given data series";

      Signature->HandledData.FunctionParams.push_back(
          openfluid::base::SignatureHandledDataItem(
              "sources","","Data sources filename for the value to produce","-"));

      Signature->HandledData.FunctionParams.push_back(
          openfluid::base::SignatureHandledDataItem(
              "distribution","","Distribution filename for the value to produce","-"));

      GenInstance = new openfluid::machine::InterpGenerator();
      break;

    default:
      throw openfluid::base::OFException("ModelModule::createGeneratorInstance","unknown generator type");
  }

  IInstance->Function = GenInstance;
  IInstance->Signature = Signature;

  return IInstance;
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::TreeStore> ModelModule::createMainTreeModel(openfluid::machine::ArrayOfModelItemInstance PlugContainers, openfluid::machine::ArrayOfModelItemInstance GeneratorContainers)
{
  Glib::RefPtr<Gdk::Pixbuf> IconRed = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,"red.svg"),8,-1,true);
  Glib::RefPtr<Gdk::Pixbuf> IconOrange = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,"orange.svg"),8,-1,true);
  Glib::RefPtr<Gdk::Pixbuf> IconGreen = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,"green.svg"),8,-1,true);

  Glib::RefPtr<Gtk::TreeStore> MainTreeModel = Gtk::TreeStore::create(m_Columns);


  // 0. Simulation functions rows

  Gtk::TreeModel::Row RowFctTitle = *(MainTreeModel->append());

  setRowTitle(&RowFctTitle,"Simulation Functions",ModelColumns::SimulationFunctions);

  for (unsigned int i=0;i<PlugContainers.size();i++)
  {
    if (PlugContainers[i]->SDKCompatible && PlugContainers[i]->Signature!=NULL)
    {
      Gtk::TreeModel::Row RowFct = *(MainTreeModel->append(RowFctTitle.children()));

      RowFct[m_Columns.m_Type] = ModelColumns::SimulationFunctions;
      RowFct[m_Columns.m_IsTitle] = false;

      RowFct[m_Columns.m_Id] = PlugContainers[i]->Signature->ID;
      RowFct[m_Columns.m_FunctionName] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Name,("(unknown)"));
      RowFct[m_Columns.m_FunctionPath] = ((boost::filesystem::path)PlugContainers[i]->Filename).parent_path().string();

      RowFct[m_Columns.m_Description] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Description,("(none)"));
      RowFct[m_Columns.m_FunctionVersion] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Version,("(unknown)"));

      RowFct[m_Columns.m_FunctionDomain] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Domain,("(unknown)"));
      RowFct[m_Columns.m_FunctionProcess] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Process,("(unknown)"));
      RowFct[m_Columns.m_FunctionMethod] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Method,("(unknown)"));
      RowFct[m_Columns.m_FunctionAuthorName] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Author,("(unknown)"));
      RowFct[m_Columns.m_FunctionAuthorEmail] = openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->AuthorEmail,("(unknown)"));

      switch(PlugContainers[i]->Signature->Status)
      {
        case openfluid::base::EXPERIMENTAL:
          RowFct[m_Columns.m_FunctionStatusStr] = "Experimental";
          RowFct[m_Columns.m_FunctionStatusIcon] = IconRed;
          break;

        case openfluid::base::BETA:
          RowFct[m_Columns.m_FunctionStatusStr] = "Beta";
          RowFct[m_Columns.m_FunctionStatusIcon] = IconOrange;
          break;

        case openfluid::base::STABLE:
          RowFct[m_Columns.m_FunctionStatusStr] = "Stable";
          RowFct[m_Columns.m_FunctionStatusIcon] = IconGreen;
          break;

        default:
          RowFct[m_Columns.m_FunctionStatusStr] = "(unknown)";
      }

      // Simulation functions Parameters
      createHandleDataTreeRows(MainTreeModel,&RowFct,"Parameters",ModelColumns::Parameters,PlugContainers[i]->Signature->HandledData.FunctionParams,false);

      // Simulation functions Input Data
      Gtk::TreeModel::Row RowFctInputDataTitle = *(MainTreeModel->append(RowFct.children()));

      setRowTitle(&RowFctInputDataTitle,"Input Data",ModelColumns::InputData,false);

      createHandleDataTreeRows(MainTreeModel,&RowFctInputDataTitle,"Required",ModelColumns::InputData,PlugContainers[i]->Signature->HandledData.RequiredInput);
      createHandleDataTreeRows(MainTreeModel,&RowFctInputDataTitle,"Used",ModelColumns::InputData,PlugContainers[i]->Signature->HandledData.UsedInput);


      // Simulation functions Variables
      Gtk::TreeModel::Row RowFctVariablesTitle = *(MainTreeModel->append(RowFct.children()));

      setRowTitle(&RowFctVariablesTitle,"Variables",ModelColumns::Variables,false);

      createHandleDataTreeRows(MainTreeModel,&RowFctVariablesTitle,"Produced",ModelColumns::Variables,PlugContainers[i]->Signature->HandledData.ProducedVars);
      createHandleDataTreeRows(MainTreeModel,&RowFctVariablesTitle,"Updated",ModelColumns::Variables,PlugContainers[i]->Signature->HandledData.UpdatedVars);
      createHandleDataTreeRows(MainTreeModel,&RowFctVariablesTitle,"Required",ModelColumns::Variables,PlugContainers[i]->Signature->HandledData.RequiredVars);
      createHandleDataTreeRows(MainTreeModel,&RowFctVariablesTitle,"Used",ModelColumns::Variables,PlugContainers[i]->Signature->HandledData.UsedVars);
      createHandleDataTreeRows(MainTreeModel,&RowFctVariablesTitle,"Required t-1",ModelColumns::Variables,PlugContainers[i]->Signature->HandledData.RequiredPrevVars);
      createHandleDataTreeRows(MainTreeModel,&RowFctVariablesTitle,"Used t-1",ModelColumns::Variables,PlugContainers[i]->Signature->HandledData.UsedPrevVars);

      // Simulation functions Events
      Gtk::TreeModel::Row RowFctEventsTitle = *(MainTreeModel->append(RowFct.children()));

      setRowTitle(&RowFctEventsTitle,"Events",ModelColumns::Events, false);

      std::vector<std::string> TheEvents = PlugContainers[i]->Signature->HandledData.UsedEventsOnUnits;
      for(unsigned int j=0 ; j<TheEvents.size() ; j++)
      {
        Gtk::TreeModel::Row RowEvents = *(MainTreeModel->append(RowFctEventsTitle.children()));
        RowEvents[m_Columns.m_HandleDataUnitClass] = TheEvents[j];
        RowEvents[m_Columns.m_Type] = ModelColumns::Events;
        RowEvents[m_Columns.m_IsTitle] = false;
      }

      // Simulation functions Extra Files
      Gtk::TreeModel::Row RowFctExtraFilesTitle = *(MainTreeModel->append(RowFct.children()));

      setRowTitle(&RowFctExtraFilesTitle,"Extra Files",ModelColumns::ExtraFiles, false);

      // -- Required Extra Files
      Gtk::TreeModel::Row RowFctExtraFilesRequiredTitle = *(MainTreeModel->append(RowFctExtraFilesTitle.children()));

      setRowTitle(&RowFctExtraFilesRequiredTitle,"Required",ModelColumns::ExtraFiles);

      std::vector<std::string> RequiredExtraFiles = PlugContainers[i]->Signature->HandledData.RequiredExtraFiles;
      for(unsigned int j=0 ; j<RequiredExtraFiles.size() ; j++)
      {
        Gtk::TreeModel::Row RowExtraFileRequired = *(MainTreeModel->append(RowFctExtraFilesRequiredTitle.children()));
        RowExtraFileRequired[m_Columns.m_Id] = RequiredExtraFiles[j];
        RowExtraFileRequired[m_Columns.m_Type] = ModelColumns::ExtraFiles;
        RowExtraFileRequired[m_Columns.m_IsTitle] = false;
      }

      // -- Used Extra Files
      Gtk::TreeModel::Row RowFctExtraFilesUsedTitle = *(MainTreeModel->append(RowFctExtraFilesTitle.children()));

      setRowTitle(&RowFctExtraFilesUsedTitle,"Used",ModelColumns::ExtraFiles);

      std::vector<std::string> UsedExtraFiles = PlugContainers[i]->Signature->HandledData.UsedExtraFiles;
      for(unsigned int j=0 ; j<UsedExtraFiles.size() ; j++)
      {
        Gtk::TreeModel::Row RowExtraFileUsed = *(MainTreeModel->append(RowFctExtraFilesUsedTitle.children()));
        RowExtraFileUsed[m_Columns.m_Id] = UsedExtraFiles[j];
        RowExtraFileUsed[m_Columns.m_Type] = ModelColumns::ExtraFiles;
        RowExtraFileUsed[m_Columns.m_IsTitle] = false;
      }

      // Simulation functions Spatial Units Graph
      Gtk::TreeModel::Row RowFctSpatialUnitsGraphTitle = *(MainTreeModel->append(RowFct.children()));

      setRowTitle(&RowFctSpatialUnitsGraphTitle,"Spatial Units",ModelColumns::SpatialUnits, false);

      // -- Updated Units Graph
      Gtk::TreeModel::Row RowFctUnitsGraphTitle = *(MainTreeModel->append(RowFctSpatialUnitsGraphTitle.children()));

      setRowTitle(&RowFctUnitsGraphTitle,"Updated Units Graph",ModelColumns::SpatialUnits);

      Gtk::TreeModel::Row RowFctUnitsGraph = *(MainTreeModel->append(RowFctUnitsGraphTitle.children()));
      RowFctUnitsGraph[m_Columns.m_Description] = PlugContainers[i]->Signature->HandledUnitsGraph.UpdatedUnitsGraph;
      RowFctUnitsGraph[m_Columns.m_Type] = ModelColumns::SpatialUnits;
      RowFctUnitsGraph[m_Columns.m_IsTitle] = false;


      // -- Updated Units Classes
      Gtk::TreeModel::Row RowFctUnitsClassesTitle = *(MainTreeModel->append(RowFctSpatialUnitsGraphTitle.children()));

      setRowTitle(&RowFctUnitsClassesTitle,"Updated Units Classes",ModelColumns::SpatialUnits);

      std::vector<openfluid::base::SignatureHandledUnitsClassItem> UpdatedUnitsClasses = PlugContainers[i]->Signature->HandledUnitsGraph.UpdatedUnitsClass;
      for(unsigned int j=0 ; j<UpdatedUnitsClasses.size() ; j++)
      {
        Gtk::TreeModel::Row RowFctUnitsClass = *(MainTreeModel->append(RowFctUnitsClassesTitle.children()));
        RowFctUnitsClass[m_Columns.m_HandleDataUnitClass] = UpdatedUnitsClasses[j].UnitsClass;
        RowFctUnitsClass[m_Columns.m_Description] = UpdatedUnitsClasses[j].Description;
        RowFctUnitsClass[m_Columns.m_Type] = ModelColumns::SpatialUnits;
        RowFctUnitsClass[m_Columns.m_IsTitle] = false;
      }
    }
  }


  // 1. Generators rows

  Gtk::TreeModel::Row RowGeneratorTitle = *(MainTreeModel->append());

  setRowTitle(&RowGeneratorTitle,"Generators",ModelColumns::Generators, false);

  for(unsigned int i=0; i<GeneratorContainers.size() ; i++)
  {
    if (GeneratorContainers[i]->SDKCompatible && GeneratorContainers[i]->Signature!=NULL)
    {
      Gtk::TreeModel::Row RowGenerator = *(MainTreeModel->append(RowGeneratorTitle.children()));

      RowGenerator[m_Columns.m_Type] = ModelColumns::Generators;
      RowGenerator[m_Columns.m_IsTitle] = false;

      RowGenerator[m_Columns.m_Id] = GeneratorContainers[i]->Signature->ID;
      RowGenerator[m_Columns.m_FunctionName] = GeneratorContainers[i]->Signature->Name;
      RowGenerator[m_Columns.m_Description] = GeneratorContainers[i]->Signature->Description;

      createHandleDataTreeRows(MainTreeModel,&RowGenerator,"Parameters",ModelColumns::Parameters,GeneratorContainers[i]->Signature->HandledData.FunctionParams,false);
    }
  }

  return MainTreeModel;
}


// =====================================================================
// =====================================================================


void ModelModule::createHandleDataTreeRows(Glib::RefPtr<Gtk::TreeStore> Model,Gtk::TreeModel::Row * PrevRow, Glib::ustring Title, ModelColumns::RowType Type, std::vector<openfluid::base::SignatureHandledDataItem> Vars, bool ShowTitle)
{
  Gtk::TreeModel::Row RowTitle = *(Model->append((*PrevRow).children()));

  setRowTitle(&RowTitle,Title,Type,ShowTitle);

  for(unsigned int i=0 ; i<Vars.size() ; i++)
  {
    Gtk::TreeModel::Row Row = *(Model->append(RowTitle.children()));

    Row[m_Columns.m_Type] = Type;
    Row[m_Columns.m_IsTitle] = false;

    Row[m_Columns.m_Id] = Vars[i].DataName;
    Row[m_Columns.m_HandleDataUnitClass] = Vars[i].UnitClass;
    Row[m_Columns.m_HandleDataUnit] = Vars[i].DataUnit;
    Row[m_Columns.m_Description] = Vars[i].Description;
  }

}


// =====================================================================
// =====================================================================


void ModelModule::setRowTitle(Gtk::TreeModel::Row * Row, Glib::ustring Title, ModelColumns::RowType Type, bool /*ShowTitle*/)
{
  (*Row)[m_Columns.m_Id] = Title;
  (*Row)[m_Columns.m_Type] = Type;
  (*Row)[m_Columns.m_IsTitle] = true;//!ShowTitle;
}


// =====================================================================
// =====================================================================


openfluid::base::ModelDescriptor * ModelModule::getModelDescriptor()
{
  return mp_ModelUsedFct->getModelDescriptor();
}


// =====================================================================
// =====================================================================


//void ModelModule::actionCheckModel()
bool ModelModule::checkModule(openfluid::machine::ModelInstance * ModelInstance)
{
  /* TODO: get WarningMsgs */

  std::cout << "* Initializing parameters... " << std::endl;

  mp_StatusParamsValues->clearErrorValues();

  bool IsOk = true;

  const std::list<openfluid::machine::ModelItemInstance *> ItemInstances = ModelInstance->getItems();

  std::list<openfluid::machine::ModelItemInstance *>::const_iterator it;

  for(it=ItemInstances.begin() ; it!=ItemInstances.end() ; ++it)
  {
    openfluid::machine::ModelItemInstance * ItemInstance = *it;

    try
    {
      ItemInstance->Function->initParams(ItemInstance->Params);
    }
    catch(openfluid::base::OFException& E)
    {
      std::cerr << E.what() << std::endl;
      mp_StatusParamsValues->appendErrorValue(E.what());
      IsOk = false;
      continue;
    }

  }

  Glib::ustring Status = IsOk ? "(OK)" : "(with errors)";

  std::cout << "...Parameters initialized " << Status << std::endl;

  return IsOk;
}

