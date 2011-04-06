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

#ifndef SIMULOUTSETDESCVIEW_HPP_
#define SIMULOUTSETDESCVIEW_HPP_

#include <iostream>

#include <sigc++/sigc++.h>

#include <gtkmm.h>

#include "SimulOutSetDescColumns.hpp"

class SimulOutSetDescView
{
  public:

    virtual sigc::signal<void> signal_NameChanged() = 0;

    virtual sigc::signal<void> signal_ClassChanged() = 0;

    virtual sigc::signal<void> signal_IDsChanged() = 0;

    virtual sigc::signal<void> signal_VarsChanged() = 0;

    virtual sigc::signal<void> signal_FormatNameChanged() = 0;

    virtual sigc::signal<void> signal_PrecisionChanged() = 0;

    virtual sigc::signal<void> signal_SaveAsked() = 0;

    virtual sigc::signal<void> signal_CancelAsked() = 0;

    virtual Gtk::Widget* asWidget() = 0;

    virtual void setCreationMode() = 0;

    virtual void setEditionMode() = 0;

    virtual void showDialog() = 0;

    virtual void setFormatNames(std::vector<std::string> FormatNames) = 0;

    virtual void setClasses(std::vector<std::string> Classes) = 0;

    virtual void setByClassIDs(
        std::map<std::string, std::vector<unsigned int> > IDsByClass) = 0;

    virtual void
        setByClassVars(
            std::map<std::string, std::vector<std::string> > VarsByClass) = 0;

    virtual void setName(std::string Value) = 0;

    virtual void setClass(std::string Value) = 0;

    virtual void setFormatName(std::string Value) = 0;

    virtual void setPrecision(int Value) = 0;

    virtual void setIDs(std::vector<unsigned int> IDs) = 0;

    virtual void setVars(std::vector<std::string> Vars) = 0;

    virtual std::string getName() = 0;

    virtual std::string getClass() = 0;

    virtual std::string getFormatName() = 0;

    virtual int getPrecision() = 0;

    virtual std::vector<unsigned int> getIDs() = 0;

    virtual std::vector<std::string> getVars() = 0;

    virtual void closeDialog() = 0;

    virtual void showErrorMessageDialog(std::string MessageText) = 0;

};


class SimulOutSetDescViewImpl: public SimulOutSetDescView
{
  private:

    sigc::signal<void> m_signal_NameChanged;

    sigc::signal<void> m_signal_ClassChanged;

    sigc::signal<void> m_signal_IDsChanged;

    sigc::signal<void> m_signal_VarsChanged;

    sigc::signal<void> m_signal_FormatNameChanged;

    sigc::signal<void> m_signal_PrecisionChanged;

    sigc::signal<void> m_signal_SaveAsked;

    sigc::signal<void> m_signal_CancelAsked;

    SimulOutSetDescColumns m_Columns;

    Gtk::Dialog* mp_Dialog;

    Gtk::Table* mp_Table;

    bool m_isInCreationMode;

    std::map<std::string, Glib::RefPtr<Gtk::ListStore> > m_ByClassIDsListStores;

    std::map<std::string, Glib::RefPtr<Gtk::ListStore> >
        m_ByClassVarsListStores;

    std::vector<unsigned int> m_IDs;

    std::vector<std::string> m_Vars;

    bool m_HaveIDsToBeStored;

    bool m_HaveVarsToBeStored;

    bool IDsListStoreExistsForCurrentClass();

    bool VarsListStoreExistsForCurrentClass();

    void selectIDs();

    void selectVars();

    void onNameChanged();

    void onClassChanged();

    void selectedIdCallback(const Gtk::TreeModel::iterator& Iter);

    void selectedVarCallback(const Gtk::TreeModel::iterator& Iter);

    void onIDsChanged();

    void onVarsChanged();

    void onFormatNameChanged();

    void onPrecisionChanged();

    void onOkButtonClicked();

    void closeDialog();


  protected:

    Gtk::Entry* mp_SetNameEntry;

    Gtk::Label* mp_SetNameLabel;

    Gtk::ComboBoxText* mp_ClassCombo;

    Gtk::ComboBoxText* mp_FormatNameCombo;

    Gtk::SpinButton* mp_PrecisionSpin;

    Gtk::TreeView* mp_IDsTreeView;

    Gtk::TreeView* mp_VarsTreeView;

    Gtk::Button* mp_OkCreateButton;

    Gtk::Button* mp_CancelCreateButton;

    Gtk::Button* mp_CloseEditButton;


  public:

    SimulOutSetDescViewImpl();

    sigc::signal<void> signal_NameChanged();

    sigc::signal<void> signal_ClassChanged();

    sigc::signal<void> signal_IDsChanged();

    sigc::signal<void> signal_VarsChanged();

    sigc::signal<void> signal_FormatNameChanged();

    sigc::signal<void> signal_PrecisionChanged();

    sigc::signal<void> signal_SaveAsked();

    sigc::signal<void> signal_CancelAsked();

    void setCreationMode();

    void setEditionMode();

    void setFormatNames(std::vector<std::string> FormatNames);

    void setClasses(std::vector<std::string> Classes);

    void setByClassIDs(
        std::map<std::string, std::vector<unsigned int> > IDsByClass);

    void setByClassVars(
        std::map<std::string, std::vector<std::string> > VarsByClass);

    void setName(std::string Value);

    void setClass(std::string Value);

    void setFormatName(std::string Value);


    void setPrecision(int Value);

    void setIDs(std::vector<unsigned int> IDs);

    void setVars(std::vector<std::string> Vars);

    std::string getName();

    std::string getClass();

    std::string getFormatName();

    int getPrecision();

    std::vector<unsigned int> getIDs();

    std::vector<std::string> getVars();

    void showDialog();

    Gtk::Widget* asWidget();

    void showErrorMessageDialog(std::string MessageText);

};


class SimulOutSetDescViewSub: public SimulOutSetDescViewImpl
{
  public:

    Gtk::TreeView* getIDsTreeView();

    Gtk::TreeView* getVarsTreeView();
};

#endif /* SIMULOUTSETDESCVIEW_HPP_ */
