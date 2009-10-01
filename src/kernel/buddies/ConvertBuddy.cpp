/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ConvertBuddy.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "ConvertBuddy.h"
#include "openfluid-tools.h"
#include "AppTools.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>
#include <fstream>


ConvertBuddy::ConvertBuddy() : OpenFLUIDBuddy()
{
  m_RequiredOptionsHelp["conv"] = "Conversion mode";
  m_RequiredOptionsHelp["inputdir"] = "Input directory for dataset to convert";
  m_RequiredOptionsHelp["outputdir"] = "Output directory for converted dataset";
}


// =====================================================================
// =====================================================================


ConvertBuddy::~ConvertBuddy()
{

}

// =====================================================================
// =====================================================================

void ConvertBuddy::convert_13_14_defs(std::string UnitClass)
{
  boost::filesystem::path InputFilePath(m_Options["inputdir"] + "/" + UnitClass + "defs.dat");
  boost::filesystem::path OutputFilePath(m_Options["outputdir"] + "/" + UnitClass + ".ddef.xml");
  boost::filesystem::path DataOutputFilePath(m_Options["outputdir"] + "/" + UnitClass + "geometry.ddata.xml");
  std::ofstream OutputFile, DataOutputFile;
  std::string OutputFileContent = "";
  std::string DataOutputFileContent = "";
  long int ToID;
  std::string Str;
  double Surf;

  int ColCount = 0;

  OutputFileContent = "<?xml version=\"1.0\" standalone=\"yes\"?>\n<openfluid>\n  <domain>\n    <definition>\n";

  if (UnitClass == "SU")
  {
    ColCount = 8;
    DataOutputFileContent = "<?xml version=\"1.0\" standalone=\"yes\"?>\n<openfluid>\n  <domain>\n    <inputdata unitclass=\"SU\">\n      <columns order=\"area;slope;flowdist\" />\n      <data>\n\n";
  }


  if (UnitClass == "RS")
  {
    ColCount = 10;
    DataOutputFileContent = "<?xml version=\"1.0\" standalone=\"yes\"?>\n<openfluid>\n  <domain>\n    <inputdata unitclass=\"RS\">\n      <columns order=\"length;width;height;slope\" />\n      <data>\n\n";
  }

  if (UnitClass == "GU")
  {
    ColCount = 4;
    DataOutputFileContent = "<?xml version=\"1.0\" standalone=\"yes\"?>\n<openfluid>\n  <domain>\n    <inputdata unitclass=\"GU\">\n      <columns order=\"area;substlevel\" />\n      <data>\n\n";
  }

  if (ColCount == 0)
    throw openfluid::base::OFException("kernel","ConvertBuddy::convert_13_14_defs()","Wrong unit class");


  if (boost::filesystem::exists(InputFilePath))
  {
    openfluid::tools::ColumnTextParser FileParser("%");
    if (FileParser.loadFromFile(InputFilePath.string()) && (FileParser.getLinesCount() > 0) && (FileParser.getColsCount() == ColCount))
    {

      for (int CurrentLine=0;CurrentLine<FileParser.getLinesCount();CurrentLine++)
      {

        // *********** processing SUs *******************
        if (UnitClass == "SU")
        {
          OutputFileContent = OutputFileContent + "      <unit class=\"SU\" ID=\""+ FileParser.getValue(CurrentLine,0) +"\" pcsorder=\""+ FileParser.getValue(CurrentLine,7) +"\">\n";

          FileParser.getLongValue(CurrentLine,4,&ToID);
          if (ToID >= 0)
          {
            if (FileParser.getValue(CurrentLine,3) == "R") OutputFileContent = OutputFileContent + "        <to class=\"RS\" ID=\""+ FileParser.getValue(CurrentLine,4) +"\" />\n";
            else OutputFileContent = OutputFileContent + "        <to class=\"SU\" ID=\""+ FileParser.getValue(CurrentLine,4) +"\" />\n";
          }

          // compute SU areas sum for GU area
          FileParser.getLongValue(CurrentLine,6,&ToID);
          if (ToID >= 0)
          {
            if (!FileParser.getDoubleValue(CurrentLine,1,&Surf))
              throw openfluid::base::OFException("kernel","ConvertBuddy::convert_13_14_defs()","Wrong surface format ("+InputFilePath.string()+")");

            if (m_GUSurfaces.find(ToID) == m_GUSurfaces.end()) m_GUSurfaces[ToID] = 0.0;
            m_GUSurfaces[ToID] = m_GUSurfaces[ToID] + Surf;

            OutputFileContent = OutputFileContent + "        <to class=\"GU\" ID=\""+ FileParser.getValue(CurrentLine,6) +"\" />\n";
          }

          OutputFileContent = OutputFileContent + "      </unit>\n";

          DataOutputFileContent = DataOutputFileContent + FileParser.getValue(CurrentLine,0) + "\t" + FileParser.getValue(CurrentLine,1) + "\t" + FileParser.getValue(CurrentLine,2) + "\t" + FileParser.getValue(CurrentLine,5) + "\n";
        }


        // *********** processing RSs *******************
        if (UnitClass == "RS")
        {
          OutputFileContent = OutputFileContent + "      <unit class=\"RS\" ID=\""+ FileParser.getValue(CurrentLine,0) +"\" pcsorder=\""+ FileParser.getValue(CurrentLine,9) +"\">\n";

          FileParser.getLongValue(CurrentLine,3,&ToID);
          if (ToID >= 0) OutputFileContent = OutputFileContent + "        <to class=\"RS\" ID=\""+ FileParser.getValue(CurrentLine,3) +"\" />\n";

          FileParser.getLongValue(CurrentLine,8,&ToID);
          if (ToID >= 0) OutputFileContent = OutputFileContent + "        <to class=\"GU\" ID=\""+ FileParser.getValue(CurrentLine,8) +"\" />\n";

          OutputFileContent = OutputFileContent + "      </unit>\n";

          DataOutputFileContent = DataOutputFileContent + FileParser.getValue(CurrentLine,0) + "\t" + FileParser.getValue(CurrentLine,4) + "\t" + FileParser.getValue(CurrentLine,5) + "\t" + FileParser.getValue(CurrentLine,6) + "\t" + FileParser.getValue(CurrentLine,7) + "\n";
        }


        // *********** processing GUs *******************
        if (UnitClass == "GU")
        {
          OutputFileContent = OutputFileContent + "      <unit class=\"GU\" ID=\""+ FileParser.getValue(CurrentLine,0) +"\" pcsorder=\""+ FileParser.getValue(CurrentLine,3) +"\">\n";

          FileParser.getLongValue(CurrentLine,2,&ToID);
          if (ToID >= 0) OutputFileContent = OutputFileContent + "        <to class=\"GU\" ID=\""+ FileParser.getValue(CurrentLine,2) +"\" />\n";

          OutputFileContent = OutputFileContent + "      </unit>\n";

          double CurrentGUSurf = 0.0;
          FileParser.getLongValue(CurrentLine,0,&ToID);
          if (ToID >= 0)
          {
            if (m_GUSurfaces.find(ToID) != m_GUSurfaces.end()) CurrentGUSurf = m_GUSurfaces[ToID];
          }

          openfluid::tools::ConvertValue(CurrentGUSurf,&Str);
          DataOutputFileContent = DataOutputFileContent + FileParser.getValue(CurrentLine,0) + "\t" + Str + "\t" + FileParser.getValue(CurrentLine,1) + "\n";
        }

      }
    }
    else
      throw openfluid::base::OFException("kernel","ConvertBuddy::convert_13_14_defs()","Cannot open or wrong file format ("+InputFilePath.string()+")");

  }


  OutputFileContent = OutputFileContent + "    </definition>\n  </domain>\n</openfluid>\n\n\n";
  DataOutputFileContent = DataOutputFileContent + "\n      </data>\n    </inputdata>\n  </domain>\n</openfluid>\n\n\n";


  OutputFile.open(OutputFilePath.string().c_str(),std::ios::out);
  OutputFile << OutputFileContent;
  OutputFile.close();

  DataOutputFile.open(DataOutputFilePath.string().c_str(),std::ios::out);
  DataOutputFile << DataOutputFileContent;
  DataOutputFile.close();

}

