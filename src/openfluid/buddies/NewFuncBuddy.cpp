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
  @brief Implements the "new function" buddy

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/buddies/NewFuncBuddy.hpp>

#include <sstream>
#include <fstream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <openfluid/base/OFException.hpp>


namespace openfluid { namespace buddies {


NewFunctionBuddy::NewFunctionBuddy(openfluid::buddies::BuddiesListener* Listener) :
                  OpenFLUIDBuddy(Listener)
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
  CPPContent << "  @file" << std::endl;
  CPPContent << "  @brief Implements " << m_Options["cppclass"] << std::endl;
  CPPContent << "" << std::endl;
  CPPContent << "  @author " << m_Options["authorname"] << " <" << m_Options["authoremail"] << ">" << std::endl;
  CPPContent << " */" << std::endl;
  CPPContent << std::endl;
  CPPContent << "/*" << std::endl;
  CPPContent << "<func2doc>" << std::endl;
  CPPContent << std::endl;
  CPPContent << "</func2doc>" << std::endl;
  CPPContent << "*/" << std::endl;
  CPPContent << std::endl;
  CPPContent << "#include <openfluid/ware/PluggableFunction.hpp>" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "// =====================================================================" << std::endl;
  CPPContent << "// =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "DECLARE_FUNCTION_PLUGIN" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "BEGIN_FUNCTION_SIGNATURE" << std::endl;
  CPPContent << "" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_ID(\"" << m_Options["funcid"] << "\");" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_NAME(\"\");" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_DESCRIPTION(\"\");" << std::endl;
  CPPContent << "" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_VERSION(\""+FuncVersionStr.str()+"\");" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_SDKVERSION;" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);" << std::endl;
  CPPContent << "" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_DOMAIN(\"\");" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_PROCESS(\"\");" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_METHOD(\"\");" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_AUTHORNAME(\"" << m_Options["authorname"] << "\");" << std::endl;
  CPPContent << "  DECLARE_SIGNATURE_AUTHOREMAIL(\"" << m_Options["authoremail"] << "\");" << std::endl;
  CPPContent << "" << std::endl;
  CPPContent << "END_FUNCTION_SIGNATURE" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "// =====================================================================" << std::endl;
  CPPContent << "// =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "class " << m_Options["cppclass"] << " : public openfluid::ware::PluggableFunction" << std::endl;
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
  CPPContent << "    void initParams(const openfluid::core::FuncParamsMap_t& /*Params*/)" << std::endl;
  CPPContent << "    {" << std::endl;
  CPPContent << "" << std::endl;
  CPPContent << "    }" << std::endl;
  CPPContent << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    void prepareData()" << std::endl;
  CPPContent << "    {" << std::endl;
  CPPContent << std::endl;
  CPPContent << "    }" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    void checkConsistency()" << std::endl;
  CPPContent << "    {" << std::endl;
  CPPContent << std::endl;
  CPPContent << "    }" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    void initializeRun()" << std::endl;
  CPPContent << "    {" << std::endl;
  CPPContent << std::endl;
  CPPContent << "    }" << std::endl;
  CPPContent << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    openfluid::core::Duration_t runStep()" << std::endl;
  CPPContent << "    {" << std::endl;
  CPPContent << std::endl;
  CPPContent << "      return Never();" << std::endl;
  CPPContent << "    }" << std::endl;
  CPPContent << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << "    // =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << std::endl;
  CPPContent << "    void finalizeRun()" << std::endl;
  CPPContent << "    {" << std::endl;
  CPPContent << std::endl;
  CPPContent << "    }" << std::endl;
  CPPContent << std::endl;
  CPPContent << "};" << std::endl;
  CPPContent << "" << std::endl;
  CPPContent << "// =====================================================================" << std::endl;
  CPPContent << "// =====================================================================" << std::endl;
  CPPContent << std::endl;
  CPPContent << "DEFINE_FUNCTION_CLASS(" << m_Options["cppclass"] << ")" << std::endl;
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

  mp_Listener->onInfo("Function ID: " + m_Options["funcid"]);
  mp_Listener->onInfo("Function C++ class: " + m_Options["cppclass"]);
  mp_Listener->onInfo("Output directory: " + m_Options["outputdir"]);
  mp_Listener->onInfo("Function filename: " + m_Options["funcid"] + ".cpp");
  mp_Listener->onInfo("Author name: " + m_Options["authorname"]);
  mp_Listener->onInfo("Author email: " + m_Options["authoremail"]);

  boost::filesystem::path OutputDirPath(m_Options["outputdir"]);

  if (m_Options["funcid"] == "")
      throw openfluid::base::OFException("OpenFLUID framework","NewFuncBuddy::run()","No function ID");

  if (m_Options["cppclass"] == "")
      throw openfluid::base::OFException("OpenFLUID framework","NewFuncBuddy::run()","No function C++ class");


  if (!boost::filesystem::exists(OutputDirPath))
    throw openfluid::base::OFException("OpenFLUID framework","NewFuncBuddy::run()","Output directory does not exist");



  writeFunctionCPP();

  return true;
}


} } //namespaces

