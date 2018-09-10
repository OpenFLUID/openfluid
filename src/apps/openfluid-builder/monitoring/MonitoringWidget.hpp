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
  @file MonitoringWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_MONITORINGWIDGET_HPP__
#define __OPENFLUID_BUILDERAPP_MONITORINGWIDGET_HPP__


#include <QWidget>

#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/fluidx/AdvancedMonitoringDescriptor.hpp>

#include "WorkspaceWidget.hpp"
#include "WaresManagementWidget.hpp"


namespace Ui
{
  class MonitoringWidget;
}


class MonitoringWidget : public WorkspaceWidget
{
  Q_OBJECT

  private:

    Ui::MonitoringWidget* ui;

    WaresManagementWidget* mp_WaresManWidget;

    openfluid::fluidx::AdvancedMonitoringDescriptor& m_Monitoring;


  private slots:

    void addObserver();

    void moveModelItemUp(const QString& ID, int CurrentIndex);

    void moveModelItemDown(const QString& ID, int CurrentIndex);

    void removeModelItem(const QString& ID, int CurrentIndex);

    void dispatchChangesFromChildren();


  public slots:

    void refresh();

    void notifySrcEditAsked(const QString& ID);


    signals:

      void srcEditAsked(const QString&,openfluid::ware::WareType WType, bool Ghost);


  public:

    MonitoringWidget(QWidget* Parent, openfluid::fluidx::AdvancedFluidXDescriptor& AFXDesc);

    virtual ~MonitoringWidget();

    void prepareWaresUpdate();

    void updateWares();


};


#endif /* __OPENFLUID_BUILDERAPP_MONITORINGWIDGET_HPP__ */
