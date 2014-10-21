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
  \file NewProjectDialog.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __NEWPROJECTDIALOG_HPP__
#define __NEWPROJECTDIALOG_HPP__

#include <openfluid/ui/common/OpenFLUIDDialog.hpp>


class QFileSystemModel;


namespace Ui
{
  class NewProjectDialog;
}


class NewProjectDialog : public openfluid::ui::common::OpenFLUIDDialog
{
  Q_OBJECT;

  private:

    Ui::NewProjectDialog *ui;

    QFileSystemModel *mp_DirectoryModel;

    void setMessage(const QString& Msg = "");

  private slots:

    void onGlobalCheck();

    void onWorkdirButtonClicked();

    void onProjectButtonClicked();

    void onDatadirButtonClicked();

  public:

    enum ImportType { IMPORT_NONE, IMPORT_PROJECT, IMPORT_DIRECTORY};

    NewProjectDialog(QWidget *Parent);

    virtual ~NewProjectDialog();

    QString getProjectName() const;

    QString getWorkingDir() const;

    QString getProjectFullPath() const;

    QString getProjectDescription() const;

    QString getProjectAuthors() const;

    ImportType getImportType() const;

    QString getImportSource() const;

};


#endif /* __NEWPROJECTDIALOG_HPP__ */
