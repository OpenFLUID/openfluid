/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file DynamicLib.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "DynamicLib.h"
#include "openfluid-base.h"

#if defined __unix__ || defined __APPLE__
#include <dlfcn.h>
#endif

#if WIN32
#include <windows.h>
#endif


// =====================================================================
// =====================================================================


DynamicLib::DynamicLib(const std::string& LibName)
{
  m_IsLoaded = false;
  m_LibPath = boost::filesystem::path(LibName);
  m_LibHandle = NULL;
}


// =====================================================================
// =====================================================================


DynamicLib::~DynamicLib()
{
  unload();
}


// =====================================================================
// =====================================================================


bool DynamicLib::load()
{

  if (isLoaded()) unload();

#if defined __unix__ || defined __APPLE__
  m_LibHandle = dlopen(m_LibPath.string().c_str(), RTLD_LAZY);
#endif

#ifdef WIN32
  m_LibHandle = (void*)LoadLibrary(m_LibPath.string().c_str());
#endif

  return m_LibHandle != NULL;
}


// =====================================================================
// =====================================================================


void DynamicLib::unload()
{

  if (isLoaded())
  {
#if defined __unix__ || defined __APPLE__
    dlclose(m_LibHandle);
#endif

#ifdef WIN32
    FreeLibrary((HINSTANCE)m_LibHandle);
#endif
  }

  m_LibHandle = NULL;
}


// =====================================================================
// =====================================================================


bool DynamicLib::hasSymbol(const std::string& Name)
{
  if (isLoaded())
  {
#if defined __unix__ || defined __APPLE__
    return (dlsym(m_LibHandle, Name.c_str()) != NULL);
#endif

#ifdef WIN32
  return ((void*)GetProcAddress((HINSTANCE)m_LibHandle,Name.c_str()) != NULL);
#endif
  }

  return false;
}


// =====================================================================
// =====================================================================


void* DynamicLib::getSymbol(const std::string& Name) const
{

  if (isLoaded())
  {
#if defined __unix__ || defined __APPLE__
    return (dlsym(m_LibHandle, Name.c_str()));
#endif

#ifdef WIN32
    return ((void*)GetProcAddress((HINSTANCE)m_LibHandle,Name.c_str()));
#endif
  }

  return NULL;
}


// =====================================================================
// =====================================================================


