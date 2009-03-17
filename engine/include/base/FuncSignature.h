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
    Signature->setSDKVersion(STRINGIFY(OFELIB_VERSION));


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
#define DECLARE_SIGNATURE_SDKVERSION Signature->setSDKVersion(STRINGIFY(OFELIB_VERSION));

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



// =====================================================================
// =====================================================================


/**
  Macro for declaration of a produced variable on all SUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_SU_PRODUCED_VAR(name,description,unit) \
  Signature->HandledData.ProducedVars.push_back(openfluid::base::SignatureHandledDataItem(name,"SU",description,unit));

/**
  Macro for declaration of an updated variable on all SUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_SU_UPDATED_VAR(name,description,unit) \
  Signature->HandledData.UpdatedVars.push_back(openfluid::base::SignatureHandledDataItem((name),("SU"),description,unit));

/**
  Macro for declaration of a required variable on all SUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_SU_REQUIRED_VAR(name,description,unit) \
  Signature->HandledData.RequiredVars.push_back(openfluid::base::SignatureHandledDataItem((name),("SU"),description,unit));



/**
  Macro for declaration of a required variable on all SUs, using a value produced on a previous time step
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_SU_REQUIRED_PREVVAR(name,description,unit) \
  Signature->HandledData.RequiredPrevVars.push_back(openfluid::base::SignatureHandledDataItem((name),("SU"),description,unit));

/**
  Macro for declaration of an used variable on all SUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_SU_USED_VAR(name,description,unit) \
  Signature->HandledData.UsedVars.push_back(openfluid::base::SignatureHandledDataItem((name),("SU"),description,unit));


/**
  Macro for declaration of an used variable on all SUs, using a value produced on a previous time step
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_SU_USED_PREVVAR(name,description,unit) \
  Signature->HandledData.UsedPrevVars.push_back(openfluid::base::SignatureHandledDataItem((name),("SU"),description,unit));

/**
  Macro for declaration of a required property on all SUs
  \param[in] name name of the property
  \param[in] description description of the property
  \param[in] unit unit of the property. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_SU_REQUIRED_PROPERTY(name,description,unit) \
  Signature->HandledData.RequiredInput.push_back(openfluid::base::SignatureHandledDataItem((name),("SU"),description,unit));

/**
  Macro for declaration of a used property on all SUs
  \param[in] name name of the property
  \param[in] description description of the property
  \param[in] unit unit of the property. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_SU_USED_PROPERTY(name,description,unit) \
  Signature->HandledData.UsedInput.push_back(openfluid::base::SignatureHandledDataItem((name),("SU"),description,unit));


/**
  Macro for declaration of a required initial condition on all SUs
  \param[in] name name of the initial condition
  \param[in] description description of the initial condition
  \param[in] unit unit of the initial condition. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_SU_REQUIRED_INICOND(name,description,unit) \
  Signature->HandledData.RequiredInput.push_back(openfluid::base::SignatureHandledDataItem((name),("SU"),description,unit));

/**
  Macro for declaration of a used initial condition on all SUs
  \param[in] name name of the initial condition
  \param[in] description description of the initial condition
  \param[in] unit unit of the initial condition. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_SU_USED_INICOND(name,description,unit) \
  Signature->HandledData.UsedInput.push_back(openfluid::base::SignatureHandledDataItem((name),("SU"),description,unit));



// =====================================================================
// =====================================================================


/**
  Macro for declaration of a produced variable on all RSs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_RS_PRODUCED_VAR(name,description,unit) \
  Signature->HandledData.ProducedVars.push_back(openfluid::base::SignatureHandledDataItem((name),("RS"),description,unit));

/**
  Macro for declaration of an updated variable on all RSs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_RS_UPDATED_VAR(name,description,unit) \
  Signature->HandledData.UpdatedVars.push_back(openfluid::base::SignatureHandledDataItem((name),("RS"),description,unit));

/**
  Macro for declaration of a required variable on all RSs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_RS_REQUIRED_VAR(name,description,unit) \
  Signature->HandledData.RequiredVars.push_back(openfluid::base::SignatureHandledDataItem((name),("RS"),description,unit));

#define DECLARE_RS_REQUIRED_PREVVAR(name,description,unit) \
  Signature->HandledData.RequiredPrevVars.push_back(openfluid::base::SignatureHandledDataItem((name),("RS"),description,unit));


/**
  Macro for declaration of an used variable on all RSs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_RS_USED_VAR(name,description,unit) \
  Signature->HandledData.UsedVars.push_back(openfluid::base::SignatureHandledDataItem((name),("RS"),description,unit));

#define DECLARE_RS_USED_PREVVAR(name,description,unit) \
  Signature->HandledData.UsedPrevVars.push_back(openfluid::base::SignatureHandledDataItem((name),("RS"),description,unit));


/**
  Macro for declaration of a required property on all RSs
  \param[in] name name of the property
  \param[in] description description of the property
  \param[in] unit unit of the property. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_RS_REQUIRED_PROPERTY(name,description,unit) \
  Signature->HandledData.RequiredInput.push_back(openfluid::base::SignatureHandledDataItem((name),("RS"),description,unit));

#define DECLARE_RS_USED_PROPERTY(name,description,unit) \
  Signature->HandledData.UsedInput.push_back(openfluid::base::SignatureHandledDataItem((name),("RS"),description,unit));

/**
  Macro for declaration of a required initial condition on all RSs
  \param[in] name name of the initial condition
  \param[in] description description of the initial condition
  \param[in] unit unit of the initial condition. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_RS_REQUIRED_INICOND(name,description,unit) \
  Signature->HandledData.RequiredInput.push_back(openfluid::base::SignatureHandledDataItem((name),("RS"),description,unit));

#define DECLARE_RS_USED_INICOND(name,description,unit) \
  Signature->HandledData.UsedInput.push_back(openfluid::base::SignatureHandledDataItem((name),("RS"),description,unit));

// =====================================================================
// =====================================================================


/**
  Macro for declaration of a produced variable on all GUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_GU_PRODUCED_VAR(name,description,unit) \
  Signature->HandledData.ProducedVars.push_back(openfluid::base::SignatureHandledDataItem((name),("GU"),description,unit));

/**
  Macro for declaration of an updated variable on all GUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_GU_UPDATED_VAR(name,description,unit) \
  Signature->HandledData.UpdatedVars.push_back(openfluid::base::SignatureHandledDataItem((name),("GU"),description,unit));

/**
  Macro for declaration of a required variable on all GUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_GU_REQUIRED_VAR(name,description,unit) \
  Signature->HandledData.RequiredVars.push_back(openfluid::base::SignatureHandledDataItem((name),("GU"),description,unit));

/**
  Macro for declaration of a required variable on all GUs, using a value produced on a previous time step
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_GU_REQUIRED_PREVVAR(name,description,unit) \
  Signature->HandledData.RequiredPrevVars.push_back(openfluid::base::SignatureHandledDataItem((name),("GU"),description,unit));


/**
  Macro for declaration of an used variable on all GUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_GU_USED_VAR(name,description,unit) \
  Signature->HandledData.UsedVars.push_back(openfluid::base::SignatureHandledDataItem((name),("GU"),description,unit));

/**
  Macro for declaration of an used variable on all GUs, using a value produced on a previous time step
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_GU_USED_PREVVAR(name,description,unit) \
  Signature->HandledData.UsedPrevVars.push_back(openfluid::base::SignatureHandledDataItem((name),("GU"),description,unit));


/**
  Macro for declaration of a required property on all GUs
  \param[in] name name of the property
  \param[in] description description of the property
  \param[in] unit unit of the property. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_GU_REQUIRED_PROPERTY(name,description,unit) \
  Signature->HandledData.RequiredInput.push_back(openfluid::base::SignatureHandledDataItem((name),("GU"),description,unit));

#define DECLARE_GU_USED_PROPERTY(name,description,unit) \
  Signature->HandledData.UsedInput.push_back(openfluid::base::SignatureHandledDataItem((name),("GU"),description,unit));


/**
  Macro for declaration of a required initial condition on all GUs
  \param[in] name name of the initial condition
  \param[in] description description of the initial condition
  \param[in] unit unit of the initial condition. Could be an empty string if there is no unit

  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_GU_REQUIRED_INICOND(name,description,unit) \
  Signature->HandledData.RequiredInput.push_back(openfluid::base::SignatureHandledDataItem((name),("GU"),description,unit));

#define DECLARE_GU_USED_INICOND(name,description,unit) \
  Signature->HandledData.UsedInput.push_back(openfluid::base::SignatureHandledDataItem((name),("GU"),description,unit));


// =====================================================================
// =====================================================================

#define DECLARE_REQUIRED_EXTRAFILE(name) \
  Signature->HandledData.RequiredExtraFiles.push_back(name);


#define DECLARE_USED_EXTRAFILE(name) \
  Signature->HandledData.UsedExtraFiles.push_back(name);


// =====================================================================
// =====================================================================

#define DECLARE_USED_EVENTS(uclass) Signature->HandledData.UsedEventsOnUnits.push_back(uclass);

/**
  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_USED_SU_EVENTS Signature->HandledData.UsedEventsOnUnits.push_back("SU");

/**
  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_USED_RS_EVENTS Signature->HandledData.UsedEventsOnUnits.push_back("RS");

/**
  \deprecated Deprecated macro, it is available for compatibility only.
*/
#define DECLARE_USED_GU_EVENTS Signature->HandledData.UsedEventsOnUnits.push_back("GU");




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
  Structure for storage of the definition of data handled by the function.
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
  /*

  */
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
  Structure encapsulating the plugin signature,
  returned from the plugin to the host app for registering
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
/*
  \endif
*/



} } // namespaces



#endif
