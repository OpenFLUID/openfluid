/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file NewDataBuddy.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "NewDataBuddy.h"
#include "openfluid-core.h"

#include <boost/filesystem/convenience.hpp>
#include <fstream>


NewDataBuddy::NewDataBuddy() : OpenFLUIDBuddy()
{
  m_RequiredOptionsHelp["outputdir"] = "Output directory for generated dataset";
}


// =====================================================================
// =====================================================================


NewDataBuddy::~NewDataBuddy()
{

}

// =====================================================================
// =====================================================================


std::string NewDataBuddy::getXMLHeader()
{
  return "<?xml version=\"1.0\" standalone=\"yes\"?>\n<openfluid>";
}


// =====================================================================
// =====================================================================


std::string NewDataBuddy::getXMLFooter()
{
  return "</openfluid>";
}


// =====================================================================
// =====================================================================


void NewDataBuddy::generateRunXML()
{
  std::ostringstream Content;

  Content << getXMLHeader() << std::endl;
  Content << "  <run>" << std::endl;
  Content << std::endl;
  Content << "    <!-- period example -->" << std::endl;
  Content << "    <period begin=\"2001-01-01 00:00:00\" end=\"2001-01-31 23:59:59\" />" << std::endl;
  Content << std::endl;
  Content << "    <!-- time step example -->" << std::endl;
  Content << "    <deltat>3600</deltat>" << std::endl;
  Content << std::endl;
  Content << "  </run>" << std::endl;
  Content << getXMLFooter() << std::endl;

  boost::filesystem::path FilePath(m_OutputDirPath.string()+"/run.fluidx");

  std::ofstream OutFile;

  OutFile.open(FilePath.string().c_str(),std::ios::out);
  OutFile << Content.str();
  OutFile.close();
}

// =====================================================================
// =====================================================================


void NewDataBuddy::generateModelXML()
{
  std::ostringstream Content;

  Content << getXMLHeader() << std::endl;
  Content << "  <model>" << std::endl;
  Content << std::endl;
  Content << "    <!-- fake function example -->" << std::endl;
  Content << "    <function fileID=\"tests.fakefunction\" />" << std::endl;
  Content << std::endl;
  Content << "  </model>" << std::endl;
  Content << getXMLFooter() << std::endl;

  boost::filesystem::path FilePath(m_OutputDirPath.string()+"/model.fluidx");

  std::ofstream OutFile;

  OutFile.open(FilePath.string().c_str(),std::ios::out);
  OutFile << Content.str();
  OutFile.close();
}

// =====================================================================
// =====================================================================


void NewDataBuddy::generateOutputXML()
{
  std::ostringstream Content;

  Content << getXMLHeader() << std::endl;
  Content << "  <output>" << std::endl;
  Content << std::endl;
  Content << "    <!-- dtformat can be predefined (6cols,iso) or using the strftime() format, default is iso -->" << std::endl;
  Content << "    <!-- colsep default is \t -->" << std::endl;
  Content << "    <files colsep=\" \" dtformat=\"%Y %m %d %H %M %S\" commentchar=\"%\">" << std::endl;
  Content << "      <set unitsclass=\"unitsA\" name=\"full\" unitsIDs=\"*\" vars=\"*\" />" << std::endl;
  Content << "      <set unitsclass=\"unitsB\" name=\"full\" unitsIDs=\"*\" vars=\"*\" />" << std::endl;
  Content << "    </files>" << std::endl;
  Content << std::endl;
  Content << "  </output>" << std::endl;
  Content << getXMLFooter() << std::endl;

  boost::filesystem::path FilePath(m_OutputDirPath.string()+"/output.fluidx");

  std::ofstream OutFile;

  OutFile.open(FilePath.string().c_str(),std::ios::out);
  OutFile << Content.str();
  OutFile.close();
}

// =====================================================================
// =====================================================================


void NewDataBuddy::generateEventsXML()
{
  std::ostringstream Content;

  Content << getXMLHeader() << std::endl;
  Content << "  <domain>" << std::endl;
  Content << "    <calendar>" << std::endl;
  Content << std::endl;
  Content << "      <event name=\"\" category=\"example\" unitclass=\"unitsA\" unitID=\"1\" date=\"2001-01-18 12:25:33\">" << std::endl;
  Content << "        <info key=\"when\" value=\"during\"/>" << std::endl;
  Content << "        <info key=\"where\" value=\"1\"/>" << std::endl;
  Content << "        <info key=\"numeric\" value=\"1.15\"/>" << std::endl;
  Content << "        <info key=\"string\" value=\"EADGBE\"/>" << std::endl;
  Content << "      </event>" << std::endl;
  Content << std::endl;
  Content << "      <event name=\"\" category=\"example\" unitclass=\"unitsB\" unitID=\"1\" date=\"2001-05-01 06:00:17\">" << std::endl;
  Content << "        <info key=\"when\" value=\"after\"/>" << std::endl;
  Content << "        <info key=\"where\" value=\"12\"/>" << std::endl;
  Content << "        <info key=\"numeric\" value=\"1.15\"/>" << std::endl;
  Content << "        <info key=\"string\" value=\"EADGBE\"/>" << std::endl;
  Content << "      </event>" << std::endl;
  Content << std::endl;
  Content << "    </calendar>" << std::endl;
  Content << "  </domain>" << std::endl;
  Content << getXMLFooter() << std::endl;

  boost::filesystem::path FilePath(m_OutputDirPath.string()+"/eventsAandB.events.fluidx");

  std::ofstream OutFile;

  OutFile.open(FilePath.string().c_str(),std::ios::out);
  OutFile << Content.str();
  OutFile.close();
}

