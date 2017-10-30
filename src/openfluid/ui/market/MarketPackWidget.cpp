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
  @file MarketPackWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/

#include <openfluid/ui/market/MarketPackWidget.hpp>
#include <openfluid/market/MarketPackage.hpp>
#include <openfluid/ui/market/MarketClientAssistant.hpp>

#include <QColor>

namespace openfluid { namespace ui { namespace market {

const static QColor GREEN("#97DE62");
const static QColor LIGHTGREEN("#C7F1B3");
const static QColor DARKGREY("#555555");
const static QColor GREY("#AAAAAA");
const static QColor LIGHTGREY("#CCCCCC");
const static QColor WHITE("#FFFFFF");


// =====================================================================
// =====================================================================


MarketPackWidget::MarketPackWidget(const openfluid::market::PackageInfo::PackageType& PackageType,
    const openfluid::market::MetaPackageInfo& MetaPackInfo)
 : QGroupBox(),
   m_PackageType(PackageType),
   m_MetaPackInfo(MetaPackInfo)
{
  setFixedHeight(90);

  mp_EmptyCartImage = new QLabel();
  mp_FullCartImage = new QLabel();

  mp_EmptyCartImage->setPixmap(QPixmap(":/ui/market/icons/shopping_cart.png"));
  mp_FullCartImage->setPixmap(QPixmap(":/ui/market/icons/shopping_cart_full.png"));

  m_InstallToggle.setCheckable(true);
  m_InstallToggle.setIcon(QIcon(*mp_EmptyCartImage->pixmap()));
  m_InstallToggle.setIconSize(QSize(40, 40));
  m_InstallToggle.setFixedSize(50, 50);

  m_IDLabel.setText("<b>"+QString::fromStdString(m_MetaPackInfo.ID)+"</b>");

  m_VersionLabel.setAlignment(Qt::AlignRight);
  m_VersionLabel.setStyleSheet("QLabel {color : " + DARKGREY.name() + "}");
  m_LicenseLabel.setAlignment(Qt::AlignRight);
  m_LicenseLabel.setStyleSheet("QLabel {color : " + DARKGREY.name() + "}");


  m_DetailsRightVBox.addWidget(&m_VersionLabel);
  m_DetailsRightVBox.addWidget(&m_LicenseLabel);


  m_DetailsLeftVBox.addWidget(&m_IDLabel);

  m_MainHBox.addWidget(&m_InstallToggle);
  m_MainHBox.addSpacing(15);
  m_MainHBox.addLayout(&m_DetailsLeftVBox);
  m_MainHBox.addLayout(&m_DetailsRightVBox);


  setStyleSheet("QGroupBox {background : " + WHITE.name() + "}");

  updateDisplayedInfos();

  if (m_PackageType == openfluid::market::PackageInfo::DATA)
    setLayout(&m_MainHBox);


  connect(&m_InstallToggle, SIGNAL(toggled(bool)), this, SLOT(onInstallModified()));

/*
  signal_button_release_event().connect(sigc::mem_fun(*this,
      &MarketPackWidget::onButtonRelease));
*/
}


// =====================================================================
// =====================================================================


MarketPackWidget::~MarketPackWidget()
{
  delete mp_EmptyCartImage;
  delete mp_FullCartImage;
}


// =====================================================================
// =====================================================================


void MarketPackWidget::onInstallModified()
{
  emit installModified(m_MetaPackInfo.ID);

  if (m_InstallToggle.isChecked())
  {
    setStyleSheet("QGroupBox {background : " + LIGHTGREEN.name() + "}");
    m_InstallToggle.setIcon(QIcon(*mp_FullCartImage->pixmap()));
  }
  else
  {
    setStyleSheet("QGroupBox {background : " + WHITE.name() + "}");
    m_InstallToggle.setIcon(QIcon(*mp_EmptyCartImage->pixmap()));
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
  QString MarkupTooltip("");
  openfluid::market::MetaPackageInfo::SelectionType SelType;

  m_VersionLabel.setText("<u>"+tr("Version:")+"</u> "+
                         replaceByUnknownIfEmpty(QString::fromStdString(m_MetaPackInfo.Version)));

  // Id
  MarkupTooltip += "<b>" + QString::fromStdString(m_MetaPackInfo.ID) + "</b>";

  // version
  MarkupTooltip += "<br/><u>"+tr("Version:")+"</u> " +
                   replaceByUnknownIfEmpty(QString::fromStdString(m_MetaPackInfo.Version));
  // author
  MarkupTooltip += "<br/><u>"+tr("Author(s):")+"</u> " +
                   replaceByUnknownIfEmpty(QString::fromStdString(m_MetaPackInfo.Authors));

  // for datasets
  if (m_MetaPackInfo.AvailablePackages.find(openfluid::market::MetaPackageInfo::FLUIDX) !=
      m_MetaPackInfo.AvailablePackages.end())
  {
    SelType = openfluid::market::MetaPackageInfo::FLUIDX;
    QString LicenseStr(QString::fromStdString((*(m_MetaPackInfo.AvailablePackages.find(SelType))).second.License));

    // license
    MarkupTooltip += "<br/><u>"+tr("License:")+"</u> " + replaceByUnknownIfEmpty(LicenseStr);
    m_LicenseLabel.setText("<u>"+tr("License:")+"</u> "+replaceByUnknownIfEmpty(LicenseStr));

    // description
    if (!m_MetaPackInfo.Description.empty())
      MarkupTooltip += "<br/><br/>"+QString::fromStdString(m_MetaPackInfo.Description);

    // dependencies
    openfluid::market::PackageInfo::Dependencies_t Dependencies =
        m_MetaPackInfo.AvailablePackages[openfluid::market::MetaPackageInfo::FLUIDX].Dependencies;
    openfluid::market::PackageInfo::Dependencies_t::const_iterator DMit;
    std::list<openfluid::ware::WareID_t>::const_iterator DLit;

    MarkupTooltip += "<br/><br/><u>" + tr("Dependencies:") + "</u>";
    for (DMit = Dependencies.begin(); DMit != Dependencies.end(); ++DMit)
    {
      MarkupTooltip += "<br/>"+MarketClientAssistant::getGraphicTypeName(DMit->first, true, true)+": ";
      for (DLit = DMit->second.begin(); DLit != DMit->second.end(); ++DLit)
      {
        MarkupTooltip += "<br/>  - " + QString::fromStdString(*DLit);
      }
    }
  }

  setToolTip(MarkupTooltip);
}


// =====================================================================
// =====================================================================


QString MarketPackWidget::replaceByUnknownIfEmpty(const QString& Str)
{
  if (Str.isEmpty())
    return tr("<i>unknown</i>");

  return Str;
}



// =====================================================================
// =====================================================================


QString MarketPackWidget::replaceByNoneIfEmpty(const QString& Str)
{
  if (Str.isEmpty())
    return tr("<i>none</i>");

  return Str;
}



} } } // namespaces
