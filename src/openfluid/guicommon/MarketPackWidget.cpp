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
  \file MarketPackWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <openfluid/guicommon/MarketPackWidget.hpp>
#include <openfluid/market/MarketPackage.hpp>
#include <openfluid/market/MarketClient.hpp>

#include <glibmm/i18n.h>

namespace openfluid { namespace guicommon {

const static Gdk::Color GREEN("#97DE62");
const static Gdk::Color LIGHTGREEN("#C7F1B3");
const static Gdk::Color DARKGREY("#555555");
const static Gdk::Color GREY("#AAAAAA");
const static Gdk::Color LIGHTGREY("#CCCCCC");
const static Gdk::Color WHITE("#FFFFFF");


// =====================================================================
// =====================================================================


MarketPackWidget::MarketPackWidget(const openfluid::market::PackageInfo::TypePackage& TypePackage,
    const openfluid::market::MetaPackageInfo& MetaPackInfo)
 : Gtk::EventBox(),
   m_TypePackage(TypePackage),
   m_MetaPackInfo(MetaPackInfo)
{
  m_EmptyCartImage = new Gtk::Image(openfluid::base::RuntimeEnvironment::getInstance()->getAppResourceFilePath("openfluid-market-client","shopping_cart.png"));
  m_FullCartImage = new Gtk::Image(openfluid::base::RuntimeEnvironment::getInstance()->getAppResourceFilePath("openfluid-market-client","shopping_cart_full.png"));

  m_InstallToggle.set_image(*m_EmptyCartImage);

  m_IDLabel.set_markup("<b>"+m_MetaPackInfo.ID+"</b>");
  m_IDLabel.set_use_markup(true);
  m_IDLabel.set_alignment(0,0);

  m_VersionLabel.set_justify(Gtk::JUSTIFY_RIGHT);
  m_VersionLabel.set_alignment(1.0,0);
  m_VersionLabel.modify_fg(Gtk::STATE_NORMAL,DARKGREY);

  m_LicenseLabel.set_justify(Gtk::JUSTIFY_RIGHT);
  m_LicenseLabel.set_alignment(1.0,0);
  m_LicenseLabel.modify_fg(Gtk::STATE_NORMAL,DARKGREY);


  set_border_width(0);

  m_MainHBox.set_border_width(10);


  m_DetailsRightVBox.pack_start(m_VersionLabel,Gtk::PACK_EXPAND_WIDGET);
  m_DetailsRightVBox.pack_start(m_LicenseLabel,Gtk::PACK_EXPAND_WIDGET);

  if (m_TypePackage == openfluid::market::PackageInfo::DATA)
  {
    // display in the middle
    m_DetailsLeftVBox.pack_start(m_IDLabel,Gtk::PACK_EXPAND_WIDGET, 15);

    m_MainHBox.pack_start(m_InstallToggle,Gtk::PACK_SHRINK,12);
    m_MainHBox.pack_start(m_DetailsLeftVBox,Gtk::PACK_EXPAND_WIDGET,12);
    m_MainHBox.pack_start(m_DetailsRightVBox,Gtk::PACK_EXPAND_WIDGET,12);
  }

  m_InstallToggle.modify_bg(Gtk::STATE_ACTIVE, GREY);
  m_InstallToggle.modify_bg(Gtk::STATE_NORMAL, LIGHTGREY);


  modify_bg(Gtk::STATE_NORMAL,WHITE);

  updateDisplayedInfos();

  if (m_TypePackage == openfluid::market::PackageInfo::DATA)
    add(m_MainHBox);

  m_InstallToggle.signal_toggled().connect(sigc::mem_fun(*this,
        &MarketPackWidget::onInstallModified));

  signal_button_release_event().connect(sigc::mem_fun(*this,
      &MarketPackWidget::onButtonRelease));

}


// =====================================================================
// =====================================================================


MarketPackWidget::~MarketPackWidget()
{
  delete m_EmptyCartImage;
  delete m_FullCartImage;
}


// =====================================================================
// =====================================================================


bool MarketPackWidget::onButtonRelease(GdkEventButton* Event)
{
  if (((GdkEventButton*)Event)->button == 1)
  m_InstallToggle.set_active(!m_InstallToggle.get_active());
  return true;
}


// =====================================================================
// =====================================================================


void MarketPackWidget::onInstallModified()
{
  if (m_InstallToggle.get_active())
  {
    m_signal_install_modified.emit();
    modify_bg(Gtk::STATE_NORMAL,LIGHTGREEN);
    m_InstallToggle.set_image(*m_FullCartImage);
  }
  else
  {
    m_signal_install_modified.emit();
    modify_bg(Gtk::STATE_NORMAL ,WHITE);
    m_InstallToggle.set_image(*m_EmptyCartImage);
  }
  updateDisplayedInfos();
}


// =====================================================================
// =====================================================================


openfluid::market::MetaPackageInfo::SelectionType MarketPackWidget::getPackageFormat() const
{
  return openfluid::market::MetaPackageInfo::FLUIDX;
}


// =====================================================================
// =====================================================================


void MarketPackWidget::updateDisplayedInfos()
{
  std::string MarkupTooltip = "";
  openfluid::market::MetaPackageInfo::SelectionType SelType;

  m_VersionLabel.set_markup(std::string("<u>")+_("Version:")+std::string("</u> ")+ replaceByUnknownIfEmpty(m_MetaPackInfo.Version));

  MarkupTooltip += "<b>" + m_MetaPackInfo.ID + "</b>";

  MarkupTooltip += std::string("\n<u>")+_("Version:")+std::string("</u> ") + replaceByUnknownIfEmpty(m_MetaPackInfo.Version);
  MarkupTooltip += std::string("\n<u>")+_("Author(s):")+std::string("</u> ") + replaceByUnknownIfEmpty(m_MetaPackInfo.Authors);

  if (m_MetaPackInfo.AvailablePackages.find(openfluid::market::MetaPackageInfo::FLUIDX) != m_MetaPackInfo.AvailablePackages.end())
  {
    SelType = openfluid::market::MetaPackageInfo::FLUIDX;
    std::string LicenseStr = (*(m_MetaPackInfo.AvailablePackages.find(SelType))).second.License;

    MarkupTooltip += std::string("\n<u>")+_("License:")+std::string("</u> ") + replaceByUnknownIfEmpty(LicenseStr);
    m_LicenseLabel.set_markup(std::string("<u>")+_("License:")+std::string("</u> ")+replaceByUnknownIfEmpty(LicenseStr));

    if (!m_MetaPackInfo.Description.empty()) MarkupTooltip += "\n\n"+m_MetaPackInfo.Description;

    // dependencies
    openfluid::market::PackageInfo::Dependencies_t Dependencies = m_MetaPackInfo.AvailablePackages[openfluid::market::MetaPackageInfo::FLUIDX].Dependencies;
    openfluid::market::PackageInfo::Dependencies_t::const_iterator DMit;
    std::list<openfluid::ware::WareID_t>::const_iterator DLit;

    MarkupTooltip += "\n\n<u>Dependencies:</u>";
    for (DMit = Dependencies.begin(); DMit != Dependencies.end(); ++DMit)
    {
      MarkupTooltip += "\n- "+openfluid::market::MarketClient::getTypeName(DMit->first, true, true)+": ";
      for (DLit = DMit->second.begin(); DLit != DMit->second.end(); ++DLit)
      {
        MarkupTooltip += "\n  " + *DLit;
      }
    }
  }

  set_tooltip_markup(MarkupTooltip);
}


// =====================================================================
// =====================================================================


MarketPackWidget::signal_install_modified_t MarketPackWidget::signal_install_modified()
{
  return m_signal_install_modified;
}


// =====================================================================
// =====================================================================


std::string MarketPackWidget::replaceByUnknownIfEmpty(const std::string& Str)
{
  if (Str.empty()) return _("<i>unknown</i>");
  return Str;
}



// =====================================================================
// =====================================================================


std::string MarketPackWidget::replaceByNoneIfEmpty(const std::string& Str)
{
  if (Str.empty()) return _("<i>none</i>");
  return Str;
}


} } //namespaces
