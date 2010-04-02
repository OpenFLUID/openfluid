/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file NewFuncBuddy.cpp
  \brief Implements the "new function" buddy

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "NewFuncBuddy.h"
#include "openfluid-core.h"
#include <sstream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>




NewFunctionBuddy::NewFunctionBuddy() : OpenFLUIDBuddy()
{
  m_RequiredOptionsHelp["funcid"] = "ID of the function";
  m_RequiredOptionsHelp["cppclass"] = "C++ class name of the function";

  m_OtherOptionsHelp["outputdir"] = "path for generated files";
  m_OtherOptionsHelp["authoremail"] = "email(s) of the author(s) of the function";
  m_OtherOptionsHelp["authorname"] = "name(s) of the author(s) of the function";
}


// =====================================================================
// =====================================================================


NewFunctionBuddy::~NewFunctionBuddy()
{

}


// =====================================================================
// =====================================================================

void NewFunctionBuddy::writeFunctionCPP()
{
  std::ostringstream CPPContent;
  std::ostringstream FuncVersionStr;


  unsigned short Year = (boost::posix_time::second_clock::local_time().date().year() % 100);
  unsigned short Month = (unsigned short)(boost::posix_time::second_clock::local_time().date().month());


  FuncVersionStr << Year << ".";
  if (Month < 10) FuncVersionStr << "0";
  FuncVersionStr << Month;


  CPPContent << "/**" << std::endl;
  CPPContent << "  \\file " << m_Options["cppclass"] << ".cpp" << std::endl;
  CPPContent << "  \\brief Implements " << m_Options["cppclass"] << std::endl;
  CPPContent << "" << std::endl;
  CPPContent << "  \\author " << m_Options["authorname"] << " <" << m_Options["authoremail"] << ">" << std::endl;
  CPPContent << " */" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "#include \"openfluid-base.h\"" << std::endl;
  CPPContent << "#include \"openfluid-core.h\"" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "// =====================================================================" << std::endl;
  CPPContent << "// =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "DECLARE_PLUGIN_HOOKS" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "BEGIN_SIGNATURE_HOOK" << std::endl;
  CPPContent << "" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_ID(\"" << m_Options["funcid"] << "\");" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_NAME(\"\");" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_DESCRIPTION(\"\");" << std::endl;
  CPPContent << "" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_VERSION(\""+FuncVersionStr.str()+"\");" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_SDKVERSION;" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);" << std::endl;
  CPPContent << "" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_DOMAIN(\"\");" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_PROCESS(\"\");" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_METHOD(\"\");" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_AUTHORNAME(\"" << m_Options["authorname"] << "\");" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_AUTHOREMAIL(\"" << m_Options["authoremail"] << "\");" << std::endl;
  CPPContent << "" << std::endl;
  CPPContent << "END_SIGNATURE_HOOK" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "// =====================================================================" << std::endl;
  CPPContent << "// =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "class " << m_Options["cppclass"] << " : public openfluid::base::PluggableFunction" << std::endl;
  CPPContent << "{" << std::endl;
  CPPContent << "  private:" << std::endl;
  CPPContent << std::endl;
  CPPContent << "  public:" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    " << m_Options["cppclass"] << "() : PluggableFunction()" << std::endl;
  CPPContent << "    {" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    }" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    ~" << m_Options["cppclass"] << "()" << std::endl;
  CPPContent << "    {" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    }" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    bool initParams(openfluid::core::FuncParamsMap_t Params)" << std::endl;
  CPPContent << "    {" << std::endl;
  CPPContent << "" << std::endl;
  CPPContent << "      return true;" << std::endl;
  CPPContent << "    }" << std::endl;
  CPPContent << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    bool prepareData()" << std::endl;
  CPPContent << "    {" << std::endl;
  CPPContent << std::endl;
  CPPContent << "      return true;" << std::endl;
  CPPContent << "    }" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    bool checkConsistency()" << std::endl;
  CPPContent << "    {" << std::endl;
  CPPContent << std::endl;
  CPPContent << "      return true;" << std::endl;
  CPPContent << "    }" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo)" << std::endl;
  CPPContent << "    {" << std::endl;
  CPPContent << std::endl;
  CPPContent << "      return true;" << std::endl;
  CPPContent << "    }" << std::endl;
  CPPContent << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    bool runStep(const openfluid::base::SimulationStatus* SimStatus)" << std::endl;
  CPPContent << "    {" << std::endl;
  CPPContent << std::endl;
  CPPContent << "      return true;" << std::endl;
  CPPContent << "    }" << std::endl;
  CPPContent << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo)" << std::endl;
  CPPContent << "    {" << std::endl;
  CPPContent << std::endl;
  CPPContent << "      return true;" << std::endl;
  CPPContent << "    }" << std::endl;
  CPPContent << std::endl;
  CPPContent << "};" << std::endl;
  CPPContent << "" << std::endl;
  CPPContent << "// =====================================================================" << std::endl;
  CPPContent << "// =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << "DEFINE_FUNCTION_HOOK(" << m_Options["cppclass"] << ")" << std::endl;
  CPPContent << std::endl;

  std::ofstream OutFile;
  boost::filesystem::path OutFilePath;

  OutFilePath = boost::filesystem::path(m_Options["outputdir"]+"/"+m_Options["cppclass"]+".cpp");
  OutFile.open(OutFilePath.string().c_str(),std::ios::out);
  OutFile << CPPContent.str();
  OutFile.close();

}

// =====================================================================
// =====================================================================


bool NewFunctionBuddy::run()
{

  setOptionIfNotSet("outputdir",boost::filesystem::current_path().string());

  std::cout << "Function ID: " << m_Options["funcid"] << std::endl;
  std::cout << "Function C++ class: " << m_Options["cppclass"] << std::endl;
  std::cout << "Output directory: " << m_Options["outputdir"] << std::endl;
  std::cout << "Function filename: " << m_Options["funcid"] << ".cpp" << std::endl;
  std::cout << "Author name: " << m_Options["authorname"] << std::endl;
  std::cout << "Author email: " << m_Options["authoremail"] << std::endl;

  boost::filesystem::path OutputDirPath(m_Options["outputdir"]);

  if (m_Options["funcid"] == "")
      throw openfluid::base::OFException("kernel","NewFuncBuddy::run()","No function ID");

  if (m_Options["cppclass"] == "")
      throw openfluid::base::OFException("kernel","NewFuncBuddy::run()","No function C++ class");


  if (!boost::filesystem::exists(OutputDirPath))
    throw openfluid::base::OFException("kernel","NewFuncBuddy::run()","Output directory does not exist");



  writeFunctionCPP();

  return true;
}
