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

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 @author Armel Thöni <armel.thoni@inrae.fr>
*/


#include <QList>
#include <QDir>
#include <QUrl>
#include <QMessageBox>
#include <QTextStream>


#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/ui/common/EditSignatureDialog.hpp>
#include <openfluid/ui/waresdev/WareSrcWidget.hpp>
#include <openfluid/ui/waresdev/WareSrcFileEditor.hpp>
#include <openfluid/ui/waresdev/WareSrcToolbar.hpp>
#include <openfluid/ui/waresdev/TextEditMsgStream.hpp>
#include <openfluid/ui/waresdev/WareBuildOptionsDialog.hpp>
#include <openfluid/ui/waresdev/WareExplorerDialog.hpp>
#include <openfluid/ui/common/MessageFrame.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/waresdev/WareBuildOptions.hpp>
#include <openfluid/waresdev/WareSignatureSerializer.hpp>
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>

#include <openfluid/config.hpp>

#include "ui_WareSrcWidget.h"


namespace openfluid { namespace ui { namespace waresdev {


WareSrcWidget::WareSrcWidget(const openfluid::waresdev::WareSrcEnquirer::WarePathInfo& Info, bool IsStandalone,
                             QWidget* Parent) :
    QWidget(Parent), ui(new Ui::WareSrcWidget),
    m_Container(QString::fromStdString(Info.AbsoluteWarePath),Info.WareType,QString::fromStdString(Info.WareDirName)), 
    m_IsStandalone(IsStandalone)
{
  ui->setupUi(this);

  QList<int> Sizes;
  Sizes << 1000 << 180;
  ui->splitter->setSizes(Sizes);

  bool IsSimulator = Info.WareType==openfluid::ware::WareType::SIMULATOR;

  mp_ActionsCollection = new openfluid::ui::waresdev::WareSrcActionsCollection(m_IsStandalone, m_IsStandalone, this);
  mp_WareSrcToolBar = new WareSrcToolbar(m_IsStandalone, mp_ActionsCollection, IsSimulator, this);


  mp_WareSrcToolBar->setObjectName("WareToolbar");
  
  if (m_IsStandalone)
  {
    connect(mp_ActionsCollection->action("OpenTerminal"), SIGNAL(triggered()), this, SIGNAL(openTerminalRequested()));
    connect(mp_ActionsCollection->action("OpenExplorer"), SIGNAL(triggered()), this, SIGNAL(openExplorerRequested()));

    connect(mp_ActionsCollection->action("APIDoc"), SIGNAL(triggered()), this, SIGNAL(openAPIDocRequested()));
    
    QMap<QString, QAction*> ExternalToolsActions = mp_WareSrcToolBar->externalToolsActions();
    for (auto const& Alias : ExternalToolsActions.keys())
    {
      connect(ExternalToolsActions[Alias], SIGNAL(triggered()), this, SLOT(onOpenExternalToolRequested()));
    }

    connect(mp_ActionsCollection->action("OpenWareOptions"), SIGNAL(triggered()),
            this, SLOT(displayBuildOptionsDialog()));
    mp_WareSrcToolBar->setStyleSheet(
      QString(R"(
#WareToolbar {
  background-color: %1;
})"
    ).arg(openfluid::ui::config::WARETOOLBAR_BGCOLOR));
  }
  else
  {
    mp_WareSrcToolBar->setIconSize(QSize(24,24));

    mp_WareSrcToolBar->setStyleSheet(
      QString(R"(
QToolButton {
  color: #555555;
  padding-left : 10px;
  padding-right : 10px;
}

#WareToolbar {
  background-color: %1;
}

#WareToolbar QToolButton, QLabel {
  background-color: %1;
  color: black;
}

#WareToolbar QToolButton[popupMode=1] {
  background-color: %1;
  border: 1px solid %1;
  padding-left : 10px;
  padding-right : 20px;
}

#WareToolbar QToolButton::hover {
  background-color: %2;
  border : 1px solid %3;
  border-radius: 4px;
}

#WareToolbar QToolButton::menu-button {
  background-color: %1;
  border: 1px solid %1;
  border-radius: 4px;
}

