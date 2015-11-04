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


#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "CSVObsTools.hpp"

#include <openfluid/ware/PluggableObserver.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>


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

    CSVFile() :
      Unit(nullptr), FileBuffer(nullptr), FileHandle(nullptr)
    { }

    ~CSVFile()
    {
      if (FileHandle.is_open()) FileHandle.close();
        delete [] FileBuffer;
    }
};


// =====================================================================
// =====================================================================


class CSVSetFiles
{
  public:

    typedef std::list<CSVFile*> CSVFilePtrList_t;

    CSVFilePtrList_t Files;

    CSVFormat* Format;

    CSVSet SetDefinition;

    CSVSetFiles() : Format(nullptr)
    { };
};


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
      "  set.<setname>.unitsclass : the unit class of the set\n"
      "  set.<setname>.unitsIDs : the unit IDs included in the set. Use * to include all units of the class\n"
      "  set.<setname>.vars : the variable included in the set, separated by semicolons. "
         "Use * to include all variables\n"
      "  set.<setname>.format : the <formatname> used, must be defined by a format parameter");

  DECLARE_VERSION(openfluid::config::VERSION_FULL);
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

END_OBSERVER_SIGNATURE


// =====================================================================
// =====================================================================


class CSVFilesObserver : public openfluid::ware::PluggableObserver
{
  private:

    typedef std::map<std::string, CSVSetFiles> SetFilesMap_t;

    FormatMap_t m_Formats;

    SetFilesMap_t m_SetsFiles;

    std::string m_OutputDir;

    unsigned int m_BufferSize;

    std::string m_OutFileExt;


  public:

    CSVFilesObserver() : PluggableObserver(),
    m_OutputDir(""),m_BufferSize(2*1024),m_OutFileExt(CSV_FILES_EXT)
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


    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      openfluid::ware::WareParamsTree ParamsTree;

      try
      {
        ParamsTree.setParams(Params);
      }
      catch (openfluid::base::FrameworkException& E)
      {
        OPENFLUID_RaiseError(E.getMessage());
      }


      std::vector<std::string> ParsingMsgs = parseFormatsFromParamsTree(ParamsTree,m_Formats);
      for (auto Msg: ParsingMsgs)
        OPENFLUID_LogWarning(Msg);

      SetMap_t TmpSetMap;
      ParsingMsgs = parseSetsFromParamsTree(ParamsTree,TmpSetMap);
      for (auto Msg: ParsingMsgs)
        OPENFLUID_LogWarning(Msg);

      for (auto& Format : m_Formats)
        Format.second.DateFormat = StrToDateFormat(Format.second.DateFormat);


      for (auto& Set : TmpSetMap)
      {
        m_SetsFiles[Set.first].SetDefinition = Set.second;

        // check units class is defined
        if (m_SetsFiles[Set.first].SetDefinition.UnitsClass.empty())
          OPENFLUID_RaiseError("Units class for " + Set.first + " is undefined");

        // check format is defined
        std::string FormatName = m_SetsFiles[Set.first].SetDefinition.FormatName;
        if (m_Formats.find(FormatName) == m_Formats.end())
          OPENFLUID_RaiseError("Format "+FormatName+" used by "+ Set.first +" is undefined");
        else
          m_SetsFiles[Set.first].Format = &(m_Formats[FormatName]);
      }

