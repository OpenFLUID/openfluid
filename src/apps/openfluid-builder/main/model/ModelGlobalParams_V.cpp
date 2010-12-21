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
  \file ModelGlobalParams_V.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */


#include <iostream>

#include <glibmm/i18n.h>

//#include "ModelModule.hpp"
#include "ModelGlobalParams.hpp"
#include "ModelGlobalParams_V.hpp"


// =====================================================================
// =====================================================================


ModelGlobalParams_V::ModelGlobalParams_V(ModelGlobalParams * Control, Glib::RefPtr<Gtk::Builder> GladeBuilder)
: mp_Control(Control)
{
  GladeBuilder->get_widget("ComboGlobal",mp_Combo);
  GladeBuilder->get_widget("ButtonAddGlobal",mp_ButtonAdd);
  GladeBuilder->get_widget("TableGlobal",mp_Table);

//  ModelModule::getGladeBuilder()->get_widget("ComboGlobal",mp_Combo);
//  ModelModule::getGladeBuilder()->get_widget("ButtonAddGlobal",mp_ButtonAdd);
//  ModelModule::getGladeBuilder()->get_widget("TableGlobal",mp_Table);

  mp_TreeModel = Gtk::ListStore::create(m_GlobalAvailColumns);

  mp_TreeModel->set_sort_column(m_GlobalAvailColumns.m_Id,Gtk::SORT_ASCENDING);


  mp_TreeModelToBeListed = Gtk::TreeModelFilter::create(mp_TreeModel);

  mp_TreeModelToBeListed->set_visible_column(m_GlobalAvailColumns.m_ToBeListed);


  mp_Combo->set_model(mp_TreeModelToBeListed);

  mp_Combo->pack_start(m_GlobalAvailColumns.m_Id);
  mp_Combo->pack_start(m_GlobalAvailColumns.m_Unit);


  mp_ButtonAdd->signal_clicked().connect(
      sigc::mem_fun(*this,&ModelGlobalParams_V::onButtonAddClicked));
}


// =====================================================================
// =====================================================================


void ModelGlobalParams_V::addEntry(Glib::ustring Key, Glib::ustring DataUnit, bool ToBeListed)
{
  Gtk::TreeRow Row = *(mp_TreeModel->append());

  Row[m_GlobalAvailColumns.m_Id] = Key;
  Row[m_GlobalAvailColumns.m_Unit] = DataUnit;
  Row[m_GlobalAvailColumns.m_ToBeListed] = ToBeListed;
}


// =====================================================================
// =====================================================================


void ModelGlobalParams_V::removeEntry(Glib::ustring Key)
{
  mp_TreeModel->erase(getIteratorFromKey(Key));
}


// =====================================================================
// =====================================================================


void ModelGlobalParams_V::addWidgetRow(std::vector<Gtk::Widget *> WidgetVector)
{
  Gtk::Table_Helpers::TableList List = mp_Table->children();

  Gtk::Table_Helpers::TableList::iterator IteTable;

  int top = 0;

  if(!List.empty())
    top = List.begin()->get_bottom_attach(); // first widget in TableList is the last attached

  for(unsigned int left=0 ; left<WidgetVector.size() ; left++)
    mp_Table->attach(*WidgetVector[left],left,left+1,top,top+1,Gtk::FILL,Gtk::FILL,5,5);
}


// =====================================================================
// =====================================================================


void ModelGlobalParams_V::removeWidgetRow(std::vector<Gtk::Widget *> WidgetVector)
{
  for(unsigned int i=0 ; i<WidgetVector.size() ; i++)
  {
      mp_Table->remove(*WidgetVector[i]);
  }
}


// =====================================================================
// =====================================================================


void ModelGlobalParams_V::setToBeListed(Glib::ustring Key, bool ToBeListed)
{
  Gtk::TreeModel::iterator it = getIteratorFromKey(Key);

  it->set_value(m_GlobalAvailColumns.m_ToBeListed,ToBeListed);
}


// =====================================================================
// =====================================================================


void ModelGlobalParams_V::updateCombo()
{
  mp_TreeModelToBeListed->refilter();

  setComboSensitive(!mp_TreeModelToBeListed->children().empty());
}


// =====================================================================
// =====================================================================


void ModelGlobalParams_V::onButtonAddClicked()
{
  Gtk::TreeModel::iterator it = mp_Combo->get_active();

  if(it)
    mp_Control->V_globalParamActivated(getKeyFromIterator(it));
}


// =====================================================================
// =====================================================================


void ModelGlobalParams_V::setComboSensitive(bool Sensitive)
{
  mp_Combo->set_sensitive(Sensitive);

  if(Sensitive)
    mp_Combo->set_active(0);
}


// =====================================================================
// =====================================================================


Gtk::TreeModel::iterator ModelGlobalParams_V::getIteratorFromKey(Glib::ustring Key)
{
  for(Gtk::TreeModel::iterator it=mp_TreeModel->children().begin() ; it !=mp_TreeModel->children().end() ; ++it)
  {
    if(it->get_value(m_GlobalAvailColumns.m_Id) == Key)
      return it;
  }

  throw std::out_of_range("ModelGlobalParams_V::getIteratorFromKey : invalid Key");
}


// =====================================================================
// =====================================================================


Glib::ustring ModelGlobalParams_V::getKeyFromIterator(Gtk::TreeModel::iterator Iter)
{
  return Iter->get_value(m_GlobalAvailColumns.m_Id);
}
