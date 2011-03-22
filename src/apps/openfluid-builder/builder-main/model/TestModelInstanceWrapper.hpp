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
 \file TestModelInstance.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __TESTMODELINSTANCE_HPP__
#define __TESTMODELINSTANCE_HPP__

#include <openfluid/base.hpp>
#include <openfluid/core.hpp>
#include <BuilderMachineListener.hpp>
#include "ModelItemInstanceFactory.hpp"
#include "GeneratorSignature.hpp"
#include "EngineProjectFactory.hpp"

class TestModelInstanceWrapper
{
  protected:
//    EngineProject* mp_EngineProject;
        openfluid::machine::SimulationBlob* mp_SimBlob;
        BuilderMachineListener* mp_Listener;
        openfluid::io::IOListener* mp_IOListener;
    openfluid::machine::ModelInstance* mp_ModelInstance;
    void addItems(unsigned int ItemCountAsked = 0)
    {
      FixedGeneratorSignature FixedGenSignature;
      RandomGeneratorSignature RandomGenSignature;
      InterpGeneratorSignature InterpGenSignature;

      switch (ItemCountAsked)
      {
        case 0:
          ;
          break;
        case 1:
          mp_ModelInstance->appendItem(
                        ModelItemInstanceFactory::createModelItemInstanceFromSignature(
                            FixedGenSignature));
          break;
        case 2:
          mp_ModelInstance->appendItem(
              ModelItemInstanceFactory::createModelItemInstanceFromSignature(
                            FixedGenSignature));
                    mp_ModelInstance->appendItem(
                        ModelItemInstanceFactory::createModelItemInstanceFromSignature(
                            RandomGenSignature));
          break;
        case 3:
          mp_ModelInstance->appendItem(
              ModelItemInstanceFactory::createModelItemInstanceFromSignature(
                            FixedGenSignature));
                    mp_ModelInstance->appendItem(
                        ModelItemInstanceFactory::createModelItemInstanceFromSignature(
                            RandomGenSignature));
                    mp_ModelInstance->appendItem(
                        ModelItemInstanceFactory::createModelItemInstanceFromSignature(
                            InterpGenSignature));
          break;
        default:
          throw openfluid::base::OFException("OpenFLUID Builder",
              "TestModelInstanceWrapper::TestModelInstanceWrapper",
              "bad count of items to create asked");
      }
    }
  public:
    TestModelInstanceWrapper(unsigned int ItemCountAsked = 0)
    {
//      mp_EngineProject = EngineProjectFactory::createEngineProject();
            mp_SimBlob = new openfluid::machine::SimulationBlob();

            mp_Listener = new BuilderMachineListener();
            mp_IOListener = new openfluid::io::IOListener();

//      mp_ModelInstance = mp_EngineProject->getModelInstance();
//
//      mp_ItemInstanceFactory = mp_EngineProject->getItemFactory();
//      mp_ItemInstanceFactory = new ModelItemInstanceFactory(
//          mp_EngineProject->getSimBlob()->getExecutionMessages(),
//          mp_EngineProject->getSimBlob()->getCoreRepository());
      //      mp_ItemInstanceFactory = new ModelItemInstanceFactory(
      //          mp_EngineProject->getExecutionMessages(),
      //          mp_EngineProject->getCoreRepository());
            mp_ModelInstance = new openfluid::machine::ModelInstance(*mp_SimBlob,
                mp_Listener);
      addItems(ItemCountAsked);
    }
    ~TestModelInstanceWrapper()
    {
//      delete mp_EngineProject;
            delete mp_IOListener;
            delete mp_Listener;
            delete mp_SimBlob;
            delete mp_ModelInstance;
    }
    openfluid::machine::ModelInstance* getModelInstance()
    {
      return mp_ModelInstance;
    }
    std::string getItemIdAt(unsigned int Position)
    {
      if (Position < mp_ModelInstance->getItemsCount())
      {
        std::list<openfluid::machine::ModelItemInstance*>::const_iterator it;
        it = mp_ModelInstance->getItems().begin();
        std::advance(it, Position);
        return (*it)->Signature->ID;
      } else
        return "no item at this Position : " + Position;
    }
};
//
//class TestModelInstanceWithProjectWrapper : public TestModelInstanceWrapper
//{private:
//    EngineProject& m_EngineProject;
//  public:
//    TestModelInstanceWithProjectWrapper(EngineProject& EngProject,unsigned int ItemCountAsked = 0)
//    : mp_EngineProject(EngProject)
//    {
//      mp_ModelInstance = m_EngineProject.getModelInstance();
//
//      mp_ItemInstanceFactory = new ModelItemInstanceFactory(
//          m_EngineProject.getExecutionMessages(), m_EngineProject.getCoreRepository());
//
//      addItems(ItemCountAsked);
//    }
//    ~TestModelInstanceWithProjectWrapper()
//    {
//      delete m_EngineProject;
//    }
//};

#endif /* __TESTMODELINSTANCE_HPP__ */
