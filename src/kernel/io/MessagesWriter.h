/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file MessagesWriter.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __MESSAGESWRITER_H___
#define __MESSAGESWRITER_H___


#include <string>

class MessagesWriter
{
  private:

  public:
    MessagesWriter();

    ~MessagesWriter();

    static void saveToFile(std::string FilePath);

};


#endif /* __MESSAGESWRITER_H___ */
