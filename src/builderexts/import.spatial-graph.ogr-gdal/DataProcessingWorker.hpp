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
  \file DataProcessingWorker.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __DATAPROCESSINGWORKER_HPP__
#define __DATAPROCESSINGWORKER_HPP__

#include "SourceInfos.hpp"
#include "SourceData.hpp"

#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include <QObject>


class DataProcessingWorker : public QObject
{
  Q_OBJECT;


  protected:

    const SourcesInfosList_t& m_SourcesInfos;

    SourcesDataList_t m_SourcesData;

    openfluid::fluidx::AdvancedFluidXDescriptor* mp_AdvDesc;

    bool runCheck(int StartStep);

    bool loadDataFromSources(int Step);

    bool checkConnectivity(int Step);

    static QString getStyledText(const QString& Text,
                                 const QString& Color, bool IsBold);

    static bool convertFieldToAttribute(const OGRFeature* Feature, const int FieldIndex,
                                        QString& Attr);

    static bool convertConnectionsStringToList(const QString& ConnStr,
                                               QList<SourceConnection>& ConnList);

    bool isUnitExists(const QString& Class, int ID);


  signals:

    void stepEntered(QString Message);

    void stepCompleted(int StepNbr, QString Message);

    void completed(QString Message);

    void finished();


  public slots:

    virtual void run() = 0;


  public:

    DataProcessingWorker(const SourcesInfosList_t& SourcesInfos,
                         openfluid::fluidx::AdvancedFluidXDescriptor* AdvDesc);

    ~DataProcessingWorker();


};


#endif /* __DATAPROCESSINGWORKER_HPP__ */
