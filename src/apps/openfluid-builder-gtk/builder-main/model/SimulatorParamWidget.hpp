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
 \file SimulatorParamWidget
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef SIMULATORPARAMWIDGET_HPP_
#define SIMULATORPARAMWIDGET_HPP_

#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/table.h>
#include <giomm/file.h>
#include "BuilderTableRowWidget.hpp"
#include "SimulatorAddParamDialog.hpp"

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

class SimulatorParamRow: public BuilderTableRowWidget
{
  private:

    openfluid::fluidx::ModelItemDescriptor& m_SimDesc;

    Gtk::Button* mp_RemoveButton;

    sigc::signal<void> m_signal_removeOccured;
    sigc::signal<void> m_signal_valueChangeOccured;

    void onRemoveButtonClicked();

    void onValueChanged();

  protected:

    std::string m_Name;

    Gtk::Entry* mp_ValueEntry;

    Gtk::Label* mp_GlobalLabel;

  public:

    SimulatorParamRow(openfluid::fluidx::ModelItemDescriptor& SimDesc,
                     std::string ParamName, std::string ParamValue,
                     std::string ParamUnit, std::string ParamDescription,
                     bool WithRemoveBt);

    void setGlobalValue(std::string GlobalValue);

    sigc::signal<void> signal_removeOccured();
    sigc::signal<void> signal_valueChangeOccured();
};

// =====================================================================
// =====================================================================

class SimulatorParamFileRow: public BuilderTableRowWidget
{
  private:

    std::string m_FileName;

    bool m_IsRequired;

    Glib::RefPtr<Gio::File> m_File;

    Gtk::Label* mp_FileNameLabel;

    Gtk::Button* mp_FileButton;

    sigc::signal<void> m_signal_FileChanged;

    void onFileButtonClicked();

  public:

    SimulatorParamFileRow(std::string FileName, bool IsRequired);

    void setFileFound();

    sigc::signal<void> signal_FileChanged();
};

// =====================================================================
// =====================================================================

class SimulatorParamWidget: public Gtk::VBox
{
  private:

    openfluid::fluidx::ModelItemDescriptor& m_SimDesc;
    openfluid::machine::ModelItemSignatureInstance* mp_Sign;

    SimulatorAddParamDialog& m_AddParamDialog;

    Gtk::Table* mp_ParamsTable;
    Gtk::Table* mp_RequiredFilesTable;
    Gtk::Table* mp_UsedFilesTable;

    unsigned int m_CurrentParamsTableBottom;
    unsigned int m_CurrentReqFilesTableBottom;
    unsigned int m_CurrentUsedFilesTableBottom;

    openfluid::ware::WareParams_t m_Globals;

    sigc::signal<void> m_signal_changeOccured;
    sigc::signal<void> m_signal_fileChangeOccured;

    void updateRows();

    void updateParamsRows();
    void updateUsedFilesRows();

    void attachParamsRow(SimulatorParamRow* Row, std::string ParamName);
    void attachRequiredFileRow(SimulatorParamFileRow* Row);
    void attachUsedFileRow(SimulatorParamFileRow* Row);

    void onAddButtonClicked();

    void onValueChangeOccured();
    void onStructureChangeOccured();
    void onFileChangeOccured();

    void updateGlobals();

  protected:

    std::map<std::string, SimulatorParamRow*> m_ParamsRows;

  public:

    SimulatorParamWidget(openfluid::fluidx::ModelItemDescriptor& SimDesc,
                        openfluid::machine::ModelItemSignatureInstance* Sign,
                        SimulatorAddParamDialog& AddParamDialog);

    ~SimulatorParamWidget();

    void updateGlobals(const openfluid::ware::WareParams_t& GlobalParams);
    void updateRequiredFilesRows();

    sigc::signal<void> signal_changeOccured();
    sigc::signal<void> signal_fileChangeOccured();
};

#endif /* SIMULATORPARAMWIDGET_HPP_ */
