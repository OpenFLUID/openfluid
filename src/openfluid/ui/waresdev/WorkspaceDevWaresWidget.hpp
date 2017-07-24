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
  @file WorkspaceDevWaresWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/



#ifndef __OPENFLUID_UIWARESDEV_WORKSPACEDEVWARESWIDGET_HPP__
#define __OPENFLUID_UIWARESDEV_WORKSPACEDEVWARESWIDGET_HPP__


#include <QWidget>

#include <openfluid/dllexport.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevDashboardTypes.hpp>


namespace Ui {
class WorkspaceDevWaresWidget;
}


namespace openfluid { namespace ui { namespace waresdev {


class WorkspaceDevActionsWidget;


class OPENFLUID_API WorkspaceDevWaresWidget: public QWidget
{
  Q_OBJECT;


  private slots:

    void selectAll();

    void selectNone();

    void selectFiltered();

    void invertSelection();

    void notifySelectionChanged();


  private:

    Ui::WorkspaceDevWaresWidget* ui;

    unsigned int m_WaresCount = 0;

    std::map<QString,WorkspaceDevActionsWidget*> m_ActionsWidgetsMap;


  signals:

    void selectionChanged();


  public:

    static const WorkspaceDevDashboardTypes::ActionsByRows Actions;

    WorkspaceDevWaresWidget(QWidget* Parent = nullptr);

    virtual ~WorkspaceDevWaresWidget();

    void addWare(const WorkspaceDevDashboardTypes::WareGitInfos& Infos);

    unsigned int getWaresCount() const
    { return m_WaresCount; }

    WorkspaceDevDashboardTypes::WaresSelection getSelection();

    void setState(const QString& ID, const QString& ActionName, bool IsOK);

};


} } } // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WORKSPACEDEVWARESWIDGET_HPP__ */
