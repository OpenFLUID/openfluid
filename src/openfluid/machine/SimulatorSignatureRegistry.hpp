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
 @file SimulatorSignatureRegistry.hpp

 @author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __OPENFLUID_MACHINE_SIMULATORSIGNATUREREGISTRY_HPP__
#define __OPENFLUID_MACHINE_SIMULATORSIGNATUREREGISTRY_HPP__

#include <openfluid/fluidx/ModelItemDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>


namespace openfluid { namespace machine {


class ModelItemSignatureInstance;
class GeneratorSignature;


// =====================================================================
// =====================================================================

class OPENFLUID_API SimulatorSignatureRegistry
{
  public:

    typedef std::map<std::string, openfluid::machine::ModelItemSignatureInstance*> SimSignaturesByName_t;

    typedef std::map<openfluid::fluidx::GeneratorDescriptor::GeneratorMethod, openfluid::machine::ModelItemSignatureInstance*> GenSignaturesByMethod_t;

    typedef std::map<openfluid::fluidx::ModelItemDescriptor::WareType,
        SimSignaturesByName_t> SimSignaturesByTypeByName_t;

  private:

    static SimulatorSignatureRegistry* mp_Instance;

  protected:

    SimSignaturesByTypeByName_t m_SimSignatures;

    GenSignaturesByMethod_t m_GenSignatures;

    SimulatorSignatureRegistry();

    void addAPluggableSignature(
        openfluid::machine::ModelItemSignatureInstance* Signature);

    void addAGeneratorSignature(
        openfluid::machine::ModelItemSignatureInstance* Signature);

  public:

    static SimulatorSignatureRegistry* instance();

    SimSignaturesByTypeByName_t getSimSignatures();

    SimSignaturesByName_t getGeneratorSignatures();

    SimSignaturesByName_t getPluggableSignatures();

    void updatePluggableSignatures();

    static ModelItemSignatureInstance* getEmptyPluggableSignature();

    bool isPluggableSimulatorAvailable(std::string SimulatorID);

    ModelItemSignatureInstance* signatureItemInstance(std::string SimulatorID);

    ModelItemSignatureInstance* signatureItemInstance(openfluid::fluidx::ModelItemDescriptor* Item);

    ModelItemSignatureInstance* signatureItemInstance(openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method);

    void unloadAllSimulators();
};


} } //namespaces

#endif /* __OPENFLUID_MACHINE_SIMULATORSIGNATUREREGISTRY_HPP__ */
