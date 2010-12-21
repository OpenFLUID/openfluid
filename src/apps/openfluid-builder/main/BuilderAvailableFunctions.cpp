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
  \file BuilderPluginManager.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */


#include "BuilderAvailableFunctions.hpp"


// =====================================================================
// =====================================================================


BuilderAvailableFunctions * BuilderAvailableFunctions::mp_Singleton = 0;


// =====================================================================
// =====================================================================


BuilderAvailableFunctions::BuilderAvailableFunctions()
{
    m_PluggableFunctions = buildPluggableFunctionList();

    m_Generators = buildGeneratorList();

    m_AllAvailableFunctions = buildAllAvailableFunctionList();
}


// =====================================================================
// =====================================================================


BuilderAvailableFunctions::~BuilderAvailableFunctions()
{

}


// =====================================================================
// =====================================================================


BuilderAvailableFunctions * BuilderAvailableFunctions::getInstance()
{
  if(!mp_Singleton)
    mp_Singleton = new BuilderAvailableFunctions();

  return mp_Singleton;
}


// =====================================================================
// =====================================================================


openfluid::machine::ArrayOfModelItemInstance BuilderAvailableFunctions::buildPluggableFunctionList()
{
  openfluid::machine::ArrayOfModelItemInstance PlugContainers = openfluid::machine::PluginManager::getInstance()->getAvailableFunctions();

  for(unsigned int i=0 ; i<PlugContainers.size() ; i++)
    PlugContainers[i]->ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  return PlugContainers;
}


// =====================================================================
// =====================================================================


openfluid::machine::ArrayOfModelItemInstance BuilderAvailableFunctions::buildGeneratorList()
{
  openfluid::machine::ArrayOfModelItemInstance GeneratorContainers;

  GeneratorContainers.push_back(buildGeneratorContainer(openfluid::base::GeneratorDescriptor::Fixed));
  GeneratorContainers.push_back(buildGeneratorContainer(openfluid::base::GeneratorDescriptor::Random));
  GeneratorContainers.push_back(buildGeneratorContainer(openfluid::base::GeneratorDescriptor::Interp));

  return GeneratorContainers;
}


// =====================================================================
// =====================================================================


openfluid::machine::ModelItemInstance * BuilderAvailableFunctions::buildGeneratorContainer(openfluid::base::GeneratorDescriptor::GeneratorMethod GeneratorMethod)
{
  openfluid::base::SignatureHandledDataItem * VarName = new openfluid::base::SignatureHandledDataItem(
      "Varname","","Name of the produced variable","-");
  openfluid::base::SignatureHandledDataItem * UnitClass = new openfluid::base::SignatureHandledDataItem(
      "Unitclass","","Unit class of the produced variable","-");
  openfluid::base::SignatureHandledDataItem * VarSize = new openfluid::base::SignatureHandledDataItem(
      "Varsize","","Optionnal : to produce a vector variable instead of a scalar variable","-");


  openfluid::machine::ModelItemInstance * IInstance = new openfluid::machine::ModelItemInstance();

  openfluid::base::FunctionSignature * Signature = new openfluid::base::FunctionSignature();

  openfluid::machine::Generator * GenInstance;

  IInstance->SDKCompatible = true;
  IInstance->ItemType = openfluid::base::ModelItemDescriptor::Generator;

  Signature->HandledData.FunctionParams.push_back(*VarName);
  Signature->HandledData.FunctionParams.push_back(*UnitClass);
  Signature->HandledData.FunctionParams.push_back(*VarSize);

  switch(GeneratorMethod)
  {
    case openfluid::base::GeneratorDescriptor::Fixed:
      Signature->ID = "(generator) Fixed";
      Signature->Name = "Fixed Generator";
      Signature->Description = "Generates a constant value";

      Signature->HandledData.FunctionParams.push_back(
          openfluid::base::SignatureHandledDataItem(
              "fixedvalue","","Value to produce","-"));

      GenInstance = new openfluid::machine::FixedGenerator();
      break;

    case openfluid::base::GeneratorDescriptor::Random:
      Signature->ID = "(generator) Random";
      Signature->Name = "Random Generator";
      Signature->Description = "Generates a random value in a range";

      Signature->HandledData.FunctionParams.push_back(
          openfluid::base::SignatureHandledDataItem(
              "min","","Lower bound of the random range for the value to produce","-"));

      Signature->HandledData.FunctionParams.push_back(
          openfluid::base::SignatureHandledDataItem(
              "max","","Upper bound of the random range for the value to produce","-"));

      GenInstance = new openfluid::machine::RandomGenerator();
      break;

    case openfluid::base::GeneratorDescriptor::Interp:
      Signature->ID = "(generator) Interp";
      Signature->Name = "Interpolation Generator";
      Signature->Description = "Generates an interpolated value from given data series";

      Signature->HandledData.FunctionParams.push_back(
          openfluid::base::SignatureHandledDataItem(
              "sources","","Data sources filename for the value to produce","-"));

      Signature->HandledData.FunctionParams.push_back(
          openfluid::base::SignatureHandledDataItem(
              "distribution","","Distribution filename for the value to produce","-"));

      GenInstance = new openfluid::machine::InterpGenerator();
      break;

    default:
      throw openfluid::base::OFException("ModelModule::createGeneratorInstance","unknown generator type");
  }

  IInstance->Function = GenInstance;
  IInstance->Signature = Signature;

  return IInstance;
}


// =====================================================================
// =====================================================================


openfluid::machine::ArrayOfModelItemInstance BuilderAvailableFunctions::buildAllAvailableFunctionList()
{
  openfluid::machine::ArrayOfModelItemInstance AllFunctions;

  AllFunctions.insert(AllFunctions.begin(),m_PluggableFunctions.begin(),m_PluggableFunctions.end());
  AllFunctions.insert(AllFunctions.end(),m_Generators.begin(),m_Generators.end());

  return AllFunctions;
}


// =====================================================================
// =====================================================================


void BuilderAvailableFunctions::update()
{
  // TODO: delete items before
  m_PluggableFunctions = buildPluggableFunctionList();
  m_AllAvailableFunctions = buildAllAvailableFunctionList();
  /* TODO: emit signal (for model module to update his parameters eg) */
}


// =====================================================================
// =====================================================================


openfluid::machine::ModelItemInstance * BuilderAvailableFunctions::getPluggableFunction(openfluid::base::FuncID_t Id)
{
  for(unsigned int i=0 ; i<m_PluggableFunctions.size() ; i++)
  {
    if(m_PluggableFunctions[i]->Signature->ID == Id)
      return m_PluggableFunctions[i];
  }

  return 0;
}
