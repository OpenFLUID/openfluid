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
  @file WorkspaceDevProcessDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_WORKSPACEDEVPROCESSDIALOG_HPP__
#define __OPENFLUID_UIWARESDEV_WORKSPACEDEVPROCESSDIALOG_HPP__


#include <QDialog>

#include <openfluid/dllexport.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevDashboardTypes.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevProcessWorker.hpp>
#include <openfluid/ware/TypeDefs.hpp>


namespace Ui {
class WorkspaceDevProcessDialog;
}


namespace openfluid { namespace ui { namespace waresdev {


class WorkspaceDevDashboardDialog;


class OPENFLUID_API WorkspaceDevProcessDialog: public QDialog
{
  Q_OBJECT;


  private slots:

    void handleWorkerFinished();

    void handleWorkerProcessed();

    void handleWorkerMessage(QString Msg);

    void copyToClipboard();

    void saveToFile();


  protected:

    Ui::WorkspaceDevProcessDialog* ui;


  public:

    WorkspaceDevProcessDialog(const QString& Title, WorkspaceDevProcessWorker* Worker,
                              WorkspaceDevDashboardDialog* Parent);

    virtual ~WorkspaceDevProcessDialog();
};


} } } // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WORKSPACEDEVPROCESSDIALOG_HPP__ */
