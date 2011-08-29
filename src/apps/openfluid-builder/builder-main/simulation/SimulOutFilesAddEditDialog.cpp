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
 \file SimulOutFilesAddEditDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutFilesAddEditDialog.hpp"

#include <glibmm/i18n.h>

#include <gtkmm/stock.h>

#include "EngineHelper.hpp"

// =====================================================================
// =====================================================================


SimulOutFilesAddEditDialog::SimulOutFilesAddEditDialog() :
  mp_OutDesc(0)
{
  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBarLabel = Gtk::manage(new Gtk::Label());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*mp_InfoBarLabel);

  mp_FormatNameLabel = Gtk::manage(new Gtk::Label());

  mp_ColSepComboEntry = Gtk::manage(new Gtk::ComboBoxEntryText());
  mp_ColSepComboEntry->append_text(":");
  mp_ColSepComboEntry->append_text("%");
  mp_ColSepComboEntry->append_text(";");
  mp_ColSepComboEntry->append_text(",");
  mp_ColSepComboEntry->append_text("#");
  mp_ColSepComboEntry->append_text("*");
  mp_ColSepComboEntry->append_text(EngineHelper::getBlankSubstitute());
  mp_ColSepComboEntry->append_text(EngineHelper::getTabSubstitute());
  mp_ColSepComboEntry->signal_changed().connect(sigc::mem_fun(*this,
      &SimulOutFilesAddEditDialog::onValueChange));

  mp_DateFormatComboEntry = Gtk::manage(new Gtk::ComboBoxEntryText());
  mp_DateFormatComboEntry->append_text("%Y%m%dT%H%M%S");
  mp_DateFormatComboEntry->append_text("%Y-%m-%dT%H:%M:%S");
  mp_DateFormatComboEntry->append_text("%Y-%m-%d %H:%M:%S");
  mp_DateFormatComboEntry->append_text("%Y %m %d %H %M %S");
  mp_DateFormatComboEntry->append_text("%d/%m/%Y %H:%M:%S");
  mp_DateFormatComboEntry->append_text("%Y-%m-%d %H.%M");
  mp_DateFormatComboEntry->signal_changed().connect(sigc::mem_fun(*this,
      &SimulOutFilesAddEditDialog::onValueChange));

  mp_CommentCharComboEntry = Gtk::manage(new Gtk::ComboBoxEntryText());
  mp_CommentCharComboEntry->append_text(":");
  mp_CommentCharComboEntry->append_text("%");
  mp_CommentCharComboEntry->append_text(";");
  mp_CommentCharComboEntry->append_text(",");
  mp_CommentCharComboEntry->append_text("#");
  mp_CommentCharComboEntry->append_text("*");
  mp_CommentCharComboEntry->append_text(EngineHelper::getBlankSubstitute());
  mp_CommentCharComboEntry->append_text(EngineHelper::getTabSubstitute());
  mp_CommentCharComboEntry->signal_changed().connect(sigc::mem_fun(*this,
      &SimulOutFilesAddEditDialog::onValueChange));

  mp_Table = Gtk::manage(new Gtk::Table());
  mp_Table->attach(*Gtk::manage(new Gtk::Label(_("Format name"), 1, 0.5)), 0,
      1, 0, 1, Gtk::FILL | Gtk::SHRINK, Gtk::SHRINK);
  mp_Table->attach(*Gtk::manage(new Gtk::Label(_("Column separator"), 1, 0.5)),
      0, 1, 1, 2, Gtk::FILL | Gtk::SHRINK, Gtk::SHRINK);
  mp_Table->attach(*Gtk::manage(new Gtk::Label(_("Date format"), 1, 0.5)), 0,
      1, 2, 3, Gtk::FILL | Gtk::SHRINK, Gtk::SHRINK);
  mp_Table->attach(
      *Gtk::manage(new Gtk::Label(_("Comment character"), 1, 0.5)), 0, 1, 3, 4,
      Gtk::FILL, Gtk::SHRINK);
  mp_Table->attach(*mp_FormatNameLabel, 1, 2, 0, 1, Gtk::EXPAND, Gtk::SHRINK);
  mp_Table->attach(*mp_ColSepComboEntry, 1, 2, 1, 2, Gtk::EXPAND, Gtk::SHRINK);
  mp_Table->attach(*mp_DateFormatComboEntry, 1, 2, 2, 3, Gtk::EXPAND,
      Gtk::SHRINK);
  mp_Table->attach(*mp_CommentCharComboEntry, 1, 2, 3, 4, Gtk::EXPAND,
      Gtk::SHRINK);
  mp_Table->set_col_spacings(3);
  mp_Table->set_row_spacings(5);
  mp_Table->set_border_width(5);

  mp_Dialog = new Gtk::Dialog();
  mp_Dialog->get_vbox()->pack_start(*mp_InfoBar, Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*mp_Table);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================


