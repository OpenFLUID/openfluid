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
  @file WaresImportWorker.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <QCoreApplication>
#include <QFileInfo>

#include <openfluid/ui/waresdev/WaresImportWorker.hpp>
#include <openfluid/ui/waresdev/GitUIProxy.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WaresImportWorker::WaresImportWorker(bool SslNoVerify, bool AutoCheckout) : GitImportWorker(SslNoVerify, AutoCheckout)
{

}


// =====================================================================
// =====================================================================


WaresImportWorker::~WaresImportWorker()
{
  
}


// =====================================================================
// =====================================================================


bool WaresImportWorker::importElement(const QString& GitUrl, const QString& WareTypePath)
{
  QString DestPath = QString("%1/%2").arg(WareTypePath).arg(QFileInfo(GitUrl).fileName());
  
  GitUIProxy Git;
  QObject::connect(&Git, SIGNAL(info(const QString&)), this, SIGNAL(info(const QString&)));
  QObject::connect(&Git, SIGNAL(error(const QString&)), this, SIGNAL(error(const QString&)));
  bool Success = Git.clone(GitUrl, DestPath, m_Username, m_Password, m_SslNoVerify);
  if (Success && m_AutoCheckout)
  {
    checkoutCurrentOpenFLUIDBranch(DestPath);
  }
  return Success;
}


} } } // namespaces
