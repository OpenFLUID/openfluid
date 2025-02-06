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
  @file TemplateProcessor.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#include <fstream>
#include <filesystem>

#include <openfluid/tools/TemplateProcessor.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/tools/MiscHelpers.hpp>


namespace openfluid { namespace tools {


TemplateProcessor::TemplateProcessor(const std::string& Begin, const std::string& End):
  m_RegEx(openfluid::tools::escapePattern(Begin)+"\\s*([\\w\\#]+)\\s*"+openfluid::tools::escapePattern(End))
{
  if (Begin.empty() || End.empty())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "delimiters cannot be empty");
  }
}


// =====================================================================
// =====================================================================


void TemplateProcessor::enableAutoComment(const std::string& CommentStr)
{
  m_AutoComment = true;
  m_CommentStr = CommentStr;
}


// =====================================================================
// =====================================================================


void TemplateProcessor::ignoreUnknown(bool Enabled)
{
  m_IgnoreUnknown = Enabled;
}


// =====================================================================
// =====================================================================


std::string TemplateProcessor::renderString(const std::string& Str,const Data& D, Errors& E)
{
  std::string RenderedStr;
  std::smatch SM;


  auto Start = Str.begin();

  while (std::regex_search(Start, Str.end(),SM,m_RegEx))
  {
    RenderedStr += SM.prefix();  // add the text before the pattern

    const auto FoundKey = SM[1].str();

    if (m_AutoComment && openfluid::tools::startsWith(FoundKey,"#"))
    {
      auto CoKey = FoundKey.substr(1); // determine the corresponding key name

      auto it = D.find(CoKey);
      if (it == D.end() || it->second.empty())
      {
        // replace the comment key by the comment str if the corresponding key does not exist or is empty
        RenderedStr += m_CommentStr;
      }
    }
    else if (D.find(FoundKey) != D.end())
    {
      // data exists : replace the pattern by the corresponding data
      RenderedStr += D.at(FoundKey);
    }
    else
    {
      if (!m_IgnoreUnknown)
      {
        // data does not exists : leave the delimited key in the text and add an error
        RenderedStr += SM[0].str();  
        E.insert(FoundKey);
      }
    }

    Start = SM[0].second;
  }

  if (Start != Str.end())
  {
    // add the remaining text
    RenderedStr += std::string(Start,Str.end());
  }

  return RenderedStr;
}


// =====================================================================
// =====================================================================


std::string TemplateProcessor::renderFile(const std::string& InFile,const Data& D,
                                          Errors& E)
{
  std::ifstream IFS(InFile);

  if (IFS.is_open())
  {
    std::string FileContent((std::istreambuf_iterator<char>(IFS)),(std::istreambuf_iterator<char>()));
    return renderString(FileContent,D,E);
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "file " + InFile + " cannot be open");
  }

  return std::string();
}


// =====================================================================
// =====================================================================


void TemplateProcessor::renderFile(const std::string& InFile, const std::string& OutFile,
                                   const Data& D,Errors& E)
{
  if (openfluid::tools::FilesystemPath(OutFile).isDirectory())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "path " + OutFile + " is an existing directory");
  }

  const auto RenderedStr = renderFile(InFile,D,E);

  openfluid::tools::FilesystemPath(openfluid::tools::FilesystemPath(OutFile).dirname()).makeDirectory();

  std::ofstream OFS(OutFile,std::ofstream::out);

  if (OFS.is_open())
  {
    OFS << RenderedStr;
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Error while writing to file " + OutFile);
  }

}


// =====================================================================
// =====================================================================


void TemplateProcessor::renderDirectory(const std::string& InDir,const std::string& OutDir,
                                        const Data& D,Errors& E)
{
  if (openfluid::tools::FilesystemPath(OutDir).isFile())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "path " + InDir + " is an existing file");
  }

  for (const auto& dir_entry : std::filesystem::recursive_directory_iterator(InDir))
  {
    const auto InPath = openfluid::tools::FilesystemPath::fromStdPath(dir_entry.path());
    const auto RelPath = InPath.relativeTo(InDir);
    const auto OutPath = openfluid::tools::FilesystemPath({OutDir,RelPath});

    if (InPath.isDirectory())
    {
      if(!OutPath.makeDirectory())
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                                                  "Error while creating " + OutPath.toGeneric() + " directory");
      }
    }
    else
    {
      renderFile(InPath.toGeneric(),OutPath.toGeneric(),D,E);
    }
  }
}


} }
