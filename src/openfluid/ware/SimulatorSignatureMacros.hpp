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
  @file SimulatorSignatureMacros.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_WARE_SIMULATORSIGNATUREMACROS_HPP__
#define __OPENFLUID_WARE_SIMULATORSIGNATUREMACROS_HPP__


#include <openfluid/ware/WareSignatureMacros.hpp>


/**
  Macro for the beginning of definition of signature hook

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature"],
    "title" : "Insert signature block",
    "text" : "BEGIN_SIMULATOR_SIGNATURE(\"%%SEL_START%%sim.id%%SEL_END%%\")\n\nEND_SIMULATOR_SIGNATURE"
  }
  @endcond
*/
#define BEGIN_SIMULATOR_SIGNATURE(id) \
  extern "C" { \
    OPENFLUID_PLUGIN openfluid::ware::SimulatorSignature* WARESIGNATURE_PROC_DECL() \
    { \
      openfluid::ware::SimulatorSignature* Signature = new openfluid::ware::SimulatorSignature(); \
      Signature->ID = (id); \
      Signature->BuildInfo.SDKVersion = openfluid::config::VERSION_FULL; \
      Signature->BuildInfo.BuildType = (WAREBUILD_BUILD_TYPE); \
      Signature->BuildInfo.CompilerID = (WAREBUILD_COMPILER_ID); \
      Signature->BuildInfo.CompilerVersion = (WAREBUILD_COMPILER_VERSION); \
      Signature->BuildInfo.CompilationFlags = (WAREBUILD_COMPILATION_FLAGS);

/**
  Macro for the end of definition of signature hook
*/
#define END_SIMULATOR_SIGNATURE \
      return Signature; \
    } \
  }


// =====================================================================
// =====================================================================


/**
  Macro for declaration of the simulator application domain

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Scientific context"],
    "title" : "Declare application domain",
    "text" : "DECLARE_DOMAIN(\"%%SEL_START%%description%%SEL_END%%\")"
  }
  @endcond
*/
#define DECLARE_DOMAIN(domain) \
  Signature->Domain = domain;

/**
  Macro for declaration of the simulator application processes

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Scientific context"],
    "title" : "Declare modeled process(es)",
    "text" : "DECLARE_PROCESS(\"%%SEL_START%%description%%SEL_END%%\")"
  }
  @endcond
*/
#define DECLARE_PROCESS(process) \
  Signature->Process = process;

/**
  Macro for declaration of the simulator application numerical method

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Scientific context"],
    "title" : "Declare modeling method",
    "text" : "DECLARE_METHOD(\"%%SEL_START%%description%%SEL_END%%\")"
  }
  @endcond
*/
#define DECLARE_METHOD(method) \
  Signature->Method = method;


// =====================================================================
// =====================================================================


/**
  Macro for declaration of an used simulator parameter
  @param[in] name name of the parameter
  @param[in] description description of the parameter
  @param[in] unit unit of the parameter. Could be an empty string if there is no unit

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Parameters"],
    "title" : "Declare used parameter",
    "text" : "DECLARE_USED_PARAMETER(\"%%SEL_START%%paramname%%SEL_END%%\",\"description\",\"SI unit\")"
  }
  @endcond
*/
#define DECLARE_USED_PARAMETER(name,description,unit) \
  Signature->HandledData.UsedParams\
  .push_back(openfluid::ware::SignatureDataItem((name),description,unit));

/**
  @deprecated Since version 2.1.0. Use #DECLARE_USED_PARAMETER instead
*/
#define DECLARE_SIMULATOR_PARAM(name,description,unit) DECLARE_USED_PARAMETER(name,description,unit)


/**
  Macro for declaration of a required simulator parameter
  @param[in] name name of the parameter
  @param[in] description description of the parameter
  @param[in] unit unit of the parameter. Could be an empty string if there is no unit

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Parameters"],
    "title" : "Declare required parameter",
    "text" : "DECLARE_REQUIRED_PARAMETER(\"%%SEL_START%%paramname%%SEL_END%%\",\"description\",\"SI unit\")"
  }
  @endcond
*/
#define DECLARE_REQUIRED_PARAMETER(name,description,unit) \
  Signature->HandledData.RequiredParams\
  .push_back(openfluid::ware::SignatureDataItem((name),description,unit));


// =====================================================================
// =====================================================================


/**
  Macro for declaration of a produced variable
  @param[in] name name of the variable
  @param[in] uclass class of the concerned units
  @param[in] description description of the variable
  @param[in] unit unit of the variable. Could be an empty string if there is no unit

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Variables"],
    "title" : "Declare produced variable",
    "text" : ["DECLARE_PRODUCED_VARIABLE(\"%%SEL_START%%var.name%%SEL_END%%\",",
              "\"unitsclass\",\"description\",\"SI unit\")"]
  }
  @endcond
*/
#define DECLARE_PRODUCED_VARIABLE(name,uclass,description,unit) \
  Signature->HandledData.ProducedVars\
  .push_back(openfluid::ware::SignatureTypedSpatialDataItem((name),uclass,description,unit));

