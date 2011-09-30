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

/**+
 \file AddDialogFileChooser.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */
#include <glibmm/i18n.h>
#include <gtkmm/stock.h>

#include "AddDialogFileChooser.hpp"
#include "ToolBox.hpp"

AddDialogFileChooser::AddDialogFileChooser(Gtk::Window& ParentWindow,
    const Glib::ustring& Title) :
  m_ClassName("")
{
  mp_FileChooserDialog = new Gtk::FileChooserDialog(ParentWindow, Title,
      Gtk::FILE_CHOOSER_ACTION_OPEN);
  mp_FileChooserDialog->set_transient_for(ParentWindow);

  mp_FileChooserDialog->set_can_focus(true);


  mp_InfoBarLabel = Gtk::manage(new Gtk::Label());
  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*mp_InfoBarLabel);

  mp_InfoBarLabel->set_label("");

  mp_FilterUnitClass = Gtk::manage(new Gtk::ComboBoxText());

  Gtk::HBox* Hbox = Gtk::manage(new Gtk::HBox());
  Hbox->set_border_width(5);//spacing(10);
  //      lab.set_padding(10,2);
  mp_LabelChoose = Gtk::manage(new Gtk::Label());
  mp_LabelChoose->set_label(_("Choose An Unit Class :"));
  mp_LabelChoose->set_visible(true);

  mp_FilterUnitClass->signal_changed().connect(
      sigc::bind<Gtk::ComboBoxText*>(
          sigc::mem_fun(*this, &AddDialogFileChooser::onComboChanged),
          mp_FilterUnitClass));

  mp_FilterUnitClass->set_sensitive(true);
  mp_FilterUnitClass->set_visible(true);
  mp_FilterUnitClass->show_all_children(true);

  Hbox->pack_start(*mp_LabelChoose, Gtk::PACK_SHRINK);
  Hbox->pack_end(*mp_FilterUnitClass, Gtk::PACK_SHRINK);
  Hbox->set_visible(true);

  Gtk::VBox* VBox = static_cast<Gtk::VBox*> (mp_FileChooserDialog->get_child());

  VBox->pack_start(*ToolBox::setHSeparator(), Gtk::PACK_SHRINK);
  VBox->pack_start(*Hbox, Gtk::PACK_SHRINK);
  VBox->pack_start(*ToolBox::setHSeparator(), Gtk::PACK_SHRINK);
  VBox->pack_start(*mp_InfoBar, Gtk::PACK_SHRINK);

  //Add filters, so that only certain file types can be selected:
  Gtk::FileFilter filter_shapefile;
  filter_shapefile.set_name("Shapefiles");
  filter_shapefile.add_pattern("*.shp");
  mp_FileChooserDialog->add_filter(filter_shapefile);

  Gtk::FileFilter filter_any;
  filter_any.set_name("Any files");
  filter_any.add_pattern("*");
  mp_FileChooserDialog->add_filter(filter_any);

  mp_FileChooserDialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_FileChooserDialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_FileChooserDialog->set_default_response(Gtk::RESPONSE_OK);
  mp_FileChooserDialog->signal_selection_changed().connect(
      sigc::mem_fun(*this, &AddDialogFileChooser::onChanged));
  mp_FileChooserDialog->show_all_children();

}

// =====================================================================
// =====================================================================

void AddDialogFileChooser::onComboChanged(Gtk::ComboBoxText* Combo)
{
  Glib::ustring text = Combo->get_active_text();
  m_ClassName = static_cast<std::string> (text);
  onChanged();
}

// =====================================================================
// =====================================================================

void AddDialogFileChooser::onChanged()
{
  std::string FileURI = mp_FileChooserDialog->get_filename();
  std::string ExtentionFile = "";

  if (!FileURI.empty())
    ExtentionFile = FileURI.substr(FileURI.size() - 4, 4);

  bool IsValid = false;

  if (FileURI == "" || isEmptyString(FileURI))
  {
    mp_InfoBarLabel->set_text(_("Choose a file"));
  } else if (ExtentionFile != ".shp")
  {
    mp_InfoBarLabel->set_text(_("Choose a file with a Shapefile extention"));
  } else if (isEmptyString(m_ClassName))
  {
    mp_InfoBarLabel->set_text(_("Select a unit class"));
  } else
  {
    IsValid = true;
  }

  mp_InfoBar->set_visible(!IsValid);
  mp_FileChooserDialog->set_response_sensitive(Gtk::RESPONSE_OK, IsValid);
}

// =====================================================================
// =====================================================================

bool AddDialogFileChooser::isEmptyString(std::string Str)
{
  bool isEmpty = true;
  for (unsigned int i = 0; i < Str.size() && isEmpty; i++)
  {
    if (!std::isspace(Str[i]))
    {
      isEmpty = false;
      break;
    }
  }
  return isEmpty;
}

// =====================================================================
// =====================================================================

std::pair<std::pair<std::string, std::string>, std::string> AddDialogFileChooser::show(
    std::set<std::string> ClassNames)
{

  mp_FilterUnitClass->clear_items();
  std::set<std::string>::iterator it;
  if (!ClassNames.empty())
  {
    for (it = ClassNames.begin(); it != ClassNames.end(); it++)
    {
      mp_FilterUnitClass->append_text(static_cast<Glib::ustring> (*it));
    }
  }
  std::pair<std::string, std::string> File = std::make_pair("", "");
  std::pair<std::pair<std::string, std::string>, std::string> Data =
      std::make_pair(File, "");

  if (mp_FileChooserDialog->run() == Gtk::RESPONSE_OK)
  {
    std::string FileURI = mp_FileChooserDialog->get_filename();
    std::string FolderURI = mp_FileChooserDialog->get_current_folder() + "/";
    std::string FileName;

    FileName = FileURI.substr(FolderURI.size(),
        FileURI.size() - FolderURI.size() - 4);

    File = std::make_pair(FolderURI, FileName);
    Data = std::make_pair(File, m_ClassName);
  }

  mp_FileChooserDialog->hide();

  return Data;
}
