/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file Message.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "Message.h"


namespace openfluid { namespace base {


// =====================================================================
// =====================================================================


Message::Message(std::string Sender, std::string Source, openfluid::core::TimeStep_t TimeStep, std::string Content)
{
  m_Sender = Sender;
  if (m_Sender == "") m_Sender = "unknown sender";

  m_Source = Source;
  if (m_Source == "") m_Source = "unknown source";

  m_TimeStep = TimeStep;
  m_IsTimeStep = true;

  m_Content = Content;
}


// =====================================================================
// =====================================================================


Message::Message(std::string Sender, std::string Source, std::string Content)
{
  m_Sender = Sender;
  if (m_Sender == "") m_Sender = "unknown sender";

  m_Source = Source;
  if (m_Source == "") m_Source = "unknown source";

  m_TimeStep = 0;
  m_IsTimeStep = false;
  m_Content = Content;
}


// =====================================================================
// =====================================================================


std::string Message::getAsFormattedString()
{
  std::ostringstream Msg;


  Msg << m_Content + " (message sent by " << m_Sender << " from " << m_Source;

  if (m_IsTimeStep)
  {
    Msg << ", at step " << m_TimeStep;
  }

  Msg << ")";

  return Msg.str();
}


} }
