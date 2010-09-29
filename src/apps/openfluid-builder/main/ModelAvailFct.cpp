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
  \file ModelAvailFct.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <glibmm/i18n.h>
#include <boost/filesystem.hpp>

#include <openfluid/machine.hpp>
#include <openfluid/base.hpp>

#include "builderconfig.hpp"

#include "ModelAvailFct.hpp"


// =====================================================================
// =====================================================================


ModelAvailFct::ModelAvailFct(Glib::RefPtr<Gtk::Builder> GladeBuilder)
{
  GladeBuilder->get_widget("TreeViewAvailFct",mp_TreeViewAvailFct);
  GladeBuilder->get_widget("TreeViewFctParameters",mp_TreeViewFctParameters);
  GladeBuilder->get_widget("TreeViewFctInputData",mp_TreeViewFctInputData);
  GladeBuilder->get_widget("TreeViewFctVars",mp_TreeViewFctVars);
  GladeBuilder->get_widget("TreeViewFctEvents",mp_TreeViewFctEvents);
  GladeBuilder->get_widget("TreeViewFctExtraFiles",mp_TreeViewFctExtraFiles);
  GladeBuilder->get_widget("TreeViewFctUnitsGraph",mp_TreeViewFctUnitsGraph);

  GladeBuilder->get_widget("TableSignatureFunction",mp_TableSignatureFunction);
  GladeBuilder->get_widget("TableSignatureGenerator",mp_TableSignatureGenerator);

  GladeBuilder->get_widget("LabelAvailFctId",mp_LabelAvailFctId);
  GladeBuilder->get_widget("LabelAvailFctName",mp_LabelAvailFctName);
  GladeBuilder->get_widget("LabelAvailFctPath",mp_LabelAvailFctPath);
  GladeBuilder->get_widget("LabelAvailFctDescription",mp_LabelAvailFctDescription);
  GladeBuilder->get_widget("LabelAvailFctVersion",mp_LabelAvailFctVersion);
  GladeBuilder->get_widget("LabelAvailFctStatus",mp_LabelAvailFctStatus);
  GladeBuilder->get_widget("LabelAvailFctDomain",mp_LabelAvailFctDomain);
  GladeBuilder->get_widget("LabelAvailFctProcess",mp_LabelAvailFctProcess);
  GladeBuilder->get_widget("LabelAvailFctMethod",mp_LabelAvailFctMethod);
  GladeBuilder->get_widget("LabelAvailFctAuthorName",mp_LabelAvailFctAuthorName);
  GladeBuilder->get_widget("LabelAvailFctAuthorEmail",mp_LabelAvailFctAuthorEmail);

  GladeBuilder->get_widget("LabelGeneratorId",mp_LabelGeneratorId);
  GladeBuilder->get_widget("LabelGeneratorName",mp_LabelGeneratorName);
  GladeBuilder->get_widget("LabelGeneratorDescription",mp_LabelGeneratorDescription);

  GladeBuilder->get_widget("NotebookAvailFct",mp_NotebookAvailFct);

  // Glade's bug ? Doesn't display last menu label text
  Gtk::Notebook * NB = dynamic_cast<Gtk::Notebook*>(mp_TreeViewFctUnitsGraph->get_parent()->get_parent());
  int PageNum = NB->page_num(*(mp_TreeViewFctUnitsGraph->get_parent()));
  NB->set_menu_label_text(*(NB->get_nth_page(PageNum)),NB->get_tab_label_text(*(NB->get_nth_page(PageNum))));

  mp_TableSignatureFunction->set_visible(true);
  mp_TableSignatureGenerator->set_visible(false);

  createMainTreeModel();

  createAvailableFunctionsTreeModel();

  createAvailableFunctionsTreeView();

  createSelectedFunctionTreeViews();

}


// =====================================================================
// =====================================================================


ModelAvailFct::~ModelAvailFct()
{

}


// =====================================================================
// =====================================================================


