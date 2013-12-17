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
  \file SimulatorGraphics.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "builderconfig.hpp"
#include "SimulatorGraphics.hpp"

#include <QBrush>


SimulatorGraphics::SimulatorGraphics(const QPointF& Coords, const QString& ID,
                                     openfluid::machine::ModelItemSignatureInstance* Signature,
                                     QGraphicsItem* Parent):
  ModelItemGraphics(Coords,ID,Parent)
{
  openfluid::ware::SimulatorSignature* SimSign = Signature->Signature;
  std::vector<openfluid::ware::SignatureHandledTypedDataItem> VarList;

  VarList = SimSign->HandledData.RequiredVars;
  for (unsigned int i=0;i<VarList.size();i++)
    m_RequiredVars[QString::fromStdString(VarList[i].UnitClass)].append(QString::fromStdString(VarList[i].DataName));

  VarList = SimSign->HandledData.UsedVars;
  for (unsigned int i=0;i<VarList.size();i++)
    m_UsedVars[QString::fromStdString(VarList[i].UnitClass)].append(QString::fromStdString(VarList[i].DataName));

  VarList = SimSign->HandledData.ProducedVars;
  for (unsigned int i=0;i<VarList.size();i++)
    m_ProducedVars[QString::fromStdString(VarList[i].UnitClass)].append(QString::fromStdString(VarList[i].DataName));

  VarList = SimSign->HandledData.UpdatedVars;
  for (unsigned int i=0;i<VarList.size();i++)
    m_UpdatedVars[QString::fromStdString(VarList[i].UnitClass)].append(QString::fromStdString(VarList[i].DataName));


  // In/Out slots
  drawIOSlot(getRequiredIOPosition(),"Req",!m_RequiredVars.isEmpty());
  drawIOSlot(getUsedIOPosition(),"Us",!m_UsedVars.isEmpty());
  drawIOSlot(getUpInIOPosition(),"Upd",!m_UpdatedVars.isEmpty());
  drawIOSlot(getProducedIOPosition(),"Prod",!m_ProducedVars.isEmpty());
  drawIOSlot(getUpOutIOPosition(),"Upd",!m_UpdatedVars.isEmpty());

  setBrush(QBrush(QColor(BUILDER_SIMULATOR_BGCOLOR)));
}


// =====================================================================
// =====================================================================


SimulatorGraphics::~SimulatorGraphics()
{

}


// =====================================================================
// =====================================================================


QPointF SimulatorGraphics::getProducedIOPosition()
{
  return scenePos()+getCenterFromOrigin()+QPoint(-30,m_DefaultSize.height()/2);
}
