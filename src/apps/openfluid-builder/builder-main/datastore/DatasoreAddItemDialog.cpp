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
 \file DatasoreAddItemDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "DatasoreAddItemDialog.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>
#include <gtkmm/box.h>
#include <gtkmm/table.h>
#include <gtkmm/button.h>
#include <gtkmm/filechooserdialog.h>
#include <openfluid/fluidx/AdvancedDatastoreDescriptor.hpp>
#include <openfluid/fluidx/DatastoreItemDescriptor.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include "BuilderFrame.hpp"
#include "EngineHelper.hpp"

// =====================================================================
// =====================================================================

DatasoreAddItemDialog::DatasoreAddItemDialog(
    openfluid::fluidx::AdvancedDatastoreDescriptor& Datastore) :
    mp_Datastore(&Datastore)
{
  mp_InfoBarLabel = Gtk::manage(new Gtk::Label());

  // InfoBar

  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*mp_InfoBarLabel);

  // Item info

  mp_IDEntry = Gtk::manage(new Gtk::Entry());
  mp_IDEntry->signal_changed().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::onIDChanged));

  mp_TypeCombo = Gtk::manage(new Gtk::ComboBoxText());
  mp_TypeCombo->append(
      openfluid::core::UnstructuredValue::getStringFromValueType(
          openfluid::core::UnstructuredValue::GeoVectorValue));
  mp_TypeCombo->append(
      openfluid::core::UnstructuredValue::getStringFromValueType(
          openfluid::core::UnstructuredValue::GeoRasterValue));
  mp_TypeCombo->set_active(0);

  Gtk::Table* IDTypeTable = Gtk::manage(new Gtk::Table(2, 2));
  IDTypeTable->set_col_spacings(3);
  IDTypeTable->set_row_spacings(5);
  IDTypeTable->attach(
      *Gtk::manage(new Gtk::Label(_("ID"), Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER)),
      0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL);
  IDTypeTable->attach(
      *Gtk::manage(
          new Gtk::Label(_("Type"), Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER)),
      0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL);
  IDTypeTable->attach(*mp_IDEntry, 1, 2, 0, 1, Gtk::SHRINK);
  IDTypeTable->attach(*mp_TypeCombo, 1, 2, 1, 2, Gtk::FILL);

  mp_ClassCheck = Gtk::manage(new Gtk::CheckButton(_("Unit class")));
  mp_ClassCheck->signal_toggled().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::onCheckToggled));
  mp_ClassCheck->set_active(false);

  mp_ClassEntry = Gtk::manage(new Gtk::Entry());
  mp_ClassEntry->set_sensitive(false);

  Gtk::HBox* ClassBox = Gtk::manage(new Gtk::HBox());
  ClassBox->pack_start(*mp_ClassCheck, Gtk::PACK_SHRINK, 5);
  ClassBox->pack_start(*mp_ClassEntry, Gtk::PACK_SHRINK);

  // Data source

  Gtk::RadioButtonGroup FileResourceGroup;

  //  // File
  mp_FileRadio = Gtk::manage(new Gtk::RadioButton(_("File")));
  mp_FileRadio->signal_toggled().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::onFileResourceToggled));
  mp_FileRadio->set_group(FileResourceGroup);

  mp_FilePathEntry = Gtk::manage(new Gtk::Entry());

  Gtk::Button* BrowseButton = Gtk::manage(new Gtk::Button(_("Browse...")));
  BrowseButton->signal_clicked().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::onBrowseButtonClicked));

  Gtk::HBox* FilePathBox = Gtk::manage(new Gtk::HBox());
  FilePathBox->pack_start(*mp_FilePathEntry, Gtk::PACK_EXPAND_WIDGET);
  FilePathBox->pack_start(*BrowseButton, Gtk::PACK_SHRINK);

  Gtk::RadioButtonGroup SubDirGroup;

  mp_NoSubDirRadio = Gtk::manage(
      new Gtk::RadioButton(_("Copied into dataset directory")));
  mp_NoSubDirRadio->set_group(SubDirGroup);

  mp_SubDirRadio = Gtk::manage(
      new Gtk::RadioButton(_("Copied into dataset subdirectory:")));
  mp_SubDirRadio->set_group(SubDirGroup);
  mp_SubDirRadio->signal_toggled().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::onSubDirToggled));
  mp_SubDirEntry = Gtk::manage(new Gtk::Entry());
  mp_SubDirEntry->set_sensitive(false);
  Gtk::HBox* SubDirBox = Gtk::manage(new Gtk::HBox());
  SubDirBox->pack_start(*mp_SubDirRadio, Gtk::PACK_SHRINK);
  SubDirBox->pack_start(*mp_SubDirEntry, Gtk::PACK_EXPAND_WIDGET);

  mp_FileDetailBox = Gtk::manage(new Gtk::VBox());
  mp_FileDetailBox->pack_start(*FilePathBox, Gtk::PACK_SHRINK);
  mp_FileDetailBox->pack_start(*mp_NoSubDirRadio, Gtk::PACK_SHRINK);
  mp_FileDetailBox->pack_start(*SubDirBox);
  mp_FileDetailBox->set_sensitive(true);

  Gtk::HBox* FileBox = Gtk::manage(new Gtk::HBox());
  FileBox->pack_start(*mp_FileRadio, Gtk::PACK_SHRINK);
  FileBox->pack_start(*mp_FileDetailBox);

  //  // Resource
  mp_ResourceRadio = Gtk::manage(new Gtk::RadioButton(_("Resource string")));
  mp_ResourceRadio->set_group(FileResourceGroup);

  mp_ResourceStringEntry = Gtk::manage(new Gtk::Entry());
  mp_ResourceStringEntry->set_sensitive(false);

  Gtk::HBox* ResourceBox = Gtk::manage(new Gtk::HBox());
  ResourceBox->pack_start(*mp_ResourceRadio, Gtk::PACK_SHRINK);
  ResourceBox->pack_start(*mp_ResourceStringEntry, Gtk::PACK_EXPAND_WIDGET);

  Gtk::VBox* DataSourceBox = Gtk::manage(new Gtk::VBox());
  DataSourceBox->pack_start(*FileBox, Gtk::PACK_SHRINK);
  DataSourceBox->pack_start(*ResourceBox, Gtk::PACK_SHRINK);

  BuilderFrame* SourceFrame = Gtk::manage(new BuilderFrame());
  SourceFrame->setLabelText(_("Data source"));
  SourceFrame->add(*DataSourceBox);

  mp_Dialog = new Gtk::Dialog(_("Add a Datasore item"));
  mp_Dialog->get_vbox()->pack_start(*mp_InfoBar, Gtk::PACK_SHRINK, 5);
  mp_Dialog->get_vbox()->pack_start(*IDTypeTable, Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*ClassBox, Gtk::PACK_SHRINK, 5);
  mp_Dialog->get_vbox()->pack_start(*SourceFrame, Gtk::PACK_SHRINK, 5);
