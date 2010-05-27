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
  \file FuncSignature.h
  \brief Header of function signature definition

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __FUNCSIGNATURE_H__
#define __FUNCSIGNATURE_H__



// =====================================================================
// =====================================================================

/**
  Macro for the beginning of definition of signature hook
*/
#define BEGIN_SIGNATURE_HOOK \
  openfluid::base::FunctionSignature* GetPlugSignature() \
  { \
    openfluid::base::FunctionSignature* Signature = new openfluid::base::FunctionSignature(); \
    Signature->setSDKVersion(STRINGIFY(OPENFLUID_VERSION));


/**
  Macro for the end of definition of signature hook
*/
#define END_SIGNATURE_HOOK \
    return Signature; \
  }


// =====================================================================
// =====================================================================


/**
  Macro for declaration of the function ID
*/
#define DECLARE_SIGNATURE_ID(id) Signature->ID = id;

/**
  Macro for declaration of the function name
*/
#define DECLARE_SIGNATURE_NAME(name) Signature->Name = name;

/**
  Macro for declaration of the function description
*/
#define DECLARE_SIGNATURE_DESCRIPTION(desc) Signature->Description = desc;

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

/**
  Macro for declaration of the function author name
*/
#define DECLARE_SIGNATURE_AUTHORNAME(name) Signature->Author = name;

/**
  Macro for declaration of the function author email
*/
#define DECLARE_SIGNATURE_AUTHOREMAIL(email) Signature->AuthorEmail = email;

/**
  Macro for declaration of the function version
*/
#define DECLARE_SIGNATURE_VERSION(version) Signature->Version = version;

/**
  Macro for declaration of the function status
*/
#define DECLARE_SIGNATURE_STATUS(status) Signature->Status = status;

/**
  Macro for declaration of SDK version used to build the function
*/
#define DECLARE_SIGNATURE_SDKVERSION Signature->setSDKVersion(STRINGIFY(OPENFLUID_VERSION));

// =====================================================================
// =====================================================================


/**
  Macro for declaration of a function parameter
  \param[in] name name of the parameter
  \param[in] description description of the parameter
  \param[in] unit unit of the parameter. Could be an empty string if there is no unit
*/
#define DECLARE_FUNCTION_PARAM(name,description,unit) \
  Signature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledDataItem((name),(""),description,unit));



// =====================================================================
// =====================================================================


/**
  Macro for declaration of a produced variable
  \param[in] name name of the variable
  \param[in] uclass class of the concerned units
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_PRODUCED_VAR(name,uclass,description,unit) \
  Signature->HandledData.ProducedVars.push_back(openfluid::base::SignatureHandledDataItem((name),uclass,description,unit));

/**
  Macro for declaration of an updated variable
  \param[in] name name of the variable
  \param[in] uclass class of the concerned units
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_UPDATED_VAR(name,uclass,description,unit) \
  Signature->HandledData.UpdatedVars.push_back(openfluid::base::SignatureHandledDataItem((name),uclass,description,unit));

/**
  Macro for declaration of a required variable
  \param[in] name name of the variable
  \param[in] uclass class of the concerned units
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_REQUIRED_VAR(name,uclass,description,unit) \
  Signature->HandledData.RequiredVars.push_back(openfluid::base::SignatureHandledDataItem((name),uclass,description,unit));



/**
  Macro for declaration of a required variable, using a value produced on a previous time step
  \param[in] name name of the variable
  \param[in] uclass class of the concerned units
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_REQUIRED_PREVVAR(name,uclass,description,unit) \
  Signature->HandledData.RequiredPrevVars.push_back(openfluid::base::SignatureHandledDataItem((name),uclass,description,unit));

/**
  Macro for declaration of an used variable
  \param[in] name name of the variable
  \param[in] uclass class of the concerned units
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_USED_VAR(name,uclass,description,unit) \
  Signature->HandledData.UsedVars.push_back(openfluid::base::SignatureHandledDataItem((name),uclass,description,unit));


/**
  Macro for declaration of an used variable, using a value produced on a previous time step
  \param[in] name name of the variable
  \param[in] uclass class of the concerned units
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_USED_PREVVAR(name,uclass,description,unit) \
  Signature->HandledData.UsedPrevVars.push_back(openfluid::base::SignatureHandledDataItem((name),uclass,description,unit));

/**
  Macro for declaration of a required input data
  \param[in] name name of the property
  \param[in] uclass class of the concerned units
  \param[in] description description of the property
  \param[in] unit unit of the property. Could be an empty string if there is no unit
*/
#define DECLARE_REQUIRED_INPUTDATA(name,uclass,description,unit) \
  Signature->HandledData.RequiredInput.push_back(openfluid::base::SignatureHandledDataItem((name),uclass,description,unit));

