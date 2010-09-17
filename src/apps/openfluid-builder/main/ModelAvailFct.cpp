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
  GladeBuilder->get_widget("NotebookAvailFct",mp_NotebookAvailFct);

  GladeBuilder->get_widget("TreeViewAvailFct",mp_TreeViewAvailFct);
  GladeBuilder->get_widget("TreeViewFctParameters",mp_TreeViewFctParameters);
  GladeBuilder->get_widget("TreeViewFctInputData",mp_TreeViewFctInputData);
  GladeBuilder->get_widget("TreeViewFctVars",mp_TreeViewFctVars);
  GladeBuilder->get_widget("TreeViewFctEvents",mp_TreeViewFctEvents);
  GladeBuilder->get_widget("TreeViewFctExtraFiles",mp_TreeViewFctExtraFiles);
  GladeBuilder->get_widget("TreeViewFctUnitsGraph",mp_TreeViewFctUnitsGraph);

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

  // Glade's bug ? Doesn't display last menu label text
  Gtk::Notebook * NB = dynamic_cast<Gtk::Notebook*>(mp_TreeViewFctUnitsGraph->get_parent()->get_parent());
  int PageNum = NB->page_num(*(mp_TreeViewFctUnitsGraph->get_parent()));
  NB->set_menu_label_text(*(NB->get_nth_page(PageNum)),NB->get_tab_label_text(*(NB->get_nth_page(PageNum))));

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

  setRowTitle(&RowFctTitle,"Simulation Functions",SimulationFunctions);

  openfluid::machine::ArrayOfModelItemInstance PlugContainers = openfluid::machine::PluginManager::getInstance()->getAvailableFunctions();
  for (unsigned int i=0;i<PlugContainers.size();i++)
  {
    if (PlugContainers[i]->SDKCompatible && PlugContainers[i]->Signature!=NULL)
    {
      Gtk::TreeModel::Row RowFct = *(mp_TreeModelAvailFct->append(RowFctTitle.children()));

      RowFct[m_Columns.m_Type] = SimulationFunctions;
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
      createHandleDataTreeRows(&RowFct,"Parameters",Parameters,PlugContainers[i]->Signature->HandledData.FunctionParams,false);

      // Simulation functions Input Data
      Gtk::TreeModel::Row RowFctInputDataTitle = *(mp_TreeModelAvailFct->append(RowFct.children()));

      setRowTitle(&RowFctInputDataTitle,"Input Data",InputData,false);

      createHandleDataTreeRows(&RowFctInputDataTitle,"Required",InputData,PlugContainers[i]->Signature->HandledData.RequiredInput);
      createHandleDataTreeRows(&RowFctInputDataTitle,"Used",InputData,PlugContainers[i]->Signature->HandledData.UsedInput);


      // Simulation functions Variables
      Gtk::TreeModel::Row RowFctVariablesTitle = *(mp_TreeModelAvailFct->append(RowFct.children()));

      setRowTitle(&RowFctVariablesTitle,"Variables",Variables,false);

      createHandleDataTreeRows(&RowFctVariablesTitle,"Produced",Variables,PlugContainers[i]->Signature->HandledData.ProducedVars);
      createHandleDataTreeRows(&RowFctVariablesTitle,"Updated",Variables,PlugContainers[i]->Signature->HandledData.UpdatedVars);
      createHandleDataTreeRows(&RowFctVariablesTitle,"Required",Variables,PlugContainers[i]->Signature->HandledData.RequiredVars);
      createHandleDataTreeRows(&RowFctVariablesTitle,"Used",Variables,PlugContainers[i]->Signature->HandledData.UsedVars);
      createHandleDataTreeRows(&RowFctVariablesTitle,"Required t-1",Variables,PlugContainers[i]->Signature->HandledData.RequiredPrevVars);
      createHandleDataTreeRows(&RowFctVariablesTitle,"Used t-1",Variables,PlugContainers[i]->Signature->HandledData.UsedPrevVars);

      // Simulation functions Events
      Gtk::TreeModel::Row RowFctEventsTitle = *(mp_TreeModelAvailFct->append(RowFct.children()));

      setRowTitle(&RowFctEventsTitle,"Events",Events, false);

      std::vector<std::string> TheEvents = PlugContainers[i]->Signature->HandledData.UsedEventsOnUnits;
      for(unsigned int j=0 ; j<TheEvents.size() ; j++)
      {
        Gtk::TreeModel::Row RowEvents = *(mp_TreeModelAvailFct->append(RowFctEventsTitle.children()));
        RowEvents[m_Columns.m_HandleDataUnitClass] = TheEvents[j];
        RowEvents[m_Columns.m_Type] = Events;
        RowEvents[m_Columns.m_IsTitle] = false;
      }

      // Simulation functions Extra Files
      Gtk::TreeModel::Row RowFctExtraFilesTitle = *(mp_TreeModelAvailFct->append(RowFct.children()));

      setRowTitle(&RowFctExtraFilesTitle,"Extra Files",ExtraFiles, false);

      // -- Required Extra Files
      Gtk::TreeModel::Row RowFctExtraFilesRequiredTitle = *(mp_TreeModelAvailFct->append(RowFctExtraFilesTitle.children()));

      setRowTitle(&RowFctExtraFilesRequiredTitle,"Required",ExtraFiles);

      std::vector<std::string> RequiredExtraFiles = PlugContainers[i]->Signature->HandledData.RequiredExtraFiles;
      for(unsigned int j=0 ; j<RequiredExtraFiles.size() ; j++)
      {
        Gtk::TreeModel::Row RowExtraFileRequired = *(mp_TreeModelAvailFct->append(RowFctExtraFilesRequiredTitle.children()));
        RowExtraFileRequired[m_Columns.m_Id] = RequiredExtraFiles[j];
        RowExtraFileRequired[m_Columns.m_Type] = ExtraFiles;
        RowExtraFileRequired[m_Columns.m_IsTitle] = false;
      }

      // -- Used Extra Files
      Gtk::TreeModel::Row RowFctExtraFilesUsedTitle = *(mp_TreeModelAvailFct->append(RowFctExtraFilesTitle.children()));

      setRowTitle(&RowFctExtraFilesUsedTitle,"Used",ExtraFiles);

      std::vector<std::string> UsedExtraFiles = PlugContainers[i]->Signature->HandledData.UsedExtraFiles;
      for(unsigned int j=0 ; j<UsedExtraFiles.size() ; j++)
      {
        Gtk::TreeModel::Row RowExtraFileUsed = *(mp_TreeModelAvailFct->append(RowFctExtraFilesUsedTitle.children()));
        RowExtraFileUsed[m_Columns.m_Id] = UsedExtraFiles[j];
        RowExtraFileUsed[m_Columns.m_Type] = ExtraFiles;
        RowExtraFileUsed[m_Columns.m_IsTitle] = false;
      }

      // Simulation functions Spatial Units Graph
      Gtk::TreeModel::Row RowFctSpatialUnitsGraphTitle = *(mp_TreeModelAvailFct->append(RowFct.children()));

      setRowTitle(&RowFctSpatialUnitsGraphTitle,"Spatial Units",SpatialUnits, false);

      // -- Updated Units Graph
      Gtk::TreeModel::Row RowFctUnitsGraphTitle = *(mp_TreeModelAvailFct->append(RowFctSpatialUnitsGraphTitle.children()));

      setRowTitle(&RowFctUnitsGraphTitle,"Updated Units Graph",SpatialUnits);

      Gtk::TreeModel::Row RowFctUnitsGraph = *(mp_TreeModelAvailFct->append(RowFctUnitsGraphTitle.children()));
      RowFctUnitsGraph[m_Columns.m_Description] = PlugContainers[i]->Signature->HandledUnitsGraph.UpdatedUnitsGraph;
      RowFctUnitsGraph[m_Columns.m_Type] = SpatialUnits;
      RowFctUnitsGraph[m_Columns.m_IsTitle] = false;


      // -- Updated Units Classes
      Gtk::TreeModel::Row RowFctUnitsClassesTitle = *(mp_TreeModelAvailFct->append(RowFctSpatialUnitsGraphTitle.children()));

      setRowTitle(&RowFctUnitsClassesTitle,"Updated Units Classes",SpatialUnits);

      std::vector<openfluid::base::SignatureHandledUnitsClassItem> UpdatedUnitsClasses = PlugContainers[i]->Signature->HandledUnitsGraph.UpdatedUnitsClass;
      for(unsigned int j=0 ; j<UpdatedUnitsClasses.size() ; j++)
      {
        Gtk::TreeModel::Row RowFctUnitsClass = *(mp_TreeModelAvailFct->append(RowFctUnitsClassesTitle.children()));
        RowFctUnitsClass[m_Columns.m_HandleDataUnitClass] = UpdatedUnitsClasses[j].UnitsClass;
        RowFctUnitsClass[m_Columns.m_Description] = UpdatedUnitsClasses[j].Description;
        RowFctUnitsClass[m_Columns.m_Type] = SpatialUnits;
        RowFctUnitsClass[m_Columns.m_IsTitle] = false;
      }
    }
  }

  // 1. Generators rows
  Gtk::TreeModel::Row RowGeneratorTitle = *(mp_TreeModelAvailFct->append());

  setRowTitle(&RowGeneratorTitle,"Generators",Generators, false);

  Glib::ustring TheGenerators[3] = { "Fixed Generator", "Random Generator", "Interp Generator" };
  for(int i=0 ; i<3 ; i++)
  {
    Gtk::TreeModel::Row RowGenerator = *(mp_TreeModelAvailFct->append(RowGeneratorTitle.children()));
    RowGenerator[m_Columns.m_Id] = TheGenerators[i];
    RowGenerator[m_Columns.m_Type] = Generators;
    RowGenerator[m_Columns.m_IsTitle] = false;
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

  std::vector<RowType> TypesToSetVisible;

  TypesToSetVisible.push_back(SimulationFunctions);
  TypesToSetVisible.push_back(Generators);

  TreeJustFilterAvailFct->set_visible_func(sigc::bind<std::vector<RowType> >(sigc::mem_fun(*this,&ModelAvailFct::isRowVisible),TypesToSetVisible));

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


void ModelAvailFct::createHandleDataTreeRows(Gtk::TreeModel::Row * PrevRow, Glib::ustring Title, RowType Type, std::vector<openfluid::base::SignatureHandledDataItem> Vars, bool ShowTitle)
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


void ModelAvailFct::setRowTitle(Gtk::TreeModel::Row * Row, Glib::ustring Title, RowType Type, bool /*ShowTitle*/)
{
  (*Row)[m_Columns.m_Id] = Title;
  (*Row)[m_Columns.m_Type] = Type;
  (*Row)[m_Columns.m_IsTitle] = true;//!ShowTitle;
}


// =====================================================================
// =====================================================================


bool ModelAvailFct::isRowVisible(const Gtk::TreeModel::const_iterator& Iter, std::vector<RowType> Type)
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
  Gtk::TreeModel::iterator iter = mp_TreeViewAvailFct->get_selection()->get_selected();

  if(iter)
  {
    // Update selected function Signature panel

    Gtk::TreeModel::Row row = *iter;

    mp_LabelAvailFctId->set_text(row[m_Columns.m_Id]);
    mp_LabelAvailFctName->set_text(row[m_Columns.m_FunctionName]);

    if(row[m_Columns.m_Type] == SimulationFunctions)
    {
      mp_LabelAvailFctPath->set_text(row[m_Columns.m_FunctionPath]);
      mp_LabelAvailFctDescription->set_text(row[m_Columns.m_Description]);

      mp_LabelAvailFctVersion->set_text(row[m_Columns.m_FunctionVersion]);
      mp_LabelAvailFctStatus->set_text(row[m_Columns.m_FunctionStatusStr]);

      mp_LabelAvailFctDomain->set_text(row[m_Columns.m_FunctionDomain]);
      mp_LabelAvailFctProcess->set_text(row[m_Columns.m_FunctionProcess]);
      mp_LabelAvailFctMethod->set_text(row[m_Columns.m_FunctionMethod]);
      mp_LabelAvailFctAuthorName->set_text(row[m_Columns.m_FunctionAuthorName]);
      mp_LabelAvailFctAuthorEmail->set_text(row[m_Columns.m_FunctionAuthorEmail]);
    }


    // Update selected function HandleData panels

    Gtk::TreePath FunctionPath(iter);

    FunctionPath.down();

    setHandleDataPanel(Parameters,mp_TreeViewFctParameters,1,FunctionPath);
    setHandleDataPanel(InputData,mp_TreeViewFctInputData,2,FunctionPath);
    setHandleDataPanel(Variables,mp_TreeViewFctVars,2,FunctionPath);
    setHandleDataPanel(Events,mp_TreeViewFctEvents,1,FunctionPath);
    setHandleDataPanel(ExtraFiles,mp_TreeViewFctExtraFiles,2,FunctionPath);
    setHandleDataPanel(SpatialUnits,mp_TreeViewFctUnitsGraph,2,FunctionPath);
  }

}