// =====================================================================
// =====================================================================

void ConvertBuddy::convert_13_14_data()
{
  throw openfluid::base::OFException("under construction");
}

// =====================================================================
// =====================================================================

void ConvertBuddy::convert_13_14_output()
{
  throw openfluid::base::OFException("under construction");
}

// =====================================================================
// =====================================================================

void ConvertBuddy::convert_13_14_model_run_events()
{
  boost::filesystem::remove(boost::filesystem::path(m_Options["outputdir"]+"/model.xml"));
  boost::filesystem::copy_file(boost::filesystem::path(m_Options["inputdir"]+"/model.xml"),
                               boost::filesystem::path(m_Options["outputdir"]+"/model.xml"));

  boost::filesystem::remove(boost::filesystem::path(m_Options["outputdir"]+"/run.xml"));
  boost::filesystem::copy_file(boost::filesystem::path(m_Options["inputdir"]+"/run.xml"),
                               boost::filesystem::path(m_Options["outputdir"]+"/run.xml"));

  std::vector<std::string> EventsFiles = GetFilesByExt(m_Options["inputdir"],"events.xml",true);

  for (unsigned int i=0;i<EventsFiles.size();i++)
  {
    std::string CurrFile = boost::filesystem::path(EventsFiles[i]).leaf();

    boost::filesystem::remove(boost::filesystem::path(m_Options["outputdir"]+"/"+CurrFile));
    boost::filesystem::copy_file(boost::filesystem::path(EventsFiles[i]),
                                 boost::filesystem::path(m_Options["outputdir"]+"/"+CurrFile));
  }

}

// =====================================================================
// =====================================================================


void ConvertBuddy::convert_13_14()
{
  convert_13_14_model_run_events();
  convert_13_14_defs("SU");
  convert_13_14_defs("RS");
  convert_13_14_defs("GU");
  convert_13_14_data();
  convert_13_14_output();
}

// =====================================================================
// =====================================================================


bool ConvertBuddy::run()
{

  std::cout << "Conversion mode: " << m_Options["convmode"] << std::endl;
  std::cout << "Input directory: " << m_Options["inputdir"] << std::endl;
  std::cout << "Output directory: " << m_Options["outputdir"] << std::endl;

  boost::filesystem::path InputDirPath(m_Options["inputdir"]);
  boost::filesystem::path OutputDirPath(m_Options["outputdir"]);


  if (!boost::filesystem::exists(InputDirPath))
    throw openfluid::base::OFException("kernel","ConvertBuddy::run()","Input directory not found");

  if (!boost::filesystem::exists(OutputDirPath))
  {
    if (!boost::filesystem::create_directories(OutputDirPath))
      throw openfluid::base::OFException("kernel","ConvertBuddy::run()","Could not create output directory ");
  }

  if (m_Options["convmode"] == "13_14")
  {
    convert_13_14();
  }
  else throw openfluid::base::OFException("kernel","ConvertBuddy::run()","Unknown conversion mode");


  return true;
}

