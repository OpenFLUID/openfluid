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
  @file

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __FUNCTIONSIGNATURE_HPP__
#define __FUNCTIONSIGNATURE_HPP__

#include <boost/regex.hpp>

#include <openfluid/config.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/ware/WareSignature.hpp>


// =====================================================================
// =====================================================================

/**
  Macro for the beginning of definition of signature hook
*/
#define BEGIN_FUNCTION_SIGNATURE(id) \
  openfluid::ware::FunctionSignature* GetWareSignature() \
  { \
    openfluid::ware::FunctionSignature* Signature = new openfluid::ware::FunctionSignature(); \
    Signature->setABIVersion(openfluid::config::FULL_VERSION); \
    Signature->ID = (id);


/**
  Macro for the end of definition of signature hook
*/
#define END_FUNCTION_SIGNATURE \
    return Signature; \
  }


// =====================================================================
// =====================================================================


/**
  Macro for declaration of the function application domain
*/
#define DECLARE_SIGNATURE_DOMAIN(domain) Signature->Domain = domain;

/**
  Macro for declaration of the function application processes
*/
#define DECLARE_SIGNATURE_PROCESS(process) Signature->Process = process;

/**
  Macro for declaration of the function application numerical method
*/
#define DECLARE_SIGNATURE_METHOD(method) Signature->Method = method;


// =====================================================================
// =====================================================================


/**
  Macro for declaration of a function parameter
  @param[in] name name of the parameter
  @param[in] description description of the parameter
  @param[in] unit unit of the parameter. Could be an empty string if there is no unit
*/
#define DECLARE_FUNCTION_PARAM(name,description,unit) \
  Signature->HandledData.FunctionParams.push_back(openfluid::ware::SignatureHandledDataItem((name),(""),description,unit));



// =====================================================================
// =====================================================================


/**
  Macro for declaration of a produced variable
  @param[in] name name of the variable
  @param[in] uclass class of the concerned units
  @param[in] description description of the variable
  @param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_PRODUCED_VAR(name,uclass,description,unit) \
  Signature->HandledData.ProducedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem((name),uclass,description,unit));

/**
  Macro for declaration of an updated variable
  @param[in] name name of the variable
  @param[in] uclass class of the concerned units
  @param[in] description description of the variable
  @param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_UPDATED_VAR(name,uclass,description,unit) \
  Signature->HandledData.UpdatedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem((name),uclass,description,unit));

/**
  Macro for declaration of a required variable
  @param[in] name name of the variable
  @param[in] uclass class of the concerned units
  @param[in] description description of the variable
  @param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_REQUIRED_VAR(name,uclass,description,unit) \
  Signature->HandledData.RequiredVars.push_back(openfluid::ware::SignatureHandledTypedDataItem((name),uclass,description,unit));



/**
  Macro for declaration of a required variable, using a value produced on a previous time step
  @param[in] name name of the variable
  @param[in] uclass class of the concerned units
  @param[in] description description of the variable
  @param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_REQUIRED_PREVVAR(name,uclass,description,unit) \
  Signature->HandledData.RequiredPrevVars.push_back(openfluid::ware::SignatureHandledTypedDataItem((name),uclass,description,unit));

/**
  Macro for declaration of an used variable
  @param[in] name name of the variable
  @param[in] uclass class of the concerned units
  @param[in] description description of the variable
  @param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_USED_VAR(name,uclass,description,unit) \
  Signature->HandledData.UsedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem((name),uclass,description,unit));


/**
  Macro for declaration of an used variable, using a value produced on a previous time step
  @param[in] name name of the variable
  @param[in] uclass class of the concerned units
  @param[in] description description of the variable
  @param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_USED_PREVVAR(name,uclass,description,unit) \
  Signature->HandledData.UsedPrevVars.push_back(openfluid::ware::SignatureHandledTypedDataItem((name),uclass,description,unit));

/**
  Macro for declaration of a required input data
  @param[in] name name of the property
  @param[in] uclass class of the concerned units
  @param[in] description description of the property
  @param[in] unit unit of the property. Could be an empty string if there is no unit
*/
#define DECLARE_REQUIRED_INPUTDATA(name,uclass,description,unit) \
  Signature->HandledData.RequiredInput.push_back(openfluid::ware::SignatureHandledDataItem((name),uclass,description,unit));

/**
  Macro for declaration of a used input data
  @param[in] name name of the property
  @param[in] uclass class of the concerned units
  @param[in] description description of the property
  @param[in] unit unit of the property. Could be an empty string if there is no unit
*/
#define DECLARE_USED_INPUTDATA(name,uclass,description,unit) \
  Signature->HandledData.UsedInput.push_back(openfluid::ware::SignatureHandledDataItem((name),uclass,description,unit));

/**
  Macro for declaration of used events
  @param[in] uclass class of the concerned units
*/
#define DECLARE_USED_EVENTS(uclass) Signature->HandledData.UsedEventsOnUnits.push_back(uclass);



/**
  Macro for declaration of units graph modification
  @param[in] description description of modification

*/
#define DECLARE_UPDATED_UNITSGRAPH(description) Signature->HandledUnitsGraph.UpdatedUnitsGraph = (description);


/**
  Macro for declaration of units class creation or update
  @param[in] uclass name of the created or updated class
  @param[in] description description of the updated class

*/
#define DECLARE_UPDATED_UNITSCLASS(uclass,description) Signature->HandledUnitsGraph.UpdatedUnitsClass.push_back(openfluid::ware::SignatureHandledUnitsClassItem(uclass,description));


/**
  Macro for declaration of required file
  @param[in] name name of the file
*/
#define DECLARE_REQUIRED_EXTRAFILE(name) \
  Signature->HandledData.RequiredExtraFiles.push_back(name);

