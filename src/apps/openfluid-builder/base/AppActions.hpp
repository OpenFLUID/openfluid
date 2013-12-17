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
  \file AppActions.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __APPACTIONS_HPP__
#define __APPACTIONS_HPP__

#include <QAction>

#include <openfluid/ware/PluggableWare.hpp>

#include "MainWindow.hpp"

class AppActions : QObject
{

  Q_OBJECT;

  private:

    std::map<std::string,QAction*> m_Actions;

    std::vector<QAction*> m_RecentProjectsActions;

    std::map<openfluid::ware::WareID_t,QAction*> m_ExtensionsActions;

    QMenu* mp_SimulationMenu;

    QMenu* mp_ExtensionsMenu;

    QMenu* mp_SpatialExtensionsMenu;

    QMenu* mp_ModelExtensionsMenu;

    QMenu* mp_ResultsExtensionsMenu;

    QMenu* mp_OtherExtensionsMenu;

    QMenu* mp_ViewMenu;

    QMenu* mp_RecentProjectsMenu;

    QToolBar* mp_MainToolbar;

    void createActions();


  public:

    AppActions();

    ~AppActions();

    QAction* getAction(const std::string& ID) const;

    std::vector<QAction*> getRecentProjectActions() const
    {
      return m_RecentProjectsActions;
    }

    std::map<openfluid::ware::WareID_t,QAction*>& getExtensionsActions()
    {
      return m_ExtensionsActions;
    }

    void setProjectMode();

    void setHomeMode();

    void createMenus(MainWindow& MainWin);

    void createToolbar(MainWindow& MainWin);

    void updateRecentProjectsActions();

    void updateExtensionsActionsAndMenus();

};


#endif /* __APPACTIONS_HPP__ */
