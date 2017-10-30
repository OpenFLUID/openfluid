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
  @file DetectQtDevToolsDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QDir>
#include <QFileInfo>
#include <QDirIterator>
#include <QFileDialog>
#include <QProcess>

#include <openfluid/ui/common/DetectQtDevToolsDialog.hpp>

#include "ui_DetectQtDevToolsDialog.h"


namespace openfluid { namespace ui { namespace common {


DetectQtDevToolsDialog::DetectQtDevToolsDialog(QWidget* Parent):
  OpenFLUIDDialog(Parent), ui(new Ui::DetectQtDevToolsDialog)
{
  ui->setupUi(this);

  ui->ButtonBox->button(QDialogButtonBox::Apply)->setEnabled(!m_FrameworkPath.isEmpty() && !m_DevToolsPath.isEmpty());

  connect(ui->ButtonBox, SIGNAL(clicked(QAbstractButton*)),this,SLOT(handleButtonBoxClick(QAbstractButton*)));
  connect(ui->RunButton, SIGNAL(clicked()),this,SLOT(runDetection()));
  connect(ui->BrowseButton, SIGNAL(clicked()),this,SLOT(browseQtRootPath()));
}


// =====================================================================
// =====================================================================


DetectQtDevToolsDialog::~DetectQtDevToolsDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void DetectQtDevToolsDialog::handleButtonBoxClick(QAbstractButton* Button)
{
  if (ui->ButtonBox->standardButton(Button) == QDialogButtonBox::Apply)
    accept();
  else
    reject();
}


// =====================================================================
// =====================================================================


void DetectQtDevToolsDialog::browseQtRootPath()
{
  QString SelectedDir = QFileDialog::getExistingDirectory(this,tr("Select working directory"));
  if (SelectedDir !=  "")
  {
    QString NativePath = QDir::toNativeSeparators(SelectedDir);
    ui->QtRootPathEdit->setText(NativePath);
  }
}

// =====================================================================
// =====================================================================


void DetectQtDevToolsDialog::setWidgetsEnabled(bool Enabled)
{
  ui->ButtonBox->setEnabled(Enabled);
  ui->QtRootPathEdit->setEnabled(Enabled);
  ui->BrowseButton->setEnabled(Enabled);
  ui->RunButton->setEnabled(Enabled);
  QApplication::instance()->processEvents();
}


// =====================================================================
// =====================================================================


void DetectQtDevToolsDialog::runDetection()
{
  setWidgetsEnabled(false);

  m_FrameworkPath.clear();
  m_DevToolsPath.clear();

  ui->DetectionProcessTextEdit->clear();
  QApplication::instance()->processEvents();


  // *** search paths for Qt framework and mingw

  QStringList PotentialQtRootPaths;

  if (!ui->QtRootPathEdit->text().isEmpty())
    PotentialQtRootPaths << ui->QtRootPathEdit->text();
  else
  {
    QFileInfoList Drives = QDir::drives();
    for (auto& CurrentDrive : Drives)
      PotentialQtRootPaths << CurrentDrive.canonicalPath()+"Qt";
  }


  // detect qt installation by searching for qtpaths.exe
  // look in Qt dirs at the root of each local volume

  QString qtpathsPath = "";

  ui->DetectionProcessTextEdit->insertHtml("<b>=== "+tr("Searching for qtpaths.exe")+" ===</b><br>");
  QApplication::instance()->processEvents();

  while (qtpathsPath.isEmpty() && !PotentialQtRootPaths.isEmpty())
  {
    QString CurrentPath = PotentialQtRootPaths.first();
    PotentialQtRootPaths.removeFirst();

    ui->DetectionProcessTextEdit->insertHtml(tr("Searching in %1... ").arg(QDir::toNativeSeparators(CurrentPath)));
    QApplication::instance()->processEvents();

    if (QFileInfo(CurrentPath).isDir())
    {
      QDirIterator DirIt(CurrentPath, QDirIterator::Subdirectories);

      while (qtpathsPath.isEmpty() && DirIt.hasNext())
      {
        QDir CurrentDir = QDir(DirIt.next());
        if (CurrentDir.exists("qtpaths.exe"))
          qtpathsPath = CurrentDir.absoluteFilePath("qtpaths.exe");
      }
    }

    if (qtpathsPath.isEmpty())
      ui->DetectionProcessTextEdit->insertHtml(tr("not found")+"<br>");
    else
      ui->DetectionProcessTextEdit->insertHtml(tr("found (%1)").arg(QDir::toNativeSeparators(qtpathsPath))+"<br>");

    QApplication::instance()->processEvents();
  }

  // use qtpaths.exe for locating qt install prefix

  if (!qtpathsPath.isEmpty())
  {
    ui->DetectionProcessTextEdit->insertHtml("<b>=== "+tr("Searching for Qt framework path")+" ===</b><br>");
    QApplication::instance()->processEvents();

    QProcess Process;
    Process.start(qtpathsPath, QStringList() << "--install-prefix");
    Process.waitForFinished();

    if (Process.exitStatus() == QProcess::NormalExit && Process.exitCode() == 0)
    {
      QString InstallPrefix(Process.readAllStandardOutput());
      InstallPrefix.replace("\n","");

      m_FrameworkPath = InstallPrefix+"/bin";
      ui->DetectionProcessTextEdit->insertHtml(
          "<font style='color: green'>"+
          tr("Qt framework path: %1").arg(QDir::toNativeSeparators(m_FrameworkPath))+
          "</font><br>");
      QApplication::instance()->processEvents();
    }
    else
    {
      ui->DetectionProcessTextEdit->insertHtml(
          "<font style='color: red'>"+
          tr("Qt framework not found")+
          "</font><br>");
      QApplication::instance()->processEvents();
    }
  }


  // find Tools directory and search for mingw in upper directories (gcc.exe, g++.exe, mingw32-make.exe)
  if (!m_FrameworkPath.isEmpty())
  {
    ui->DetectionProcessTextEdit->insertHtml("<b>=== "+tr("Searching for Qt development tools")+" ===</b><br>");
    QApplication::instance()->processEvents();

    QStringList UpperDirs = qtpathsPath.split("/");

    while (m_DevToolsPath.isEmpty() && UpperDirs.size() > 2)
    {
      UpperDirs.removeLast();
      QString PathToExplore = UpperDirs.join("/");

      ui->DetectionProcessTextEdit->insertHtml(tr("Searching in %1... ").arg(QDir::toNativeSeparators(PathToExplore)));
      QApplication::instance()->processEvents();

      QDirIterator DirIt(PathToExplore,QDirIterator::Subdirectories);
      while (m_DevToolsPath.isEmpty() && DirIt.hasNext())
      {
        QDir CurrentDir = QDir(DirIt.next());
        if (CurrentDir.exists("gcc.exe") && CurrentDir.exists("g++.exe") && CurrentDir.exists("mingw32-make.exe"))
        {
          m_DevToolsPath = CurrentDir.canonicalPath();
          ui->DetectionProcessTextEdit->insertHtml(tr("found")+"<br>");
          ui->DetectionProcessTextEdit->insertHtml(
              "<font style='color: green'>"+
              tr("Qt development tools path: %1").arg(QDir::toNativeSeparators(m_DevToolsPath))+
              "</font><br>");
          QApplication::instance()->processEvents();
        }
      }

      if (m_DevToolsPath.isEmpty())
      {
        ui->DetectionProcessTextEdit->insertHtml(tr("not found")+"<br>");
        QApplication::instance()->processEvents();
      }
    }
  }

  if (m_DevToolsPath.isEmpty())
  {
    ui->DetectionProcessTextEdit->insertHtml(
        "<font style='color: red'>"+
        tr("Qt development tools not found")+
        "</font><br>");
    QApplication::instance()->processEvents();
  }

  ui->ButtonBox->button(QDialogButtonBox::Apply)->setEnabled(isDetectionSuccessful());
  setWidgetsEnabled(true);
  QApplication::instance()->processEvents();
}


} } } // namespaces
