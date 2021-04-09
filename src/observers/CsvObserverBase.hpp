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
 
 
#include <algorithm>
#include <fstream>
#include <vector>

#include <openfluid/core/DateTime.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/ware/PluggableObserver.hpp>


constexpr const char* CSV_FILES_EXT = "csv";
 
 
// =====================================================================
// =====================================================================


inline void setStreamFormat(std::ostream& Stream, const unsigned int Precision, const std::string& FloatFormat)
{
  // set precision
  if (FloatFormat == "fixed")
  {
    Stream << std::fixed;
  }
  else if (FloatFormat == "scientific")
  {
    Stream << std::scientific;
  }
  else
  {
    Stream << std::defaultfloat;
  }
  
  Stream << std::setprecision(Precision);
}
 
 
// =====================================================================
// =====================================================================


class CSVFormat
{
  public:

    enum class HeaderType { None, Info, ColnamesAsData, ColnamesAsComment, Full };

    HeaderType Header;
    
    std::string ColSeparator;
    
    std::string DateFormat;
    
    std::string CommentChar;
    
    unsigned int Precision;

    std::string FloatFormat;

    bool IsTimeIndexDateFormat;

    CSVFormat() : Header(HeaderType::Info), ColSeparator(";"), DateFormat("%Y%m%dT%H%M%S"),
                  CommentChar("#"), Precision(5), FloatFormat("default"), IsTimeIndexDateFormat(false)
    {

    }
    
    void adaptStreamFormat(std::ostream& Stream)
    {
      setStreamFormat(Stream, Precision, FloatFormat);
    }
        
    std::vector<std::string> generateFormatFields(const std::string& FormatName);

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
      FileBuffer(nullptr), FileHandle()
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
    return CSVFormat::HeaderType::None;
  }
  else if (HeaderStr == "colnames-as-data")
  {
    return CSVFormat::HeaderType::ColnamesAsData;
  }
  else if (HeaderStr == "colnames-as-comment")
  {
    return CSVFormat::HeaderType::ColnamesAsComment;
  }
  else if (HeaderStr == "full")
  {
    return CSVFormat::HeaderType::Full;
  }
  else
  {
    return CSVFormat::HeaderType::Info;
  }
}


// =====================================================================
// =====================================================================


inline std::string HeaderTypeToStr(CSVFormat::HeaderType HType)
{
  if (HType == CSVFormat::HeaderType::ColnamesAsComment)
  {
    return "colnames-as-comment";
  }
  else if (HType == CSVFormat::HeaderType::ColnamesAsData)
  {
    return "colnames-as-data";
  }
  else if (HType == CSVFormat::HeaderType::Full)
  {
    return "full";
  }
  else if (HType == CSVFormat::HeaderType::None)
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
  std::string UpperFormatStr = FormatStr;
  std::for_each(UpperFormatStr.begin(), UpperFormatStr.end(), [](char & c){c = ::toupper(c);});

  if (UpperFormatStr == "ISO")
  {
    return "%Y%m%dT%H%M%S";
  }
  else if (UpperFormatStr == "ISOEXT")
  {
    return "%Y-%m-%dT%H:%M:%S";
  }
  else if (UpperFormatStr == "6COLS")
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
  if(Format.Header == CSVFormat::HeaderType::ColnamesAsComment || Format.Header == CSVFormat::HeaderType::Full)
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

  if(Format.Header == CSVFormat::HeaderType::ColnamesAsData)
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


// =====================================================================
// =====================================================================


inline openfluid::core::StringValue basicParseFormatsFromParamsTree(
                       const openfluid::core::Tree<std::string,openfluid::core::StringValue>& Format, 
                       const std::string& Key)
{
  if (Key == "colsep")
  {
    return Format.getChildValue(Key,"\t");
  }
  else if (Key == "precision")
  {
    return Format.getChildValue(Key,5);
  }
  else if (Key == "float-format")
  {
    return Format.getChildValue(Key,"auto");
  }
  else if (Key == "date")
  {
    return Format.getChildValue(Key,"ISO");
  }
  else if (Key == "commentchar")
  {
    return Format.getChildValue(Key, "#");
  }
  else if (Key == "header")
  {
    return Format.getChildValue(Key, "");
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Key for basic parsing not found " + Key);
  }
}


// =====================================================================
// =====================================================================


inline std::vector<std::string> CSVFormat::generateFormatFields(const std::string& FormatName)
{
  std::string VisibleColSeparator = ColSeparator;
  openfluid::tools::stringReplace(VisibleColSeparator, "\t", "\\t");
  return {
           FormatName, 
           VisibleColSeparator,
           DateFormat, 
           std::to_string(Precision),
           FloatFormat,
           HeaderTypeToStr(Header),
           CommentChar
         };
}


// =====================================================================
// =====================================================================


inline std::vector<openfluid::core::DateTime> getPreviewDateTimes()
{
  return {
          openfluid::core::DateTime(2010,7,30,16,30,0), 
          openfluid::core::DateTime(2010,7,30,16,45,18), 
          openfluid::core::DateTime(2010,7,30,17,52,22), 
          openfluid::core::DateTime(2010,7,30,18,0,00), 
          openfluid::core::DateTime(2010,7,30,19,0,0)
        };
}


// =====================================================================
// =====================================================================


inline std::vector<double> getPreviewValues(bool missing=false)
{
  if (missing)
  {
    return {3.896554654, 19.2,  0.0, 0.000000523};
  }
  else
  {
    return {1.5, 3.896554654, 19.2,  0.0, 0.000000523};
  }
}


#endif /* __CSVOBSERVERBASE_HPP__ */
