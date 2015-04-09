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
 @file WareSrcWidget.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#include "WareSrcWidget.hpp"
#include "ui_WareSrcWidget.h"

#include <QList>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>

#include <openfluid/base/FrameworkException.hpp>

#include <openfluid/ui/waresdev/WareSrcFileEditor.hpp>
#include <openfluid/ui/waresdev/WareSrcToolbar.hpp>
#include <openfluid/ui/waresdev/TextEditMsgStream.hpp>
#include <openfluid/ui/waresdev/NewSrcFileAssistant.hpp>
#include <openfluid/ui/waresdev/WareExplorerDialog.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcWidget::WareSrcWidget(const openfluid::waresdev::WareSrcManager::PathInfo& Info, bool IsStandalone,
                             openfluid::waresdev::WareSrcContainer::ConfigMode Config,
                             openfluid::waresdev::WareSrcContainer::BuildMode Build, QWidget* Parent) :
    QWidget(Parent), ui(new Ui::WareSrcWidget),
    m_Container(Info.m_AbsolutePathOfWare, Info.m_WareType, Info.m_WareName), m_IsStandalone(IsStandalone)
{
  ui->setupUi(this);

  QList<int> Sizes;
  Sizes << 1000 << 180;
  ui->splitter->setSizes(Sizes);

  if (m_IsStandalone)
  {
    WareSrcToolbar* TB = new WareSrcToolbar(true, this);

    ui->Toolbar_Layout->addWidget(TB);

    connect(TB->action("NewFile"), SIGNAL(triggered()), this, SLOT(newFile()));
    connect(TB->action("OpenFile"), SIGNAL(triggered()), this, SLOT(openFile()));
    connect(TB->action("SaveFile"), SIGNAL(triggered()), this, SLOT(saveCurrentEditor()));
    connect(TB->action("SaveAsFile"), SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(TB->action("CloseFile"), SIGNAL(triggered()), this, SLOT(closeCurrentEditor()));
    connect(TB->action("DeleteFile"), SIGNAL(triggered()), this, SLOT(deleteCurrentFile()));

    connect(TB->action("FindReplace"), SIGNAL(triggered()), this, SIGNAL(findReplaceRequested()));

    connect(TB->action("Release"), SIGNAL(triggered()), this, SLOT(setReleaseMode()));
    connect(TB->action("Debug"), SIGNAL(triggered()), this, SLOT(setDebugMode()));
    connect(TB->action("BuildInstall"), SIGNAL(triggered()), this, SLOT(setBuildWithInstallMode()));
    connect(TB->action("BuildOnly"), SIGNAL(triggered()), this, SLOT(setBuildNoInstallMode()));
    connect(TB->action("Configure"), SIGNAL(triggered()), this, SLOT(configure()));
    connect(TB->action("Build"), SIGNAL(triggered()), this, SLOT(build()));

    connect(TB->action("OpenTerminal"), SIGNAL(triggered()), this, SIGNAL(openTerminalRequested()));
    connect(TB->action("OpenExplorer"), SIGNAL(triggered()), this, SIGNAL(openExplorerRequested()));
  }

  connect(ui->WareSrcFileCollection, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseFileTabRequested(int)));
  connect(ui->WareSrcFileCollection, SIGNAL(currentChanged(int)), this, SLOT(onCurrentTabChanged(int)));

  m_Container.setConfigMode(Config);
  m_Container.setBuildMode(Build);

  mp_TextEditMsgStream = new openfluid::ui::waresdev::TextEditMsgStream(ui->WareSrcMessageWidget);
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


void WareSrcWidget::openFileTab(const openfluid::waresdev::WareSrcManager::PathInfo& Info, int Index)
{
  if (!setCurrent(Info))
    addNewFileTab(Index, Info.m_AbsolutePath, Info.m_FileName, QDir::toNativeSeparators(Info.m_RelativePathToWareDir));
}


// =====================================================================
// =====================================================================


void WareSrcWidget::addNewFileTab(int Index, const QString& AbsolutePath, const QString& TabLabel,
                                  const QString& TabTooltip)
{
  WareSrcFileEditor* Widget = new WareSrcFileEditor(AbsolutePath, this);

  int Pos = ui->WareSrcFileCollection->insertTab(Index, Widget, TabLabel);
  ui->WareSrcFileCollection->setTabToolTip(Pos, TabTooltip);

  m_WareSrcFilesByPath[AbsolutePath] = Widget;

  ui->WareSrcFileCollection->setCurrentWidget(Widget);

  if (m_IsStandalone)
    Widget->installEventFilter(this);

  connect(Widget, SIGNAL(editorTxtChanged(WareSrcFileEditor*,bool)), this,
          SLOT(onEditorTxtModified(WareSrcFileEditor*,bool)));

  connect(Widget, SIGNAL(editorSaved()), this, SIGNAL(editorSaved()));
}


// =====================================================================
// =====================================================================


int WareSrcWidget::closeCurrentEditor(bool WithConfirm)
{
  return onCloseFileTabRequested(ui->WareSrcFileCollection->currentIndex(), WithConfirm);
}


// =====================================================================
// =====================================================================


int WareSrcWidget::onCloseFileTabRequested(int Index, bool WithConfirm)
{
  int ClosedTabPos = -1;

  if (WareSrcFileEditor* Editor = qobject_cast<WareSrcFileEditor*>(ui->WareSrcFileCollection->widget(Index)))
  {
    int Choice = QMessageBox::Discard;

    if (WithConfirm && Editor->document()->isModified())
    {
      QMessageBox MsgBox;
      MsgBox.setText(tr("The document has been modified."));
      MsgBox.setInformativeText(tr("Do you want to save your changes?"));
      MsgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
      MsgBox.setDefaultButton(QMessageBox::Save);
      Choice = MsgBox.exec();
    }

    switch (Choice)
    {
      case QMessageBox::Save:
        Editor->saveContent();
      case QMessageBox::Discard:
        ClosedTabPos = closeFileTab(Editor);
        break;
      case QMessageBox::Cancel:
      default:
        break;
    }

  }

  return ClosedTabPos;
}


// =====================================================================
// =====================================================================


int WareSrcWidget::closeFileTab(WareSrcFileEditor* Editor)
{
  if (!Editor)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Null Editor");

  int Index = ui->WareSrcFileCollection->indexOf(Editor);

  ui->WareSrcFileCollection->removeTab(Index);

  m_WareSrcFilesByPath.remove(Editor->getFilePath());

  delete Editor;

  emit wareTextModified(this, isModified());

  return Index;
}


// =====================================================================
// =====================================================================


void WareSrcWidget::saveAllFileTabs()
{
  foreach(WareSrcFileEditor* Editor,m_WareSrcFilesByPath){
  Editor->saveContent();
}
}


// =====================================================================
// =====================================================================


void WareSrcWidget::closeAllFileTabs()
{
  foreach(WareSrcFileEditor* Editor,m_WareSrcFilesByPath){
  closeFileTab(Editor);
}
}


// =====================================================================
// =====================================================================


QString WareSrcWidget::getCurrentFilePath()
{
  if (WareSrcFileEditor* Editor = currentEditor())
    return Editor->getFilePath();

  return "";
}


// =====================================================================
// =====================================================================


WareSrcFileEditor* WareSrcWidget::currentEditor()
{
  return qobject_cast<WareSrcFileEditor*>(ui->WareSrcFileCollection->currentWidget());
}


// =====================================================================
// =====================================================================


int WareSrcWidget::closeFileTab(const QString& Path)
{
  if (m_WareSrcFilesByPath.contains(Path))
    return closeFileTab(m_WareSrcFilesByPath.value(Path));

  return -1;
}


// =====================================================================
// =====================================================================


bool WareSrcWidget::eventFilter(QObject* Obj, QEvent* Event)
{
  if (Event->type() == QEvent::ShortcutOverride && static_cast<QKeyEvent*>(Event)->matches(QKeySequence::Save))
  {
    saveCurrentEditor();
    Event->accept();
    return true;
  }

  return QObject::eventFilter(Obj, Event);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::openDefaultFiles()
{
  foreach(QString F,m_Container.getDefaultFilesPaths()){
  QString FileName = QFileInfo(F).fileName();
  addNewFileTab(-1,F, FileName, QDir::toNativeSeparators(FileName));
}
}


// =====================================================================
// =====================================================================


bool WareSrcWidget::setCurrent(const openfluid::waresdev::WareSrcManager::PathInfo& Info)
{
  WareSrcFileEditor* Widget = m_WareSrcFilesByPath.value(Info.m_AbsolutePath, 0);

  if (Widget)
    ui->WareSrcFileCollection->setCurrentWidget(Widget);

  return Widget;
}


// =====================================================================
// =====================================================================


openfluid::waresdev::WareSrcContainer& WareSrcWidget::wareSrcContainer()
{
  return m_Container;
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setReleaseMode()
{
  m_Container.setConfigMode(openfluid::waresdev::WareSrcContainer::CONFIG_RELEASE);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setDebugMode()

{
  m_Container.setConfigMode(openfluid::waresdev::WareSrcContainer::CONFIG_DEBUG);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setBuildWithInstallMode()

{
  m_Container.setBuildMode(openfluid::waresdev::WareSrcContainer::BUILD_WITHINSTALL);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setBuildNoInstallMode()

{
  m_Container.setBuildMode(openfluid::waresdev::WareSrcContainer::BUILD_NOINSTALL);
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


void WareSrcWidget::onEditorTxtModified(WareSrcFileEditor* Editor, bool Modified)
{
  int Pos = ui->WareSrcFileCollection->indexOf(Editor);

  if (Pos > -1)
  {
    QString Label = ui->WareSrcFileCollection->tabText(Pos);
    bool ModifiedState = Label.startsWith("*");

    if (Modified && !ModifiedState)
      ui->WareSrcFileCollection->setTabText(Pos, Label.prepend("*"));
    else if (!Modified && ModifiedState)
      ui->WareSrcFileCollection->setTabText(Pos, Label.remove(0, 1));

    emit wareTextModified(this, isModified());
  }

}


// =====================================================================
// =====================================================================


bool WareSrcWidget::isModified()
{
  foreach(WareSrcFileEditor* Editor,m_WareSrcFilesByPath.values()){
  if(Editor->document()->isModified())
  return true;
}

return false;
}


// =====================================================================
// =====================================================================


void WareSrcWidget::saveCurrentEditor()
{
  if (WareSrcFileEditor* Editor = currentEditor())
    Editor->saveContent();
}


// =====================================================================
// =====================================================================


QString WareSrcWidget::saveAs(const QString& TopDirectory)
{
  WareSrcFileEditor* CurrentEditor = currentEditor();
  if (!CurrentEditor)
  {
    QMessageBox::warning(0, tr("No open file"), tr("No file to save as"));
    return "";
  }

  QString CurrentFilePath = CurrentEditor->getFilePath();
  QString CurrentWarePath = m_Container.getAbsolutePath();

  QString NewFilePath = WareExplorerDialog::getSaveFilePath(this,
                                                            TopDirectory.isEmpty() ? CurrentWarePath : TopDirectory,
                                                            CurrentFilePath);

  if (NewFilePath.isEmpty())
    return "";

  if (NewFilePath == CurrentFilePath)
  {
    saveCurrentEditor();
    return "";
  }

  CurrentEditor->saveContentToPath(NewFilePath);

  openfluid::waresdev::WareSrcManager::PathInfo NewPathInfo = openfluid::waresdev::WareSrcManager::instance()
      ->getPathInfo(NewFilePath);

  int CurrentIndex = closeFileTab(CurrentEditor);

  if (NewPathInfo.m_AbsolutePathOfWare == CurrentWarePath)
  {
    // in case the new path was already opened
    closeFileTab(NewFilePath);

    openFileTab(NewPathInfo, CurrentIndex);

    return "";
  }

  return NewFilePath;
}


// =====================================================================
// =====================================================================


void WareSrcWidget::newFile()
{
  NewSrcFileAssistant Assistant(m_Container, this);
  Assistant.exec();

  QString NewFilePath = Assistant.getNewFilePath();
  if (!NewFilePath.isEmpty())
  {
    m_Container.update();
    openFileTab(openfluid::waresdev::WareSrcManager::instance()->getPathInfo(NewFilePath));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::deleteCurrentFile()
{
  if (WareSrcFileEditor* Editor = currentEditor())
  {
    QString Path = Editor->getFilePath();

    if (QMessageBox::warning(this, tr("Delete file"), tr("Are you sure you want to delete \"%1\"?").arg(Path),
                             QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel)
        == QMessageBox::Cancel)
      return;

    // Careful as it deletes Editor
    closeFileTab(Editor);

    if (QDir().remove(Path))
      m_Container.update();
    else
      QMessageBox::critical(0, "Error", tr("Unable to remove the file \"%1\"").arg(Path));
  }
  else
    QMessageBox::warning(0, tr("No open file"), tr("No file to delete"));
}


// =====================================================================
// =====================================================================


void WareSrcWidget::openFile()
{
  QString PathToOpen = openfluid::ui::waresdev::WareExplorerDialog::getOpenFilePath(this, m_Container.getAbsolutePath(),
                                                                                    getCurrentFilePath());

  if (PathToOpen.isEmpty())
    return;

  openFileTab(openfluid::waresdev::WareSrcManager::instance()->getPathInfo(PathToOpen));
}


// =====================================================================
// =====================================================================


void WareSrcWidget::onCurrentTabChanged(int Index)
{
  if (WareSrcFileEditor* Editor = qobject_cast<WareSrcFileEditor*>(ui->WareSrcFileCollection->widget(Index)))
    emit currentTabChanged(Editor->getFilePath());
}


// =====================================================================
// =====================================================================


} } }  // namespaces
