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
  \file ModelUsedFct.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <iostream>

#include <glibmm/i18n.h>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "ModelUsedFct.hpp"


// =====================================================================
// =====================================================================


ModelUsedFct::ModelUsedFct(Glib::RefPtr<Gtk::Builder> GladeBuilder, openfluid::machine::ModelInstance & ModelInstance, openfluid::machine::SimulationBlob & SimBlob)
: m_ModelInstance(ModelInstance), m_SimBlob(SimBlob)
{
  GladeBuilder->get_widget("TreeViewUsedFct",mp_TreeViewUsedFct);
  GladeBuilder->get_widget("ImageModelUsedFctTrash",mp_ImageModelUsedFctTrash);
  GladeBuilder->get_widget("NotebookParams",mp_NotebookParams);

//  mp_Status = new ModelStatus();

  mp_TreeModelUsedFct = createTreeModelUsedFct();

  mp_TreeViewUsedFct->set_model(mp_TreeModelUsedFct);

  std::list<Gtk::TargetEntry> ListTargets;
  ListTargets.push_back(Gtk::TargetEntry("text/plain"));
  ListTargets.push_back(Gtk::TargetEntry("TEXT"));
  ListTargets.push_back(Gtk::TargetEntry("STRING"));

  initTreeViewUsedFct(ListTargets);

  // Make the trash a DnD dest (for move from the used functions listview)
  mp_ImageModelUsedFctTrash->drag_dest_set(ListTargets,Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_MOVE);
}


// =====================================================================
// =====================================================================