#WareToolbar QToolButton::menu-button:pressed, QToolButton::menu-button:hover {
  background-color: %2;
  border : 1px solid %3;
  border-radius: 4px;
}
             )").arg(
          openfluid::ui::config::WARETOOLBAR_BGCOLOR, "black",
          "dark gray"));
  }
  
  ui->Toolbar_Layout->addWidget(mp_WareSrcToolBar);

  connect(mp_ActionsCollection->action("NewFile"), SIGNAL(triggered()), this, SLOT(newFile()));
  connect(mp_ActionsCollection->action("OpenFile"), SIGNAL(triggered()), this, SLOT(openFile()));
  connect(mp_ActionsCollection->action("SaveFile"), SIGNAL(triggered()), this, SLOT(saveCurrentEditor()));
  connect(mp_ActionsCollection->action("SaveAsFile"), SIGNAL(triggered()), this, SLOT(saveAs()));
  connect(mp_ActionsCollection->action("SaveAllFiles"), SIGNAL(triggered()), this, SLOT(saveAllFileTabs()));
  connect(mp_ActionsCollection->action("CloseFile"), SIGNAL(triggered()), this, SLOT(closeCurrentEditor()));
  connect(mp_ActionsCollection->action("DeleteFile"), SIGNAL(triggered()), this, SLOT(deleteCurrentFile()));

  connect(mp_ActionsCollection->action("Copy"), SIGNAL(triggered()), this, SLOT(copyText()));
  connect(mp_ActionsCollection->action("Cut"), SIGNAL(triggered()), this, SLOT(cutText()));
  connect(mp_ActionsCollection->action("Paste"), SIGNAL(triggered()), this, SLOT(pasteText()));
  connect(mp_ActionsCollection->action("FindReplace"), SIGNAL(triggered()), this, SIGNAL(findReplaceRequested()));
  connect(mp_ActionsCollection->action("EditSignature"), SIGNAL(triggered()), this, SLOT(editSignature()));//TOIMPL
  connect(mp_ActionsCollection->action("GoToLine"), SIGNAL(triggered()), this, SLOT(goToLine()));

  connect(mp_ActionsCollection->action("ConfigureWare"), SIGNAL(triggered()), this, SLOT(configure()));
  connect(mp_ActionsCollection->action("BuildWare"), SIGNAL(triggered()), this, SLOT(build()));
  
  if (IsSimulator)
  {
    connect(mp_ActionsCollection->action("GenerateDoc"), SIGNAL(triggered()), this, SLOT(generateDoc()));
  }

  connect(ui->WareSrcFileCollection, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseFileTabRequested(int)));
  connect(ui->WareSrcFileCollection, SIGNAL(currentChanged(int)), this, SLOT(onCurrentTabChanged(int)));

  connect(&m_Container, SIGNAL(processFinished()), this, SLOT(onProcessFinished()));

  connect(&m_Container, SIGNAL(configureProcessLaunched(openfluid::ware::WareType, const QString&)),
          this, SLOT(notifyConfigureLaunched(openfluid::ware::WareType, const QString&)));

  connect(&m_Container, SIGNAL(configureProcessFinished(openfluid::ware::WareType, const QString&)),
          this, SLOT(notifyConfigureFinished(openfluid::ware::WareType, const QString&)));

  connect(&m_Container, SIGNAL(buildProcessLaunched(openfluid::ware::WareType, const QString&)),
          this, SLOT(notifyBuildLaunched(openfluid::ware::WareType, const QString&)));

  connect(&m_Container, SIGNAL(buildProcessFinished(openfluid::ware::WareType, const QString&)),
          this, SLOT(notifyBuildFinished(openfluid::ware::WareType, const QString&)));


  // BOARD TAB
  mp_Board = new openfluid::ui::waresdev::WareStatusDashboardWidget(this, m_Container.getAbsolutePath());
  m_TabIndexByName["Board"] = ui->tabWidget->addTab(mp_Board, "Board");
  
  connect(mp_Board, SIGNAL(operationRequested(const QString&)), this, SLOT(onOperationRequested(const QString&)));

  // MESSAGES TAB
  mp_MessagesWidget = new openfluid::ui::waresdev::WareSrcMsgViewer(this);
  m_TabIndexByName["Messages"] = ui->tabWidget->addTab(mp_MessagesWidget, "Messages");

  mp_TextEditMsgStream = new openfluid::ui::waresdev::TextEditMsgStream(mp_MessagesWidget);
  m_Container.setMsgStream(*mp_TextEditMsgStream);

  connect(mp_MessagesWidget, SIGNAL(messageClicked(WareSrcMsgParser::WareSrcMsg&)),
          this, SLOT(onMessageClicked(WareSrcMsgParser::WareSrcMsg&)));


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


