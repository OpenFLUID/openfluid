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
  @file WareSrcFactory.hpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/


#ifndef __OPENFLUID_WARESDEV_WARESRCFACTORY_HPP__
#define __OPENFLUID_WARESDEV_WARESRCFACTORY_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/tools/TemplateProcessor.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/ware/ObserverSignature.hpp>
#include <openfluid/builderext/BuilderExtensionSignature.hpp>
#include <openfluid/utils/CppLangHelpers.hpp>


namespace openfluid { namespace waresdev {


class OPENFLUID_API WareSrcFactory
{
  public:

    class Configuration
    {
      public:

        bool WithParamsUI = false;
        
        std::string MainClassName = "WareMain";

        std::string ParamsUIClassName = "WareUIWidget";

        openfluid::tools::TemplateProcessor::Data ExtraTemplateData;
    };


  private:

    static openfluid::tools::TemplateProcessor::Data 
    prepareCommonTemplateData(const openfluid::ware::WareSignature* Signature, const Configuration& Config);

    static openfluid::tools::TemplateProcessor::Data 
    prepareTemplateData(const openfluid::ware::SimulatorSignature& Signature,const Configuration& Config);

    static openfluid::tools::TemplateProcessor::Data 
    prepareTemplateData(const openfluid::ware::ObserverSignature& Signature,const Configuration& Config);

    static openfluid::tools::TemplateProcessor::Data 
    prepareTemplateData(const openfluid::builderext::BuilderExtensionSignature& Signature,const Configuration& Config);

    static std::string getSimulatorInitCode(const openfluid::ware::SimulatorSignature& Signature);

    static std::string getSimulatorRunCode(const openfluid::ware::SimulatorSignature& Signature);

    static std::string getSimulatorSchedulingReturn(const openfluid::ware::SimulatorSignature& Signature);


  public:

    CPPCLASS_DELETE_FIVE(WareSrcFactory);
    
    static std::string createSimulator(const openfluid::ware::SimulatorSignature& Signature, 
                                       const Configuration& Config,
                                       const std::string& ParentPath,
                                       bool WithIDSubDir = true);

    static std::string createObserver(const openfluid::ware::ObserverSignature& Signature, 
                                      const Configuration& Config,
                                      const std::string& ParentPath,
                                      bool WithIDSubDir = true);

    static std::string createBuilderext(const openfluid::builderext::BuilderExtensionSignature& Signature, 
                                        const Configuration& Config,
                                        const std::string& ParentPath,
                                        bool WithIDSubDir = true);

};


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESRCFACTORY_HPP__ */
