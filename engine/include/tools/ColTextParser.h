/**
  \file ColTextParser.h
  \brief header of column file parser

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __COLFILEPARSER_H__
#define __COLFILEPARSER_H__


#include <vector>
#include <wx/textfile.h>


namespace openfluid { namespace tools {

//WX_DEFINE_ARRAY(wxArrayString*, ArrayContents);

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

    bool loadFromFile(std::string Filename);

    bool setFromString(std::string Contents, unsigned int ColumnsNbr);

    /**
      Returns the value at a specified row-column, as a string
    */
    std::string getValue(unsigned int Line, unsigned int Column);

    bool getStringValue(unsigned int Line, unsigned int Column, std::string *Value);

    /**
      Returns the value at a specified row-column, as an int
    */
    bool getLongValue(unsigned int Line, unsigned int Column, long* Value);


    /**
      Returns the value at a specified row-column, as a double
    */
    bool getDoubleValue(unsigned int Line, unsigned int Column, double* Value);

    /**
      Returns the values of a line as astring list
    */
    std::vector<std::string> getValues(unsigned int Line);

    int getLinesCount() const { return m_LinesCount;};

    int getColsCount() const { return m_ColsCount;};

    void coutContents();

};

} }


#endif


