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
 @file WareSrcDocalyzer.hpp
 
 @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/


#ifndef __OPENFLUID_WARESDEV_WARESRCDOCALYZER_HPP__
#define __OPENFLUID_WARESDEV_WARESRCDOCALYZER_HPP__


#include <string>
#include <vector>

#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/waresdev/WareSrcDocalyzerListener.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace waresdev {


/**
  Class to check wares source code
*/
class OPENFLUID_API WareSrcDocalyzer
{
  private:

    const openfluid::tools::FilesystemPath m_SrcPathObj;

    const openfluid::tools::FilesystemPath m_OutPathObj;

    const openfluid::tools::FilesystemPath m_DocalyzerSharePathObj;

    WareSrcDocalyzerListener* mp_Listener;

    std::vector<std::string> m_InputFormats;

    openfluid::tools::FilesystemPath m_WorkPathObj;

    struct DocData
    {

      openfluid::ware::WareID_t ID;

      openfluid::ware::WareType Type;

      std::string Signature;

      std::string Content;

      void clear()
      {
        ID.clear();
        Type = openfluid::ware::WareType::UNDEFINED;
        Signature.clear();
        Content.clear();
      }

      std::string getTypeAsText() const;

    } m_DocData;

    void clear();

    const std::vector<std::string> getMainFileRelativePath(const std::string& Format) const;

    static std::string getGeneratedFileName(const std::string& FilePart, const std::string& FileExt = "");

    const openfluid::tools::FilesystemPath getGeneratedFilePath(const std::string& FilePart, 
                                                                const std::string& FileExt = "tex") const;

    const std::string detectFormat() const;

    void processSignature(bool IgnoreEmptyFields);

    void processLatexContent();

    void processRmarkdownContent();

    void processMarkdownContent();

    void processReadmeContent();

    void processContent(const std::string& Format);

    void integrateDocument(const std::string& Format) const;

    void buildDocument() const;
 
    void releaseDocument() const;


  public:

    CPPCLASS_DELETE_FIVE(WareSrcDocalyzer);

    /**
      Constructs a docalyzer
      @param[in] SrcPath The path to the sources to docalyze
      @param[in] OutputPath The path where to put the built documentation
      @param Listener The Listener for Docalyzer actions
      @param[in] InputFormats The doc format searched by the docalyzer (searches for all possible formats if empty)
    */
    WareSrcDocalyzer(const std::string& SrcPath, const std::string& OutputPath, WareSrcDocalyzerListener* Listener,
                     std::vector<std::string> InputFormats);

    void performDocalyze(bool KeepWorkData = false, bool IncludeEmptyFields = false);

};


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESRCDOCALYZER_HPP__ */
