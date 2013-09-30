/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file ModelScene.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/machine/SimulatorSignatureRegistry.hpp>


#include "ModelScene.hpp"
#include "SimulatorGraphics.hpp"
#include "GeneratorGraphics.hpp"


ModelScene::ModelScene(const openfluid::fluidx::AdvancedModelDescriptor& ModelDesc, QObject* Parent):
  QGraphicsScene(Parent), m_Model(ModelDesc)
{

}


// =====================================================================
// =====================================================================


ModelScene::~ModelScene()
{

}


// =====================================================================
// =====================================================================


void ModelScene::refresh()
{

  foreach (ModelItemGraphics* MItemG, m_GraphicsItems)
  {
    removeItem(MItemG);
    delete MItemG;
  }
  m_GraphicsItems.clear();

  foreach (ConnectorGraphics* ConnG, m_GraphicsConnections)
  {
    removeItem(ConnG);
    delete ConnG;
  }
  m_GraphicsConnections.clear();

  clear();


  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items = m_Model.getItems();

  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator itb = Items.begin();
  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator ite = Items.end();
  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it;

  unsigned int SimCount = 0;
  unsigned int GenCount = 0;

  // model items

  for (it = itb; it!= ite; ++it)
  {
    if ((*it)->isEnabled())
    {

      QString ID = QString::fromStdString(m_Model.getID(*it));

      QVariant ConfigPos = openfluid::base::ProjectManager::getInstance()->getConfigValue("builder.model.graphicalview",ID);
      QPoint Position(0,0);

      if (ConfigPos.type() == QVariant::Point)
        Position = ConfigPos.toPoint();

      if ((*it)->getType() == openfluid::fluidx::WareDescriptor::PluggedSimulator &&
          openfluid::machine::SimulatorSignatureRegistry::getInstance()->isPluggableSimulatorAvailable(ID.toStdString()))
      {
        SimulatorGraphics* SimG = new SimulatorGraphics(QPoint(0,0),ID,
                                                        openfluid::machine::SimulatorSignatureRegistry::getInstance()->getSignatureItemInstance(ID.toStdString()));
        addItem(SimG);
        SimG->moveBy(Position.x(),Position.y());
        SimCount++;
        SimG->initialize();
        m_GraphicsItems.append(SimG);
      }
      if ((*it)->getType() == openfluid::fluidx::WareDescriptor::Generator)
      {

        openfluid::fluidx::GeneratorDescriptor* GenDesc =
                 dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it);

        GeneratorGraphics* GenG = new GeneratorGraphics(QPoint(0,0),ID,
                                                        QString::fromStdString(GenDesc->getVariableName()),
                                                        QString::fromStdString(GenDesc->getUnitClass()));
        addItem(GenG);
        GenG->moveBy(Position.x(),Position.y());
        GenCount++;
        GenG->initialize();
        m_GraphicsItems.append(GenG);
      }
    }
  }


  // connections items
  buildConnections();
}


// =====================================================================
// =====================================================================


void ModelScene::buildConnections()
{
  foreach (ModelItemGraphics* ToMItemG, m_GraphicsItems)
  {
    const ModelItemGraphics::IOSet_t* VarsSet;
    ModelItemGraphics::IOSet_t::const_iterator itTo;


    // Required vars
    VarsSet = ToMItemG->getRequired();
    itTo = VarsSet->begin();

    for (itTo = VarsSet->constBegin(); itTo != VarsSet->constEnd(); ++itTo)
    {
      foreach (QString VarName, itTo.value())
          {
        foreach (ModelItemGraphics* FromMItemG, m_GraphicsItems)
            {
          if (ToMItemG != FromMItemG)
          {

            if (FromMItemG->hasProducedVar(itTo.key(),VarName))
            {
              addConnection(FromMItemG,ConnectorGraphics::NODE_PROD,
                            ToMItemG,ConnectorGraphics::NODE_REQ,
                            VarName);
            }

            if (FromMItemG->hasUpdatedVar(itTo.key(),VarName))
            {
              addConnection(FromMItemG,ConnectorGraphics::NODE_OUTUP,
                            ToMItemG,ConnectorGraphics::NODE_REQ,
                            VarName);
            }
          }
        }
      }
    }


    // Used vars
    VarsSet = ToMItemG->getUsed();
    itTo = VarsSet->begin();

    for (itTo = VarsSet->constBegin(); itTo != VarsSet->constEnd(); ++itTo)
    {
      foreach (QString VarName, itTo.value())
      {
        foreach (ModelItemGraphics* FromMItemG, m_GraphicsItems)
        {
          if (ToMItemG != FromMItemG)
          {

            if (FromMItemG->hasProducedVar(itTo.key(),VarName))
            {
              addConnection(FromMItemG,ConnectorGraphics::NODE_PROD,
                            ToMItemG,ConnectorGraphics::NODE_US,
                            VarName);
            }

            if (FromMItemG->hasUpdatedVar(itTo.key(),VarName))
            {
              addConnection(FromMItemG,ConnectorGraphics::NODE_OUTUP,
                            ToMItemG,ConnectorGraphics::NODE_US,
                            VarName);
            }
          }
        }
      }
    }


    // Updated vars
    VarsSet = ToMItemG->getUpdated();
    itTo = VarsSet->begin();

    for (itTo = VarsSet->constBegin(); itTo != VarsSet->constEnd(); ++itTo)
    {
      foreach (QString VarName, itTo.value())
      {
        foreach (ModelItemGraphics* FromMItemG, m_GraphicsItems)
        {

          if (ToMItemG != FromMItemG)
          {
            if (FromMItemG->hasProducedVar(itTo.key(),VarName))
            {
              addConnection(FromMItemG,ConnectorGraphics::NODE_PROD,
                            ToMItemG,ConnectorGraphics::NODE_INUP,
                            VarName);
            }

            if (FromMItemG->hasUpdatedVar(itTo.key(),VarName))
            {
              addConnection(FromMItemG,ConnectorGraphics::NODE_OUTUP,
                            ToMItemG,ConnectorGraphics::NODE_INUP,
                            VarName);
            }
          }
        }
      }
    }
  }
}


// =====================================================================
// =====================================================================


void ModelScene::addConnection(ModelItemGraphics* FromItem, ConnectorGraphics::OutNodeType FromOutNode,
                               ModelItemGraphics* ToItem, ConnectorGraphics::InNodeType ToInNode,
                               const QString& VarName)
{
  bool Found = false;

  foreach(ConnectorGraphics* Conn, m_GraphicsConnections)
  {
    if (Conn->getFromItem() == FromItem && Conn->getFromNode() == FromOutNode &&
        Conn->getToItem() == FromItem && Conn->getToNode() == ToInNode)
    {
      Conn->addVariable(VarName);
      Found = true;
    }
    if (Found) break;
  }

  if (!Found)
  {
    ConnectorGraphics* ConnG = new ConnectorGraphics(FromItem,FromOutNode,
                                                     ToItem,ToInNode);
    ConnG->addVariable(VarName);
    FromItem->addConnector(ConnG);
    ToItem->addConnector(ConnG);
    addItem(ConnG);
  }

}
