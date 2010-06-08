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
  \file ConvertBuddy.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/buddies/ConvertBuddy.hpp>
#include <openfluid/tools.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <libxml/parser.h>
#include <libxml/xpath.h>


namespace openfluid { namespace buddies {


ConvertBuddy::ConvertBuddy(openfluid::buddies::BuddiesListener* Listener) :
              OpenFLUIDBuddy(Listener)
{
  m_RequiredOptionsHelp["convmode"] = "Conversion mode. Available modes are: 13_14, 14_15";
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
    throw openfluid::base::OFException("OpenFLUID framework","ConvertBuddy::convert_13_14_defs()","Wrong unit class");


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
              throw openfluid::base::OFException("OpenFLUID framework","ConvertBuddy::convert_13_14_defs()","Wrong surface format ("+InputFilePath.string()+")");

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
      throw openfluid::base::OFException("OpenFLUID framework","ConvertBuddy::convert_13_14_defs()","Cannot open or wrong file format ("+InputFilePath.string()+")");

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
	std::vector<std::string> DataFiles = openfluid::tools::GetFilesByExt(m_Options["inputdir"],"ddata.xml",true);
	std::string StrLine, FileContent;

	for (unsigned int i=0;i<DataFiles.size();i++)
	{
		std::string CurrInFile = boost::filesystem::path(DataFiles[i]).leaf();
		boost::filesystem::path CurrInPath(m_Options["inputdir"]+"/"+CurrInFile);
		boost::filesystem::path CurrOutPath(m_Options["outputdir"]+"/"+CurrInFile);

		boost::filesystem::remove(CurrOutPath);
		std::ifstream InputFile(CurrInPath.string().c_str());

		FileContent = "";
		while(std::getline(InputFile,StrLine))
		{
		   FileContent = FileContent + StrLine + "\n";
		}

		InputFile.close();

    boost::algorithm::replace_all(FileContent,"<openfluid>","<openfluid>\n  <domain>");
    boost::algorithm::replace_all(FileContent,"</openfluid>","  </domain>\n</openfluid>");

    boost::algorithm::replace_all(FileContent,"<distridata","  <inputdata");
    boost::algorithm::replace_all(FileContent,"</distridata","  </inputdata");

    boost::algorithm::replace_all(FileContent,"datacat=\"ini\"","");
    boost::algorithm::replace_all(FileContent,"datacat=\"param\"","");

    boost::algorithm::replace_all(FileContent,"<columns","  <columns");
    boost::algorithm::replace_all(FileContent,"</columns","  </columns");

    boost::algorithm::replace_all(FileContent,"<data","  <data");
    boost::algorithm::replace_all(FileContent,"</data","  </data");


		std::ofstream OutputFile(CurrOutPath.string().c_str(),std::ios::out);
    OutputFile << FileContent;
    OutputFile.close();

	}
}

// =====================================================================
// =====================================================================

void ConvertBuddy::convert_13_14_output()
{

  boost::filesystem::path FileInPath(m_Options["inputdir"]+"/output.xml");
  boost::filesystem::path FileOutPath(m_Options["outputdir"]+"/output.xml");

  std::string StrLine, FileContent;

  boost::filesystem::remove(FileOutPath);
  std::ifstream InputFile(FileInPath.string().c_str());

  FileContent = "";
  while(std::getline(InputFile,StrLine))
  {
    FileContent = FileContent + StrLine + "\n";
  }

  InputFile.close();

  boost::algorithm::replace_all(FileContent,"<autooutfiles","<files");
  boost::algorithm::replace_all(FileContent,"</autooutfiles","</files");

  boost::algorithm::replace_all(FileContent,"<SUout","<set unitsclass=\"SU\"");
  boost::algorithm::replace_all(FileContent,"</SUout","</set");

  boost::algorithm::replace_all(FileContent,"<RSout","<set unitsclass=\"RS\"");
  boost::algorithm::replace_all(FileContent,"</RSout","</set");

  boost::algorithm::replace_all(FileContent,"<GUout","<set unitsclass=\"GU\"");
  boost::algorithm::replace_all(FileContent,"</GUout","</set");

  boost::algorithm::replace_all(FileContent,"filesuffix=","name=");
  boost::algorithm::replace_all(FileContent,"selection=","unitsIDs=");

  std::ofstream OutputFile(FileOutPath.string().c_str(),std::ios::out);
  OutputFile << FileContent;
  OutputFile.close();



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

  std::vector<std::string> EventsFiles = openfluid::tools::GetFilesByExt(m_Options["inputdir"],"events.xml",true);

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


void ConvertBuddy::convert_14_15_events()
{
  std::vector<std::string> EventsFiles = openfluid::tools::GetFilesByExt(m_Options["inputdir"],"events.xml",true);

  for (unsigned int i=0;i<EventsFiles.size();i++)
  {
    std::string CurrInFile = boost::filesystem::path(EventsFiles[i]).leaf();
    std::string CurrOutFile = boost::filesystem::change_extension(boost::filesystem::path(EventsFiles[i]),".fluidx").leaf();

    std::ifstream InputFile(boost::filesystem::path(m_Options["inputdir"]+"/"+CurrInFile).string().c_str());
    std::string FileContent = "";
    std::string StrLine;

    while(std::getline(InputFile,StrLine))
    {
       FileContent = FileContent + StrLine + "\n";
    }

    InputFile.close();

    boost::algorithm::replace_all(FileContent,"<openfluid>","<openfluid>\n  <domain>");
    boost::algorithm::replace_all(FileContent,"</openfluid>","  </domain>\n</openfluid>");

    std::ofstream OutputFile(boost::filesystem::path(m_Options["outputdir"]+"/"+CurrOutFile).string().c_str(),std::ios::out);
    OutputFile << FileContent;
    OutputFile.close();

  }

}

// =====================================================================
// =====================================================================


void ConvertBuddy::convert_14_15_data()
{

  std::vector<std::string> DataFiles = openfluid::tools::GetFilesByExt(m_Options["inputdir"],"ddata.xml",true);

  for (unsigned int i=0;i<DataFiles.size();i++)
  {

    xmlDocPtr Doc = NULL;
    xmlNodePtr Root = NULL;

    Doc = xmlParseFile(DataFiles[i].c_str());

    std::string CurrInFile = boost::filesystem::path(DataFiles[i]).leaf();
    std::string CurrOutFile = boost::filesystem::change_extension(boost::filesystem::path(CurrInFile),".fluidx").string();

    if (Doc != NULL)
    {
      Root =  xmlDocGetRootElement(Doc);

      if (Root != NULL)
      {
        xmlXPathInit();
        xmlXPathContextPtr XPathCtxt = xmlXPathNewContext(Doc);
        xmlNodePtr XPathNode;
        xmlChar* xmlColumns;
        xmlXPathObjectPtr XPathRes;
        xmlNodePtr InputdataNode;

        // processing of inputdata tag
        XPathRes = xmlXPathEvalExpression((const xmlChar*)"/openfluid/domain/inputdata", XPathCtxt);
        if (XPathRes == NULL || XPathRes->nodesetval->nodeNr != 1)
          throw openfluid::base::OFException("OpenFLUID framework","ConvertBuddy::convert_14_15_data","wrong file format in" + DataFiles[i] + ", wrong, duplicate or missing <inputdata>");
        InputdataNode = XPathRes->nodesetval->nodeTab[0];


        // processing of columns tag
        XPathRes = xmlXPathEvalExpression((const xmlChar*)"/openfluid/domain/inputdata/columns", XPathCtxt);
        if (XPathRes == NULL || XPathRes->nodesetval->nodeNr != 1)
          throw openfluid::base::OFException("OpenFLUID framework","ConvertBuddy::convert_14_15_data","wrong file format in" + DataFiles[i] + ", wrong, duplicate or missing <columns>");
        XPathNode = XPathRes->nodesetval->nodeTab[0];

        xmlColumns = xmlGetProp(XPathNode,(const xmlChar*)"order");
        if (xmlColumns == NULL)
          throw openfluid::base::OFException("OpenFLUID framework","ConvertBuddy::convert_14_15_data","wrong file format in" + DataFiles[i] + ", missing order attribute in <columns>");
        xmlSetProp(InputdataNode,(const xmlChar*)"colorder",xmlColumns);


        // processing of data tag
        XPathRes = xmlXPathEvalExpression((const xmlChar*)"/openfluid/domain/inputdata/data/text()", XPathCtxt);
        if (XPathRes == NULL || XPathRes->nodesetval->nodeNr != 1)
          throw openfluid::base::OFException("OpenFLUID framework","ConvertBuddy::convert_14_15_data","wrong file format in" + DataFiles[i] + ", wrong, duplicate or missing <data>");
        XPathNode = XPathRes->nodesetval->nodeTab[0];
        xmlNodeSetContent(InputdataNode,xmlNodeGetContent(XPathNode));

        xmlXPathFreeContext(XPathCtxt);

        xmlSaveFile(boost::filesystem::path(m_Options["outputdir"]+"/"+CurrOutFile).string().c_str(),Doc);
      }
      else
      {
        throw openfluid::base::OFException("OpenFLUID framework","ConvertBuddy::convert_14_15_data","file " + DataFiles[i] + " is empty");
      }
    }
    else
    {
      throw openfluid::base::OFException("OpenFLUID framework","ConvertBuddy::convert_14_15_data","file " + DataFiles[i] + " cannot be parsed");
    }

  }


}

// =====================================================================
// =====================================================================


void ConvertBuddy::copy_14_15_model_run_output_defs()
{
  boost::filesystem::remove(boost::filesystem::path(m_Options["outputdir"]+"/model.fluidx"));
  boost::filesystem::copy_file(boost::filesystem::path(m_Options["inputdir"]+"/model.xml"),
                               boost::filesystem::path(m_Options["outputdir"]+"/model.fluidx"));

  boost::filesystem::remove(boost::filesystem::path(m_Options["outputdir"]+"/run.fluidx"));
  boost::filesystem::copy_file(boost::filesystem::path(m_Options["inputdir"]+"/run.xml"),
                               boost::filesystem::path(m_Options["outputdir"]+"/run.fluidx"));

  boost::filesystem::remove(boost::filesystem::path(m_Options["outputdir"]+"/output.fluidx"));
  boost::filesystem::copy_file(boost::filesystem::path(m_Options["inputdir"]+"/output.xml"),
                               boost::filesystem::path(m_Options["outputdir"]+"/output.fluidx"));


  std::vector<std::string> DefsFiles = openfluid::tools::GetFilesByExt(m_Options["inputdir"],"ddef.xml",true);

  for (unsigned int i=0;i<DefsFiles.size();i++)
  {
    std::string CurrInFile = boost::filesystem::path(DefsFiles[i]).leaf();
    std::string CurrOutFile = boost::filesystem::change_extension(boost::filesystem::path(CurrInFile),".fluidx").leaf();

    boost::filesystem::remove(boost::filesystem::path(m_Options["outputdir"]+"/"+CurrOutFile));
    boost::filesystem::copy_file(boost::filesystem::path(DefsFiles[i]),
                                 boost::filesystem::path(m_Options["outputdir"]+"/"+CurrOutFile));
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


void ConvertBuddy::convert_14_15()
{
  copy_14_15_model_run_output_defs();
  convert_14_15_data();
  convert_14_15_events();
}

// =====================================================================
// =====================================================================


bool ConvertBuddy::run()
{

  mp_Listener->onInfo("Conversion mode: " + m_Options["convmode"]);
  mp_Listener->onInfo("Input directory: " + m_Options["inputdir"]);
  mp_Listener->onInfo("Output directory: " + m_Options["outputdir"]);

  boost::filesystem::path InputDirPath(m_Options["inputdir"]);
  boost::filesystem::path OutputDirPath(m_Options["outputdir"]);


  if (!boost::filesystem::exists(InputDirPath))
    throw openfluid::base::OFException("OpenFLUID framework","ConvertBuddy::run()","Input directory not found");

  if (!boost::filesystem::exists(OutputDirPath))
  {
    if (!boost::filesystem::create_directories(OutputDirPath))
      throw openfluid::base::OFException("OpenFLUID framework","ConvertBuddy::run()","Could not create output directory ");
  }

  bool FoundMode = false;

  if (m_Options["convmode"] == "13_14")
  {
    convert_13_14();
    FoundMode = true;
  }
  if (m_Options["convmode"] == "14_15")
  {
    convert_14_15();
    FoundMode = true;
  }

  if (!FoundMode) throw openfluid::base::OFException("OpenFLUID framework","ConvertBuddy::run()","Unknown conversion mode");


  return true;
}


} } //namespaces

