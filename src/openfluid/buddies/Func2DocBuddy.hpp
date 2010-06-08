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
  \file Func2DocBuddy.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __FUNC2DOCBUDDY_H___
#define __FUNC2DOCBUDDY_H___

#include <boost/filesystem/path.hpp>

#include <openfluid/dllexport.hpp>
#include <openfluid/buddies/OFBuddy.hpp>


namespace openfluid { namespace buddies {


class DLLEXPORT Func2DocBuddy : public OpenFLUIDBuddy
{
  private:
    typedef std::map< std::string,std::vector<std::string> > SignatureData_t;

    std::string m_CPPFile;
    std::string m_LatexOutFile;

    boost::filesystem::path m_InputDirPath;
    boost::filesystem::path m_OutputDirPath;
    boost::filesystem::path m_InputFilePath;
    boost::filesystem::path m_OutputLatexFilePath;
    boost::filesystem::path m_TplFilePath;
    boost::filesystem::path m_CProcessedFilePath;
    boost::filesystem::path m_PDFLatexPath;
    boost::filesystem::path m_BibtexPath;
    boost::filesystem::path m_Latex2HTMLPath;

    boost::filesystem::path m_CPreProcessorPath;

    std::string m_ExtractedLatexDoc;

    std::string m_Title;
    std::string m_NewCommands;
    std::string m_FuncID;
    std::string m_FuncName;
    std::string m_FuncVersion;
    std::string m_FuncAuthorName;
    std::string m_FuncAuthorEmail;
    std::string m_FuncDomain;
    std::string m_FuncDescription;
    std::string m_FuncData;

    std::string m_BeginSignatureTag;
    std::string m_EndSignatureTag;
    std::string m_BeginFunc2DocTag;
    std::string m_EndFunc2DocTag;

    std::string m_HTMLPackageLatexCommand;

    SignatureData_t m_InVars;
    SignatureData_t m_OutVars;
    SignatureData_t m_ModVars;
    SignatureData_t m_InData;
    SignatureData_t m_ParamsData;
    SignatureData_t m_Events;
    SignatureData_t m_ExtraFiles;

    std::string extractBetweenTags(std::string Content, const std::string BeginTag, const std::string EndTag);

    std::string toLatexFriendly(std::string Content);

    void addLatexDataCatBegin(std::string& Content, const std::string Title, const std::string ColsFormat);

    void addLatexDataCatEnd(std::string& Content);

    std::vector<std::string> extractSignatureLines();

    std::vector<std::string> searchStringLitterals(std::string StrToParse);

    void copyDocDirectory();

    void extractLatexDocFromCPP();

    void cpreprocessCPP();

    void processSignature();

    void generateLatex();

    bool isErrorInPDFLatexLog();

    void buildPDF();

    void buildHTML();


  public:

    Func2DocBuddy(openfluid::buddies::BuddiesListener* Listener);

    ~Func2DocBuddy();

    bool run();

};


} } //namespaces

#endif /* __FUNC2DOCBUDDY_HPP__ */