void ModelAvailFct::createMainTreeModel()
{

  Glib::RefPtr<Gdk::Pixbuf> IconRed = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,"red.svg"),8,-1,true);
  Glib::RefPtr<Gdk::Pixbuf> IconOrange = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,"orange.svg"),8,-1,true);
  Glib::RefPtr<Gdk::Pixbuf> IconGreen = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,"green.svg"),8,-1,true);

  mp_TreeModelAvailFct = Gtk::TreeStore::create(m_Columns);


  // 0. Simulation functions rows

  Gtk::TreeModel::Row RowFctTitle = *(mp_TreeModelAvailFct->append());

  setRowTitle(&RowFctTitle,"Simulation Functions",ModelColumns::SimulationFunctions);

  openfluid::machine::ArrayOfModelItemInstance PlugContainers = openfluid::machine::PluginManager::getInstance()->getAvailableFunctions();
  for (unsigned int i=0;i<PlugContainers.size();i++)
  {
    if (PlugContainers[i]->SDKCompatible && PlugContainers[i]->Signature!=NULL)
    {
      Gtk::TreeModel::Row RowFct = *(mp_TreeModelAvailFct->append(RowFctTitle.children()));

      RowFct[m_Columns.m_ModelItemInstance] = PlugContainers[i];

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
      createHandleDataTreeRows(&RowFct,"Parameters",ModelColumns::Parameters,PlugContainers[i]->Signature->HandledData.FunctionParams,false);

      // Simulation functions Input Data
      Gtk::TreeModel::Row RowFctInputDataTitle = *(mp_TreeModelAvailFct->append(RowFct.children()));

      setRowTitle(&RowFctInputDataTitle,"Input Data",ModelColumns::InputData,false);

      createHandleDataTreeRows(&RowFctInputDataTitle,"Required",ModelColumns::InputData,PlugContainers[i]->Signature->HandledData.RequiredInput);
      createHandleDataTreeRows(&RowFctInputDataTitle,"Used",ModelColumns::InputData,PlugContainers[i]->Signature->HandledData.UsedInput);


      // Simulation functions Variables
      Gtk::TreeModel::Row RowFctVariablesTitle = *(mp_TreeModelAvailFct->append(RowFct.children()));

      setRowTitle(&RowFctVariablesTitle,"Variables",ModelColumns::Variables,false);

      createHandleDataTreeRows(&RowFctVariablesTitle,"Produced",ModelColumns::Variables,PlugContainers[i]->Signature->HandledData.ProducedVars);
      createHandleDataTreeRows(&RowFctVariablesTitle,"Updated",ModelColumns::Variables,PlugContainers[i]->Signature->HandledData.UpdatedVars);
      createHandleDataTreeRows(&RowFctVariablesTitle,"Required",ModelColumns::Variables,PlugContainers[i]->Signature->HandledData.RequiredVars);
      createHandleDataTreeRows(&RowFctVariablesTitle,"Used",ModelColumns::Variables,PlugContainers[i]->Signature->HandledData.UsedVars);
      createHandleDataTreeRows(&RowFctVariablesTitle,"Required t-1",ModelColumns::Variables,PlugContainers[i]->Signature->HandledData.RequiredPrevVars);
      createHandleDataTreeRows(&RowFctVariablesTitle,"Used t-1",ModelColumns::Variables,PlugContainers[i]->Signature->HandledData.UsedPrevVars);

      // Simulation functions Events
      Gtk::TreeModel::Row RowFctEventsTitle = *(mp_TreeModelAvailFct->append(RowFct.children()));

      setRowTitle(&RowFctEventsTitle,"Events",ModelColumns::Events, false);

      std::vector<std::string> TheEvents = PlugContainers[i]->Signature->HandledData.UsedEventsOnUnits;
      for(unsigned int j=0 ; j<TheEvents.size() ; j++)
      {
        Gtk::TreeModel::Row RowEvents = *(mp_TreeModelAvailFct->append(RowFctEventsTitle.children()));
        RowEvents[m_Columns.m_HandleDataUnitClass] = TheEvents[j];
        RowEvents[m_Columns.m_Type] = ModelColumns::Events;
        RowEvents[m_Columns.m_IsTitle] = false;
      }

      // Simulation functions Extra Files
      Gtk::TreeModel::Row RowFctExtraFilesTitle = *(mp_TreeModelAvailFct->append(RowFct.children()));

      setRowTitle(&RowFctExtraFilesTitle,"Extra Files",ModelColumns::ExtraFiles, false);

      // -- Required Extra Files
      Gtk::TreeModel::Row RowFctExtraFilesRequiredTitle = *(mp_TreeModelAvailFct->append(RowFctExtraFilesTitle.children()));

      setRowTitle(&RowFctExtraFilesRequiredTitle,"Required",ModelColumns::ExtraFiles);

      std::vector<std::string> RequiredExtraFiles = PlugContainers[i]->Signature->HandledData.RequiredExtraFiles;
      for(unsigned int j=0 ; j<RequiredExtraFiles.size() ; j++)
      {
        Gtk::TreeModel::Row RowExtraFileRequired = *(mp_TreeModelAvailFct->append(RowFctExtraFilesRequiredTitle.children()));
        RowExtraFileRequired[m_Columns.m_Id] = RequiredExtraFiles[j];
        RowExtraFileRequired[m_Columns.m_Type] = ModelColumns::ExtraFiles;
        RowExtraFileRequired[m_Columns.m_IsTitle] = false;
      }

      // -- Used Extra Files
      Gtk::TreeModel::Row RowFctExtraFilesUsedTitle = *(mp_TreeModelAvailFct->append(RowFctExtraFilesTitle.children()));

      setRowTitle(&RowFctExtraFilesUsedTitle,"Used",ModelColumns::ExtraFiles);

      std::vector<std::string> UsedExtraFiles = PlugContainers[i]->Signature->HandledData.UsedExtraFiles;
      for(unsigned int j=0 ; j<UsedExtraFiles.size() ; j++)
      {
        Gtk::TreeModel::Row RowExtraFileUsed = *(mp_TreeModelAvailFct->append(RowFctExtraFilesUsedTitle.children()));
        RowExtraFileUsed[m_Columns.m_Id] = UsedExtraFiles[j];
        RowExtraFileUsed[m_Columns.m_Type] = ModelColumns::ExtraFiles;
        RowExtraFileUsed[m_Columns.m_IsTitle] = false;
      }

      // Simulation functions Spatial Units Graph
      Gtk::TreeModel::Row RowFctSpatialUnitsGraphTitle = *(mp_TreeModelAvailFct->append(RowFct.children()));

      setRowTitle(&RowFctSpatialUnitsGraphTitle,"Spatial Units",ModelColumns::SpatialUnits, false);

      // -- Updated Units Graph
      Gtk::TreeModel::Row RowFctUnitsGraphTitle = *(mp_TreeModelAvailFct->append(RowFctSpatialUnitsGraphTitle.children()));

      setRowTitle(&RowFctUnitsGraphTitle,"Updated Units Graph",ModelColumns::SpatialUnits);

      Gtk::TreeModel::Row RowFctUnitsGraph = *(mp_TreeModelAvailFct->append(RowFctUnitsGraphTitle.children()));
      RowFctUnitsGraph[m_Columns.m_Description] = PlugContainers[i]->Signature->HandledUnitsGraph.UpdatedUnitsGraph;
      RowFctUnitsGraph[m_Columns.m_Type] = ModelColumns::SpatialUnits;
      RowFctUnitsGraph[m_Columns.m_IsTitle] = false;


      // -- Updated Units Classes
      Gtk::TreeModel::Row RowFctUnitsClassesTitle = *(mp_TreeModelAvailFct->append(RowFctSpatialUnitsGraphTitle.children()));

      setRowTitle(&RowFctUnitsClassesTitle,"Updated Units Classes",ModelColumns::SpatialUnits);

      std::vector<openfluid::base::SignatureHandledUnitsClassItem> UpdatedUnitsClasses = PlugContainers[i]->Signature->HandledUnitsGraph.UpdatedUnitsClass;
      for(unsigned int j=0 ; j<UpdatedUnitsClasses.size() ; j++)
      {
        Gtk::TreeModel::Row RowFctUnitsClass = *(mp_TreeModelAvailFct->append(RowFctUnitsClassesTitle.children()));
        RowFctUnitsClass[m_Columns.m_HandleDataUnitClass] = UpdatedUnitsClasses[j].UnitsClass;
        RowFctUnitsClass[m_Columns.m_Description] = UpdatedUnitsClasses[j].Description;
        RowFctUnitsClass[m_Columns.m_Type] = ModelColumns::SpatialUnits;
        RowFctUnitsClass[m_Columns.m_IsTitle] = false;
      }
    }
  }

  // 1. Generators rows
  Gtk::TreeModel::Row RowGeneratorTitle = *(mp_TreeModelAvailFct->append());

  setRowTitle(&RowGeneratorTitle,"Generators",ModelColumns::Generators, false);

  for(int i=0 ; i<ModelColumns::MaxGeneratorType ; i++)
  {
    Gtk::TreeModel::Row RowGenerator = *(mp_TreeModelAvailFct->append(RowGeneratorTitle.children()));

    RowGenerator[m_Columns.m_Type] = ModelColumns::Generators;
    RowGenerator[m_Columns.m_IsTitle] = false;

    // Generator common parameters

    Gtk::TreeModel::Row RowGeneratorParametersTitle = *(mp_TreeModelAvailFct->append(RowGenerator.children()));

    setRowTitle(&RowGeneratorParametersTitle,"Parameters",ModelColumns::Parameters,false);


    Gtk::TreeModel::Row RowGeneratorParameters = *(mp_TreeModelAvailFct->append(RowGeneratorParametersTitle.children()));

    RowGeneratorParameters[m_Columns.m_Id] = "Varname";
    RowGeneratorParameters[m_Columns.m_Description] = "Name of the produced variable";
    RowGeneratorParameters[m_Columns.m_Type] = ModelColumns::Parameters;
    RowGeneratorParameters[m_Columns.m_HandleDataUnit] = "-";
    RowGeneratorParameters[m_Columns.m_IsTitle] = false;


    RowGeneratorParameters = *(mp_TreeModelAvailFct->append(RowGeneratorParametersTitle.children()));

    RowGeneratorParameters[m_Columns.m_Id] = "Unitclass";
    RowGeneratorParameters[m_Columns.m_Description] = "Unit class of the produced variable";
    RowGeneratorParameters[m_Columns.m_Type] = ModelColumns::Parameters;
    RowGeneratorParameters[m_Columns.m_HandleDataUnit] = "-";
    RowGeneratorParameters[m_Columns.m_IsTitle] = false;


    RowGeneratorParameters = *(mp_TreeModelAvailFct->append(RowGeneratorParametersTitle.children()));

    RowGeneratorParameters[m_Columns.m_Id] = "Varsize";
    RowGeneratorParameters[m_Columns.m_Description] = "Optionnal : to produce a vector variable instead of a scalar variable";
    RowGeneratorParameters[m_Columns.m_Type] = ModelColumns::Parameters;
    RowGeneratorParameters[m_Columns.m_HandleDataUnit] = "-";
    RowGeneratorParameters[m_Columns.m_IsTitle] = false;

    // Generator specific parameters

    switch(static_cast<ModelColumns::GeneratorType>(i))
    {
      case ModelColumns::Fixed :

        RowGenerator[m_Columns.m_Id] = "Fixed";
        RowGenerator[m_Columns.m_FunctionName] = "Fixed Generator";
        RowGenerator[m_Columns.m_Description] = "Generates a constant value";

        RowGeneratorParameters = *(mp_TreeModelAvailFct->append(RowGeneratorParametersTitle.children()));

        RowGeneratorParameters[m_Columns.m_Id] = "fixedvalue";
        RowGeneratorParameters[m_Columns.m_Description] = "Value to produce";
        RowGeneratorParameters[m_Columns.m_Type] = ModelColumns::Parameters;
        RowGeneratorParameters[m_Columns.m_HandleDataUnit] = "-";
        RowGeneratorParameters[m_Columns.m_IsTitle] = false;

        break;

      case ModelColumns::Random :

        RowGenerator[m_Columns.m_Id] = "Random";
        RowGenerator[m_Columns.m_FunctionName] = "Random Generator";
        RowGenerator[m_Columns.m_Description] = "Generates a random value in a range";

        RowGeneratorParameters = *(mp_TreeModelAvailFct->append(RowGeneratorParametersTitle.children()));

        RowGeneratorParameters[m_Columns.m_Id] = "min";
        RowGeneratorParameters[m_Columns.m_Description] = "Lower bound of the random range for the value to produce";
        RowGeneratorParameters[m_Columns.m_Type] = ModelColumns::Parameters;
        RowGeneratorParameters[m_Columns.m_HandleDataUnit] = "-";
        RowGeneratorParameters[m_Columns.m_IsTitle] = false;

        RowGeneratorParameters = *(mp_TreeModelAvailFct->append(RowGeneratorParametersTitle.children()));

        RowGeneratorParameters[m_Columns.m_Id] = "max";
        RowGeneratorParameters[m_Columns.m_Description] = "Upper bound of the random range for the value to produce";
        RowGeneratorParameters[m_Columns.m_Type] = ModelColumns::Parameters;
        RowGeneratorParameters[m_Columns.m_HandleDataUnit] = "-";
        RowGeneratorParameters[m_Columns.m_IsTitle] = false;

        break;

      case ModelColumns::Interp :

        RowGenerator[m_Columns.m_Id] = "Interp";
        RowGenerator[m_Columns.m_FunctionName] = "Interpolation Generator";
        RowGenerator[m_Columns.m_Description] = "Generates an interpolated value from given data series";

        RowGeneratorParameters = *(mp_TreeModelAvailFct->append(RowGeneratorParametersTitle.children()));

        RowGeneratorParameters[m_Columns.m_Id] = "sources";
        RowGeneratorParameters[m_Columns.m_Description] = "Data sources filename for the value to produce";
        RowGeneratorParameters[m_Columns.m_Type] = ModelColumns::Parameters;
        RowGeneratorParameters[m_Columns.m_HandleDataUnit] = "-";
        RowGeneratorParameters[m_Columns.m_IsTitle] = false;

        RowGeneratorParameters = *(mp_TreeModelAvailFct->append(RowGeneratorParametersTitle.children()));

        RowGeneratorParameters[m_Columns.m_Id] = "distribution";
        RowGeneratorParameters[m_Columns.m_Description] = "Distribution filename for the value to produce";
        RowGeneratorParameters[m_Columns.m_Type] = ModelColumns::Parameters;
        RowGeneratorParameters[m_Columns.m_HandleDataUnit] = "-";
        RowGeneratorParameters[m_Columns.m_IsTitle] = false;

        break;

      default:;
    }
  }

}


