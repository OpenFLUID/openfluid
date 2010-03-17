/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file Message.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __MESSAGE_H___
#define __MESSAGE_H___

#include "openfluid-core.h"



namespace openfluid { namespace base {


class Message
{
  private:
    std::string m_Sender;
    std::string m_Source;
    openfluid::core::TimeStep_t m_TimeStep;
    bool m_IsTimeStep;
    std::string m_Content;


  public:

    Message(std::string Sender, std::string Source, openfluid::core::TimeStep_t TimeStep, std::string Content);

    Message(std::string Sender, std::string Source, std::string Content);

    std::string getAsFormattedString();

};

} }


#endif /* __MESSAGE_H___ */
