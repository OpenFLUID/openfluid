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
 \file ModelUsedFct.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */


#ifndef __MODELUSEDFCT_HPP__
#define __MODELUSEDFCT_HPP__


#include <gtkmm.h>

#include <openfluid/machine.hpp>
#include <openfluid/base.hpp>

#include "ModelColumns.hpp"
#include "ModelGlobalParams.hpp"


// =====================================================================
// =====================================================================


class ModelUsedFct
{
  public:

    ModelUsedFct(Glib::RefPtr<Gtk::Builder> GladeBuilder, openfluid::machine::ArrayOfModelItemInstance AllFctContainers, openfluid::machine::ModelInstance & ModelInstance);

    ~ModelUsedFct();

    std::vector<std::string> getUsedFctIDs();

    openfluid::base::ModelDescriptor * getModelDescriptor();

    openfluid::machine::ArrayOfModelItemInstance m_AllFctContainers;

    ModelColumns m_Columns;

    Glib::RefPtr<Gtk::ListStore> mp_TreeModelUsedFct;

    Gtk::TreeView * mp_TreeViewUsedFct;

    Gtk::Image * mp_ImageModelUsedFctTrash;

    Gtk::Notebook * mp_NotebookParams;

    ModelGlobalParams * mp_ModelGlobalParams;

    Glib::RefPtr<Gtk::ListStore> createTreeModelUsedFct(openfluid::machine::ModelInstance & ModelInstance);

    void initTreeViewUsedFct(std::list<Gtk::TargetEntry> ListTargets);

    void onSourceDragDataGet(const Glib::RefPtr< Gdk::DragContext >& /*context*/,
        Gtk::SelectionData& selection_data, guint /*info*/, guint /*time*/);

    void onDestDragDataReceived(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
        const Gtk::SelectionData& selection_data, guint /*info*/, guint time);

    void onTrashDragDataReceived(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
        const Gtk::SelectionData& /*selection_data*/, guint /*info*/, guint time);

    void addAFunction(Glib::ustring Id, Gtk::TreeModel::Row & RowDest);

    void moveAFunction(Gtk::TreeModel::iterator & IterSrc, Gtk::TreeModel::iterator & IterDest);

    void removeSelectedUsedFunction();

    void createParamTab(Gtk::TreeModel::Row Row, std::vector<openfluid::base::SignatureHandledDataItem> AvailParams, int Position);

    bool onEntryFocusOut(GdkEventFocus * /*Event*/, Glib::ustring ParamName, Gtk::Entry * Entry, Gtk::TreeModel::Row Row);

    void onCheckToggled(Glib::ustring ParamName, Gtk::Entry * Entry, Gtk::CheckButton * Check, Gtk::TreeModel::Row Row);

    /* Temporary function, to be removed - Print to console used functions list information */
    void tempCheckModel();

};


#endif /* __MODELUSEDFCT_HPP__ */
