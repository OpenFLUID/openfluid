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
  @file ChronFileInterpolator.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_TOOLS_CHRONFILEINTERPOLATOR_HPP__
#define __OPENFLUID_TOOLS_CHRONFILEINTERPOLATOR_HPP__


#include <list>
#include <iostream>

#include <openfluid/core/DateTime.hpp>
#include <openfluid/tools/ChronologicalSerie.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {


class OPENFLUID_API ChronFileInterpolator
{
  public:

    enum PreProcess { PREPROCESS_NONE, PREPROCESS_CUMULATE };


  private:

    void checkPreload();

    void checkPostload();


  protected:

    std::string m_InFilePath;

    std::string m_InDateFormat;

    std::string m_InColumnSeparators;

    std::string m_InCommentChar;

    std::string m_OutFilePath;

    std::string m_OutDateFormat;

    std::string m_OutColumnSeparator;

    std::string m_OutCommentChar;

    openfluid::core::DateTime m_BeginDate;

    openfluid::core::DateTime m_EndDate;

    openfluid::core::Duration_t m_DeltaT;

    PreProcess m_PreProcess;

    void loadInFile(ChronologicalSerie& Data);

    static void displayChronSerie(ChronologicalSerie& Data)
    {
      ChronologicalSerie::iterator it;
      ChronologicalSerie::iterator itb = Data.begin();
      ChronologicalSerie::iterator ite = Data.end();

      std::cout << "--------------------------------" << std::endl;
      for (it=itb;it!=ite;++it)
      {
        std::cout << (*it).first.getAsISOString() << " -> " << (*it).second << std::endl;
      }
      std::cout << "--------------------------------" << std::endl;
    }


  public:


    ChronFileInterpolator(const std::string& InFilePath, const std::string& OutFilePath,
                          const openfluid::core::DateTime& BeginDate, const openfluid::core::DateTime& EndDate,
                          const openfluid::core::Duration_t& DeltaT, PreProcess PrePcs = PREPROCESS_NONE);

    virtual ~ChronFileInterpolator();


    virtual void runInterpolation() = 0;


    std::string getInColumnSeparators() const
    {
      return m_InColumnSeparators;
    }

    void setInColumnSeparators(const std::string& InColumnSeparators)
    {
      m_InColumnSeparators = InColumnSeparators;
    }

    std::string getInCommentChar() const
    {
      return m_InCommentChar;
    }

    void setInCommentChar(const std::string& InCommentChar)
    {
      m_InCommentChar = InCommentChar;
    }

    std::string getInDateFormat() const
    {
      return m_InDateFormat;
    }

    void setInDateFormat(const std::string& InDateFormat)
    {
      m_InDateFormat = InDateFormat;
    }

    std::string getInFilePath() const
    {
      return m_InFilePath;
    }

    std::string getOutColumnSeparator() const
    {
      return m_OutColumnSeparator;
    }

    void setOutColumnSeparator(const std::string& OutColumnSeparator)
    {
      m_OutColumnSeparator = OutColumnSeparator;
    }

    std::string getOutCommentChar() const
    {
      return m_OutCommentChar;
    }

    void setOutCommentChar(const std::string& OutCommentChar)
    {
      m_OutCommentChar = OutCommentChar;
    }

    std::string getOutDateFormat() const
    {
      return m_OutDateFormat;
    }

    void setOutDateFormat(const std::string& OutDateFormat)
    {
      m_OutDateFormat = OutDateFormat;
    }

    std::string getOutFilePath() const
    {
      return m_OutFilePath;
    }

    openfluid::core::DateTime getBeginDate() const
    {
      return m_BeginDate;
    }

    openfluid::core::Duration_t getDeltaT() const
    {
      return m_DeltaT;
    }

    openfluid::core::DateTime getEndDate() const
    {
      return m_EndDate;
    }

    PreProcess getPreProcess() const
    {
      return m_PreProcess;
    }
};


} } // namespaces


#endif /* __OPENFLUID_TOOLS_CHRONFILEINTERPOLATOR_HPP__ */
