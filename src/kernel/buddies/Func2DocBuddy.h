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
    boost::filesystem::path m_CProcessedFilePath;

    boost::filesystem::path m_CPreProcessorPath;

    std::string m_ExtractedLatexDoc;

    std::string m_Title;
    std::string m_FuncName;
    std::string m_FuncAuthor;
    std::string m_FuncDomain;

    std::string m_BeginSignatureTag;
    std::string m_EndSignatureTag;
    std::string m_BeginFunc2DocTag;
    std::string m_EndFunc2DocTag;

    std::string m_FuncDoc;

    SignatureData_t m_InVars;
    SignatureData_t m_OutVars;
    SignatureData_t m_ModVars;
    SignatureData_t m_InData;
    SignatureData_t m_ParamsData;
    SignatureData_t m_Events;
    SignatureData_t m_ExtraFiles;


    void copyDocDirectory();

    void extractLatexDocFromCPP();

    void cpreprocessCPP();

    void processSignature();

    void generateLatex();

    void buildPDF();

    void buildHTML();


  public:

    Func2DocBuddy();

    ~Func2DocBuddy();

    bool run();

};

#endif /* __FUNC2DOCBUDDY_H___ */
