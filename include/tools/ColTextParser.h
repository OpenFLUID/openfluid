/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ColTextParser.h
  \brief header of column file parser

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __COLFILEPARSER_H__
#define __COLFILEPARSER_H__


#include <vector>
#include <string>

namespace openfluid { namespace tools {


/**
  Class for column file management and handling
*/
class ColumnTextParser
{

  protected:

  private:

    std::string m_Delimiter;
    std::string m_CommentSymbol;

    std::vector<std::vector<std::string> > m_Contents;

    std::vector<std::string> tokenizeLine(std::string Line);

    bool checkContents();

    bool isCommentLineStr(std::string LineStr);

    bool isEmptyLineStr(std::string LineStr);

    int m_LinesCount;

    int m_ColsCount;


  public:

    /**
      Constructor
    */
    ColumnTextParser(std::string CommentLineSymbol = "", std::string Delimiter = " \t\r\n");

    /**
      Destructor
    */
    ~ColumnTextParser();


    /**
      Loads a column text file
      \param[in] Filename the full path of the file to load
      \return true if everything went fine
    */
    bool loadFromFile(std::string Filename);

    /**
      Parses a one-line delimiter-separated string to set contents
      ex: "147.2 18 15 25 36 51.3 25.1 15", for 4 columns
      \param[in] Contents the string to parse
      \param[in] ColumnsNbr the number of columns (for splitting the sting into lines)
      \return true if everything went fine
    */
    bool setFromString(std::string Contents, unsigned int ColumnsNbr);

    /**
      Returns the value at a specified row-column, as a string
      \param[in] Line the line number of the value (first line is 0)
      \param[in] Column the column number of the value (first column is 0)
      \return the requested value
    */
    std::string getValue(unsigned int Line, unsigned int Column);

    /**
      Gets the value at a specified row-column, as a string
      \param[in] Line the line number of the value (first line is 0)
      \param[in] Column the column number of the value (first column is 0)
      \param[out] Value the requested value if exists
      \return true if the value has been found, false otherwise
    */
    bool getStringValue(unsigned int Line, unsigned int Column, std::string *Value);

    /**
      Gets the value at a specified row-column, as a long int
      \param[in] Line the line number of the value (first line is 0)
      \param[in] Column the column number of the value (first column is 0)
      \param[out] Value the requested value if exists
      \return true if the value has been found, false otherwise
    */
    bool getLongValue(unsigned int Line, unsigned int Column, long* Value);


    /**
      Gets the value at a specified row-column, as a double precision float
      \param[in] Line the line number of the value (first line is 0)
      \param[in] Column the column number of the value (first column is 0)
      \param[out] Value the requested value if exists
      \return true if the value has been found, false otherwise
    */
    bool getDoubleValue(unsigned int Line, unsigned int Column, double* Value);

    /**
      Returns the values at a specified line
      \param[in] Line the line number of the value (first line is 0)
      \return the requested values
    */
    std::vector<std::string> getValues(unsigned int Line);

    /**
      Returns the number of lines
      \return the number of lines
    */
    int getLinesCount() const { return m_LinesCount;};

    /**
      Returns the number of columns
      \return the number of columns
    */
    int getColsCount() const { return m_ColsCount;};

    void coutContents();

};

} }


#endif