void SimulOutFilesAddEditDialog::setEngineRequirements(
    openfluid::base::OutputDescriptor& OutDesc)
{
  mp_OutDesc = &OutDesc;
}

// =====================================================================
// =====================================================================

void SimulOutFilesAddEditDialog::onValueChange()
{
  bool m_IsValid = false;

  if (mp_ColSepComboEntry->get_entry()->get_text().empty())
    mp_InfoBarLabel->set_text(_("Column separator cannot be empty"));
  else if (mp_DateFormatComboEntry->get_entry()->get_text().empty())
    mp_InfoBarLabel->set_text(_("Date format cannot be empty"));
  else if (mp_CommentCharComboEntry->get_entry()->get_text().empty())
    mp_InfoBarLabel->set_text(_("Comment character cannot be empty"));
  else
    m_IsValid = true;

  mp_InfoBar->set_visible(!m_IsValid);
  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, m_IsValid);
}

// =====================================================================
// =====================================================================


openfluid::base::OutputFilesDescriptor* SimulOutFilesAddEditDialog::show(
    openfluid::base::OutputFilesDescriptor* OutFilesDesc)
{
  // edition mode
  if (OutFilesDesc)
  {
    mp_Dialog->set_title(_("Files format edition"));

    mp_FormatNameLabel->set_text(OutFilesDesc->getName());

    mp_ColSepComboEntry->get_entry()->set_text(
        EngineHelper::fromRealCharToSubstitute(OutFilesDesc->getColSeparator()));
    mp_CommentCharComboEntry->get_entry()->set_text(
        EngineHelper::fromRealCharToSubstitute(OutFilesDesc->getCommentChar()));
    mp_DateFormatComboEntry->get_entry()->set_text(
        OutFilesDesc->getDateFormat());
  }
  //add mode
  else
  {
    mp_Dialog->set_title(_("Files format creation"));

    m_FileFormatNames.clear();
    for (unsigned int i = 0; i < mp_OutDesc->getFileSets().size(); i++)
      m_FileFormatNames.insert(mp_OutDesc->getFileSets()[i].getName());

    int NextIndex = mp_OutDesc->getFileSets().size() + 1;
    std::string NextFormatName =
        Glib::ustring::compose("Format #%1", NextIndex);
    while (m_FileFormatNames.find(NextFormatName) != m_FileFormatNames.end())
    {
      NextFormatName = Glib::ustring::compose("Format #%1", ++NextIndex);
    }

    mp_FormatNameLabel->set_text(NextFormatName);

    mp_ColSepComboEntry->set_active(0);
    mp_CommentCharComboEntry->set_active(0);
    mp_DateFormatComboEntry->set_active(0);
  }

  onValueChange();

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    if (OutFilesDesc)
    {
      OutFilesDesc->setColSeparator(EngineHelper::fromSubstituteToRealChar(
          mp_ColSepComboEntry->get_entry()->get_text()));
      OutFilesDesc->setCommentChar(EngineHelper::fromSubstituteToRealChar(
          mp_CommentCharComboEntry->get_entry()->get_text()));
      OutFilesDesc->setDateFormat(
          mp_DateFormatComboEntry->get_entry()->get_text());

      mp_Dialog->hide();

      return OutFilesDesc;
    }
    else
    {
      openfluid::base::OutputFilesDescriptor* NewOutFilesDesc =
          new openfluid::base::OutputFilesDescriptor();

      NewOutFilesDesc->setName(mp_FormatNameLabel->get_text());
      NewOutFilesDesc->setColSeparator(EngineHelper::fromSubstituteToRealChar(
          mp_ColSepComboEntry->get_entry()->get_text()));
      NewOutFilesDesc->setCommentChar(EngineHelper::fromSubstituteToRealChar(
          mp_CommentCharComboEntry->get_entry()->get_text()));
      NewOutFilesDesc->setDateFormat(
          mp_DateFormatComboEntry->get_entry()->get_text());

      mp_Dialog->hide();

      return NewOutFilesDesc;
    }
  }

  mp_Dialog->hide();

  return (openfluid::base::OutputFilesDescriptor*) 0;
}

// =====================================================================
// =====================================================================

