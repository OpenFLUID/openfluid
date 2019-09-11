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
  @file RunCLISimulationDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_UICOMMON_RUNCLISIMULATIONDIALOG_HPP__
#define __OPENFLUID_UICOMMON_RUNCLISIMULATIONDIALOG_HPP__


#include <memory>

#include <openfluid/ui/common/MessageDialog.hpp>
#include <openfluid/dllexport.hpp>


namespace Ui
{
  class RunCLISimulationDialog;
}

class QProcess;
class QRegularExpression;


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API RunCLISimulationDialog : public MessageDialog
{
  Q_OBJECT;

  private:

    Ui::RunCLISimulationDialog* ui;

    QString m_PrjPath;

    std::unique_ptr<QProcess> mp_Process;

    std::unique_ptr<QRegularExpression> mp_ANSIRegex;

    bool m_IsKilled;
    
    bool m_CanClose;


  private slots:

    void displayOutput();
    
    void finishProcess();

    void killProcess();


  protected:

    void showEvent(QShowEvent* event);

    bool event(QEvent* event);


  public:

    RunCLISimulationDialog(QWidget* Parent, const QString& PrjPath);

    virtual ~RunCLISimulationDialog();

    bool execute();

};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_RUNCLISIMULATIONDIALOG_HPP__ */
