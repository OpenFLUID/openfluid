/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file DynamicLib.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


// OpenFLUID:stylecheck:!incs
// OpenFLUID:stylecheck:!inco


#include <openfluid/global.hpp>

#ifdef OPENFLUID_OS_UNIX
#include <dlfcn.h>
#endif

#ifdef OPENFLUID_OS_WINDOWS
#include <windows.h>
#endif

#include <openfluid/machine/DynamicLib.hpp>


namespace openfluid { namespace machine {


DynamicLib::DynamicLib(const std::string& LibPath):
  m_LibPath(LibPath), m_LibHandle(nullptr)
{ }


// =====================================================================
// =====================================================================


DynamicLib::~DynamicLib()
{
  unload();
}


// =====================================================================
// =====================================================================


std::string DynamicLib::getPath() const 
{ 
  return m_LibPath; 
}


// =====================================================================
// =====================================================================


void DynamicLib::storeErrorMessage()
{
  // internally store the error message immediately 
  // instead of giving the ability to retreive it using a dedicated method that can be called later
  // to be sure to get the right message as it can be lost in the flow of system messages

  std::string Msg;

#ifdef OPENFLUID_OS_UNIX
  char* ErrMsg = dlerror();
  if (ErrMsg != nullptr)
  {
    Msg = std::string(ErrMsg);
  }
#endif

#ifdef OPENFLUID_OS_WINDOWS
  DWORD Err = GetLastError();
  if (Err)
  {
    LPVOID ErrMsgBuf;
    DWORD BufLen = 
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, Err,0,(LPTSTR)&ErrMsgBuf,0, NULL );
    if (BufLen)
    {
      LPCSTR MsgStr = (LPCSTR)ErrMsgBuf;
      Msg = std::string(MsgStr,MsgStr+BufLen);
      LocalFree(ErrMsgBuf);
    }
  }
#endif

  m_LatestErrMsg = Msg;
}


// =====================================================================
// =====================================================================


std::string DynamicLib::getLatestErrorMsg() const
{
  return m_LatestErrMsg;
}


// =====================================================================
// =====================================================================


bool DynamicLib::load()
{
  if (!isLoaded())
  {

#ifdef OPENFLUID_OS_UNIX
    m_LibHandle = dlopen(m_LibPath.c_str(), RTLD_LAZY);
#endif

#ifdef OPENFLUID_OS_WINDOWS
    m_LibHandle = (void*)LoadLibrary(m_LibPath.c_str());
#endif

    storeErrorMessage();
  }

  return isLoaded();
}


// =====================================================================
// =====================================================================


bool DynamicLib::unload()
{
  bool Ret;

  if (isLoaded())
  {
#ifdef OPENFLUID_OS_UNIX
    Ret = (dlclose(m_LibHandle) == 0);
#endif

#ifdef OPENFLUID_OS_WINDOWS
    Ret = FreeLibrary((HINSTANCE)m_LibHandle);
#endif

    storeErrorMessage();

    if (Ret)
    {
      m_LibHandle = nullptr;
    }

    return Ret;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool DynamicLib::isLoaded() const 
{ 
  return m_LibHandle != nullptr;
}


// =====================================================================
// =====================================================================


bool DynamicLib::hasSymbol(const std::string& Name) const
{
  if (isLoaded())
  {
#ifdef OPENFLUID_OS_UNIX
    return (dlsym(m_LibHandle, Name.c_str()) != nullptr);
#endif

#ifdef OPENFLUID_OS_WINDOWS
  return ((void*)GetProcAddress((HINSTANCE)m_LibHandle,Name.c_str()) != nullptr);
#endif
  }

  return false;
}


// =====================================================================
// =====================================================================


void* DynamicLib::getVoidSymbol(const std::string& Name) const
{
  void* Symbol = nullptr;

  if (isLoaded())
  {
#ifdef OPENFLUID_OS_UNIX
    Symbol = (dlsym(m_LibHandle, Name.c_str()));
#endif

#ifdef OPENFLUID_OS_WINDOWS
    Symbol = ((void*)GetProcAddress((HINSTANCE)m_LibHandle,Name.c_str()));
#endif
  }

  return Symbol;
}


} } //namespaces

