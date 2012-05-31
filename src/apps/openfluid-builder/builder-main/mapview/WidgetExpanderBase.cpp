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
 \file WidgetExpanderBase.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include "ToolBox.hpp"
#include <glibmm/i18n.h>
#include <gtkmm/alignment.h>
#include "WidgetExpanderBase.hpp"

WidgetExpanderBase::WidgetExpanderBase() :
  WidgetExpander()
{
  mp_ColorButton = Gtk::manage(new Gtk::ColorButton(getARandColor()));

  mp_WidthSpinButton = Gtk::manage(new Gtk::SpinButton());
  mp_WidthSpinButton->set_range(1, 100);
  mp_WidthSpinButton->set_increments(1, 10);
  mp_WidthSpinButton->set_numeric(true);
  mp_WidthSpinButton->set_value(1);

  mp_OpacityHScale = Gtk::manage(new Gtk::HScale(0, 101, 1));
  mp_OpacityHScale->set_value_pos(Gtk::POS_LEFT);
  mp_OpacityHScale->set_value(100);
  mp_OpacityHScale->set_update_policy(Gtk::UPDATE_DELAYED);

  mp_ShowIDCheckBox = Gtk::manage(new Gtk::CheckButton(_("Show units IDs")));
  //  mp_CheckButtonGraph = Gtk::manage(new Gtk::CheckButton(_("show graph")));

  Gtk::HBox* ColorButtonBox = Gtk::manage(new Gtk::HBox());
  ColorButtonBox->pack_start(*mp_ColorButton, Gtk::PACK_SHRINK);

  Gtk::HBox* WidthSpinButtonBox = Gtk::manage(new Gtk::HBox());
  WidthSpinButtonBox->pack_start(*mp_WidthSpinButton, Gtk::PACK_SHRINK);

  Gtk::Table* mp_MainTable = Gtk::manage(new Gtk::Table());
  mp_MainTable->set_col_spacings(5);
  mp_MainTable->attach(*Gtk::manage(new Gtk::Label(_("Color:"), 0, 0.5)), 0, 1,
      0, 1, Gtk::FILL, Gtk::SHRINK);
  mp_MainTable->attach(*Gtk::manage(new Gtk::Label(_("Width:"), 0, 0.5)), 0, 1,
      1, 2, Gtk::FILL, Gtk::SHRINK);
  mp_MainTable->attach(*Gtk::manage(new Gtk::Label(_("Opacity:"), 0, 0.5)), 0,
      1, 2, 3, Gtk::FILL, Gtk::SHRINK);
  mp_MainTable->attach(*ColorButtonBox, 1, 2, 0, 1, Gtk::FILL, Gtk::SHRINK);
  mp_MainTable->attach(*WidthSpinButtonBox, 1, 2, 1, 2, Gtk::FILL, Gtk::SHRINK);
  mp_MainTable->attach(*mp_OpacityHScale, 1, 2, 2, 3, Gtk::FILL | Gtk::EXPAND,
      Gtk::SHRINK);
  mp_MainTable->attach(*mp_ShowIDCheckBox, 0, 2, 3, 4, Gtk::FILL, Gtk::SHRINK);
  //  mp_MainTable->attach(*mp_CheckButtonGraph, 0, 2, 4, 5, Gtk::FILL, Gtk::SHRINK);

  Gtk::Alignment* MainTableAlign = Gtk::manage(new Gtk::Alignment());
  MainTableAlign->set_padding(10, 20, 20, 0);
  MainTableAlign->add(*mp_MainTable);

  mp_MainExpander->set_label(_("Style"));
  mp_MainExpander->add(*MainTableAlign);
  mp_MainExpander->show_all_children();

  //******************Signal connexion*********************

  mp_ColorButton->signal_color_set().connect(sigc::mem_fun(*this,
      &WidgetExpanderBase::onWidgetExpanderBaseChanged));
  mp_WidthSpinButton->signal_value_changed().connect(sigc::mem_fun(*this,
      &WidgetExpanderBase::onWidgetExpanderBaseChanged));
  mp_OpacityHScale->signal_value_changed().connect(sigc::mem_fun(*this,
      &WidgetExpanderBase::onWidgetExpanderBaseChanged));
  mp_ShowIDCheckBox->signal_toggled().connect(sigc::mem_fun(*this,
      &WidgetExpanderBase::onWidgetExpanderBaseChanged));
  //  mp_CheckButtonGraph->signal_toggled().connect(
  //      sigc::mem_fun(*this, &WidgetExpanderBase::onWidgetExpanderBaseChanged));
}

// =====================================================================
// =====================================================================

Gdk::Color WidgetExpanderBase::getARandColor()
{
  Gdk::Color RandColor;

  RandColor.set_rgb(Glib::Rand().get_int_range(0, 65535),
      Glib::Rand().get_int_range(0, 65535),
      Glib::Rand().get_int_range(0, 65535));

  return RandColor;
}

// =====================================================================
// =====================================================================

WidgetExpanderBase::mtype_SignalWidgetExpanderBase WidgetExpanderBase::signalWidgetExpanderBaseChanged()
{
  return m_signal_WidgetExpanderBaseChanged;
}

// =====================================================================
// =====================================================================

void WidgetExpanderBase::onWidgetExpanderBaseChanged()
{
  Gdk::Color CurrentColor = mp_ColorButton->get_color();

  double CurrentAlpha = mp_OpacityHScale->get_value() / 100;

  int CurrentWidth = mp_WidthSpinButton->get_value();

  m_signal_WidgetExpanderBaseChanged.emit(CurrentWidth,
      CurrentColor.get_red_p(), CurrentColor.get_green_p(),
      CurrentColor.get_blue_p(), CurrentAlpha, mp_ShowIDCheckBox->get_active()/*, mp_CheckButtonGraph->get_active()*/);
}
