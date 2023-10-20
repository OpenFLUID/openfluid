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
  @file AppMainWindow.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <QApplication>
#include <QResizeEvent>
#include <QMoveEvent>
#if (QT_VERSION_MAJOR < 6)
#include <QDesktopWidget>
#endif

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/ui/common/AppMainWindow.hpp>
#include <openfluid/ui/common/DefaultAction.hpp>


namespace openfluid { namespace ui { namespace common {


AppMainWindow::AppMainWindow(const std::string& AppSettings, QWidget *Parent):
  QMainWindow(Parent), mp_ResetGeometryAction(nullptr), m_AppSettings(AppSettings)
{
  applyWindowGeometry();
}


// =====================================================================
// =====================================================================


AppMainWindow::~AppMainWindow()
{
  saveWindowGeometry();
}


// =====================================================================
// =====================================================================


void AppMainWindow::saveWindowGeometry() const
{
  openfluid::base::PreferencesManager::instance()->setAppWindowGeometry(m_AppSettings,
                                                                        {x(),y(),width(),height(),true,true});
}


// =====================================================================
// =====================================================================


QSize AppMainWindow::computeDefaultSize() const
{
#if (QT_VERSION_MAJOR < 6)
  const QRect& ScreenRect = QApplication::desktop()->screenGeometry();
#else
  const QRect& ScreenRect = QGuiApplication::primaryScreen()->geometry();
#endif

  return QSize(ScreenRect.width()*0.9,ScreenRect.height()*0.9);
}


// =====================================================================
// =====================================================================


QPoint AppMainWindow::computeDefaultPosition() const
{
#if (QT_VERSION_MAJOR < 6)
  const QRect& ScreenRect = QApplication::desktop()->screenGeometry();
#else
  const QRect& ScreenRect = QGuiApplication::primaryScreen()->geometry();
#endif

  return QPoint((ScreenRect.width()-width())/2,(ScreenRect.height()-height())/2);
}


// =====================================================================
// =====================================================================


void AppMainWindow::applyWindowGeometry()
{
  auto AppGeometry = openfluid::base::PreferencesManager::instance()->getAppWindowGeometry(m_AppSettings);

  if (!(AppGeometry.ValidSize && AppGeometry.Width > 0 && AppGeometry.Height > 0))
  {
    auto Size = computeDefaultSize();
    AppGeometry.Width = Size.width();
    AppGeometry.Height = Size.height();
    AppGeometry.ValidSize = true;
  }
  resize(AppGeometry.Width,AppGeometry.Height);

  if (!AppGeometry.ValidPosition)
  {
    auto Position = computeDefaultPosition();
    AppGeometry.X = Position.x();
    AppGeometry.Y = Position.y();
    AppGeometry.ValidPosition = true;
  }
  move(AppGeometry.X,AppGeometry.Y);
}


// =====================================================================
// =====================================================================


void AppMainWindow::restoreToDefaultGeometry()
{
  resize(computeDefaultSize());
  move(computeDefaultPosition());
}


// =====================================================================
// =====================================================================


QAction* AppMainWindow::getResetGeometryAction(const QString& Text)
{
  if (mp_ResetGeometryAction == nullptr)
  {
    mp_ResetGeometryAction = new DefaultAction(Text,this);
    connect(mp_ResetGeometryAction, &QAction::triggered, this, &AppMainWindow::restoreToDefaultGeometry);
  }

  return mp_ResetGeometryAction;
}


} } } // namespaces

