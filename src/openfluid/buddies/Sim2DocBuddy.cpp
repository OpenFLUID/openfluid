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
  @brief Implements ...

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/buddies/Sim2DocBuddy.hpp>

#include <iosfwd>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include <openfluid/tools/SwissTools.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/buddies/BuddiesListener.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace buddies {

typedef parse_info<std::string::const_iterator>   Parser_t;



Sim2DocBuddy::Sim2DocBuddy(openfluid::buddies::BuddiesListener* Listener) :
               OpenFLUIDBuddy(Listener)
{
  m_RequiredOptionsHelp["outputdir"] = "path for generated files";
  m_RequiredOptionsHelp["inputcpp"] = "path for cpp file to parse";

  m_OtherOptionsHelp["html"] = "set to 1 in order to generate documentation as HTML files";
  m_OtherOptionsHelp["pdf"] = "set to 1 in order to generate documentation as PDF file";
  m_OtherOptionsHelp["tplfile"] = "path to template file";


  m_Title = "Unknown title";
  m_SimID = "unknown.id";
  m_SimName = "Unknown simulator Name";
  m_SimVersion = "Unknown version";
  m_SimAuthorName = "Unknown simulator Author";
  m_SimAuthorEmail = "Unknown simulator Author Email";
  m_SimDomain = "Unknown simulator Domain";
  m_NewCommands = "";

  m_HTMLPackageLatexCommand = "";


  m_BeginSignatureTag = "BEGIN_SIMULATOR_SIGNATURE";
  m_EndSignatureTag = "END_SIMULATOR_SIGNATURE";
  m_BeginSim2DocTag = "<sim2doc>";
  m_EndSim2DocTag = "</sim2doc>";

  m_KeyValue = "";
  m_BuiltParam = "";
}


// =====================================================================
// =====================================================================


Sim2DocBuddy::~Sim2DocBuddy()
{

}


// =====================================================================
// =====================================================================


std::string Sim2DocBuddy::extractBetweenTags(std::string Content, const std::string BeginTag, const std::string EndTag)
{
  std::string::size_type Index = Content.find(BeginTag);

  if (Index != std::string::npos)
  {
    Content.erase(0,Index+BeginTag.length());
    Index = Content.find(EndTag);
    if (Index != std::string::npos) Content.erase(Index,Content.length()-Index);
  }

  return Content;

}


// =====================================================================
// =====================================================================


std::string Sim2DocBuddy::toLatexFriendly(std::string Content)
{
  //boost::algorithm::replace_all(Content,"\\","\\backslash");
  boost::algorithm::replace_all(Content,"$","\\$");
  boost::algorithm::replace_all(Content,"_","\\_");
  boost::algorithm::replace_all(Content,"&","\\&");
  boost::algorithm::replace_all(Content,"%","\\%");
  boost::algorithm::replace_all(Content,"{","\\{");
  boost::algorithm::replace_all(Content,"}","\\}");
  boost::algorithm::replace_all(Content,"~","$\\sim$");

  return Content;
}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::addLatexDataCatBegin(std::string& Content, const std::string Title, const std::string ColsFormat)
{
  std::string ColsNbrStr;
  openfluid::tools::ConvertValue(ColsFormat.length(),&ColsNbrStr);

  Content = Content + "\\begin{center}\\begin{small}"+"\n"+"\\begin{tabularx}{\\linewidth}{"+ColsFormat+"}"+"\n";
  Content = Content + "\\multicolumn{"+ColsNbrStr+"}{l}{\\begin{large}\\textbf{"+Title+"}\\end{large}}\\\\"+"\n"+"\\hline"+"\n";
}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::addLatexDataCatEnd(std::string& Content)
{
  Content = Content + "\\hline"+"\n"+"\\end{tabularx}"+"\n"+"\\end{small}\\end{center}"+"\n"+"\\smallskip"+"\n\n";
}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::copyDocDirectory()
{
  if (m_InputDirPath != m_OutputDirPath)
  {
    boost::filesystem::path InputDocDirPath(m_InputDirPath.string() + "/doc");

    if (boost::filesystem::is_directory(InputDocDirPath))
    {
      mp_Listener->onSubstageCompleted("** Processing doc directory...");
      openfluid::tools::CopyDirectoryRecursively(InputDocDirPath.string(),m_OutputDirPath.string(),true);
      mp_Listener->onStageCompleted(" done");

    }
  }
}

