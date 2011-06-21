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
 \file ModelGlobalParamsView.h
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __MODELGLOBALPARAMSVIEW_H__
#define __MODELGLOBALPARAMSVIEW_H__

#include <sigc++/sigc++.h>
#include <set>

#include <gtkmm.h>

class ModelGlobalParamRow;

class ModelGlobalParamsView
{
  public:

    virtual sigc::signal<void, std::string> signal_GlobalValueChanged() = 0;

    virtual sigc::signal<void, std::string> signal_GlobalParamSetAsked() = 0;

    virtual sigc::signal<void, std::string> signal_GlobalParamUnsetAsked() = 0;

    virtual void setComboParams(std::set<std::string> Params) = 0;

    virtual void setGlobalParamsRow(std::map<std::string, std::pair<
        std::string, std::string> > GlobalParams) = 0;

    virtual void addGlobalParamsRow(std::string ParamName,
        std::string ParamUnit) = 0;

    virtual std::string getGlobalValue(std::string ParamName) = 0;

    virtual Gtk::Widget* asWidget() = 0;

};

class ModelGlobalParamsViewImpl: public ModelGlobalParamsView
{
  private:

    sigc::signal<void, std::string> m_signal_GlobalValueChanged;

    sigc::signal<void, std::string> m_signal_GlobalParamSetAsked;

    sigc::signal<void, std::string> m_signal_GlobalParamUnsetAsked;

    Gtk::Box* mp_MainBox;

    Gtk::ComboBoxText* mp_Combo;

    Gtk::Button* mp_AddButton;

    Gtk::Table* mp_Table;

    unsigned int m_CurrentTableBottom;

    std::map<std::string, ModelGlobalParamRow*> m_ByParamNameParamRow;

    void onGlobalValueChanged(std::string ParamName);

    void onAddButtonClicked();

  public:

    ModelGlobalParamsViewImpl();

    void setComboParams(std::set<std::string> Params);

    void setGlobalParamsRow(std::map<std::string, std::pair<std::string,
        std::string> > GlobalParams);

    void addGlobalParamsRow(std::string ParamName, std::string ParamUnit);

    void onRowRemoveAsked(std::string ParamName);

    sigc::signal<void, std::string> signal_GlobalValueChanged();

    sigc::signal<void, std::string> signal_GlobalParamSetAsked();

    sigc::signal<void, std::string> signal_GlobalParamUnsetAsked();

    std::string getGlobalValue(std::string ParamName);

    Gtk::Widget* asWidget();

};

#endif /* __MODELGLOBALPARAMSVIEW_H__ */
