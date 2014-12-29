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

#include <openfluid/base/FrameworkException.hpp>

#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/ui/waresdev/WareSrcWidget.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcWidgetCollection::WareSrcWidgetCollection(QTabWidget* TabWidget) :
    mp_TabWidget(TabWidget), mp_Manager(
        openfluid::waresdev::WareSrcManager::getInstance()), m_DefaultConfigMode(
        openfluid::waresdev::WareSrcContainer::CONFIG_RELEASE), m_DefaultBuildMode(
        openfluid::waresdev::WareSrcContainer::BUILD_WITHINSTALL), m_ChangedNb(
        0)
{

}


// =====================================================================
// =====================================================================


WareSrcWidgetCollection::~WareSrcWidgetCollection()
{

}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openPath(const QString& Path, bool IsStandalone)
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
      Widget = new WareSrcWidget(Info, IsStandalone, m_DefaultConfigMode,
                                 m_DefaultBuildMode, mp_TabWidget);

      mp_TabWidget->addTab(Widget, Info.m_WareName);

      m_WareSrcWidgetByPath[Info.m_AbsolutePathOfWare] = Widget;

      if (Info.m_isAWare)
        Widget->openDefaultFiles();

      connect(Widget, SIGNAL(wareTextChanged(WareSrcWidget*,bool)), this,
              SLOT(onWareTxtChanged(WareSrcWidget*,bool)));
    }

    if (Info.m_isAWareFile)
      Widget->openFile(Info);

    mp_TabWidget->setCurrentWidget(Widget);
  }

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
    QString Current = getCurrentPath();

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
    QString Current = getCurrentPath();

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


QString WareSrcWidgetCollection::getCurrentPath()
{
  try
  {
    return getCurrentWidgetContainer().getAbsolutePath();
  }
  catch (openfluid::base::FrameworkException& e)
  {
    return "";
  }
}


// =====================================================================
// =====================================================================


openfluid::waresdev::WareSrcContainer& WareSrcWidgetCollection::getCurrentWidgetContainer()
{
  if (WareSrcWidget* Widget = qobject_cast<WareSrcWidget*>(
      mp_TabWidget->currentWidget()))
    return Widget->getWareSrcContainer();

  throw openfluid::base::FrameworkException(
      "WareSrcWidgetCollection::getCurrentWidgetContainer",
      "unable to get current ware widget");
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
  try
  {
    getCurrentWidgetContainer().configure();
  }
  catch (openfluid::base::FrameworkException& e)
  {
    QMessageBox::warning(0, "No ware open", "Open a ware first");
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::build()
{
  try
  {
    getCurrentWidgetContainer().build();
  }
  catch (openfluid::base::FrameworkException& e)
  {
    QMessageBox::warning(0, "No ware open", "Open a ware first");
  }
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::onWareTxtChanged(WareSrcWidget* Widget,
                                               bool Changed)
{
  int Pos = mp_TabWidget->indexOf(Widget);
  if (Pos > -1)
  {
    QString Label = mp_TabWidget->tabText(Pos);
    bool ChangedState = Label.endsWith("*");

    if (Changed)
    {
      m_ChangedNb++;

      if (!ChangedState)
        mp_TabWidget->setTabText(Pos, Label.append("*"));
    }
    else
    {
      m_ChangedNb--;

      if (ChangedState)
      {
        Label.chop(1);
        mp_TabWidget->setTabText(Pos, Label);
      }
    }

  }
}


// =====================================================================
// =====================================================================


bool WareSrcWidgetCollection::isChanged()
{
  return m_ChangedNb > 0;
}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::saveCurrent()
{
  if (WareSrcWidget* Widget = qobject_cast<WareSrcWidget*>(
      mp_TabWidget->currentWidget()))
    Widget->saveCurrent();
  else
    QMessageBox::warning(0, "No ware open", "Open a ware first");
}


// =====================================================================
// =====================================================================


} } } //namespaces
