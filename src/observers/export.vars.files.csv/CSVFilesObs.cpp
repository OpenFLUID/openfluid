/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/



/**
  @file CSVFilesObs.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <boost/foreach.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <openfluid/ware/PluggableObserver.hpp>
#include <openfluid/tools/DataHelpers.hpp>



// =====================================================================
// =====================================================================


class CSVFormat
{
  public:

    enum HeaderType { None, Info, ColnamesAsData, ColnamesAsComment, Full };

    HeaderType Header;

    std::string ColSeparator;

    std::string DateFormat;

    std::string CommentChar;

    unsigned int Precision;

    bool IsTimeIndexDateFormat;

    CSVFormat(): Header(Info), ColSeparator("\t"), DateFormat("%Y%m%dT%H%M%S"), CommentChar("#"), Precision(5), IsTimeIndexDateFormat(false)
    { };


};


// =====================================================================
// =====================================================================

class CSVFile
{
  public:
    openfluid::core::SpatialUnit* Unit;

    char* FileBuffer;

    std::ofstream FileHandle;

    std::string FileName;

    openfluid::core::VariableName_t VarName;

    CSVFile() : Unit(NULL), FileHandle(NULL), VarName("")
    { }

    ~CSVFile()
    {
      if (FileHandle.is_open()) FileHandle.close();
      delete [] FileBuffer;
    }
};



// =====================================================================
// =====================================================================


class CSVSet
{
  public:

    typedef std::list<CSVFile*> CSVFilePtrList_t;

    openfluid::core::UnitClass_t UnitClass;

    std::string UnitsIDsStr;

    bool isAllUnits;

    CSVFilePtrList_t Files;

    std::string VariablesStr;

    bool isAllVars;

    //std::list<openfluid::core::VariableName_t> Variables;

    CSVFormat* Format;

    CSVSet() : UnitClass(""), UnitsIDsStr(""), isAllUnits(false), VariablesStr(""), isAllVars(false), Format(NULL)
    { };
};


// =====================================================================
// =====================================================================


DECLARE_OBSERVER_PLUGIN


// =====================================================================
// =====================================================================


BEGIN_OBSERVER_SIGNATURE("export.vars.files.csv")
  DECLARE_NAME("Exports simulation variables to CSV files");
  DECLARE_DESCRIPTION("This observer exports variables to CSV files\n"
      "Parameters can be\n"
      "  format.<formatname>.date : the date format using the standard C date format\n"
      "  format.<formatname>.commentchar : the character for comment lines\n"
      "  format.<formatname>.header : the header type\n"
      "  format.<formatname>.precision : the precision for real values\n"
      "  set.<setname>.unitclass : the unit class of the set\n"
      "  set.<setname>.unitsIDs : the unit IDs included in the set. Use * to include all units of the class\n"
      "  set.<setname>.vars : the variable included in the set, separated by semicolons. Use * to include all variables\n"
      "  set.<setname>.format : the <formatname> used, must be defined by a format parameter");

  DECLARE_VERSION(openfluid::config::FULL_VERSION);
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

END_OBSERVER_SIGNATURE


// =====================================================================
// =====================================================================


class CSVFilesObserver : public openfluid::ware::PluggableObserver
{
  private:

    typedef std::map<std::string, CSVFormat> FormatMap_t;

    typedef std::map<std::string, CSVSet> SetMap_t;

    FormatMap_t m_Formats;

    SetMap_t m_Sets;

    std::string m_OutputDir;

    unsigned int m_BufferSize;

    std::string m_OutFileExt;

    std::string buildFilename(const std::string& SetName,
                              const openfluid::core::UnitClass_t& UnitClass,
                              const openfluid::core::UnitID_t& UnitID,
                              const openfluid::core::VariableName_t& Varname)
    {
      std::ostringstream oss;

      oss << m_OutputDir << "/" << SetName << "_" << UnitClass << UnitID << "_" << Varname << "." << m_OutFileExt;

      return boost::filesystem::path(oss.str()).string();
    }


  public:

    CSVFilesObserver() : PluggableObserver(),
    m_OutputDir(""),m_BufferSize(2*1024),m_OutFileExt("csv")
    {

    }

    // =====================================================================
    // =====================================================================


    ~CSVFilesObserver()
    {
      onFinalizedRun();
    }


    // =====================================================================
    // =====================================================================

    static CSVFormat::HeaderType StrToHeaderType(const std::string& HeaderStr)
    {
      if (HeaderStr == "none")
        return CSVFormat::None;
      else if (HeaderStr == "colnames-as-data")
        return CSVFormat::ColnamesAsData;
      else if (HeaderStr == "colnames-as-comment")
              return CSVFormat::ColnamesAsComment;
      else if (HeaderStr == "full")
        return CSVFormat::Full;
      else
        return CSVFormat::Info;
    }

    // =====================================================================
    // =====================================================================

    static std::string StrToDateFormat(const std::string& FormatStr)
    {
      if (FormatStr == "ISO")
        return "%Y%m%dT%H%M%S";
      else if (FormatStr == "6cols")
        return "%Y\t%m\t%d\t%H\t%M\t%S";
      else
        return FormatStr;
    }

    // =====================================================================
    // =====================================================================

    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      boost::property_tree::ptree ParamsPT;

      try
      {
        ParamsPT = openfluid::ware::PluggableWare::getParamsAsPropertyTree(Params);
      }
      catch (openfluid::base::FrameworkException& E)
      {
        OPENFLUID_RaiseError(E.getMessage());
      }


      if (!ParamsPT.get_child_optional("format"))
        OPENFLUID_RaiseError("No format defined");

      if (!ParamsPT.get_child_optional("set"))
        OPENFLUID_RaiseError("No set defined");


      BOOST_FOREACH(const boost::property_tree::ptree::value_type &v,ParamsPT.get_child("format"))
      {
        std::string FormatName = v.first;
        m_Formats[FormatName].ColSeparator = ParamsPT.get("format."+FormatName+".colsep","\t");
        m_Formats[FormatName].Precision = ParamsPT.get<unsigned int>("format."+FormatName+".precision",5);
        m_Formats[FormatName].DateFormat = StrToDateFormat(ParamsPT.get("format."+FormatName+".date","ISO"));
        m_Formats[FormatName].IsTimeIndexDateFormat = (m_Formats[FormatName].DateFormat == "timeindex");
        m_Formats[FormatName].CommentChar = ParamsPT.get("format."+FormatName+".commentchar","#");
        m_Formats[FormatName].Header = StrToHeaderType(ParamsPT.get("format."+FormatName+".header",""));
      }

      BOOST_FOREACH(const boost::property_tree::ptree::value_type &v,ParamsPT.get_child("set"))
      {
        std::string SetName = v.first;
        m_Sets[SetName].UnitClass = ParamsPT.get("set."+SetName+".unitclass","");
        if (m_Sets[SetName].UnitClass == "")
          OPENFLUID_RaiseError("Unit class of set "+SetName+" is undefined");


        m_Sets[SetName].UnitsIDsStr = ParamsPT.get("set."+SetName+".unitsIDs","*");
        m_Sets[SetName].VariablesStr = ParamsPT.get("set."+SetName+".vars","*");

        std::string FormatName = ParamsPT.get("set."+SetName+".format","");
        if (m_Formats.find(FormatName) == m_Formats.end())
          OPENFLUID_RaiseError("Format "+FormatName+" used by "+ SetName+" is undefined");
        else
          m_Sets[SetName].Format = &(m_Formats[FormatName]);
      }

      m_BufferSize = ParamsPT.get<unsigned int>("general.buffersize",2) * 1024;

    }


    // =====================================================================
    // =====================================================================


    void onPrepared()
    {
      OPENFLUID_GetRunEnvironment("dir.output",m_OutputDir);

      SetMap_t::iterator SetItE = m_Sets.end();
      SetMap_t::iterator SetItB = m_Sets.begin();
      SetMap_t::iterator SetIt;

      openfluid::core::SpatialUnit* TmpU;

      for (SetIt=SetItB;SetIt!=SetItE;++SetIt)
      {

        if (OPENFLUID_IsUnitClassExist((*SetIt).second.UnitClass))
        {

          std::vector<openfluid::core::VariableName_t> VarArray;
          VarArray.clear();

          if ((*SetIt).second.VariablesStr == "*")
          {
            // process all variables
            VarArray = mp_SpatialData->spatialUnits((*SetIt).second.UnitClass)->list()->begin()->variables()->getVariablesNames();
          }
          else
          {
            // process selected variables
            std::vector<openfluid::core::VariableName_t> TmpVarArray;

            TmpVarArray = openfluid::tools::splitString((*SetIt).second.VariablesStr,";");

            for (unsigned int i = 0; i < TmpVarArray.size(); i++)
            {
              if (mp_SpatialData->spatialUnits((*SetIt).second.UnitClass)->list()->begin()->variables()->isVariableExist(TmpVarArray[i]))
              {
                 VarArray.push_back(TmpVarArray[i]);
              }
              else
                OPENFLUID_RaiseWarning("Variable "+TmpVarArray[i]+" for unit class "+(*SetIt).second.UnitClass+" does not exist. Ignored.");
            }

          }


          if ((*SetIt).second.UnitsIDsStr == "*")
          {
            // all units

            OPENFLUID_UNITS_ORDERED_LOOP((*SetIt).second.UnitClass,TmpU)
            {

              for (unsigned int i = 0; i < VarArray.size(); i++)
              {
                CSVFile* CF = new CSVFile();
                CF->Unit = TmpU;
                CF->VarName = VarArray[i];
                (*SetIt).second.Files.push_back(CF);
              }
            }

          }
          else
          {
            // selected units

            openfluid::core::UnitID_t UID;

            std::vector<std::string> UIDArray = openfluid::tools::splitString((*SetIt).second.UnitsIDsStr, ";");
            for (unsigned int i = 0; i < UIDArray.size(); i++)
            {
              TmpU = NULL;
              if (openfluid::tools::convertString(UIDArray[i],&UID))
              {
                TmpU = mp_SpatialData->spatialUnit((*SetIt).second.UnitClass,UID);
                if (TmpU != NULL)
                {
                  for (unsigned int i = 0; i < VarArray.size(); i++)
                  {
                    CSVFile* CF = new CSVFile();
                    CF->Unit = TmpU;
                    CF->VarName = VarArray[i];
                    (*SetIt).second.Files.push_back(CF);
                  }
                }
                else
                  OPENFLUID_RaiseWarning("Unit #"+UIDArray[i]+" does not exist in class "+(*SetIt).second.UnitClass+". Ignored.");
              }
            }
          }
        }
        else
          OPENFLUID_RaiseWarning("Unit class "+(*SetIt).second.UnitClass+" does not exist. Ignored.");

      }


      for (SetIt=SetItB;SetIt!=SetItE;++SetIt)
      {

        CSVSet::CSVFilePtrList_t::iterator FLItB = (*SetIt).second.Files.begin();
        CSVSet::CSVFilePtrList_t::iterator FLItE = (*SetIt).second.Files.end();;
        CSVSet::CSVFilePtrList_t::iterator FLIt;

        for (FLIt=FLItB;FLIt!=FLItE;++FLIt)
        {
          // create files with correct headers

          (*FLIt)->FileBuffer = new char[m_BufferSize];
          (*FLIt)->FileHandle.rdbuf()->pubsetbuf((*FLIt)->FileBuffer,m_BufferSize);

          (*FLIt)->FileName = buildFilename((*SetIt).first,(*SetIt).second.UnitClass,(*FLIt)->Unit->getID(),(*FLIt)->VarName);
          (*FLIt)->FileHandle.open((*FLIt)->FileName.c_str(),
                                   std::ios::out);

          // add header
          if((*SetIt).second.Format->Header == CSVFormat::Info ||
             (*SetIt).second.Format->Header == CSVFormat::Full)
          {
            (*FLIt)->FileHandle << (*SetIt).second.Format->CommentChar << "========================================================================\n";
            (*FLIt)->FileHandle << (*SetIt).second.Format->CommentChar << " file: " << boost::filesystem::path((*FLIt)->FileName).leaf() << "\n";
            (*FLIt)->FileHandle << (*SetIt).second.Format->CommentChar << " date: " << boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::local_time()) << "\n";
            (*FLIt)->FileHandle << (*SetIt).second.Format->CommentChar << " unit: " << (*FLIt)->Unit->getClass() << " #" << (*FLIt)->Unit->getID() << "\n";
            (*FLIt)->FileHandle << (*SetIt).second.Format->CommentChar << " variable: " << (*FLIt)->VarName << "\n";
            (*FLIt)->FileHandle << (*SetIt).second.Format->CommentChar << "========================================================================\n";

          }

          if((*SetIt).second.Format->Header == CSVFormat::ColnamesAsComment ||
             (*SetIt).second.Format->Header == CSVFormat::Full)

          {
            if ((*SetIt).second.Format->IsTimeIndexDateFormat)
              (*FLIt)->FileHandle << (*SetIt).second.Format->CommentChar << "timeindex";
            else
              (*FLIt)->FileHandle << (*SetIt).second.Format->CommentChar << "datetime";

            (*FLIt)->FileHandle << (*SetIt).second.Format->ColSeparator << (*FLIt)->VarName << "\n";

          }

          if((*SetIt).second.Format->Header == CSVFormat::ColnamesAsData)
          {
            if ((*SetIt).second.Format->IsTimeIndexDateFormat)
              (*FLIt)->FileHandle << "timeindex";
            else
              (*FLIt)->FileHandle << "datetime";

            (*FLIt)->FileHandle << (*SetIt).second.Format->ColSeparator << (*FLIt)->VarName << "\n";
          }

          // set precision
          (*FLIt)->FileHandle << std::fixed << std::setprecision((*SetIt).second.Format->Precision);

        }

      }
    }


    // =====================================================================
    // =====================================================================


    void saveToFiles()
    {
      SetMap_t::iterator SetItE = m_Sets.end();
      SetMap_t::iterator SetItB = m_Sets.begin();
      SetMap_t::iterator SetIt;


      for (SetIt=SetItB;SetIt!=SetItE;++SetIt)
      {
        CSVSet::CSVFilePtrList_t::iterator FLItB = (*SetIt).second.Files.begin();
        CSVSet::CSVFilePtrList_t::iterator FLItE = (*SetIt).second.Files.end();;
        CSVSet::CSVFilePtrList_t::iterator FLIt;

        for (FLIt=FLItB;FLIt!=FLItE;++FLIt)
        {
          openfluid::core::Value* Val = (*FLIt)->Unit->variables()->currentValueIfIndex((*FLIt)->VarName,OPENFLUID_GetCurrentTimeIndex());

          if (Val!=NULL)
          {
            if ((*SetIt).second.Format->IsTimeIndexDateFormat)
              (*FLIt)->FileHandle << OPENFLUID_GetCurrentTimeIndex();
            else
              (*FLIt)->FileHandle << OPENFLUID_GetCurrentDate().getAsString((*SetIt).second.Format->DateFormat);
            (*FLIt)->FileHandle << (*SetIt).second.Format->ColSeparator;
            Val->writeToStream((*FLIt)->FileHandle);
            (*FLIt)->FileHandle << "\n";
          }

        }

      }

    }


    // =====================================================================
    // =====================================================================


    void onInitializedRun()
    {
      saveToFiles();
    }


    // =====================================================================
    // =====================================================================


    void onStepCompleted()
    {
      saveToFiles();
    }


    // =====================================================================
    // =====================================================================


    void onFinalizedRun()
    {
      SetMap_t::iterator SetItE = m_Sets.end();
      SetMap_t::iterator SetItB = m_Sets.begin();
      SetMap_t::iterator SetIt;

      for (SetIt=SetItB;SetIt!=SetItE;++SetIt)
      {

        CSVSet::CSVFilePtrList_t::iterator FLItB = (*SetIt).second.Files.begin();
        CSVSet::CSVFilePtrList_t::iterator FLItE = (*SetIt).second.Files.end();;
        CSVSet::CSVFilePtrList_t::iterator FLIt;

        for (FLIt=FLItB;FLIt!=FLItE;++FLIt)
          delete (*FLIt);

        (*SetIt).second.Files.clear();
      }
    }


};


// =====================================================================
// =====================================================================


DEFINE_OBSERVER_CLASS(CSVFilesObserver)
