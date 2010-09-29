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

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "BuilderHelper.hpp"
#include "ModelModule.hpp"


// =====================================================================
// =====================================================================


ModelModule::ModelModule(openfluid::machine::ModelInstance & Model, openfluid::machine::SimulationBlob & SimBlob)
: ModuleInterface("Model.glade", "ViewportModel", "MenuModel", "ToolBarModel"),
  m_Model(Model), m_SimBlob(SimBlob)
{
  m_ModuleName = _("_Model");
  m_ModuleLongName = _("Model");
  mp_StockId = BuilderHelper::createIconStockId(BUILDER_RESOURCE_PATH, "model-base.svg", "builder-model-base");

  mp_Builder->get_widget("TreeViewUsedFct",mp_TreeViewUsedFct);
  mp_Builder->get_widget("ImageModelUsedFctTrash",mp_ImageModelUsedFctTrash);
  mp_Builder->get_widget("NotebookParams",mp_NotebookParams);

  createActions();

  // create Available functions panel
  mp_ModelAvailFct = new ModelAvailFct(mp_Builder);

  // create Used functions panel
  createModelUsedFct();

  // set DnD to manage used functions list
  setDragAndDropManagement();

}


// =====================================================================
// =====================================================================


ModelModule::~ModelModule()
{

}


// =====================================================================
// =====================================================================


void ModelModule::createActions()
{
  // create action
  Glib::RefPtr<Gtk::Action> ActionCheckModel;
  ActionCheckModel = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionCheckModel"));
  ActionCheckModel->set_label(_("Check Model"));
  ActionCheckModel->set_tooltip(_("Check Model"));
  ActionCheckModel->signal_activate().connect(sigc::mem_fun(*this,&ModelModule::actionCheckModel));

  // add customized icons
  Gtk::StockID * CheckStockId = BuilderHelper::createIconStockId(BUILDER_RESOURCE_PATH, "check-model.svg", "builder-model-check");
  ActionCheckModel->set_stock_id(*CheckStockId);

  m_Actions.push_back(ActionCheckModel);

}


// =====================================================================
// =====================================================================


void ModelModule::createModelUsedFct()
{
  // create Used functions TreeModel

  mp_TreeModelUsedFct = Gtk::ListStore::create(m_Columns);

  mp_TreeModelUsedFct->signal_row_deleted().connect(
          sigc::mem_fun(*this, &ModelModule::onUsedFctRowDeleted));

  const std::list<openfluid::machine::ModelItemInstance*> Functions = m_Model.getItems();

  std::list<openfluid::machine::ModelItemInstance*>::const_iterator it;

  for(it=Functions.begin() ; it!=Functions.end() ; ++it)
  {
    openfluid::machine::ModelItemInstance * Function = *it;

    Gtk::TreeModel::Row RowFct = *(mp_TreeModelUsedFct->append());

    RowFct[m_Columns.m_Id] = Function->Signature->ID;

    RowFct[m_Columns.m_ModelItemInstance] = Function;

    if(Function->ItemType == openfluid::base::ModelItemDescriptor::PluggedFunction)
      RowFct[m_Columns.m_Type] = ModelColumns::SimulationFunctions;

    else if(Function->ItemType == openfluid::base::ModelItemDescriptor::Generator)
      RowFct[m_Columns.m_Type] = ModelColumns::Generators;

    addParamTab(*Function);
  }


  // create Used functions TreeView

  mp_TreeViewUsedFct->append_column("Id",m_Columns.m_Id);

  mp_TreeViewUsedFct->set_model(mp_TreeModelUsedFct);

  mp_TreeViewUsedFct->set_search_column(m_Columns.m_Id);

}


// =====================================================================
// =====================================================================