// =====================================================================
// =====================================================================


void Sim2DocBuddy::extractLatexDocFromCPP()
{
  mp_Listener->onSubstageCompleted("** Extracting documentation from C++ file...");

  std::ifstream CPPFile(m_InputFilePath.string().c_str());

  // check if file exists and if it is "openable"
  if (!CPPFile)
    throw openfluid::base::FrameworkException("Sim2DocBuddy::extractLatexDocFromCPP()","Could not open input file");

  std::string StrLine  = "";
  std::string CPPFileContent = "";

  // parse and loads file contents
  while(std::getline(CPPFile,StrLine))
  {
    CPPFileContent = CPPFileContent + StrLine + "\n";
  }

  CPPFile.close();

  m_ExtractedLatexDoc = extractBetweenTags(CPPFileContent,m_BeginSim2DocTag,m_EndSim2DocTag);

  mp_Listener->onStageCompleted(" done");

}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::cpreprocessCPP()
{
#if defined __unix__ || defined __APPLE__

  mp_Listener->onSubstageCompleted("** Preprocessing C++ file (using gcc)...");

  m_CProcessedFilePath = boost::filesystem::path(m_OutputDirPath.string() + "/" + m_InputFilePath.filename().string() + ".sim2doc");

  boost::filesystem::remove(m_CProcessedFilePath);

  std::string CommandToRun = m_CPreProcessorPath.string() + " -E -fdirectives-only -nostdinc -nostdinc++ -undef -fpreprocessed " + m_InputFilePath.string() + " > " + m_CProcessedFilePath.string() + " 2>/dev/null";

  if (system(CommandToRun.c_str()) == 0)
    throw openfluid::base::FrameworkException("Sim2DocBuddy::cpreprocessCPP()","Error running c preprocessor");

  if (!boost::filesystem::is_regular(m_CProcessedFilePath))
    throw openfluid::base::FrameworkException("Sim2DocBuddy::cpreprocessCPP()","C preprocessed file not generated");

  mp_Listener->onStageCompleted(" done");

#endif
}

// =====================================================================
// =====================================================================


std::string Sim2DocBuddy::extractSignatureLines()
{
  std::ifstream CProcessedFile(m_CProcessedFilePath.string().c_str());

  // check if file exists and if it is "openable"
  if (!CProcessedFile)
    throw openfluid::base::FrameworkException("Sim2DocBuddy::extractSignatureLines()","Could not open C preprocessed file");

  std::string StrLine  = "";
  std::string FileContent = "";

  // parse and loads file contents
  while(std::getline(CProcessedFile,StrLine))
  {
    FileContent = FileContent + StrLine + "\n";
  }

  CProcessedFile.close();

  std::string SignatureContent = extractBetweenTags(FileContent,m_BeginSignatureTag,m_EndSignatureTag);

  // Add of delimiters to start and end parsing
  SignatureContent = m_BeginSignatureTag + SignatureContent + m_EndSignatureTag;

  return SignatureContent;
}


// =====================================================================
// =====================================================================

