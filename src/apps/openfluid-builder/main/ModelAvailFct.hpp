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
 \file ModelAvailFct.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */


#ifndef __MODELAVAILFCT_HPP__
#define __MODELAVAILFCT_HPP__


#include <gtkmm.h>

#include "ModelColumns.hpp"


// =====================================================================
// =====================================================================


class ModelAvailFct
{
  public:

    ModelAvailFct(Glib::RefPtr<Gtk::Builder> GladeBuilder);

    ~ModelAvailFct();

    Gtk::TreeView * getTreeViewAvailFct()
        { return mp_TreeViewAvailFct; }


  private:

    ModelColumns m_Columns;

    Glib::RefPtr<Gtk::TreeStore> mp_MainTreeModel;
    Glib::RefPtr<Gtk::TreeModelSort> mp_TreeModelAvailFct;

    Gtk::TreeView * mp_TreeViewAvailFct;
    Gtk::TreeView * mp_TreeViewFctParameters;
    Gtk::TreeView * mp_TreeViewFctInputData;
    Gtk::TreeView * mp_TreeViewFctVars;
    Gtk::TreeView * mp_TreeViewFctEvents;
    Gtk::TreeView * mp_TreeViewFctExtraFiles;
    Gtk::TreeView * mp_TreeViewFctUnitsGraph;

    Gtk::Table * mp_TableSignatureFunction;
    Gtk::Table * mp_TableSignatureGenerator;

    Gtk::Label * mp_LabelAvailFctId;
    Gtk::Label * mp_LabelAvailFctName;
    Gtk::Label * mp_LabelAvailFctPath;
    Gtk::Label * mp_LabelAvailFctDescription;
    Gtk::Label * mp_LabelAvailFctVersion;
    Gtk::Label * mp_LabelAvailFctStatus;
    Gtk::Label * mp_LabelAvailFctDomain;
    Gtk::Label * mp_LabelAvailFctProcess;
    Gtk::Label * mp_LabelAvailFctMethod;
    Gtk::Label * mp_LabelAvailFctAuthorName;
    Gtk::Label * mp_LabelAvailFctAuthorEmail;

    Gtk::Label * mp_LabelGeneratorId;
    Gtk::Label * mp_LabelGeneratorName;
    Gtk::Label * mp_LabelGeneratorDescription;

    Gtk::Notebook * mp_NotebookAvailFct;


    Glib::RefPtr<Gtk::TreeStore> createMainTreeModel();

    Glib::RefPtr<Gtk::TreeModelSort> createAvailableFunctionsTreeModel(Glib::RefPtr<Gtk::TreeStore> ModelBase);

    void initAvailableFunctionsTreeView();

    void initSelectedFunctionTreeViews();

    void setTreeViewProperties(Gtk::TreeView * TreeView);

    void createHandleDataTreeRows(Glib::RefPtr<Gtk::TreeStore> Model,Gtk::TreeModel::Row * PrevRow, Glib::ustring Title, ModelColumns::RowType Type, std::vector<openfluid::base::SignatureHandledDataItem> Vars, bool ShowTitle = true);

    void setRowTitle(Gtk::TreeModel::Row * Row, Glib::ustring Title, ModelColumns::RowType Type, bool ShowTitle = true);

    bool isRowVisible(const Gtk::TreeModel::const_iterator& Iter, std::vector<ModelColumns::RowType> Type);

    bool isFunctionRowSelectable(const Glib::RefPtr<Gtk::TreeModel>& model, const Gtk::TreeModel::Path& path, bool path_currently_selected);

    void onAvalaibleFunctionSelected();

    void createSelectedFunctionTreeModel(Gtk::TreeView * TreeView, const Gtk::TreeRow & TreeRow, ModelColumns::RowType Type);

    void setHandleDataPanel(ModelColumns::RowType Type, Gtk::TreeView * TreeView, const Gtk::TreeRow & FunctionIter);

    void toggleEmptyTreeView(Gtk::TreeView * TreeView, ModelColumns::RowType Type, bool TypeIsEmpty);

    void onSourceDragDataGet(const Glib::RefPtr< Gdk::DragContext >& /*context*/,
        Gtk::SelectionData& selection_data, guint /*info*/, guint /*time*/);
};

#endif /* __MODELAVAILFCT_HPP__ */
