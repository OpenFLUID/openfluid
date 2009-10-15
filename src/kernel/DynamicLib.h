/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file DynamicLib.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __DYNAMICLIB_H___
#define __DYNAMICLIB_H___

#include <boost/filesystem/path.hpp>

class DynamicLib
{
  private:

    void* m_LibHandle;

    boost::filesystem::path m_LibPath;

    bool m_IsLoaded;

  public:

    DynamicLib(const std::string& LibName);

    ~DynamicLib();

    std::string getLibName() const { return m_LibPath.string(); }

    bool load();

    void unload();

    bool hasSymbol(const std::string& Name);

    void* getSymbol(const std::string& Name) const;

    bool isLoaded() const { return m_LibHandle != NULL; }
};


#endif /* __DYNAMICLIB_H___ */
