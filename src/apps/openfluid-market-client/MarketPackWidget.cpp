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
const static Gdk::Color GREY("#777777");

// =====================================================================
// =====================================================================


MarketPackWidget::MarketPackWidget(std::string ID, bool IsBin, bool IsSrc)
 : Gtk::Frame(),
   m_ID(ID),
   m_FormatLabel("Package Format:")
{
  //set_shadow_type(Gtk::SHADOW_NONE);

  Gtk::Image* TImage = new Gtk::Image(openfluid::base::RuntimeEnvironment::getInstance()->getAppResourceFilePath("openfluid-market-client","shopping_cart.png"));

  m_InstallToggle.set_image(*TImage);

  m_IDLabel.set_markup("<b>"+m_ID+"</b>");
  m_IDLabel.set_use_markup(true);
  m_IDLabel.set_alignment(0,0);

  set_border_width(5);

  m_MainHBox.set_border_width(10);

  m_RefFormatComboBoxModel = Gtk::ListStore::create(m_FormatColumns);
  m_FormatCombo.set_model(m_RefFormatComboBoxModel);

  m_FormatCombo.pack_start(m_FormatColumns.m_FormatName);

  Gtk::TreeModel::Row TmpFormatRow;

  if (IsSrc)
  {
    TmpFormatRow = *(m_RefFormatComboBoxModel->append());
    TmpFormatRow[m_FormatColumns.m_FormatName] = "source";
    TmpFormatRow[m_FormatColumns.m_SelType] = openfluid::market::MetaPackageInfo::SRC;
  }

  if (IsBin)
  {
    TmpFormatRow = *(m_RefFormatComboBoxModel->append());
    TmpFormatRow[m_FormatColumns.m_FormatName] = "binary";
    TmpFormatRow[m_FormatColumns.m_SelType] = openfluid::market::MetaPackageInfo::BIN;
  }

  m_FormatCombo.set_active(0);

  m_FormatHBox.pack_start(m_FormatLabel,Gtk::PACK_SHRINK,6);
  m_FormatHBox.pack_start(m_FormatCombo,Gtk::PACK_SHRINK);

  m_DetailsVBox.pack_start(m_IDLabel,Gtk::PACK_SHRINK);
  m_DetailsVBox.pack_start(m_FormatHBox,Gtk::PACK_SHRINK);

  m_MainHBox.pack_start(m_InstallToggle,Gtk::PACK_SHRINK,12);
  m_MainHBox.pack_start(m_DetailsVBox,Gtk::PACK_EXPAND_WIDGET,12);

  m_InstallToggle.signal_toggled().connect(sigc::mem_fun(*this,
      &MarketPackWidget::onInstallToggled));

  m_InstallToggle.modify_bg(Gtk::STATE_ACTIVE, GREEN);

  modify_bg(Gtk::STATE_NORMAL,GREY);

  add(m_MainHBox);
}


// =====================================================================
// =====================================================================


void MarketPackWidget::onInstallToggled()
{
  if (m_InstallToggle.get_active())
  {
    modify_bg(Gtk::STATE_NORMAL ,GREEN);
  }
  else
  {
    modify_bg(Gtk::STATE_NORMAL ,GREY);
  }

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

