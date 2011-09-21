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
#include "WidgetExpanderBase.hpp"

WidgetExpanderBase::WidgetExpanderBase() :
  WidgetExpander()
{
  mp_MainTableExpander = Gtk::manage(new Gtk::Table(7, 2, false));
  m_LabelColor.set_label("Color");
  m_LabelCheckButton.set_label("Graph");
  m_LabelHScale.set_label("Opacity");
  m_LabelSpinButton.set_label("Width");

  m_LabelColor.set_alignment(.02, 0);
  m_LabelCheckButton.set_alignment(.02, 0);
  m_LabelHScale.set_alignment(.02, 0);
  m_LabelSpinButton.set_alignment(.02, 0);

  mp_AdjustmentHScale = Gtk::manage(
      new Gtk::Adjustment(0.0, 0.0, 101.0, 1.0, 1.0, 1.0));
  mp_AdjustmentSpinButton = Gtk::manage(
      new Gtk::Adjustment(1.0, 1.0, 100.0, 1.0, 1.0, 0.0));

  mp_ColorButton = Gtk::manage(new Gtk::ColorButton());
  mp_CheckButton = Gtk::manage(new Gtk::CheckButton());
  mp_HScale = Gtk::manage(new Gtk::HScale(*mp_AdjustmentHScale));
  mp_SpinButton = Gtk::manage(new Gtk::SpinButton(*mp_AdjustmentSpinButton));

  mp_HScale->set_value_pos(Gtk::POS_LEFT);
  mp_HScale->set_value(100);
  mp_HScale->set_update_policy(Gtk::UPDATE_DELAYED);

  setAlphaPercent();
  randColor();
  m_SizeLine = 1;

  mp_SpinButton->set_numeric(true);
  mp_ColorButton->set_color(m_Color);

  mp_MainTableExpander->attach(m_LabelColor, 0, 1, 0, 1,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
  mp_MainTableExpander->attach(m_LabelSpinButton, 0, 1, 2, 3,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
  mp_MainTableExpander->attach(m_LabelHScale, 0, 1, 4, 5,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
  mp_MainTableExpander->attach(m_LabelCheckButton, 0, 1, 6, 7,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);

  mp_MainTableExpander->attach(*mp_ColorButton, 1, 2, 0, 1, Gtk::SHRINK,
      Gtk::SHRINK);
  mp_MainTableExpander->attach(*mp_SpinButton, 1, 2, 2, 3, Gtk::SHRINK,
      Gtk::SHRINK);
  mp_MainTableExpander->attach(*mp_HScale, 1, 2, 4, 5, Gtk::FILL | Gtk::EXPAND,
      Gtk::SHRINK);
  mp_MainTableExpander->attach(*mp_CheckButton, 1, 2, 6, 7, Gtk::SHRINK,
      Gtk::SHRINK);

  mp_MainTableExpander->attach(*ToolBox::setHSeparator(), 0, 2, 1, 2,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
  mp_MainTableExpander->attach(*ToolBox::setHSeparator(), 0, 2, 3, 4,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
  mp_MainTableExpander->attach(*ToolBox::setHSeparator(), 0, 2, 5, 6,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
  mp_MainTableExpander->set_visible(true);
  mp_MainTableExpander->show_all_children(true);
  mp_MainExpander->set_label(_("Layer Option"));
  mp_MainExpander->add(*mp_MainTableExpander);

  //******************Signal connexion*********************

  mp_ColorButton->signal_color_set().connect(
      sigc::mem_fun(*this, &WidgetExpanderBase::onWidgetExpanderBaseChanged));
  mp_AdjustmentHScale->signal_value_changed().connect(
      sigc::mem_fun(*this, &WidgetExpanderBase::onWidgetExpanderBaseChanged));
  mp_AdjustmentSpinButton->signal_value_changed().connect(
      sigc::mem_fun(*this, &WidgetExpanderBase::onWidgetExpanderBaseChanged));
}

// =====================================================================
// =====================================================================

void WidgetExpanderBase::randColor()
{
  int r = static_cast<int> (static_cast<float> (65536)
      * static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
  m_Color.set_red(r);
  int g = static_cast<int> (static_cast<float> (65536)
      * static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
  m_Color.set_green(g);
  int b = static_cast<int> (static_cast<float> (65536)
      * static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
  m_Color.set_blue(b);
}

// =====================================================================
// =====================================================================

void WidgetExpanderBase::setAlphaPercent()
{
  m_Alpha = ((mp_HScale->get_value() * 65535) / 100) / 65535;
  mp_ColorButton->set_alpha(m_Alpha * 65535);
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
  m_Color = mp_ColorButton->get_color();
  setAlphaPercent();
  m_SizeLine = mp_SpinButton->get_value();
  mp_SpinButton->update();
  m_signal_WidgetExpanderBaseChanged.emit(m_SizeLine, m_Color.get_red_p(),
      m_Color.get_green_p(), m_Color.get_blue_p(), m_Alpha);
}
