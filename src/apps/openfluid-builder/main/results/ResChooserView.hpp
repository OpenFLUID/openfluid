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
 \file ResChooserView.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __RESCHOOSERVIEW_HPP__
#define __RESCHOOSERVIEW_HPP__


#include <sigc++/sigc++.h>

#include <gtkmm.h>

#include "SimulOutSetDescColumns.hpp"

class ResChooserView
{
  public:
    virtual sigc::signal<void> signal_SelectionChanged() = 0;
    virtual sigc::signal<void> signal_SelectionValidated() = 0;
    virtual void setBySetNameClassNames(
        std::map<std::string, std::string> ClassNames) = 0;
    virtual void setBySetNameIDs(std::map<std::string,
        std::vector<unsigned int> > IDsBySet) = 0;
    virtual void selectFirstItem() = 0;
    virtual std::string getSetName() = 0;
    virtual std::string getClassName() = 0;
    virtual int getID() = 0;
    virtual Gtk::Widget* asWidget() = 0;
};

class ResChooserViewImpl: public ResChooserView
{
  private:

    class ResChooserColumns: public Gtk::TreeModel::ColumnRecord
    {
      public:
        ResChooserColumns()
        {
          add(m_ID);
        }
        Gtk::TreeModelColumn<unsigned int> m_ID;
    };
    ResChooserColumns m_Columns;
    sigc::signal<void> m_signal_SelectionChanged;
    sigc::signal<void> m_signal_SelectionValidated;
    Gtk::Box* mp_MainBox;

    bool IDsListStoreExistsForCurrentSet();
    bool ClassNameExistsForCurrentSet();
    void onSetChanged();
    void onIdChanged();
    void onOkButtonClicked();
  protected:
    Gtk::Label* mp_MessageLabel;
    Gtk::Label* mp_ClassNameValue;
    Gtk::ComboBoxText* mp_SetCombo;
    Gtk::ComboBox* mp_IdCombo;
    Gtk::Button* mp_OkButton;
    std::map<std::string, std::string> m_BySetNameClassNames;
    std::map<std::string, Glib::RefPtr<Gtk::ListStore> >
        m_BySetNameIDsListStores;
  public:
    ResChooserViewImpl();
    sigc::signal<void> signal_SelectionChanged();
    sigc::signal<void> signal_SelectionValidated();
    void setBySetNameClassNames(std::map<std::string, std::string> ClassNames);
    void setBySetNameIDs(
        std::map<std::string, std::vector<unsigned int> > IDsBySet);
    void selectFirstItem();
    std::string getSetName();
    std::string getClassName();
    int getID();
    Gtk::Widget* asWidget();
};

class ResChooserViewSub: public ResChooserViewImpl
{
  public:
    std::string getClassName();
    void selectSetName(std::string Value);
    void selectId(int Index);
    std::map<std::string, std::string> getBySetNameClassNames();
    std::map<std::string, Glib::RefPtr<Gtk::ListStore> >
        getBySetNameIDsListStores();
};

#endif /* __RESCHOOSERVIEW_HPP__ */
