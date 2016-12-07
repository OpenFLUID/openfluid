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
  @file HomeModuleWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID__BUILDERAPP_HOMEMODULEWIDGET_HPP__
#define __OPENFLUID__BUILDERAPP_HOMEMODULEWIDGET_HPP__


#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "AbstractModuleWidget.hpp"


class AppActions;

namespace Ui
{
  class HomeModuleWidget;
}


class HomeModuleWidget : public AbstractModuleWidget
{
  Q_OBJECT;


  private:

     Ui::HomeModuleWidget* ui;

     const AppActions* mp_Actions;

     QVBoxLayout* mp_RecentsLayout;

     QLabel* mp_RecentProjectsLabel;

     QPushButton* createButton(const QAction* Action, const QString& Text);


  public slots:

    void whenSrcEditAsked(const QString& ID,openfluid::ware::WareType WType,bool Ghost);


  public:

    HomeModuleWidget(const AppActions* Actions, QWidget* Parent = nullptr);

    ~HomeModuleWidget();

    QWidget* dockWidgetRebuilt(QWidget* /*Parent*/)
    { return nullptr; }

    bool whenQuitAsked();

    bool whenNewAsked();

    bool whenOpenAsked();

    bool whenReloadAsked();

    void whenSaveAsked();

    bool whenSaveAsAsked();

    void whenPropertiesAsked();

    bool whenCloseAsked();

    bool whenPreferencesAsked();

    void whenRecentProjectsActionsChanged();

    void whenRunAsked();

    void whenExtensionAsked(const QString& ID);

    void whenMarketAsked();

    void whenWaresRefreshAsked();

    bool whenOpenExampleAsked();

    void whenNewSimulatorSrcAsked();

    void whenOpenSimulatorSrcAsked();

    void whenNewObserverSrcAsked();

    void whenNewGhostSimulatorAsked();

    void whenOpenObserverSrcAsked();

    void whenLaunchDevStudioAsked();

    void refreshRecentProjects();

};



#endif /* __OPENFLUID__BUILDERAPP_HOMEMODULEWIDGET_HPP__ */
