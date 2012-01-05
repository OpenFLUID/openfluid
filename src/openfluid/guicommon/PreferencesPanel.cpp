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
 \file PreferencesPanel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "PreferencesPanel.hpp"

#include <gtkmm/alignment.h>
#include <gtkmm/label.h>


namespace openfluid { namespace guicommon {

// =====================================================================
// =====================================================================


PreferencesPanel::PreferencesPanel(Glib::ustring PanelTitle)
{
  Gtk::Label* TitleLabel = Gtk::manage(new Gtk::Label());
  TitleLabel->set_markup(Glib::ustring::compose("<b><big>%1</big></b>",
      PanelTitle));

  mp_ContentWindow = Gtk::manage(new Gtk::ScrolledWindow());
  mp_ContentWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_ContentWindow->set_shadow_type(Gtk::SHADOW_NONE);

  mp_MainBox = Gtk::manage(new Gtk::VBox());
  mp_MainBox->pack_start(*TitleLabel, Gtk::PACK_SHRINK,3);
//  mp_MainBox->pack_start(*Gtk::manage(new Gtk::HSeparator()), Gtk::PACK_SHRINK);
  mp_MainBox->pack_start(*mp_ContentWindow, Gtk::PACK_EXPAND_WIDGET);

  mp_MainBox->set_visible(true);
  mp_MainBox->show_all_children();
}

// =====================================================================
// =====================================================================


Gtk::Widget* PreferencesPanel::asWidget()
{
  return mp_MainBox;
}

// =====================================================================
// =====================================================================


Gtk::Widget* PreferencesPanel::createSubTitle(Glib::ustring SubTitle)
{
  Gtk::Label* SubLabel = Gtk::manage(new Gtk::Label());

  SubLabel->set_markup(Glib::ustring::compose("<b>%1</b>", SubTitle));

  SubLabel->set_alignment(0, 0.5);

  SubLabel->set_visible(true);

  return SubLabel;
}

// =====================================================================
// =====================================================================


Gtk::Widget* PreferencesPanel::createSubBoxAlignement(Gtk::Widget* InnerWidget)
{
  Gtk::Alignment* Align = Gtk::manage(new Gtk::Alignment());

  Align->set_padding(10, 20, 20, 0);

  Align->add(*InnerWidget);

  Align->set_visible(true);

  return Align;
}

// =====================================================================
// =====================================================================


Gtk::Box* PreferencesPanel::createPanelBox()
{
  Gtk::VBox* PanelBox = Gtk::manage(new Gtk::VBox(false, 5));

  PanelBox->set_border_width(10);

  PanelBox->set_visible(true);

  return PanelBox;
}

// =====================================================================
// =====================================================================

} } //namespaces