/**
  @deprecated Since version 2.1.0. Use #DECLARE_PRODUCED_VARIABLE instead
*/
#define DECLARE_PRODUCED_VAR(name,uclass,description,unit) DECLARE_PRODUCED_VARIABLE(name,uclass,description,unit)


/**
  Macro for declaration of an updated variable
  @param[in] name name of the variable
  @param[in] uclass class of the concerned units
  @param[in] description description of the variable
  @param[in] unit unit of the variable. Could be an empty string if there is no unit

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Variables"],
    "title" : "Declare updated variable",
    "text" : "DECLARE_UPDATED_VARIABLE(\"%%SEL_START%%var.name%%SEL_END%%\",\"unitsclass\",\"description\",\"SI unit\")"
  }
  @endcond
*/
#define DECLARE_UPDATED_VARIABLE(name,uclass,description,unit) \
  Signature->HandledData.UpdatedVars\
  .push_back(openfluid::ware::SignatureTypedSpatialDataItem((name),uclass,description,unit));

/**
  @deprecated Since version 2.1.0. Use #DECLARE_UPDATED_VARIABLE instead
*/
#define DECLARE_UPDATED_VAR(name,uclass,description,unit) DECLARE_UPDATED_VARIABLE(name,uclass,description,unit)


/**
  Macro for declaration of a required variable
  @param[in] name name of the variable
  @param[in] uclass class of the concerned units
  @param[in] description description of the variable
  @param[in] unit unit of the variable. Could be an empty string if there is no unit

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Variables"],
    "title" : "Declare required variable",
    "text" : ["DECLARE_REQUIRED_VARIABLE(\"%%SEL_START%%var.name%%SEL_END%%\",",
              "\"unitsclass\",\"description\",\"SI unit\")"]
  }
  @endcond
*/
#define DECLARE_REQUIRED_VARIABLE(name,uclass,description,unit) \
  Signature->HandledData.RequiredVars\
  .push_back(openfluid::ware::SignatureTypedSpatialDataItem((name),uclass,description,unit));

/**
  @deprecated Since version 2.1.0. Use #DECLARE_REQUIRED_VARIABLE instead
*/
#define DECLARE_REQUIRED_VAR(name,uclass,description,unit) DECLARE_REQUIRED_VARIABLE(name,uclass,description,unit)


/**
  Macro for declaration of an used variable
  @param[in] name name of the variable
  @param[in] uclass class of the concerned units
  @param[in] description description of the variable
  @param[in] unit unit of the variable. Could be an empty string if there is no unit

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Variables"],
    "title" : "Declare used variable",
    "text" : "DECLARE_USED_VARIABLE(\"%%SEL_START%%var.name%%SEL_END%%\",\"unitsclass\",\"description\",\"SI unit\")"
  }
  @endcond
*/
#define DECLARE_USED_VARIABLE(name,uclass,description,unit) \
  Signature->HandledData.UsedVars\
  .push_back(openfluid::ware::SignatureTypedSpatialDataItem((name),uclass,description,unit));

/**
  @deprecated Since version 2.1.0. Use #DECLARE_USED_VARIABLE instead
*/
#define DECLARE_USED_VAR(name,uclass,description,unit) DECLARE_USED_VARIABLE(name,uclass,description,unit)


/**
  Macro for declaration of a produced attribute
  @param[in] name name of the attribute
  @param[in] uclass class of the concerned units
  @param[in] description description of the attribute
  @param[in] unit unit of the attribute. Could be an empty string if there is no unit

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Attributes"],
    "title" : "Declare produced attribute",
    "text" : "DECLARE_PRODUCED_ATTRIBUTE(\"%%SEL_START%%name%%SEL_END%%\",\"unitsclass\",\"description\",\"SI unit\")"
  }
  @endcond
*/
#define DECLARE_PRODUCED_ATTRIBUTE(name,uclass,description,unit) \
  Signature->HandledData.ProducedAttribute\
  .push_back(openfluid::ware::SignatureSpatialDataItem((name),uclass,description,unit));


/**
  Macro for declaration of a required attribute
  @param[in] name name of the attribute
  @param[in] uclass class of the concerned units
  @param[in] description description of the attribute
  @param[in] unit unit of the attribute. Could be an empty string if there is no unit

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Attributes"],
    "title" : "Declare required attribute",
    "text" : "DECLARE_REQUIRED_ATTRIBUTE(\"%%SEL_START%%name%%SEL_END%%\",\"unitsclass\",\"description\",\"SI unit\")"
  }
  @endcond
*/
#define DECLARE_REQUIRED_ATTRIBUTE(name,uclass,description,unit) \
  Signature->HandledData.RequiredAttribute\
  .push_back(openfluid::ware::SignatureSpatialDataItem((name),uclass,description,unit));