/**
  Macro for declaration of a used input data
  \param[in] name name of the property
  \param[in] uclass class of the concerned units
  \param[in] description description of the property
  \param[in] unit unit of the property. Could be an empty string if there is no unit
*/
#define DECLARE_USED_INPUTDATA(name,uclass,description,unit) \
  Signature->HandledData.UsedInput.push_back(openfluid::base::SignatureHandledDataItem((name),uclass,description,unit));

/**
  Macro for declaration of used events
  \param[in] uclass class of the concerned units
*/
#define DECLARE_USED_EVENTS(uclass) Signature->HandledData.UsedEventsOnUnits.push_back(uclass);

/**
  Macro for declaration of required file
  \param[in] name name of the file
*/
#define DECLARE_REQUIRED_EXTRAFILE(name) \
  Signature->HandledData.RequiredExtraFiles.push_back(name);

/**
  Macro for declaration of used file
  \param[in] name name of the file
*/
#define DECLARE_USED_EXTRAFILE(name) \
  Signature->HandledData.UsedExtraFiles.push_back(name);


// =====================================================================
// =====================================================================



namespace openfluid { namespace base {


typedef std::string FuncID_t;
typedef std::string FuncName_t;
typedef std::string FuncVersion_t;
typedef std::string FuncMethod_t;
typedef std::string FuncProcess_t;
typedef std::string FuncDomain_t;


/**
  Function status
*/
enum FuncStatus_t
{
  /**
    Experimental status, for testing only
  */
  EXPERIMENTAL,

  /**
    Beta status, on the road to a stable status
  */
  BETA,

  /**
    Stable status
  */
  STABLE
};


// =====================================================================
// =====================================================================
/**
  \if DOCFORDEV
  Structure for storage of the definition of data handled by the function.
  \endif
*/
struct SignatureHandledDataItem
{
  std::string DataName;
  openfluid::core::UnitClass_t UnitClass; // "SU", "RS", "GU", or empty if none
  std::string Description;
  std::string DataUnit; // empty if none, "?" if unknown

  SignatureHandledDataItem()
  {
    DataName = "";
    UnitClass = "";
    Description = "";
    DataUnit = "";
  }

  SignatureHandledDataItem(std::string DName, openfluid::core::UnitClass_t UClass,
                           std::string DDescription, std::string DUnit)
  {
    DataName = DName;
    UnitClass = UClass;
    Description = DDescription;
    DataUnit = DUnit;
  }

};

/**
  \if DOCFORDEV
  Structure for storage of the definition of the data handled by the function. This is part of the signature.
  \endif
*/
struct SignatureHandledData
{
  std::vector<SignatureHandledDataItem> ProducedVars;

  std::vector<SignatureHandledDataItem> UpdatedVars;

  std::vector<SignatureHandledDataItem> RequiredVars;

  std::vector<SignatureHandledDataItem> UsedVars;

  std::vector<SignatureHandledDataItem> RequiredPrevVars;

  std::vector<SignatureHandledDataItem> UsedPrevVars;

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



/**
  \if DOCFORDEV
  Structure encapsulating the plugin signature,
  returned from the plugin to the host app for registering
  \endif
*/
struct FunctionSignature
{

  /**
    Function ID
  */
  FuncID_t ID;

  /**
    Plugin name
  */
  FuncName_t Name;

  /**
    Plugin Description
  */
  std::string Description;

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
    Plug-in version number
  */
  FuncVersion_t Version;

  /**
    Development status
  */
  FuncStatus_t Status;

  /**
    SDK version number used to build the function
  */
  std::string SDKVersion;

  /**
    Author's name
  */
  std::string Author;

  /**
    Author's email
  */
  std::string AuthorEmail;

  /**
    Handled data
  */
  SignatureHandledData HandledData;

  FunctionSignature()
  {
    ID = "";
    Name = "";
    Description = "";
    Domain = "";
    Process = "";
    Method = "";
    Author = "";
    AuthorEmail = "";
    Version = "";
    Status = EXPERIMENTAL;
    SDKVersion = "";
  }

  void setSDKVersion(FuncVersion_t Version)
  {
    SDKVersion = Version;
  }

};



} } // namespaces



#endif
