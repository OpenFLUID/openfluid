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

#include "MarketPackWidget.hpp"


const static Gdk::Color GREEN("#97DE62");
const static Gdk::Color LIGHTGREEN("#C7F1B3");
const static Gdk::Color DARKGREY("#555555");
const static Gdk::Color GREY("#AAAAAA");
const static Gdk::Color LIGHTGREY("#CCCCCC");
const static Gdk::Color WHITE("#FFFFFF");

// =====================================================================
// =====================================================================


MarketPackWidget::MarketPackWidget(const openfluid::market::MetaPackageInfo& MetaPackInfo)
 : Gtk::EventBox(),
   m_MetaPackInfo(MetaPackInfo),
   m_FormatLabel("Package Format:")
{

  Gtk::Image* TImage = new Gtk::Image(openfluid::base::RuntimeEnvironment::getInstance()->getAppResourceFilePath("openfluid-market-client","shopping_cart.png"));

  m_InstallToggle.set_image(*TImage);

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

  m_RefFormatComboBoxModel = Gtk::ListStore::create(m_FormatColumns);
  m_FormatCombo.set_model(m_RefFormatComboBoxModel);

  m_FormatCombo.pack_start(m_FormatColumns.m_FormatName);

  Gtk::TreeModel::Row TmpFormatRow;

  if (MetaPackInfo.AvailablePackages.find(openfluid::market::MetaPackageInfo::SRC) != MetaPackInfo.AvailablePackages.end())
  {
    TmpFormatRow = *(m_RefFormatComboBoxModel->append());
    TmpFormatRow[m_FormatColumns.m_FormatName] = "source";
    TmpFormatRow[m_FormatColumns.m_SelType] = openfluid::market::MetaPackageInfo::SRC;
  }

  if (MetaPackInfo.AvailablePackages.find(openfluid::market::MetaPackageInfo::BIN) != MetaPackInfo.AvailablePackages.end())
  {
    TmpFormatRow = *(m_RefFormatComboBoxModel->append());
    TmpFormatRow[m_FormatColumns.m_FormatName] = "binary";
    TmpFormatRow[m_FormatColumns.m_SelType] = openfluid::market::MetaPackageInfo::BIN;
  }

  m_FormatCombo.set_active(0);

  m_FormatHBox.pack_start(m_FormatLabel,Gtk::PACK_SHRINK,6);
  m_FormatHBox.pack_start(m_FormatCombo,Gtk::PACK_SHRINK);

  m_DetailsLeftVBox.pack_start(m_IDLabel,Gtk::PACK_EXPAND_WIDGET);
  m_DetailsLeftVBox.pack_start(m_FormatHBox,Gtk::PACK_EXPAND_WIDGET);

  m_DetailsRightVBox.pack_start(m_VersionLabel,Gtk::PACK_EXPAND_WIDGET);
  m_DetailsRightVBox.pack_start(m_LicenseLabel,Gtk::PACK_EXPAND_WIDGET);


  m_MainHBox.pack_start(m_InstallToggle,Gtk::PACK_SHRINK,12);
  m_MainHBox.pack_start(m_DetailsLeftVBox,Gtk::PACK_EXPAND_WIDGET,12);
  m_MainHBox.pack_start(m_DetailsRightVBox,Gtk::PACK_EXPAND_WIDGET,12);

  m_InstallToggle.modify_bg(Gtk::STATE_ACTIVE, GREY);
  m_InstallToggle.modify_bg(Gtk::STATE_NORMAL, LIGHTGREY);


  modify_bg(Gtk::STATE_NORMAL,WHITE);

  updateDisplayedInfos();


  add(m_MainHBox);

  m_InstallToggle.signal_toggled().connect(sigc::mem_fun(*this,
        &MarketPackWidget::onInstallModified));

  m_FormatCombo.signal_changed().connect(sigc::mem_fun(*this,
        &MarketPackWidget::onInstallModified));

  signal_button_release_event().connect(sigc::mem_fun(*this,
      &MarketPackWidget::onButtonRelease));


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
  }
  else
  {
    m_signal_install_modified.emit();
    modify_bg(Gtk::STATE_NORMAL ,WHITE);
  }
  updateDisplayedInfos();
}


// =====================================================================
// =====================================================================


openfluid::market::MetaPackageInfo::SelectionType MarketPackWidget::getPackageFormat() const
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


void MarketPackWidget::updateDisplayedInfos()
{
  std::string MarkupTooltip = "";
  openfluid::market::MetaPackageInfo::SelectionType SelType;

  m_VersionLabel.set_markup("<u>Version:</u> "+ replaceByUnknownIfEmpty(m_MetaPackInfo.Version));


  MarkupTooltip += "<b>" + m_MetaPackInfo.ID + "</b>";


  MarkupTooltip += "\n<u>Version:</u> " + replaceByUnknownIfEmpty(m_MetaPackInfo.Version);
  MarkupTooltip += "\n<u>Author(s):</u> " + replaceByUnknownIfEmpty(m_MetaPackInfo.Authors);

  Gtk::TreeModel::iterator TmpIter = m_FormatCombo.get_active();
  if (TmpIter)
  {
    Gtk::TreeModel::Row TmpRow = *TmpIter;
    if (TmpRow)
    {
      SelType = TmpRow[m_FormatColumns.m_SelType];
      MarkupTooltip += "\n<u>License :</u> " + replaceByUnknownIfEmpty(m_MetaPackInfo.AvailablePackages.at(SelType).License);

      m_LicenseLabel.set_markup("<u>License:</u> "+replaceByUnknownIfEmpty(m_MetaPackInfo.AvailablePackages.at(SelType).License));
    }
  }


  if (!m_MetaPackInfo.Description.empty()) MarkupTooltip += "\n\n"+m_MetaPackInfo.Description;


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
  if (Str.empty()) return "<i>unknown</i>";
  return Str;
}

