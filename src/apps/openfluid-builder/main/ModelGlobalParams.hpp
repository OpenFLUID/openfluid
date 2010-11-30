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
 \file ModelGlobalParams.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */


#ifndef __MODELGLOBALPARAMS_HPP__
#define __MODELGLOBALPARAMS_HPP__

#include <gtkmm.h>


// =====================================================================
// =====================================================================


class ModelGlobalParams
{
  public:

    ModelGlobalParams(Glib::RefPtr<Gtk::Builder> GladeBuilder);

    ~ModelGlobalParams();

    void addParamRequest(Glib::ustring ParamId, Glib::ustring ParamUnit, Gtk::Label * GlobalLabel);

    void removeParamRequest(std::vector<Gtk::Label *> GlobalParamsLabels);

    Glib::ustring getGlobalParamValue(Glib::ustring ParamId/*, Glib::ustring ParamUnit*/);

    /* Temporary function, to be removed - Print to console used functions list information */
    void tempCheckModel();

  private:

     class GlobalAvailColumns : public Gtk::TreeModel::ColumnRecord
     {
       public:

       GlobalAvailColumns()
       { add(m_Id); add(m_Unit); add(m_Labels); add(m_IsNotUsed); add(m_TableRowWidgets); add(m_Value); }

       Gtk::TreeModelColumn<Glib::ustring> m_Id;
       Gtk::TreeModelColumn<Glib::ustring> m_Unit;
       Gtk::TreeModelColumn<std::vector<Gtk::Label *> > m_Labels;
       Gtk::TreeModelColumn<bool> m_IsNotUsed; //and not the opposite, because of TreeModel::set_visible_column()
       Gtk::TreeModelColumn<std::vector<Gtk::Widget *> > m_TableRowWidgets;
       Gtk::TreeModelColumn<Glib::ustring> m_Value;
     };

     GlobalAvailColumns m_GlobalAvailColumns;

     Gtk::ComboBox * mp_Combo;

     Gtk::Table * mp_Table;

     Glib::RefPtr<Gtk::TreeStore> mp_TreeModelParams;

     Glib::RefPtr<Gtk::TreeModelFilter> mp_TreeModelFilterCombo;


     Gtk::TreeIter getParamIter(Glib::ustring ParamId, Glib::ustring ParamUnit="");

     void setGlobalParamUsed(Gtk::TreeRowReference RefRow);

     void setGlobalParamUnused(Gtk::TreeRowReference RefRow);

     void removeGlobalParams(std::vector<Gtk::TreeRowReference> ParamsToDel);

     void createTableRowWidgets(Gtk::TreeRowReference RefParam);

     void removeTableRowWidgets(Gtk::TreeModel::iterator & Iter);

     void updateComboState();

     void setGlobalLabelValue(Gtk::Label * GlobalLabel, Glib::ustring GlobalValue);

     void onButtonAddClicked();

     void onButtonSupprClicked(Gtk::TreeRowReference RowRef);

     bool onEntryFocusOut(GdkEventFocus * /*Event*/, Gtk::Entry * Entry, Gtk::TreeRowReference RefRow);

     void onEntryActivate(Gtk::Entry * Entry, Gtk::TreeRowReference RefRow);

};


#endif /* __MODELGLOBALPARAMS_HPP__ */
