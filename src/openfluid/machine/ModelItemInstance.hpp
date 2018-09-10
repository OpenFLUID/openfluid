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
  @file ModelItemInstance.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_MACHINE_MODELITEMINSTANCE_HPP__
#define __OPENFLUID_MACHINE_MODELITEMINSTANCE_HPP__


#include <string>
#include <memory>

#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/machine/WareSignatureInstance.hpp>


namespace openfluid { namespace machine {


class OPENFLUID_API GeneratorExtraInfo
{
  public:

    openfluid::core::VariableName_t VariableName;
    openfluid::core::UnitsClass_t UnitsClass;
    unsigned int VariableSize;
    openfluid::fluidx::GeneratorDescriptor::GeneratorMethod GeneratorMethod;

    GeneratorExtraInfo();
};


// =====================================================================
// =====================================================================


class OPENFLUID_API ModelItemSignatureInstance : public WareSignatureInstance
{
  public:

    // Declared as a classic pointer because of DLL cross-boundaries hell on Windows systems
    // TODO should be replaced by a factory for memory management across DLLs
    openfluid::ware::SimulatorSignature* Signature;

    std::unique_ptr<GeneratorExtraInfo> GeneratorInfo;

    bool Ghost;


    ModelItemSignatureInstance();

};


// =====================================================================
// =====================================================================


class OPENFLUID_API ModelItemInstance : public ModelItemSignatureInstance
{
  public:

    openfluid::ware::WareParams_t Params;

    std::unique_ptr<openfluid::ware::PluggableSimulator> Body;

    unsigned int OriginalPosition;


    ModelItemInstance();
};


} } //namespaces


#endif /* __OPENFLUID_MACHINE_MODELITEMINSTANCE_HPP__ */

