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
  @file WareBuildStatusWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_DEVSTUDIOAPP_WAREBUILDSTATUSWIDGET_HPP__
#define __OPENFLUID_DEVSTUDIOAPP_WAREBUILDSTATUSWIDGET_HPP__


#include <QWidget>

#include <openfluid/dllexport.hpp>
#include <openfluid/ui/common/ActionLabel.hpp>
#include <openfluid/waresdev/WareBuildOptions.hpp>


namespace Ui
{
  class WareBuildStatusWidget;
}

class OPENFLUID_API WareBuildStatusWidget : public QWidget
{
  Q_OBJECT;

  private:

    Ui::WareBuildStatusWidget *ui;

    openfluid::waresdev::WareBuildOptions m_BuildOptions;

    void refreshBuildJobsStatus();

    void refreshBuildModeStatus();

    void refreshConfigureModeStatus();


  private slots:

    void onConfigureModeClicked();

    void onBuildModeClicked();

    void onJobsClicked();

    void onJobsScrolled(bool Up);

    void refreshAllLabels();


  signals:

    void settingsButtonClicked();

    void settingsChanged(openfluid::waresdev::WareBuildOptions BuildOptions);


  public:

    WareBuildStatusWidget(QWidget* Parent = nullptr);

    ~WareBuildStatusWidget();

    void setBuildOptions(openfluid::waresdev::WareBuildOptions BuildOptions);
};


#endif /* __OPENFLUID_DEVSTUDIOAPP_WAREBUILDSTATUSWIDGET_HPP__ */