/**
  Macro for declaration of used file
  @param[in] name name of the file
*/
#define DECLARE_USED_EXTRAFILE(name) \
  Signature->HandledData.UsedExtraFiles.push_back(name);


// =====================================================================
// =====================================================================



namespace openfluid { namespace ware {


typedef std::string FuncMethod_t;

typedef std::string FuncProcess_t;

typedef std::string FuncDomain_t;


// =====================================================================
// =====================================================================


/**
Class for storage of the definition of data handled by the function.
*/
class SignatureHandledDataItem
{
  public:

    std::string DataName;
    openfluid::core::UnitClass_t UnitClass;
    std::string Description;
    std::string DataUnit;

    SignatureHandledDataItem() :
      DataName(""),UnitClass(""),Description(""),DataUnit("") {}

    SignatureHandledDataItem(std::string DName, openfluid::core::UnitClass_t UClass,
        std::string DDescription, std::string DUnit) :
          DataName(DName),UnitClass(UClass),Description(DDescription),DataUnit(DUnit) {}

};

/**
Class for storage of the definition of typed data handled by the function.
*/
class SignatureHandledTypedDataItem : public SignatureHandledDataItem
{

  public:

    static bool getVariableNameAndType(const std::string SourceStr, std::string& VarName, openfluid::core::Value::Type& VarType)
    {
      const boost::basic_regex<char> eVect("^([-.\\w]+)\\[\\]$"); //match "abc[]"
      const boost::basic_regex<char> eNone("[^[\\]][-.\\w]+"); //match "abc"
      const boost::basic_regex<char> eType("^([-.\\w]+)\\[(\\w+)\\]$");  //match "abc[type]"
      boost::smatch Type;

      if(boost::regex_match(SourceStr,Type,eVect) && Type.size() == 2)
      {
        VarName = Type[1];
        VarType = openfluid::core::Value::VECTOR;
        return true;
      }

      if(boost::regex_match(SourceStr,eNone))
      {
        VarName = SourceStr;
        VarType = openfluid::core::Value::NONE;
        return true;
      }

      if(boost::regex_match(SourceStr,Type,eType) && Type.size() == 3)
      {
        VarName = Type[1];
        return openfluid::core::Value::getValueTypeFromString(Type[2],VarType);
      }

      return false;
    }

    openfluid::core::Value::Type DataType;

    SignatureHandledTypedDataItem() :
      DataType(openfluid::core::Value::NONE) {}

    SignatureHandledTypedDataItem(std::string DName, openfluid::core::UnitClass_t UClass,
        std::string DDescription, std::string DUnit)
    {
      UnitClass = UClass;
      Description = DDescription;
      DataUnit = DUnit;

      if(!getVariableNameAndType(DName,DataName,DataType))
        throw openfluid::base::OFException("OpenFLUID framework","SignatureHandledTypedDataItem::SignatureHandledTypedDataItem","Variable " + DName + " is not well formated.");
    }

};


/**
  Class for storage of the definition of spatial units handled by the function.
*/
class SignatureHandledUnitsClassItem
{
  public:

    openfluid::core::UnitClass_t UnitsClass;
    std::string Description;

    SignatureHandledUnitsClassItem() :
      UnitsClass(""),Description("") {}

    SignatureHandledUnitsClassItem(openfluid::core::UnitClass_t UClass,
        std::string DDescription) :
          UnitsClass(UClass),Description(DDescription) {}
};


/**
  Class for storage of the definition of the data handled by the function. This is part of the signature.
*/
class SignatureHandledData
{
  public:

    std::vector<SignatureHandledTypedDataItem> ProducedVars;

    std::vector<SignatureHandledTypedDataItem> UpdatedVars;

    std::vector<SignatureHandledTypedDataItem> RequiredVars;

    std::vector<SignatureHandledTypedDataItem> UsedVars;

    std::vector<SignatureHandledTypedDataItem> RequiredPrevVars;

    std::vector<SignatureHandledTypedDataItem> UsedPrevVars;

    std::vector<SignatureHandledDataItem> FunctionParams;

    std::vector<SignatureHandledDataItem> RequiredInput;

    std::vector<SignatureHandledDataItem> UsedInput;

    std::vector<std::string> RequiredExtraFiles;

    std::vector<std::string> UsedExtraFiles;

    std::vector<openfluid::core::UnitClass_t> UsedEventsOnUnits;


    SignatureHandledData()
    {

    }

};


class SignatureHandledUnitsGraph
{
  public:

    std::string UpdatedUnitsGraph;

    std::vector<SignatureHandledUnitsClassItem> UpdatedUnitsClass;

    SignatureHandledUnitsGraph()
    {
      UpdatedUnitsGraph.clear();
    }
};



/**
  Class encapsulating the plugin signature,
  returned from the plugin to the host app for registering
*/
class DLLEXPORT FunctionSignature : public WareSignature
{

  public:


    /**
    Plugin domain (i.e. hydrology, pop, erosion, ...)
    */
    FuncDomain_t Domain;

    /**
    Plugin simulated process (i.e. surface rainfall-runoff production, ditch infiltration, ...)
    */
    FuncProcess_t Process;

    /**
    Plugin involved method (i.e. morel-seytoux, hayami, ...)
    */
    FuncMethod_t Method;

    /**
    Handled data
    */
    SignatureHandledData HandledData;

    /**
    Handled units graph
    */
    SignatureHandledUnitsGraph HandledUnitsGraph;


    FunctionSignature() : WareSignature(),
      Domain(""),Process(""),Method("")
      {}

};



} } // namespaces



#endif
