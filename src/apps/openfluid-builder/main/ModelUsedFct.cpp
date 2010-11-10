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

#include "openfluid/base.hpp"

#include "ModelUsedFct.hpp"


// =====================================================================
// =====================================================================


ModelUsedFct::ModelUsedFct(Glib::RefPtr<Gtk::Builder> GladeBuilder, openfluid::machine::ArrayOfModelItemInstance AllFctContainers, openfluid::machine::ModelInstance & ModelInstance)
: m_AllFctContainers(AllFctContainers)
{
  GladeBuilder->get_widget("TreeViewUsedFct",mp_TreeViewUsedFct);
  GladeBuilder->get_widget("ImageModelUsedFctTrash",mp_ImageModelUsedFctTrash);
  GladeBuilder->get_widget("NotebookParams",mp_NotebookParams);

  mp_ModelGlobalParams = new ModelGlobalParams(GladeBuilder);

  mp_TreeModelUsedFct = createTreeModelUsedFct(ModelInstance);

  mp_TreeViewUsedFct->set_model(mp_TreeModelUsedFct);

  std::list<Gtk::TargetEntry> ListTargets;
  ListTargets.push_back(Gtk::TargetEntry("text/plain"));
  ListTargets.push_back(Gtk::TargetEntry("TEXT"));
  ListTargets.push_back(Gtk::TargetEntry("STRING"));

  initTreeViewUsedFct(ListTargets);

  // Make the trash a DnD dest (for move from the used functions listview)
  mp_ImageModelUsedFctTrash->drag_dest_set(ListTargets,Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_MOVE);
  mp_ImageModelUsedFctTrash->signal_drag_data_received().connect(
          sigc::mem_fun(*this,&ModelUsedFct::onTrashDragDataReceived));

}


// =====================================================================
// =====================================================================


ModelUsedFct::~ModelUsedFct()
{
  delete mp_ModelGlobalParams;
}


// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::ListStore> ModelUsedFct::createTreeModelUsedFct(openfluid::machine::ModelInstance & ModelInstance)
{
  Glib::RefPtr<Gtk::ListStore> TreeModelUsedFct = Gtk::ListStore::create(m_Columns);

  const std::list<openfluid::machine::ModelItemInstance*> ItemInstances = ModelInstance.getItems();

  std::list<openfluid::machine::ModelItemInstance*>::const_iterator it;

  for(it=ItemInstances.begin() ; it!=ItemInstances.end() ; ++it)
  {
    openfluid::machine::ModelItemInstance * ItemInstance = *it;

    Gtk::TreeModel::Row RowIInstance = *(TreeModelUsedFct->append());

    RowIInstance[m_Columns.m_Id] = ItemInstance->Signature->ID;

    RowIInstance[m_Columns.m_UsedParams] = ItemInstance->Params;

    if(ItemInstance->ItemType == openfluid::base::ModelItemDescriptor::PluggedFunction)
      RowIInstance[m_Columns.m_Type] = ModelColumns::SimulationFunctions;
    else if(ItemInstance->ItemType == openfluid::base::ModelItemDescriptor::Generator)
      RowIInstance[m_Columns.m_Type] = ModelColumns::Generators;

    std::vector<openfluid::base::SignatureHandledDataItem> AvailParams;

    for(unsigned int i=0 ; i<m_AllFctContainers.size() ; i++)
    {
      if(m_AllFctContainers[i]->Signature->ID == ItemInstance->Signature->ID)
      {
        AvailParams = m_AllFctContainers[i]->Signature->HandledData.FunctionParams;

        createParamTab(RowIInstance,AvailParams,-1);

        break;
      }
    }
  }

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
  //empty but necessary to make drop to trash works
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

    mp_TreeViewUsedFct->get_selection()->select(*RowDest);
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


void ModelUsedFct::addAFunction(Glib::ustring Id, Gtk::TreeModel::Row & RowDest)
{
  unsigned int Index = *(mp_TreeModelUsedFct->get_path(RowDest).begin());

  for(unsigned int i=0 ; i<m_AllFctContainers.size() ; i++)
  {
    if(m_AllFctContainers[i]->Signature->ID == Id)
    {
      RowDest[m_Columns.m_Id] = Id;

      if(m_AllFctContainers[i]->ItemType == openfluid::base::ModelItemDescriptor::PluggedFunction)
        RowDest[m_Columns.m_Type] = ModelColumns::SimulationFunctions;
      else if(m_AllFctContainers[i]->ItemType == openfluid::base::ModelItemDescriptor::Generator)
        RowDest[m_Columns.m_Type] = ModelColumns::Generators;

      std::vector<openfluid::base::SignatureHandledDataItem> AvailParams = m_AllFctContainers[i]->Signature->HandledData.FunctionParams;

      openfluid::core::FuncParamsMap_t UsedParam;

      for(unsigned int j=0 ; j<AvailParams.size() ; j++)
        UsedParam[AvailParams[j].DataName] = "";

      RowDest[m_Columns.m_UsedParams] = UsedParam;

      createParamTab(RowDest,AvailParams,Index+1);

      break;
    }
  }

  tempCheckModel();
}


// =====================================================================
// =====================================================================


void ModelUsedFct::moveAFunction(Gtk::TreeModel::iterator & IterSrc, Gtk::TreeModel::iterator & IterDest)
{
  // Move in TreeModel (move the row)

  mp_TreeModelUsedFct->move(IterSrc,IterDest);// move to the end of the list if IterDest is null


  // Move in Notebook

  int Index = mp_TreeModelUsedFct->get_path(IterSrc)[0] +1 ; // index of the row in the new reordered list + the first Notebook page

  mp_NotebookParams->reorder_child(*((*IterSrc).get_value(m_Columns.m_NotebookParamsPage)),Index);

  mp_NotebookParams->set_current_page(Index);


  tempCheckModel();
}


// =====================================================================
// =====================================================================

void ModelUsedFct::removeSelectedUsedFunction()
{
  Gtk::TreeIter Iter = mp_TreeViewUsedFct->get_selection()->get_selected();

    if(Iter)
    {
      Gtk::TreeRow Row = *Iter;

      mp_NotebookParams->remove_page(*Row[m_Columns.m_NotebookParamsPage]);

      mp_ModelGlobalParams->removeParamRequest(Row[m_Columns.m_GlobalParamsLabels]);
    }

  tempCheckModel();
}


// =====================================================================
// =====================================================================


void ModelUsedFct::onTrashDragDataReceived(const Glib::RefPtr<Gdk::DragContext>& context, int /*x*/, int /*y*/,
    const Gtk::SelectionData& /*selection_data*/, guint /*info*/, guint time)
{
  removeSelectedUsedFunction();

  context->drag_finish(true,true,time);
}


// =====================================================================
// =====================================================================


void ModelUsedFct::createParamTab(Gtk::TreeModel::Row Row, std::vector<openfluid::base::SignatureHandledDataItem> AvailParams, int Position)
{
  Glib::ustring Id = Row[m_Columns.m_Id];

  Gtk::ScrolledWindow * Scrolled = Gtk::manage(new Gtk::ScrolledWindow());
  Scrolled->set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);

  Gtk::Table * TableParams = Gtk::manage(new Gtk::Table());

  TableParams->set_spacings(5);
  TableParams->set_border_width(5);

  Scrolled->add(*TableParams);

  std::vector<Gtk::Label *> GlobalParamsLabels;

  for(unsigned int i=0 ; i<AvailParams.size() ; i++)
  {
    Gtk::CheckButton * Check = Gtk::manage(new Gtk::CheckButton());

    Gtk::Label * Label = Gtk::manage(new Gtk::Label(AvailParams[i].DataName,Gtk::ALIGN_LEFT,Gtk::ALIGN_CENTER));

    Gtk::Entry * Entry = Gtk::manage(new Gtk::Entry());

    Gtk::Label * Unit = Gtk::manage(new Gtk::Label(AvailParams[i].DataUnit,Gtk::ALIGN_LEFT,Gtk::ALIGN_CENTER));

    Gtk::Label * LabelGlobal = Gtk::manage(new Gtk::Label("",Gtk::ALIGN_LEFT,Gtk::ALIGN_CENTER));

    Check->set_active(true);

    openfluid::core::FuncParamsMap_t UsedParams = Row[m_Columns.m_UsedParams];

    if(UsedParams.find(AvailParams[i].DataName) != UsedParams.end())
      Entry->set_text(UsedParams[AvailParams[i].DataName]);

    Check->signal_toggled().connect(
        sigc::bind<Glib::ustring,Gtk::Entry *,Gtk::CheckButton *,Gtk::TreeModel::Row>(
            sigc::mem_fun(*this,&ModelUsedFct::onCheckToggled),
            AvailParams[i].DataName,Entry,Check,Row));

    Entry->signal_focus_out_event().connect(
        sigc::bind<Glib::ustring,Gtk::Entry *,Gtk::TreeModel::Row>(
            sigc::mem_fun(*this,&ModelUsedFct::onEntryFocusOut),
            AvailParams[i].DataName,Entry,Row));

    if(Row[m_Columns.m_Type] == ModelColumns::Generators)
      Check->set_visible(false);
    else
    {
      Check->set_visible(true);

      mp_ModelGlobalParams->addParamRequest(AvailParams[i].DataName,AvailParams[i].DataUnit,LabelGlobal);
    }

    Label->set_visible(true);
    Entry->set_visible(true);
    Unit->set_visible(true);
    LabelGlobal->set_visible(true);

    TableParams->attach(*Check,0,1,i*2,i*2+1,Gtk::FILL,Gtk::FILL);
    TableParams->attach(*Label,1,2,i*2,i*2+1,Gtk::FILL,Gtk::FILL);
    TableParams->attach(*Entry,2,3,i*2,i*2+1,Gtk::FILL,Gtk::FILL);
    TableParams->attach(*Unit,3,4,i*2,i*2+1,Gtk::FILL,Gtk::FILL);
    TableParams->attach(*LabelGlobal,4,5,i*2,i*2+1,Gtk::FILL,Gtk::FILL);

    // don't set show_all(true) because of Check widget
    TableParams->set_visible(true);

    Scrolled->set_visible(true);

    GlobalParamsLabels.push_back(LabelGlobal);
  }

  // Add a page, even if no parameter (to keep synchro with used functions list).
  // In this case, we insert an empty widget to create a hidden page.

  Gtk::Label * LabelTab = Gtk::manage(new Gtk::Label(Id));
  Gtk::Label * LabelMenu = Gtk::manage(new Gtk::Label(Id,Gtk::ALIGN_LEFT));

//  int Index = mp_NotebookParams->insert_page(*TableParams,*LabelTab,*LabelMenu,Position);
  int Index = mp_NotebookParams->insert_page(*Scrolled,*LabelTab,*LabelMenu,Position);

  mp_NotebookParams->set_current_page(Index);

//  Row[m_Columns.m_NotebookParamsPage] = TableParams;
  Row[m_Columns.m_NotebookParamsPage] = Scrolled;
  Row[m_Columns.m_GlobalParamsLabels] = GlobalParamsLabels;

}


