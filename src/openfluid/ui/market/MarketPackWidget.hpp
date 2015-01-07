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
  @file MarketPackWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_UIMARKET_MARKETPACKWIDGET_HPP__
#define __OPENFLUID_UIMARKET_MARKETPACKWIDGET_HPP__


#include <QLabel>
#include <QGroupBox>
#include <QBoxLayout>
#include <QPushButton>

#include <openfluid/dllexport.hpp>
#include <openfluid/market/MarketInfos.hpp>


namespace openfluid { namespace ui { namespace market {

// =====================================================================
// =====================================================================

class OPENFLUID_API MarketPackWidget : public QGroupBox
{
  Q_OBJECT

  private:
    QLabel *mp_EmptyCartImage;
    QLabel *mp_FullCartImage;

    QLabel m_VersionLabel;

    void setWidgetColor(QWidget *Widget);

    //bool onButtonRelease(GdkEventButton* Event);

  protected:
    openfluid::market::PackageInfo::PackageType m_PackageType;
    openfluid::market::MetaPackageInfo m_MetaPackInfo;

    QLabel m_IDLabel;
    QLabel m_LicenseLabel;

    QHBoxLayout m_MainHBox;

    QPushButton m_InstallToggle;

    QVBoxLayout m_DetailsLeftVBox;
    QVBoxLayout m_DetailsRightVBox;

    static QString replaceByUnknownIfEmpty(const QString& Str);

    static QString replaceByNoneIfEmpty(const QString& Str);

  protected slots:

    void onInstallModified();

  signals:

    void installModified(openfluid::ware::WareID_t);

  public:
    MarketPackWidget(const openfluid::market::PackageInfo::PackageType& Type,const openfluid::market::MetaPackageInfo& MetaPackInfo);

    ~MarketPackWidget();

    QString getID() const { return QString::fromStdString(m_MetaPackInfo.ID); };

    bool isInstall() const { return m_InstallToggle.isChecked(); };

    void setInstall(bool Install) { m_InstallToggle.setChecked(Install); };

    /**
     @return selected format in combobox
    */
    virtual openfluid::market::MetaPackageInfo::SelectionType getPackageFormat() const;

    /**
     Creates tooltip for market pack widget
    */
    virtual void updateDisplayedInfos();

};

} } } // namespaces


#endif /* __OPENFLUID_UIMARKET_MARKETPACKWIDGET_HPP__ */
