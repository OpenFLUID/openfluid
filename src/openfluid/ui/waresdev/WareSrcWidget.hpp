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
 @file WareSrcWidget.hpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/

#ifndef __OPENFLUID_UIWARESDEV_WARESRCWIDGET_HPP__
#define __OPENFLUID_UIWARESDEV_WARESRCWIDGET_HPP__


#include <QWidget>

#include <openfluid/dllexport.hpp>
#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/waresdev/WareSrcContainer.hpp>


namespace Ui {
class WareSrcWidget;
}


namespace openfluid { namespace ui { namespace waresdev {


class WareFileEditor;
class TextEditMsgStream;
class WareSrcToolbar;


class OPENFLUID_API WareSrcWidget: public QWidget
{
  Q_OBJECT


  private slots:

    void onEditorTxtModified(WareFileEditor* Editor, bool Modified);

    void onCurrentTabChanged(int Index);

    void onProcessFinished();

    void onMessageClicked(openfluid::waresdev::WareSrcMsgParser::WareSrcMsg& Msg);

    void notifyConfigureLaunched(openfluid::ware::WareType Type, const QString& ID);

    void notifyConfigureFinished(openfluid::ware::WareType Type, const QString& ID);

    void notifyBuildLaunched(openfluid::ware::WareType Type, const QString& ID);

    void notifyBuildFinished(openfluid::ware::WareType Type, const QString& ID);

    void updateWareOptions();


  private:

    Ui::WareSrcWidget* ui;

    openfluid::waresdev::WareSrcContainer m_Container;

    /**
     * List of opened source file editors by their absolute path
     */
    QMap<QString, WareFileEditor*> m_WareFilesByPath;

    openfluid::ui::waresdev::TextEditMsgStream* mp_TextEditMsgStream;

    bool m_IsStandalone;

    WareSrcToolbar* mp_StandaloneToolBar = 0;

    void addNewFileTab(int Index, const QString& AbsolutePath, const QString& TabLabel, const QString& TabTooltip = "");

    /**
      Deletes Editor
      @return Editor index before it was removed
    */
    int closeFileTab(WareFileEditor* Editor);

    void clearEditorsMessages();


  protected:

    bool eventFilter(QObject* Obj, QEvent* Event);


  signals:

    void wareTextModified(WareSrcWidget* Widget, bool Modified);

    void editorSaved();

    void currentTabChanged(const QString& Path);

    void findReplaceRequested();

    void openTerminalRequested();

    void openExplorerRequested();

    void modifiedStatusChanged(bool CurrentEditorModified, bool WareModified);

    void openAPIDocRequested();

    void configureLaunched(openfluid::ware::WareType Type, const QString& ID);

    void configureFinished(openfluid::ware::WareType Type, const QString& ID);

    void buildLaunched(openfluid::ware::WareType Type, const QString& ID);

    void buildFinished(openfluid::ware::WareType Type, const QString& ID);


  public slots:

    void setConfigureMode(openfluid::waresdev::WareSrcContainer::ConfigMode Mode);

    void setBuildMode(openfluid::waresdev::WareSrcContainer::BuildMode Mode);

    void configure();

    void build();

    void generateDoc();

    void saveCurrentEditor();

    void saveAllFileTabs();

    int closeCurrentEditor(bool WithConfirm = true);

    int onCloseFileTabRequested(int Index, bool WithConfirm = true);

    void newFile();

    void deleteCurrentFile();

    void openFile();

    /**
      @param TopDirectory The path to the topmost directory where may be saved the file,
      an empty string meaning this ware directory
      @return The path where has been saved the file if it's above this ware, an empty string otherwise
    */
    QString saveAs(const QString& TopDirectory = "");

    void copyText();

    void cutText();

    void pasteText();

    void goToLine();


  public:

    WareSrcWidget(const openfluid::waresdev::WareSrcManager::PathInfo& Info, bool IsStandalone,
                  openfluid::waresdev::WareSrcContainer::ConfigMode Config,
                  openfluid::waresdev::WareSrcContainer::BuildMode Build, QWidget* Parent = nullptr);

    ~WareSrcWidget();

    void openFileTab(const openfluid::waresdev::WareSrcManager::PathInfo& Info, int Index = -1);

    void openDefaultFiles();

    /**
      Set the file editor for the absolute path of Info as the current tab
      @details Check if the file editor is already opened. If true, set this editor as the current tab.
      Otherwise does nothing.
      @return true if the file editor was already opened, false otherwise
    */
    bool setCurrent(const openfluid::waresdev::WareSrcManager::PathInfo& Info);

    openfluid::waresdev::WareSrcContainer& wareSrcContainer();

    bool isWareModified();

    bool isWareProcessRunning() const;

    void closeAllFileTabs();

    WareFileEditor* currentEditor();

    QString getCurrentFilePath();

    int closeFileTab(const QString& Path);

    void checkModifiedStatus();

    void updateEditorsSettings();

};


} } }  // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WARESRCWIDGET_HPP__ */
