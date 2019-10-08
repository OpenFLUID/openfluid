/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
 @file MarketPackWidgetFormat.cpp

 @author Manuel Chataigner <manuel.chataigner@supagro.inra.fr>
*/


#include <openfluid/ui/market/MarketPackWidgetFormat.hpp>
#include <openfluid/ui/market/MarketBuildOptionsDialog.hpp>
#include <openfluid/market/MarketPackage.hpp>


namespace openfluid { namespace ui { namespace market {


MarketPackWidgetFormat::MarketPackWidgetFormat(const openfluid::market::PackageInfo::PackageType& Type,
    const openfluid::market::MetaPackageInfo& MetaPackInfo)
    : MarketPackWidget(Type, MetaPackInfo), m_EditedBuildOptions(""), m_FormatLabel(tr("Package Format:"))
{

  m_RefFormatComboBoxModel.setRowCount(0);
  m_RefFormatComboBoxModel.setColumnCount(2);
  m_FormatCombo.setModel(&m_RefFormatComboBoxModel);


  // source
  if (MetaPackInfo.AvailablePackages.find(openfluid::market::MetaPackageInfo::SRC) !=
      MetaPackInfo.AvailablePackages.end())
  {
    m_FormatColumns.mp_FormatName = new QStandardItem(tr("source"));
    m_FormatColumns.mp_SelType = new QStandardItem();
    m_FormatColumns.mp_SelType->setData(QVariant(openfluid::market::MetaPackageInfo::SRC));

    m_FormatColumns.appendItems();
    m_RefFormatComboBoxModel.appendRow(m_FormatColumns);

    m_EditedBuildOptions =
        QString::fromStdString(m_MetaPackInfo.AvailablePackages[openfluid::market::MetaPackageInfo::SRC].BuildOptions);
  }

  // binary
  if (MetaPackInfo.AvailablePackages.find(openfluid::market::MetaPackageInfo::BIN) !=
      MetaPackInfo.AvailablePackages.end())
  {
    m_FormatColumns.mp_FormatName = new QStandardItem(tr("binary"));
    m_FormatColumns.mp_SelType = new QStandardItem();
    m_FormatColumns.mp_SelType->setData(QVariant(openfluid::market::MetaPackageInfo::BIN));

    m_FormatColumns.appendItems();
    m_RefFormatComboBoxModel.appendRow(m_FormatColumns);
  }

  m_FormatCombo.setCurrentIndex(0);


  m_ConfigButton.setIcon(QIcon(":/ui/market/icons/preferences.png"));
  m_ConfigButton.setToolTip(tr("Edit build options for ") + QString::fromStdString(MetaPackInfo.ID));


  m_FormatHBox.setSpacing(5);
  m_FormatHBox.setAlignment(Qt::AlignLeft);
  m_FormatHBox.addWidget(&m_FormatLabel);
  m_FormatHBox.addWidget(&m_FormatCombo);
  m_FormatHBox.addWidget(&m_ConfigButton);

  // Add of format box
  m_DetailsLeftVBox.addLayout(&m_FormatHBox);


  updateDisplayedInfos();

  setLayout(&m_MainHBox);

  connect(&m_FormatCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onInstallModified()));
  connect(&m_ConfigButton, SIGNAL(clicked()), this, SLOT(onConfigClicked()));
}


// =====================================================================
// =====================================================================


void MarketPackWidgetFormat::onConfigClicked()
{
  MarketBuildOptionsDialog OptDialog(
      QString::fromStdString(openfluid::market::MarketPackage::getCommonBuildOptions(m_PackageType)),
      m_EditedBuildOptions,QString::fromStdString(m_MetaPackInfo.ID));


  if (OptDialog.exec() == QDialog::Accepted)
  {
    m_EditedBuildOptions = OptDialog.getEditedOptions();
  }

  onInstallModified();
}


// =====================================================================
// =====================================================================


openfluid::market::MetaPackageInfo::SelectionType MarketPackWidgetFormat::getPackageFormat() const
{
  int CurrRow = m_FormatCombo.currentIndex();

  return (openfluid::market::MetaPackageInfo::SelectionType)
      m_RefFormatComboBoxModel.item(CurrRow, FormatComboColumns::TYPE)->data().toInt();
}


// =====================================================================
// =====================================================================


void MarketPackWidgetFormat::updateDisplayedInfos()
{
  MarketPackWidget::updateDisplayedInfos();

  // get tooltip set in MarketPackWidget
  QString MarkupTooltip = toolTip();
  openfluid::market::MetaPackageInfo::SelectionType SelType;

  SelType = getPackageFormat();

  QString LicenseStr = QString::fromStdString((*(m_MetaPackInfo.AvailablePackages.find(SelType))).second.License);

  // license
  MarkupTooltip += "<br/><u>"+tr("License:")+"</u> " + replaceByUnknownIfEmpty(LicenseStr);
  m_LicenseLabel.setText("<u>"+tr("License:")+"</u> "+replaceByUnknownIfEmpty(LicenseStr));

  // build options
  if (SelType == openfluid::market::MetaPackageInfo::SRC)
  {
    QString BuildOptions =
        QString::fromStdString(
            openfluid::market::MarketPackage::composeFullBuildOptions(m_PackageType,
                                                                      m_EditedBuildOptions.toStdString()));
    MarkupTooltip += "<br/><u>"+tr("Build options:")+"</u> " + replaceByNoneIfEmpty(BuildOptions);
  }

  // description
  if (!m_MetaPackInfo.Description.empty())
  {
    MarkupTooltip += "<br/><br/>"+QString::fromStdString(m_MetaPackInfo.Description);
  }

  m_ConfigButton.setEnabled(getPackageFormat() == openfluid::market::MetaPackageInfo::SRC);

  setToolTip(MarkupTooltip);
}


} } } // namespaces
