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
 \file WareSrcWidgetCollection.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_UIWARESDEV_WARESRCWIDGETCOLLECTION_HPP_
#define __OPENFLUID_UIWARESDEV_WARESRCWIDGETCOLLECTION_HPP_

#include <openfluid/dllexport.hpp>

#include <QObject>
#include <QMap>
#include <QString>

#include <openfluid/waresdev/WareSrcContainer.hpp>

class QTabWidget;


namespace openfluid { namespace waresdev {
class WareSrcManager;
}
namespace ui { namespace waresdev {

class WareSrcWidget;

class OPENFLUID_API WareSrcWidgetCollection: public QObject
{
  Q_OBJECT

  private:

    QTabWidget* mp_TabWidget;

    bool m_IsStandalone;

    openfluid::waresdev::WareSrcManager* mp_Manager;

    /**
     * List of opened ware widgets by their absolute path
     */
    QMap<QString, WareSrcWidget*> m_WareSrcWidgetByPath;

    openfluid::waresdev::WareSrcContainer::ConfigMode m_DefaultConfigMode;
    openfluid::waresdev::WareSrcContainer::BuildMode m_DefaultBuildMode;

    QString getCurrentPath();

    /**
     * @throw openfluid::base::FrameworkException
     */
    openfluid::waresdev::WareSrcContainer& getCurrentWidgetContainer();

    bool isModified();

    void closeWareTab(WareSrcWidget* Ware);

  public:

    WareSrcWidgetCollection(QTabWidget* TabWidget, bool IsStandalone);

    ~WareSrcWidgetCollection();

  public slots:

    void openPath(const QString& Path);

    void setCurrent(const QString& Path);

    void openExplorer(const QString& Path = "");

    void openTerminal(const QString& Path = "");

    void setReleaseMode();

    void setDebugMode();

    void setBuildWithInstallMode();

    void setBuildNoInstallMode();

    void configure();

    void build();

    void saveCurrentEditor();

    void saveCurrentEditorAs(const QString& TopDirectory = "");

    void closeCurrentEditor();

  private slots:

    void onWareTxtModified(WareSrcWidget* Widget, bool Modified);

    void onCloseWareTabRequested(int Index);
};

} } }  // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WARESRCWIDGETCOLLECTION_HPP_ */
