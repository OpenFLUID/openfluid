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
  @file WareStatusDashboardWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#ifndef __OPENFLUID_UIWARESDEV_WARESTATUSDASHBOARDWIDGET_HPP__
#define __OPENFLUID_UIWARESDEV_WARESTATUSDASHBOARDWIDGET_HPP__


#include <QPixmap>
#include <QPushButton>

#include <openfluid/waresdev/WareSrcChecker.hpp>


namespace Ui
{
  class WareStatusDashboardWidget;
}


namespace openfluid { namespace ui { namespace waresdev {
class WareStatusDashboardWidget : public QWidget
{
  Q_OBJECT;

  private:

    Ui::WareStatusDashboardWidget* ui;

    std::string m_WarePath;

    QString m_TitleColor;

    QWidget* mp_MessageContainer;

    void setLevel(const openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus Level);

    void applyStyleSheet();

    void addMessage(openfluid::waresdev::WareSrcChecker::ReportingData::ReportingItem Item);

    void clear();


  public slots:

    void refresh();

    void onActionTriggered(const QString& ActionCode);


  signals:
  
    void operationRequested(const QString& ActionCode);


  public:

    WareStatusDashboardWidget(QWidget* Parent, const std::string& WarePath);

    ~WareStatusDashboardWidget();

};

} } }  // namespaces

#endif /* __OPENFLUID_UIWARESDEV_WARESTATUSDASHBOARDWIDGET_HPP__ */
