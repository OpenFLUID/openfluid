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
  @file ColumnTextParser.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_TOOLS_COLUMNTEXTPARSER_HPP__
#define __OPENFLUID_TOOLS_COLUMNTEXTPARSER_HPP__


#include <vector>
#include <string>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {


/**
  Class for column file management and handling
*/
class OPENFLUID_API ColumnTextParser
{

  protected:

  private:

    std::string m_Delimiter;

    std::string m_CommentSymbol;

    unsigned int m_LinesCount;
    
    unsigned int m_ColsCount;

    std::vector<std::vector<std::string> > m_Contents;

    std::vector<std::string> tokenizeLine(const std::string& Line);

    std::vector<std::string> tokenizeString(const std::string& String);

    bool checkContents();

    bool isCommentLineStr(const std::string& LineStr);

    bool isEmptyLineStr(const std::string& LineStr);


  public:

    /**
      Constructor
    */
    ColumnTextParser(const std::string& CommentLineSymbol = "", const std::string& Delimiter = " \t\r\n");

    /**
      Destructor
    */
    ~ColumnTextParser();


    /**
      Loads a column text file
      @param[in] Filename the full path of the file to load
      @return true if everything went fine
    */
    bool loadFromFile(const std::string& Filename);

    /**
      Parses a one-line delimiter-separated string to set contents
      ex: "147.2 18 15 25 36 51.3 25.1 15", for 4 columns
      @param[in] Contents the string to parse
      @param[in] ColumnsNbr the number of columns (for splitting the sting into lines)
      @return true if everything went fine
    */
    bool setFromString(const std::string& Contents, unsigned int ColumnsNbr);

    /**
      Returns the value at a specified row-column, as a string
      @param[in] Line the line number of the value (first line is 0)
      @param[in] Column the column number of the value (first column is 0)
      @return the requested value
    */
    std::string getValue(unsigned int Line, unsigned int Column) const;

    /**
      Gets the value at a specified row-column, as a string
      @param[in] Line the line number of the value (first line is 0)
      @param[in] Column the column number of the value (first column is 0)
      @param[out] Value the requested value if exists
      @return true if the value has been found, false otherwise
    */
    bool getStringValue(unsigned int Line, unsigned int Column, std::string *Value) const;

    /**
      Gets the value at a specified row-column, as a long int
      @param[in] Line the line number of the value (first line is 0)
      @param[in] Column the column number of the value (first column is 0)
      @param[out] Value the requested value if exists
      @return true if the value has been found, false otherwise
    */
    bool getLongValue(unsigned int Line, unsigned int Column, long* Value) const;


    /**
      Gets the value at a specified row-column, as a double precision float
      @param[in] Line the line number of the value (first line is 0)
      @param[in] Column the column number of the value (first column is 0)
      @param[out] Value the requested value if exists
      @return true if the value has been found, false otherwise
    */
    bool getDoubleValue(unsigned int Line, unsigned int Column, double* Value) const;

    /**
      Returns the values at a specified line
      @param[in] Line the line number of the value (first line is 0)
      @return the requested values
    */
    std::vector<std::string> getValues(unsigned int Line) const;

    /**
      Returns the number of lines
      @return the number of lines
    */
    inline unsigned int getLinesCount() const { return m_LinesCount;};

    /**
      Returns the number of columns
      @return the number of columns
    */
    inline unsigned int getColsCount() const { return m_ColsCount;};

    void streamContents(std::ostream& OStream) const;

};


} }


#endif /* __OPENFLUID_TOOLS_COLUMNTEXTPARSER_HPP__ */

