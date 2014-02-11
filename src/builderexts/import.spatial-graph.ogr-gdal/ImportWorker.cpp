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
  \file ImportWorker.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "ImportWorker.hpp"


ImportWorker::ImportWorker(const SourcesInfosList_t& SourcesInfos,
                           openfluid::fluidx::AdvancedFluidXDescriptor* AdvDesc):
  DataProcessingWorker(SourcesInfos,AdvDesc)
{

}


// =====================================================================
// =====================================================================


ImportWorker::~ImportWorker()
{

}


// =====================================================================
// =====================================================================


bool ImportWorker::importLayer(int Step,int Index)
{
  emit stepEntered(tr("Importing layer \"%1\"...").arg(m_SourcesInfos[Index].LayerName));

  QString UnitsClass = m_SourcesInfos[Index].UnitsClass;
  bool AtttrsCreated = false;

  QMapIterator<int, SourceUnit> It(m_SourcesData[Index].Units);

  while (It.hasNext())
  {
    It.next();

    openfluid::fluidx::UnitDescriptor* UDesc = new openfluid::fluidx::UnitDescriptor();

    UDesc->getUnitClass() = UnitsClass.toStdString();
    UDesc->getUnitID() = It.key();
    UDesc->getProcessOrder() = It.value().ProcessOrder;

    mp_AdvDesc->getDomain().addUnit(UDesc);


    QMapIterator<QString,QString> AttrsIt(It.value().Attributes);

    // create attributes on first unit of the units class
    if (!AtttrsCreated)
    {
      while (AttrsIt.hasNext())
      {
        AttrsIt.next();
        mp_AdvDesc->getDomain().addAttribute(UnitsClass.toStdString(),
                                             AttrsIt.key().toStdString(),"-");
      }

      AttrsIt.toFront();
      AtttrsCreated = true;
    }

    // set attributes values for the current imported unit
    while (AttrsIt.hasNext())
    {
      AttrsIt.next();

      mp_AdvDesc->getDomain().getAttribute(UnitsClass.toStdString(),
                                           It.key(),
                                           AttrsIt.key().toStdString()) = AttrsIt.value().toStdString();
    }

  }

  emit stepCompleted(Step,getStyledText(tr("[OK]"),"green"));

  return true;
}


// =====================================================================
// =====================================================================


bool ImportWorker::buildConnections(int Step)
{
  emit stepEntered(tr("Building connections..."));

  int i=0;
  bool OKToContinue = true;

  while (i<m_SourcesInfos.size() && OKToContinue)
  {
    QMapIterator<int, SourceUnit> It(m_SourcesData[i].Units);

    while (It.hasNext())
    {
      It.next();

      // "to" connections
      for (int j=0;j<It.value().ToConn.size();j++)
      {
        mp_AdvDesc->getDomain().addFromToRelation(std::make_pair(m_SourcesInfos[i].UnitsClass.toStdString(),
                                                                 It.key()),
                                                  std::make_pair(It.value().ToConn[j].DestClass.toStdString(),
                                                                 It.value().ToConn[j].DestID));
      }

      // "childof" connections
      for (int j=0;j<It.value().ChildofConn.size();j++)
      {
        mp_AdvDesc->getDomain().addParentChildRelation(std::make_pair(It.value().ChildofConn[j].DestClass.toStdString(),
                                                                      It.value().ChildofConn[j].DestID),
                                                       std::make_pair(m_SourcesInfos[i].UnitsClass.toStdString(),
                                                                      It.key()));
      }
    }
    i++;
  }

  emit stepCompleted(Step,getStyledText(tr("[OK]"),"green"));

  return OKToContinue;

}


// =====================================================================
// =====================================================================


bool ImportWorker::runImport(int StartStep)
{
  int i=0;

  bool OKToContinue = true;

  while (i<m_SourcesInfos.size() && OKToContinue)
  {
    OKToContinue = importLayer(StartStep+i,i);
    i++;
  }

  if (OKToContinue)
    OKToContinue = buildConnections(StartStep+m_SourcesInfos.size()+1);

  return OKToContinue;
}


// =====================================================================
// =====================================================================


void ImportWorker::run()
{
  if (!runCheck(1))
    emit completed(getStyledText(tr("Consistency error, "
                                    "import could not be performed with current import configuration"),
                                 "red",true));
  else
  {
    if (runImport(OGRGDALEXT_PRECHECK_STEPS+1))
    {
      emit completed(getStyledText(tr("Import sucessful"),
                                       "green",true));
    }
    else
    {
      emit completed(getStyledText(tr("Import error. "
                                       "Import may be uncompleted or inconsistent"),
                                       "red",true));
    }

    emit closeRequired();
  }

  emit finished();
}
