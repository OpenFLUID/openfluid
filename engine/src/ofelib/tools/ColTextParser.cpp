/**
  \file ColFileParser.cpp
  \brief implements column file parser

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "ColTextParser.h"
#include "openfluid-tools.h"

#include <wx/tokenzr.h>

#include <iostream>


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

  wxStringTokenizer Tokenizer(_U(Line.c_str()), _U(m_Delimiter.c_str()));

  while (Tokenizer.HasMoreTokens())
  {
    wxString Token = Tokenizer.GetNextToken();
    NewLine.push_back(_S(Token));
  }


  return NewLine;
}


// =====================================================================
// =====================================================================



bool ColumnTextParser::checkContents()
{
  int LineColCount;
  int LineCount = m_Contents.size();

  if (LineCount == 0) return true;

  // checks that all lines have the same size
  // i.e. same columns number
  LineColCount = m_Contents.at(0).size();

  for (int i=1;i<LineCount;i++)
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
  wxString LineStrWX = _U(LineStr.c_str());

  if (m_CommentSymbol.length() > 0)
  {
    LineStrWX = LineStrWX.Trim(false);
    if (LineStrWX.StartsWith(_U(m_CommentSymbol.c_str())))
      return true;
  }

  return false;

}



// =====================================================================
// =====================================================================


bool ColumnTextParser::isEmptyLineStr(std::string LineStr)
{

  wxString LineStrWX = _U(LineStr.c_str());

  LineStrWX = LineStrWX.Trim(true).Trim(false);

  if (LineStrWX.Length() == 0) return true;
  else return false;

}



// =====================================================================
// =====================================================================



bool ColumnTextParser::loadFromFile(std::string Filename)
{

  int LinesCount;
  wxString StrLine;
  wxTextFile *ColumnFile;

  if (m_Contents.size()> 0 ) m_Contents.clear();

  m_LinesCount = 0;
  m_ColsCount = 0;


  ColumnFile = new wxTextFile(_U(Filename.c_str()));

  // check if file exists and opens it
  if (!ColumnFile->Exists()) return false;
  if (!ColumnFile->Open()) return false;

  LinesCount = ColumnFile->GetLineCount();

  if (LinesCount == 0) return false;

  // identifies file type, i.e. DOS/MAC/UNIX/...
  ColumnFile->GuessType();

  // parse and loads file contents


  // processing of the file except the last line
  for (StrLine=ColumnFile->GetFirstLine();!ColumnFile->Eof();StrLine = ColumnFile->GetNextLine())
  {
    if (!isCommentLineStr(_S(StrLine)) && !isEmptyLineStr(_S(StrLine))) m_Contents.push_back(tokenizeLine(_S(StrLine)));
  }

  // processing of the last line
  if (!isCommentLineStr(_S(StrLine)) && !isEmptyLineStr(_S(StrLine))) m_Contents.push_back(tokenizeLine(_S(StrLine)));


  ColumnFile->Close();

  delete ColumnFile;


  return checkContents();

}


// =====================================================================
// =====================================================================

bool ColumnTextParser::setFromString(std::string Contents, int ColumnsNbr)
{
  /** \internal

    The string is tokenized.
    The number of tokens must be modulo number of columns

  */


  bool IsOK = true;

  wxStringTokenizer Tkz(_U(Contents.c_str()), _U(m_Delimiter.c_str()));

  if ((Tkz.CountTokens() % ColumnsNbr) == 0)
  {

    if (m_Contents.size() > 0) m_Contents.clear();

    std::vector<std::string> LineStr;

    while (Tkz.HasMoreTokens() && IsOK)
    {
      // add to the current line
      LineStr.push_back(_S(Tkz.GetNextToken()));

      if (LineStr.size() == ColumnsNbr)
      {

        // if current line has ColumnsNbr columns, it is added to the contents
        m_Contents.push_back(LineStr);

        if (Tkz.CountTokens() > 0) LineStr.clear();
      }
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



std::vector<std::string> ColumnTextParser::getValues(int Line)
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


std::string ColumnTextParser::getValue(int Line, int Column)
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


bool ColumnTextParser::getStringValue(int Line, int Column, std::string *Value)
{
  std::string StrValue = getValue(Line,Column);

  if (StrValue.length() == 0) return false;

  *Value = StrValue;

  return true;

}


// =====================================================================
// =====================================================================


bool ColumnTextParser::getLongValue(int Line, int Column, long* Value)
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


bool ColumnTextParser::getDoubleValue(int Line, int Column, double* Value)
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

