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
 @file WareSrcSyntaxHighlighter.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <openfluid/ui/waresdev/WareSrcSyntaxHighlighter.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WareSrcSyntaxHighlighter::WareSrcSyntaxHighlighter(QTextDocument* Document,
                                                   WareSrcFiletypeManager::HighlightingRules_t HighlightingRules) :
    QSyntaxHighlighter(Document), m_HighlightingRules(HighlightingRules)
{

}


// =====================================================================
// =====================================================================


WareSrcSyntaxHighlighter::~WareSrcSyntaxHighlighter()
{

}


// =====================================================================
// =====================================================================


void WareSrcSyntaxHighlighter::highlightBlock(const QString& Text)
{
  for (const WareSrcFiletypeManager::HighlightingRule& Rule : m_HighlightingRules)
  {
    auto Expression(Rule.Pattern);

    if(Rule.EndPattern.pattern().length() == 0)
    {
#if (QT_VERSION_MAJOR < 6)
      int Index = Expression.indexIn(Text);
      while (Index >= 0)
      {
        int Length = Expression.matchedLength();
        setFormat(Index, Length, Rule.Format);
        Index = Expression.indexIn(Text, Index + Length);
      }
#else
      QRegularExpressionMatchIterator It = Expression.globalMatch(Text);
      while (It.hasNext())
      {
        QRegularExpressionMatch Match = It.next();
        setFormat(Match.capturedStart(), Match.capturedLength(), Rule.Format);
      }
#endif      
    }
    else
    {
      auto EndExpression(Rule.EndPattern);

      setCurrentBlockState(0);
      int StartIndex = 0;
      if (previousBlockState() != 1)
      {
#if (QT_VERSION_MAJOR < 6)
        StartIndex = Expression.indexIn(Text);
#else
        StartIndex = Text.indexOf(Expression);
#endif
      }

      while (StartIndex >= 0)
      {
#if (QT_VERSION_MAJOR < 6)
        int EndIndex = EndExpression.indexIn(Text, StartIndex);
#else
        QRegularExpressionMatch EndMatch;
        int EndIndex = Text.indexOf(EndExpression, StartIndex, &EndMatch);
#endif
        int MatchedLength;
        if (EndIndex == -1)
        {
          setCurrentBlockState(1);
          MatchedLength = Text.length() - StartIndex;
        }
        else
        {
          MatchedLength = EndIndex - StartIndex
#if (QT_VERSION_MAJOR < 6)
              + EndExpression.matchedLength();
#else
              + EndMatch.capturedLength();
#endif
        }
        setFormat(StartIndex, MatchedLength, Rule.Format);
#if (QT_VERSION_MAJOR < 6)
        StartIndex = Expression.indexIn(Text, StartIndex + MatchedLength);
#else
        StartIndex = Text.indexOf(Expression, StartIndex + MatchedLength);
#endif
      }
    }

  }

}


} } }  // namespaces
