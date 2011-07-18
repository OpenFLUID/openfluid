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
 \file DomainUnitAddEditDialog.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __DOMAINUNITADDEDITDIALOG_HPP__
#define __DOMAINUNITADDEDITDIALOG_HPP__

#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/infobar.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/table.h>
#include <gtkmm/comboboxentrytext.h>

#include <openfluid/core/CoreRepository.hpp>

#include <set>

#include "DomainUnitRelationWidget.hpp"

class DomainUnitRelationAddDialog;

// =====================================================================
// =====================================================================


class DomainUnitAddEditDialog
{
  private:

    Gtk::Dialog* mp_Dialog;

    Gtk::Table* mp_InfoTable;

    Gtk::InfoBar* mp_InfoBar;

    Gtk::Label* mp_InfoBarLabel;

    Gtk::Label* mp_ClassLabel;

    Gtk::Label* mp_IdLabel;

    Gtk::ComboBoxEntryText* mp_ClassComboEntryText;

    Gtk::SpinButton* mp_IdSpin;

    Gtk::SpinButton* mp_PcsOrderSpin;

    openfluid::core::CoreRepository* mp_CoreRepos;

    std::set<int> m_IDs;

    DomainUnitRelationAddDialog& m_AddDialog;

    std::set<std::string> m_Classes;

  protected:

    openfluid::core::Unit* mp_Unit;

    DomainUnitRelationWidget* mp_FromWidget;
    DomainUnitRelationWidget* mp_ToWidget;
    DomainUnitRelationWidget* mp_ParentWidget;
    DomainUnitRelationWidget* mp_ChildWidget;

    void initEditionMode();

    void initCreationMode(std::string SelectedClass);

    void clearAllRelations();

    void createAllRelationsFromRelationWidgets();

    void onClassChanged();

    void onIdChanged();

  public:

        DomainUnitAddEditDialog(
            DomainUnitRelationAddDialog& UnitRelationAddDialog);

    void setEngineRequirements(openfluid::core::CoreRepository& CoreRepos);

    void update();

    openfluid::core::Unit* show(std::string SelectedClass = "",
        openfluid::core::Unit* Unit = 0);

};

// =====================================================================
// =====================================================================


class DomainUnitAddEditDialogSub: DomainUnitAddEditDialog
{
  public:

    DomainUnitAddEditDialogSub(
        DomainUnitRelationAddDialog& UnitRelationAddDialog) :
      DomainUnitAddEditDialog(UnitRelationAddDialog)
    {
    }

    void setEngineRequirements(openfluid::core::CoreRepository& CoreRepos)
    {
      DomainUnitAddEditDialog::setEngineRequirements(CoreRepos);
    }

    void initEditionMode()
    {
      DomainUnitAddEditDialog::initEditionMode();
    }

    void initCreationMode(std::string SelectedClass)
    {
      DomainUnitAddEditDialog::initCreationMode(SelectedClass);
    }

    void clearAllRelations()
    {
      DomainUnitAddEditDialog::clearAllRelations();
    }

    void createAllRelations()
    {
      DomainUnitAddEditDialog::createAllRelationsFromRelationWidgets();
    }

    void setUnit(openfluid::core::Unit* Unit)
    {
      mp_Unit = Unit;
    }

    openfluid::core::Unit* getUnit()
    {
      return mp_Unit;
    }

    DomainUnitRelationWidgetSub* getFromWidget()
    {
      return (DomainUnitRelationWidgetSub*) mp_FromWidget;
    }

    DomainUnitRelationWidgetSub* getToWidget()
    {
      return (DomainUnitRelationWidgetSub*) mp_ToWidget;
    }

    DomainUnitRelationWidgetSub* getParentWidget()
    {
      return (DomainUnitRelationWidgetSub*) mp_ParentWidget;
    }

    DomainUnitRelationWidgetSub* getChildWidget()
    {
      return (DomainUnitRelationWidgetSub*) mp_ChildWidget;
    }

};

#endif /* __DOMAINUNITADDEDITDIALOG_HPP__ */
