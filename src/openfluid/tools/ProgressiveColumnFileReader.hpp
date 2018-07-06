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
  @file ProgressiveColumnFileReader.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_TOOLS_PROGRESSIVECOLUMNFILEREADER_HPP__
#define __OPENFLUID_TOOLS_PROGRESSIVECOLUMNFILEREADER_HPP__


#include <fstream>
#include <vector>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {


/**
  Progressive reader for column text files
*/
class OPENFLUID_API ProgressiveColumnFileReader
{
  private:

    std::ifstream m_File;

    std::string m_ColSeparators;


  protected:

    std::string m_FileName;


  public:

    /**
      Constructor
      @param[in] FileName the file to open
      @param[in] ColSeparators a list of columns separators. Default value is " \t\r\n"
    */
    ProgressiveColumnFileReader(const std::string& FileName,
    		                        const std::string& ColSeparators = " \t\r\n");

    virtual ~ProgressiveColumnFileReader()
    { };

    /**
      Gets the next line of data in the file, as a single string
      @param[out] Line the read line
      @return true if the next line has been read, false otherwise
    */
    bool getNextLine(std::string& Line);

    /**
      Gets the next line of data in the file, as a vector of strings
      @param[out] Values the vector of read values
      @return true if the next line has been read and splitted into a vector of strings,
      false otherwise
    */
    bool getNextLine(std::vector<std::string>& Values);

    /**
      Resets the internal iterator of the file
    */
    void reset();

    /**
      Returns the file name
      @return the file name
    */
    std::string getFileName() const { return m_FileName; };

};


} } // namespaces


#endif /* __OPENFLUID_TOOLS_PROGRESSIVECOLUMNFILEREADER_HPP__ */
