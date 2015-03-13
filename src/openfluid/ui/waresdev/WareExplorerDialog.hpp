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
 @file WareExplorerDialog.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_UIWARESDEV_WAREEXPLORERDIALOG_HPP__
#define __OPENFLUID_UIWARESDEV_WAREEXPLORERDIALOG_HPP__

#include <openfluid/dllexport.hpp>

#include <QDialog>
#include <QPushButton>

#include <openfluid/waresdev/WareSrcManager.hpp>

namespace Ui { class WareExplorerDialog;
}

namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API WareExplorerDialog: public QDialog
{
  Q_OBJECT

  private:

    Ui::WareExplorerDialog* ui;

    openfluid::waresdev::WareSrcManager* mp_Manager;

    QPushButton* mp_AcceptButton;

    QString m_DefaulMessage;

    WareExplorerDialog(QWidget* Parent, const QString& TopDirectoryPath, const QString& CurrentPath,
                       const QString& Title, const QString& DefaultMessage, const QString& AcceptButtonLabel);

    void setStatus(const QString WarningMsg);

  public:

    ~WareExplorerDialog();

    static QString getOpenWarePath(QWidget* Parent, const QString& TopDirectoryPath, const QString& Title,
                                   const QString& CurrentPath = "");

    static QString getOpenFilePath(QWidget* Parent, const QString& TopDirectoryPath, const QString& CurrentPath = "");

    static QString getSaveFilePath(QWidget* Parent, const QString& TopDirectoryPath, const QString& CurrentPath = "");

    static QString getCreateFilePath(QWidget* Parent, const QString& TopDirectoryPath, const QString& CurrentPath = "");

    void setOpenWareMode();
    void setOpenFileMode();
    void setSaveFileMode();
    void setCreateFileMode();

    /*
     * Returns the path selected in the explorer (may be a file or a directory)
     */
    QString getSelectedPath();

    /*
     * Returns the filename (i.e. without path) in the line edit (may be empty)
     */
    QString getEditedFilename();

    /*
     * Returns the complete selected path, i.e. the combination of the filename in the selected directory
     */
    QString getCompleteFilePath();


  private slots:

    void onCurrentChangedOpenWareMode(const QString& Path);
    void onCurrentChangedOpenFileMode(const QString& Path);
    void onCurrentChangedSaveFileMode(const QString& Path);

    void onTextChangedCheckEmpty(const QString& Text);
    void onTextChangedCheckExists(const QString& Text);
};

} } }  // namespaces

#endif /* __OPENFLUID_UIWARESDEV_WAREEXPLORERDIALOG_HPP__ */
