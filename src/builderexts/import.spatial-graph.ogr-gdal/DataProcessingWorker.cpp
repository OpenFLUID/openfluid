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
  \file DataProcessingWorker.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "DataProcessingWorker.hpp"

#include <ogrsf_frmts.h>


DataProcessingWorker::DataProcessingWorker(const SourcesInfosList_t& SourcesInfos,
                                           openfluid::fluidx::AdvancedFluidXDescriptor* AdvDesc):
  m_SourcesInfos(SourcesInfos), mp_AdvDesc(AdvDesc)
{

}


// =====================================================================
// =====================================================================


DataProcessingWorker::~DataProcessingWorker()
{

}


// =====================================================================
// =====================================================================


QString DataProcessingWorker::getStyledText(const QString& Text,
                                            const QString& Color, bool IsBold=false)
{
  QString BoldStyle;

  if (IsBold)
    BoldStyle = " font-weight: bold;";

  return QString("<font style='color: %1;%2'>%3</font>").arg(Color,BoldStyle,Text);
}


// =====================================================================
// =====================================================================


void DataProcessingWorker::loadDataFromSources()
{
  // Parse and import "standard" attributes (ID, PCsOrd, To, Childof),
  //   check that IDs are unique, PcsOrd >= 0


  // Import other attributes, replacing unwanted chars (space, tab) by underscore

}


// =====================================================================
// =====================================================================


bool DataProcessingWorker::runCheck(int StartStep)
{
  // Check unit class does not exist
  emit stepEntered(tr("Checking units classes..."));

  for (int i=0; i<m_SourcesInfos.size();i++)
  {
    if (m_SourcesInfos[i].UnitsClass.isEmpty())
    {
      emit stepCompleted(StartStep,getStyledText(tr("[Error] Class name for layer \"%1\" is empty").arg(m_SourcesInfos[i].LayerName),"red"));
      return false;
    }

    if (mp_AdvDesc->getDomain().isClassNameExists(m_SourcesInfos[i].UnitsClass.toStdString()))
    {
      emit stepCompleted(StartStep,getStyledText(tr("[Error] Class name for layer \"%1\" already exists").arg(m_SourcesInfos[i].LayerName),"red"));
      return false;
    }
  }

  emit stepCompleted(StartStep,getStyledText(tr("[OK]"),"green"));

  // load data from source
  emit stepEntered(tr("Loading and checking data from sources..."));
  loadDataFromSources();
  emit stepCompleted(StartStep+1,getStyledText(tr("[OK]"),"green"));

  // Check connectivity against both existing and imported units
  emit stepEntered(tr("Checking connectivity consistency..."));
  emit stepCompleted(StartStep+2,getStyledText(tr("[OK]"),"green"));

  return true;
}
