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


// =====================================================================
// =====================================================================


class ModelAvailFct
{
  public:

    ModelAvailFct(Glib::RefPtr<Gtk::Builder> GladeBuilder);

    ~ModelAvailFct();

  private:

    Gtk::TreeView * mp_TreeViewAvailFct;
    Gtk::TreeView * mp_TreeViewFctVars;

    Glib::RefPtr<Gtk::TreeStore> mp_TreeModelAvailFct;

    // Available functions tree model columns
    class ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
      public:
      ModelColumns()
        { add(m_FunctionId); add(m_FunctionName); add(m_FunctionPath); add(m_FunctionDescription);
          add(m_FunctionVersion); add(m_FunctionStatusStr);add(m_FunctionStatusIcon); add(m_FunctionDomain);
          add(m_FunctionProcess); add(m_FunctionMethod); add(m_FunctionAuthorName); add(m_FunctionAuthorEmail);
          add(m_FunctionParams); add(m_FunctionProducedVars); add(m_FunctionUpdatedVars);
          add(m_FunctionRequiredVars); add(m_FunctionUsedVars); add(m_FunctionRequiredPrevVars);
          add(m_FunctionUsedPrevVars);}

      Gtk::TreeModelColumn<Glib::ustring> m_FunctionId;
      Gtk::TreeModelColumn<Glib::ustring> m_FunctionName;
      Gtk::TreeModelColumn<Glib::ustring> m_FunctionPath;
      Gtk::TreeModelColumn<Glib::ustring> m_FunctionDescription;

      Gtk::TreeModelColumn<Glib::ustring> m_FunctionVersion;
      Gtk::TreeModelColumn<Glib::ustring> m_FunctionStatusStr;
      Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_FunctionStatusIcon;

      Gtk::TreeModelColumn<Glib::ustring> m_FunctionDomain;
      Gtk::TreeModelColumn<Glib::ustring> m_FunctionProcess;
      Gtk::TreeModelColumn<Glib::ustring> m_FunctionMethod;
      Gtk::TreeModelColumn<Glib::ustring> m_FunctionAuthorName;
      Gtk::TreeModelColumn<Glib::ustring> m_FunctionAuthorEmail;

      Gtk::TreeModelColumn<std::vector<openfluid::base::SignatureHandledDataItem> > m_FunctionParams;
      Gtk::TreeModelColumn<std::vector<openfluid::base::SignatureHandledDataItem> > m_FunctionProducedVars;
      Gtk::TreeModelColumn<std::vector<openfluid::base::SignatureHandledDataItem> > m_FunctionUpdatedVars;
      Gtk::TreeModelColumn<std::vector<openfluid::base::SignatureHandledDataItem> > m_FunctionRequiredVars;
      Gtk::TreeModelColumn<std::vector<openfluid::base::SignatureHandledDataItem> > m_FunctionUsedVars;
      Gtk::TreeModelColumn<std::vector<openfluid::base::SignatureHandledDataItem> > m_FunctionRequiredPrevVars;
      Gtk::TreeModelColumn<std::vector<openfluid::base::SignatureHandledDataItem> > m_FunctionUsedPrevVars;

    };


    ModelColumns m_Columns;


    // Functions Variables tree model columns
    class ModelColumnsVariables : public Gtk::TreeModel::ColumnRecord
    {
      public:
      ModelColumnsVariables()
      { add(m_FctVarName); add(m_FctVarUnit);
        add(m_FctVarUnitClass); add(m_FctVarDesc); }

      Gtk::TreeModelColumn<Glib::ustring> m_FctVarName;
      Gtk::TreeModelColumn<Glib::ustring> m_FctVarUnit;
      Gtk::TreeModelColumn<Glib::ustring> m_FctVarUnitClass;
      Gtk::TreeModelColumn<Glib::ustring> m_FctVarDesc;
     };

    ModelColumnsVariables m_ColumnsVar;

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

    Gtk::Table * mp_TableAvailFctParams;
    Gtk::Label * mp_LabelAvailFctParamsEmpty;


    bool isFunctionRowSelectable(const Glib::RefPtr<Gtk::TreeModel>& model, const Gtk::TreeModel::Path& path, bool path_currently_selected);

    void onAvalaibleFunctionSelected();

    void createFctVarsTree(Glib::RefPtr<Gtk::TreeStore> TreeModel, Glib::ustring Title, std::vector<openfluid::base::SignatureHandledDataItem> Vars);

};


// =====================================================================
// =====================================================================





#endif /* __MODELAVAILFCT_HPP__ */
