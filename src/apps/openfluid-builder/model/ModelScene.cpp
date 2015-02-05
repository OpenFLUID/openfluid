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
  @file ModelScene.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
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


void ModelScene::notifySrcEditAsked(const QString& ID)
{
  emit srcEditAsked(ID);
}


// =====================================================================
// =====================================================================


void ModelScene::refresh()
{
  // clear existing scene

  // clear model items
  foreach (ModelItemGraphics* MItemG, m_GraphicsItems)
  {
    removeItem(MItemG);
    delete MItemG;
  }
  m_GraphicsItems.clear();

  // clear connections
  foreach (ConnectorGraphics* ConnG, m_GraphicsConnections)
  {
    removeItem(ConnG);
    delete ConnG;
  }
  m_GraphicsConnections.clear();

  clear();


  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items = m_Model.items();

  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator itb = Items.begin();
  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator ite = Items.end();
  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it;

  unsigned int SimCount = 0;
  unsigned int GenCount = 0;



  // add model items

  for (it = itb; it!= ite; ++it)
  {
    if ((*it)->isEnabled())
    {

      QString ID = QString::fromStdString(m_Model.getID(*it));

      QVariant ConfigPos =
          openfluid::base::ProjectManager::instance()->getConfigValue("builder.model.graphicalview",ID);
      QPoint Position(0,0);

      if (ConfigPos.type() == QVariant::Point)
        Position = ConfigPos.toPoint();
      else
      {
        QRectF CurrentRect = itemsBoundingRect();
        Position.setX(int(CurrentRect.x()+CurrentRect.width()/2));
        Position.setY(int(CurrentRect.y()+CurrentRect.height()/2));
      }

      if ((*it)->getType() == openfluid::fluidx::WareDescriptor::PluggedSimulator &&
          openfluid::machine::SimulatorSignatureRegistry::instance()->isSimulatorAvailable(ID.toStdString()))
      {
        SimulatorGraphics* SimG =
            new SimulatorGraphics(QPoint(0,0),
                                  ID,
                                  openfluid::machine::SimulatorSignatureRegistry::instance()
                                  ->signature(ID.toStdString()));
        addItem(SimG);
        SimG->moveBy(Position.x(),Position.y());
        SimCount++;
        SimG->initialize();
        m_GraphicsItems.append(SimG);
        connect(SimG,SIGNAL(srcEditAsked(const QString&)),this,SLOT(notifySrcEditAsked(const QString&)));
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


  // add connections
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

    ModelItemGraphics* ProviderItem;
    ConnectorGraphics::OutNodeType ProviderOutNode;

    // Required vars
    VarsSet = ToMItemG->requiredVariables();
    itTo = VarsSet->begin();

    for (itTo = VarsSet->constBegin(); itTo != VarsSet->constEnd(); ++itTo)
    {
      foreach (QString VarName, itTo.value())
      {
        ProviderItem = NULL;

        foreach (ModelItemGraphics* FromMItemG, m_GraphicsItems)
        {
          if (ToMItemG != FromMItemG)
          {

            if (FromMItemG->hasProducedVariable(itTo.key(),VarName))
            {
              ProviderItem = FromMItemG;
              ProviderOutNode = ConnectorGraphics::NODE_PROD;

            }

            if (FromMItemG->hasUpdatedVariable(itTo.key(),VarName))
            {
              ProviderItem = FromMItemG;
              ProviderOutNode = ConnectorGraphics::NODE_OUTUP;
            }
          }
        }

        // use the latest provider item
        if (ProviderItem != NULL)
        {
          addConnection(ProviderItem,ProviderOutNode,
                        ToMItemG,ConnectorGraphics::NODE_REQ,
                        itTo.key(),VarName);
        }
      }
    }


    // Used vars
    VarsSet = ToMItemG->usedVariables();
    itTo = VarsSet->begin();

    for (itTo = VarsSet->constBegin(); itTo != VarsSet->constEnd(); ++itTo)
    {
      foreach (QString VarName, itTo.value())
      {
        ProviderItem = NULL;
        foreach (ModelItemGraphics* FromMItemG, m_GraphicsItems)
        {
          if (ToMItemG != FromMItemG)
          {

            if (FromMItemG->hasProducedVariable(itTo.key(),VarName))
            {
              ProviderItem = FromMItemG;
              ProviderOutNode = ConnectorGraphics::NODE_PROD;
            }

            if (FromMItemG->hasUpdatedVariable(itTo.key(),VarName))
            {
              ProviderItem = FromMItemG;
              ProviderOutNode = ConnectorGraphics::NODE_OUTUP;
            }
          }
        }

        // use the latest provider item
        if (ProviderItem != NULL)
        {
          addConnection(ProviderItem,ProviderOutNode,
                        ToMItemG,ConnectorGraphics::NODE_US,
                        itTo.key(),VarName);
        }
      }
    }


    // Updated vars
    VarsSet = ToMItemG->updatedVariables();
    itTo = VarsSet->begin();

    for (itTo = VarsSet->constBegin(); itTo != VarsSet->constEnd(); ++itTo)
    {
      foreach (QString VarName, itTo.value())
      {
        ProviderItem = NULL;
        foreach (ModelItemGraphics* FromMItemG, m_GraphicsItems)
        {

          if (ToMItemG != FromMItemG)
          {
            if (FromMItemG->hasProducedVariable(itTo.key(),VarName))
            {
              ProviderItem = FromMItemG;
              ProviderOutNode = ConnectorGraphics::NODE_PROD;
            }

            if (FromMItemG->hasUpdatedVariable(itTo.key(),VarName))
            {
              ProviderItem = FromMItemG;
              ProviderOutNode = ConnectorGraphics::NODE_OUTUP;
            }
          }
          // input for updated vars cannot be after the current simulator/generator
          if (FromMItemG == ToMItemG) break;
        }

        // use the latest provider item
        if (ProviderItem != NULL)
        {
          addConnection(ProviderItem,ProviderOutNode,
                        ToMItemG,ConnectorGraphics::NODE_INUP,
                        itTo.key(),VarName);
        }
      }
    }
  }
}


// =====================================================================
// =====================================================================


void ModelScene::addConnection(ModelItemGraphics* FromItem, ConnectorGraphics::OutNodeType FromOutNode,
                               ModelItemGraphics* ToItem, ConnectorGraphics::InNodeType ToInNode,
                               const QString& UnitClass, const QString& VarName)
{
  bool Found = false;

  foreach(ConnectorGraphics* Conn, m_GraphicsConnections)
  {
    if (Conn->fromItem() == FromItem && Conn->getFromNode() == FromOutNode &&
        Conn->toItem() == ToItem && Conn->getToNode() == ToInNode)
    {
      // if connection already exist, just add the variable name
      Conn->addVariable(UnitClass,VarName);
      Found = true;
    }
    if (Found) break;
  }

  if (!Found)
  {
    // if connection does not exist, add the connection and the variable name

    ConnectorGraphics* ConnG = new ConnectorGraphics(FromItem,FromOutNode,
                                                     ToItem,ToInNode);
    ConnG->addVariable(UnitClass,VarName);
    FromItem->addConnector(ConnG);
    ToItem->addConnector(ConnG);
    addItem(ConnG);
    m_GraphicsConnections.append(ConnG);
  }

}


// =====================================================================
// =====================================================================


void ModelScene::showVariables(bool Show)
{
  foreach(ConnectorGraphics* Conn, m_GraphicsConnections)
  {
    Conn->setVariablesNamesVisible(Show);
  }
}
