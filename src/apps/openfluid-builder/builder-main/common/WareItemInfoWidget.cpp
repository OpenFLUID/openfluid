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
 \file WareItemInfoWidget.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "WareItemInfoWidget.hpp"

#include <boost/filesystem/path.hpp>
#include <glibmm/i18n.h>
#include <gtkmm/label.h>
#include <gtkmm/separator.h>
#include <gtkmm/textbuffer.h>
#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/ware/GeneratorSignature.hpp>
#include <openfluid/ware/ObserverSignature.hpp>
#include <openfluid/machine/WareSignatureInstance.hpp>

// =====================================================================
// =====================================================================

WareItemInfoWidget::WareItemInfoWidget(
    const openfluid::machine::WareSignatureInstance& SignInst,
    const openfluid::ware::WareSignature& Sign) :
    Gtk::Table(), m_TableBottom(0)
{
  set_border_width(3);
  set_visible(true);

  signal_realize().connect(
      sigc::mem_fun(*this, &WareItemInfoWidget::updateColorBg));

  openfluid::fluidx::WareDescriptor::WareType Type = SignInst.ItemType;

  attachKey(_("ID:"));
  attachValue(Sign.ID);

  attachSeparator();

  attachKey(_("Name:"));
  attachValue(Sign.Name);

  attachSeparator();

  if (Type == openfluid::fluidx::WareDescriptor::PluggedFunction || Type
      == openfluid::fluidx::WareDescriptor::PluggedObserver)
  {
    attachKey(_("Path:"));
    attachValue(
        ((boost::filesystem::path) SignInst.Filename).parent_path().string());

    attachSeparator();
  }

  attachKey(_("Description:"));
  attachValue(Sign.Description);

  attachSeparator();

  if (Type == openfluid::fluidx::WareDescriptor::Generator)
  {
    show_all_children();
    return;
  }

  attachKey(_("Version:"));
  attachValue(Sign.Version);

  attachKey(_("Status:"));
  switch (Sign.Status)
  {
    case openfluid::ware::EXPERIMENTAL:
      attachValue("Experimental");
      break;
    case openfluid::ware::BETA:
      attachValue("Beta");
      break;
    case openfluid::ware::STABLE:
      attachValue("Stable");
      break;
    default:
      attachValue("");
      break;
  }

  attachSeparator();

  if (Type == openfluid::fluidx::WareDescriptor::PluggedFunction)
  {
    openfluid::ware::SimulatorSignature* FSign =
        dynamic_cast<openfluid::ware::SimulatorSignature*>(const_cast<openfluid::ware::WareSignature*>(&Sign));

    attachKey(_("Domain:"));
    attachValue(FSign->Domain);

    attachKey(_("Process:"));
    attachValue(FSign->Process);

    attachKey(_("Method:"));
    attachValue(FSign->Method);

    attachSeparator();

    attachKey(_("Author(s):"));
    attachValue(FSign->Author);

    attachKey(_("Email(s):"));
    attachValue(FSign->AuthorEmail);

    attachSeparator();
  }

  show_all_children();
}

// =====================================================================
// =====================================================================

void WareItemInfoWidget::attachKey(std::string Text)
{
  attach(*Gtk::manage(new Gtk::Label(Text, Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP)), 0,
         1, m_TableBottom, m_TableBottom + 1, Gtk::FILL, Gtk::FILL, 0, 0);
}

// =====================================================================
// =====================================================================

void WareItemInfoWidget::attachValue(std::string Text)
{
  Glib::RefPtr<Gtk::TextBuffer> Buf = Gtk::TextBuffer::create();
  Buf->set_text(Text);

  Gtk::TextView* Val = Gtk::manage(new Gtk::TextView(Buf));
  Val->set_wrap_mode(Gtk::WRAP_WORD);
  Val->set_editable(false);

  attach(*Val, 1, 2, m_TableBottom, m_TableBottom + 1, Gtk::FILL | Gtk::EXPAND,
         Gtk::FILL, 5, 0);

  m_TableBottom++;

  TextViews.push_back(Val);
}

// =====================================================================
// =====================================================================

Glib::ustring WareItemInfoWidget::replaceEmpty(Glib::ustring TextToCheck)
{
  if (TextToCheck.empty())
    TextToCheck = _("(unknown)");
  return TextToCheck;
}

// =====================================================================
// =====================================================================

void WareItemInfoWidget::attachSeparator()
{
  attach(*Gtk::manage(new Gtk::HSeparator()), 0, 2, m_TableBottom,
         m_TableBottom + 1, Gtk::FILL, Gtk::SHRINK, 0, 0);

  m_TableBottom++;
}

// =====================================================================
// =====================================================================

void WareItemInfoWidget::updateColorBg()
{
  Gdk::Color Color = get_parent()->get_style()->get_background(
      Gtk::STATE_NORMAL);

  for (std::list<Gtk::TextView*>::iterator it = TextViews.begin();
      it != TextViews.end(); ++it)
  {
    (*it)->modify_base((*it)->get_state(), Color);
  }
}
