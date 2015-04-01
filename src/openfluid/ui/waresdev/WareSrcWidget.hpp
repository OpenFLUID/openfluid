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
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef __OPENFLUID_UIWARESDEV_WARESRCWIDGET_HPP__
#define __OPENFLUID_UIWARESDEV_WARESRCWIDGET_HPP__

#include <openfluid/dllexport.hpp>

#include <QWidget>

#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/waresdev/WareSrcContainer.hpp>

namespace Ui {
class WareSrcWidget;
}


namespace openfluid { namespace ui { namespace waresdev {

class WareSrcFileEditor;
class TextEditMsgStream;


class OPENFLUID_API WareSrcWidget: public QWidget
{
  Q_OBJECT

  private:

    Ui::WareSrcWidget* ui;

    openfluid::waresdev::WareSrcContainer m_Container;

    /**
     * List of opened source file editors by their absolute path
     */
    QMap<QString, WareSrcFileEditor*> m_WareSrcFilesByPath;

    openfluid::ui::waresdev::TextEditMsgStream* mp_TextEditMsgStream;

    bool m_IsStandalone;

    void addNewFileTab(int Index, const QString& AbsolutePath, const QString& TabLabel, const QString& TabTooltip = "");

    /**
     * Deletes Editor
     * @return Editor index before it was removed
     */
    int closeFileTab(WareSrcFileEditor* Editor);

  protected:

    bool eventFilter(QObject* Obj, QEvent* Event);

  public:

    WareSrcWidget(const openfluid::waresdev::WareSrcManager::PathInfo& Info, bool IsStandalone,
                  openfluid::waresdev::WareSrcContainer::ConfigMode Config,
                  openfluid::waresdev::WareSrcContainer::BuildMode Build, QWidget* Parent = 0);

    ~WareSrcWidget();

    void openFile(const openfluid::waresdev::WareSrcManager::PathInfo& Info, int Index = -1);

    void openDefaultFiles();

    /**
     * Set the file editor for the absolute path of Info as the current tab
     * @details Check if the file editor is already opened. If true, set this editor as the current tab.
     * Otherwise does nothing.
     * @return true if the file editor was already opened, false otherwise
     */
    bool setCurrent(const openfluid::waresdev::WareSrcManager::PathInfo& Info);

    openfluid::waresdev::WareSrcContainer& wareSrcContainer();

    bool isModified();

    void saveAllFileTabs();

    void closeAllFileTabs();

    WareSrcFileEditor* getCurrentEditor();

    QString getCurrentFilePath();

    int closeFileTab(const QString& Path);

  public slots:

    void setReleaseMode();

    void setDebugMode();

    void setBuildWithInstallMode();

    void setBuildNoInstallMode();

    void configure();

    void build();

    void showNotYetImplemented();

    void saveCurrentEditor();

    void saveCurrentEditorAs(const QString& Path);

    int closeCurrentEditor(bool WithConfirm = true);

    int onCloseFileTabRequested(int Index, bool WithConfirm = true);

    void deleteCurrentFile();

  private slots:

    void onEditorTxtModified(WareSrcFileEditor* Editor, bool Modified);

  signals:

    void wareTextModified(WareSrcWidget* Widget, bool Modified);

    void saveAsRequested();

    void newFileRequested();

    void openFileRequested();
};

} } }  // namespaces

#endif /* __OPENFLUID_UIWARESDEV_WARESRCWIDGET_HPP__ */
