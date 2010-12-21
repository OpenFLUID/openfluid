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
  \file ModelStructure_VList.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */


#include <iostream>

#include <glibmm/i18n.h>

#include "ModelStructure.hpp"
#include "ModelStructure_VList.hpp"


// =====================================================================
// =====================================================================


ModelStructure_VList::ModelStructure_VList(ModelStructure * Control, Glib::RefPtr<Gtk::Builder> GladeBuilder)
:mp_Control(Control),mp_TreeView(0)
{
  GladeBuilder->get_widget("TreeViewUsedFct",mp_TreeView);

  if(mp_TreeView)
  {
    mp_TreeModel = Gtk::ListStore::create(m_ListColumns);

    mp_TreeView->set_model(mp_TreeModel);

    mp_TreeView->append_column(_(""),m_ListColumns.m_Index);
    mp_TreeView->append_column(_("Id"),m_ListColumns.m_Id);

    mp_TreeView->set_search_column(m_ListColumns.m_Id);


    std::list<Gtk::TargetEntry> ListTargets;
    ListTargets.push_back(Gtk::TargetEntry("text/plain"));
    ListTargets.push_back(Gtk::TargetEntry("TEXT"));
    ListTargets.push_back(Gtk::TargetEntry("STRING"));

    // Make this view a DnD source (for move to this view or to the trash)
    mp_TreeView->enable_model_drag_source(ListTargets,Gdk::BUTTON1_MASK, Gdk::ACTION_MOVE);

    mp_TreeView->signal_drag_data_get().connect(
        sigc::mem_fun(*this,&ModelStructure_VList::onSourceDragDataGet));

    // Make this view a DnD dest (for move from this view or copy from available functions listview)
    mp_TreeView->enable_model_drag_dest(ListTargets, Gdk::ACTION_COPY | Gdk::ACTION_MOVE);

    mp_TreeView->signal_drag_data_received().connect(
        sigc::mem_fun(*this,&ModelStructure_VList::onDestDragDataReceived));
  }
}


// =====================================================================
// =====================================================================


void ModelStructure_VList::onSourceDragDataGet(const Glib::RefPtr< Gdk::DragContext >& /*context*/,
    Gtk::SelectionData& selection_data, guint /*info*/, guint /*time*/)
{
  int Position = *(mp_TreeModel->get_path(mp_TreeView->get_selection()->get_selected()).begin());

  selection_data.set("text/plain",Glib::ustring::format(std::dec,Position));
}


// =====================================================================
// =====================================================================


void ModelStructure_VList::onDestDragDataReceived(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
    const Gtk::SelectionData& selection_data, guint /*info*/, guint time)
{
  Gtk::TreeModel::Path PathDest;
  Gtk::TreeViewDropPosition PositionDest;

  mp_TreeView->get_dest_row_at_pos(x,y,PathDest,PositionDest);

  if(context->get_action() == Gdk::ACTION_COPY) // it's a copy from available functions list to used functions list
  {
    int Position = getPositionFromPath(PathDest,PositionDest);

    mp_Control->VList_itemAdded(selection_data.get_text(),Position);
  }

  else if(context->get_action() == Gdk::ACTION_MOVE) // it's a move within used functions list
  {
    int PositionFrom = *(mp_TreeView->get_model()->get_path(mp_TreeView->get_selection()->get_selected()).begin());

    int PositionTo = getPositionFromPath(PathDest,PositionDest);

    if(PositionFrom != PositionTo)
      mp_Control->VList_itemMoved(PositionFrom,PositionTo);

  }

  context->drag_finish(true,false,time);

}


// =====================================================================
// =====================================================================


int ModelStructure_VList::getPositionFromPath(Gtk::TreeModel::Path PathDest, Gtk::TreeViewDropPosition PositionDest)
{
  int Position;

  if(PathDest) // dest is not the empty space after the last row
  {
    if (PositionDest == Gtk::TREE_VIEW_DROP_BEFORE || PositionDest == Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE)
      Position = *(PathDest.begin());
    else
      Position = *(PathDest.begin()) + 1;
  }
  else
    Position = -1;

  return Position;
}


// =====================================================================
// =====================================================================


Gtk::TreeModel::iterator ModelStructure_VList::getIterFromPosition(int Position)
{
  if(Position > -1 && Position < (int)mp_TreeModel->children().size())
    return mp_TreeModel->children()[Position];

  return mp_TreeModel->children().end();
}


// =====================================================================
// =====================================================================


void ModelStructure_VList::addRow(Glib::ustring Id, int Index, int Position)
{
  Gtk::TreeRow Row = *(mp_TreeModel->insert(getIterFromPosition(Position)));

  Row[m_ListColumns.m_Id] = Id;
  Row[m_ListColumns.m_Index] = Index;
}


// =====================================================================
// =====================================================================


void ModelStructure_VList::moveRow(int PositionFrom, int PositionTo)
{
  mp_TreeModel->move(getIterFromPosition(PositionFrom),getIterFromPosition(PositionTo));
}


// =====================================================================
// =====================================================================


void ModelStructure_VList::removeRow(int Position)
{
  mp_TreeModel->erase(getIterFromPosition(Position));
}


// =====================================================================
// =====================================================================


void ModelStructure_VList::updateIndexes()
{
  for(int i=0 ; i<(int)mp_TreeModel->children().size() ; i++)
    mp_TreeModel->children()[i].set_value(m_ListColumns.m_Index,i+1);
}
