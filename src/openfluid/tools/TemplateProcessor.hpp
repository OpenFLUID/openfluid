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
  @file TemplateProcessor.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_TOOLS_TEMPLATEPROCESSOR_HPP__
#define __OPENFLUID_TOOLS_TEMPLATEPROCESSOR_HPP__


#include <string>
#include <map>
#include <set>
#include <regex>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {


/**
  Class for rendering templates from string, files or directories.

_Example of use_
```cpp
  const openfluid::tools::TemplateProcessor::Data Data = {
    {"key1","value #1"},
    {"key2","value #2"}
  };
  openfluid::tools::TemplateProcessor::Errors Errors;

  // construction of the processor using the ${datakey} pattern
  // where datakey can be keys in the Data structure
  openfluid::tools::TemplateProcessor TplProc("${","}");

  // file rendering
  TplProc.renderFile("/path/to/template/file.tpl","/path/to/rendered/file.txt",Data,Errors);

  // recursive directory rendering
  TplProc.renderDirectory("/path/to/template/directory","/path/to/rendered/directory",Data,Errors);
```
*/
class OPENFLUID_API TemplateProcessor
{
  private:

    std::regex m_RegEx;


  public:

    /**
      Type for the input data used to resolve the template, as a key-value structure
    */
    using Data = std::map<std::string,std::string>;

    /**
      Type for the resolution errors returned at render time, containing unresolved patterns found in the template
    */
    using Errors = std::set<std::string>;
    

    TemplateProcessor() = delete;

    /**
      Constructs a template processor, giving the beginning end ending delimiters used ath rendering time
      @param[in] Begin The beginning delimiter
      @param[in] End The ending delimiter
      @throw openfluid::base::FrameworkException if the beginning or ending delimiter is empty
    */
    TemplateProcessor(const std::string& Begin, const std::string& End);

    virtual ~TemplateProcessor() = default;

    /**
      Renders a template string into a string
      @param[in] Str The template content
      @param[in] D The data to resolve the template
      @param[inout] E The unresolved patterns if any
      @return The resolved template as a string
    */
    std::string renderString(const std::string& Str,const Data& D,Errors& E);

    /**
      Renders a template file into a string
      @param[in] InFile The template file path
      @param[in] D The data to resolve the template
      @param[inout] E The unresolved patterns if any
      @return The resolved template as a string
      @throw openfluid::base::FrameworkException if the template file cannot be open
    */
    std::string renderFile(const std::string& InFile,const Data& D,Errors& E);

    /**
      Renders a template file into another file
      @param[in] InFile The template file path
      @param[in] OutFile The rendered file path
      @param[in] D The data to resolve the template
      @param[inout] E The unresolved patterns if any
      @throw openfluid::base::FrameworkException if the output file is an existing directory
    */
    void renderFile(const std::string& InFile, const std::string& OutFile,const Data& D,Errors& E);

    /**
      Renders a template directory into another directory recursively.
      @param[in] InDir The template directory path
      @param[in] OutDir The rendered directory path
      @param[in] D The data to resolve the template
      @param[inout] E The unresolved patterns if any
      @throw openfluid::base::FrameworkException if the output directory is an existing file
    */
    void renderDirectory(const std::string& InDir,const std::string& OutDir,const Data& D,Errors& E);

};


} }  // namespaces


#endif /* __OPENFLUID_TOOLS_TEMPLATEPROCESSOR_HPP__ */
