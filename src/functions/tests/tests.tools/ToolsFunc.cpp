/**
  \file ToolsFunc.cpp
  \brief Implements ...
*/


#include "openfluid-base.h"
#include "openfluid-core.h"
#include "openfluid-tools.h"
#include <boost/filesystem.hpp>

// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS


// =====================================================================
// =====================================================================


/**

*/
class ToolsFunction : public openfluid::base::PluggableFunction
{
  private:


  public:
    /**
      Constructor
    */
    ToolsFunction();

    /**
      Destructor
    */
    ~ToolsFunction();

    bool initParams(openfluid::core::FuncParamsMap_t Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};



// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(ToolsFunction)


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.tools"));
  DECLARE_SIGNATURE_NAME(("test function for openfluid::tools"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


ToolsFunction::ToolsFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


ToolsFunction::~ToolsFunction()
{


}


// =====================================================================
// =====================================================================


bool ToolsFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{

  return true;
}

// =====================================================================
// =====================================================================


bool ToolsFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool ToolsFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool ToolsFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  openfluid::tools::ColumnTextParser CTParser("#");
  std::string InputDir;
  double DoubleValue;
  std::string StrValue;

  OPENFLUID_GetRunEnvironment("dir.input",&InputDir);
  boost::filesystem::path FileName(InputDir + "/" + "columnfile.txt");

  if (boost::filesystem::exists(FileName))
  {
    CTParser.loadFromFile(FileName.string());

    if (CTParser.getColsCount() != 5)
      OPENFLUID_RaiseError("tests.tools","wrong columns number in " + FileName.string());

    if (!CTParser.getDoubleValue(2,1,&DoubleValue))
      OPENFLUID_RaiseError("tests.tools","error reading double value (3,2) in " + FileName.string());

    if (!openfluid::tools::IsVeryClose(DoubleValue,1.2))
      OPENFLUID_RaiseError("tests.tools","wrong double value for (3,2) in " + FileName.string());

    if (!CTParser.getStringValue(0,0,&StrValue))
      OPENFLUID_RaiseError("tests.tools","error reading string value (0,0) in " + FileName.string());

    if (StrValue != "5")
      OPENFLUID_RaiseError("tests.tools","wrong string value for (0,0) in " + FileName.string());


    if (CTParser.getStringValue(5,5,&StrValue))
      OPENFLUID_RaiseError("tests.tools","error : found value out of range (5,5) in " + FileName.string());
  }
  else OPENFLUID_RaiseError("tests.tools","file " + FileName.string() + " not found!");





  return true;
}

// =====================================================================
// =====================================================================


bool ToolsFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{

  return true;
}

// =====================================================================
// =====================================================================


bool ToolsFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  return true;
}

