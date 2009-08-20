/**
  \file ColFileParser.cpp
  \brief implements column file parser

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "ColTextParser.h"
#include "openfluid-tools.h"


#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>



// =====================================================================
// =====================================================================

namespace openfluid { namespace tools {


ColumnTextParser::ColumnTextParser(std::string CommentLineSymbol, std::string Delimiter)
{

  m_Delimiter = Delimiter;
  m_CommentSymbol = CommentLineSymbol;

  m_LinesCount = 0;
  m_ColsCount = 0;

}



// =====================================================================
// =====================================================================



ColumnTextParser::~ColumnTextParser()
{

}


// =====================================================================
// =====================================================================



std::vector<std::string> ColumnTextParser::tokenizeLine(std::string Line)
{
  std::vector<std::string> NewLine;

  TokenizeString(Line,NewLine,m_Delimiter);

  return NewLine;
}


// =====================================================================
// =====================================================================



bool ColumnTextParser::checkContents()
{
  unsigned int LineColCount;
  unsigned int LineCount = m_Contents.size();

  if (LineCount == 0) return true;

  // checks that all lines have the same size
  // i.e. same columns number
  LineColCount = m_Contents.at(0).size();

  for (unsigned int i=1;i<LineCount;i++)
  {
    if (m_Contents.at(i).size() != LineColCount)
      return false;
  }

  m_LinesCount = m_Contents.size();
  m_ColsCount = LineColCount;

  return true;
}


// =====================================================================
// =====================================================================


bool ColumnTextParser::isCommentLineStr(std::string LineStr)
{

  if (m_CommentSymbol.length() > 0)
  {
    boost::trim_left(LineStr);
    return boost::starts_with(LineStr,m_CommentSymbol.c_str());
  }

  return false;

}



// =====================================================================
// =====================================================================


bool ColumnTextParser::isEmptyLineStr(std::string LineStr)
{

  boost::trim(LineStr);

  return (LineStr.length() == 0);
}



// =====================================================================
// =====================================================================



bool ColumnTextParser::loadFromFile(std::string Filename)
{

  std::string StrLine;
  boost::filesystem::path ColumnFilename;

  if (m_Contents.size()> 0 ) m_Contents.clear();

  m_LinesCount = 0;
  m_ColsCount = 0;


  ColumnFilename = boost::filesystem::path(Filename);


  std::ifstream FileContent(ColumnFilename.string().c_str());


  // check if file exists and if it is "openable"
  if (!boost::filesystem::exists(ColumnFilename)) return false;
  if (!FileContent) return false;

  //  TODO check if this is interesting : if (LinesCount == 0) return false;

  // parse and loads file contents
  while(std::getline(FileContent,StrLine))
  {
    if (!isCommentLineStr(StrLine) && !isEmptyLineStr(StrLine)) m_Contents.push_back(tokenizeLine(StrLine));
  }

  //  TODO check if this is required : process last line after the loop;
  //  if (!isCommentLineStr(_S(StrLine)) && !isEmptyLineStr(_S(StrLine))) m_Contents.push_back(tokenizeLine(_S(StrLine)));


  return checkContents();

}


// =====================================================================
// =====================================================================

bool ColumnTextParser::setFromString(std::string Contents, unsigned int ColumnsNbr)
{
  /** \internal

    The string is tokenized.
    The number of tokens must be modulo number of columns

  */

  std::vector<std::string> Tokens;
  std::vector<std::string>::iterator it;

  bool IsOK = true;

  TokenizeString(Contents,Tokens,m_Delimiter);

  if (Tokens.size() % ColumnsNbr == 0)
  {

    if (m_Contents.size() > 0) m_Contents.clear();

    std::vector<std::string> LineStr;

    it = Tokens.begin();
    while (it != Tokens.end() && IsOK)
    {
      // add to the current line
      LineStr.push_back(*it);

      if (LineStr.size() == ColumnsNbr)
      {

        // if current line has ColumnsNbr columns, it is added to the contents
        m_Contents.push_back(LineStr);

        LineStr.clear();
      }

      it++;
    }

    // more tokens processed but not a complete line. not good!
    if (LineStr.size() != 0 && LineStr.size() != ColumnsNbr)
    {
      IsOK = false;
    }
  }
  else
  {
    IsOK = false;
  }

  return IsOK && checkContents();
}

// =====================================================================
// =====================================================================



std::vector<std::string> ColumnTextParser::getValues(unsigned int Line)
{
  if (Line < m_Contents.size())
  {
    return m_Contents.at(Line);
  }
  else
  {
    return std::vector<std::string>();
  }

}



// =====================================================================
// =====================================================================


std::string ColumnTextParser::getValue(unsigned int Line, unsigned int Column)
{
  std::vector<std::string> LineString = getValues(Line);

  if (LineString.size()!=0 && Column < LineString.size())
  {
    return LineString.at(Column);
  }
  else
  {
    return "";
  }

}



// =====================================================================
// =====================================================================


bool ColumnTextParser::getStringValue(unsigned int Line, unsigned int Column, std::string *Value)
{
  std::string StrValue = getValue(Line,Column);

  if (StrValue.length() == 0) return false;

  *Value = StrValue;

  return true;

}


// =====================================================================
// =====================================================================


bool ColumnTextParser::getLongValue(unsigned int Line, unsigned int Column, long* Value)
{
  std::string StrValue = getValue(Line,Column);

  if (StrValue.length() != 0)
  {
    return openfluid::tools::ConvertString(StrValue,Value);
  }

  return false;
}



// =====================================================================
// =====================================================================


bool ColumnTextParser::getDoubleValue(unsigned int Line, unsigned int Column, double* Value)
{

  std::string StrValue = getValue(Line,Column);

  if (StrValue.length() != 0)
  {
    return openfluid::tools::ConvertString(StrValue,Value);
  }

  return false;
}



// =====================================================================
// =====================================================================

void ColumnTextParser::coutContents()
{
  std::cout << "" << std::endl;
  int i,j;

  for (i=0;i<m_LinesCount;i++)
  {
    for (j=0;j<m_ColsCount;j++)
    {
      std::cout << getValue(i,j) << "\t";

    }
    std::cout.flush();

  }
  std::cout << "" << std::endl;
}


} }