void WareSrcWidget::displayBuildOptionsDialog()
{
  openfluid::waresdev::WareBuildOptions WareBuildOptions;
  openfluid::ui::waresdev::WareBuildOptionsDialog SettingsDialog(WareBuildOptions, QApplication::activeWindow());

  if (SettingsDialog.exec() == QDialog::Accepted)
  {
    WareBuildOptions = SettingsDialog.getOptions();
    WareBuildOptions.writeOptionsInWorkspace();
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::openFileTab(const openfluid::waresdev::WareSrcEnquirer::WarePathInfo& Info, int Index)
{
  if (!setCurrent(Info))
  {
    addNewFileTab(Index, 
                  QString::fromStdString(Info.AbsolutePath), QString::fromStdString(Info.FileName),
                  QDir::toNativeSeparators(QString::fromStdString(Info.RelativePathToWare)));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::addNewFileTab(int Index, const QString& AbsolutePath, const QString& TabLabel,
                                  const QString& TabTooltip)
{
  WareFileEditor* Editor;
  bool IsMeta = false;

  if (QFileInfo(AbsolutePath).fileName().toStdString() == openfluid::config::WARESDEV_WAREMETA_FILE)
  {
    IsMeta = true;
  }
  WareSrcFileEditor* SrcEditor = new WareSrcFileEditor(AbsolutePath, this);

  for (WareSrcMsgParser::WareSrcMsg Msg : m_Container.getMessages())
  {
    if (Msg.m_Path == AbsolutePath)
    {
      SrcEditor->addLineMessage(Msg);
    }
  }

  connect(SrcEditor, SIGNAL(editorTxtChanged(WareFileEditor*,bool)), this,
          SLOT(onEditorTxtModified(WareFileEditor*,bool)));

  connect(SrcEditor, SIGNAL(editorSaved()), this, SIGNAL(editorSaved()));

  Editor = SrcEditor;
  QWidget* Widget = Editor->getWidget();

  int Pos = ui->WareSrcFileCollection->insertTab(Index, Widget, TabLabel);
  ui->WareSrcFileCollection->setTabToolTip(Pos, TabTooltip);

  m_WareFilesByPath[AbsolutePath] = Editor;

  ui->WareSrcFileCollection->setCurrentWidget(Widget);

  if (m_IsStandalone)
  {
    Widget->installEventFilter(this);
  }
  if (IsMeta)
  {
    QMessageBox::warning(this, tr("Signature direct edit"), 
                               tr("Warning: you are editing the signature file of the ware.\n"
                                  "It is advised to use instead the dedicated dialog to avoid any issue."));
  } 
}


// =====================================================================
// =====================================================================


void WareSrcWidget::onOpenExternalToolRequested()
{
  QAction* Sender = (QAction*)(QObject::sender());
  if (Sender != nullptr)
  {
    emit openExternalToolRequested(Sender->data().toString(), "");
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::onOperationRequested(const QString& OperationCode)
{
  emit operationRequestedOnWare(OperationCode, QString::fromStdString(m_Container.getAbsolutePath()));
}


// =====================================================================
// =====================================================================


int WareSrcWidget::closeCurrentEditor(bool WithConfirm)
{
  return onCloseFileTabRequested(ui->WareSrcFileCollection->currentIndex(), WithConfirm);
}


// =====================================================================
// =====================================================================


int WareSrcWidget::editorCheckChangeClose(WareFileEditor* Editor, bool WithConfirm)
{
  int ClosedTabPos = -1;
  int Choice = QMessageBox::Discard;

  if (WithConfirm && Editor->isModified())
  {
    QString FileName = QFileInfo(Editor->getFilePath()).fileName();
    QMessageBox MsgBox;
    MsgBox.setText(tr("The document %1 has been modified.").arg(FileName));
    MsgBox.setInformativeText(tr("Do you want to save changes?"));
    MsgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    MsgBox.setDefaultButton(QMessageBox::Save);
    Choice = MsgBox.exec();
  }

  switch (Choice)
  {
    case QMessageBox::Save:
      Editor->saveContent();
      [[fallthrough]];
    case QMessageBox::Discard:
      ClosedTabPos = closeFileTab(Editor);
      break;
    case QMessageBox::Cancel:
    default:
      break;
  }
  return ClosedTabPos;
}


// =====================================================================
// =====================================================================


int WareSrcWidget::onCloseFileTabRequested(int Index, bool WithConfirm)
{
  int ClosedTabPos = -1;

  if (WareFileEditor* Editor = dynamic_cast<WareFileEditor*>(ui->WareSrcFileCollection->widget(Index)))
  {
    ClosedTabPos = editorCheckChangeClose(Editor, WithConfirm);
  }

  return ClosedTabPos;
}


// =====================================================================
// =====================================================================


int WareSrcWidget::closeFileTab(WareFileEditor* Editor)
{
  if (!Editor)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Null Editor");
  }

  int Index = ui->WareSrcFileCollection->indexOf(Editor->getWidget());

  ui->WareSrcFileCollection->removeTab(Index);

  m_WareFilesByPath.remove(Editor->getFilePath());

  delete Editor;

  emit wareTextModified(this, isWareModified());

  checkModifiedStatus();

  return Index;
}


// =====================================================================
// =====================================================================


void WareSrcWidget::closeFileTabsInFolder(QString FolderPath, bool Confirm)
{
  for (const auto& Path : m_WareFilesByPath.keys())
  {
    if (Path.startsWith(FolderPath))
    {
      editorCheckChangeClose(m_WareFilesByPath[Path], Confirm);
    }
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::saveAllFileTabs()
{
  for(WareFileEditor* Editor : m_WareFilesByPath.values())
  {
    if (Editor->isModified())
    {
      Editor->saveContent();
    }
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::closeAllFileTabs()
{
  for(WareFileEditor* Editor : m_WareFilesByPath.values())
  {
    closeFileTab(Editor);
  }
}


// =====================================================================
// =====================================================================


QString WareSrcWidget::getCurrentFilePath()
{
  if (WareFileEditor* Editor = currentEditor())
  {
    return Editor->getFilePath();
  }

  return "";
}


// =====================================================================
// =====================================================================


WareFileEditor* WareSrcWidget::currentEditor()
{
  return dynamic_cast<WareFileEditor*>(ui->WareSrcFileCollection->currentWidget());
}


// =====================================================================
// =====================================================================


int WareSrcWidget::closeFileTab(const QString& Path)
{
  if (m_WareFilesByPath.contains(Path))
  {
    return closeFileTab(m_WareFilesByPath.value(Path));
  }

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
  for(const auto& F : m_Container.getDefaultFilesPaths())
  {
    QString FileName = QFileInfo(QString::fromStdString(F)).fileName();
    addNewFileTab(-1,QString::fromStdString(F), FileName, QDir::toNativeSeparators(FileName));
  }
}


// =====================================================================
// =====================================================================


bool WareSrcWidget::setCurrent(const openfluid::waresdev::WareSrcEnquirer::WarePathInfo& Info)
{
  WareFileEditor* Widget = m_WareFilesByPath.value(QString::fromStdString(Info.AbsolutePath), 0);

  if (Widget)
  {
    ui->WareSrcFileCollection->setCurrentWidget(Widget->getWidget());
  }

  return Widget;
}


// =====================================================================
// =====================================================================


WareSrcUIContainer& WareSrcWidget::wareSrcContainer()
{
  return m_Container;
}


// =====================================================================
// =====================================================================


void WareSrcWidget::loadWareOptions()
{
  openfluid::waresdev::WareBuildOptions WareBuildOptions;
  m_Container.setConfigMode(WareBuildOptions.getConfigMode());
  m_Container.setBuildMode(WareBuildOptions.getBuildMode());
  if (WareBuildOptions.IsMultipleJobs)
  {
    m_Container.setBuildJobs(WareBuildOptions.JobsNumber);
  }
  else
  {
    m_Container.setBuildJobs(0);
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setConfigureMode(openfluid::waresdev::WareSrcContainer::ConfigMode Mode)
{
  m_Container.setConfigMode(Mode);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setBuildMode(openfluid::waresdev::WareSrcContainer::BuildMode Mode)
{
  m_Container.setBuildMode(Mode);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::setBuildJobs(unsigned int Jobs)
{
  m_Container.setBuildJobs(Jobs);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::configure()

{
  ui->tabWidget->setCurrentIndex(m_TabIndexByName["Messages"]);

  loadWareOptions();
  clearEditorsMessages();

  try
  {
    m_Container.configure();
  }
  catch (openfluid::base::FrameworkException& E )
  {
    QMessageBox::critical(nullptr, tr("Configure error"), QString::fromStdString(E.getMessage()));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::build()

{
  ui->tabWidget->setCurrentIndex(m_TabIndexByName["Messages"]);

  loadWareOptions();
  clearEditorsMessages();

  try
  {
    m_Container.build();
  }
  catch (openfluid::base::FrameworkException& E )
  {
    QMessageBox::critical(nullptr, tr("Build error"), QString::fromStdString(E.getMessage()));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::generateDoc()
{
  ui->tabWidget->setCurrentIndex(m_TabIndexByName["Messages"]);

  clearEditorsMessages();

  try
  {
    if (m_Container.getType() == openfluid::ware::WareType::SIMULATOR)
    {
      m_Container.generateDoc();
    }
    else
    {
      QMessageBox::warning(nullptr,tr("Generate documentation"),
                           tr("Documentation generator only works with simulators"));
    }
  }
  catch (openfluid::base::FrameworkException& E )
  {
    QMessageBox::critical(nullptr, tr("Generate doc error"), QString::fromStdString(E.getMessage()));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::onEditorTxtModified(WareFileEditor* Editor, bool Modified)
{
  int Pos = ui->WareSrcFileCollection->indexOf(Editor->getWidget());

  if (Pos > -1)
  {
    QString Label = ui->WareSrcFileCollection->tabText(Pos);
    bool ModifiedState = Label.startsWith("*");

    if (Modified && !ModifiedState)
    {
      ui->WareSrcFileCollection->setTabText(Pos, Label.prepend("*"));
    }
    else if (!Modified && ModifiedState)
    {
      ui->WareSrcFileCollection->setTabText(Pos, Label.remove(0, 1));
    }

    emit wareTextModified(this, isWareModified());

    checkModifiedStatus();
  }

}


// =====================================================================
// =====================================================================


bool WareSrcWidget::isWareModified()
{
  for(WareFileEditor* Editor : m_WareFilesByPath.values())
  {
    if(Editor->isModified())
    {
      return true;
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


bool WareSrcWidget::isWareProcessRunning() const
{
  return m_Container.isProcessRunning();
}


// =====================================================================
// =====================================================================


void WareSrcWidget::onWareChange()
{
  mp_Board->refresh();
}


// =====================================================================
// =====================================================================


void WareSrcWidget::saveCurrentEditor()
{
  if (WareFileEditor* Editor = currentEditor())
  {
    Editor->saveContent();

    ui->tabWidget->setCurrentIndex(m_TabIndexByName["Board"]);
    
    onWareChange();
  // TOIMPL emit wareChanged for other ware changes through watcher
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::editSignature(QString Path)
{
  if (Path.isEmpty())
  {
    Path = QDir::fromNativeSeparators(QDir(QString::fromStdString(
      m_Container.getAbsolutePath())+"/"+QString::fromStdString(openfluid::config::WARESDEV_WAREMETA_FILE)).path());
  }

  openfluid::ui::common::EditSignatureDialog Dlg;
  try
  {
    Dlg.initialize(Path);
  }
  catch (openfluid::base::FrameworkException& E)
  {
    QMessageBox::critical(nullptr, tr("Signature issue"), tr("In ") + Path + ":\n" + 
                                   QString::fromStdString(E.getMessage()) + 
                                   tr("\n\nTo solve this issue, you can edit the json file with an external program."));
    return;
  }
  if (Dlg.exec() == QDialog::Accepted)
  {
    Dlg.exportSignature(Path);
  }
}


// =====================================================================
// =====================================================================


QString WareSrcWidget::saveAs(const QString& TopDirectory)
{
  WareFileEditor* CurrentEditor = currentEditor();
  if (!CurrentEditor)
  {
    QMessageBox::warning(0, tr("No open file"), tr("No file to save"));
    return "";
  }

  QString CurrentFilePath = CurrentEditor->getFilePath();
  QString CurrentWarePath = QString::fromStdString(m_Container.getAbsolutePath());

  QString NewFilePath = WareExplorerDialog::getSaveFilePath(this,
                                                            TopDirectory.isEmpty() ? CurrentWarePath : TopDirectory,
                                                            CurrentFilePath);

  if (NewFilePath.isEmpty())
  {
    return "";
  }

  if (NewFilePath == CurrentFilePath)
  {
    saveCurrentEditor();
    return "";
  }

  CurrentEditor->saveContentToPath(NewFilePath);

  auto NewPathInfo = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(NewFilePath.toStdString());

  int CurrentIndex = closeFileTab(CurrentEditor);

  if (NewPathInfo.AbsoluteWarePath == CurrentWarePath.toStdString())
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
  const auto FilePath = openfluid::ui::common::createNewFile(this,
                                                             QString::fromStdString(m_Container.getAbsolutePath()));
  if (!FilePath.isEmpty())
  {
    openFileTab(openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(FilePath.toStdString()));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::deleteCurrentFile()
{
  if (WareFileEditor* Editor = currentEditor())
  {
    QString Path = Editor->getFilePath();

    if (QMessageBox::warning(this, tr("Delete file"), tr("Are you sure you want to delete \"%1\"?").arg(Path),
                             QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel)
        == QMessageBox::Cancel)
    {
      return;
    }

    // Careful as it deletes Editor
    closeFileTab(Editor);

    if (QDir().remove(Path))
    {
      m_Container.update();
    }
    else
    {
      QMessageBox::critical(0, "Error", tr("Unable to remove the file \"%1\"").arg(Path));
    }
  }
  else
  {
    QMessageBox::warning(0, tr("No open file"), tr("No file to delete"));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::openFile()
{
  QString PathToOpen = 
    openfluid::ui::waresdev::WareExplorerDialog::getOpenFilePath(this,
                                                                 QString::fromStdString(m_Container.getAbsolutePath()),
                                                                 getCurrentFilePath());

  if (PathToOpen.isEmpty())
  {
    return;
  }

  openFileTab(openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(PathToOpen.toStdString()));
}


// =====================================================================
// =====================================================================


void WareSrcWidget::onCurrentTabChanged(int Index)
{
  if (WareFileEditor* Editor = dynamic_cast<WareFileEditor*>(ui->WareSrcFileCollection->widget(Index)))
  {
    emit currentTabChanged(Editor->getFilePath());
  }

  checkModifiedStatus();
}


// =====================================================================
// =====================================================================


void WareSrcWidget::copyText()
{
  if (WareFileEditor* Editor = currentEditor())
  {
    Editor->copy();
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::cutText()
{
  if (WareFileEditor* Editor = currentEditor())
  {
    Editor->cut();
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::pasteText()
{
  if (WareFileEditor* Editor = currentEditor())
  {
    Editor->paste();
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::clearEditorsMessages()
{
  for (WareFileEditor* Editor : m_WareFilesByPath)
  {
    Editor->clearLineMessages();
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::onProcessFinished()
{
  for (WareSrcMsgParser::WareSrcMsg Msg : m_Container.getMessages())
  {
    if (WareFileEditor* Editor = m_WareFilesByPath.value(Msg.m_Path, 0))
    {
      Editor->addLineMessage(Msg);
    }
  }

  for (WareFileEditor* Editor : m_WareFilesByPath)
  {
    Editor->updateLineNumberArea();
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::onMessageClicked(WareSrcMsgParser::WareSrcMsg& Msg)
{
  auto Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(Msg.m_Path.toStdString());

  if (Info.AbsoluteWarePath != m_Container.getAbsolutePath())
  {
    return;
  }

  openFileTab(Info);

  if (WareFileEditor* Editor = m_WareFilesByPath.value(Msg.m_Path, 0))
  {
    Editor->selectLine(Msg.m_LineNb);
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::checkModifiedStatus()
{
  bool IsCurrentEditorModified = false;
  if (WareFileEditor* Editor = currentEditor())
  {
    IsCurrentEditorModified = Editor->isModified();
  }

  bool IsWareModified = isWareModified();

  bool IsFileOpen = currentEditor()!=0;

  if (mp_WareSrcToolBar)
  {
    mp_ActionsCollection->action("SaveFile")->setEnabled(IsCurrentEditorModified);
    mp_ActionsCollection->action("SaveAsFile")->setEnabled(IsFileOpen);
    mp_ActionsCollection->action("SaveAllFiles")->setEnabled(IsWareModified);
  }

  emit modifiedStatusChanged(IsCurrentEditorModified, IsFileOpen, IsWareModified);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::goToLine()
{
  if (WareFileEditor* Editor = currentEditor())
  {
    Editor->goToLine();
  }
  else
  {
    QMessageBox::warning(0, tr("No open file"), tr("No open editor"));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::updateEditorsSettings()
{
  for (WareFileEditor* Editor : m_WareFilesByPath.values())
  {
    Editor->updateSettings();
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidget::notifyConfigureLaunched(openfluid::ware::WareType Type, const QString& ID)
{
  emit configureLaunched(Type,ID);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::notifyConfigureFinished(openfluid::ware::WareType Type, const QString& ID)
{
  emit configureFinished(Type,ID);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::notifyBuildLaunched(openfluid::ware::WareType Type, const QString& ID)
{
  emit buildLaunched(Type,ID);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::notifyBuildFinished(openfluid::ware::WareType Type, const QString& ID)
{
  emit buildFinished(Type,ID);
}


} } } // namespaces