// =====================================================================
// =====================================================================


void ModelAvailFct::createAvailableFunctionsTreeModel()
{
  // Create sub Tree model for Available functions list
  // !! to have a filtered AND sortable model, have to create a TreeModelSort from a TreeModelFilter

  // Create filtered model
  Glib::RefPtr<Gtk::TreeModelFilter> TreeJustFilterAvailFct = Gtk::TreeModelFilter::create(mp_TreeModelAvailFct);

  // Define types to set visible

  std::vector<ModelColumns::RowType> TypesToSetVisible;

  TypesToSetVisible.push_back(ModelColumns::SimulationFunctions);
  TypesToSetVisible.push_back(ModelColumns::Generators);

  TreeJustFilterAvailFct->set_visible_func(sigc::bind<std::vector<ModelColumns::RowType> >(sigc::mem_fun(*this,&ModelAvailFct::isRowVisible),TypesToSetVisible));

  // Create sortable model
  Glib::RefPtr<Gtk::TreeModelSort> TreeFilterAvailFct = Gtk::TreeModelSort::create(TreeJustFilterAvailFct);

  // Attach the tree model to the tree view
  mp_TreeViewAvailFct->set_model(TreeFilterAvailFct);

}


// =====================================================================
// =====================================================================


void ModelAvailFct::createAvailableFunctionsTreeView()
{
  // Create column with colored bullet
   Gtk::TreeView::Column * ColumnId = Gtk::manage(new Gtk::TreeView::Column("Id"));

   ColumnId->pack_start(m_Columns.m_FunctionStatusIcon,false);
   ColumnId->pack_start(m_Columns.m_Id);

   mp_TreeViewAvailFct->append_column(*ColumnId);

   mp_TreeViewAvailFct->append_column("Domain",m_Columns.m_FunctionDomain);


   mp_TreeViewAvailFct->get_column(0)->set_sort_column(m_Columns.m_Id);
   mp_TreeViewAvailFct->get_column(1)->set_sort_column(m_Columns.m_FunctionDomain);

   setTreeViewProperties(mp_TreeViewAvailFct);

   mp_TreeViewAvailFct->set_search_column(m_Columns.m_Id);

   mp_TreeViewAvailFct->expand_all();


   Glib::RefPtr<Gtk::TreeSelection> TreeSelection = mp_TreeViewAvailFct->get_selection();

   TreeSelection->signal_changed().connect(sigc::mem_fun(*this, &ModelAvailFct::onAvalaibleFunctionSelected));

   // Select first available function
   Gtk::TreeModel::iterator sub = TreeSelection->get_model()->children()[0]->children()[0];

   if(sub)
     TreeSelection->select(sub);

}


