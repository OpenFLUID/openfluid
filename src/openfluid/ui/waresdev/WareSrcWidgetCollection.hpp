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
 @file WareSrcWidgetCollection.hpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_WARESRCWIDGETCOLLECTION_HPP__
#define __OPENFLUID_UIWARESDEV_WARESRCWIDGETCOLLECTION_HPP__


#include <QObject>
#include <QMap>
#include <QString>
#include <QTextDocument>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/waresdev/WareBuildOptions.hpp>
#include <openfluid/waresdev/WareSrcContainer.hpp>
#include <openfluid/ui/waresdev/FindReplaceDialog.hpp>


class QTabWidget;


namespace openfluid {

namespace waresdev {
class WareSrcEnquirer;
}


namespace ui { namespace waresdev {

class WareSrcWidget;
class FindReplaceDialog;


class OPENFLUID_API WareSrcWidgetCollection: public QObject
{
  Q_OBJECT


  private slots:

    void onWareTxtModified(WareSrcWidget* Widget, bool Modified);

    void onCloseWareTabRequested(int Index);

    void onCurrentTabChanged(int Index);

    void onFindReplaceRequested(FindReplaceDialog::FindReplaceAction Action, const QString& StringToFind,
                                const QString& StringForReplace, QTextDocument::FindFlags Options);

    void checkModifiedStatus();

    void notifyConfigureLaunched(openfluid::ware::WareType Type, const QString& ID);

    void notifyConfigureFinished(openfluid::ware::WareType Type, const QString& ID);

    void notifyBuildLaunched(openfluid::ware::WareType Type, const QString& ID);

    void notifyBuildFinished(openfluid::ware::WareType Type, const QString& ID);


  private:

    QTabWidget* mp_TabWidget;

    bool m_IsStandalone;

    openfluid::waresdev::WareBuildOptions m_DefaultWareBuildOptions;

    const openfluid::base::WorkspaceManager* mp_Manager;

    /**
     * List of opened ware widgets by their absolute path
     */
    QMap<QString, WareSrcWidget*> m_WareSrcWidgetByPath;

    openfluid::waresdev::WareSrcContainer::ConfigMode m_DefaultConfigMode;
    openfluid::waresdev::WareSrcContainer::BuildMode m_DefaultBuildMode;
    unsigned int m_DefaultBuildJobs;

    FindReplaceDialog* mp_FindReplaceDialog;

    WareSrcWidget* currentWareWidget();

    bool isModified() const;

    bool isProcessRunning() const;

    void closeWareTab(WareSrcWidget* Ware);

    void openWare(openfluid::ware::WareType Type, const QString& Title);

    void newWare(openfluid::ware::WareType Type);


  signals:

    void editorSaved();

    void currentTabChanged(const QString& Path);

    void modifiedStatusChanged(bool CurrentEditorModified, bool FileOpen, bool CurrentWareModified);

    void configureLaunched(openfluid::ware::WareType Type, const QString& ID);

    void configureFinished(openfluid::ware::WareType Type, const QString& ID);

    void buildLaunched(openfluid::ware::WareType Type, const QString& ID);

    void buildFinished(openfluid::ware::WareType Type, const QString& ID);


  public slots:

    bool openPath(const QString& Path);

    void setCurrent(const QString& Path);

    QString getContextualPath(const QString& Path);

    void openExplorerAtWarePath();

    void openExplorer(const QString& Path = "");

    void openTerminalAtWarePath();

    void openTerminal(const QString& Path = "");

    void openExternalTool(const QString& Command, const QString& Path = "");

    void setConfigureMode(openfluid::waresdev::WareSrcContainer::ConfigMode Mode);

    void setBuildMode(openfluid::waresdev::WareSrcContainer::BuildMode Mode);

    void setBuildJobs(unsigned int Jobs);

    void configure();

    void build();

    void generateDoc();

    void saveCurrentEditor();

    /**
      @param TopDirectory The path to the topmost directory where may be saved the file,
      an empty string meaning the current ware directory
      @return The path where has been saved the file if it's above this ware, an empty string otherwise
    */
    QString saveAs(const QString& TopDirectory = "");

    void saveAllCurrent();

    void closeCurrentEditor();

    void openFile();

    void openSimulator();

    void openObserver();

    void openBuilderExtension();

    void newFile();

    void deleteCurrentFile();

    void newSimulator();

    void newSimulatorFromGhost(const openfluid::ware::SimulatorSignature& Signature);

    void newObserver();

    void newBuilderExtension();

    void showFindReplaceDialog();

    void copyText();

    void cutText();

    void pasteText();

    void openAPIDoc();

    void goToLine();

    void closeEditor(const QString& FilePath);

    void closeEditorsInFolderFromWare(const QString& WarePath, const QString& FolderPath, const bool Confirm);

    void updateEditorsSettings();


  public:

    WareSrcWidgetCollection(QTabWidget* TabWidget, bool IsStandalone);

    virtual ~WareSrcWidgetCollection();

    /**
      Returns false is there is unchanged modifications and user chooses to cancel closing, true otherwise
    */
    bool closeAllWidgets();

    QStringList getOpenWarePaths();

    QString getCurrentWarePath();

    bool isFileOpen();

    bool isDebugMode();

    bool isBuildNoInstallMode();

    void deleteWare(const QString& WarePath);

};


} } }  // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WARESRCWIDGETCOLLECTION_HPP__ */
