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

#ifndef __SIMULATORSIGNATURE_HPP__
#define __SIMULATORSIGNATURE_HPP__

#include <boost/regex.hpp>

#include <openfluid/config.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/ware/WareSignature.hpp>



// =====================================================================
// =====================================================================

/**
  Macro for the beginning of definition of signature hook
*/
#define BEGIN_SIMULATOR_SIGNATURE(id) \
  openfluid::ware::SimulatorSignature* GetWareSignature() \
  { \
    openfluid::ware::SimulatorSignature* Signature = new openfluid::ware::SimulatorSignature(); \
    Signature->setABIVersion(openfluid::config::FULL_VERSION); \
    Signature->ID = (id);


/**
  Macro for the end of definition of signature hook
*/
#define END_SIMULATOR_SIGNATURE \
    return Signature; \
  }


// =====================================================================
// =====================================================================


/**
  Macro for declaration of the simulator application domain
*/
#define DECLARE_SIGNATURE_DOMAIN(domain) Signature->Domain = domain;

/**
  Macro for declaration of the simulator application processes
*/
#define DECLARE_SIGNATURE_PROCESS(process) Signature->Process = process;

/**
  Macro for declaration of the simulator application numerical method
*/
#define DECLARE_SIGNATURE_METHOD(method) Signature->Method = method;


// =====================================================================
// =====================================================================


/**
  Macro for declaration of a simulator parameter
  @param[in] name name of the parameter
  @param[in] description description of the parameter
  @param[in] unit unit of the parameter. Could be an empty string if there is no unit
*/
#define DECLARE_SIMULATOR_PARAM(name,description,unit) \
  Signature->HandledData.SimulatorParams.push_back(openfluid::ware::SignatureHandledDataItem((name),(""),description,unit));



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
  Macro for declaration of an used variable
  @param[in] name name of the variable
  @param[in] uclass class of the concerned units
  @param[in] description description of the variable
  @param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_USED_VAR(name,uclass,description,unit) \
  Signature->HandledData.UsedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem((name),uclass,description,unit));


/**
  Macro for declaration of a produced input data
  @param[in] name name of the input data
  @param[in] uclass class of the concerned units
  @param[in] description description of the input data
  @param[in] unit unit of the input data. Could be an empty string if there is no unit
*/
#define DECLARE_PRODUCED_INPUTDATA(name,uclass,description,unit) \
  Signature->HandledData.ProducedInputdata.push_back(openfluid::ware::SignatureHandledDataItem((name),uclass,description,unit));

/**
  Macro for declaration of a required input data
  @param[in] name name of the input data
  @param[in] uclass class of the concerned units
  @param[in] description description of the input data
  @param[in] unit unit of the input data. Could be an empty string if there is no unit
*/
#define DECLARE_REQUIRED_INPUTDATA(name,uclass,description,unit) \
  Signature->HandledData.RequiredInputdata.push_back(openfluid::ware::SignatureHandledDataItem((name),uclass,description,unit));

/**
  Macro for declaration of a used input data
  @param[in] name name of the input data
  @param[in] uclass class of the concerned units
  @param[in] description description of the input data
  @param[in] unit unit of the input data. Could be an empty string if there is no unit
*/
#define DECLARE_USED_INPUTDATA(name,uclass,description,unit) \
  Signature->HandledData.UsedInputdata.push_back(openfluid::ware::SignatureHandledDataItem((name),uclass,description,unit));

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

/**
  Macro for declaration of time scheduling as undefined
*/
#define DECLARE_SCHEDULING_UNDEFINED() \
  Signature->TimeScheduling.setAsUndefined();

/**
  Macro for declaration of time scheduling as default delta T
*/
#define DECLARE_SCHEDULING_DEFAULT() \
  Signature->TimeScheduling.setAsDefaultDeltaT();

/**
  Macro for declaration of fixed time scheduling
  @param[in] deltat fixed time scheduling value
*/
#define DECLARE_SCHEDULING_FIXED(deltat) \
  Signature->TimeScheduling.setAsFixed(deltat);

/**
  Macro for declaration of range of time scheduling
  @param[in] min minimal time scheduling value
  @param[in] max maximal time scheduling value
*/
#define DECLARE_SCHEDULING_RANGE(min,max) \
  Signature->TimeScheduling.setAsRange(min,max);


// =====================================================================
// =====================================================================



namespace openfluid { namespace ware {


typedef std::string SimMethod_t;

typedef std::string SimProcess_t;

typedef std::string SimDomain_t;


// =====================================================================
// =====================================================================


/**
Class for storage of the definition of data handled by the simulator.
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
Class for storage of the definition of typed data handled by the simulator.
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
  Class for storage of the definition of spatial units handled by the simulator.
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
  Class for storage of the definition of the data handled by the simulator. This is part of the signature.
*/
class SignatureHandledData
{
  public:

    std::vector<SignatureHandledTypedDataItem> ProducedVars;

    std::vector<SignatureHandledTypedDataItem> UpdatedVars;

    std::vector<SignatureHandledTypedDataItem> RequiredVars;

    std::vector<SignatureHandledTypedDataItem> UsedVars;

    std::vector<SignatureHandledDataItem> SimulatorParams;

    std::vector<SignatureHandledDataItem> ProducedInputdata;

    std::vector<SignatureHandledDataItem> RequiredInputdata;

    std::vector<SignatureHandledDataItem> UsedInputdata;

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


class SignatureTimeScheduling
{
  public:

    enum SchedulingType { UNDEFINED, DEFAULT, FIXED, RANGE };

    SchedulingType Type;

    openfluid::core::Duration_t Min;

    openfluid::core::Duration_t Max;

    SignatureTimeScheduling():
      Type(UNDEFINED), Min(0), Max(0)
    { }

    void setAsUndefined()
    {
      Type = UNDEFINED;
      Min = 0;
      Max = 0;
    }

    void setAsDefaultDeltaT()
    {
      Type = DEFAULT;
      Min = 0;
      Max = 0;
    }

    void setAsFixed(openfluid::core::Duration_t Val)
    {
      Type = FIXED;
      Min = Val;
      Max = Val;
    }

    void setAsRange(openfluid::core::Duration_t MinVal, openfluid::core::Duration_t MaxVal)
    {
      Type = RANGE;
      Min = MinVal;
      Max = MaxVal;
    }

};


/**
  Class encapsulating the plugin signature,
  returned from the plugin to the host app for registering
*/
class DLLEXPORT SimulatorSignature : public WareSignature
{

  public:


    /**
    Plugin domain (i.e. hydrology, pop, erosion, ...)
    */
    SimDomain_t Domain;

    /**
    Plugin simulated process (i.e. surface rainfall-runoff production, ditch infiltration, ...)
    */
    SimProcess_t Process;

    /**
    Plugin involved method (i.e. morel-seytoux, hayami, ...)
    */
    SimMethod_t Method;

    /**
    Handled data
    */
    SignatureHandledData HandledData;

    /**
    Handled units graph
    */
    SignatureHandledUnitsGraph HandledUnitsGraph;

    /**
      Time scheduling
    */
    SignatureTimeScheduling TimeScheduling;

    SimulatorSignature() : WareSignature(),
      Domain(""),Process(""),Method("")
      {}

};



} } // namespaces



#endif
