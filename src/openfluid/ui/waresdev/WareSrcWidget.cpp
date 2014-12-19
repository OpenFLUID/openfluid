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
 \file WareSrcWidget.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "WareSrcWidget.hpp"
#include "ui_WareSrcWidget.h"

#include <QList>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

#include "WareSrcFileEditor.hpp"
#include "WareSrcToolbar.hpp"
#include "TextEditMsgStream.hpp"


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcWidget::WareSrcWidget(
    const openfluid::waresdev::WareSrcManager::PathInfo& Info,
    bool IsStandalone, openfluid::waresdev::WareSrcContainer::ConfigMode Config,
    openfluid::waresdev::WareSrcContainer::BuildMode Build, QWidget* Parent) :
    QWidget(Parent), ui(new Ui::WareSrcWidget), m_Container(
        Info.m_AbsolutePathOfWare, Info.m_WareType, Info.m_WareName)
{
  ui->setupUi(this);

  QList<int> Sizes;
  Sizes << 1000 << 180;
  ui->splitter->setSizes(Sizes);

  if (IsStandalone)
  {
    WareSrcToolbar* TB = new WareSrcToolbar(true, this);

    ui->Toolbar_Layout->addWidget(TB);

    connect(TB->getAction("NewFile"), SIGNAL(triggered()), this,
            SLOT(showNotYetImplemented()));
    connect(TB->getAction("OpenFile"), SIGNAL(triggered()), this,
            SLOT(showNotYetImplemented()));
    connect(TB->getAction("SaveFile"), SIGNAL(triggered()), this,
            SLOT(showNotYetImplemented()));
    connect(TB->getAction("SaveAsFile"), SIGNAL(triggered()), this,
            SLOT(showNotYetImplemented()));
    connect(TB->getAction("CloseFile"), SIGNAL(triggered()), this,
            SLOT(showNotYetImplemented()));
    connect(TB->getAction("DeleteFile"), SIGNAL(triggered()), this,
            SLOT(showNotYetImplemented()));

    connect(TB->getAction("Release"), SIGNAL(triggered()), this,
            SLOT(setReleaseMode()));
    connect(TB->getAction("Debug"), SIGNAL(triggered()), this,
            SLOT(setDebugMode()));
    connect(TB->getAction("BuildInstall"), SIGNAL(triggered()), this,
            SLOT(setBuildWithInstallMode()));
    connect(TB->getAction("BuildOnly"), SIGNAL(triggered()), this,
            SLOT(setBuildNoInstallMode()));
    connect(TB->getAction("Configure"), SIGNAL(triggered()), this,
            SLOT(configure()));
    connect(TB->getAction("Build"), SIGNAL(triggered()), this, SLOT(build()));
  }

  m_Container.setConfigMode(Config);
  m_Container.setBuildMode(Build);

  mp_TextEditMsgStream = new openfluid::ui::waresdev::TextEditMsgStream(
      ui->WareSrcMessageWidget);
  m_Container.setMsgStream(*mp_TextEditMsgStream);
}


// =====================================================================
// =====================================================================


WareSrcWidget::~WareSrcWidget()
{
  delete mp_TextEditMsgStream;
  delete ui;
}


// =====================================================================
// =====================================================================

void WareSrcWidget::openFile(
    const openfluid::waresdev::WareSrcManager::PathInfo& Info)
{
  WareSrcFileEditor* Widget = m_WareSrcFilesByPath.value(Info.m_AbsolutePath,
                                                         0);

  if (Widget)
    ui->WareSrcFileCollection->setCurrentWidget(Widget);
  else
    addNewFileTab(Info.m_AbsolutePath, Info.m_FileName,
                  QDir::toNativeSeparators(Info.m_RelativePathToWareDir));
}


// =====================================================================
// =====================================================================


void WareSrcWidget::addNewFileTab(const QString& AbsolutePath,
                                  const QString& TabLabel,
                                  const QString& TabTooltip)
{
  WareSrcFileEditor* Widget = new WareSrcFileEditor(AbsolutePath, this);

  int Pos = ui->WareSrcFileCollection->addTab(Widget, TabLabel);
  ui->WareSrcFileCollection->setTabToolTip(Pos, TabTooltip);

  m_WareSrcFilesByPath[AbsolutePath] = Widget;

  ui->WareSrcFileCollection->setCurrentWidget(Widget);
}

// =====================================================================
// =====================================================================


void WareSrcWidget::openDefaultFiles()
{
  foreach(QString F,m_Container.getDefaultFiles()){
  QString FileName = QFileInfo(F).fileName();
  addNewFileTab(F, FileName, QDir::toNativeSeparators(FileName));
}
}


// =====================================================================
// =====================================================================


bool WareSrcWidget::setCurrent(
    const openfluid::waresdev::WareSrcManager::PathInfo& Info)
{
  WareSrcFileEditor* Widget = m_WareSrcFilesByPath.value(Info.m_AbsolutePath,
                                                         0);

  if (Widget)
    ui->WareSrcFileCollection->setCurrentWidget(Widget);

  return Widget;
}


// =====================================================================
// =====================================================================


openfluid::waresdev::WareSrcContainer& WareSrcWidget::getWareSrcContainer()
{
  return m_Container;
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setReleaseMode()
{
  m_Container.setConfigMode(
      openfluid::waresdev::WareSrcContainer::CONFIG_RELEASE);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setDebugMode()

{
  m_Container.setConfigMode(
      openfluid::waresdev::WareSrcContainer::CONFIG_DEBUG);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setBuildWithInstallMode()

{
  m_Container.setBuildMode(
      openfluid::waresdev::WareSrcContainer::BUILD_WITHINSTALL);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setBuildNoInstallMode()

{
  m_Container.setBuildMode(
      openfluid::waresdev::WareSrcContainer::BUILD_NOINSTALL);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::configure()

{
  m_Container.configure();
}


// =====================================================================
// =====================================================================


void WareSrcWidget::build()

{
  m_Container.build();
}


// =====================================================================
// =====================================================================


void WareSrcWidget::showNotYetImplemented()
{
  QMessageBox::information(0, "", "Not yet implemented");
}


// =====================================================================
// =====================================================================


} } } // namespaces