ModelUsedFct::~ModelUsedFct()
{
//  delete mp_Status;
//  mp_Status = 0;
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::ListStore> ModelUsedFct::createTreeModelUsedFct()
{
  Glib::RefPtr<Gtk::ListStore> TreeModelUsedFct = Gtk::ListStore::create(m_Columns);

  const std::list<openfluid::machine::ModelItemInstance*> ItemInstances = m_ModelInstance.getItems();

  std::list<openfluid::machine::ModelItemInstance*>::const_iterator it;

  for(it=ItemInstances.begin() ; it!=ItemInstances.end() ; ++it)
  {
    openfluid::machine::ModelItemInstance * ItemInstance = *it;

    Gtk::TreeModel::Row RowIInstance = *(TreeModelUsedFct->append());

    RowIInstance[m_Columns.m_Id] = ItemInstance->Signature->ID;

    RowIInstance[m_Columns.m_ModelItemInstance] = ItemInstance;

    RowIInstance[m_Columns.m_NotebookParamsPage] = createParamTab(*ItemInstance,-1);
  }

  TreeModelUsedFct->signal_row_deleted().connect(
      sigc::mem_fun(*this, &ModelUsedFct::deleteAFunction));

  return TreeModelUsedFct;
}


// =====================================================================
// =====================================================================


void ModelUsedFct::initTreeViewUsedFct(std::list<Gtk::TargetEntry> ListTargets)
{
  mp_TreeViewUsedFct->append_column("Id",m_Columns.m_Id);

  mp_TreeViewUsedFct->set_search_column(m_Columns.m_Id);

  // Make this view a DnD source (for move to this view or to the trash)
  mp_TreeViewUsedFct->enable_model_drag_source(ListTargets,Gdk::BUTTON1_MASK, Gdk::ACTION_MOVE);

  mp_TreeViewUsedFct->signal_drag_data_get().connect(
          sigc::mem_fun(*this,&ModelUsedFct::onSourceDragDataGet));

  // Make this view a DnD dest (for move from this view or copy from available functions listview)
  mp_TreeViewUsedFct->enable_model_drag_dest(ListTargets, Gdk::ACTION_COPY | Gdk::ACTION_MOVE);

  mp_TreeViewUsedFct->signal_drag_data_received().connect(
          sigc::mem_fun(*this,&ModelUsedFct::onDestDragDataReceived));
}


// =====================================================================
// =====================================================================


void ModelUsedFct::onSourceDragDataGet(const Glib::RefPtr< Gdk::DragContext >& /*context*/,
    Gtk::SelectionData& selection_data, guint /*info*/, guint /*time*/)
{
  // dummy function, used to make this DnD source being recognized (for drop to trash)
  selection_data.set("text/plain","");
}


// =====================================================================
// =====================================================================


void ModelUsedFct::onDestDragDataReceived(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
    const Gtk::SelectionData& selection_data, guint /*info*/, guint time)
{
  Gtk::TreeModel::Path PathDest;
  Gtk::TreeViewDropPosition PositionDest;

  mp_TreeViewUsedFct->get_dest_row_at_pos(x,y,PathDest,PositionDest);

  // Pointer to the row before which we have to insert, relative to the old non-reordered list
  Gtk::TreeModel::iterator IterDest;

  if(context->get_action() == Gdk::ACTION_COPY) // it's a copy from available functions list to used functions list
  {
    Gtk::TreeModel::Row RowDest;

    if(PathDest) // dest is not the empty space after the last row
    {
      IterDest = mp_TreeModelUsedFct->get_iter(PathDest);

      if (PositionDest == Gtk::TREE_VIEW_DROP_BEFORE || PositionDest == Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE)
        RowDest = *(mp_TreeModelUsedFct->insert(IterDest));
      else
        RowDest = *(mp_TreeModelUsedFct->insert_after(IterDest));
    }
    else
      RowDest = *(mp_TreeModelUsedFct->append());

    addAFunction(selection_data.get_text(),RowDest);

    mp_TreeViewUsedFct->get_selection()->select(RowDest);
  }

  else if(context->get_action() == Gdk::ACTION_MOVE) // it's a move within used functions list
  {
    Gtk::TreeIter IterSrc = mp_TreeViewUsedFct->get_selection()->get_selected();

    if(PathDest) // dest is not the empty space after the last row
    {
      // Pointer to the row to move, relative to the old non-reordered list
      IterDest = mp_TreeModelUsedFct->get_iter(PathDest);

      if (PositionDest != Gtk::TREE_VIEW_DROP_BEFORE && PositionDest != Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE)
        ++IterDest;
    }
    else
      IterDest = mp_TreeModelUsedFct->children().end(); // return an iterator just past the end of the container, so is null

    Gtk::TreeIter IterSrcTest = IterSrc;
    ++IterSrcTest;
    if(IterDest != IterSrc && IterDest != IterSrcTest)
      moveAFunction(IterSrc,IterDest);
  }

  context->drag_finish(true,false,time);

}


// =====================================================================
// =====================================================================


void ModelUsedFct::addAFunction(Glib::ustring Selection_Data, Gtk::TreeModel::Row & Row)
{
  // Get data for the new function

  std::vector<Glib::ustring> SplitTxt;

  boost::algorithm::split(SplitTxt, Selection_Data, boost::is_any_of(std::string("[]")));

  int Type = boost::lexical_cast<int>(SplitTxt[1]);
  Glib::ustring Id = SplitTxt[2];

  unsigned int Index = *(mp_TreeModelUsedFct->get_path(Row).begin());


  // Add to ModelInstance (create a new Item and add it)

  openfluid::machine::ModelItemInstance * IInstance = 0;

  if(Type == ModelColumns::SimulationFunctions)
    IInstance = createAModelItemInstance(openfluid::base::ModelItemDescriptor::PluggedFunction, Id);
  else if(Type == ModelColumns::Generators)
    IInstance = createAModelItemInstance(openfluid::base::ModelItemDescriptor::Generator, Id);

  if(Index < m_ModelInstance.getItemsCount())
    m_ModelInstance.insertItem(IInstance,Index);
  else
    m_ModelInstance.appendItem(IInstance);


  // Add to TreeModel (set values to the newly created row)

  Row[m_Columns.m_ModelItemInstance] = IInstance;

  Row[m_Columns.m_Id] = IInstance->Signature->ID;


  // Add to Notebook

  Row[m_Columns.m_NotebookParamsPage] = createParamTab(*IInstance,Index+1);


  checkModel();

}


// =====================================================================
// =====================================================================


void ModelUsedFct::moveAFunction(Gtk::TreeModel::iterator & IterSrc, Gtk::TreeModel::iterator & IterDest)
{
  // Move in TreeModel (move the row)

  mp_TreeModelUsedFct->move(IterSrc,IterDest);// move to the end of the list if IterDest is null


  // Move in ModelInstance (clear it and repopulate it from new ordered list)

  const std::list<openfluid::machine::ModelItemInstance *> ItemInstances = m_ModelInstance.getItems();

  // to replace m_ModelInstance.clear() which crashes
  while(m_ModelInstance.getItemsCount() != 0)
    m_ModelInstance.deleteItem(0);

  Gtk::TreeModel::Children Children = mp_TreeModelUsedFct->children();

  for(Gtk::TreeModel::Children::iterator TheIter = Children.begin() ; TheIter != Children.end() ; ++TheIter)
  {
    openfluid::machine::ModelItemInstance * IInstance = (*TheIter).get_value(m_Columns.m_ModelItemInstance);

    m_ModelInstance.appendItem(IInstance);
  }


  // Move in Notebook

  int Index = mp_TreeModelUsedFct->get_path(IterSrc)[0] +1 ; // index of the row in the new reordered list + the first Notebook page

  mp_NotebookParams->reorder_child(*((*IterSrc).get_value(m_Columns.m_NotebookParamsPage)),Index);

  mp_NotebookParams->set_current_page(Index);


  checkModel();
}


// =====================================================================
// =====================================================================


void ModelUsedFct::deleteAFunction(const Gtk::TreeModel::Path& Path)
{
  unsigned int Index = *Path.get_indices().begin();

  m_ModelInstance.deleteItem(Index);

  mp_NotebookParams->remove_page(Index+1);

  checkModel();
}


// =====================================================================
// =====================================================================


openfluid::machine::ModelItemInstance * ModelUsedFct::createAModelItemInstance(openfluid::base::ModelItemDescriptor::ModelItemType ItemType, Glib::ustring ItemId)
{
  openfluid::machine::ModelItemInstance * IInstance = 0;

  if(ItemType == openfluid::base::ModelItemDescriptor::PluggedFunction)
  {
    IInstance = openfluid::machine::PluginManager::getInstance()->getPlugin(ItemId.c_str(),&(m_SimBlob.getExecutionMessages()),&(m_SimBlob.getCoreRepository()));

    IInstance->ItemType = ItemType;
  }

  else if(ItemType == openfluid::base::ModelItemDescriptor::Generator)
  {
//    openfluid::base::GeneratorDescriptor* GenDesc = (openfluid::base::GeneratorDescriptor*)(*it);
    openfluid::machine::Generator * GenInstance = 0;

    IInstance = new openfluid::machine::ModelItemInstance();
    IInstance->SDKCompatible = true;
    IInstance->ItemType = ItemType;

    openfluid::base::FunctionSignature* Signature = new openfluid::base::FunctionSignature();

    Signature->ID = Glib::ustring::compose("(generator)%1",ItemId);

    //    std::string VarName = GenDesc->getVariableName();
    //    if (GenDesc->isVectorVariable()) VarName = VarName + "[]";
    //
    //    Signature->ID = "(generator)"+VarName;
    //    Signature->HandledData.ProducedVars.push_back(openfluid::base::SignatureHandledDataItem(VarName,GenDesc->getUnitClass(),"",""));

    Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem("Varname","","",""));
    Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem("Unitclass","","",""));
    Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem("Varsize","","",""));
    //    if (GenDesc->getGeneratorMethod() == openfluid::base::GeneratorDescriptor::Fixed)
    if(ItemId == "Fixed")
    {
      GenInstance = new openfluid::machine::FixedGenerator();
      Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem("fixedvalue","","",""));
    }
    //    if (GenDesc->getGeneratorMethod() == openfluid::base::GeneratorDescriptor::Random)
    else if(ItemId == "Random")
    {
      GenInstance = new openfluid::machine::RandomGenerator();
      Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem("min","","",""));
      Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem("max","","",""));
    }
    //    if (GenDesc->getGeneratorMethod() == openfluid::base::GeneratorDescriptor::Interp)
    else if(ItemId == "Interp")
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

  return IInstance;
}



