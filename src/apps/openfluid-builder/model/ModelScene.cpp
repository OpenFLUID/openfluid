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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>

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


void ModelScene::notifySrcEditAsked(const QString& ID, bool Ghost)
{
  emit srcEditAsked(ID,Ghost);
}


// =====================================================================
// =====================================================================


void ModelScene::refresh()
{
  // clear existing scene

  // clear model items
  for (ModelItemGraphics* MItemG : m_GraphicsItems)
  {
    removeItem(MItemG);
    delete MItemG;
  }
  m_GraphicsItems.clear();

  // clear connections
  for  (ConnectorGraphics* ConnG : m_GraphicsConnections)
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
          openfluid::base::RunContextManager::instance()->getProjectConfigValue("builder.model.graphicalview",ID);
      QPoint Position(0,0);

      if (ConfigPos.type() == QVariant::Point)
        Position = ConfigPos.toPoint();
      else
      {
        QRectF CurrentRect = itemsBoundingRect();
        Position.setX(int(CurrentRect.x()+CurrentRect.width()/2));
        Position.setY(int(CurrentRect.y()+CurrentRect.height()/2));
      }

      if ((*it)->getType() == openfluid::ware::WareType::SIMULATOR &&
          openfluid::machine::SimulatorSignatureRegistry::instance()->isSimulatorAvailable(ID.toStdString()))
      {
        SimCount++;

        SimulatorGraphics* SimG =
            new SimulatorGraphics(QPoint(0,0),
                                  ID, SimCount+GenCount,
                                  openfluid::machine::SimulatorSignatureRegistry::instance()
                                  ->signature(ID.toStdString()));

        addItem(SimG);
        SimG->moveBy(Position.x(),Position.y());
        SimG->initialize();
        m_GraphicsItems.append(SimG);
        connect(SimG,SIGNAL(srcEditAsked(const QString&,bool)),this,SLOT(notifySrcEditAsked(const QString&,bool)));
      }
      else if ((*it)->getType() == openfluid::ware::WareType::GENERATOR)
      {
        openfluid::fluidx::GeneratorDescriptor* GenDesc =
                 dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it);

        GenCount++;

        GeneratorGraphics* GenG = new GeneratorGraphics(QPoint(0,0),
                                                        ID,SimCount+GenCount,
                                                        QString::fromStdString(GenDesc->getVariableName()),
                                                        QString::fromStdString(GenDesc->getUnitsClass()));
        addItem(GenG);
        GenG->moveBy(Position.x(),Position.y());
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

  for (ModelItemGraphics* ToMItemG : m_GraphicsItems)
  {
    ModelItemGraphics* ProviderItem = nullptr;
    ConnectorGraphics::OutNodeType ProviderOutNode;

    // Required vars
    for (const auto& ReqVarInfos : *(ToMItemG->requiredVariables()))
    {
      ProviderItem = nullptr;

      for (ModelItemGraphics* FromMItemG : m_GraphicsItems)
      {
        if (ToMItemG != FromMItemG)
        {
          if (FromMItemG->hasProducedVariable(ReqVarInfos.UnitsClass,ReqVarInfos.DataName))
          {
            ProviderItem = FromMItemG;
            ProviderOutNode = ConnectorGraphics::NODE_PROD;
          }

          if (FromMItemG->hasUpdatedVariable(ReqVarInfos.UnitsClass,ReqVarInfos.DataName))
          {
            ProviderItem = FromMItemG;
            ProviderOutNode = ConnectorGraphics::NODE_OUTUP;
          }
        }
      }
      // use the latest provider item
      if (ProviderItem != nullptr)
      {
        addConnection(ProviderItem,ProviderOutNode,
                      ToMItemG,ConnectorGraphics::NODE_REQ,
                      ReqVarInfos);
      }
    }


    // Used vars
    for (const auto& UsedVarInfos : *(ToMItemG->usedVariables()))
    {
      ProviderItem = nullptr;

      for (ModelItemGraphics* FromMItemG : m_GraphicsItems)
      {
        if (ToMItemG != FromMItemG)
        {

          if (FromMItemG->hasProducedVariable(UsedVarInfos.UnitsClass,UsedVarInfos.DataName))
          {
            ProviderItem = FromMItemG;
            ProviderOutNode = ConnectorGraphics::NODE_PROD;
          }

          if (FromMItemG->hasUpdatedVariable(UsedVarInfos.UnitsClass,UsedVarInfos.DataName))
          {
            ProviderItem = FromMItemG;
            ProviderOutNode = ConnectorGraphics::NODE_OUTUP;
          }
        }
      }

      // use the latest provider item
      if (ProviderItem != nullptr)
      {
        addConnection(ProviderItem,ProviderOutNode,
                      ToMItemG,ConnectorGraphics::NODE_US,
                      UsedVarInfos);
      }
    }


    // Updated vars
    for (const auto& UpdatedVarInfos : *(ToMItemG->updatedVariables()))
    {
      ProviderItem = nullptr;
      for (ModelItemGraphics* FromMItemG : m_GraphicsItems)
      {
        if (ToMItemG != FromMItemG)
        {
          if (FromMItemG->hasProducedVariable(UpdatedVarInfos.UnitsClass,UpdatedVarInfos.DataName))
          {
            ProviderItem = FromMItemG;
            ProviderOutNode = ConnectorGraphics::NODE_PROD;
          }

          if (FromMItemG->hasUpdatedVariable(UpdatedVarInfos.UnitsClass,UpdatedVarInfos.DataName))
          {
            ProviderItem = FromMItemG;
            ProviderOutNode = ConnectorGraphics::NODE_OUTUP;
          }
        }
        // input for updated vars cannot be after the current simulator/generator
        if (FromMItemG == ToMItemG) break;
      }

      // use the latest provider item
      if (ProviderItem != nullptr)
      {
        addConnection(ProviderItem,ProviderOutNode,
                      ToMItemG,ConnectorGraphics::NODE_INUP,
                      UpdatedVarInfos);
      }
    }
  }
}


// =====================================================================
// =====================================================================


void ModelScene::addConnection(ModelItemGraphics* FromItem, ConnectorGraphics::OutNodeType FromOutNode,
                               ModelItemGraphics* ToItem, ConnectorGraphics::InNodeType ToInNode,
                               const openfluid::ware::SignatureTypedSpatialDataItem& VarInfos)
{
  bool Found = false;

  for (ConnectorGraphics* Conn : m_GraphicsConnections)
  {
    if (Conn->fromItem() == FromItem && Conn->getFromNode() == FromOutNode &&
        Conn->toItem() == ToItem && Conn->getToNode() == ToInNode)
    {
      // if connection already exist, just add the variable name
      Conn->addVariable(VarInfos);
      Found = true;
    }
    if (Found) break;
  }

  if (!Found)
  {
    // if connection does not exist, add the connection and the variable name

    ConnectorGraphics* ConnG = new ConnectorGraphics(FromItem,FromOutNode,
                                                     ToItem,ToInNode);
    ConnG->addVariable(VarInfos);
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
  for (ConnectorGraphics* Conn : m_GraphicsConnections)
  {
    Conn->setVariablesNamesVisible(Show);
  }
}
