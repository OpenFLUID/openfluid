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
 @file WareSrcWidgetCollection.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 @author Armel Thoni <armel.thoni@inra.fr>
 @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#include <QTabWidget>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QProcess>
#include <QMessageBox>
#include <QThread>

#include <openfluid/base/Environment.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/config.hpp>
#include <openfluid/utils/InternalLogger.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/waresdev/WareSrcFactory.hpp>
#include <openfluid/ui/waresdev/WareSrcWidgetCollection.hpp>
#include <openfluid/ui/waresdev/WareSrcWidget.hpp>
#include <openfluid/ui/waresdev/WareExplorerDialog.hpp>
#include <openfluid/ui/waresdev/NewWareDialog.hpp>
#include <openfluid/ui/waresdev/FindReplaceDialog.hpp>
#include <openfluid/ui/waresdev/MigrationSetupDialog.hpp>
#include <openfluid/ui/waresdev/WareSrcFiletypeManager.hpp>
#include <openfluid/ui/waresdev/WaresSrcIOProgressDialog.hpp>
#include <openfluid/ui/waresdev/WareFileEditor.hpp>
#include <openfluid/ui/waresdev/WareStatusItemWidget.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WareSrcWidgetCollection::WareSrcWidgetCollection(QTabWidget* TabWidget, bool IsStandalone) :
    mp_TabWidget(TabWidget), m_IsStandalone(IsStandalone), mp_Manager(openfluid::base::WorkspaceManager::instance()),
    mp_FindReplaceDialog(0)
{
  WareStatusItemWidget::populateReportItemLabels();
  
  connect(mp_TabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseWareTabRequested(int)));
  connect(mp_TabWidget, SIGNAL(currentChanged(int)), this, SLOT(onCurrentTabChanged(int)));
}


// =====================================================================
// =====================================================================


WareSrcWidgetCollection::~WareSrcWidgetCollection()
{
  delete mp_FindReplaceDialog;
}


// =====================================================================
// =====================================================================


