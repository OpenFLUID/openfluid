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
 \file BuilderHelper.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "BuilderGraphicsHelper.hpp"

#include <gtkmm/iconfactory.h>
#include <gtkmm/icontheme.h>
#include <giomm/emblem.h>
#include <boost/filesystem.hpp>

#include <openfluid/base/RuntimeEnv.hpp>

// =====================================================================
// =====================================================================

Gtk::StockID* BuilderGraphicsHelper::createIconStockId(
    Glib::ustring FilePath, Glib::ustring FileName, Glib::ustring StockIdString)
{
  Gtk::IconSource IconSource;

  IconSource.set_filename(FilePath + "/" + FileName);

  Gtk::IconSet IconSet;

  IconSet.add_source(IconSource);

  Glib::RefPtr<Gtk::IconFactory> BuilderIconFactory =
      Gtk::IconFactory::create();

  BuilderIconFactory->add_default();

  Gtk::StockID* StockId = new Gtk::StockID(StockIdString);

  BuilderIconFactory->add(*StockId, IconSet);

  return StockId;

}

// =====================================================================
// =====================================================================

Gdk::Color& BuilderGraphicsHelper::applyColorAlpha(Gdk::Color& Color,
                                                   double Alpha)
{

  if (Alpha > 0 && Alpha < 1)
  {
    int R = (1 - Alpha) * 65535 + Alpha * Color.get_red();
    int G = (1 - Alpha) * 65535 + Alpha * Color.get_green();
    int B = (1 - Alpha) * 65535 + Alpha * Color.get_blue();
    Color.set_rgb(R, G, B);
  }

  return Color;
}

// =====================================================================
// =====================================================================

Glib::ustring BuilderGraphicsHelper::getPathForFileName(Glib::ustring FileName)
{
  std::string Path =
      openfluid::base::RuntimeEnvironment::getInstance()->getAppResourceFilePath(
          "openfluid-builder", FileName);
  if (!boost::filesystem::exists(Path))
    Path =
        openfluid::base::RuntimeEnvironment::getInstance()->getCommonResourceFilePath(
            FileName);
  if (!boost::filesystem::exists(Path))
    Path = "";
  return Path;
}

// =====================================================================
// =====================================================================

Gtk::Image* BuilderGraphicsHelper::createImageFromFileName(
    Glib::ustring FileName)
{
  return new Gtk::Image(getPathForFileName(FileName));
}

// =====================================================================
// =====================================================================

Gtk::StockID* BuilderGraphicsHelper::createBuilderIconStockId(
    Glib::ustring IconFileName, Glib::ustring StockIdString)
{
  Gtk::IconSource IconSource;
  IconSource.set_filename(
      openfluid::base::RuntimeEnvironment::getInstance()->getAppResourceFilePath(
          "openfluid-builder", IconFileName));

  Gtk::IconSet IconSet;
  IconSet.add_source(IconSource);

  Glib::RefPtr<Gtk::IconFactory> BuilderIconFactory =
      Gtk::IconFactory::create();
  BuilderIconFactory->add_default();

  Gtk::StockID * StockId = new Gtk::StockID(StockIdString);

  BuilderIconFactory->add(*StockId, IconSet);

  return StockId;
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gdk::Pixbuf> BuilderGraphicsHelper::createSmallPixbufFromFile(
    Glib::ustring FileName)
{
  return Gdk::Pixbuf::create_from_file(getPathForFileName(FileName), 8, -1,
                                       true);
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gdk::Pixbuf> BuilderGraphicsHelper::createPixbufFromFile(
    Glib::ustring FileName)
{
  return Gdk::Pixbuf::create_from_file(getPathForFileName(FileName));

}

// =====================================================================
// =====================================================================

Gtk::Image* BuilderGraphicsHelper::createEmblemedImage(
    Gtk::StockID IconStockID, int IconStockSize, Gtk::StockID EmblemIconStockID,
    int EmblemIconStockSize, Gtk::IconSize ImageSize)
{
  Glib::RefPtr<Gtk::IconTheme> Theme = Gtk::IconTheme::get_default();

  Gtk::IconInfo IconInfo = Theme->lookup_icon(IconStockID.get_string(),
                                              IconStockSize,
                                              Gtk::ICON_LOOKUP_USE_BUILTIN);

  Glib::RefPtr<Gio::Icon> Icon = Gio::Icon::create(IconInfo.get_filename());

  Gtk::IconInfo EmblemIconInfo = Theme->lookup_icon(
      EmblemIconStockID.get_string(), EmblemIconStockSize,
      Gtk::ICON_LOOKUP_USE_BUILTIN);

  Glib::RefPtr<Gio::Icon> EmblemIcon = Gio::Icon::create(
      EmblemIconInfo.get_filename());

  Glib::RefPtr<Gio::Emblem> Emblem = Gio::Emblem::create(EmblemIcon);
  // giomm version doesn't work
  GIcon* Emblemed = g_emblemed_icon_new(Icon->gobj(), Emblem->gobj());

  Gtk::Image* Image = Gtk::manage(new Gtk::Image());
  Image->set(Glib::wrap(Emblemed), ImageSize);

  return Image;
}
