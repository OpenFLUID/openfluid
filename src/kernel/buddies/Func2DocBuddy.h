/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file Func2DocBuddy.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __FUNC2DOCBUDDY_H___
#define __FUNC2DOCBUDDY_H___

#include <boost/filesystem/path.hpp>

#include "OFBuddy.h"

class Func2DocBuddy : public OpenFLUIDBuddy
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

    Func2DocBuddy();

    ~Func2DocBuddy();

    bool run();

};

#endif /* __FUNC2DOCBUDDY_H___ */
