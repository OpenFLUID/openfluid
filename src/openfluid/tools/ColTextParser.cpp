/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  @file
  @brief implements column file parser

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include <openfluid/tools/ColTextParser.hpp>

#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <openfluid/tools/SwissTools.hpp>


// =====================================================================
// =====================================================================

namespace openfluid { namespace tools {


ColumnTextParser::ColumnTextParser(const std::string& CommentLineSymbol, const std::string& Delimiter):
  m_Delimiter(Delimiter), m_CommentSymbol(CommentLineSymbol),
  m_LinesCount(0), m_ColsCount(0)
{

}



// =====================================================================
// =====================================================================



ColumnTextParser::~ColumnTextParser()
{

}


// =====================================================================
// =====================================================================



std::vector<std::string> ColumnTextParser::tokenizeLine(const std::string& Line)
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


bool ColumnTextParser::isCommentLineStr(const std::string& LineStr)
{

  if (m_CommentSymbol.length() > 0)
  {
    std::string TmpStr = boost::trim_left_copy(LineStr);
    return boost::starts_with(TmpStr,m_CommentSymbol.c_str());
  }

  return false;

}



// =====================================================================
// =====================================================================


bool ColumnTextParser::isEmptyLineStr(const std::string& LineStr)
{
  return (boost::trim_copy(LineStr).empty());
}



// =====================================================================
// =====================================================================



bool ColumnTextParser::loadFromFile(const std::string& Filename)
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

  // parse and loads file contents
  while(std::getline(FileContent,StrLine))
  {
    if (!isCommentLineStr(StrLine) && !isEmptyLineStr(StrLine)) m_Contents.push_back(tokenizeLine(StrLine));
  }

  return checkContents();
}


// =====================================================================
// =====================================================================

bool ColumnTextParser::setFromString(const std::string& Contents, unsigned int ColumnsNbr)
{
  /** @internal

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

  if (!LineString.empty() && Column < LineString.size())
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

void ColumnTextParser::streamContents(std::ostream& OStream)
{
  OStream << "" << std::endl;
  unsigned int i,j;

  for (i=0;i<m_LinesCount;i++)
  {
    for (j=0;j<m_ColsCount;j++)
    {
      OStream << getValue(i,j) << "\t";

    }
    OStream.flush();

  }
  OStream << "" << std::endl;
}


} }

