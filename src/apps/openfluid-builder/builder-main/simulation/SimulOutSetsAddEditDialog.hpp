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
 \file SimulOutSetsAddEditDialog.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __SIMULOUTSETSADDEDITDIALOG_HPP__
#define __SIMULOUTSETSADDEDITDIALOG_HPP__

#include <gtkmm/dialog.h>
#include <gtkmm/table.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/treeview.h>
#include <gtkmm/infobar.h>
#include <gtkmm/liststore.h>

#include <set>

#include <openfluid/base/OutputDescriptor.hpp>
#include <openfluid/base/OutputSetDescriptor.hpp>
#include <openfluid/core/CoreRepository.hpp>
#include <openfluid/machine/ModelInstance.hpp>

class SimulOutSetsAddEditDialog
{
  private:

    Gtk::Dialog* mp_Dialog;

    Gtk::Table* mp_Table;

    Gtk::InfoBar* mp_InfoBar;
    Gtk::Label* mp_InfoBarLabel;

    Gtk::Entry* mp_SetNameEntry;

    Gtk::ComboBoxText* mp_ClassCombo;

    class IdsColumns: public Gtk::TreeModel::ColumnRecord
    {
      public:
        IdsColumns()
        {
          add(m_Value);
        }
        Gtk::TreeModelColumn<int> m_Value;
    };

    class VarsColumns: public Gtk::TreeModel::ColumnRecord
    {
      public:
        VarsColumns()
        {
          add(m_Value);
          add(m_IsVector);
        }
        Gtk::TreeModelColumn<std::string> m_Value;
        Gtk::TreeModelColumn<bool> m_IsVector;
    };

    IdsColumns m_IdsColumns;
    VarsColumns m_VarsColumns;

    Glib::RefPtr<Gtk::ListStore> mref_IdsModel;
    Glib::RefPtr<Gtk::ListStore> mref_VarsModel;

    Gtk::TreeView* mp_IdsTreeView;
    Gtk::TreeView* mp_VarsTreeView;

    Gtk::ComboBoxText* mp_FormatNameCombo;

    Gtk::SpinButton* mp_PrecisionSpin;

    openfluid::base::OutputDescriptor* mp_OutDesc;
    openfluid::core::CoreRepository* mp_CoreRepos;
    openfluid::machine::ModelInstance* mp_ModelInstance;

    openfluid::base::OutputSetDescriptor* mp_OutSetDesc;

    std::set<std::string> m_SetNames;

    bool m_ClassesHaveToUpdate;

    std::string m_EditionOriginalName;

    void createIdRows(std::string ClassName);

    void selectIDRows();

    void createVarRows(std::string ClassName);

    void selectVarRows();

    void initEditionMode();

    void setNewSetIDs(openfluid::base::OutputSetDescriptor* NewOutSetDesc);

    void setNewSetVars(openfluid::base::OutputSetDescriptor* NewOutSetDesc);

    void initAdditionMode();

    void onNameChanged();

    void onClassChanged();

  public:

    SimulOutSetsAddEditDialog();

    void setEngineRequirements(openfluid::base::OutputDescriptor& OutDesc,
        openfluid::core::CoreRepository& CoreRepos,
        openfluid::machine::ModelInstance& ModelInstance);

    void update();

    std::pair<openfluid::base::OutputSetDescriptor*, std::string> show(
        openfluid::base::OutputSetDescriptor* OutSetDesc = 0);
};

#endif /* __SIMULOUTSETSADDEDITDIALOG_HPP__ */