std::vector<std::string> Sim2DocBuddy::searchStringLitterals(std::string StrToParse)
{
  std::vector<std::string> FoundStrings;
  std::string::const_iterator StrStart, StrEnd;
  boost::regex Expression("\"([^\"\\\\]|\\\\.)*\"");

  StrStart = StrToParse.begin();
  StrEnd = StrToParse.end();

  boost::sregex_token_iterator It(StrStart, StrEnd, Expression, 0);
  boost::sregex_token_iterator EndIt;

  for (; It != EndIt ; ++It)
  {
    std::string FoundStr = std::string(It->first, It->second);
    FoundStr = FoundStr.substr(1,FoundStr.length()-2);
    FoundStrings.push_back(FoundStr);
  }

  return FoundStrings;
}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::addVectorMember(std::vector<openfluid::ware::SignatureHandledUnitsClassItem> *UpdatedUnitsClass)
{
  UpdatedUnitsClass->push_back(openfluid::ware::SignatureHandledUnitsClassItem());
}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::storeInUnitsClass(std::vector<openfluid::ware::SignatureHandledUnitsClassItem> *UpdatedUnitsClass,
    int Att)
{
  if (!UpdatedUnitsClass->empty())
  {
    if (Att == 1)
      UpdatedUnitsClass->back().UnitsClass = m_BuiltParam;
    else
      UpdatedUnitsClass->back().Description = m_BuiltParam;
  }
  m_BuiltParam.erase();
}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::storeKey(SignatureData_t *SignatureData, std::string State)
{
  m_KeyValue = toLatexFriendly(m_BuiltParam);
  if (!State.empty())
    (*SignatureData)[m_KeyValue].push_back(State);
  else
    (*SignatureData)[m_KeyValue].clear();
  m_BuiltParam.erase();
}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::storeIntoSignatureData(SignatureData_t *SignatureData)
{
  (*SignatureData)[m_KeyValue].push_back(m_BuiltParam);
  m_BuiltParam.erase();
}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::buildParam(const char* First, const char* Last)
{
  std::string Str(First, Last);
  std::size_t Found;

  while ((Found = Str.find("\\\"")) != std::string::npos)
    Str.erase(Found, 1);

  m_BuiltParam += Str;
}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::storeData(std::string *Param)
{
  if (Param != 0)
    *Param = m_BuiltParam;

  m_BuiltParam.erase();
}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::setSchedulingFixed(double Val)
{
  m_TimeScheduling.setAsFixed((core::Duration_t) Val);
}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::setSchedulingRange(double Max)
{
  m_TimeScheduling.setAsRange(m_TimeScheduling.Min, (core::Duration_t) Max);
}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::turnIntoLatexSyntax()
{
  m_SimID = toLatexFriendly(m_SimID);
  m_Title = m_SimID;
  m_NewCommands = m_NewCommands + "\\newcommand{\\simID}{"+m_SimID+"}\n";

  m_SimName = toLatexFriendly(m_SimName);
  m_NewCommands = m_NewCommands + "\\newcommand{\\simNAME}{"+m_SimName+"}\n";

  m_SimVersion = toLatexFriendly(m_SimVersion);
  m_NewCommands = m_NewCommands + "\\newcommand{\\simVERSION}{"+m_SimVersion+"}\n";

  m_SimDescription = toLatexFriendly(m_SimDescription);
  m_SimDomain = toLatexFriendly(m_SimDomain);
  m_SimAuthorName = toLatexFriendly(m_SimAuthorName);
  m_SimAuthorEmail = toLatexFriendly(m_SimAuthorEmail);

  SignatureData_t::iterator ParamIt;
  unsigned int i;

  for (ParamIt = m_ParamsData.begin(); ParamIt != m_ParamsData.end(); ++ParamIt)
  {
    for (i = 0; i < ParamIt->second.size(); ++i)
      ParamIt->second[i] = toLatexFriendly(ParamIt->second[i]);
    if (ParamIt->second[1].empty())
      ParamIt->second[1] = "-";
  }

  for (ParamIt = m_InVars.begin(); ParamIt != m_InVars.end(); ++ParamIt)
  {
    for (i = 2; i < ParamIt->second.size(); ++i)
      ParamIt->second[i] = toLatexFriendly(ParamIt->second[i]);
    if (ParamIt->second[3].empty())
      ParamIt->second[3] = "-";
  }

  for (ParamIt = m_OutVars.begin(); ParamIt != m_OutVars.end(); ++ParamIt)
  {
    for (i = 2; i < ParamIt->second.size(); ++i)
      ParamIt->second[i] = toLatexFriendly(ParamIt->second[i]);
    if (ParamIt->second[3].empty())
      ParamIt->second[3] = "-";
  }

  for (ParamIt = m_InAttrs.begin(); ParamIt != m_InAttrs.end(); ++ParamIt)
  {
    for (i = 2; i < ParamIt->second.size(); ++i)
      ParamIt->second[i] = toLatexFriendly(ParamIt->second[i]);
    if (ParamIt->second[3].empty())
      ParamIt->second[3] = "-";
  }

  for (ParamIt = m_OutAttrs.begin(); ParamIt != m_OutAttrs.end(); ++ParamIt)
  {
    for (i = 2; i < ParamIt->second.size(); ++i)
      ParamIt->second[i] = toLatexFriendly(ParamIt->second[i]);
    if (ParamIt->second[3].empty())
      ParamIt->second[3] = "-";
  }


  std::vector<ware::SignatureHandledUnitsClassItem>::iterator UnitsClassIt;

  m_UnitsGraph.UpdatedUnitsGraph = toLatexFriendly(m_UnitsGraph.UpdatedUnitsGraph);

  for (UnitsClassIt = m_UnitsGraph.UpdatedUnitsClass.begin();
        UnitsClassIt != m_UnitsGraph.UpdatedUnitsClass.end(); ++UnitsClassIt)
  {
    UnitsClassIt->UnitsClass = toLatexFriendly(UnitsClassIt->UnitsClass);
    UnitsClassIt->Description = toLatexFriendly(UnitsClassIt->UnitsClass);
  }
}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::processSignature()
{
  mp_Listener->onSubstageCompleted("** Processing simulator signature...");

  std::string SignatureContentStr = extractSignatureLines();
  const char* SignatureContent = SignatureContentStr.c_str();

  // Creation of grammar for parsing
  SimSignatureGrammar Grammar(this);
  // Parse signature
  Parser_t pInfo = parse(SignatureContent, SignatureContent + strlen(SignatureContent), Grammar);

  if (!pInfo.full)
  {
    std::string Line = "";
    while (*pInfo.stop != 0 && *pInfo.stop != '\n' && *pInfo.stop != '\r')
    {
      Line += *pInfo.stop;
      ++pInfo.stop;
    }

    throw openfluid::base::FrameworkException("Sim2DocBuddy::processSignature()","Error parsing simulator signature line \""+Line+"\"");
  }


  // Apply Latex syntax to attributes
  turnIntoLatexSyntax();

  mp_Listener->onStageCompleted(" done");

}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::generateLatex()
{
  mp_Listener->onSubstageCompleted("** Generating latex file...");

  // loading template

  std::ifstream TPLFile(m_TplFilePath.string().c_str());

  // check if file exists and if it is "openable"
  if (!TPLFile)
    throw openfluid::base::FrameworkException("Sim2DocBuddy::generateLatex()","Could not open template file");

  std::string StrLine  = "";
  m_LatexOutFile = "";

  // parse and loads file contents
  while(std::getline(TPLFile,StrLine))
  {
    m_LatexOutFile = m_LatexOutFile + StrLine + "\n";
  }

  TPLFile.close();


  // generate simulator data

  SignatureData_t::iterator it;

  if (m_ParamsData.size() > 0)
  {
    addLatexDataCatBegin(m_SimData,"Simulator parameter(s)","lXr");
    for (it = m_ParamsData.begin(); it != m_ParamsData.end(); ++it)
    {
      m_SimData = m_SimData + "\\texttt{" + it->first + "}&" + it->second[0] + "&$" + it->second[1] + "$\\\\" + "\n";
    }
    addLatexDataCatEnd(m_SimData);
  }

  if (m_InAttrs.size() > 0)
  {
    addLatexDataCatBegin(m_SimData,"Required or used Attribute(s)","lllXr");
    for (it = m_InAttrs.begin(); it != m_InAttrs.end(); ++it)
    {
      m_SimData = m_SimData + "\\texttt{" + it->first + "}&" + it->second[0] + "&" + it->second[1] + "&" + it->second[2] + "&$" + it->second[3] + "$\\\\" + "\n";
    }
    addLatexDataCatEnd(m_SimData);
  }

  if (m_OutAttrs.size() > 0)
  {
    addLatexDataCatBegin(m_SimData,"Produced Attribute(s)","lllXr");
    for (it = m_OutAttrs.begin(); it != m_OutAttrs.end(); ++it)
    {
      m_SimData = m_SimData + "\\texttt{" + it->first + "}&" + it->second[0] + "&" + it->second[1] + "&" + it->second[2] + "&$" + it->second[3] + "$\\\\" + "\n";
    }
    addLatexDataCatEnd(m_SimData);
  }

  if (m_InVars.size() > 0)
  {
    addLatexDataCatBegin(m_SimData,"Required or used variable(s)","llllXr");
    for (it = m_InVars.begin(); it != m_InVars.end(); ++it)
    {
      m_SimData = m_SimData + "\\texttt{" + it->first + "}&" + it->second[0] + "&" + it->second[1] + "&" + it->second[2] + "&" + it->second[3] + "$\\\\" + "\n";
    }
    addLatexDataCatEnd(m_SimData);
  }

  if (m_OutVars.size() > 0)
  {
    addLatexDataCatBegin(m_SimData,"Produced or updated variable(s)","lllXr");
    for (it = m_OutVars.begin(); it != m_OutVars.end(); ++it)
    {
      m_SimData = m_SimData + "\\texttt{" + it->first + "}&" + it->second[0] + "&" + it->second[1] + "&" + it->second[2] + "&$" + it->second[3] + "$\\\\" + "\n";
    }
    addLatexDataCatEnd(m_SimData);
  }

  if (m_Events.size() > 0)
  {
    addLatexDataCatBegin(m_SimData,"Used event(s)","l");
    for (it = m_Events.begin(); it != m_Events.end(); ++it)
    {
      m_SimData = m_SimData + "\\texttt{" + it->first + "}\\\\" + "\n";
    }
    addLatexDataCatEnd(m_SimData);
  }

  if (m_ExtraFiles.size() > 0)
  {
    addLatexDataCatBegin(m_SimData,"Required or used extrafile(s)","lX");
    for (it = m_ExtraFiles.begin(); it != m_ExtraFiles.end(); ++it)
    {
      m_SimData = m_SimData + "\\texttt{" + it->first + "}&" + it->second[0] + "\\\\" + "\n";
    }
    addLatexDataCatEnd(m_SimData);
  }

  // replacing values

  boost::algorithm::replace_all(m_LatexOutFile,"#htmlpackage#",m_HTMLPackageLatexCommand);
  boost::algorithm::replace_all(m_LatexOutFile,"#title#",m_Title);
  boost::algorithm::replace_all(m_LatexOutFile,"#version#",m_SimVersion);
  boost::algorithm::replace_all(m_LatexOutFile,"#author#",m_SimAuthorName);
  boost::algorithm::replace_all(m_LatexOutFile,"#newcommands#",m_NewCommands);
  boost::algorithm::replace_all(m_LatexOutFile,"#name#",m_SimName);
  boost::algorithm::replace_all(m_LatexOutFile,"#domain#",m_SimDomain);
  boost::algorithm::replace_all(m_LatexOutFile,"#description#",m_SimDescription);
  boost::algorithm::replace_all(m_LatexOutFile,"#handleddata#",m_SimData);
  boost::algorithm::replace_all(m_LatexOutFile,"#simdoc#",m_ExtractedLatexDoc);

  // save of latex file content

  m_OutputLatexFilePath = boost::filesystem::path(m_OutputDirPath.string()+"/"+m_SimID+".tex");

  std::ofstream OutputFile(m_OutputLatexFilePath.string().c_str(),std::ios::out);
  OutputFile << m_LatexOutFile;
  OutputFile.close();


  mp_Listener->onStageCompleted(" done");
}