// =====================================================================
// =====================================================================


void ModelAvailFct::createSelectedFunctionTreeViews()
{
  // --Parameters
  mp_TreeViewFctParameters->append_column("Name",m_Columns.m_Id);
  mp_TreeViewFctParameters->append_column("Unit",m_Columns.m_HandleDataUnit);
  mp_TreeViewFctParameters->append_column("Description",m_Columns.m_Description);

  mp_TreeViewFctParameters->get_column(0)->set_sort_column(m_Columns.m_Id);

  setTreeViewProperties(mp_TreeViewFctParameters);


  // --Input Data
  mp_TreeViewFctInputData->append_column("Name",m_Columns.m_Id);
  mp_TreeViewFctInputData->append_column("Class",m_Columns.m_HandleDataUnitClass);

  mp_TreeViewFctInputData->get_column(0)->set_sort_column(m_Columns.m_Id);
  mp_TreeViewFctInputData->get_column(1)->set_sort_column(m_Columns.m_HandleDataUnitClass);

  setTreeViewProperties(mp_TreeViewFctInputData);


  // --Variables
  mp_TreeViewFctVars->append_column("Name",m_Columns.m_Id);
  mp_TreeViewFctVars->append_column("Class",m_Columns.m_HandleDataUnitClass);
  mp_TreeViewFctVars->append_column("Unit",m_Columns.m_HandleDataUnit);
  mp_TreeViewFctVars->append_column("Description",m_Columns.m_Description);

  mp_TreeViewFctVars->get_column(0)->set_sort_column(m_Columns.m_Id);
  mp_TreeViewFctVars->get_column(1)->set_sort_column(m_Columns.m_HandleDataUnitClass);

  setTreeViewProperties(mp_TreeViewFctVars);


  // --Events
  mp_TreeViewFctEvents->append_column("",m_Columns.m_Id);
  mp_TreeViewFctEvents->append_column("Class",m_Columns.m_HandleDataUnitClass);

  mp_TreeViewFctEvents->get_column(1)->set_sort_column(m_Columns.m_HandleDataUnitClass);

  setTreeViewProperties(mp_TreeViewFctEvents);


  // --Extra Files
  mp_TreeViewFctExtraFiles->append_column("Name",m_Columns.m_Id);

  mp_TreeViewFctExtraFiles->get_column(0)->set_sort_column(m_Columns.m_Id);

  setTreeViewProperties(mp_TreeViewFctExtraFiles);


  // --UnitsGraph
  mp_TreeViewFctUnitsGraph->append_column("",m_Columns.m_Id);
  mp_TreeViewFctUnitsGraph->append_column("Class",m_Columns.m_HandleDataUnitClass);
  mp_TreeViewFctUnitsGraph->append_column("Description",m_Columns.m_Description);

  mp_TreeViewFctUnitsGraph->get_column(1)->set_sort_column(m_Columns.m_HandleDataUnitClass);

  setTreeViewProperties(mp_TreeViewFctUnitsGraph);

}


