/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file MessagesWriter.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "MessagesWriter.h"
#include "openfluid-base.h"
#include <fstream>


// =====================================================================
// =====================================================================


MessagesWriter::MessagesWriter()
{

}


// =====================================================================
// =====================================================================


MessagesWriter::~MessagesWriter()
{

}


// =====================================================================
// =====================================================================


void MessagesWriter::saveToFile(std::string FilePath)
{
  std::ostringstream FileContents;

  openfluid::base::ExecutionMessages* ExecMsgs = openfluid::base::ExecutionMessages::getInstance();

  std::vector<openfluid::base::Message> WMessages = ExecMsgs->getWarningMsgs();

  unsigned int WarningCount = ExecMsgs->getWarningMsgs().size();

  for (unsigned int i=0; i<WarningCount;i++)
  {
    // TODO try to remove the following hack
    // hack for mingw32
#ifdef __MINGW32__
    FileContents << ("WARNING: ") << WMessages.at(i).getAsFormattedString() << std::endl;
#else
    FileContents << ("WARNING: ") << WMessages.at(i).getAsFormattedString() << std::endl;
#endif

  }

  std::ofstream OutMsgFile;

  OutMsgFile.open(FilePath.c_str(),std::ios::app);
  OutMsgFile << FileContents.str();
  OutMsgFile.close();

}