void ModelModule::setDragAndDropManagement()
{
  std::list<Gtk::TargetEntry> listTargets;
  listTargets.push_back(Gtk::TargetEntry("text/plain"));
  listTargets.push_back(Gtk::TargetEntry("TEXT"));
  listTargets.push_back(Gtk::TargetEntry("STRING"));

  // Enable source drag'n drop from Available functions (for copy)
  Gtk::TreeView * TreeViewAvailFct = mp_ModelAvailFct->getTreeViewAvailFct();

  TreeViewAvailFct->enable_model_drag_source(listTargets,Gdk::BUTTON1_MASK, Gdk::ACTION_COPY);

  TreeViewAvailFct->signal_drag_data_get().connect(
      sigc::bind<Gtk::TreeView*>(
          sigc::mem_fun(*this,&ModelModule::onSourceDragDataGet),
          TreeViewAvailFct));

  // Enable source drag'n drop from  Used functions (for move)
  mp_TreeViewUsedFct->enable_model_drag_source(listTargets,Gdk::BUTTON1_MASK, Gdk::ACTION_MOVE);

  mp_TreeViewUsedFct->signal_drag_data_get().connect(
      sigc::bind<Gtk::TreeView*>(
          sigc::mem_fun(*this,&ModelModule::onSourceDragDataGet),
          mp_TreeViewUsedFct));

  // Enable dest drag'n drop to Used functions (for move)
  mp_TreeViewUsedFct->enable_model_drag_dest(listTargets, Gdk::ACTION_COPY | Gdk::ACTION_MOVE);

  mp_TreeViewUsedFct->signal_drag_data_received().connect(
      sigc::bind<Gtk::TreeView *>(
          sigc::mem_fun(*this,&ModelModule::onDestDragDataReceived),
          mp_TreeViewUsedFct));

  // Enable dest drag'n drop to Trash (for move)
  mp_ImageModelUsedFctTrash->drag_dest_set(listTargets,Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_MOVE);

}


// =====================================================================
// =====================================================================


void ModelModule::onSourceDragDataGet(const Glib::RefPtr< Gdk::DragContext >& /*context*/,
    Gtk::SelectionData& selection_data, guint /*info*/, guint /*time*/,
    Gtk::TreeView * TreeViewSource)
{
  Gtk::TreeModel::iterator iter = TreeViewSource->get_selection()->get_selected();

  Gtk::TreeModel::Row row = *iter;

  Glib::ustring Txt = Glib::ustring::compose("[%1]%2",row.get_value(m_Columns.m_Type),row.get_value(m_Columns.m_Id));

  selection_data.set("text/plain",Txt);
}


// =====================================================================
// =====================================================================


