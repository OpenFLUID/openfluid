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
  \file ModelStructureItem_V.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <iostream>

#include <glibmm/i18n.h>

#include "ModelStructureItem.hpp"
#include "ModelStructureItem_V.hpp"


// =====================================================================
// =====================================================================

/* This widget displays the function parameters list.
 * It's created, even if the function has no parameter,
 * but is not set visible, thus it inserts an empty widget into the notebook,
 * which creates a hidden page.
 * This keeps Position synchro between notebook pages and the model.
 * We set it visible when we add a parameter.
 */
ModelStructureItem_V::ModelStructureItem_V(ModelStructureItem * Control, Glib::RefPtr<Gtk::Builder> /*GladeBuilder*/)
:mp_Control(Control)
{
  set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);

  mp_Table = Gtk::manage(new Gtk::Table());

  mp_Table->set_spacings(5);
  mp_Table->set_border_width(5);

  add(*mp_Table);

  mp_Table->set_visible(true);

  m_ParamNb = 0;
}


// =====================================================================
// =====================================================================


void ModelStructureItem_V::addParamWidgetRow(std::vector<Gtk::Widget *> WidgetVector/*ModelParam_P * Param_P*/)
{
  int TopAttach = m_ParamNb*2;
  int BottomAttach = TopAttach+1;

  for(unsigned int i=0 ; i<WidgetVector.size() ; i++)
  {
    mp_Table->attach(*WidgetVector[i],i,i+1,TopAttach,BottomAttach,Gtk::FILL,Gtk::FILL);
  }

  m_ParamNb ++;

  set_visible(true); // this widget is visible only if it has at least one param
}