// =====================================================================
// =====================================================================

bool Sim2DocBuddy::isErrorInPDFLatexLog()
{
  boost::filesystem::path LogFilePath(m_OutputDirPath.string() + "/" + boost::filesystem::basename(m_OutputLatexFilePath) + ".log");

  std::ifstream LogFile(LogFilePath.string().c_str());

  // check if file exists and if it is "openable"
  if (!LogFile) return true;

  std::string LogFileContent = "";
  std::string StrLine = "";

  // parse and loads file contents
  while(std::getline(LogFile,StrLine))
  {
    LogFileContent = LogFileContent + StrLine + "\n";
  }

  LogFile.close();

  return (LogFileContent.find("Fatal error") != std::string::npos);

}

// =====================================================================
// =====================================================================


void Sim2DocBuddy::buildPDF()
{
#if defined __unix__ || defined __APPLE__

  mp_Listener->onSubstageCompleted("** Building PDF...");

  if (chdir(m_OutputDirPath.string().c_str()) != 0)
    throw openfluid::base::FrameworkException("Sim2DocBuddy::buildPDF()","Error changing current directory to " + m_OutputDirPath.string());

  std::string PDFCommandToRun = m_PDFLatexPath.string() + " -shell-escape -interaction=nonstopmode -output-directory="+m_OutputDirPath.string()+" "+ m_OutputLatexFilePath.string() + " > /dev/null";
  std::string BibCommandToRun = m_BibtexPath.string() + " " + boost::filesystem::path(m_OutputDirPath.string()+"/"+m_SimID).string() + " > /dev/null";

  mp_Listener->onSubstageCompleted(" first pass...");

  if (system(PDFCommandToRun.c_str()) == -1)
    throw openfluid::base::FrameworkException("Sim2DocBuddy::buildPDF()","Error running pdflatex command");

  if (isErrorInPDFLatexLog())
    throw openfluid::base::FrameworkException("Sim2DocBuddy::buildPDF()","Error running pdflatex command (catched in log file)");

  mp_Listener->onSubstageCompleted(" bibliography and references...");


  if (system(BibCommandToRun.c_str()) == -1)
    throw openfluid::base::FrameworkException("Sim2DocBuddy::buildPDF()","Error running bibtex command");

  mp_Listener->onSubstageCompleted(" second pass...");

  if (system(PDFCommandToRun.c_str()) == -1)
    throw openfluid::base::FrameworkException("Sim2DocBuddy::buildPDF()","Error running pdflatex command");

  if (isErrorInPDFLatexLog())
    throw openfluid::base::FrameworkException("Sim2DocBuddy::buildPDF()","Error running pdflatex command (catched in log file)");

  mp_Listener->onSubstageCompleted(" third pass...");

  if (system(PDFCommandToRun.c_str()) == -1)
    throw openfluid::base::FrameworkException("Sim2DocBuddy::buildPDF()","Error running pdflatex command");

  if (isErrorInPDFLatexLog())
    throw openfluid::base::FrameworkException("Sim2DocBuddy::buildPDF()","Error running pdflatex command (catched in log file)");

  mp_Listener->onStageCompleted(" done");

#endif
}


