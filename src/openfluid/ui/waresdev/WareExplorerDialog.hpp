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
 @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_WAREEXPLORERDIALOG_HPP__
#define __OPENFLUID_UIWARESDEV_WAREEXPLORERDIALOG_HPP__


#include <QPushButton>
#include <QDir>

#include <openfluid/ui/common/MessageDialog.hpp>
#include <openfluid/waresdev/WareSrcEnquirer.hpp>
#include <openfluid/dllexport.hpp>


namespace Ui {
class WareExplorerDialog;
}


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API WareExplorerDialog : public openfluid::ui::common::MessageDialog
{
  Q_OBJECT


  private slots:

    void onCurrentChangedOpenWareMode(const QString& Path);

    void onCurrentChangedOpenFileMode(const QString& Path);

    void onCurrentChangedSaveCreateMode(const QString& Path);

    void onTextChangedSaveMode(const QString& Text);

    void onTextChangedCreateFileMode(const QString& Text);

    void onTextChangedCreateFolderMode(const QString& Text);

    void onDoubleClickedFileMode();


  private:

    Ui::WareExplorerDialog* ui;

    QPushButton* mp_AcceptButton;

    QDir m_TopDir;

    WareExplorerDialog(QWidget* Parent, const QString& TopDirectoryPath, const QString& CurrentPath,
                       const QString& Title, const QString& DefaultMessage, const QString& AcceptButtonLabel);

    void setStatus(const QString WarningMsg);


  public:

    virtual ~WareExplorerDialog();

    static QString getOpenWarePath(QWidget* Parent, const QString& TopDirectoryPath, const QString& Title,
                                   const QString& CurrentPath = "");

    static QString getOpenFilePath(QWidget* Parent, const QString& TopDirectoryPath,
                                   const QString& CurrentPath = "");

    static QString getSaveFilePath(QWidget* Parent, const QString& TopDirectoryPath,
                                   const QString& CurrentPath = "");

    static QString getCreateFilePath(QWidget* Parent, const QString& TopDirectoryPath,
                                     const QString& CurrentPath = "");

    static QString getCreateFolderPath(QWidget* Parent, const QString& TopDirectoryPath,
                                       const QString& CurrentPath = "");

    void setOpenWareMode();

    void setOpenFileMode();

    void setSaveFileMode();

    void setCreateFileMode(const QString& CurrentPath);

    void setCreateFolderMode();

    /**
      Returns the path selected in the explorer (may be a file or a directory)
    */
    QString getSelectedPath();

    /**
      Returns the combination of the entered filepath below the top directory
    */
    QString getCompleteFilePath();

};


} } }  // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WAREEXPLORERDIALOG_HPP__ */
