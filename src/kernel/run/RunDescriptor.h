/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file RunDescriptor.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __RUNDESCRIPTOR_H__
#define __RUNDESCRIPTOR_H__

#include "openfluid-core.h"

class RunDescriptor
{
  private:
    int m_DeltaT;

    std::string m_SimID;

    openfluid::core::DateTime m_BeginDate;
    openfluid::core::DateTime m_EndDate;

    unsigned int m_ProgOutPacket;
    unsigned int m_ProgOutKeep;


  public:

    RunDescriptor(int DeltaT,
                  openfluid::core::DateTime BeginDate,openfluid::core::DateTime EndDate);

    ~RunDescriptor();

    openfluid::core::DateTime getBeginDate() const {return m_BeginDate; };

    openfluid::core::DateTime getEndDate() const {return m_EndDate; };

    int getDeltaT() const {return m_DeltaT; };

    void setSimulationID(const std::string SimID) { m_SimID = SimID; };

    std::string getSimulationID() const { return m_SimID; };

    bool isSimulationID() const { return (m_SimID != ""); };

    void setProgressiveOutput(const unsigned int Packet, const unsigned int Keep);

    bool isProgressiveOutput() const;

    unsigned int getProgressiveOutputPacket() const { return m_ProgOutPacket; };

    unsigned int getProgressiveOutputKeep() const { return m_ProgOutKeep; };


};

#endif /* __RUNDESCRIPTOR_H__ */
