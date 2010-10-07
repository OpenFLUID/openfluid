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
  \file StatusItemInterface.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */


#include <iostream>

#include <glibmm/i18n.h>

#include "builderconfig.hpp"

#include "StatusItemInterface.hpp"


// =====================================================================
// =====================================================================


StatusItemInterface::StatusItemInterface(Glib::ustring LabelTxt) : Gtk::HBox(false,5),
m_StatusError(Ok)
{
  Glib::RefPtr<Gdk::Pixbuf> PixbufRed = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,"red.svg"),8,-1,true);
  Glib::RefPtr<Gdk::Pixbuf> PixbufGreen = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,"green.svg"),8,-1,true);

  mp_IconRed = Gtk::manage(new Gtk::Image(PixbufRed));
  mp_IconGreen = Gtk::manage(new Gtk::Image(PixbufGreen));

  mp_EventBox = Gtk::manage(new Gtk::EventBox());

  mp_Arrow = Gtk::manage(new Gtk::Arrow(Gtk::ARROW_DOWN,Gtk::SHADOW_NONE));

  mp_MenuError = Gtk::manage(new Gtk::Menu());


  mp_EventBox->add(*mp_Arrow);


  pack_start(*mp_IconGreen);

  pack_start(*Gtk::manage(new Gtk::Label(LabelTxt)));


  mp_EventBox->signal_button_press_event().connect(
      sigc::mem_fun(*this,&StatusItemInterface::on_button_press_event));

}


// =====================================================================
// =====================================================================


StatusItemInterface::~StatusItemInterface()
{

}


// =====================================================================
// =====================================================================


void StatusItemInterface::appendErrorValue(Glib::ustring Errors)
{
  if(m_StatusError == Ok)
    setStatus(Error);

  mp_MenuError->items().push_back(Gtk::Menu_Helpers::MenuElem(Errors));

}


// =====================================================================
// =====================================================================


void StatusItemInterface::setStatus(Status NewStatus)
{
  if(NewStatus == m_StatusError)
    return;

  if(NewStatus == Error)
  {
    remove(*mp_IconGreen);
    pack_start(*mp_IconRed);
    reorder_child(*mp_IconRed,0);

    pack_end(*mp_EventBox);

    m_StatusError = Error;
  }
  else if(NewStatus == Ok)
  {
    remove(*mp_IconRed);
    pack_start(*mp_IconGreen);
    reorder_child(*mp_IconGreen,0);

    remove(*mp_EventBox);

    m_StatusError = Ok;
  }

  show_all_children();

}


// =====================================================================
// =====================================================================


void StatusItemInterface::clearErrorValues()
{
  mp_MenuError->items().clear();

  setStatus(Ok);
}


// =====================================================================
// =====================================================================


bool StatusItemInterface::on_button_press_event(GdkEventButton * Event)
{
  if(m_StatusError == Error)
    mp_MenuError->popup(Event->button,Event->time);

  return true;
}
