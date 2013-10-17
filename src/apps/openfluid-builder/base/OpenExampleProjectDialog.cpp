/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file OpenExampleProjectDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <QPushButton>
#include <QDir>

#include <openfluid/config.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/buddies/ExamplesBuddy.hpp>

#include "ui_OpenExampleProjectDialog.h"
#include "OpenExampleProjectDialog.hpp"

#include "AppTools.hpp"
#include "builderconfig.hpp"


OpenExampleProjectDialog::OpenExampleProjectDialog(QWidget *Parent):
QDialog(Parent), ui(new Ui::OpenExampleProjectDialog)
{
  ui->setupUi(this);

  // installation of missing examples
  openfluid::buddies::ExamplesBuddy(NULL).run();

  m_ProjectsRootPath = QString(openfluid::base::RuntimeEnvironment::getInstance()->getUserExamplesDir().c_str()) +
                       QString("/") +
                       QString(openfluid::config::PROJECTS_SUBDIR.c_str());

  QStringList ExamplesDirs;
  ExamplesDirs = QDir(m_ProjectsRootPath).entryList(QStringList("*"),QDir::Dirs | QDir::NoDotAndDotDot);

  ExamplesDirs.sort();

  for (int i=0;i<ExamplesDirs.size();++i)
  {
    QString CurrentFullDir(m_ProjectsRootPath+"/"+ExamplesDirs[i]);

    if (openfluid::base::ProjectManager::isProject(CurrentFullDir.toStdString()))
      ui->ProjectsListWidget->addItem(ExamplesDirs[i]);
  }


  updateProjectInfo();

  connect(ui->ProjectsListWidget,
          SIGNAL(itemSelectionChanged()),
          this, SLOT(updateProjectInfo()));

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));
}


// =====================================================================
// =====================================================================


OpenExampleProjectDialog::~OpenExampleProjectDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void OpenExampleProjectDialog::setMessage(const QString& Msg)
{
  if (Msg.isEmpty())
  {
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(BUILDER_DIALOGBANNER_BGCOLOR));
    ui->MessageLabel->setText(tr("Open an example project"));
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
  else
  {
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(BUILDER_DIALOGBANNER_WARNBGCOLOR));
    ui->MessageLabel->setText(Msg);
  }
}


// =====================================================================
// =====================================================================


void OpenExampleProjectDialog::updateProjectInfo()
{
  if (ui->ProjectsListWidget->currentRow()<0)
  {
    setMessage(tr("No project selected"));
    ui->ProjectInfosLabel->setText("(No project information)");
    ui->ProjectInfosLabel->setStyleSheet("font:italic;");
  }
  else
  {
    QString InfosStr = getProjectInfosAsHTML(m_ProjectsRootPath+"/"+ui->ProjectsListWidget->currentItem()->text());

    if (!InfosStr.isEmpty())
    {
      setMessage();
      ui->ProjectInfosLabel->setStyleSheet("");
      ui->ProjectInfosLabel->setText(InfosStr);
    }
    else
    {
      setMessage(tr("Wrong project format"));
      ui->ProjectInfosLabel->setText("(No project information)");
      ui->ProjectInfosLabel->setStyleSheet("font:italic;");
    }
  }
}


// =====================================================================
// =====================================================================


QString OpenExampleProjectDialog::getSelectedProjectPath() const
{
  if (ui->ProjectsListWidget->currentRow()<0)
    return "";
  else
    return QString(m_ProjectsRootPath+"/"+ui->ProjectsListWidget->currentItem()->text());

}
