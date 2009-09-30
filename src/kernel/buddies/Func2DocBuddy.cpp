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




#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <fstream>

#include "Func2DocBuddy.h"
#include "openfluid-core.h"

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


void Func2DocBuddy::generateLatex()
{
  std::cout << "** Generating latex file...";

  // extraction of func2doc content

  // C preprocessing using gcc

  // extraction of signature lines

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

  setOptionIfNotSet("outputdir",boost::filesystem::current_path().string());
  setOptionIfNotSet("pdf","0");
  setOptionIfNotSet("html","0");

  std::cout << "Input CPP file: " << m_Options["inputcpp"] << std::endl;
  std::cout << "Output directory: " << m_Options["outputdir"] << std::endl;
  std::cout << "Template file: " << m_Options["tplfile"] << std::endl;
  std::cout << "Generate PDF: " << getYesNoFromOneZero(m_Options["pdf"]) << std::endl;
  std::cout << "Generate HTML: " << getYesNoFromOneZero(m_Options["html"]) << std::endl;

  boost::filesystem::path InputCPPPath(m_Options["inputcpp"]);
  boost::filesystem::path OutputDirPath(m_Options["outputdir"]);

  if (!boost::filesystem::exists(InputCPPPath))
    throw openfluid::base::OFException("kernel","Func2DocBuddy::run()","Input file does not exist");

  if (!boost::filesystem::exists(OutputDirPath))
    throw openfluid::base::OFException("kernel","Func2DocBuddy::run()","Output directory does not exist");

  std::ifstream FileContent(InputCPPPath.string().c_str());

  // check if file exists and if it is "openable"
  if (!FileContent)
    throw openfluid::base::OFException("kernel","Func2DocBuddy::run()","Could not open input file");

  std::string StrLine;


  // parse and loads file contents
  while(std::getline(FileContent,StrLine))
  {
    m_CPPFile = m_CPPFile + " " + StrLine;
  }

  FileContent.close();

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
