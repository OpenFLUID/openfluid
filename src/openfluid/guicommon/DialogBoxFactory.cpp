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
 \file DialogBoxFactory.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <openfluid/guicommon/DialogBoxFactory.hpp>

#include <glibmm.h>
#include <glibmm/i18n.h>


namespace openfluid { namespace guicommon {


// =====================================================================
// =====================================================================


bool DialogBoxFactory::showSimpleOkCancelQuestionDialog(
    Glib::ustring Message)
{
  Gtk::MessageDialog Dialog(Message, false, Gtk::MESSAGE_QUESTION,
      Gtk::BUTTONS_OK_CANCEL);

  return (Dialog.run() == Gtk::RESPONSE_OK);
}


// =====================================================================
// =====================================================================


Glib::ustring DialogBoxFactory::showOpenProjectDialog()
{
  Gtk::FileChooserDialog Dialog(_("Select existing project folder"),
      Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

  /* TODO: set default folder from Preferences */
  Dialog.set_current_folder(
      openfluid::base::RuntimeEnvironment::getInstance()->getInputDir() + "/..");

  Dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  Dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  if (Dialog.run() == Gtk::RESPONSE_OK)
  {
    return Dialog.get_filename();
  }

  return "";
}


// =====================================================================
// =====================================================================


void DialogBoxFactory::showSimpleErrorMessage(Glib::ustring MessageText)
{
  Gtk::MessageDialog Dialog(MessageText, false, Gtk::MESSAGE_ERROR,
      Gtk::BUTTONS_OK);
  if (Dialog.run())
    Dialog.hide();
}


// =====================================================================
// =====================================================================


std::string DialogBoxFactory::showTextEntryDialog(
    Glib::ustring MessageText, Glib::ustring LabelText)
{
  Gtk::Dialog Dialog;

  Gtk::Label Message(MessageText);

  Gtk::Label Label(LabelText);
  Gtk::Entry Entry;
  Gtk::HBox Box;
  Box.pack_start(Label);
  Box.pack_start(Entry);

  Dialog.get_vbox()->pack_start(Message, true, true, 10);
  Dialog.get_vbox()->pack_start(Box, true, true, 10);

  Dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  Dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  Dialog.show_all_children();

  if (Dialog.run() == Gtk::RESPONSE_OK)
    return Entry.get_text();

  return "";
}


} } //namespaces