void ModelModule::onDestDragDataReceived(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
    const Gtk::SelectionData& selection_data, guint /*info*/, guint time,
    Gtk::TreeView * TreeViewDest)
{
  Glib::RefPtr<Gtk::ListStore> TreeModelDest = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(TreeViewDest->get_model());

  Gtk::TreeModel::Path PathDest;
  Gtk::TreeViewDropPosition PositionDest;
  TreeViewDest->get_dest_row_at_pos(x,y,PathDest,PositionDest);

  Gtk::TreeModel::iterator IterDest;

  Gtk::TreeModel::Row RowDest;

  // Move action inside used functions
  if(context->get_action() == Gdk::ACTION_MOVE)
  {
    Gtk::TreeModel::iterator IterSrc = TreeViewDest->get_selection()->get_selected();

    Gtk::TreeModel::Row RowSrc = *IterSrc;
    openfluid::machine::ModelItemInstance * IInstance = RowSrc[m_Columns.m_ModelItemInstance];

    if(PathDest)
    {
      IterDest = TreeModelDest->get_iter(PathDest);

      unsigned int IndexDest = *PathDest.begin();

      if (PositionDest == Gtk::TREE_VIEW_DROP_BEFORE || PositionDest == Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE)
      {
        RowDest = *(TreeModelDest->insert(IterDest));
      }
      else
      {
        RowDest = *(TreeModelDest->insert_after(IterDest));
        ++IndexDest;
      }

      if(IndexDest < m_Model.getItemsCount())
      {
      m_Model.insertItem(IInstance,IndexDest);

      addParamTab(*IInstance,IndexDest);
      }
      else
      {
        m_Model.appendItem(IInstance);

        addParamTab(*IInstance);
      }
    }
    else
    {
      RowDest = *(TreeModelDest->append());

      m_Model.appendItem(IInstance);

      addParamTab(*IInstance);
    }

    RowDest[m_Columns.m_Id] = IInstance->Signature->ID;
    RowDest[m_Columns.m_ModelItemInstance] = IInstance;

    // Delete the source (Model and Tab deletion are handled by on_row_deleted)
    context->drag_finish(true,true,time);
  }


  // Copy action from available functions to used functions
  else if(context->get_action() == Gdk::ACTION_COPY)
  {
    if(PathDest)
    {
      IterDest = TreeModelDest->get_iter(PathDest);

      if (PositionDest == Gtk::TREE_VIEW_DROP_BEFORE || PositionDest == Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE)
        RowDest = *(TreeModelDest->insert(IterDest));
      else
        RowDest = *(TreeModelDest->insert_after(IterDest));
    }
    else
      RowDest = *(TreeModelDest->append());

    Glib::ustring Txt = selection_data.get_text();
    std::vector<Glib::ustring> SplitTxt;

    boost::algorithm::split(SplitTxt, Txt, boost::is_any_of(std::string("[]")));

    int Type = boost::lexical_cast<int>(SplitTxt[1]);

    Glib::ustring Id = SplitTxt[2];

    openfluid::machine::ModelItemInstance * IInstance = 0;

    if(Type == ModelColumns::SimulationFunctions)
    {
      IInstance = openfluid::machine::PluginManager::getInstance()->getPlugin(Id.c_str(),&(m_SimBlob.getExecutionMessages()),&(m_SimBlob.getCoreRepository()));

      IInstance->ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

      RowDest[m_Columns.m_Type] = ModelColumns::SimulationFunctions;
    }
    else if(Type == ModelColumns::Generators)
    {
      //    openfluid::base::GeneratorDescriptor* GenDesc = (openfluid::base::GeneratorDescriptor*)(*it);
      openfluid::machine::Generator * GenInstance = NULL;

      IInstance = new openfluid::machine::ModelItemInstance();
      IInstance->SDKCompatible = true;

      IInstance->ItemType = openfluid::base::ModelItemDescriptor::Generator;

      RowDest[m_Columns.m_Type] = ModelColumns::Generators;

      openfluid::base::FunctionSignature* Signature = new openfluid::base::FunctionSignature();

//      Signature->ID = "(generator)";
      Signature->ID = Id;

      //    std::string VarName = GenDesc->getVariableName();
      //    if (GenDesc->isVectorVariable()) VarName = VarName + "[]";
      //
      //    Signature->ID = "(generator)"+VarName;
      //    Signature->HandledData.ProducedVars.push_back(openfluid::base::SignatureHandledDataItem(VarName,GenDesc->getUnitClass(),"",""));

      Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem("Varname","","",""));
      Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem("Unitclass","","",""));
      Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem("Varsize","","",""));
      //    if (GenDesc->getGeneratorMethod() == openfluid::base::GeneratorDescriptor::Fixed)
      if(Id == "Fixed")
      {
        GenInstance = new openfluid::machine::FixedGenerator();
        Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem("fixedvalue","","",""));
      }
      //    if (GenDesc->getGeneratorMethod() == openfluid::base::GeneratorDescriptor::Random)
      else if(Id == "Random")
      {
        GenInstance = new openfluid::machine::RandomGenerator();
        Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem("min","","",""));
        Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem("max","","",""));
      }
      //    if (GenDesc->getGeneratorMethod() == openfluid::base::GeneratorDescriptor::Interp)
      else if(Id == "Interp")
      {
        //      Signature->HandledData.RequiredExtraFiles.push_back(GenDesc->getParameters()["sources"]);
        //      Signature->HandledData.RequiredExtraFiles.push_back(GenDesc->getParameters()["distribution"]);
        GenInstance = new openfluid::machine::InterpGenerator();
        Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem("sources","","",""));
        Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem("distribution","","",""));
      }

      //    if (GenInstance == NULL)
      //      throw openfluid::base::OFException("OpenFLUID framework","ModelFactory::buildInstanceFromDescriptor","unknown generator type");

      GenInstance->setDataRepository(&(m_SimBlob.getCoreRepository()));
      GenInstance->setExecutionMessages(&(m_SimBlob.getExecutionMessages()));
      GenInstance->setFunctionEnvironment(openfluid::base::RuntimeEnvironment::getInstance()->getFunctionEnvironment());
      //    GenInstance->setDescriptor(*GenDesc);
      IInstance->Function = GenInstance;
      IInstance->Signature = Signature;
    }

    if(IInstance != 0)
    {
      RowDest[m_Columns.m_Id] = Id;

      RowDest[m_Columns.m_ModelItemInstance] = IInstance;

      Gtk::TreePath DestRowPath(RowDest);

      unsigned int Index = *DestRowPath.begin();
//      unsigned int Index = *DestRowPath.get_indices().begin();

      if(Index < m_Model.getItemsCount())
        m_Model.insertItem(IInstance,Index);
      else
        m_Model.appendItem(IInstance);

      addParamTab(*IInstance,Index);
    }

    // Does not delete the source (it's a copy)
    context->drag_finish(true,false,time);
  }

  TreeViewDest->get_selection()->select(RowDest);

  actionCheckModel();
}


