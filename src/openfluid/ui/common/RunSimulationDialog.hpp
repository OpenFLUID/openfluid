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
  @file RunSimulationDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_UICOMMON_RUNSIMULATIONDIALOG_HPP__
#define __OPENFLUID_UICOMMON_RUNSIMULATIONDIALOG_HPP__


#include <QObject>
#include <QDialog>
#include <QElapsedTimer>

#include <openfluid/machine/MachineListener.hpp>
#include <openfluid/base/SimulationStatus.hpp>
#include <openfluid/ui/common/RunSimulationWorker.hpp>
#include <openfluid/base/Exception.hpp>
#include <openfluid/dllexport.hpp>


namespace Ui
{
  class RunSimulationDialog;
}


namespace openfluid {

namespace fluidx {
  class FluidXDescriptor;
}


namespace ui { namespace common {


class OPENFLUID_API RunSimulationDialog : public QDialog
{
  Q_OBJECT;

  private:

    Ui::RunSimulationDialog *ui;

    const openfluid::fluidx::FluidXDescriptor* mp_FXDesc;

    openfluid::ui::common::RunSimulationListener* mp_Listener;

    QElapsedTimer m_ElapsedTimer;

    bool m_Launched;

    bool m_Success;

    static QString getDurationAsDaysHoursMinsSecsString(openfluid::core::Duration_t Duration);

    void setCompleted();


  private slots:

    void setPeriod(QString Begin, QString End, int Duration);

    void setStage(openfluid::ui::common::RunSimulationListener::Stage S);

    void setProgressValue(int Index);

    void setProgressMax(int Index);

    void setWarningsCount(unsigned int Count);

    void requestAbort();

    void requestSuspendResume();

    void validateSuspend();

    void handleError(QString Msg,openfluid::base::ExceptionContext Context);

    void handleFinish();

    void handleUserAbort();

    void showErrorDetails();


  public:

    RunSimulationDialog(QWidget* Parent, const openfluid::fluidx::FluidXDescriptor* FXDesc);

    virtual ~RunSimulationDialog();

    bool execute();

};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_RUNSIMULATIONDIALOG_HPP__ */
