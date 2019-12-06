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
  @file MultiCSVFilesObs.cpp

  @author Armel THONI <armel.thoni@inra.fr>
*/


#include <chrono>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include <openfluid/tools/Filesystem.hpp>

#include "MultiCSVObsTools.hpp"


// =====================================================================
// =====================================================================


class CSVMulticolsSetFile
{
  public:
    
    BaseCSVFile* File;

    MultiCSVFormat* Format;

    CSVMultiSet SetDefinition;
    
    std::string ColumnsHeaders;

    CSVMulticolsSetFile() : Format(nullptr)
    { }
};


// =====================================================================
// =====================================================================


BEGIN_OBSERVER_SIGNATURE("export.vars.files.csv-multicols")
  DECLARE_NAME("Exports simulation variables to multi-column CSV files");
  DECLARE_DESCRIPTION("This observer exports variables to CSV files\n"
      "Parameters can be\n"
      "  naemptylinekeep : boolean, set to true to keep lines containing NA for every column\n"
      "  format.<formatname>.date : the date format using the standard C date format\n"
      "  format.<formatname>.commentchar : the character for comment lines\n"
      "  format.<formatname>.header : the header type\n"
      "  format.<formatname>.missingvalue : the string used when data not available\n"
      "  format.<formatname>.precision : the precision for real values\n"
      "  format.<formatname>.float-format : the floating point system used for real values (auto/fixed/scientific)\n"
      "  set.<setname>.format : the <formatname> used, must be defined by a format parameter.\n"
      "  set.<setname>.selection : the wanted variables, following the pattern: \n"
      "    <UnitsClass1>#<UnitNumber1>:<VarName1>%<digitsPrecision1>;<UnitsClass2>#...\n"
      "    Float format can be adjusted at column level by adding e (for scientific) "
                                                              "or f (for fixed) after thre precision value.\n"
      "      Example: U#1:myVar%3f;U#1:myOtherVar%4e");

  DECLARE_VERSION(openfluid::config::VERSION_FULL);
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

END_OBSERVER_SIGNATURE


// =====================================================================
// =====================================================================


class CSVMultiColFilesObserver : public CSVFilesObserverBase
{
  private:

    typedef std::map<std::string, CSVMulticolsSetFile> MultiColSetFilesMap_t;

    MultiFormatMap_t m_Formats;

    MultiColSetFilesMap_t m_SetsFiles;
    
    bool m_KeepNA;


  public:
    
    ~CSVMultiColFilesObserver()
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

      m_KeepNA = false;
      ParamsTree.root().getChildValue("naemptylinekeep", m_KeepNA).toBoolean(m_KeepNA);
      
      std::vector<std::string> ParsingMsgs = parseMultiColsFormatsFromParamsTree(ParamsTree,m_Formats);
      for (auto Msg: ParsingMsgs)
      {
        OPENFLUID_LogWarning(Msg);
      }

      MultiSetMap_t TmpSetMap;
      ParsingMsgs = parseMultiColsSetsFromParamsTree(ParamsTree,TmpSetMap);
      for (auto Msg: ParsingMsgs)
      {
        OPENFLUID_LogWarning(Msg);
      }

      for (auto& Format : m_Formats)
      {
        Format.second.DateFormat = StrToDateFormat(Format.second.DateFormat);
      }

