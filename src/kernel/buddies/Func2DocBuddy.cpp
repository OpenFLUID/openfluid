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
  m_BeginSignatureTag = "BEGIN_SIGNATURE_HOOK";
  m_EndSignatureTag = "END_SIGNATURE_HOOK";
  m_BeginFunc2DocTag = "<func2doc>";
  m_EndFunc2DocTag = "</func2doc>";


  m_RequiredOptionsHelp["outputdir"] = "path for generated files";
  m_RequiredOptionsHelp["inputcpp"] = "path for cpp file to parse";

  m_OtherOptionsHelp["html"] = "set to 1 in order to generate documentation as HTML files";
  m_OtherOptionsHelp["pdf"] = "set to 1 in order to generate documentation as PDF file";
  m_OtherOptionsHelp["tplfile"] = "path to template file";

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

  // extraction of func2doc content

  // C preprocessing of original file using gcc

  // extraction of signature lines from C preprocessed file

  // processing of signature lines

  // build of latex file content

  // save of latex file content

  std::cout << " done" << std::endl;
}

// =====================================================================
// =====================================================================


void Func2DocBuddy::buildPDF()
{

  std::cout << "** Building PDF...";

  std::cout << " first pass...";

  std::cout << " bibliography and references...";

  std::cout << " second pass...";

  std::cout << " third pass...";

  std::cout << " done" << std::endl;

}


// =====================================================================
// =====================================================================


void Func2DocBuddy::buildHTML()
{
  std::cout << "** Building HTML...";

  std::cout << " done" << std::endl;
}


// =====================================================================
// =====================================================================



bool Func2DocBuddy::run()
{
#ifdef __unix__

  setOptionIfNotSet("tplfile",FUNCDOC_TPLFILE_PATH + "/" + FUNCDOC_TPLFILE_PATH);
  setOptionIfNotSet("outputdir",boost::filesystem::current_path().string());
  setOptionIfNotSet("pdf","0");
  setOptionIfNotSet("html","0");

  std::cout << "Input CPP file: " << m_Options["inputcpp"] << std::endl;
  std::cout << "Output directory: " << m_Options["outputdir"] << std::endl;
  std::cout << "Template file: " << m_Options["tplfile"] << std::endl;
  std::cout << "Generate PDF: " << getYesNoFromOneZero(m_Options["pdf"]) << std::endl;
  std::cout << "Generate HTML: " << getYesNoFromOneZero(m_Options["html"]) << std::endl;

  m_CPreProcessorPath = boost::filesystem::path("/usr/bin/gcc");

  m_InputFilePath = boost::filesystem::path(m_Options["inputcpp"]);
  m_OutputDirPath = boost::filesystem::path(m_Options["outputdir"]);

  if (!boost::filesystem::exists(m_InputFilePath))
    throw openfluid::base::OFException("kernel","Func2DocBuddy::run()","Input file does not exist");

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
  std::cout << "func2doc buddy only runs on unix systems." << std::endl;
  return true;
#endif

  throw openfluid::base::OFException("under construction");

  return true;
}

