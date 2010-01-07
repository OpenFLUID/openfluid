/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OutputFilesDescriptor.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OUTPUTFILESDESCRIPTOR_H___
#define __OUTPUTFILESDESCRIPTOR_H___

#include <vector>
#include "OutputSetDescriptor.h"

class OutputFilesDescriptor
{
  private:

    std::vector<OutputSetDescriptor> m_Sets;
    std::string m_ColSeparator;
    std::string m_DateFormat;
    std::string m_CommentChar;


  public:

    OutputFilesDescriptor();

    ~OutputFilesDescriptor();

    std::vector<OutputSetDescriptor>& getSets() { return m_Sets; };

    std::string getColSeparator() const { return m_ColSeparator; };

    void setColSeparator(const std::string& ColSep) { m_ColSeparator = ColSep; };

    std::string getDateFormat() const { return m_DateFormat; };

    void setDateFormat(const std::string& DateFormat) { m_DateFormat = DateFormat; };

    std::string getCommentChar() const { return m_CommentChar; };

    void setCommentChar(const std::string& CommentChar) { m_CommentChar = CommentChar; };


};


#endif /* __OUTPUTFILESDESCRIPTOR_H___ */
