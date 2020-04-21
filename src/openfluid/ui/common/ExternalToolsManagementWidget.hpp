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
  @file ExternalToolsManagementWidget.hpp

  @author Armel THONI <armel.thoni@inrae.fr>
 */


#ifndef __OPENFLUID_UICOMMON_EXTERNALTOOLSMANAGEMENTWIDGET_HPP__
#define __OPENFLUID_UICOMMON_EXTERNALTOOLSMANAGEMENTWIDGET_HPP__


#include <QWidget>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/dllexport.hpp>


namespace Ui
{
  class ExternalToolsManagementWidget;
}


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API ExternalToolsManagementWidget : public QWidget
{
  Q_OBJECT

  private slots:

    void addTool();

    void removeTool();

    void editTool();

    void moveupTool();

    void movedownTool();


  private:

    Ui::ExternalToolsManagementWidget* ui;

    openfluid::base::PreferencesManager::ExternalToolsCommands_t m_ToolCommands;

    QList<QString> m_ToolsOrder;

    bool m_NeedRestart = false;

    void update();


  signals:

    void toolsUpdated();


  public:

    explicit ExternalToolsManagementWidget(QWidget* Parent = nullptr);

    virtual ~ExternalToolsManagementWidget();


    /**
      Returns the map of tools
      @return The map of tools
    */
    openfluid::base::PreferencesManager::ExternalToolsCommands_t getToolsList() const;


    /**
      Returns the ordered list of tools
      @return The ordered list of tools
    */
    QList<QString> getToolsOrder() const;

    bool getNeedRestart() {return m_NeedRestart;}

};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_EXTERNALTOOLSMANAGEMENTWIDGET_HPP__ */