bool WareSrcWidgetCollection::openPath(const QString& Path)
{
  if (Path.isEmpty())
  {
    return false;
  }

  auto Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(Path.toStdString());

  if (!Info.isValid())
  {
    return false;
  }

  // TODO manage other workspaces later
  if (!Info.IsInCurrentWorkspace && !Info.IsInExamples)
  {
    return false;
  }

  if (Info.IsWareDirectory || Info.IsWareFile)
  {

    openfluid::ui::waresdev::WareSrcWidget* Widget = 
      m_WareSrcWidgetByPath.value(QString::fromStdString(Info.AbsoluteWarePath), 0);

    if (!Widget)
    {
      Widget = new WareSrcWidget(Info, m_IsStandalone, 
                                 mp_TabWidget);

      mp_TabWidget->addTab(Widget,QString::fromStdString(Info.WareDirName));

      m_WareSrcWidgetByPath[QString::fromStdString(Info.AbsoluteWarePath)] = Widget;

      // leave before openDefaultFiles()
      connect(Widget, SIGNAL(currentTabChanged(const QString&)), this, SIGNAL(currentTabChanged(const QString&)));

      if (Info.IsWareDirectory)
      {
        Widget->openDefaultFiles();
      }

      connect(Widget, SIGNAL(wareTextModified(WareSrcWidget*,bool)), this,
              SLOT(onWareTxtModified(WareSrcWidget*,bool)));
      connect(Widget, SIGNAL(editorSaved()), this, SIGNAL(editorSaved()));
      connect(Widget, SIGNAL(findReplaceRequested()), this, SLOT(showFindReplaceDialog()));
      connect(Widget, SIGNAL(openTerminalRequested()), this, SLOT(openTerminalAtWarePath()));
      connect(Widget, SIGNAL(openExplorerRequested()), this, SLOT(openExplorerAtWarePath()));
      connect(Widget, SIGNAL(openExternalToolRequested(const QString&, const QString&)), 
              this, SLOT(openExternalTool(const QString&, const QString&)));
      
      connect(Widget, SIGNAL(openAPIDocRequested()), this, SLOT(openAPIDoc()));

      connect(Widget, SIGNAL(modifiedStatusChanged(bool, bool, bool)), 
              this, SIGNAL(modifiedStatusChanged(bool, bool, bool)));

      connect(Widget, SIGNAL(configureLaunched(openfluid::ware::WareType, const QString&)),
              this, SLOT(notifyConfigureLaunched(openfluid::ware::WareType, const QString&)));

      connect(Widget, SIGNAL(configureFinished(openfluid::ware::WareType, const QString&)),
              this, SLOT(notifyConfigureFinished(openfluid::ware::WareType, const QString&)));

      connect(Widget, SIGNAL(buildLaunched(openfluid::ware::WareType, const QString&)),
              this, SLOT(notifyBuildLaunched(openfluid::ware::WareType, const QString&)));

      connect(Widget, SIGNAL(buildFinished(openfluid::ware::WareType, const QString&)),
              this, SLOT(notifyBuildFinished(openfluid::ware::WareType, const QString&)));

      connect(Widget, SIGNAL(operationRequestedOnWare(const QString&, const QString&)),
              this, SLOT(onOperationRequestedOnWare(const QString&, const QString&)));
    }


    if (Info.IsWareFile) 
    {
      Widget->openFileTab(Info);
    }
    mp_TabWidget->setCurrentWidget(Widget);
  }
  return true;

}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openWarePath(const std::string& WarePath, bool EditSignature)
{
  const auto QWarePath = QString::fromStdString(WarePath);
  openPath(QWarePath);
  // check if open signature dialog requested
  if (EditSignature)
  {
    openfluid::ui::waresdev::WareSrcWidget* Widget = 
      m_WareSrcWidgetByPath.value(QWarePath, 0);
    Widget->editSignature();
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::onCloseWareTabRequested(int Index)
{
  if (WareSrcWidget* Ware = qobject_cast<WareSrcWidget*>(mp_TabWidget->widget(Index)))
  {
    requestWareTabClosing(Ware);
  }
}


// =====================================================================
// =====================================================================


bool WareSrcWidgetCollection::requestWareTabClosing(WareSrcWidget* Ware)
{
  // check if a process is not running
  if (Ware->isWareProcessRunning())
  {
    QMessageBox::warning(Ware,
                         tr("Process running"),
                         tr("Closing tab is not allowed while a configure or build process is running."),
                         QMessageBox::Close,QMessageBox::Close);
    return false;
  }


  int Choice = QMessageBox::Discard;

  if (Ware->isWareModified())
  {
    Choice = QMessageBox::information(Ware, 
                                      tr("Modified documents"),
                                      tr("Documents have been modified.\n\n"
                                          "Do you want to save changes?"),
                                      QMessageBox::SaveAll | QMessageBox::Discard | QMessageBox::Cancel,
                                      QMessageBox::SaveAll);
  }

  bool IsClosed = false;
  switch (Choice)
  {
    case QMessageBox::SaveAll:
      Ware->saveAllFileTabs();
      /* fall through */
    case QMessageBox::Discard:
      Ware->closeAllFileTabs();
      closeWareTab(Ware);
      IsClosed = true;
      break;
    case QMessageBox::Cancel:
      /* fall through */
    default:
      break;
  }
  return IsClosed;
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::onOperationRequestedOnWare(const QString& OperationCode, const QString& WarePath)
{
  QMap<QString, WareSrcWidget*>::iterator it = m_WareSrcWidgetByPath.find(WarePath);
  if (OperationCode == "migration")
  {
    if (it == m_WareSrcWidgetByPath.end())
    {
      QMessageBox::warning(nullptr,
                          tr("Migration failure"),
                          tr("Ware requested not found"),
                          QMessageBox::Close,QMessageBox::Close);
      return;
    }

    MigrationSetupDialog MigrationSetup;
    bool Proceed = true;
    if (!MigrationSetup.exec())
    {
      Proceed = false;
    }

    // try to close ware tab before migration
    if (Proceed && !requestWareTabClosing(it.value()))
    {
      Proceed = false;
    }

    if (!Proceed)
    {
      QMessageBox::warning(it.value(),
                          tr("Migration failure"),
                          tr("Migration cancelled by user."),
                          QMessageBox::Close,QMessageBox::Close);
      return;
    }
    
    
    openfluid::ui::waresdev::WaresSrcIOProgressDialog ProgressDialog(tr("Ware migration"), false, mp_TabWidget);
    
    MigrationWorker* MWorker = new MigrationWorker(WarePath, 
                                                  MigrationSetup.isVerbose(), 
                                                  MigrationSetup.isNewBranchChecked());

    try
    {

      QThread* Thread = new QThread();

      MWorker->moveToThread(Thread);

      connect(Thread, SIGNAL(started()), MWorker, SLOT(run()));
      connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));
      connect(MWorker, SIGNAL(finished(bool, const QString&)), MWorker, SLOT(deleteLater()));
      connect(MWorker, SIGNAL(finished(bool, const QString&)), Thread, SLOT(quit()));
      
      connect(MWorker, SIGNAL(finished(bool, const QString&)), &ProgressDialog,
              SLOT(finish(bool, const QString&)));
      connect(MWorker, SIGNAL(info(const QString&)), &ProgressDialog,
              SLOT(writeInfo(const QString&)));
      connect(MWorker, SIGNAL(warning(const QString&)), &ProgressDialog,
              SLOT(writeWarning(const QString&)));
      connect(MWorker, SIGNAL(error(const QString&)), &ProgressDialog,
              SLOT(writeError(const QString&)));
      connect(MWorker, SIGNAL(progressed(const int)), &ProgressDialog,
              SLOT(progress(const int)));

      Thread->start();

    }
    catch (std::exception& e)
    {
      ProgressDialog.writeError(e.what());
    }

    ProgressDialog.exec();

    // reopen ware
    openPath(WarePath);
  }

  else if(OperationCode == "revert-migration")
  {
    if (it == m_WareSrcWidgetByPath.end())
    {
      QMessageBox::warning(nullptr,
                          tr("Revert migration failure"),
                          tr("Ware requested not found"),
                          QMessageBox::Close,QMessageBox::Close);
      return;
    }

    if (QMessageBox::question(nullptr, tr("Revert migration"),
                            tr("Reverting the migration will result in the loss of all post-migration "
                            "changes and will reset the ware to the pre-migration state.") + "\n\n" +
                            tr("Proceed anyway?"),
                            QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
    {
      return;
    }

    if(!requestWareTabClosing(it.value()))
    {
      return;
    }

    openfluid::tools::Path MigrationFolderPath = openfluid::tools::Path({WarePath.toStdString(),
                                                 openfluid::config::WARESDEV_MIGRATION_WORK_DIR});

    openfluid::tools::Path OriginalFolderPath = openfluid::tools::Path({WarePath.toStdString(), 
                                                openfluid::config::WARESDEV_MIGRATION_ORIGINAL_DIR});

    openfluid::tools::Path HiddenPaths = openfluid::tools::Path({WarePath.toStdString(),  ".*"});

    openfluid::tools::Path UnderscorePaths = openfluid::tools::Path({WarePath.toStdString(), "_*"});

    if(OriginalFolderPath.exists() && MigrationFolderPath.exists())   
    {  
      openfluid::tools::Path BranchInfoFile = openfluid::tools::Path({MigrationFolderPath.toGeneric(),
                                                                      openfluid::config::WARESDEV_BRANCH_INFO_FILE});
      if(BranchInfoFile.exists())
      {
        std::string GitBranchNameFromFile = openfluid::tools::Filesystem::readFile(BranchInfoFile);
        GitUIProxy Git;
        try
        {
          std::string CurrentBranchName = GitUIProxy::getCurrentBranchName(WarePath.toStdString());
          if(CurrentBranchName != GitBranchNameFromFile)
          {
            if(Git.checkout(WarePath, QString::fromStdString(GitBranchNameFromFile), false))
            {
              QProcess Process; 
              Process.setWorkingDirectory(WarePath);
              Process.start("git", {"branch", "-d", GitUIProxy::getCurrentOpenFLUIDBranchName()});
              Process.waitForFinished(-1);
              if(Process.exitCode() != 0)
              {
                QString gitBranchErrorMsg = tr("Error while deleting previous git branch");
                QMessageBox::warning(nullptr, tr("Revert migration failure"), 
                                  gitBranchErrorMsg, QMessageBox::Close);
                openfluid::utils::log::error("Migration:revert", gitBranchErrorMsg.toStdString());
                openfluid::utils::log::debug("Migration:revert", Process.readAllStandardOutput().toStdString());
                openfluid::utils::log::debug("Migration:revert", Process.readAllStandardError().toStdString());
                return;
              }
            }
            else
            {
              QString gitCheckoutErrorMsg = tr("Not able to checkout previous git branch");
              QMessageBox::warning(nullptr, tr("Revert migration failure"), 
                                  gitCheckoutErrorMsg, QMessageBox::Close);
              openfluid::utils::log::error("Migration:revert", gitCheckoutErrorMsg.toStdString());
              return;
            }
          }
        }
        catch(const std::exception& E)
        {
          QMessageBox::warning(nullptr, tr("Revert migration failure"), 
                                  tr(E.what()), QMessageBox::Close);
          openfluid::utils::log::error("Migration:revert", E.what());
          return;
        }
      }

      openfluid::tools::Filesystem::emptyDirectory(WarePath.toStdString(), {OriginalFolderPath.toGeneric(), 
                                                                            HiddenPaths.toGeneric(),
                                                                            UnderscorePaths.toGeneric()
                                                                          }); 

      openfluid::tools::Filesystem::copyDirectoryContent(std::filesystem::path(OriginalFolderPath.toGeneric()),
                                                        std::filesystem::path(WarePath.toStdString()));

      OriginalFolderPath.removeDirectory();
      MigrationFolderPath.removeDirectory();
      openPath(WarePath);    
    }   
    else 
    {
      QMessageBox::warning(nullptr, tr("Revert migration failure"), tr("No migration folders available."),
                           QMessageBox::Close);
      return;
    }                                                           
  }

  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Custom action not recognized");
  }

}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::onCurrentTabChanged(int Index)
{
  if (WareSrcWidget* Ware = qobject_cast<WareSrcWidget*>(mp_TabWidget->widget(Index)))
  {
    QString FilePath = Ware->getCurrentFilePath();

    emit currentTabChanged(FilePath.isEmpty() ? 
                           QString::fromStdString(Ware->wareSrcContainer().getAbsolutePath()) : FilePath);
  }

  checkModifiedStatus();
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::closeWareTab(WareSrcWidget* Ware)
{
  mp_TabWidget->removeTab(mp_TabWidget->indexOf(Ware));

  m_WareSrcWidgetByPath.remove(QString::fromStdString(Ware->wareSrcContainer().getAbsolutePath()));

  delete Ware;
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::setCurrent(const QString& Path)
{
  if (Path.isEmpty())
  {
    return;
  }

  auto Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(Path.toStdString());

// TODO manage other workspaces later
  if (!Info.IsInCurrentWorkspace)
  {
    return;
  }

  if (Info.IsWareDirectory || Info.IsWareFile)
  {
    auto Widget = m_WareSrcWidgetByPath.value(QString::fromStdString(Info.AbsoluteWarePath), 0);

    if (Widget)
    {
      if (Info.IsWareDirectory || Widget->setCurrent(Info))
      {
        mp_TabWidget->setCurrentWidget(Widget);
      }
    }
  }

}


// =====================================================================
// =====================================================================


QString WareSrcWidgetCollection::getContextualPath(const QString& Path)
{
  QString FileToOpen;

  if (!Path.isEmpty())
  {
    FileToOpen = Path;
  }
  else
  {
    FileToOpen = QString::fromStdString(mp_Manager->getWaresPath());
  }
  return FileToOpen;
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openExplorerAtWarePath()
{
  openExplorer(getCurrentWarePath());
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openExplorer(const QString& Path)
{
  QString FileToOpen = getContextualPath(Path);

  QDesktopServices::openUrl(QUrl::fromLocalFile(FileToOpen));
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openTerminalAtWarePath()
{
  openTerminal(getCurrentWarePath());
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openTerminal(const QString& Path)
{
  QString FileToOpen = getContextualPath(Path);

  bool TermFound = true;

// TODO test on Mac and not Debian-based distros
#if defined(OPENFLUID_OS_UNIX)
  if (!QProcess::startDetached("x-terminal-emulator", QStringList(), FileToOpen))
  {
    TermFound = QProcess::startDetached("xterm", QStringList(), FileToOpen);
  }
#elif defined(OPENFLUID_OS_WINDOWS)
  TermFound = QProcess::startDetached("cmd.exe", QStringList() << "/c" << "start" << "cmd.exe", FileToOpen);
#else
  TermFound = false;
#endif

  if (!TermFound)
  {
    QMessageBox::warning(0, tr("Error"), tr("No terminal found"));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openExternalTool(const QString& Command, const QString& Path)
{
  QString AdjustedCommand = Command;
  std::map<QString, QString> ContextPaths;

  ContextPaths.insert({"%%S%%", getCurrentWarePath()});
  ContextPaths.insert({"%%W%%", QString::fromStdString(mp_Manager->getWaresPath())});

  QString PathToOpen;
  if (!Path.isEmpty())
  {
    ContextPaths.insert({"%%C%%", Path});
    PathToOpen = Path;
  }
  else
  {
    QString Current = getCurrentWarePath();

    if (!Current.isEmpty())
    {
      PathToOpen = Current;
    }
    else
    {
      PathToOpen = QString::fromStdString(mp_Manager->getWaresPath());
    }
  }

  bool ChangeDirectory = false;
  if (QFileInfo(PathToOpen).isDir())
  {
    ChangeDirectory = true;
  }

  QString Program;
  QStringList SplittedCommand;

  QStringList Args = QStringList();

  if (AdjustedCommand.count("\"") >= 2)
  {
#if (QT_VERSION_MAJOR < 6)
    QRegExp Rx("^\"(.*)\"(.*)");
    if (Rx.indexIn(AdjustedCommand) != -1)
    {
      QStringList CommandParts = Rx.capturedTexts();
      Program = CommandParts[1];
      SplittedCommand = CommandParts[2].split(" ");
    }
#else
    QRegularExpression Rx("^\"(.*)\"(.*)");
    auto Match = Rx.match(AdjustedCommand);
    if (Match.hasMatch())
    {
      Program = Match.captured(1);
      SplittedCommand = Match.captured(2).split(" ");
    }
    else
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Error in tool syntax");
    }
#endif
  }
  else
  {
    SplittedCommand = AdjustedCommand.split(" ");
  }

  for (int Pos=0 ; Pos<SplittedCommand.size() ; Pos++)
  {
    QString Word = SplittedCommand[Pos];
    if (Pos == 0 && Program.isEmpty())
    {
      Program = Word;
    }
    else if (Word.size() > 0)
    {
      if (ContextPaths.find(Word) != ContextPaths.end())
      {
        Args << ContextPaths[Word];
      }
      else
      {
        Args << Word;
      }
    }
  }

  if (ChangeDirectory)
  {
    QProcess::startDetached(Program, Args, PathToOpen);
  }
  else
  {
    QProcess::startDetached(Program, Args);
  }
}


// =====================================================================
// =====================================================================


QString WareSrcWidgetCollection::getCurrentWarePath()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    return QString::fromStdString(CurrentWare->wareSrcContainer().getAbsolutePath());
  }

  return "";
}


// =====================================================================
// =====================================================================


WareSrcWidget* WareSrcWidgetCollection::currentWareWidget()
{
  if (WareSrcWidget* Widget = qobject_cast<WareSrcWidget*>(mp_TabWidget->currentWidget()))
  {
    return Widget;
  }

  return nullptr;
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::setConfigureMode(openfluid::waresdev::WareSrcContainer::ConfigMode Mode)
{
  m_DefaultConfigMode = Mode;

  for(WareSrcWidget* Ware : m_WareSrcWidgetByPath.values())
  {
    Ware->setConfigureMode(Mode);
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::setBuildMode(openfluid::waresdev::WareSrcContainer::BuildMode Mode)
{
  m_DefaultBuildMode = Mode;

  for(WareSrcWidget* Ware : m_WareSrcWidgetByPath.values())
  {
    Ware->setBuildMode(Mode);
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::setBuildJobs(unsigned int Jobs)
{
  for(WareSrcWidget* Ware : m_WareSrcWidgetByPath.values())
  {
    Ware->setBuildJobs(Jobs);
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::configure()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    if (openfluid::base::PreferencesManager::instance()->isWaresdevAutomaticSaveBeforeBuild())
    {
      CurrentWare->saveAllFileTabs();
    }

    CurrentWare->configure();
  }
  else
  {
    QMessageBox::warning(nullptr,tr("Configure"),tr("At least one ware must be open to perform this action"));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::build()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    if (openfluid::base::PreferencesManager::instance()->isWaresdevAutomaticSaveBeforeBuild())
    {
      CurrentWare->saveAllFileTabs();
    }

    CurrentWare->build();
  }
  else
  {
    QMessageBox::warning(nullptr, tr("Build"), tr("At least one ware must be open to perform this action"));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::generateDoc()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    if (openfluid::base::PreferencesManager::instance()->isWaresdevAutomaticSaveBeforeBuild())
    {
      CurrentWare->saveAllFileTabs();
    }

    CurrentWare->generateDoc();
  }
  else
  {
    QMessageBox::warning(nullptr,tr("Generate documentation"),
                         tr("At least one ware must be open to perform this action"));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::onWareTxtModified(WareSrcWidget* Widget, bool Modified)
{
  int Pos = mp_TabWidget->indexOf(Widget);
  if (Pos > -1)
  {
    QString Label = mp_TabWidget->tabText(Pos);
    bool ModifiedState = Label.startsWith("*");

    if (Modified && !ModifiedState)
    {
      mp_TabWidget->setTabText(Pos, Label.prepend("*"));
    }
    else if (!Modified && ModifiedState)
    {
      mp_TabWidget->setTabText(Pos, Label.remove(0, 1));
    }
  }
}


// =====================================================================
// =====================================================================


bool WareSrcWidgetCollection::isModified() const
{
  for(WareSrcWidget* Ware : m_WareSrcWidgetByPath.values())
  {
    if(Ware->isWareModified())
    {
      return true;
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


bool WareSrcWidgetCollection::isProcessRunning() const
{
  for(WareSrcWidget* Ware : m_WareSrcWidgetByPath.values())
  {
    if(Ware->isWareProcessRunning())
    {
      return true;
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::saveCurrentEditor()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    CurrentWare->saveCurrentEditor();
  }
  else
  {
    QMessageBox::warning(0, tr("Save"), tr("At least one ware must be open to perform this action"));
  }
}


// =====================================================================
// =====================================================================


QString WareSrcWidgetCollection::saveAs(const QString& TopDirectory)
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    return CurrentWare->saveAs(TopDirectory);
  }

  QMessageBox::warning(0, tr("Save"), tr("At least one ware must be open to perform this action"));
  return "";
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::saveAllCurrent()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    CurrentWare->saveAllFileTabs();
  }
  else
  {
    QMessageBox::warning(0, tr("Save"), tr("At least one ware must be open to perform this action"));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::closeCurrentEditor()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    CurrentWare->closeCurrentEditor();
  }
  else
  {
    QMessageBox::warning(0, tr("Close"), tr("At least one ware must be open to perform this action"));
  }
}


// =====================================================================
// =====================================================================


bool WareSrcWidgetCollection::closeAllWidgets()
{
  if (isProcessRunning())
  {
    QMessageBox::warning(nullptr,tr("Process running"),
                         tr("Closing tab is not allowed while a configure or build process is running."),
                         QMessageBox::Close,QMessageBox::Close);
    return false;
  }


  int Choice = QMessageBox::Discard;

  if (isModified())
  {
    QMessageBox MsgBox;
    MsgBox.setText(tr("Documents have been modified."));
    MsgBox.setInformativeText(tr("Do you want to save changes?"));
    MsgBox.setStandardButtons(QMessageBox::SaveAll | QMessageBox::Discard | QMessageBox::Cancel);
    MsgBox.setDefaultButton(QMessageBox::SaveAll);
    Choice = MsgBox.exec();
  }

  switch (Choice)
  {
    case QMessageBox::SaveAll:
      for(WareSrcWidget* Ware : m_WareSrcWidgetByPath.values())
      {
        Ware->saveAllFileTabs();
      }
      /* fall through */
    case QMessageBox::Discard:
      for(WareSrcWidget* Ware : m_WareSrcWidgetByPath.values())
      {
        Ware->closeAllFileTabs();
        closeWareTab(Ware);
      }
      return true;
      break;
    case QMessageBox::Cancel:
    default:
      return false;
      break;
  }
}


// =====================================================================
// =====================================================================


QStringList WareSrcWidgetCollection::getOpenWarePaths()
{
  QStringList OpenPaths;
  for (int Index=0; Index < mp_TabWidget->count(); Index++)
  {
    if (WareSrcWidget* Ware = qobject_cast<WareSrcWidget*>(mp_TabWidget->widget(Index)))
    {
      OpenPaths << QString::fromStdString(Ware->wareSrcContainer().getAbsolutePath());
    }
  }
  return OpenPaths;
}


// =====================================================================
// =====================================================================


bool WareSrcWidgetCollection::isFileOpen()
{
  auto* Widget = currentWareWidget();

  return (Widget != nullptr) && (Widget->currentEditor() != nullptr);
}


// =====================================================================
// =====================================================================


bool WareSrcWidgetCollection::isDebugMode()
{
  return m_DefaultConfigMode == openfluid::waresdev::WareSrcContainer::ConfigMode::CONFIG_DEBUG;
}


// =====================================================================
// =====================================================================


bool WareSrcWidgetCollection::isBuildNoInstallMode()
{
  return m_DefaultBuildMode == openfluid::waresdev::WareSrcContainer::BuildMode::BUILD_NOINSTALL;
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openSimulator()
{
  openWare(openfluid::ware::WareType::SIMULATOR, tr("Open a simulator"));
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openObserver()
{
  openWare(openfluid::ware::WareType::OBSERVER, tr("Open an observer"));
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openBuilderExtension()
{
  openWare(openfluid::ware::WareType::BUILDEREXT, tr("Open a Builder extension"));
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openFile()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    CurrentWare->openFile();
  }
  else
  {
    QMessageBox::warning(0, tr("Open file"), tr("At least one ware must be open to perform this action"));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::deleteCurrentFile()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    CurrentWare->deleteCurrentFile();
  }
  else
  {
    QMessageBox::warning(0, tr("Delete file"), tr("At least one ware must be open to perform this action"));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openWare(openfluid::ware::WareType Type, const QString& Title)
{
  QString PathToOpen = openfluid::ui::waresdev::WareExplorerDialog::getOpenWarePath(
    QApplication::activeWindow(),
    QString::fromStdString(mp_Manager->getWaresPath(Type)),
    Title
  );

  if (!PathToOpen.isEmpty())
  {
    openPath(PathToOpen);
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::deleteWare(const QString& WarePath)
{
  if (QMessageBox::warning(QApplication::activeWindow(), tr("Delete ware"),
                           tr("Are you sure you want to delete \"%1\" and all its content?").arg(WarePath),
                           QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel)
      == QMessageBox::Cancel)
  {
    return;
  }

  QMap<QString, WareSrcWidget*>::iterator it = m_WareSrcWidgetByPath.find(WarePath);
  if (it != m_WareSrcWidgetByPath.end())
  {
    closeWareTab(it.value());
  }

  if (!openfluid::tools::FilesystemPath(WarePath.toStdString()).removeDirectory())
  {
    QMessageBox::critical(0, tr("Delete ware"), tr("Unable to remove the directory \"%1\"").arg(WarePath));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::newFile()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    CurrentWare->newFile();
  }
  else
  {
    QMessageBox::warning(0, tr("New file"), tr("At least one ware must be open to perform this action"));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::newSimulator()
{
  newWare(openfluid::ware::WareType::SIMULATOR);
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::newSimulatorFromGhost(const openfluid::ware::SimulatorSignature& Signature)
{
  openfluid::ui::waresdev::NewWareDialog Dialog(Signature, QApplication::activeWindow());
  if (Dialog.exec())
  {
    openfluid::waresdev::WareSrcFactory::Configuration Config;
    Config.MainClassName = Dialog.getClassName();
    Config.WithParamsUI = Dialog.isWareUI();

    auto WarePath =
      openfluid::waresdev::WareSrcFactory::createSimulator(
        Signature,Config,
        openfluid::base::WorkspaceManager::instance()->getWaresPath(openfluid::ware::WareType::SIMULATOR)
      );

    if (!WarePath.empty())
    {
      openWarePath(WarePath, Dialog.openSignature());
    }
    else
    {
      QMessageBox::critical(nullptr, tr("Create simulator"), tr("Error creating simulator %1 from ghost")
                                                             .arg(QString::fromStdString(Signature.ID)));
    }
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::newObserver()
{
  newWare(openfluid::ware::WareType::OBSERVER);
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::newBuilderExtension()
{
  newWare(openfluid::ware::WareType::BUILDEREXT);
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::newWare(openfluid::ware::WareType Type)
{
  auto errorMessage = [](openfluid::ware::WareType Type, const openfluid::ware::WareID_t ID) {
    // TODO merge with method in FLuidHubAPIClient into a generic function to manage ware types as translatable strings
    QString TypeStr = "ware";
    if (Type == openfluid::ware::WareType::SIMULATOR)
    {
      TypeStr = tr("simulator");
    }
    else if (Type == openfluid::ware::WareType::OBSERVER)
    {
      TypeStr = tr("observer");
    }
    else if (Type == openfluid::ware::WareType::BUILDEREXT)
    {
      TypeStr = tr("builder-extension");
    }

    QMessageBox::critical(nullptr, tr("Create %1").arg(TypeStr), tr("Error creating %1 %2")
                                                                 .arg(TypeStr).arg(QString::fromStdString(ID)));
  };

  openfluid::ui::waresdev::NewWareDialog Dialog(Type, QApplication::activeWindow());

  if (Dialog.exec())
  {

    if (Type == openfluid::ware::WareType::SIMULATOR)
    {
      openfluid::ware::SimulatorSignature Sign;
      Sign.ID = Dialog.getID();
      Sign.TimeScheduling.setAsDefaultDeltaT();

      openfluid::waresdev::WareSrcFactory::Configuration Config;
      Config.MainClassName = Dialog.getClassName();
      Config.WithParamsUI = Dialog.isWareUI();

      auto WarePath =
        openfluid::waresdev::WareSrcFactory::createSimulator(
          Sign,Config,openfluid::base::WorkspaceManager::instance()->getWaresPath(Type)
        );

      if (!WarePath.empty())
      {
        openWarePath(WarePath, Dialog.openSignature());
      }
      else
      {
        errorMessage(Type,Sign.ID);
      }
    }
    else if (Type == openfluid::ware::WareType::OBSERVER)
    {
      openfluid::ware::ObserverSignature Sign;
      Sign.ID = Dialog.getID();

      openfluid::waresdev::WareSrcFactory::Configuration Config;
      Config.MainClassName = Dialog.getClassName();
      Config.WithParamsUI = Dialog.isWareUI();

      auto WarePath = 
        openfluid::waresdev::WareSrcFactory::createObserver(
          Sign,Config,openfluid::base::WorkspaceManager::instance()->getWaresPath(Type)
        );

      if (!WarePath.empty())
      {
        openWarePath(WarePath, Dialog.openSignature());
      }
      else
      {
        errorMessage(Type,Sign.ID);
      }
    }
    else if (Type == openfluid::ware::WareType::BUILDEREXT)
    {
      openfluid::builderext::BuilderExtensionSignature Sign;
      Sign.ID = Dialog.getID();
      Sign.MenuText = Dialog.getBuilderextMenuText().toStdString();
      Sign.Category = Dialog.getBuilderextCategory();
      Sign.Mode = Dialog.getBuilderextMode();

      openfluid::waresdev::WareSrcFactory::Configuration Config;
      Config.MainClassName = Dialog.getClassName();

      auto WarePath = 
        openfluid::waresdev::WareSrcFactory::createBuilderext(
          Sign,Config,openfluid::base::WorkspaceManager::instance()->getWaresPath(Type)
        );

      if (!WarePath.empty())
      {
        openWarePath(WarePath, Dialog.openSignature());
      }
      else
      {
        errorMessage(Type,Sign.ID);
      }
    }
    else
    {
      QMessageBox::critical(0, tr("Create ware"), tr("Internal error during process of ware creation"));
    }
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::showFindReplaceDialog()
{
  if (!mp_FindReplaceDialog)
  {
    mp_FindReplaceDialog = new FindReplaceDialog(mp_TabWidget);

    connect(mp_FindReplaceDialog, SIGNAL(findReplaceRequested( FindReplaceDialog::FindReplaceAction,
                                                               const QString&, const QString&,
                                                               QTextDocument::FindFlags)),
            this,SLOT(onFindReplaceRequested( FindReplaceDialog::FindReplaceAction,
                                              const QString&, const QString&,
                                              QTextDocument::FindFlags)));
  }

  QString SelectedText = "";
  if (WareSrcWidget* Ware = currentWareWidget())
  {
    if (WareFileEditor* Editor = Ware->currentEditor())
    {
      SelectedText = Editor->getSelectedText();
    }
  }

  mp_FindReplaceDialog->show(SelectedText);
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::editSignatureRequested(const QString& Path)
{
  if (WareSrcWidget* Ware = currentWareWidget())
  {
    Ware->editSignature(Path);
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::onFindReplaceRequested(FindReplaceDialog::FindReplaceAction Action,
                                                     const QString& StringToFind, const QString& StringForReplace,
                                                     QTextDocument::FindFlags Options)
{
  if (WareSrcWidget* Ware = currentWareWidget())
  {
    if (WareFileEditor* Editor = Ware->currentEditor())
    {
      Editor->setFocus();
      QString Message;
      bool TextFound = Editor->findReplace(Action, StringToFind, StringForReplace, Options, Message);
      mp_FindReplaceDialog->setMessage(Message, TextFound);
    }
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::copyText()
{
  if (WareSrcWidget* Ware = currentWareWidget())
  {
    Ware->copyText();
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::cutText()
{
  if (WareSrcWidget* Ware = currentWareWidget())
  {
    Ware->cutText();
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::pasteText()
{
  if (WareSrcWidget* Ware = currentWareWidget())
  {
    Ware->pasteText();
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::checkModifiedStatus()
{
  if (WareSrcWidget* Ware = currentWareWidget())
  {
    Ware->checkModifiedStatus();
  }
  else
  {
    emit modifiedStatusChanged(false, false, false);
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openAPIDoc()
{
  QDesktopServices::openUrl(QUrl(QString::fromStdString(openfluid::config::URL_DOC), QUrl::TolerantMode));
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::goToLine()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    CurrentWare->goToLine();
  }
  else
  {
    QMessageBox::warning(0, tr("Go to line"), tr("At least one ware must be open to perform this action"));
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::closeEditor(const QString& FilePath)
{
  QString WarePath(
    openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(FilePath.toStdString()).AbsoluteWarePath.c_str()
  );

  QMap<QString, WareSrcWidget*>::iterator it = m_WareSrcWidgetByPath.find(WarePath);
  if (it != m_WareSrcWidgetByPath.end())
  {
    it.value()->closeFileTab(FilePath);
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::closeEditorsInFolderFromWare(const QString& WarePath, const QString& FolderPath,
                                                           const bool Confirm)
{
  // Can not deduce safely ware path when not based on an explicit file (true here but not in waresrcexplorer)
  // FIXME investigate why context not deducted here

  QMap<QString, WareSrcWidget*>::iterator it = m_WareSrcWidgetByPath.find(WarePath);
  if (it != m_WareSrcWidgetByPath.end())
  {
    // closes all files with a given dir path: iterate through all tabs of a given waresrcwidget
    it.value()->closeFileTabsInFolder(FolderPath, Confirm);
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::updateEditorsSettings()
{
  openfluid::ui::waresdev::WareSrcFiletypeManager::instance()->updateStyles();
  for (WareSrcWidget* Ware : m_WareSrcWidgetByPath.values())
  {
    Ware->updateEditorsSettings();
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::onWareChange(const QString& /*WarePath*/) //FIXME: why WarePath ignored here?
{
  openfluid::ui::waresdev::WareSrcFiletypeManager::instance()->updateStyles();
  for (WareSrcWidget* Ware : m_WareSrcWidgetByPath.values())
  {
    Ware->onWareChange();  // can not be set as signal/slot connexion because of asymetrical relationship
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::notifyConfigureLaunched(openfluid::ware::WareType Type, const QString& ID)
{
  emit configureLaunched(Type,ID);
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::notifyConfigureFinished(openfluid::ware::WareType Type, const QString& ID)
{
  emit configureFinished(Type,ID);
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::notifyBuildLaunched(openfluid::ware::WareType Type, const QString& ID)
{
  emit buildLaunched(Type,ID);
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::notifyBuildFinished(openfluid::ware::WareType Type, const QString& ID)
{
  emit buildFinished(Type,ID);
}


} } } //namespaces
