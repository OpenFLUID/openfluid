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
  @file AbstractModuleWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_BUILDERAPP_ABSTRACTMODULEWIDGET_HPP__
#define __OPENFLUID_BUILDERAPP_ABSTRACTMODULEWIDGET_HPP__


#include <QWidget>

#include <openfluid/ware/PluggableWare.hpp>


class AbstractModuleWidget : public QWidget
{
  Q_OBJECT;


  public slots:

    virtual void whenSrcEditAsked(const QString&,openfluid::ware::WareType WType,bool Ghost) = 0;


  public:

    AbstractModuleWidget(QWidget* Parent = nullptr) : QWidget(Parent)
    {  }

    virtual ~AbstractModuleWidget()
    {  }

    virtual QWidget* dockWidgetRebuilt(QWidget* Parent) = 0;

    virtual bool whenQuitAsked() = 0;

    virtual bool whenNewAsked() = 0;

    virtual bool whenOpenAsked() = 0;

    virtual bool whenReloadAsked() = 0;

    virtual void whenSaveAsked() = 0;

    virtual bool whenSaveAsAsked() = 0;

    virtual void whenPropertiesAsked() = 0;

    virtual bool whenCloseAsked() = 0;

    /**
      @return true if restart is required after modified preferences
    */
    virtual bool whenPreferencesAsked() = 0;

    virtual void whenRecentProjectsActionsChanged() = 0;

    virtual void whenRunAsked() = 0;

    virtual void whenExtensionAsked(const QString& ID) = 0;

    virtual void whenMarketAsked() = 0;

    virtual void whenWaresRefreshAsked() = 0;

    virtual bool whenOpenExampleAsked() = 0;

    virtual void whenNewSimulatorSrcAsked() = 0;

    virtual void whenNewGhostSimulatorAsked() = 0;

    virtual void whenOpenSimulatorSrcAsked() = 0;

    virtual void whenNewObserverSrcAsked() = 0;

    virtual void whenOpenObserverSrcAsked() = 0;

    virtual void whenLaunchDevStudioAsked() = 0;

};


#endif /* __OPENFLUID_BUILDERAPP_ABSTRACTMODULEWIDGET_HPP__ */
