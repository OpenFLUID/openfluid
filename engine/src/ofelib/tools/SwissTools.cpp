

#include "SwissTools.h"


void TokenizeString(const std::string& StrToTokenize,
                    std::vector<std::string>& Tokens,
                    const std::string& Delimiters = " ")
{

  std::string::size_type LastPos = StrToTokenize.find_first_not_of(Delimiters, 0);
  std::string::size_type Pos = StrToTokenize.find_first_of(Delimiters, LastPos);

  while (std::string::npos != Pos || std::string::npos != LastPos)
  {
    // Found a token, add it to the vector.
    Tokens.push_back(StrToTokenize.substr(LastPos, Pos - LastPos));
    // Skip delimiters.  Note the "not_of"
    LastPos = StrToTokenize.find_first_not_of(Delimiters, Pos);
    // Find next "non-delimiter"
    Pos = StrToTokenize.find_first_of(Delimiters, LastPos);
  }
}