// =====================================================================
// =====================================================================


Gtk::Widget * ModelUsedFct::createParamTab(openfluid::machine::ModelItemInstance & Function, int Position)
{
  Glib::ustring Id = Function.Signature->ID;

  Gtk::Table * TableParams = Gtk::manage(new Gtk::Table());

  TableParams->set_spacings(5);


  std::vector<openfluid::base::SignatureHandledDataItem> Params = Function.Signature->HandledData.FunctionParams;
  //  openfluid::core::FuncParamsMap_t Params = Function.Params;

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
            sigc::mem_fun(*this,&ModelUsedFct::onEntryFocusOut),
            Params[i].DataName,Entry,Function));

    Gtk::Label * Unit = Gtk::manage(new Gtk::Label(Params[i].DataUnit,Gtk::ALIGN_LEFT,Gtk::ALIGN_CENTER));

    Unit->set_visible(true);

    TableParams->attach(*Unit,2,3,i*2,i*2+1,Gtk::FILL,Gtk::FILL);

    TableParams->set_visible(true);
  }


  // Add a page, even if no parameter (to keep synchro with used functions list).
  // In this case, we insert an empty widget to create a hidden page.

  Gtk::Label * LabelTab = Gtk::manage(new Gtk::Label(Id));
  Gtk::Label * LabelMenu = Gtk::manage(new Gtk::Label(Id,Gtk::ALIGN_LEFT));

  int Index = mp_NotebookParams->insert_page(*TableParams,*LabelTab,*LabelMenu,Position);

  mp_NotebookParams->set_current_page(Index);

  return TableParams;

}


// =====================================================================
// =====================================================================


bool ModelUsedFct::onEntryFocusOut(GdkEventFocus * /*Event*/, Glib::ustring ParamName, Gtk::Entry * Entry, openfluid::machine::ModelItemInstance & Function)
{
  if(Function.Params[ParamName] != Entry->get_text())
  {
    Function.Params[ParamName] = Entry->get_text();
    checkModel();
  }

  return true;
}


// =====================================================================
// =====================================================================


void ModelUsedFct::checkModel()
{
  const std::list<openfluid::machine::ModelItemInstance *> ItemInstances = m_ModelInstance.getItems();

  std::list<openfluid::machine::ModelItemInstance *>::const_iterator it;

//  mp_Status->clearErrorParamsValue();

  std::cout << "Check :" << std::endl;

  for(it=ItemInstances.begin() ; it!=ItemInstances.end() ; ++it)
  {
    openfluid::machine::ModelItemInstance * ItemInstance = *it;

    // Display used functions list for information
    std::cout << "-" << ItemInstance->Signature->ID << std::endl;

//    try
//    {
//      ItemInstance->Function->initParams(ItemInstance->Params);
//    }
//    catch(openfluid::base::OFException& E)
//    {
//      mp_Status->appendErrorParamsValue(Glib::ustring::compose("%1 : %2",ItemInstance->Signature->ID,E.what()));
//      continue;
//    }
  }

}

