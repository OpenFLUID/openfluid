/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OutputsWriter.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OUTPUTSWRITER_H___
#define __OUTPUTSWRITER_H___

#include <string>
#include "openfluid-core.h"
#include "openfluid-base.h"
#include "OutputDescriptor.h"

class OutputsWriter
{
  private:

    std::string m_DirPath;
    OutputDescriptor m_OutDesc;


    static std::string generateOuputFilename(const std::string UnitClass, const openfluid::core::UnitID_t UnitID,
                                   const std::string Suffix, const std::string VectorName = "");

    static std::string generateOutputScalarsFileHeader(const openfluid::core::UnitClass_t UnitClass, const openfluid::core::UnitID_t UnitID,
                                             const std::string Filename, const std::vector<std::string> ScalarsNames,
                                             const std::string CommentChar);

    static std::string generateOutputVectorFileHeader(const openfluid::core::UnitClass_t UnitClass, const openfluid::core::UnitID_t UnitID,
                                            const std::string Filename, const std::string VectorName,
                                            const std::string CommentChar);


    static std::string generateOutputScalarsFileContent(const openfluid::core::Unit* aUnit,
                                              const std::vector<std::string> ScalarsNames,
                                              openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                                              openfluid::base::SimulationInfo *SimInfo,
                                              std::string DateFormat,
                                              std::string ColSeparator,
                                              unsigned int Precision);

    static std::string generateOutputVectorFileContent(const openfluid::core::Unit* aUnit, const std::string VectorName,
                                              openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                                              openfluid::base::SimulationInfo *SimInfo,
                                              std::string DateFormat,
                                              std::string ColSeparator,
                                              unsigned int Precision);

    void saveUnitFileOutput(openfluid::core::Unit* aUnit, int FileOutputIndex, int OutputSetIndex,
                            openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                            openfluid::base::SimulationInfo *SimInfo);

    void prepareUnitFileOutput(openfluid::core::Unit* aUnit, int FileOutputIndex, int OutputSetIndex);


  public:
    OutputsWriter(const std::string DirPath, const OutputDescriptor& OutDesc);

    ~OutputsWriter();

    void prepareDirectory();

    void saveToDirectory(openfluid::core::TimeStep_t CurrentStep,
                         openfluid::base::SimulationInfo *SimInfo, bool WithoutKeep);

};



#endif /* __OUTPUTSWRITER_H___ */