// =====================================================================
// =====================================================================


bool ModelUsedFct::onEntryFocusOut(GdkEventFocus * /*Event*/, Glib::ustring ParamName, Gtk::Entry * Entry, Gtk::TreeModel::Row Row)
{
  openfluid::core::FuncParamsMap_t UsedParams = Row[m_Columns.m_UsedParams];

  // if param doesn't exist we create it, otherwise we update it if necessary
  if(UsedParams.find(ParamName) == UsedParams.end() || UsedParams[ParamName] != Entry->get_text())
  {
    UsedParams[ParamName] = Entry->get_text();

    Row[m_Columns.m_UsedParams] = UsedParams;

    tempCheckModel();
  }

  return true;
}


// =====================================================================
// =====================================================================


void ModelUsedFct::onCheckToggled(Glib::ustring ParamName, Gtk::Entry * Entry, Gtk::CheckButton * Check, Gtk::TreeModel::Row Row)
{
  Entry->set_sensitive(Check->get_active());

  if(Check->get_active())
    onEntryFocusOut(0,ParamName,Entry,Row);
  else
  {
    openfluid::core::FuncParamsMap_t UsedParams = Row[m_Columns.m_UsedParams];

    UsedParams.erase(ParamName);

    Row[m_Columns.m_UsedParams] = UsedParams;
  }

  tempCheckModel();
}