// =====================================================================
// =====================================================================


void NewDataBuddy::generateDDefXML()
{
  std::ostringstream Content;

  Content << getXMLHeader() << std::endl;
  Content << "  <domain>" << std::endl;
  Content << "    <definition>" << std::endl;
  Content << std::endl;
  Content << "      <!-- example: units (A,3) and (A,8) are connected to unit (B,1) -->" << std::endl;
  Content << std::endl;
  Content << "      <unit class=\"unitsA\" ID=\"3\" pcsorder=\"1\">" << std::endl;
  Content << "        <to class=\"unitsB\" ID=\"1\" />" << std::endl;
  Content << "      </unit>" << std::endl;
  Content << "      <unit class=\"unitsA\" ID=\"8\" pcsorder=\"1\">" << std::endl;
  Content << "        <to class=\"unitsB\" ID=\"1\" />" << std::endl;
  Content << "      </unit>" << std::endl;
  Content << std::endl;
  Content << "      <unit class=\"unitsB\" ID=\"1\" pcsorder=\"1\" />" << std::endl;
  Content << std::endl;
  Content << "    </definition>" << std::endl;
  Content << "  </domain>" << std::endl;
  Content << getXMLFooter() << std::endl;

  boost::filesystem::path FilePath(m_OutputDirPath.string()+"/unitsAandB.ddef.fluidx");

  std::ofstream OutFile;

  OutFile.open(FilePath.string().c_str(),std::ios::out);
  OutFile << Content.str();
  OutFile.close();
}

// =====================================================================
// =====================================================================


void NewDataBuddy::generateDDataXML()
{
  std::ostringstream ContentA, ContentB;
  boost::filesystem::path FilePath;
  std::ofstream OutFileA, OutFileB;


  ContentA.clear();
  ContentA << getXMLHeader() << std::endl;
  ContentA << "  <domain>" << std::endl;
  ContentA << "    <inputdata unitclass=\"unitsA\" colorder=\"foodata\" >" << std::endl;
  ContentA << std::endl;
  ContentA << "3 1.25" << std::endl;
  ContentA << "8 2.385" << std::endl;
  ContentA << std::endl;
  ContentA << "    </inputdata>" << std::endl;
  ContentA << "  </domain>" << std::endl;
  ContentA << getXMLFooter() << std::endl;

  FilePath = boost::filesystem::path(m_OutputDirPath.string()+"/unitsA.ddata.fluidx");

  OutFileA.open(FilePath.string().c_str(),std::ios::out);
  OutFileA << ContentA.str();
  OutFileA.close();



  ContentB.clear();
  ContentB << getXMLHeader() << std::endl;
  ContentB << "  <domain>" << std::endl;
  ContentB << "    <inputdata unitclass=\"unitsB\" colorder=\"bardata;otherbardata\">" << std::endl;
  ContentB << std::endl;
  ContentB << "1 0.00035 185" << std::endl;
  ContentB << std::endl;
  ContentB << "    </inputdata>" << std::endl;
  ContentB << "  </domain>" << std::endl;
  ContentB << getXMLFooter() << std::endl;

  FilePath = boost::filesystem::path(m_OutputDirPath.string()+"/unitsB.ddata.fluidx");

  OutFileB.open(FilePath.string().c_str(),std::ios::out);
  OutFileB << ContentB.str();
  OutFileB.close();

}

// =====================================================================
// =====================================================================


bool NewDataBuddy::run()
{
  std::cout << "Output directory: " << m_Options["outputdir"] << std::endl;

  m_OutputDirPath = boost::filesystem::path(m_Options["outputdir"]);

  if (!boost::filesystem::is_directory(m_OutputDirPath)) boost::filesystem::create_directories(m_OutputDirPath);

  generateModelXML();
  generateRunXML();
  generateOutputXML();
  generateDDefXML();
  generateDDataXML();
  generateEventsXML();

  return true;
}
