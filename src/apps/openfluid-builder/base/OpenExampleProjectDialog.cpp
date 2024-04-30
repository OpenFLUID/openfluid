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
  @file OpenExampleProjectDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QPushButton>
#include <QDir>

#include <openfluid/config.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/base/ExamplesManager.hpp>
#include <openfluid/tools/Filesystem.hpp>

#include "ui_OpenExampleProjectDialog.h"
#include "OpenExampleProjectDialog.hpp"
#include "AppTools.hpp"


OpenExampleProjectDialog::OpenExampleProjectDialog(QWidget *Parent):
  openfluid::ui::common::MessageDialog(Parent),
  ui(new Ui::OpenExampleProjectDialog)
{
  ui->setupUi(this);

  setupMessageUi(tr("Open an example project"));

  // installation of missing examples
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  openfluid::base::ExamplesManager::installAll();
  QApplication::restoreOverrideCursor();

  m_ProjectsRootPath = QString::fromStdString(openfluid::tools::Filesystem::joinPath(
                         {openfluid::base::Environment::getUserExamplesDir(),openfluid::config::PROJECTS_PATH})
                       );

  QStringList ExamplesDirs;
  ExamplesDirs = QDir(m_ProjectsRootPath).entryList(QStringList("*"),QDir::Dirs | QDir::NoDotAndDotDot);

  ExamplesDirs.sort();

  for (int i=0;i<ExamplesDirs.size();++i)
  {
    QString CurrentFullDir(m_ProjectsRootPath+"/"+ExamplesDirs[i]);

    if (openfluid::base::RunContextManager::isProject(CurrentFullDir.toStdString()))
    {
      ui->ProjectsListWidget->addItem(ExamplesDirs[i]);
    }
  }


  updateProjectInfo();

  connect(ui->ProjectsListWidget,
          SIGNAL(itemSelectionChanged()),
          this, SLOT(updateProjectInfo()));
}


// =====================================================================
// =====================================================================


OpenExampleProjectDialog::~OpenExampleProjectDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void OpenExampleProjectDialog::updateProjectInfo()
{
  if (ui->ProjectsListWidget->currentRow() < 0)
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
  {
    return "";
  }
  else
  {
    return QString(m_ProjectsRootPath+"/"+ui->ProjectsListWidget->currentItem()->text());
  }

}
