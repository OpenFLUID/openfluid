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
  @file TimePeriodWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UICOMMON_TIMEPERIODWIDGET_HPP__
#define __OPENFLUID_UICOMMON_TIMEPERIODWIDGET_HPP__


#include <QWidget>
#include <QDateTime>

#include <openfluid/dllexport.hpp>


namespace Ui
{
  class TimePeriodWidget;
}


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API TimePeriodWidget : public QWidget
{
  Q_OBJECT;


  private slots:

    void handleBeginChanged(const QDateTime& DT);

    void handleEndChanged(const QDateTime& DT);

    void handleLinkModeChanged();


  private:

    Ui::TimePeriodWidget *ui;

    QIcon m_LinkedIcon;

    QIcon m_UnlinkedIcon;

    bool m_LinkMode;

    qint64 m_FixedDiff;


  signals:

    void datesChanged(const QDateTime&, const QDateTime&);

    void linkModeChanged(bool);


  public:

    explicit TimePeriodWidget(QWidget* Parent = nullptr);

    virtual ~TimePeriodWidget();

    void initialize(const QDateTime& BeginDate, const QDateTime& EndDate);

    QDateTime getBeginDateTime() const;

    void setBeginDateTime(const QDateTime& DT);

    QDateTime getEndDateTime() const;

    void setEndDateTime(const QDateTime& DT);
};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_TIMEPERIODWIDGET_HPP__ */
