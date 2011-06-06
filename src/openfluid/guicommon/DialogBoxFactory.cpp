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

namespace openfluid {
namespace guicommon {

// =====================================================================
// =====================================================================


bool DialogBoxFactory::showSimpleOkCancelQuestionDialog(Glib::ustring Message)
{
  Gtk::MessageDialog Dialog(Message, false, Gtk::MESSAGE_QUESTION,
      Gtk::BUTTONS_OK_CANCEL);

  return (Dialog.run() == Gtk::RESPONSE_OK);
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


void DialogBoxFactory::showSimpleWarningMessage(Glib::ustring MessageText)
{
  Gtk::MessageDialog Dialog(MessageText, false, Gtk::MESSAGE_WARNING,
      Gtk::BUTTONS_OK);
  if (Dialog.run())
    Dialog.hide();
}

// =====================================================================
// =====================================================================


void DialogBoxFactory::showDisabledFeatureMessage()
{
  showSimpleWarningMessage(_("This feature is currently disabled"));
}

// =====================================================================
// =====================================================================


std::string DialogBoxFactory::showTextEntryDialog(Glib::ustring MessageText,
    Glib::ustring LabelText)
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

// =====================================================================
// =====================================================================


std::map<std::string, std::string> DialogBoxFactory::showGeneratorCreationDialog(
    std::vector<std::string> Classes)
{
  Gtk::Dialog Dialog(_("Generator creation"));

  Gtk::Label VarNameLabel(_("Variable name"));
  Gtk::Label ClassLabel(_("Unit class"));
  Gtk::Label VarSizeLabel("Size");

  Gtk::RadioButton::Group RadioGrp;
  Gtk::RadioButton ScalarRadio(RadioGrp, _("Scalar Value"));
  ScalarRadio.set_active(true);
  Gtk::RadioButton VectorRadio(RadioGrp, _("Vector Value:")+std::string(" "));

  Gtk::Entry VarNameEntry;

  Gtk::ComboBoxText ClassCombo;
  for (unsigned int i = 0; i < Classes.size(); i++)
  {
    ClassCombo.append_text(Classes[i]);
  }
  ClassCombo.set_active(0);

  Gtk::SpinButton VarSizeSpin;
  VarSizeSpin.set_numeric(true);
  VarSizeSpin.set_increments(1, 1);
  VarSizeSpin.set_range(2.0, 9.0);

  Gtk::Table Table;
  Table.set_row_spacings(10);
  Table.attach(VarNameLabel, 0, 1, 0, 1);
  Table.attach(VarNameEntry, 1, 2, 0, 1);
  Table.attach(ClassLabel, 0, 1, 1, 2);
  Table.attach(ClassCombo, 1, 2, 1, 2);

  Gtk::HBox VectorBox;
  VectorBox.pack_start(VectorRadio, Gtk::PACK_SHRINK);
  VectorBox.pack_start(VarSizeLabel, Gtk::PACK_SHRINK);
  VectorBox.pack_start(VarSizeSpin, Gtk::PACK_SHRINK);

  Dialog.get_vbox()->pack_start(Table, true, true, 10);
  Dialog.get_vbox()->pack_start(ScalarRadio, true, true, 0);
  Dialog.get_vbox()->pack_start(VectorBox, true, true, 10);

  Dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  Dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  Dialog.show_all_children();

  std::map<std::string, std::string> GenInfo;

  if (Dialog.run() == Gtk::RESPONSE_OK)
  {
    GenInfo["varname"] = VarNameEntry.get_text();
    GenInfo["classname"] = ClassCombo.get_active_text();
    if (ScalarRadio.get_active())
      GenInfo["varsize"] = Glib::ustring::compose("%1", 1);
    else
      GenInfo["varsize"] = Glib::ustring::compose("%1",
          VarSizeSpin.get_value_as_int());
  }

  return GenInfo;
}

// =====================================================================
// =====================================================================


int DialogBoxFactory::showCloseProjectDialog(bool HasToBeSaved)
{
  Gtk::Dialog Dialog(_("Close project?"));

  Gtk::Label Label;

  Gtk::HBox MainBox;
  Gtk::Image Image(Gtk::Stock::DIALOG_QUESTION, Gtk::ICON_SIZE_DIALOG);
  MainBox.pack_start(Image, Gtk::PACK_SHRINK, 10);
  MainBox.pack_start(Label, Gtk::PACK_SHRINK, 10);

  Dialog.get_vbox()->pack_start(MainBox, false, false, 10);

  Dialog.add_button(Gtk::Stock::CANCEL, 0);

  if (HasToBeSaved)
  {
    Label.set_text(_("Do you want to save this project before closing?"));
    Dialog.add_button(_("Close project without saving"), 1);
    Dialog.add_button(_("Save and close project"), 2);
  } else
  {
    Label.set_text(_("Are you sure you want to close this project?"));
    Dialog.add_button(Gtk::Stock::OK, 1);
  }

  Dialog.show_all_children();

  int Res = Dialog.run();

  if (Res == Gtk::RESPONSE_DELETE_EVENT)
    Res = 0;

  return Res;
}

// =====================================================================
// =====================================================================

std::string DialogBoxFactory::showDomainIDataEditDialog(int Id,
    std::string DataName, std::string Val)
{
  Gtk::Dialog Dialog(_("Edit Inputdata"), true, false);

  Gtk::Entry Entry;
  Entry.set_text(Val);

  Gtk::Label Label(Glib::ustring::compose(_("%1 for ID %2"), DataName,Id));

  Dialog.get_vbox()->pack_start(Label);
  Dialog.get_vbox()->pack_start(Entry);

  Dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  Dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  Dialog.set_default_response(Gtk::RESPONSE_OK);

  Dialog.show_all_children();

  int Res = Dialog.run();

  if (Res == Gtk::RESPONSE_CANCEL || Res == Gtk::RESPONSE_DELETE_EVENT)
    return Val;

  std::string NewVal = Entry.get_text();
  if (NewVal == "" || NewVal == " ")
    NewVal = "0";

  return NewVal;
}

// =====================================================================
// =====================================================================

}
} //namespaces


