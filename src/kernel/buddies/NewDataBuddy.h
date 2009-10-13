/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file NewDataBuddy.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __NEWDATABUDDY_H___
#define __NEWDATABUDDY_H___


#include "OFBuddy.h"
#include <boost/filesystem/path.hpp>


class NewDataBuddy : public OpenFLUIDBuddy
{
  private:

    boost::filesystem::path m_OutputDirPath;

    std::string getXMLHeader();

    std::string getXMLFooter();

    void generateRunXML();

    void generateModelXML();

    void generateOutputXML();

    void generateEventsXML();

    void generateDDefXML();

    void generateDDataXML();

  public:

    NewDataBuddy();

    ~NewDataBuddy();

    bool run();

};



#endif /* __NEWDATABUDDY_H___ */
