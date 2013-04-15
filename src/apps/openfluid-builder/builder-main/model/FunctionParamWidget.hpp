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
 \file FunctionParamWidget
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef FUNCTIONPARAMWIDGET_HPP_
#define FUNCTIONPARAMWIDGET_HPP_

#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/table.h>
#include "BuilderTableRowWidget.hpp"
#include "FunctionAddParamDialog.hpp"

namespace openfluid {
namespace fluidx {
class ModelItemDescriptor;
}
namespace machine {
class ModelItemSignatureInstance;
}
}

// =====================================================================
// =====================================================================

class FunctionParamRow: public BuilderTableRowWidget
{
  private:

    openfluid::fluidx::ModelItemDescriptor& m_FctDesc;
    std::string m_Name;

    Gtk::Entry* mp_ValueEntry;

    Gtk::Label* mp_GlobalLabel;

    Gtk::Button* mp_RemoveButton;

    sigc::signal<void> m_signal_removeOccured;
    sigc::signal<void> m_signal_valueChangeOccured;

    void onRemoveButtonClicked();

    void onValueChanged();

  public:

    FunctionParamRow(openfluid::fluidx::ModelItemDescriptor& FctDesc,
                     std::string ParamName, std::string ParamValue,
                     std::string ParamUnit, bool WithRemoveBt);

    void setGlobalValue(std::string GlobalValue);

    sigc::signal<void> signal_removeOccured();
    sigc::signal<void> signal_valueChangeOccured();
};

// =====================================================================
// =====================================================================

class FunctionParamWidget: public Gtk::VBox
{
  private:

    openfluid::fluidx::ModelItemDescriptor& m_FctDesc;
    openfluid::machine::ModelItemSignatureInstance* mp_Sign;

    FunctionAddParamDialog& m_AddParamDialog;

    Gtk::Table* mp_Table;

    unsigned int m_CurrentTableBottom;

    std::map<std::string, FunctionParamRow*> m_Rows;

    std::map<std::string, std::string> m_Globals;

    sigc::signal<void> m_signal_changeOccured;

    void updateRows();
    void attachRow(FunctionParamRow* Row, std::string ParamName);

    void onAddButtonClicked();

    void onValueChangeOccured();
    void onStructureChangeOccured();

    void updateGlobals();

  public:

    FunctionParamWidget(openfluid::fluidx::ModelItemDescriptor& FctDesc,
                        openfluid::machine::ModelItemSignatureInstance* Sign,
                        FunctionAddParamDialog& AddParamDialog);

    ~FunctionParamWidget();

    void updateGlobals(const std::map<std::string, std::string>& GlobalParams);

    sigc::signal<void> signal_changeOccured();
};

#endif /* FUNCTIONPARAMWIDGET_HPP_ */
