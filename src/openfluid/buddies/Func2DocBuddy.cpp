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


#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <fstream>
#include <cstdlib>

#include <openfluid/buddies/Func2DocBuddy.hpp>
#include <openfluid/core.hpp>
#include <openfluid/base.hpp>
#include <openfluid/tools.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace buddies {


Func2DocBuddy::Func2DocBuddy(openfluid::buddies::BuddiesListener* Listener) :
               OpenFLUIDBuddy(Listener)
{
  m_RequiredOptionsHelp["outputdir"] = "path for generated files";
  m_RequiredOptionsHelp["inputcpp"] = "path for cpp file to parse";

  m_OtherOptionsHelp["html"] = "set to 1 in order to generate documentation as HTML files";
  m_OtherOptionsHelp["pdf"] = "set to 1 in order to generate documentation as PDF file";
  m_OtherOptionsHelp["tplfile"] = "path to template file";


  m_Title = "Unknown title";
  m_FuncID = "unknown.id";
  m_FuncName = "Unknown Function Name";
  m_FuncVersion = "Unknown version";
  m_FuncAuthorName = "Unknown Function Author";
  m_FuncAuthorEmail = "Unknown Function Author Email";
  m_FuncDomain = "Unknown Function Domain";
  m_NewCommands = "";

  m_HTMLPackageLatexCommand = "";


  m_BeginSignatureTag = "BEGIN_SIGNATURE_HOOK";
  m_EndSignatureTag = "END_SIGNATURE_HOOK";
  m_BeginFunc2DocTag = "<func2doc>";
  m_EndFunc2DocTag = "</func2doc>";

}


// =====================================================================
// =====================================================================


Func2DocBuddy::~Func2DocBuddy()
{

}


// =====================================================================
// =====================================================================


std::string Func2DocBuddy::extractBetweenTags(std::string Content, const std::string BeginTag, const std::string EndTag)
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