// =====================================================================
// =====================================================================


void Sim2DocBuddy::buildHTML()
{
#if defined __unix__ || defined __APPLE__

  mp_Listener->onSubstageCompleted("** Building HTML...");

  if (chdir(m_OutputDirPath.string().c_str()) != 0)
    throw openfluid::base::FrameworkException("Sim2DocBuddy::buildHTML()","Error changing current directory to " + m_OutputDirPath.string());

  std::string CommandToRun = m_Latex2HTMLPath.string() + " -dir="+m_OutputDirPath.string()+" "+ m_OutputLatexFilePath.string() +" > /dev/null";

  if (system(CommandToRun.c_str()) != 0)
    throw openfluid::base::FrameworkException("Sim2DocBuddy::buildHTML()","Error running latex2html command");

  mp_Listener->onStageCompleted(" done");

#endif
}


// =====================================================================
// =====================================================================



bool Sim2DocBuddy::run()
{
#if defined __unix__ || defined __APPLE__

  setOptionIfNotSet("tplfile", openfluid::base::RuntimeEnvironment::getInstance()->getCommonResourceFilePath(openfluid::config::SIM2DOC_TPLFILE_NAME));
  setOptionIfNotSet("outputdir",boost::filesystem::current_path().string());
  setOptionIfNotSet("pdf","0");
  setOptionIfNotSet("html","0");

  mp_Listener->onInfo("Input CPP file: " + m_Options["inputcpp"]);
  mp_Listener->onInfo("Output directory: " + m_Options["outputdir"]);
  mp_Listener->onInfo("Template file: " + m_Options["tplfile"]);
  mp_Listener->onInfo("Generate PDF: " + getYesNoFromOneZero(m_Options["pdf"]));
  mp_Listener->onInfo("Generate HTML: " + getYesNoFromOneZero(m_Options["html"]));

  std::vector<std::string> GCCPaths = openfluid::tools::GetFileLocationsUsingPATHEnvVar("gcc");

  if (GCCPaths.size() > 0)
  {
    m_CPreProcessorPath = boost::filesystem::path(GCCPaths[0]);
    mp_Listener->onInfo("Using C preprocessor: " + m_CPreProcessorPath.string());
  }
  else throw openfluid::base::FrameworkException("Sim2DocBuddy::run()","C preprocessor (gcc) not found");


  if (m_Options["pdf"] == "1")
  {
    std::vector<std::string> PDFLatexPaths = openfluid::tools::GetFileLocationsUsingPATHEnvVar("pdflatex");
    std::vector<std::string> BibtexPaths = openfluid::tools::GetFileLocationsUsingPATHEnvVar("bibtex");

    if (PDFLatexPaths.size() > 0 && BibtexPaths.size() > 0)
      {
        m_PDFLatexPath = boost::filesystem::path(PDFLatexPaths[0]);
        mp_Listener->onInfo("Using PDFLatex compiler: " + m_PDFLatexPath.string());
        m_BibtexPath = boost::filesystem::path(BibtexPaths[0]);
        mp_Listener->onInfo("Using Bibtex compiler: " + m_BibtexPath.string());
      }
      else
      {
        mp_Listener->onInfo("!! PDFLatex compiler and/or Bibtex compiler not found. Skipped. ");
        m_Options["pdf"] = "0";
      }
  }

  if (m_Options["html"] == "1")
  {
    std::vector<std::string> Latex2HTMLPaths = openfluid::tools::GetFileLocationsUsingPATHEnvVar("latex2html");

    m_HTMLPackageLatexCommand = "\\usepackage{html}";

    if (Latex2HTMLPaths.size() > 0)
      {
        m_Latex2HTMLPath = boost::filesystem::path(Latex2HTMLPaths[0]);
        mp_Listener->onInfo("Using Latex2HTML converter: " + m_Latex2HTMLPath.string());
      }
      else
      {
        mp_Listener->onInfo("!! Latex2HTML converter not found. Skipped. ");
        m_Options["html"] = "0";
      }
  }

  m_InputFilePath = boost::filesystem::path(m_Options["inputcpp"]);
  m_OutputDirPath = boost::filesystem::path(m_Options["outputdir"]);

  if (!boost::filesystem::exists(m_InputFilePath))
    throw openfluid::base::FrameworkException("Sim2DocBuddy::run()","Input file does not exist");

  m_TplFilePath = boost::filesystem::path(m_Options["tplfile"]);
  if (!boost::filesystem::exists(m_TplFilePath))
    throw openfluid::base::FrameworkException("Sim2DocBuddy::run()","Template file does not exist");

  if (!boost::filesystem::is_directory(m_OutputDirPath)) boost::filesystem::create_directories(m_OutputDirPath);


  m_InputDirPath = m_InputFilePath.branch_path();

  copyDocDirectory();

  extractLatexDocFromCPP();

  cpreprocessCPP();

  processSignature();

  generateLatex();


  if (m_Options["pdf"] == "1") buildPDF();

  if (m_Options["html"] == "1") buildHTML();

#else
  mp_Listener->onInfo("sim2doc buddy only runs on Unix/Linux and MacOSX systems.");
  return true;
#endif


  return true;
}


} } //namespaces

