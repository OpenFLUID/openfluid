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
 * MarketPackWidgetFormat.cpp
 *
 *  Created on: 21 mars 2013
 *      Author: Manuel CHATAIGNER
*/


#include <openfluid/guicommon/MarketPackWidgetFormat.hpp>
#include <openfluid/guicommon/MarketBuildOptionsDialog.hpp>
#include <openfluid/market/MarketPackage.hpp>

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>

namespace openfluid { namespace guicommon {


MarketPackWidgetFormat::MarketPackWidgetFormat(const openfluid::market::PackageInfo::TypePackage& TypePackage,
    const openfluid::market::MetaPackageInfo& MetaPackInfo)
    : MarketPackWidget(TypePackage, MetaPackInfo), m_EditedBuildOptions(""), m_FormatLabel(_("Package Format:"))
{

  m_RefFormatComboBoxModel = Gtk::ListStore::create(m_FormatColumns);
  m_FormatCombo.set_model(m_RefFormatComboBoxModel);

  m_FormatCombo.pack_start(m_FormatColumns.m_FormatName);

  Gtk::TreeModel::Row TmpFormatRow;

  if (MetaPackInfo.AvailablePackages.find(openfluid::market::MetaPackageInfo::SRC) != MetaPackInfo.AvailablePackages.end())
  {
    TmpFormatRow = *(m_RefFormatComboBoxModel->append());
    TmpFormatRow[m_FormatColumns.m_FormatName] = _("source");
    TmpFormatRow[m_FormatColumns.m_SelType] = openfluid::market::MetaPackageInfo::SRC;
    m_EditedBuildOptions = m_MetaPackInfo.AvailablePackages[openfluid::market::MetaPackageInfo::SRC].BuildOptions;
  }

  if (MetaPackInfo.AvailablePackages.find(openfluid::market::MetaPackageInfo::BIN) != MetaPackInfo.AvailablePackages.end())
  {
    TmpFormatRow = *(m_RefFormatComboBoxModel->append());
    TmpFormatRow[m_FormatColumns.m_FormatName] = _("binary");
    TmpFormatRow[m_FormatColumns.m_SelType] = openfluid::market::MetaPackageInfo::BIN;
  }

  m_FormatCombo.set_active(0);

  Gtk::Image* Img = Gtk::manage(new Gtk::Image(Gtk::Stock::PREFERENCES, Gtk::ICON_SIZE_BUTTON));
  m_ConfigButton.add(*Img);
  m_ConfigButton.set_image_position(Gtk::POS_LEFT);
  m_ConfigButton.set_tooltip_text(_("Edit build options for ") + MetaPackInfo.ID);
  m_ConfigButton.show_all_children(true);

  m_FormatHBox.pack_start(m_FormatLabel,Gtk::PACK_SHRINK,6);
  m_FormatHBox.pack_start(m_FormatCombo,Gtk::PACK_SHRINK);
  m_FormatHBox.pack_start(m_ConfigButton,Gtk::PACK_SHRINK,6);

  m_DetailsLeftVBox.pack_start(m_IDLabel,Gtk::PACK_EXPAND_WIDGET);
  m_DetailsLeftVBox.pack_start(m_FormatHBox,Gtk::PACK_EXPAND_WIDGET);

  m_MainHBox.pack_start(m_InstallToggle,Gtk::PACK_SHRINK,12);
  m_MainHBox.pack_start(m_DetailsLeftVBox,Gtk::PACK_EXPAND_WIDGET,12);
  m_MainHBox.pack_start(m_DetailsRightVBox,Gtk::PACK_EXPAND_WIDGET,12);

  updateDisplayedInfos();

  add(m_MainHBox);

  m_FormatCombo.signal_changed().connect(sigc::mem_fun(*this,
        &MarketPackWidgetFormat::onInstallModified));

  m_ConfigButton.signal_clicked().connect(sigc::mem_fun(*this,
      &MarketPackWidgetFormat::onConfigClicked));
}


// =====================================================================
// =====================================================================


void MarketPackWidgetFormat::onConfigClicked()
{
  MarketBuildOptionsDialog OptDialog(openfluid::market::MarketPackage::getCommonBuildOptions(m_TypePackage),
                                     m_EditedBuildOptions,m_MetaPackInfo.ID);


  if (OptDialog.run() == Gtk::RESPONSE_OK)
    m_EditedBuildOptions = OptDialog.getEditedOptions();

  onInstallModified();
}


// =====================================================================
// =====================================================================


openfluid::market::MetaPackageInfo::SelectionType MarketPackWidgetFormat::getPackageFormat() const
{
  Gtk::TreeModel::iterator TmpIter = m_FormatCombo.get_active();
  if (TmpIter)
  {
    Gtk::TreeModel::Row TmpRow = *TmpIter;
    if (TmpRow)
    {
      return TmpRow[m_FormatColumns.m_SelType];
    }
  }

  return openfluid::market::MetaPackageInfo::NONE;
}


// =====================================================================
// =====================================================================


void MarketPackWidgetFormat::updateDisplayedInfos()
{
  MarketPackWidget::updateDisplayedInfos();

  std::string MarkupTooltip = get_tooltip_markup();
  openfluid::market::MetaPackageInfo::SelectionType SelType;

  Gtk::TreeModel::iterator TmpIter = m_FormatCombo.get_active();
  if (TmpIter)
  {
    Gtk::TreeModel::Row TmpRow = *TmpIter;
    if (TmpRow)
    {
      SelType = TmpRow[m_FormatColumns.m_SelType];
      std::string LicenseStr = (*(m_MetaPackInfo.AvailablePackages.find(SelType))).second.License;

      MarkupTooltip += std::string("\n<u>")+_("License:")+std::string("</u> ") + replaceByUnknownIfEmpty(LicenseStr);
      m_LicenseLabel.set_markup(std::string("<u>")+_("License:")+std::string("</u> ")+replaceByUnknownIfEmpty(LicenseStr));

      if (SelType == openfluid::market::MetaPackageInfo::SRC)
      {
        MarkupTooltip += std::string("\n<u>")+_("Build options:")+std::string("</u> ") + replaceByNoneIfEmpty(openfluid::market::MarketPackage::composeFullBuildOptions(m_TypePackage,m_EditedBuildOptions));
      }
    }
  }

  if (!m_MetaPackInfo.Description.empty()) MarkupTooltip += "\n\n"+m_MetaPackInfo.Description;

  m_ConfigButton.set_sensitive(getPackageFormat() == openfluid::market::MetaPackageInfo::SRC);

  set_tooltip_markup(MarkupTooltip);
}


} } //namespaces
