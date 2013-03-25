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
 \file EnginProjectPreviewWidget.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "EngineProjectPreviewWidget.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <glibmm/i18n.h>
#include <gtkmm/separator.h>
#include <gtkmm/scrolledwindow.h>
#include <openfluid/base/ProjectManager.hpp>
#include "BuilderGraphicsHelper.hpp"

// =====================================================================
// =====================================================================

EngineProjectPreviewWidget::EngineProjectPreviewWidget()
{
  Gtk::Label* NameLabel = Gtk::manage(new Gtk::Label());
  NameLabel->set_markup(std::string("<i>") + _("Name:") + std::string("</i>"));
  NameLabel->set_alignment(0.0, 0.5);

  Gtk::Label* DescLabel = Gtk::manage(new Gtk::Label());
  DescLabel->set_markup(
      std::string("<i>") + _("Description:") + std::string("</i>"));
  DescLabel->set_alignment(0.0, 0.5);

  Gtk::Label* AuthorsLabel = Gtk::manage(new Gtk::Label());
  AuthorsLabel->set_markup(
      std::string("<i>") + _("Authors:") + std::string("</i>"));
  AuthorsLabel->set_alignment(0.0, 0.5);

  Gtk::Label* CreationDateLabel = Gtk::manage(new Gtk::Label());
  CreationDateLabel->set_markup(
      std::string("<i>") + _("Creation date:") + std::string("</i>"));
  CreationDateLabel->set_alignment(0.0, 0.5);

  Gtk::Label* LastModDateLabel = Gtk::manage(new Gtk::Label());
  LastModDateLabel->set_markup(
      std::string("<i>") + _("Modification date:") + std::string("</i>"));
  LastModDateLabel->set_alignment(0.0, 0.5);

  mp_ProjectName = Gtk::manage(
      new Gtk::Label("", Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, 5));
  mp_ProjectName->set_line_wrap(true);
  mp_ProjectName->set_line_wrap_mode(Pango::WRAP_WORD);

  mp_ProjectDesc = Gtk::manage(
      new Gtk::Label("", Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, 5));
  mp_ProjectDesc->set_line_wrap(true);
  mp_ProjectDesc->set_line_wrap_mode(Pango::WRAP_WORD);
  mp_ProjectDesc->set_size_request(220, -1);

  mp_ProjectAuthors = Gtk::manage(
      new Gtk::Label("", Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, 5));
  mp_ProjectAuthors->set_line_wrap(true);
  mp_ProjectAuthors->set_line_wrap_mode(Pango::WRAP_WORD);

  mp_ProjectCreationDate = Gtk::manage(
      new Gtk::Label("", Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, 5));
  mp_ProjectLastModDate = Gtk::manage(
      new Gtk::Label("", Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, 5));

  Gtk::VBox* PreviewInfoBox = Gtk::manage(new Gtk::VBox());
  PreviewInfoBox->pack_start(*NameLabel, Gtk::PACK_SHRINK);
  PreviewInfoBox->pack_start(*mp_ProjectName, Gtk::PACK_SHRINK);
  PreviewInfoBox->pack_start(*Gtk::manage(new Gtk::HSeparator()),
                                Gtk::PACK_SHRINK, 8);
  PreviewInfoBox->pack_start(*DescLabel, Gtk::PACK_SHRINK);
  PreviewInfoBox->pack_start(*mp_ProjectDesc, Gtk::PACK_SHRINK);
  PreviewInfoBox->pack_start(*Gtk::manage(new Gtk::HSeparator()),
                                Gtk::PACK_SHRINK, 8);
  PreviewInfoBox->pack_start(*AuthorsLabel, Gtk::PACK_SHRINK);
  PreviewInfoBox->pack_start(*mp_ProjectAuthors, Gtk::PACK_SHRINK);
  PreviewInfoBox->pack_start(*Gtk::manage(new Gtk::HSeparator()),
                                Gtk::PACK_SHRINK, 8);
  PreviewInfoBox->pack_start(*CreationDateLabel, Gtk::PACK_SHRINK);
  PreviewInfoBox->pack_start(*mp_ProjectCreationDate, Gtk::PACK_SHRINK);
  PreviewInfoBox->pack_start(*Gtk::manage(new Gtk::HSeparator()),
                                Gtk::PACK_SHRINK, 8);
  PreviewInfoBox->pack_start(*LastModDateLabel, Gtk::PACK_SHRINK);
  PreviewInfoBox->pack_start(*mp_ProjectLastModDate, Gtk::PACK_SHRINK);
  PreviewInfoBox->set_border_width(5);
  PreviewInfoBox->set_visible(true);
  PreviewInfoBox->show_all_children();

  Gtk::ScrolledWindow* DetailsWin = Gtk::manage(new Gtk::ScrolledWindow());
  DetailsWin->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  DetailsWin->set_shadow_type(Gtk::SHADOW_NONE);
  DetailsWin->add(*PreviewInfoBox);
  DetailsWin->set_visible(true);
  DetailsWin->show_all_children(true);

  Gtk::HBox* TitleBox = Gtk::manage(new Gtk::HBox());

  Gtk::Label* TitleLabel = Gtk::manage(new Gtk::Label());
  TitleLabel->set_markup(_("<b>Project details</b>"));
  TitleLabel->set_use_markup(true);
  TitleLabel->set_visible(true);

  Gtk::Image* OFImage = BuilderGraphicsHelper::createImageFromFileName(
      "openfluid_icon.png");
  OFImage->set_visible(true);

  TitleBox->pack_start(*OFImage, Gtk::PACK_SHRINK, 6);
  TitleBox->pack_start(*TitleLabel, Gtk::PACK_SHRINK, 6);
  TitleBox->show_all_children(true);
  TitleBox->set_visible(true);

  mp_PreviewBox = Gtk::manage(new Gtk::VBox());

  mp_PreviewBox->pack_start(*TitleBox, Gtk::PACK_SHRINK, 12);
  mp_PreviewBox->pack_start(*DetailsWin, Gtk::PACK_EXPAND_WIDGET, 6);
}

