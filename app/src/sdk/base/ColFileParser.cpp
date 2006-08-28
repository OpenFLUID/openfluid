/**
  \file ColFileParser.cpp
  \brief implements column file parser

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "ColFileParser.h"
#include <wx/tokenzr.h>

#include <iostream>


// =====================================================================
// =====================================================================


namespace mhydasdk { namespace base {



ColumnFileParser::ColumnFileParser(wxString FileName, wxString CommentLineSymbol, wxString Delimiter)
{

  m_FileName = FileName;
  m_Delimiter = Delimiter;
  m_CommentSymbol = CommentLineSymbol;

  m_FileContents = NULL;

  m_LinesCount = 0;
  m_ColsCount = 0;

}



// =====================================================================
// =====================================================================



ColumnFileParser::~ColumnFileParser()
{

}


// =====================================================================
// =====================================================================



wxArrayString* ColumnFileParser::tokenizeLine(wxString Line)
{
  wxArrayString *NewLine = new wxArrayString();

  wxStringTokenizer Tokenizer(Line, m_Delimiter);

  while (Tokenizer.HasMoreTokens())
  {
    wxString Token = Tokenizer.GetNextToken();
    NewLine->Add(Token);
  }


  return NewLine;
}


// =====================================================================
// =====================================================================



bool ColumnFileParser::checkContents()
{
  int LineColCount;
  int LineCount = m_FileContents->GetCount();

  if (LineCount == 0) return true;

  // checks that all lines have the same size
  // i.e. same columns number
  LineColCount = ((wxArrayString*)(m_FileContents->Item(0)))->GetCount();

  for (int i=1;i<LineCount;i++)
  {
    if (((wxArrayString*)(m_FileContents->Item(i)))->GetCount() != LineColCount)
      return false;
  }

  m_LinesCount = m_FileContents->GetCount();
  m_ColsCount = LineColCount;

  return true;
}


// =====================================================================
// =====================================================================


bool ColumnFileParser::isCommentLineStr(wxString LineStr)
{

  if (m_CommentSymbol.Length() > 0)
  {
    LineStr = LineStr.Trim(false);
    if (LineStr.StartsWith(m_CommentSymbol.c_str()))
      return true;
  }

  return false;

}



// =====================================================================
// =====================================================================


bool ColumnFileParser::isEmptyLineStr(wxString LineStr)
{

  LineStr = LineStr.Trim(true).Trim(false);

  if (LineStr.Length() == 0) return true;
  else return false;

}



// =====================================================================
// =====================================================================



bool ColumnFileParser::parseFile()
{

  bool IsOK = true;
  int LinesCount;
  wxString StrLine;
  wxTextFile *ColumnFile;

  if (m_FileContents != NULL) delete m_FileContents;
  m_FileContents = new ArrayFileContents();

  m_LinesCount = 0;
  m_ColsCount = 0;


  ColumnFile = new wxTextFile(m_FileName);

  // check if file exists and opens it
  if (!ColumnFile->Exists()) return false;
  if (!ColumnFile->Open()) return false;

  LinesCount = ColumnFile->GetLineCount();

  if (LinesCount == 0) return false;

  // identifies file type, i.e. DOS/MAC/UNIX/...
  ColumnFile->GuessType();

  // parse and loads file contents
  StrLine = ColumnFile->GetFirstLine();

  while (!ColumnFile->Eof())
  {
    // adds the line if it is not a comment line nor empty
    if (!isCommentLineStr(StrLine) && !isEmptyLineStr(StrLine)) m_FileContents->Add(tokenizeLine(StrLine));

    StrLine = ColumnFile->GetNextLine();
  }

  ColumnFile->Close();

  delete ColumnFile;


  return checkContents();

}



// =====================================================================
// =====================================================================



wxArrayString* ColumnFileParser::getValues(int Line)
{
  if (Line < m_FileContents->GetCount())
  {
    return ((wxArrayString*)(m_FileContents->Item(Line)));
  }
  else
  {
    return NULL;
  }

}



// =====================================================================
// =====================================================================


wxString ColumnFileParser::getValue(int Line, int Column)
{
  wxArrayString* LineString = getValues(Line);

  if (LineString != NULL && Column < LineString->GetCount())
  {
    return LineString->Item(Column);
  }
  else
  {
    return wxT("");
  }

}



// =====================================================================
// =====================================================================


bool ColumnFileParser::getStringValue(int Line, int Column, wxString *Value)
{
  wxString StrValue = getValue(Line,Column);

  if (StrValue.Length() == 0) return false;

  *Value = StrValue;

  return true;

}


// =====================================================================
// =====================================================================


bool ColumnFileParser::getLongValue(int Line, int Column, long* Value)
{
  wxString StrValue = getValue(Line,Column);

  if (StrValue.Length() != 0)
  {
    return StrValue.ToLong(Value);
  }

  return false;
}



// =====================================================================
// =====================================================================


bool ColumnFileParser::getDoubleValue(int Line, int Column, double* Value)
{

  wxString StrValue = getValue(Line,Column);

  if (StrValue.Length() != 0)
  {
    return StrValue.ToDouble(Value);
  }

  return false;
}



} } // namespace mhydasdk::base

