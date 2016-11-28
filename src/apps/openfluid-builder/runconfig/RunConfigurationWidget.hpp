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
  @file RunConfigurationWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_RUNCONFIGURATIONWIDGET_HPP__
#define __OPENFLUID_BUILDERAPP_RUNCONFIGURATIONWIDGET_HPP__


#include "WorkspaceWidget.hpp"


namespace Ui
{
  class RunConfigurationWidget;
}


class RunConfigurationWidget : public WorkspaceWidget
{
  Q_OBJECT

  private:

    Ui::RunConfigurationWidget* ui;


  private slots:

      void updateConstraintFXDesc(int Index);

      void updateDeltaTFXDesc(int Value);

      void updateBeginDateFXDesc(const QDateTime& QDT);

      void updateEndDateFXDesc(const QDateTime& QDT);

      void updateMemoryFXDesc(bool On);

      void updateMemoryStepsFXDesc(int Value);

      void updateClearOutput(bool On);

      void updateProfiling(bool On);

      void updateMaxThreads(int Num);

      void resetThreadsToIdeal();


  public slots:

    void refresh();


  public:

    RunConfigurationWidget(QWidget* Parent, openfluid::fluidx::AdvancedFluidXDescriptor& AFXDesc);

    virtual ~RunConfigurationWidget();
};




#endif /* __OPENFLUID_BUILDERAPP_RUNCONFIGURATIONWIDGET_HPP__ */
