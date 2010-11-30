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
 \file SimulationOutput.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */


#ifndef __SIMULATIONOUTPUT_HPP__
#define __SIMULATIONOUTPUT_HPP__


#include <gtkmm.h>

#include <openfluid/base.hpp>
#include <openfluid/core.hpp>


// =====================================================================
// =====================================================================


typedef std::map<std::string,std::vector<std::string> > ItemsStrByClassMap_t;


class SimulationOutput
{
  public:

    SimulationOutput(Glib::RefPtr<Gtk::Builder> GladeBuilder, openfluid::base::OutputDescriptor & OutputDesc,
        ItemsStrByClassMap_t UnitsMap, ItemsStrByClassMap_t VarsMap);

    ~SimulationOutput();


  private:

    class OutputColumns : public Gtk::TreeModel::ColumnRecord
    {
      public:
      OutputColumns()
      { add(m_FileDesc); add(m_IsAFile);
        add(m_FormatName); add(m_ColSep); add(m_DtFormat); add(m_CommentChar);
        add(m_ComboColSep); add(m_ComboDTFormat); add(m_ComboCommentChar);
        add(m_SetDesc); add(m_IsASet);
        add(m_SetName); add(m_UnitClass); add(m_UnitIDsList); add(m_VarsList); add(m_Precision);
        add(m_ComboUnitClass); }

      Gtk::TreeModelColumn<openfluid::base::OutputFilesDescriptor*> m_FileDesc;
      Gtk::TreeModelColumn<bool> m_IsAFile;
      Gtk::TreeModelColumn<Glib::ustring> m_FormatName;
      Gtk::TreeModelColumn<std::string> m_ColSep;
      Gtk::TreeModelColumn<std::string> m_DtFormat;
      Gtk::TreeModelColumn<std::string> m_CommentChar;
      Gtk::TreeModelColumn<Glib::RefPtr<Gtk::TreeModel> > m_ComboColSep;
      Gtk::TreeModelColumn<Glib::RefPtr<Gtk::TreeModel> > m_ComboDTFormat;
      Gtk::TreeModelColumn<Glib::RefPtr<Gtk::TreeModel> > m_ComboCommentChar;

      Gtk::TreeModelColumn<openfluid::base::OutputSetDescriptor*> m_SetDesc;
      Gtk::TreeModelColumn<bool> m_IsASet;
      Gtk::TreeModelColumn<std::string> m_SetName;
      Gtk::TreeModelColumn<std::string> m_UnitClass;
      Gtk::TreeModelColumn<std::string> m_UnitIDsList;
      Gtk::TreeModelColumn<std::string> m_VarsList;
      Gtk::TreeModelColumn<int> m_Precision;

      Gtk::TreeModelColumn<Glib::RefPtr<Gtk::TreeModel> > m_ComboUnitClass;
    };

    OutputColumns m_OutputColumns;

    class ComboColumns : public Gtk::TreeModel::ColumnRecord
    {
      public:
      ComboColumns() { add(m_Value); }

      Gtk::TreeModelColumn<Glib::ustring> m_Value;
    };

    ComboColumns m_ComboColumns;

    Glib::RefPtr<Gtk::ListStore> m_refColSepModel;
    Glib::RefPtr<Gtk::ListStore> m_refCommentCharModel;
    Glib::RefPtr<Gtk::ListStore> m_refDTFormatModel;

    Glib::RefPtr<Gtk::TreeStore> m_refUnitsModel;
    Glib::RefPtr<Gtk::TreeStore> m_refVarsModel;

    openfluid::base::OutputDescriptor & m_OutputDesc;

    Gtk::TreeView * mp_TreeViewOutput;

    Glib::RefPtr<Gtk::TreeStore> mp_MainTreeModel;

    Glib::RefPtr<Gtk::TreeStore> createMainTreeModel();
    void createComboFixedTreeModels();
    Glib::RefPtr<Gtk::TreeStore> createComboByClassTreeModel(ItemsStrByClassMap_t ItemsByClassMap);

    void initTreeView();

    Glib::ustring vectorToString(std::vector<std::string> vect, bool VectorValues=false);
    Glib::ustring vectorToString(std::vector<unsigned int> vect);

    bool isUnitClassVisible(const Gtk::TreeModel::const_iterator & Iter);

    void onColSepEdited(const Glib::ustring PathString, const Glib::ustring NewText);
    void onDTFormatEdited(const Glib::ustring PathString, const Glib::ustring NewText);
    void onCommentCharEdited(const Glib::ustring PathString, const Glib::ustring NewText);
    void onSetNameEdited(const Glib::ustring PathString, const Glib::ustring NewText);
    void onUnitClassEdited(const Glib::ustring PathString, const Glib::ustring NewText);
    void onUnitIDsEditingStarted(Gtk::CellEditable * Editable, const Glib::ustring & PathString);
    void onVarsEditingStarted(Gtk::CellEditable * Editable, const Glib::ustring & PathString);
    void onSetPrecisionEdited(const Glib::ustring PathString, const Glib::ustring NewText);

    std::pair<bool,std::vector<std::string> > editListInDialogBox(const Glib::ustring & PathString, Glib::ustring ColumnHeader, Glib::ustring DialogTitle,
        Glib::RefPtr<Gtk::TreeModel> ListModel, Gtk::TreeModelColumn<std::string> MainModelListColumn, bool & HasChanged);

};


#endif /* __SIMULATIONOUTPUT_HPP__ */