// =====================================================================
// =====================================================================


std::vector<std::string> ModelUsedFct::getUsedFctIDs()
{
  std::vector<std::string> FctIDs;

  Gtk::TreeModel::Children Functions = mp_TreeModelUsedFct->children();
  Gtk::TreeModel::Children::iterator it;

  for(it=Functions.begin() ; it!=Functions.end() ; ++it)
  {
    FctIDs.push_back(it->get_value(m_Columns.m_Id));
  }

  return FctIDs;
}


// =====================================================================
// =====================================================================


openfluid::base::ModelDescriptor * ModelUsedFct::getModelDescriptor()
{
  openfluid::base::ModelDescriptor * ModelDesc = new openfluid::base::ModelDescriptor();

  Gtk::TreeModel::Children Functions = mp_TreeModelUsedFct->children();
  Gtk::TreeModel::Children::iterator it;

  for(it=Functions.begin() ; it!=Functions.end() ; ++it)
  {
    openfluid::base::ModelItemDescriptor * ItemDesc = 0;

    if((*it).get_value(m_Columns.m_Type) == ModelColumns::SimulationFunctions)
    {
      ItemDesc = new openfluid::base::FunctionDescriptor((*it).get_value(m_Columns.m_Id));
    }
    else if((*it).get_value(m_Columns.m_Type) == ModelColumns::Generators)
    {
      /* TODO: test Generator params before creating GeneratorDesc */
      ItemDesc = new openfluid::base::GeneratorDescriptor();
    }

    if(ItemDesc)
    {
      openfluid::core::FuncParamsMap_t UsedParams = (*it).get_value(m_Columns.m_UsedParams);

      openfluid::core::FuncParamsMap_t::iterator It;

      for(It=UsedParams.begin() ; It!=UsedParams.end() ; ++It)
      {
        if(It->second == "")
          It->second = mp_ModelGlobalParams->getGlobalParamValue(It->first);
      }

      ItemDesc->setParameters(UsedParams);

      ModelDesc->appendItem(ItemDesc);
    }

  }

  return ModelDesc;
}


// =====================================================================
// =====================================================================


void ModelUsedFct::tempCheckModel()
{
//  std::cout << "-------- checking Functions used: " << std::endl;
//
//  Gtk::TreeModel::Children Functions = mp_TreeModelUsedFct->children();
//
//  Gtk::TreeModel::Children::iterator it;
//
//  for(it=Functions.begin() ; it!=Functions.end() ; ++it)
//  {
//    std::cout << "- " << (*it).get_value(m_Columns.m_Id) << std::endl;
//
//    openfluid::core::FuncParamsMap_t Params = (*it).get_value(m_Columns.m_UsedParams);
//
//    openfluid::core::FuncParamsMap_t::iterator it2;
//
//    for(it2=Params.begin() ; it2 != Params.end(); ++it2)
//      std::cout << "   * " << it2->first << " : -" << it2->second << "-" << std::endl;
//  }

//  mp_ModelGlobalParams->tempCheckModel();

}
