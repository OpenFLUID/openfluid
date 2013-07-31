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
  \file MarketBuildOptionsDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <openfluid/guicommon-gtk/MarketBuildOptionsDialog.hpp>
#include <openfluid/market/MarketPackage.hpp>
#include <openfluid/tools/SwissTools.hpp>

#include <glibmm/i18n.h>

#include <gtkmm/stock.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>


namespace openfluid { namespace guicommon {

// =====================================================================
// =====================================================================


MarketBuildOptionsDialog::MarketBuildOptionsDialog(const std::string& CommonBuildOptions, const std::string& BuildOptions, const std::string SimID)
: Gtk::Dialog(), m_CommonBuildOptions(CommonBuildOptions),m_BuildOptions(BuildOptions),m_SimID(SimID)
{

  set_size_request(450,-1);
  set_border_width(6);

  Gtk::Label* InfoLabel = Gtk::manage(new Gtk::Label());
  InfoLabel->set_markup(std::string("<i>")+_("These options control the builds of source packages.\nChanging this is at your own risk.")+std::string("</i>"));
  InfoLabel->set_justify(Gtk::JUSTIFY_CENTER);

  get_vbox()->pack_start(*InfoLabel);

  Gtk::Label* CommonOptsLabel = Gtk::manage(new Gtk::Label());

  if (!SimID.empty())
    CommonOptsLabel->set_markup(_("<u>Common source build options:</u>\n")
                                +openfluid::tools::ReplaceEmptyString(CommonBuildOptions,_("<i>none</i>")));
  else
    CommonOptsLabel->set_label("");

  CommonOptsLabel->set_alignment(0,0.5);
  get_vbox()->pack_start(*CommonOptsLabel,Gtk::PACK_SHRINK,12);


  Gtk::Label* EditLabel = Gtk::manage(new Gtk::Label());
  if (!SimID.empty())
  {
    EditLabel->set_label(_("Specific build options for ")+SimID+_(":"));
  }
  else
  {
    EditLabel->set_label(_("Common source build options:"));
  }
  EditLabel->set_alignment(0,0.5);
  get_vbox()->pack_start(*EditLabel);

  if (SimID.empty()) m_OptionsEntry.set_text(CommonBuildOptions);
  else m_OptionsEntry.set_text(BuildOptions);
  get_vbox()->pack_start(m_OptionsEntry);

  add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  if(m_SimID.empty())
    set_title(_("Common build options for all source packages"));
  else
    set_title(_("Build options for ") + m_SimID);


  show_all_children();
}


// =====================================================================
// =====================================================================

} } //namespaces




