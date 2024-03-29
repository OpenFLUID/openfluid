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
 @file WorkspaceToolbar.hpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_DEVSTUDIOAPP_WORKSPACETOOLBAR_HPP__
#define __OPENFLUID_DEVSTUDIOAPP_WORKSPACETOOLBAR_HPP__


#include <QToolBar>
#include <QAction>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/ui/waresdev/WareBuildOptionsWidget.hpp>


class OPENFLUID_API WorkspaceToolbar: public QToolBar
{
  Q_OBJECT

  private:

    std::list<openfluid::base::PreferencesManager::ExternalTool_t> m_ExternalTools;

    QList<QString> m_ExternalToolsOrder;

    QMap<QString, QAction*> m_Actions;

    void createActions();


  public:

    WorkspaceToolbar(QWidget* Parent = nullptr);

    virtual ~WorkspaceToolbar();

    QAction* action(const QString& ActionName);
};


#endif /* __OPENFLUID_DEVSTUDIOAPP_WORKSPACETOOLBAR_HPP__ */