//  mp_Dialog->set_default_size(700, 200);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->show_all_children();

  onIDChanged();
  mp_FileRadio->set_active(true);
  mp_SubDirRadio->set_active(false);
}

// =====================================================================
// =====================================================================

openfluid::fluidx::DatastoreItemDescriptor* DatasoreAddItemDialog::show()
{
  openfluid::fluidx::DatastoreItemDescriptor* NewItem = 0;

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    //TODO
  }

  mp_Dialog->hide();

  return NewItem;
}

// =====================================================================
// =====================================================================

void DatasoreAddItemDialog::onIDChanged()
{
  std::string NewID = mp_IDEntry->get_text();

  bool IsValid = false;

  if (NewID == "" || EngineHelper::isEmptyString(NewID))
  {
    mp_InfoBarLabel->set_text(_("ID can not be empty"));
  }
  else if (mp_Datastore->isItemAlreadyExist(NewID))
  {
    mp_InfoBarLabel->set_text(_("This ID already exists"));
  }
  else
  {
    IsValid = true;
  }

  mp_InfoBar->set_visible(!IsValid);
  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, IsValid);
}

// =====================================================================
// =====================================================================

void DatasoreAddItemDialog::onCheckToggled()
{
  mp_ClassEntry->set_sensitive(mp_ClassCheck->get_active());
}

// =====================================================================
// =====================================================================

void DatasoreAddItemDialog::onFileResourceToggled()
{
  mp_FileDetailBox->set_sensitive(!mp_FileDetailBox->get_sensitive());
  mp_ResourceStringEntry->set_sensitive(
      !mp_ResourceStringEntry->get_sensitive());
}

// =====================================================================
// =====================================================================

void DatasoreAddItemDialog::onBrowseButtonClicked()
{
  Gtk::FileChooserDialog Dialog(_("Choose the file to add"));

  Dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  Dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  Dialog.set_current_folder(
      openfluid::base::ProjectManager::getInstance()->getInputDir());

  if (Dialog.run() == Gtk::RESPONSE_OK)
    mp_FilePathEntry->set_text(Glib::filename_to_utf8(Dialog.get_filename()));
}

// =====================================================================
// =====================================================================

void DatasoreAddItemDialog::onSubDirToggled()
{
  mp_SubDirEntry->set_sensitive(!mp_SubDirEntry->get_sensitive());
}

// =====================================================================
// =====================================================================

