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
  @file WorkspaceDevDashboardDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/



#ifndef __OPENFLUID_UIWARESDEV_WORKSPACEDEVDASHBOARDDIALOG_HPP__
#define __OPENFLUID_UIWARESDEV_WORKSPACEDEVDASHBOARDDIALOG_HPP__


#include <QDialog>

#include <openfluid/dllexport.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevDashboardTypes.hpp>
#include <openfluid/ware/TypeDefs.hpp>


namespace Ui {
class WorkspaceDevDashboardDialog;
}


class QPushButton;


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API WorkspaceDevDashboardDialog: public QDialog
{
  Q_OBJECT;


  private slots:

    void activateDialog();

    void updateBatchActionsGroup(unsigned int SelectionCount);

    void addParsedWare(openfluid::ui::waresdev::WorkspaceDevDashboardTypes::WareGitInfos Infos);

    void setWareActionStatus(openfluid::ware::WareType Type,QString ID,QString ActionName, bool Status);

    void handleSelectionChanged();

    void handleCheckChanged();

    void handlePurgeChanged();

    void runConfigureBuild();

    void runCheck();

    void runPurge();


  private:

    Ui::WorkspaceDevDashboardDialog* ui;

    WorkspaceDevDashboardTypes::WaresSelectionByType getSelectionByType() const;


  public:

    WorkspaceDevDashboardDialog(QWidget* Parent = nullptr);

    virtual ~WorkspaceDevDashboardDialog();

};


} } } // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WORKSPACEDEVDASHBOARDDIALOG_HPP__ */
