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
  @file SourceWorker.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __SOURCEWORKER_HPP__
#define __SOURCEWORKER_HPP__


#include "ogrsf_frmts.h"

#include <QObject>

#include <openfluid/utils/GDALCompatibility.hpp>


class SourceWorker : public QObject
{
  Q_OBJECT;


  private:

    QString m_URI;

    GDALDataset_COMPAT* mp_DataSource;


  signals:

    void sourceLinked(void* Src);

    void layerCounted(int Count);

    void layerFetched(int Index, QString Name, QString GeomStr);

    void finished();

    void errorHappened(QString Message);


  public slots:

    void run();


  public:

    SourceWorker(const QString& URI, GDALDataset_COMPAT* Src);

    ~SourceWorker();



 };


#endif /* __SOURCEWORKER_HPP__ */