      for (auto& Set : TmpSetMap)
      {
        m_SetsFiles[Set.first].SetDefinition = Set.second;
        // check units class is defined
        if (m_SetsFiles[Set.first].SetDefinition.Selection.empty())
        {
          OPENFLUID_RaiseError("Selection for " + Set.first + " is undefined");
        }

        // check format is defined
        std::string FormatName = m_SetsFiles[Set.first].SetDefinition.FormatName;
        if (m_Formats.find(FormatName) == m_Formats.end())
        {
          OPENFLUID_RaiseError("Format "+FormatName+" used by "+ Set.first +" is undefined");
        }
        else
        {
          m_SetsFiles[Set.first].Format = &(m_Formats[FormatName]);
        }
        
        m_SetsFiles[Set.first].SetDefinition.SelectionList = stringSelectionToClassIDVarList(Set.second.Selection,
                                                                          m_SetsFiles[Set.first].Format->Precision,
                                                                          m_SetsFiles[Set.first].Format->FloatFormat);

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
        std::string ColumnsHeaders;
        bool FirstColumn = true;
        
        for (const auto& Triplet : SetFiles.second.SetDefinition.SelectionList)
        {
          // UNIT CLASS
          std::vector<openfluid::core::UnitsClass_t> UnitsClassArray;
          if (Triplet.UnitsClassesStr == "*")
          {
            openfluid::core::UnitsListByClassMap_t::const_iterator UnitsIt;
            for (UnitsIt = mp_SpatialData->allSpatialUnitsByClass()->begin();
                 UnitsIt != mp_SpatialData->allSpatialUnitsByClass()->end();
                 ++UnitsIt )
            {
              UnitsClassArray.push_back((*UnitsIt).first);
            }
          }
          else if (OPENFLUID_IsUnitsClassExist(Triplet.UnitsClassesStr))
          {
            UnitsClassArray.push_back(Triplet.UnitsClassesStr);
          }
          else
          {
            OPENFLUID_LogWarning("Unit class "+Triplet.UnitsClassesStr+" does not exist. Ignored.");
          }
          std::sort(UnitsClassArray.begin(), UnitsClassArray.end());
          
          // UNITS
          for (const openfluid::core::UnitsClass_t UnitsClass : UnitsClassArray)
          {
            std::vector<openfluid::core::UnitID_t> UnitIDArray;
            if (Triplet.UnitsIDsStr == "*")
            {
              // all units
              OPENFLUID_UNITS_ORDERED_LOOP(UnitsClass,TmpU)
              {
                UnitIDArray.push_back(TmpU->getID());
              }
            }
            else
            {
              TmpU = mp_SpatialData->spatialUnit(UnitsClass,std::atoi(Triplet.UnitsIDsStr.c_str()));
              if (TmpU != nullptr)
              {
                UnitIDArray.push_back(TmpU->getID());
              }
              else
              {
                OPENFLUID_LogWarning("Unit #"+Triplet.UnitsIDsStr+" does not exist in class "+
                                     UnitsClass+". Ignored.");
              }
            }
            std::sort(UnitIDArray.begin(), UnitIDArray.end());
            
            //VARS
            std::vector<openfluid::core::VariableName_t> VarArray;
            VarArray.clear();

            if (Triplet.VariablesStr == "*")
            {
              // process all variables
              VarArray =
                  mp_SpatialData->spatialUnits(UnitsClass)
                      ->list()->begin()->variables()->getVariablesNames();
            }
            else
            {
              // process selected variables
              if (mp_SpatialData->spatialUnits(UnitsClass)
                      ->list()->begin()->variables()->isVariableExist(Triplet.VariablesStr))
              {
                 VarArray.push_back(Triplet.VariablesStr);
              }
              else
              {
                OPENFLUID_LogWarning("Variable "+Triplet.VariablesStr+" for units class "+
                                     UnitsClass+" does not exist. Ignored.");
              }
            }
            std::sort(VarArray.begin(), VarArray.end());

            
            for (openfluid::core::UnitID_t CurrentID : UnitIDArray)
            {
              for (openfluid::core::VariableName_t CurrentVar : VarArray)
              {
                ClassIDVar CurrentTriplet(UnitsClass, std::to_string(CurrentID), CurrentVar, Triplet.Precision,
                                          Triplet.FloatFormat);
                SetFiles.second.SetDefinition.ExpandedSelection.push_back(CurrentTriplet);
                
                if (FirstColumn)
                {
                  FirstColumn = false;
                }
                else
                {
                  ColumnsHeaders += SetFiles.second.Format->ColSeparator;
                }
                ColumnsHeaders += CurrentTriplet.GetClassIDVarString(false);
              }
            }
          }
        }
        
        SetFiles.second.File = new BaseCSVFile();
        SetFiles.second.ColumnsHeaders = ColumnsHeaders;
      }


      for (auto& SetFiles : m_SetsFiles)
      {
        // create file
        SetFiles.second.File->FileBuffer = new char[m_BufferSize];
        SetFiles.second.File->FileHandle.rdbuf()->pubsetbuf(SetFiles.second.File->FileBuffer,m_BufferSize);

        SetFiles.second.File->FileName = buildMultiColsFilename(m_OutputDir,m_OutFileExt,
                                       SetFiles.first);
        SetFiles.second.File->FileHandle.open(SetFiles.second.File->FileName.c_str(),
                                 std::ios::out);

        // add header
         SetFiles.second.File->FileHandle << buildMultiColsdHeader(*SetFiles.second.Format,
                                                                   SetFiles.second.File->FileName,
                                                                   SetFiles.second.ColumnsHeaders);

        // set precision
        SetFiles.second.File->FileHandle << std::fixed << std::setprecision(SetFiles.second.Format->Precision);

      }
    }


    // =====================================================================
    // =====================================================================


    void saveToFiles()
    {
      for (auto& SetFiles : m_SetsFiles)
      {
        std::stringstream LineHandle;
        if (SetFiles.second.Format->IsTimeIndexDateFormat)
        {
          LineHandle << OPENFLUID_GetCurrentTimeIndex();
        }
        else
        {
          LineHandle << OPENFLUID_GetCurrentDate().getAsString(SetFiles.second.Format->DateFormat);
        }
        
        bool IsValue = false;
        for (ClassIDVar Column : SetFiles.second.SetDefinition.ExpandedSelection)
        {
          LineHandle << SetFiles.second.Format->ColSeparator;
          
          openfluid::core::Value* Val =
              mp_SpatialData->spatialUnit(Column.UnitsClassesStr, 
                                          std::stoi(Column.UnitsIDsStr.c_str()))->variables()->currentValueIfIndex(
                                                     Column.VariablesStr,OPENFLUID_GetCurrentTimeIndex());

          if (Val!=nullptr)
          {
            IsValue = true;
            setStreamFormat(LineHandle, Column.Precision, Column.FloatFormat);
            
            Val->writeQuotedToStream(LineHandle);
          }
          else // if val not found, use missing value string
          {
            LineHandle << SetFiles.second.Format->MissingValueString;
          }
        }
        LineHandle << "\n";
        if (IsValue || m_KeepNA)
        {
          SetFiles.second.File->FileHandle << LineHandle.str();
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void onFinalizedRun()
    {
      for (auto& SetFiles : m_SetsFiles)
      {
        delete SetFiles.second.File;
        SetFiles.second.File = 0; // avoid double delete issues
      }
    }


};


// =====================================================================
// =====================================================================


DEFINE_OBSERVER_CLASS(CSVMultiColFilesObserver)

DEFINE_WARE_LINKUID(WARE_LINKUID)

