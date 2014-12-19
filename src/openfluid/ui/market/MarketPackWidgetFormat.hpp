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
 * MarketPackWidgetFormat.hpp
 *
 *  Created on: 21 mars 2013
 *      Author: Manuel CHATAIGNER
*/

#ifndef __MARKETPACKWIDGETFORMAT__HPP__
#define __MARKETPACKWIDGETFORMAT__HPP__


#include <openfluid/ui/market/MarketPackWidget.hpp>
#include <QComboBox>
#include <QList>
#include <QStandardItemModel>


namespace openfluid { namespace ui { namespace market {


class OPENFLUID_API MarketPackWidgetFormat : public MarketPackWidget
{
  Q_OBJECT

  private:

    QString m_EditedBuildOptions;

    QHBoxLayout m_FormatHBox;
    QLabel m_FormatLabel;
    QComboBox m_FormatCombo;
    QPushButton m_ConfigButton;

    QStandardItemModel m_RefFormatComboBoxModel;

    class FormatComboColumns : public QList<QStandardItem*>
    {
      public:

        enum { NAME, TYPE};

        QStandardItem *mp_FormatName;
        QStandardItem *mp_SelType;

        FormatComboColumns() {}

        void appendItems() { clear(); append(mp_FormatName); append(mp_SelType); }
    };

    FormatComboColumns m_FormatColumns;

  private slots:

    void onConfigClicked();

  public:

    MarketPackWidgetFormat(const openfluid::market::PackageInfo::PackageType& Type,const openfluid::market::MetaPackageInfo& MetaPackInfo);

    virtual openfluid::market::MetaPackageInfo::SelectionType getPackageFormat() const;

    QString getEditedBuildOptions() const { return m_EditedBuildOptions; };

    virtual void updateDisplayedInfos();

};


} } } // namespaces


#endif /* __OPENFLUID_UIMARKET_MARKETPACKWIDGETFORMAT_HPP__ */