// =====================================================================
// =====================================================================


void ModelModule::onUsedFctRowDeleted(const Gtk::TreeModel::Path& Path)
{
  unsigned int Index = *Path.get_indices().begin();

  m_Model.deleteItem(Index);

  mp_NotebookParams->remove_page(Index+1);

  actionCheckModel();
}


// =====================================================================
// =====================================================================


void ModelModule::addParamTab(openfluid::machine::ModelItemInstance & Function, int Position)
{
  Glib::ustring Id = Function.Signature->ID;

  Gtk::Table * TableParams = Gtk::manage(new Gtk::Table());

  TableParams->set_spacings(5);


  std::vector<openfluid::base::SignatureHandledDataItem> Params = Function.Signature->HandledData.FunctionParams;

  if(!Params.empty())
  {
    for(unsigned int i=0 ; i<Params.size() ; i++)
    {
      Gtk::Label * Label = Gtk::manage(new Gtk::Label(Params[i].DataName,Gtk::ALIGN_LEFT,Gtk::ALIGN_CENTER));
      Label->set_visible(true);
      TableParams->attach(*Label,0,1,i*2,i*2+1,Gtk::FILL,Gtk::FILL);

      Gtk::Entry * Entry = Gtk::manage(new Gtk::Entry());
      Entry->set_text(Function.Params[Params[i].DataName]);
      Entry->set_visible(true);
      TableParams->attach(*Entry,1,2,i*2,i*2+1,Gtk::FILL,Gtk::FILL);
      Entry->signal_focus_out_event().connect(
          sigc::bind<Glib::ustring,Gtk::Entry *,openfluid::machine::ModelItemInstance &>(
              sigc::mem_fun(*this,&ModelModule::onEntryFocusOut),
              Params[i].DataName,Entry,Function));

      Gtk::Label * Unit = Gtk::manage(new Gtk::Label(Params[i].DataUnit,Gtk::ALIGN_LEFT,Gtk::ALIGN_CENTER));
      Unit->set_visible(true);
      TableParams->attach(*Unit,2,3,i*2,i*2+1,Gtk::FILL,Gtk::FILL);
    }

    TableParams->set_visible(true);
  }

  // Add a page, even if no parameter (to keep synchro with used function list).
  // In this case, we insert an empty widget to create a hidden page.

    Gtk::Label * LabelTab = Gtk::manage(new Gtk::Label(Id));
    Gtk::Label * LabelMenu = Gtk::manage(new Gtk::Label(Id,Gtk::ALIGN_LEFT));

    int Index = mp_NotebookParams->insert_page(*TableParams,*LabelTab,*LabelMenu,Position+1);

    mp_NotebookParams->set_current_page(Index);

}


// =====================================================================
// =====================================================================


bool ModelModule::onEntryFocusOut(GdkEventFocus * /*Event*/, Glib::ustring ParamName, Gtk::Entry * Entry, openfluid::machine::ModelItemInstance & Function)
{
  Function.Params[ParamName] = Entry->get_text();
  return true;
}


// =====================================================================
// =====================================================================


void ModelModule::actionCheckModel()
{
  std::cout << "------------------" << std::endl;

  const std::list<openfluid::machine::ModelItemInstance*> Functions = m_Model.getItems();

  std::list<openfluid::machine::ModelItemInstance*>::const_iterator it;

  for(it=Functions.begin() ; it!=Functions.end() ; ++it)
  {
    openfluid::machine::ModelItemInstance * Function = *it;

    std::cout << "-" << Function->Signature->ID << std::endl;
  }
}



