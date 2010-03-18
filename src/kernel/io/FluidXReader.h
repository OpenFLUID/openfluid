/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file FluidXReader.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __FLUIDXREADER_H___
#define __FLUIDXREADER_H___

#include <libxml/parser.h>
#include <string>

#include "output/OutputDescriptor.h"
#include "run/RunDescriptor.h"
#include "domain/DomainDescriptor.h"
#include "model/ModelDescriptor.h"
#include "model/FunctionDescriptor.h"
#include "model/GeneratorDescriptor.h"
#include "output/OutputDescriptor.h"
#include "output/OutputFilesDescriptor.h"
#include "output/OutputSetDescriptor.h"

#include "openfluid-core.h"
#include "openfluid-tools.h"




class FluidXReader
{
  private:

    std::string m_CurrentFile;

    bool m_RunConfigDefined;

    bool m_ModelDefined;


    ModelDescriptor m_ModelDescriptor;

    RunDescriptor m_RunDescriptor;

    OutputDescriptor m_OutputDescriptor;

    DomainDescriptor m_DomainDescriptor;

    openfluid::core::FuncParamsMap_t m_ModelGlobalParams;


    OutputFilesDescriptor extractFilesDecriptorFromNode(xmlNodePtr NodePtr);

    OutputSetDescriptor extractSetDecriptorFromNode(xmlNodePtr NodePtr);

    void extractOutputFromNode(xmlNodePtr NodePtr);

    openfluid::core::FuncParamsMap_t extractParamsFromNode(xmlNodePtr NodePtr);

    openfluid::core::FuncParamsMap_t mergeParams(const openfluid::core::FuncParamsMap_t& Params,
                                                 const openfluid::core::FuncParamsMap_t& OverloadParams);

    void propagateGlobalParamsInModel();

    void extractModelFromNode(xmlNodePtr NodePtr);

    void extractRunFromNode(xmlNodePtr NodePtr);

    void extractDomainFomNode(xmlNodePtr NodePtr);

    openfluid::core::UnitClassID_t extractUnitClassIDFromNode(xmlNodePtr NodePtr);

    void extractDomainDefinitionFromNode(xmlNodePtr NodePtr);

    void extractDomainInputdataFromNode(xmlNodePtr NodePtr);

    void extractDomainCalendarFromNode(xmlNodePtr NodePtr);

    void parseFile(std::string Filename);

  public:

    FluidXReader();

    ~FluidXReader();

    void loadFromDirectory(std::string DirPath, std::ostream& OStream);

    ModelDescriptor& getModelDescriptor() {return m_ModelDescriptor; };

    RunDescriptor& getRunDescriptor() {return m_RunDescriptor; };

    OutputDescriptor& getOutputDescriptor() {return m_OutputDescriptor; };

    DomainDescriptor& getDomainDescriptor() {return m_DomainDescriptor; };

};



#endif /* __FLUIDXREADER_H___ */