/**
  Macro for declaration of a used attribute
  @param[in] name name of the attribute
  @param[in] uclass class of the concerned units
  @param[in] description description of the attribute
  @param[in] unit unit of the attribute. Could be an empty string if there is no unit

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Attributes"],
    "title" : "Declare used attribute",
    "text" : "DECLARE_USED_ATTRIBUTE(\"%%SEL_START%%name%%SEL_END%%\",\"unitsclass\",\"description\",\"SI unit\")"
  }
  @endcond
*/
#define DECLARE_USED_ATTRIBUTE(name,uclass,description,unit) \
  Signature->HandledData.UsedAttribute\
  .push_back(openfluid::ware::SignatureSpatialDataItem((name),uclass,description,unit));


/**
  Macro for declaration of used events
  @param[in] uclass class of the concerned units

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Events"],
    "title" : "Declare used events",
    "text" : "DECLARE_USED_EVENTS(\"%%SEL_START%%unitsclass%%SEL_END%%\",\"description\")"
  }
  @endcond
*/
#define DECLARE_USED_EVENTS(uclass) Signature->HandledData.UsedEventsOnUnits.push_back(uclass);


/**
  Macro for declaration of units graph modification
  @param[in] description description of modification

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Spatial structure"],
    "title" : "Declare updated units graph",
    "text" : "DECLARE_UPDATED_UNITSGRAPH(\"%%SEL_START%%description%%SEL_END%%\")"
  }
  @endcond
*/
#define DECLARE_UPDATED_UNITSGRAPH(description) Signature->HandledUnitsGraph.UpdatedUnitsGraph = (description);


/**
  Macro for declaration of units class creation or update
  @param[in] uclass name of the created or updated class
  @param[in] description description of the updated class

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Spatial structure"],
    "title" : "Declare updated units class",
    "text" : "DECLARE_UPDATED_UNITSCLASS(\"%%SEL_START%%unitsclass%%SEL_END%%\",\"description\")"
  }
  @endcond
*/
#define DECLARE_UPDATED_UNITSCLASS(uclass,description) \
    Signature->HandledUnitsGraph.UpdatedUnitsClass\
    .push_back(openfluid::ware::SignatureUnitsClassItem(uclass,description));


/**
  Macro for declaration of required file
  @param[in] name name of the file

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Extra files"],
    "title" : "Declare required extra file",
    "text" : "DECLARE_REQUIRED_EXTRAFILE(\"%%SEL_START%%filename%%SEL_END%%\")"
  }
  @endcond
*/
#define DECLARE_REQUIRED_EXTRAFILE(name) \
  Signature->HandledData.RequiredExtraFiles.push_back(name);


/**
  Macro for declaration of used file
  @param[in] name name of the file

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Extra files"],
    "title" : "Declare used extra file",
    "text" : "DECLARE_USED_EXTRAFILE(\"%%SEL_START%%filename%%SEL_END%%\")"
  }
  @endcond
*/
#define DECLARE_USED_EXTRAFILE(name) \
  Signature->HandledData.UsedExtraFiles.push_back(name);


/**
  Macro for declaration of time scheduling as undefined

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Scheduling"],
    "title" : "Declare undefined scheduling",
    "text" : "DECLARE_SCHEDULING_UNDEFINED"
  }
  @endcond
*/
#define DECLARE_SCHEDULING_UNDEFINED \
  Signature->TimeScheduling.setAsUndefined();


/**
  Macro for declaration of time scheduling as default delta T

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Scheduling"],
    "title" : "Declare default deltaT scheduling",
    "text" : "DECLARE_SCHEDULING_DEFAULT"
  }
  @endcond
*/
#define DECLARE_SCHEDULING_DEFAULT \
  Signature->TimeScheduling.setAsDefaultDeltaT();


/**
  Macro for declaration of fixed time scheduling
  @param[in] deltat fixed time scheduling value

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Scheduling"],
    "title" : "Declare fixed scheduling",
    "text" : "DECLARE_SCHEDULING_FIXED(%%SEL_START%%duration%%SEL_END%%)"
  }
  @endcond
*/
#define DECLARE_SCHEDULING_FIXED(deltat) \
  Signature->TimeScheduling.setAsFixed(deltat);


/**
  Macro for declaration of range of time scheduling
  @param[in] min minimal time scheduling value
  @param[in] max maximal time scheduling value

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR"],
    "menupath" : ["Signature", "Scheduling"],
    "title" : "Declare scheduling in range",
    "text" : "DECLARE_SCHEDULING_RANGE(%%SEL_START%%min%%SEL_END%%,max)"
  }
  @endcond
*/
#define DECLARE_SCHEDULING_RANGE(min,max) \
  Signature->TimeScheduling.setAsRange(min,max);


#endif /* __OPENFLUID_WARE_SIMULATORSIGNATUREMACROS_HPP__ */
