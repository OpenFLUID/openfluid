/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file Func2DocBuddy.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <fstream>
#include <cstdlib>

#include "Func2DocBuddy.h"
#include "openfluid-core.h"
#include "config.h"
#include "AppTools.h"

Func2DocBuddy::Func2DocBuddy() : OpenFLUIDBuddy()
{
  m_RequiredOptionsHelp["outputdir"] = "path for generated files";
  m_RequiredOptionsHelp["inputcpp"] = "path for cpp file to parse";

  m_OtherOptionsHelp["html"] = "set to 1 in order to generate documentation as HTML files";
  m_OtherOptionsHelp["pdf"] = "set to 1 in order to generate documentation as PDF file";
  m_OtherOptionsHelp["tplfile"] = "path to template file";


  m_Title = "Unknown Title";
  m_FuncID = "unknown.id";
  m_FuncName = "Unknown Function Name";
  m_FuncAuthor = "Unknown Function Author";
  m_FuncEmail = "Unknown Function Author Email";
  m_FuncDomain = "Unknown Function Domain";

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


void Func2DocBuddy::copyDocDirectory()
{
  if (m_InputDirPath != m_OutputDirPath)
  {
    boost::filesystem::path InputDocDirPath(m_InputDirPath.string() + "/doc");

    if (boost::filesystem::is_directory(InputDocDirPath))
    {
      std::cout << "** Processing doc directory...";
      CopyDirectoryRecursively(InputDocDirPath.string(),m_OutputDirPath.string());
      std::cout << " done" << std::endl;

    }
  }
}

// =====================================================================
// =====================================================================


void Func2DocBuddy::extractLatexDocFromCPP()
{
  std::cout << "** Extracting doumentation from C++ file...";

  std::ifstream CPPFile(m_InputFilePath.string().c_str());

  // check if file exists and if it is "openable"
  if (!CPPFile)
    throw openfluid::base::OFException("kernel","Func2DocBuddy::extractLatexDocFromCPP()","Could not open input file");

  std::string StrLine  = "";
  std::string CPPFileContent = "";

  // parse and loads file contents
  while(std::getline(CPPFile,StrLine))
  {
    CPPFileContent = CPPFileContent + StrLine + "\n";
  }

  CPPFile.close();


  std::string::size_type Index = CPPFileContent.find(m_BeginFunc2DocTag);

  if (Index != std::string::npos)
  {
    CPPFileContent.erase(0,Index+m_BeginFunc2DocTag.length());
    Index = CPPFileContent.find(m_EndFunc2DocTag);
    if (Index != std::string::npos) CPPFileContent.erase(Index,CPPFileContent.length()-Index);
  }

  m_ExtractedLatexDoc = CPPFileContent;

  std::cout << " done" << std::endl;

}


// =====================================================================
// =====================================================================


void Func2DocBuddy::cpreprocessCPP()
{
  std::cout << "** Preprocessing C++ file (using gcc)...";

  m_CProcessedFilePath = boost::filesystem::path(m_OutputDirPath.string() + "/" + m_InputFilePath.leaf() + ".func2doc");

  boost::filesystem::remove(m_CProcessedFilePath);

  std::string CommandToRun = m_CPreProcessorPath.string() + " -E -fdirectives-only -nostdinc -nostdinc++ -undef -fpreprocessed " + m_InputFilePath.string() + " > " + m_CProcessedFilePath.string() + " 2>/dev/null";

  system(CommandToRun.c_str());
  if (!boost::filesystem::is_regular(m_CProcessedFilePath))
    throw openfluid::base::OFException("kernel","Func2DocBuddy::cpreprocessCPP()","Error running c preprocessor");

  std::cout << " done" << std::endl;

}


// =====================================================================
// =====================================================================


void Func2DocBuddy::processSignature()
{
  std::cout << "** Processing function signature...";

  std::cout << " done" << std::endl;

}


// =====================================================================
// =====================================================================


void Func2DocBuddy::generateLatex()
{
  std::cout << "** Generating latex file...";

  // loading template

  std::ifstream TPLFile(m_TplFilePath.string().c_str());

  // check if file exists and if it is "openable"
  if (!TPLFile)
    throw openfluid::base::OFException("kernel","Func2DocBuddy::generateLatex()","Could not open template file");

  std::string StrLine  = "";
  m_LatexOutFile = "";

  // parse and loads file contents
  while(std::getline(TPLFile,StrLine))
  {
    m_LatexOutFile = m_LatexOutFile + StrLine + "\n";
  }

  TPLFile.close();

  // replacing values

  boost::algorithm::replace_all(m_LatexOutFile,"#title#",m_Title);
  boost::algorithm::replace_all(m_LatexOutFile,"#author#",m_FuncAuthor);
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


  std::cout << " done" << std::endl;
}

// =====================================================================
// =====================================================================


void Func2DocBuddy::buildPDF()
{

  std::cout << "** Building PDF...";

  std::string PDFCommandToRun = m_PDFLatexPath.string() + " -shell-escape -interaction=nonstopmode -output-directory="+m_OutputDirPath.string()+" "+ m_OutputLatexFilePath.string() + " > /dev/null";
  std::string BibCommandToRun = m_BibtexPath.string() + " " + boost::filesystem::path(m_OutputDirPath.string()+"/"+m_FuncID).string() + " > /dev/null";

  std::cout << " first pass...";

  std::cout << PDFCommandToRun << std::endl;
  system(PDFCommandToRun.c_str());

  std::cout << " bibliography and references...";

  system(BibCommandToRun.c_str());

  std::cout << " second pass...";

  system(PDFCommandToRun.c_str());

  std::cout << " third pass...";

  system(PDFCommandToRun.c_str());

  std::cout << " done" << std::endl;

}


// =====================================================================
// =====================================================================


void Func2DocBuddy::buildHTML()
{
  std::cout << "** Building HTML...";

  std::string CommandToRun = m_Latex2HTMLPath.string() + " -dir="+m_OutputDirPath.string()+" "+ m_OutputLatexFilePath.string();

  system(CommandToRun.c_str());

  std::cout << " done" << std::endl;
}


// =====================================================================
// =====================================================================



bool Func2DocBuddy::run()
{
#if defined __unix__ || defined __APPLE__

  setOptionIfNotSet("tplfile",FUNC2DOC_TPLFILE_PATH + "/" + FUNC2DOC_TPLFILE_PATH);
  setOptionIfNotSet("outputdir",boost::filesystem::current_path().string());
  setOptionIfNotSet("pdf","0");
  setOptionIfNotSet("html","0");

  std::cout << "Input CPP file: " << m_Options["inputcpp"] << std::endl;
  std::cout << "Output directory: " << m_Options["outputdir"] << std::endl;
  std::cout << "Template file: " << m_Options["tplfile"] << std::endl;
  std::cout << "Generate PDF: " << getYesNoFromOneZero(m_Options["pdf"]) << std::endl;
  std::cout << "Generate HTML: " << getYesNoFromOneZero(m_Options["html"]) << std::endl;


  std::vector<std::string> GCCPaths = GetFileLocationsUsingPATHEnvVar("gcc");

  if (GCCPaths.size() > 0)
  {
    m_CPreProcessorPath = boost::filesystem::path(GCCPaths[0]);
    std::cout << "Using C preprocessor: " << m_CPreProcessorPath.string() << std::endl;
  }
  else throw openfluid::base::OFException("kernel","Func2DocBuddy::run()","C preprocessor (gcc) not found");


  if (m_Options["pdf"] == "1")
  {
    std::vector<std::string> PDFLatexPaths = GetFileLocationsUsingPATHEnvVar("pdflatex");
    std::vector<std::string> BibtexPaths = GetFileLocationsUsingPATHEnvVar("bibtex");

    if (PDFLatexPaths.size() > 0 && BibtexPaths.size() > 0)
      {
        m_PDFLatexPath = boost::filesystem::path(PDFLatexPaths[0]);
        std::cout << "Using PDFLatex compiler: " << m_PDFLatexPath.string() << std::endl;
        m_BibtexPath = boost::filesystem::path(BibtexPaths[0]);
        std::cout << "Using Bibtex compiler: " << m_BibtexPath.string() << std::endl;
      }
      else std::cout << "!! PDFLatex compiler and/or Bibtex compiler not found. Skipped. " << std::endl;
  }

  if (m_Options["html"] == "1")
  {
    std::vector<std::string> Latex2HTMLPaths = GetFileLocationsUsingPATHEnvVar("latex2html");

    if (Latex2HTMLPaths.size() > 0)
      {
        m_Latex2HTMLPath = boost::filesystem::path(Latex2HTMLPaths[0]);
        std::cout << "Using Latex2HTML converter: " << m_Latex2HTMLPath.string() << std::endl;
      }
      else std::cout << "!! Latex2HTML converter not found. Skipped. " << std::endl;
  }


  m_InputFilePath = boost::filesystem::path(m_Options["inputcpp"]);
  m_OutputDirPath = boost::filesystem::path(m_Options["outputdir"]);

  if (!boost::filesystem::exists(m_InputFilePath))
    throw openfluid::base::OFException("kernel","Func2DocBuddy::run()","Input file does not exist");

  m_TplFilePath = boost::filesystem::path(m_Options["tplfile"]);
  if (!boost::filesystem::exists(m_TplFilePath))
    throw openfluid::base::OFException("kernel","Func2DocBuddy::run()","Template file does not exist");

  if (!boost::filesystem::is_directory(m_OutputDirPath)) boost::filesystem::create_directories(m_OutputDirPath);


  m_InputDirPath = m_InputFilePath.branch_path();

  copyDocDirectory();

  extractLatexDocFromCPP();

  cpreprocessCPP();

  processSignature();

  generateLatex();


  if (m_Options["pdf"] == "1") buildPDF();

  if (m_Options["html"] == "1") buildHTML();

  throw openfluid::base::OFException("under construction");

#else
  std::cout << "func2doc buddy only runs on Unix/Linux and MacOSX systems." << std::endl;
  return true;
#endif


  return true;
}


