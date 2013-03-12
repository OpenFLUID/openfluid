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
 \file MonitoringEditParamsDialog.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef MONITORINGEDITPARAMSDIALOG_HPP_
#define MONITORINGEDITPARAMSDIALOG_HPP_

#include <gtkmm/dialog.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/table.h>
#include <gtkmm/scrolledwindow.h>
#include <openfluid/guicommon/BuilderMonitoring.hpp>
#include "BuilderTableRowWidget.hpp"

// =====================================================================
// =====================================================================

class MonitoringParamRow: public BuilderTableRowWidget
{
  private:

    const int m_Index;

    Gtk::Entry* mp_ParamValueEntry;
    Gtk::Entry* mp_ParamNameEntry;

    Gtk::Button* mp_RemoveButton;

    sigc::signal<void, int> m_signal_removeAsked;

    void onRemoveButtonClicked();

  public:

    MonitoringParamRow(std::string ParamName, std::string ParamValue,
                       const int Index);

    std::string getName();
    std::string getValue();

    sigc::signal<void, int> signal_removeAsked();
};

// =====================================================================
// =====================================================================

class MonitoringEditParamsDialog
{
  private:

    openfluid::guicommon::BuilderMonitoring& m_Monit;

    Gtk::Dialog* mp_Dialog;

    Gtk::Label* mp_IdLabel;

    Gtk::Label* mp_DescLabel;

    Gtk::Table* mp_Table;

    std::map<int, std::pair<std::string, std::string> > m_ParamsByIndex;

    unsigned int m_CurrentTableBottom;

    std::map<int, MonitoringParamRow*> m_ParamRowsByIndex;

    void init(std::string ObserverID);

    void updateRows();

    void onAddButtonClicked();

    void onRemoveAsked(int Index);

  public:

    MonitoringEditParamsDialog(openfluid::guicommon::BuilderMonitoring& Monit);

    ~MonitoringEditParamsDialog();

    bool show(std::string ObserverID);

    openfluid::ware::WareParams_t getParams();
};

#endif /* MONITORINGEDITPARAMSDIALOG_HPP_ */