      long BufferSize;
      ParamsTree.getValueUsingFullKey("general.buffersize","2").toInteger(BufferSize);
      m_BufferSize = BufferSize * 1024;

    }


    // =====================================================================
    // =====================================================================


    void onPrepared()
    {
      OPENFLUID_GetRunEnvironment("dir.output",m_OutputDir);

      openfluid::core::SpatialUnit* TmpU;

      for (auto& SetFiles : m_SetsFiles)
      {

        if (OPENFLUID_IsUnitsClassExist(SetFiles.second.SetDefinition.UnitsClass))
        {

          std::vector<openfluid::core::VariableName_t> VarArray;
          VarArray.clear();

          if (SetFiles.second.SetDefinition.VariablesStr == "*")
          {
            // process all variables
            VarArray =
                mp_SpatialData->spatialUnits(SetFiles.second.SetDefinition.UnitsClass)
                    ->list()->begin()->variables()->getVariablesNames();
          }
          else
          {
            // process selected variables
            std::vector<openfluid::core::VariableName_t> TmpVarArray;

            TmpVarArray = openfluid::tools::splitString(SetFiles.second.SetDefinition.VariablesStr,";");

            for (unsigned int i = 0; i < TmpVarArray.size(); i++)
            {
              if (mp_SpatialData->spatialUnits(SetFiles.second.SetDefinition.UnitsClass)
                      ->list()->begin()->variables()->isVariableExist(TmpVarArray[i]))
              {
                 VarArray.push_back(TmpVarArray[i]);
              }
              else
                OPENFLUID_LogWarning("Variable "+TmpVarArray[i]+" for units class "+
                                     SetFiles.second.SetDefinition.UnitsClass+" does not exist. Ignored.");
            }

          }


          if (SetFiles.second.SetDefinition.UnitsIDsStr == "*")
          {
            // all units

            OPENFLUID_UNITS_ORDERED_LOOP(SetFiles.second.SetDefinition.UnitsClass,TmpU)
            {

              for (unsigned int i = 0; i < VarArray.size(); i++)
              {
                CSVFile* CF = new CSVFile();
                CF->Unit = TmpU;
                CF->VarName = VarArray[i];
                SetFiles.second.Files.push_back(CF);
              }
            }

          }
          else
          {
            // selected units

            openfluid::core::UnitID_t UID;

            std::vector<std::string> UIDArray =
                openfluid::tools::splitString(SetFiles.second.SetDefinition.UnitsIDsStr, ";");

            for (unsigned int i = 0; i < UIDArray.size(); i++)
            {
              TmpU = nullptr;
              if (openfluid::tools::convertString(UIDArray[i],&UID))
              {
                TmpU = mp_SpatialData->spatialUnit(SetFiles.second.SetDefinition.UnitsClass,UID);
                if (TmpU != nullptr)
                {
                  for (unsigned int i = 0; i < VarArray.size(); i++)
                  {
                    CSVFile* CF = new CSVFile();
                    CF->Unit = TmpU;
                    CF->VarName = VarArray[i];
                    SetFiles.second.Files.push_back(CF);
                  }
                }
                else
                  OPENFLUID_LogWarning("Unit #"+UIDArray[i]+" does not exist in class "+
                                       SetFiles.second.SetDefinition.UnitsClass+". Ignored.");
              }
            }
          }
        }
        else
          OPENFLUID_LogWarning("Unit class "+SetFiles.second.SetDefinition.UnitsClass+" does not exist. Ignored.");

      }


      for (auto& SetFiles : m_SetsFiles)
      {
        for (auto& File : SetFiles.second.Files)
        {
          // create file
          File->FileBuffer = new char[m_BufferSize];
          File->FileHandle.rdbuf()->pubsetbuf(File->FileBuffer,m_BufferSize);

          File->FileName = buildFilename(m_OutputDir,m_OutFileExt,
                                         SetFiles.first,SetFiles.second.SetDefinition.UnitsClass,
                                         File->Unit->getID(),File->VarName);
          File->FileHandle.open(File->FileName.c_str(),
                                   std::ios::out);

          // add header
           File->FileHandle << buildHeader(*SetFiles.second.Format,File->FileName,
                                          File->Unit->getClass(),File->Unit->getID(),
                                          File->VarName);

          // set precision
          File->FileHandle << std::fixed << std::setprecision(SetFiles.second.Format->Precision);

        }

      }
    }


    // =====================================================================
    // =====================================================================


    void saveToFiles()
    {
      SetFilesMap_t::iterator SetItE = m_SetsFiles.end();
      SetFilesMap_t::iterator SetItB = m_SetsFiles.begin();
      SetFilesMap_t::iterator SetIt;


      for (SetIt=SetItB;SetIt!=SetItE;++SetIt)
      {
        CSVSetFiles::CSVFilePtrList_t::iterator FLItB = (*SetIt).second.Files.begin();
        CSVSetFiles::CSVFilePtrList_t::iterator FLItE = (*SetIt).second.Files.end();;
        CSVSetFiles::CSVFilePtrList_t::iterator FLIt;

        for (FLIt=FLItB;FLIt!=FLItE;++FLIt)
        {
          openfluid::core::Value* Val =
              (*FLIt)->Unit->variables()->currentValueIfIndex((*FLIt)->VarName,OPENFLUID_GetCurrentTimeIndex());

          if (Val!=nullptr)
          {
            if ((*SetIt).second.Format->IsTimeIndexDateFormat)
              (*FLIt)->FileHandle << OPENFLUID_GetCurrentTimeIndex();
            else
              (*FLIt)->FileHandle << OPENFLUID_GetCurrentDate().getAsString((*SetIt).second.Format->DateFormat);
            (*FLIt)->FileHandle << (*SetIt).second.Format->ColSeparator;
            Val->writeQuotedToStream((*FLIt)->FileHandle);
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
      SetFilesMap_t::iterator SetItE = m_SetsFiles.end();
      SetFilesMap_t::iterator SetItB = m_SetsFiles.begin();
      SetFilesMap_t::iterator SetIt;

      for (SetIt=SetItB;SetIt!=SetItE;++SetIt)
      {

        CSVSetFiles::CSVFilePtrList_t::iterator FLItB = (*SetIt).second.Files.begin();
        CSVSetFiles::CSVFilePtrList_t::iterator FLItE = (*SetIt).second.Files.end();;
        CSVSetFiles::CSVFilePtrList_t::iterator FLIt;

        for (FLIt=FLItB;FLIt!=FLItE;++FLIt)
          delete (*FLIt);

        (*SetIt).second.Files.clear();
      }
    }


};


// =====================================================================
// =====================================================================


DEFINE_OBSERVER_CLASS(CSVFilesObserver)

DEFINE_WARE_LINKUID(WARE_LINKUID)

