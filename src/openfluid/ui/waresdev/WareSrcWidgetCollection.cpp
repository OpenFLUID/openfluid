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
 \file WareSrcWidgetCollection.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/ui/waresdev/WareSrcWidgetCollection.hpp>

#include <QTabWidget>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QProcess>
#include <QMessageBox>

#include <openfluid/base/PreferencesManager.hpp>

#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/ui/waresdev/WareSrcWidget.hpp>
#include <openfluid/ui/waresdev/RestrictedFileDialog.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcWidgetCollection::WareSrcWidgetCollection(QTabWidget* TabWidget,
                                                 bool IsStandalone) :
    mp_TabWidget(TabWidget), m_IsStandalone(IsStandalone), mp_Manager(
        openfluid::waresdev::WareSrcManager::instance()), m_DefaultConfigMode(
        openfluid::waresdev::WareSrcContainer::CONFIG_RELEASE), m_DefaultBuildMode(
        openfluid::waresdev::WareSrcContainer::BUILD_WITHINSTALL)
{
  connect(mp_TabWidget, SIGNAL(tabCloseRequested(int)), this,
          SLOT(onCloseWareTabRequested(int)));
}


// =====================================================================
// =====================================================================


WareSrcWidgetCollection::~WareSrcWidgetCollection()
{

}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openPath(const QString& Path)
{
  openfluid::waresdev::WareSrcManager::PathInfo Info = mp_Manager->getPathInfo(
      Path);

// TODO manage other workspaces later
  if (!Info.m_IsInCurrentWorkspace)
    return;

  if (Info.m_isAWare || Info.m_isAWareFile)
  {
    openfluid::ui::waresdev::WareSrcWidget* Widget =
        m_WareSrcWidgetByPath.value(Info.m_AbsolutePathOfWare, 0);

    if (!Widget)
    {
      Widget = new WareSrcWidget(Info, m_IsStandalone, m_DefaultConfigMode,
                                 m_DefaultBuildMode, mp_TabWidget);

      mp_TabWidget->addTab(Widget, Info.m_WareName);

      m_WareSrcWidgetByPath[Info.m_AbsolutePathOfWare] = Widget;

      if (Info.m_isAWare)
        Widget->openDefaultFiles();

      connect(Widget, SIGNAL(wareTextModified(WareSrcWidget*,bool)), this,
              SLOT(onWareTxtModified(WareSrcWidget*,bool)));
      connect(Widget, SIGNAL(saveAsRequested()), this,
              SLOT(saveCurrentEditorAs()));
    }

    if (Info.m_isAWareFile)
      Widget->openFile(Info);

    mp_TabWidget->setCurrentWidget(Widget);
  }

}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::onCloseWareTabRequested(int Index)
{
  if (WareSrcWidget* Ware = qobject_cast<WareSrcWidget*>(
      mp_TabWidget->widget(Index)))
  {
    int Choice = QMessageBox::Discard;

    if (Ware->isModified())
    {
      QMessageBox MsgBox;
      MsgBox.setText(tr("Documents have been modified."));
      MsgBox.setInformativeText(tr("Do you want to save your changes?"));
      MsgBox.setStandardButtons(
          QMessageBox::SaveAll | QMessageBox::Discard | QMessageBox::Cancel);
      MsgBox.setDefaultButton(QMessageBox::SaveAll);
      Choice = MsgBox.exec();
    }

    switch (Choice)
    {
      case QMessageBox::SaveAll:
        Ware->saveAllFileTabs();
      case QMessageBox::Discard:
        Ware->closeAllFileTabs();
        closeWareTab(Ware);
        break;
      case QMessageBox::Cancel:
      default:
        break;
    }

  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::closeWareTab(WareSrcWidget* Ware)
{
  mp_TabWidget->removeTab(mp_TabWidget->indexOf(Ware));

  m_WareSrcWidgetByPath.remove(Ware->wareSrcContainer().getAbsolutePath());

  delete Ware;
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::setCurrent(const QString& Path)
{
  openfluid::waresdev::WareSrcManager::PathInfo Info = mp_Manager->getPathInfo(
      Path);

// TODO manage other workspaces later
  if (!Info.m_IsInCurrentWorkspace)
    return;

  if (Info.m_isAWare || Info.m_isAWareFile)
  {
    openfluid::ui::waresdev::WareSrcWidget* Widget =
        m_WareSrcWidgetByPath.value(Info.m_AbsolutePathOfWare, 0);

    if (Widget)
    {
      if (Info.m_isAWare || Widget->setCurrent(Info))
        mp_TabWidget->setCurrentWidget(Widget);
    }
  }

}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openExplorer(const QString& Path)
{
  QString FileToOpen;

  if (!Path.isEmpty())
    FileToOpen = Path;
  else
  {
    QString Current = getCurrentWarePath();

    if (!Current.isEmpty())
      FileToOpen = Current;
    else
      FileToOpen = mp_Manager->getWaresdevPath();
  }

  QDesktopServices::openUrl(QUrl::fromLocalFile(FileToOpen));
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openTerminal(const QString& Path)
{
  QString FileToOpen;

  if (!Path.isEmpty())
    FileToOpen = Path;
  else
  {
    QString Current = getCurrentWarePath();

    if (!Current.isEmpty())
      FileToOpen = Current;
    else
      FileToOpen = mp_Manager->getWaresdevPath();
  }

  bool TermFound = true;

  // TODO test on Mac and not Debian-based distros
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
  if (!QProcess::startDetached("x-terminal-emulator", QStringList(),
                               FileToOpen))
    TermFound = QProcess::startDetached("xterm", QStringList(), FileToOpen);
#elif defined(Q_OS_WIN32)
  TermFound = QProcess::startDetached("cmd.exe",QStringList(),FileToOpen);
#else
  TermFound = false;
#endif

  if (!TermFound)
    QMessageBox::warning(0, "Error", "No terminal found");
}


// =====================================================================
// =====================================================================


QString WareSrcWidgetCollection::getCurrentWarePath()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
    return CurrentWare->wareSrcContainer().getAbsolutePath();

  return "";
}


// =====================================================================
// =====================================================================

WareSrcWidget* WareSrcWidgetCollection::currentWareWidget()
{
  if (WareSrcWidget* Widget = qobject_cast<WareSrcWidget*>(
      mp_TabWidget->currentWidget()))
    return Widget;

  return 0;
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::setReleaseMode()
{
  m_DefaultConfigMode = openfluid::waresdev::WareSrcContainer::CONFIG_RELEASE;

  foreach(WareSrcWidget* Ware,m_WareSrcWidgetByPath)Ware->setReleaseMode();
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::setDebugMode()
{
  m_DefaultConfigMode = openfluid::waresdev::WareSrcContainer::CONFIG_DEBUG;

  foreach(WareSrcWidget* Ware,m_WareSrcWidgetByPath)Ware->setDebugMode();
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::setBuildWithInstallMode()
{
  m_DefaultBuildMode = openfluid::waresdev::WareSrcContainer::BUILD_WITHINSTALL;

  foreach(WareSrcWidget* Ware,m_WareSrcWidgetByPath)Ware->setBuildWithInstallMode();
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::setBuildNoInstallMode()
{
  m_DefaultBuildMode = openfluid::waresdev::WareSrcContainer::BUILD_NOINSTALL;

  foreach(WareSrcWidget* Ware,m_WareSrcWidgetByPath)Ware->setBuildNoInstallMode();
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::configure()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    if (openfluid::base::PreferencesManager::instance()
        ->isAutomaticSaveBeforeBuild())
      CurrentWare->saveAllFileTabs();
    CurrentWare->wareSrcContainer().configure();
  }
  else
    QMessageBox::warning(0, "No ware open", "Open a ware first");
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::build()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    if (openfluid::base::PreferencesManager::instance()
        ->isAutomaticSaveBeforeBuild())
      CurrentWare->saveAllFileTabs();
    CurrentWare->wareSrcContainer().build();
  }
  else
    QMessageBox::warning(0, "No ware open", "Open a ware first");
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::onWareTxtModified(WareSrcWidget* Widget,
                                                bool Modified)
{
  int Pos = mp_TabWidget->indexOf(Widget);
  if (Pos > -1)
  {
    QString Label = mp_TabWidget->tabText(Pos);
    bool ModifiedState = Label.startsWith("*");

    if (Modified && !ModifiedState)
      mp_TabWidget->setTabText(Pos, Label.prepend("*"));
    else if (!Modified && ModifiedState)
      mp_TabWidget->setTabText(Pos, Label.remove(0, 1));
  }
}


// =====================================================================
// =====================================================================


bool WareSrcWidgetCollection::isModified()
{
  foreach(WareSrcWidget* Ware,m_WareSrcWidgetByPath){
  if(Ware->isModified())
  return true;
}

return false;
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::saveCurrentEditor()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
    CurrentWare->saveCurrentEditor();
  else
    QMessageBox::warning(0, "No ware open", "Open a ware first");
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::saveCurrentEditorAs(const QString& TopDirectory)
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
  {
    QString CurrentPath = CurrentWare->getCurrentFilePath();

    QString TopDir =
        TopDirectory.isEmpty() ? CurrentWare->wareSrcContainer().getAbsolutePath() :
                                 TopDirectory;

    QString NewPath = RestrictedFileDialog::getSaveFileName(CurrentWare,
                                                            CurrentPath,
                                                            TopDir);

    if (NewPath.isEmpty())
      return;

    if (NewPath == CurrentPath)
      CurrentWare->saveCurrentEditor();
    else
    {
      CurrentWare->saveCurrentEditorAs(NewPath);
      int CurrentIndexInCurrentWare = CurrentWare->closeCurrentEditor(false);

      openfluid::waresdev::WareSrcManager::PathInfo NewPathInfo = mp_Manager
          ->getPathInfo(NewPath);

      QString NewPathWarePath = NewPathInfo.m_AbsolutePathOfWare;

      // NewPath is in an already opened ware
      if (m_WareSrcWidgetByPath.contains(NewPathWarePath))
      {
        WareSrcWidget* NewPathWare = m_WareSrcWidgetByPath.value(
            NewPathWarePath);

        // in case the new path was already opened, we close it
        int IndexInOtherWare = NewPathWare->closeFileTab(NewPath);

        int NewPathIndex = -1;
        if (NewPathWare == CurrentWare)
          NewPathIndex = CurrentIndexInCurrentWare;
        else if (IndexInOtherWare > -1)
          NewPathIndex = IndexInOtherWare;

        NewPathWare->openFile(NewPathInfo, NewPathIndex);
        mp_TabWidget->setCurrentWidget(NewPathWare);
      }
      else
        openPath(NewPath);
    }
  }
  else
    QMessageBox::warning(0, "No ware open", "Open a ware first");
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::closeCurrentEditor()
{
  if (WareSrcWidget* CurrentWare = currentWareWidget())
    CurrentWare->closeCurrentEditor();
  else
    QMessageBox::warning(0, "No ware open", "Open a ware first");
}


// =====================================================================
// =====================================================================


bool WareSrcWidgetCollection::closeAllWidgets()
{
  int Choice = QMessageBox::Discard;

  if (isModified())
  {
    QMessageBox MsgBox;
    MsgBox.setText(tr("Documents have been modified."));
    MsgBox.setInformativeText(tr("Do you want to save your changes?"));
    MsgBox.setStandardButtons(
        QMessageBox::SaveAll | QMessageBox::Discard | QMessageBox::Cancel);
    MsgBox.setDefaultButton(QMessageBox::SaveAll);
    Choice = MsgBox.exec();
  }

  switch (Choice)
  {
    case QMessageBox::SaveAll:
      foreach(WareSrcWidget* Ware,m_WareSrcWidgetByPath){
      Ware->saveAllFileTabs();
    }
    case QMessageBox::Discard:
    foreach(WareSrcWidget* Ware,m_WareSrcWidgetByPath)
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
  return m_WareSrcWidgetByPath.keys();
}


// =====================================================================
// =====================================================================


bool WareSrcWidgetCollection::isDebugMode()
{
  return m_DefaultConfigMode
      == openfluid::waresdev::WareSrcContainer::CONFIG_DEBUG;
}


// =====================================================================
// =====================================================================


bool WareSrcWidgetCollection::isBuildNoInstallMode()
{
  return m_DefaultBuildMode
      == openfluid::waresdev::WareSrcContainer::BUILD_NOINSTALL;
}


// =====================================================================
// =====================================================================


} } } //namespaces
