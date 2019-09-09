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
  @file CsvObserverBase.hpp

  @author Armel THONI <armel.thoni@inra.fr>
*/
 
#ifndef __CSVOBSERVERBASE_HPP__
#define __CSVOBSERVERBASE_HPP__
 
 
#include <fstream>

#include <openfluid/ware/PluggableObserver.hpp>


constexpr const char* CSV_FILES_EXT = "csv";
 
 
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

    CSVFormat() : Header(Info), ColSeparator(";"), DateFormat("%Y%m%dT%H%M%S"),
                  CommentChar("#"), Precision(5), IsTimeIndexDateFormat(false)
    {

    }

};


// =====================================================================
// =====================================================================

 
class BaseCSVFile
{
  public:

    char* FileBuffer;

    std::ofstream FileHandle;

    std::string FileName;

    BaseCSVFile() :
      FileBuffer(nullptr), FileHandle(nullptr)
    { }

    ~BaseCSVFile()
    {
      if (FileHandle.is_open())
      {
        FileHandle.close();
      }
      delete [] FileBuffer;
    }
};
 
 
// =====================================================================
// =====================================================================


class CSVFilesObserverBase : public openfluid::ware::PluggableObserver
{
  protected:

    std::string m_OutputDir;

    unsigned int m_BufferSize;

    std::string m_OutFileExt;


  public:

    CSVFilesObserverBase() : PluggableObserver(),
                             m_OutputDir(""), m_BufferSize(2*1024), m_OutFileExt(CSV_FILES_EXT)
    {

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
     
     
    virtual void saveToFiles()=0;
};


// =====================================================================
// =====================================================================


inline CSVFormat::HeaderType StrToHeaderType(const std::string& HeaderStr)
{
  if (HeaderStr == "none")
  {
    return CSVFormat::None;
  }
  else if (HeaderStr == "colnames-as-data")
  {
    return CSVFormat::ColnamesAsData;
  }
  else if (HeaderStr == "colnames-as-comment")
  {
    return CSVFormat::ColnamesAsComment;
  }
  else if (HeaderStr == "full")
  {
    return CSVFormat::Full;
  }
  else
  {
    return CSVFormat::Info;
  }
}


// =====================================================================
// =====================================================================


inline std::string HeaderTypeToStr(CSVFormat::HeaderType HType)
{
  if (HType == CSVFormat::ColnamesAsComment)
  {
    return "colnames-as-comment";
  }
  else if (HType == CSVFormat::ColnamesAsData)
  {
    return "colnames-as-data";
  }
  else if (HType == CSVFormat::Full)
  {
    return "full";
  }
  else if (HType == CSVFormat::None)
  {
    return "none";
  }
  else
  {
    return "";
  }
}


// =====================================================================
// =====================================================================


inline std::string StrToDateFormat(const std::string& FormatStr)
{
  if (FormatStr == "ISO")
  {
    return "%Y%m%dT%H%M%S";
  }
  else if (FormatStr == "6cols")
  {
    return "%Y\t%m\t%d\t%H\t%M\t%S";
  }
  else
  {
    return FormatStr;
  }
}

 
 // =====================================================================
 // =====================================================================


typedef std::map<std::string, CSVFormat> FormatMap_t;


// =====================================================================
// =====================================================================


inline std::string buildTimeHeader(const CSVFormat& Format, const openfluid::core::VariableName_t& VarName)
{ 
  std::ostringstream HeaderSStr;
  if(Format.Header == CSVFormat::ColnamesAsComment || Format.Header == CSVFormat::Full)
  {
    if (Format.IsTimeIndexDateFormat)
    {
      HeaderSStr << Format.CommentChar << "timeindex";
    }
    else
    {
      HeaderSStr << Format.CommentChar << "datetime";
    }
  
    HeaderSStr << Format.ColSeparator << VarName << "\n";
  }

  if(Format.Header == CSVFormat::ColnamesAsData)
  {
    if (Format.IsTimeIndexDateFormat)
    {
      HeaderSStr << "timeindex";
    }
    else
    {
      HeaderSStr << "datetime";
    }
  
    HeaderSStr << Format.ColSeparator << VarName << "\n";
  }
  
  return HeaderSStr.str();
}

 
 #endif /* __CSVOBSERVERBASE_HPP__ */