std::string Func2DocBuddy::toLatexFriendly(std::string Content)
{
  boost::algorithm::replace_all(Content,"\\","$\backslash$");
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


void Func2DocBuddy::addLatexDataCatBegin(std::string& Content, const std::string Title, const std::string ColsFormat)
{
  std::string ColsNbrStr;
  openfluid::tools::ConvertValue(ColsFormat.length(),&ColsNbrStr);

  Content = Content + "\\begin{center}\\begin{small}"+"\n"+"\\begin{tabularx}{\\linewidth}{"+ColsFormat+"}"+"\n";
  Content = Content + "\\multicolumn{"+ColsNbrStr+"}{l}{\\begin{large}\\textbf{"+Title+"}\\end{large}}\\\\"+"\n"+"\\hline"+"\n";
}


// =====================================================================
// =====================================================================


void Func2DocBuddy::addLatexDataCatEnd(std::string& Content)
{
  Content = Content + "\\hline"+"\n"+"\\end{tabularx}"+"\n"+"\\end{small}\\end{center}"+"\n"+"\\smallskip"+"\n\n";
}


// =====================================================================
// =====================================================================


void Func2DocBuddy::copyDocDirectory()
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


void Func2DocBuddy::extractLatexDocFromCPP()
{
  mp_Listener->onSubstageCompleted("** Extracting documentation from C++ file...");

  std::ifstream CPPFile(m_InputFilePath.string().c_str());

  // check if file exists and if it is "openable"
  if (!CPPFile)
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::extractLatexDocFromCPP()","Could not open input file");

  std::string StrLine  = "";
  std::string CPPFileContent = "";

  // parse and loads file contents
  while(std::getline(CPPFile,StrLine))
  {
    CPPFileContent = CPPFileContent + StrLine + "\n";
  }

  CPPFile.close();

  m_ExtractedLatexDoc = extractBetweenTags(CPPFileContent,m_BeginFunc2DocTag,m_EndFunc2DocTag);

  mp_Listener->onStageCompleted(" done");

}


// =====================================================================
// =====================================================================


void Func2DocBuddy::cpreprocessCPP()
{
#if defined __unix__ || defined __APPLE__

  mp_Listener->onSubstageCompleted("** Preprocessing C++ file (using gcc)...");

  m_CProcessedFilePath = boost::filesystem::path(m_OutputDirPath.string() + "/" + m_InputFilePath.leaf() + ".func2doc");

  boost::filesystem::remove(m_CProcessedFilePath);

  std::string CommandToRun = m_CPreProcessorPath.string() + " -E -fdirectives-only -nostdinc -nostdinc++ -undef -fpreprocessed " + m_InputFilePath.string() + " > " + m_CProcessedFilePath.string() + " 2>/dev/null";

  if (system(CommandToRun.c_str()) == 0)
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::cpreprocessCPP()","Error running c preprocessor");

  if (!boost::filesystem::is_regular(m_CProcessedFilePath))
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::cpreprocessCPP()","C preprocessed file not generated");

  mp_Listener->onStageCompleted(" done");

#endif
}

// =====================================================================
// =====================================================================


std::vector<std::string> Func2DocBuddy::extractSignatureLines()
{
  std::ifstream CProcessedFile(m_CProcessedFilePath.string().c_str());

  // check if file exists and if it is "openable"
  if (!CProcessedFile)
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::extractSignatureLines()","Could not open C preprocessed file");

  std::string StrLine  = "";
  std::string FileContent = "";

  // parse and loads file contents
  while(std::getline(CProcessedFile,StrLine))
  {
    FileContent = FileContent + StrLine;
  }

  CProcessedFile.close();

  std::string SignatureContent = extractBetweenTags(FileContent,m_BeginSignatureTag,m_EndSignatureTag);

  std::vector<std::string> Lines;

  boost::algorithm::split_regex(Lines, SignatureContent,boost::regex("DECLARE_"));

  for (unsigned int i = 0; i < Lines.size(); i++)
  {
    boost::algorithm::trim(Lines[i]);
    boost::algorithm::replace_all(Lines[i],"\n","");
  }

  return Lines;

}


// =====================================================================
// =====================================================================

std::vector<std::string> Func2DocBuddy::searchStringLitterals(std::string StrToParse)
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

void Func2DocBuddy::processSignature()
{

  std::vector<std::string> LineParts;


  mp_Listener->onSubstageCompleted("** Processing function signature...");


  std::vector<std::string> Lines = extractSignatureLines();


  for (unsigned i=0; i< Lines.size();i++)
  {
    if (boost::algorithm::starts_with(Lines[i],"SIGNATURE_ID"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 1)
      {
        m_FuncID = toLatexFriendly(LineParts[0]);
        m_Title = m_FuncID;
      }
      m_NewCommands = m_NewCommands + "\\newcommand{\\funcID}{"+m_FuncID+"}\n";
    }

    if (boost::algorithm::starts_with(Lines[i],"SIGNATURE_VERSION"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 1) m_FuncVersion = toLatexFriendly(LineParts[0]);
      m_NewCommands = m_NewCommands + "\\newcommand{\\funcVERSION}{"+m_FuncVersion+"}\n";
    }

    if (boost::algorithm::starts_with(Lines[i],"SIGNATURE_DOMAIN"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 1) m_FuncDomain = toLatexFriendly(LineParts[0]);
    }

    if (boost::algorithm::starts_with(Lines[i],"SIGNATURE_NAME"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 1) m_FuncName = toLatexFriendly(LineParts[0]);
      m_NewCommands = m_NewCommands + "\\newcommand{\\funcNAME}{"+m_FuncName+"}\n";
    }

    if (boost::algorithm::starts_with(Lines[i],"SIGNATURE_DOMAIN"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 1) m_FuncDomain = toLatexFriendly(LineParts[0]);
    }

    if (boost::algorithm::starts_with(Lines[i],"SIGNATURE_DESCRIPTION"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 1) m_FuncDescription = toLatexFriendly(LineParts[0]);
    }

    if (boost::algorithm::starts_with(Lines[i],"SIGNATURE_AUTHORNAME"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 1) m_FuncAuthorName = toLatexFriendly(LineParts[0]);
    }

    if (boost::algorithm::starts_with(Lines[i],"SIGNATURE_AUTHOREMAIL"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 1) m_FuncAuthorEmail = toLatexFriendly(LineParts[0]);
    }

    if (boost::algorithm::starts_with(Lines[i],"FUNCTION_PARAM"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 3)
      {
        m_ParamsData[LineParts[0]].push_back(toLatexFriendly(LineParts[1]));
        m_ParamsData[LineParts[0]].push_back(toLatexFriendly(LineParts[2]));
      }
    }

    if (boost::algorithm::starts_with(Lines[i],"REQUIRED_INPUTDATA"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 4)
      {
        m_InData[LineParts[0]].push_back("required");
        m_InData[LineParts[0]].push_back(LineParts[1]);
        m_InData[LineParts[0]].push_back(toLatexFriendly(LineParts[2]));
        m_InData[LineParts[0]].push_back(toLatexFriendly(LineParts[3]));
      }
    }

    if (boost::algorithm::starts_with(Lines[i],"USED_INPUTDATA"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 4)
      {
        m_InData[LineParts[0]].push_back("used");
        m_InData[LineParts[0]].push_back(LineParts[1]);
        m_InData[LineParts[0]].push_back(toLatexFriendly(LineParts[2]));
        m_InData[LineParts[0]].push_back(toLatexFriendly(LineParts[3]));
      }
    }

    if (boost::algorithm::starts_with(Lines[i],"REQUIRED_VAR"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 4)
      {
        m_InVars[LineParts[0]].push_back("required");
        m_InVars[LineParts[0]].push_back("on step");
        m_InVars[LineParts[0]].push_back(LineParts[1]);
        m_InVars[LineParts[0]].push_back(toLatexFriendly(LineParts[2]));
        m_InVars[LineParts[0]].push_back(toLatexFriendly(LineParts[3]));
      }
    }

    if (boost::algorithm::starts_with(Lines[i],"USED_VAR"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 4)
      {
        m_InVars[LineParts[0]].push_back("used");
        m_InVars[LineParts[0]].push_back("on step");
        m_InVars[LineParts[0]].push_back(LineParts[1]);
        m_InVars[LineParts[0]].push_back(toLatexFriendly(LineParts[2]));
        m_InVars[LineParts[0]].push_back(toLatexFriendly(LineParts[3]));
      }
    }

    if (boost::algorithm::starts_with(Lines[i],"REQUIRED_PREVVAR"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 4)
      {
        m_InVars[LineParts[0]].push_back("required");
        m_InVars[LineParts[0]].push_back("on prev. step");
        m_InVars[LineParts[0]].push_back(LineParts[1]);
        m_InVars[LineParts[0]].push_back(toLatexFriendly(LineParts[2]));
        m_InVars[LineParts[0]].push_back(toLatexFriendly(LineParts[3]));
      }
    }

    if (boost::algorithm::starts_with(Lines[i],"USED_PREVVAR"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 4)
      {
        m_InVars[LineParts[0]].push_back("used");
        m_InVars[LineParts[0]].push_back("on prev. step");
        m_InVars[LineParts[0]].push_back(LineParts[1]);
        m_InVars[LineParts[0]].push_back(toLatexFriendly(LineParts[2]));
        m_InVars[LineParts[0]].push_back(toLatexFriendly(LineParts[3]));
      }
    }

    if (boost::algorithm::starts_with(Lines[i],"PRODUCED_VAR"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 4)
      {
        m_OutVars[LineParts[0]].push_back("produced");
        m_OutVars[LineParts[0]].push_back(LineParts[1]);
        m_OutVars[LineParts[0]].push_back(toLatexFriendly(LineParts[2]));
        m_OutVars[LineParts[0]].push_back(toLatexFriendly(LineParts[3]));
      }
    }

    if (boost::algorithm::starts_with(Lines[i],"UPDATED_VAR"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 4)
      {
        m_OutVars[LineParts[0]].push_back("updated");
        m_OutVars[LineParts[0]].push_back(LineParts[1]);
        m_OutVars[LineParts[0]].push_back(toLatexFriendly(LineParts[2]));
        m_OutVars[LineParts[0]].push_back(toLatexFriendly(LineParts[3]));
      }
    }

    if (boost::algorithm::starts_with(Lines[i],"USED_EVENTS"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 1)
      {
        m_Events[LineParts[0]].clear();
      }
    }

    if (boost::algorithm::starts_with(Lines[i],"USED_EXTRAFILE"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 1)
      {
        m_ExtraFiles[LineParts[0]].push_back("used");
      }
    }

    if (boost::algorithm::starts_with(Lines[i],"REQUIRED_EXTRAFILE"))
    {
      LineParts = searchStringLitterals(Lines[i]);
      if (LineParts.size() == 1)
      {
        m_ExtraFiles[LineParts[0]].push_back("required");
      }
    }


  }

  mp_Listener->onStageCompleted(" done");

}


// =====================================================================
// =====================================================================


void Func2DocBuddy::generateLatex()
{
  mp_Listener->onSubstageCompleted("** Generating latex file...");

  // loading template

  std::ifstream TPLFile(m_TplFilePath.string().c_str());

  // check if file exists and if it is "openable"
  if (!TPLFile)
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::generateLatex()","Could not open template file");

  std::string StrLine  = "";
  m_LatexOutFile = "";

  // parse and loads file contents
  while(std::getline(TPLFile,StrLine))
  {
    m_LatexOutFile = m_LatexOutFile + StrLine + "\n";
  }

  TPLFile.close();


  // generate Function Data

  SignatureData_t::iterator it;

  if (m_ParamsData.size() > 0)
  {
    addLatexDataCatBegin(m_FuncData,"Function parameter(s)","lXr");
    for (it = m_ParamsData.begin(); it != m_ParamsData.end(); ++it)
    {
      m_FuncData = m_FuncData + "\\texttt{" + toLatexFriendly(it->first) + "}&" + it->second[0] + "&$" + it->second[1] + "$\\\\" + "\n";
    }
    addLatexDataCatEnd(m_FuncData);
  }

  if (m_InData.size() > 0)
  {
    addLatexDataCatBegin(m_FuncData,"Input data","lllXr");
    for (it = m_InData.begin(); it != m_InData.end(); ++it)
    {
      m_FuncData = m_FuncData + "\\texttt{" + toLatexFriendly(it->first) + "}&" + it->second[0] + "&" + it->second[1] + "&" + it->second[2] + "&$" + it->second[3] + "$\\\\" + "\n";
    }
    addLatexDataCatEnd(m_FuncData);
  }

  if (m_InVars.size() > 0)
  {
    addLatexDataCatBegin(m_FuncData,"Required or used variable(s)","llllXr");
    for (it = m_InVars.begin(); it != m_InVars.end(); ++it)
    {
      m_FuncData = m_FuncData + "\\texttt{" + toLatexFriendly(it->first) + "}&" + it->second[0] + "&" + it->second[1] + "&" + it->second[2] + "&" + it->second[3] + "&$" + it->second[4] + "$\\\\" + "\n";
    }
    addLatexDataCatEnd(m_FuncData);
  }

  if (m_OutVars.size() > 0)
  {
    addLatexDataCatBegin(m_FuncData,"Produced or updated variable(s)","lllXr");
    for (it = m_OutVars.begin(); it != m_OutVars.end(); ++it)
    {
      m_FuncData = m_FuncData + "\\texttt{" + toLatexFriendly(it->first) + "}&" + it->second[0] + "&" + it->second[1] + "&" + it->second[2] + "&$" + it->second[3] + "$\\\\" + "\n";
    }
    addLatexDataCatEnd(m_FuncData);
  }

  if (m_Events.size() > 0)
  {
    addLatexDataCatBegin(m_FuncData,"Used event(s)","l");
    for (it = m_Events.begin(); it != m_Events.end(); ++it)
    {
      m_FuncData = m_FuncData + "\\texttt{" + toLatexFriendly(it->first) + "}\\\\" + "\n";
    }
    addLatexDataCatEnd(m_FuncData);
  }

  if (m_ExtraFiles.size() > 0)
  {
    addLatexDataCatBegin(m_FuncData,"Required or used extrafile(s)","lX");
    for (it = m_ExtraFiles.begin(); it != m_ExtraFiles.end(); ++it)
    {
      m_FuncData = m_FuncData + "\\texttt{" + toLatexFriendly(it->first) + "}&" + it->second[0] + "\\\\" + "\n";
    }
    addLatexDataCatEnd(m_FuncData);
  }

  // replacing values

  boost::algorithm::replace_all(m_LatexOutFile,"#htmlpackage#",m_HTMLPackageLatexCommand);
  boost::algorithm::replace_all(m_LatexOutFile,"#title#",m_Title);
  boost::algorithm::replace_all(m_LatexOutFile,"#version#",m_FuncVersion);
  boost::algorithm::replace_all(m_LatexOutFile,"#author#",m_FuncAuthorName);
  boost::algorithm::replace_all(m_LatexOutFile,"#newcommands#",m_NewCommands);
  boost::algorithm::replace_all(m_LatexOutFile,"#name#",m_FuncName);
  boost::algorithm::replace_all(m_LatexOutFile,"#domain#",m_FuncDomain);
  boost::algorithm::replace_all(m_LatexOutFile,"#description#",m_FuncDescription);
  boost::algorithm::replace_all(m_LatexOutFile,"#handleddata#",m_FuncData);
  boost::algorithm::replace_all(m_LatexOutFile,"#funcdoc#",m_ExtractedLatexDoc);

  // save of latex file content

  m_OutputLatexFilePath = boost::filesystem::path(m_OutputDirPath.string()+"/"+m_FuncID+".tex");

  std::ofstream OutputFile(m_OutputLatexFilePath.string().c_str(),std::ios::out);
  OutputFile << m_LatexOutFile;
  OutputFile.close();


  mp_Listener->onStageCompleted(" done");
}

// =====================================================================
// =====================================================================

bool Func2DocBuddy::isErrorInPDFLatexLog()
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


void Func2DocBuddy::buildPDF()
{
#if defined __unix__ || defined __APPLE__

  mp_Listener->onSubstageCompleted("** Building PDF...");

  if (chdir(m_OutputDirPath.string().c_str()) != 0)
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::buildPDF()","Error changing current directory to " + m_OutputDirPath.string());

  std::string PDFCommandToRun = m_PDFLatexPath.string() + " -shell-escape -interaction=nonstopmode -output-directory="+m_OutputDirPath.string()+" "+ m_OutputLatexFilePath.string() + " > /dev/null";
  std::string BibCommandToRun = m_BibtexPath.string() + " " + boost::filesystem::path(m_OutputDirPath.string()+"/"+m_FuncID).string() + " > /dev/null";

  mp_Listener->onSubstageCompleted(" first pass...");

  if (system(PDFCommandToRun.c_str()) == -1)
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::buildPDF()","Error running pdflatex command");

  if (isErrorInPDFLatexLog())
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::buildPDF()","Error running pdflatex command (catched in log file)");

  mp_Listener->onSubstageCompleted(" bibliography and references...");


  if (system(BibCommandToRun.c_str()) == -1)
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::buildPDF()","Error running bibtex command");

  mp_Listener->onSubstageCompleted(" second pass...");

  if (system(PDFCommandToRun.c_str()) == -1)
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::buildPDF()","Error running pdflatex command");

  if (isErrorInPDFLatexLog())
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::buildPDF()","Error running pdflatex command (catched in log file)");

  mp_Listener->onSubstageCompleted(" third pass...");

  if (system(PDFCommandToRun.c_str()) == -1)
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::buildPDF()","Error running pdflatex command");

  if (isErrorInPDFLatexLog())
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::buildPDF()","Error running pdflatex command (catched in log file)");

  mp_Listener->onStageCompleted(" done");

#endif
}


// =====================================================================
// =====================================================================


void Func2DocBuddy::buildHTML()
{
#if defined __unix__ || defined __APPLE__

  mp_Listener->onSubstageCompleted("** Building HTML...");

  if (chdir(m_OutputDirPath.string().c_str()) != 0)
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::buildHTML()","Error changing current directory to " + m_OutputDirPath.string());

  std::string CommandToRun = m_Latex2HTMLPath.string() + " -dir="+m_OutputDirPath.string()+" "+ m_OutputLatexFilePath.string() +" > /dev/null";

  if (system(CommandToRun.c_str()) != 0)
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::buildHTML()","Error running latex2html command");

  mp_Listener->onStageCompleted(" done");

#endif
}


// =====================================================================
// =====================================================================



bool Func2DocBuddy::run()
{
#if defined __unix__ || defined __APPLE__

  setOptionIfNotSet("tplfile", openfluid::base::RuntimeEnvironment::getInstance()->getCommonResourceFilePath(openfluid::config::FUNC2DOC_TPLFILE_NAME));
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
  else throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::run()","C preprocessor (gcc) not found");


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
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::run()","Input file does not exist");

  m_TplFilePath = boost::filesystem::path(m_Options["tplfile"]);
  if (!boost::filesystem::exists(m_TplFilePath))
    throw openfluid::base::OFException("OpenFLUID framework","Func2DocBuddy::run()","Template file does not exist");

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
  mp_Listener->onInfo("func2doc buddy only runs on Unix/Linux and MacOSX systems.");
  return true;
#endif


  return true;
}


} } //namespaces