// =====================================================================
// =====================================================================


void ModelAvailFct::createSelectedFunctionTreeModel(Gtk::TreeView * TreeView, Gtk::TreePath * Path, RowType Type)
{
  // Create child model
  Glib::RefPtr<Gtk::TreeModelFilter> TreeJustFilter = Gtk::TreeModelFilter::create(mp_TreeModelAvailFct,*Path);

  std::vector<RowType> TypesToSetVisible(1,Type);

  TreeJustFilter->set_visible_func(sigc::bind<std::vector<RowType> >(sigc::mem_fun(*this,&ModelAvailFct::isRowVisible),TypesToSetVisible));


  // Make child model model sortable
  Glib::RefPtr<Gtk::TreeModelSort> TreeFilterSort = Gtk::TreeModelSort::create(TreeJustFilter);


  TreeView->set_model(TreeFilterSort);

  // Have to be set after each model change
  TreeView->set_search_column(m_Columns.m_Id);

  TreeView->expand_all();


  Path->next();

}


// =====================================================================
// =====================================================================


void ModelAvailFct::setHandleDataPanel(RowType Type, Gtk::TreeView * TreeView, int LeafValuesDepth, Gtk::TreePath & FunctionPath)
{
  bool Empty = true;

  if(LeafValuesDepth == 1)
    Empty = mp_TreeModelAvailFct->get_iter(FunctionPath)->children().empty();

  else if(LeafValuesDepth == 2)
  {
    for(unsigned int i=0 ; i< mp_TreeModelAvailFct->get_iter(FunctionPath)->children().size() ; i++)
    {
      if(!mp_TreeModelAvailFct->get_iter(FunctionPath)->children()[i]->children().empty())
      {
        Empty = false;
        break;
      }
    }
  }
  else
    std::cerr << "Error ModelAvailFct::setHandleDataPanel : LeafValuesDeep can only be 1 or 2." << std::endl;


  if(Empty)
  {
    toggleEmptyTreeView(TreeView,Type,true);
    FunctionPath.next();
  }
  else
  {
    toggleEmptyTreeView(TreeView,Type,false);
    createSelectedFunctionTreeModel(TreeView,&FunctionPath,Type);
  }

}


// =====================================================================
// =====================================================================


void ModelAvailFct::toggleEmptyTreeView(Gtk::TreeView * TreeView, RowType Type, bool TypeIsEmpty)
{
  Gtk::ScrolledWindow * ScrolledWindow = dynamic_cast<Gtk::ScrolledWindow *>(mp_NotebookAvailFct->get_nth_page(Type));

  Gtk::Container * Container = dynamic_cast<Gtk::Container*>(ScrolledWindow->get_child());

  // Replace TreeView by a label
  if(TypeIsEmpty && (G_OBJECT_TYPE(Container->gobj()) == G_OBJECT_TYPE(TreeView->gobj())))
  {
    ScrolledWindow->remove();

    Gtk::Label * LabelEmpty = Gtk::manage(new Gtk::Label());//,0.1,0.1));

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


