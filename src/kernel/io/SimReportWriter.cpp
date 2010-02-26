/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file SimReportWriter.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include "openfluid-base.h"
#include "SimReportWriter.h"
#include "RuntimeEnv.h"
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>



// =====================================================================
// =====================================================================


SimulationReportWriter::SimulationReportWriter()
{

}


// =====================================================================
// =====================================================================


SimulationReportWriter::~SimulationReportWriter()
{

}


// =====================================================================
// =====================================================================


void SimulationReportWriter::saveToFile(std::string FilePath,openfluid::base::SimulationInfo *SimInfo)
{
  openfluid::core::UnitsListByClassMap_t::const_iterator UnitsIt;
  RuntimeEnvironment* RunEnv = RuntimeEnvironment::getInstance();
  openfluid::core::CoreRepository* Repository = openfluid::core::CoreRepository::getInstance();


  // ********** text file **********

  std::ostringstream FileContents;

  FileContents << "************************************************************" << "\n";
  FileContents << "*                                                          *\n";
  FileContents << "*                     Simulation report                    *" << "\n";
  FileContents << "*                                                          *\n";
  FileContents << "************************************************************" << "\n";
  FileContents << std::endl;


  FileContents << ("Simulation ID: ") << RunEnv->getSimulationID() << std::endl;
  FileContents << ("Date: ") <<  boost::posix_time::to_simple_string(RunEnv->getIgnitionDateTime()) << std::endl;
  FileContents << ("Computer: ") << RunEnv->getHostName() << std::endl;
  FileContents << ("User: ") << RunEnv->getUserID() << std::endl;
  FileContents << std::endl;
  FileContents << ("Input data set: ") << RunEnv->getInputDir() << std::endl;
  FileContents << ("Output data set: ") << RunEnv->getOutputDir()  << std::endl;
  FileContents << std::endl;

  FileContents << "Spatial domain:" << std::endl;
  for (UnitsIt = Repository->getUnits()->begin(); UnitsIt != Repository->getUnits()->end();++UnitsIt )
  {
    FileContents << "  - " << (*UnitsIt).first << ", " << (*UnitsIt).second.getList()->size() << " units" << std::endl;
  }

  if (SimInfo != NULL)
  {
    FileContents << std::endl;
    FileContents << ("Simulation period: ") << (SimInfo->getStartTime().getAsString(("%Y-%m-%d %H:%M:%S"))) << (" to ") << (SimInfo->getEndTime().getAsString(("%Y-%m-%d %H:%M:%S"))) << std::endl;
    FileContents << ("Time steps: ") << SimInfo->getStepsCount() << (" of ") << SimInfo->getTimeStep() << (" seconds") << std::endl;
  }


  // write file to disk

  std::ofstream SimInfoFile(FilePath.c_str(),std::ios::out);
  SimInfoFile << FileContents.str();
  SimInfoFile.close();

}