// =====================================================================
// =====================================================================

EngineProjectPreviewWidget::~EngineProjectPreviewWidget()
{

}

// =====================================================================
// =====================================================================

std::string EngineProjectPreviewWidget::replaceEmpty(std::string StringToCheck)
{
  if (StringToCheck.empty())
    return _("(none)");

  return StringToCheck;
}

// =====================================================================
// =====================================================================

Gtk::Widget* EngineProjectPreviewWidget::asWidget()
{
  return mp_PreviewBox;
}

// =====================================================================
// =====================================================================

bool EngineProjectPreviewWidget::update(std::string FileName)
{
  if (openfluid::base::ProjectManager::isProject(FileName))
  {
    openfluid::base::ProjectManager* Manager =
        openfluid::base::ProjectManager::getInstance();

    Manager->open(FileName);

    mp_ProjectName->set_text(replaceEmpty(Manager->getName()));
    mp_ProjectDesc->set_text(replaceEmpty(Manager->getDescription()));
    mp_ProjectAuthors->set_text(replaceEmpty(Manager->getAuthors()));
    mp_ProjectCreationDate->set_text(
        boost::posix_time::to_simple_string(
            boost::posix_time::from_iso_string(Manager->getCreationDate())));
    mp_ProjectLastModDate->set_text(
        boost::posix_time::to_simple_string(
            boost::posix_time::from_iso_string(Manager->getLastModDate())));

    Manager->close();

    return true;
  }

  mp_ProjectName->set_text(replaceEmpty(""));
  mp_ProjectDesc->set_text(replaceEmpty(""));
  mp_ProjectAuthors->set_text(replaceEmpty(""));
  mp_ProjectCreationDate->set_text(replaceEmpty(""));
  mp_ProjectLastModDate->set_text(replaceEmpty(""));

  return false;
}
