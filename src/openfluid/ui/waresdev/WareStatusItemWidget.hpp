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
  @file WareStatusItemWidget.hpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_WARESTATUSITEMWIDGET_HPP__
#define __OPENFLUID_UIWARESDEV_WARESTATUSITEMWIDGET_HPP__


#include <optional>

#include <QWidget>

#include <openfluid/waresdev/WareSrcChecker.hpp>


namespace Ui
{
  class WareStatusItemWidget;
}


namespace openfluid { namespace ui { namespace waresdev {

class WareStatusItemWidget : public QWidget
{
  Q_OBJECT;

  private:

    typedef openfluid::waresdev::WareSrcChecker::ReportingData ReportingData;

    static std::map<std::pair<std::string, ReportingData::ReportingStatus>, 
             QString> ms_LabelForReportItem; // translatable text for each report item
    static std::map<std::pair<std::string, ReportingData::ReportingStatus>, 
             std::pair<QString, QString>> ms_ActionForReportItem; // action code and translatable button message

    Ui::WareStatusItemWidget* ui;

    QString m_ActionCode;

    void setStatusLevel(const ReportingData::ReportingStatus Level);

    QString getLabelForItem(const ReportingData::ReportingItem& Item);
    
    std::optional<std::pair<QString, QString>> getActionForItem(const ReportingData::ReportingItem& Item);


  private slots:
    
    void onButtonPressed();


  signals:

    void actionTriggered(QString ActionCode);


  public:
  
    static void populateReportItemLabels();

    WareStatusItemWidget(const ReportingData::ReportingItem& Item, 
                              QWidget* Parent = nullptr);

    ~WareStatusItemWidget();


};


} } }  // namespaces

#endif /* __OPENFLUID_UIWARESDEV_WARESTATUSITEMWIDGET_HPP__ */