// =====================================================================
// =====================================================================


void ModelAvailFct::setTreeViewProperties(Gtk::TreeView * TreeView)
{
  for(unsigned int i=0 ; i<TreeView->get_columns().size() ; i++)
  {
    TreeView->get_column(i)->set_resizable();

    TreeView->get_column(i)->set_reorderable();

    TreeView->get_selection()->set_select_function(sigc::mem_fun(*this, &ModelAvailFct::isFunctionRowSelectable));
  }
}


// =====================================================================
// =====================================================================


void ModelAvailFct::createHandleDataTreeRows(Gtk::TreeModel::Row * PrevRow, Glib::ustring Title, ModelColumns::RowType Type, std::vector<openfluid::base::SignatureHandledDataItem> Vars, bool ShowTitle)
{

  Gtk::TreeModel::Row RowTitle = *(mp_TreeModelAvailFct->append((*PrevRow).children()));

  setRowTitle(&RowTitle,Title,Type,ShowTitle);

  for(unsigned int i=0 ; i<Vars.size() ; i++)
  {
    Gtk::TreeModel::Row Row = *(mp_TreeModelAvailFct->append(RowTitle.children()));

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


void ModelAvailFct::setRowTitle(Gtk::TreeModel::Row * Row, Glib::ustring Title, ModelColumns::RowType Type, bool /*ShowTitle*/)
{
  (*Row)[m_Columns.m_Id] = Title;
  (*Row)[m_Columns.m_Type] = Type;
  (*Row)[m_Columns.m_IsTitle] = true;//!ShowTitle;
}


// =====================================================================
// =====================================================================


bool ModelAvailFct::isRowVisible(const Gtk::TreeModel::const_iterator& Iter, std::vector<ModelColumns::RowType> Type)
{
  Gtk::TreeModel::Row row = *Iter;

  // true if Row is parameter Type
  return find(Type.begin(), Type.end(), row[m_Columns.m_Type]) != Type.end();
}


// =====================================================================
// =====================================================================


// Title columns are not selectable
bool ModelAvailFct::isFunctionRowSelectable(const Glib::RefPtr<Gtk::TreeModel>& model, const Gtk::TreeModel::Path& path, bool /*path_currently_selected*/)
{
  const Gtk::TreeModel::iterator it = model->get_iter(path);

  return (!it->get_value(m_Columns.m_IsTitle));
}


// =====================================================================
// =====================================================================


void ModelAvailFct::onAvalaibleFunctionSelected()
{
  Glib::RefPtr<Gtk::TreeModelSort> TreeModelSort = Glib::RefPtr<Gtk::TreeModelSort>::cast_dynamic(mp_TreeViewAvailFct->get_model());
  Glib::RefPtr<Gtk::TreeModelFilter> TreeModelFilter = Glib::RefPtr<Gtk::TreeModelFilter>::cast_dynamic(TreeModelSort->get_model());

  Gtk::TreeIter IterFilterSort = mp_TreeViewAvailFct->get_selection()->get_selected();
  Gtk::TreeIter IterFilter = TreeModelSort->convert_iter_to_child_iter(IterFilterSort);
  Gtk::TreeIter Iter = TreeModelFilter->convert_iter_to_child_iter(IterFilter);

  if(Iter)
  {
    // Update selected function Signature panel

    Gtk::TreeModel::Row row = *Iter;

    if(row[m_Columns.m_Type] == ModelColumns::SimulationFunctions)
    {
      if(mp_TableSignatureGenerator->is_visible())
      {
        Gtk::Container * ContainerTable = mp_TableSignatureGenerator->get_parent();
        mp_TableSignatureGenerator->hide_all();
        ContainerTable->remove(*mp_TableSignatureGenerator);
        ContainerTable->add(*mp_TableSignatureFunction);
        mp_TableSignatureFunction->show_all();
      }

      mp_LabelAvailFctId->set_text(row[m_Columns.m_Id]);
      mp_LabelAvailFctName->set_text(row[m_Columns.m_FunctionName]);
      mp_LabelAvailFctDescription->set_text(row[m_Columns.m_Description]);

      mp_LabelAvailFctPath->set_text(row[m_Columns.m_FunctionPath]);

      mp_LabelAvailFctVersion->set_text(row[m_Columns.m_FunctionVersion]);
      mp_LabelAvailFctStatus->set_text(row[m_Columns.m_FunctionStatusStr]);

      mp_LabelAvailFctDomain->set_text(row[m_Columns.m_FunctionDomain]);
      mp_LabelAvailFctProcess->set_text(row[m_Columns.m_FunctionProcess]);
      mp_LabelAvailFctMethod->set_text(row[m_Columns.m_FunctionMethod]);
      mp_LabelAvailFctAuthorName->set_text(row[m_Columns.m_FunctionAuthorName]);
      mp_LabelAvailFctAuthorEmail->set_text(row[m_Columns.m_FunctionAuthorEmail]);

      // Update selected function HandleData panels
      setHandleDataPanel(ModelColumns::Parameters,mp_TreeViewFctParameters,*(row.children()[0]));
      setHandleDataPanel(ModelColumns::InputData,mp_TreeViewFctInputData,*(row.children()[1]));
      setHandleDataPanel(ModelColumns::Variables,mp_TreeViewFctVars,*(row.children()[2]));
      setHandleDataPanel(ModelColumns::Events,mp_TreeViewFctEvents,*(row.children()[3]));
      setHandleDataPanel(ModelColumns::ExtraFiles,mp_TreeViewFctExtraFiles,*(row.children()[4]));
      setHandleDataPanel(ModelColumns::SpatialUnits,mp_TreeViewFctUnitsGraph,*(row.children()[5]));

      // Show all notebook tabs
      mp_NotebookAvailFct->show_all_children();
    }

    else if(row[m_Columns.m_Type] == ModelColumns::Generators)
    {
      if(mp_TableSignatureFunction->is_visible())
      {
        Gtk::Container * ContainerTable = mp_TableSignatureFunction->get_parent();
        mp_TableSignatureFunction->hide_all();
        ContainerTable->remove(*mp_TableSignatureFunction);
        ContainerTable->add(*mp_TableSignatureGenerator);
        mp_TableSignatureGenerator->show_all();
      }

      mp_LabelGeneratorId->set_text(row[m_Columns.m_Id]);
      mp_LabelGeneratorName->set_text(row[m_Columns.m_FunctionName]);
      mp_LabelGeneratorDescription->set_text(row[m_Columns.m_Description]);

      // Update selected generator Parameters panels
      setHandleDataPanel(ModelColumns::Parameters,mp_TreeViewFctParameters,*(row.children()[0]));

      // Hide unused notebook tabs, just keep first one (Signature) and second one (Parameters)
      for(unsigned int i=2 ; i<mp_NotebookAvailFct->pages().size() ; i++)
        mp_NotebookAvailFct->get_nth_page(i)->hide();
    }
  }
}


// =====================================================================
// =====================================================================


void ModelAvailFct::createSelectedFunctionTreeModel(Gtk::TreeView * TreeView, const Gtk::TreeRow & TreeRow, ModelColumns::RowType Type)
{
  Gtk::TreePath TreePath = Gtk::TreePath(TreeRow);

  // Create child model
  Glib::RefPtr<Gtk::TreeModelFilter> TreeJustFilter = Gtk::TreeModelFilter::create(mp_TreeModelAvailFct,TreePath);

  std::vector<ModelColumns::RowType> TypesToSetVisible(1,Type);

  TreeJustFilter->set_visible_func(sigc::bind<std::vector<ModelColumns::RowType> >(sigc::mem_fun(*this,&ModelAvailFct::isRowVisible),TypesToSetVisible));


  // Make child model model sortable
  Glib::RefPtr<Gtk::TreeModelSort> TreeFilterSort = Gtk::TreeModelSort::create(TreeJustFilter);


  TreeView->set_model(TreeFilterSort);

  // Has to be set after each model change
  TreeView->set_search_column(m_Columns.m_Id);

  TreeView->expand_all();

}


// =====================================================================
// =====================================================================


void ModelAvailFct::setHandleDataPanel(ModelColumns::RowType Type, Gtk::TreeView * TreeView, const Gtk::TreeRow & FunctionIter)
{
  bool Empty = true;

  if(!FunctionIter->children().empty())
  {
    for(unsigned int i=0 ; i<FunctionIter->children().size() ; i++)
    {
      if((*FunctionIter->children()[i])[m_Columns.m_IsTitle])
      {
        if(!FunctionIter->children()[i]->children().empty())
        {
          Empty = false;
          break;
        }

      }
      else
        Empty = false;
    }

  }

  if(Empty)
    toggleEmptyTreeView(TreeView,Type,true);
  else
  {
    toggleEmptyTreeView(TreeView,Type,false);
    createSelectedFunctionTreeModel(TreeView,FunctionIter,Type);
  }

}


// =====================================================================
// =====================================================================


void ModelAvailFct::toggleEmptyTreeView(Gtk::TreeView * TreeView, ModelColumns::RowType Type, bool TypeIsEmpty)
{
  Gtk::ScrolledWindow * ScrolledWindow = dynamic_cast<Gtk::ScrolledWindow *>(mp_NotebookAvailFct->get_nth_page(Type));

  Gtk::Container * Container = dynamic_cast<Gtk::Container*>(ScrolledWindow->get_child());

  // Replace TreeView by a label
  if(TypeIsEmpty && (G_OBJECT_TYPE(Container->gobj()) == G_OBJECT_TYPE(TreeView->gobj())))
  {
    ScrolledWindow->remove();

    Gtk::Label * LabelEmpty = Gtk::manage(new Gtk::Label());

    LabelEmpty->set_markup(_("<i>(None)</i>"));
    LabelEmpty->set_alignment(0,0);
    LabelEmpty->set_padding(20,20);

    // ! ScrolledWindow::add puts the child inside a Gtk::Viewport if it doesn't have native scrolling capability (case of Label)
    ScrolledWindow->add(*LabelEmpty);

    ScrolledWindow->show_all_children();
  }
  // Replace Label by the TreeView
  else if(!TypeIsEmpty && (G_OBJECT_TYPE(Container->gobj()) == GTK_TYPE_VIEWPORT))
  {
    ScrolledWindow->remove();

    ScrolledWindow->add(*TreeView);
  }
}


