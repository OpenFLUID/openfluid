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
  @file SettingsBackend.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <fstream>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/tools/SettingsBackend.hpp>


namespace openfluid { namespace tools {


SettingsBackend::SettingsBackend(const std::string& BackendFile, const std::string& Role, bool AutoSave) :
  m_BackendFile(BackendFile), m_Role(Role), m_AutoSave(AutoSave)
{
  prepareForData();

  if (!BackendFile.empty())
  {
    load();
  }
}


// =====================================================================
// =====================================================================


SettingsBackend::~SettingsBackend()
{

}


// =====================================================================
// =====================================================================


std::string SettingsBackend::getPointerWithRole(const std::string& Pointer) const
{
  std::string Ptr = openfluid::tools::trim(Pointer);

  if (Ptr.empty() || (Ptr == "/"))
  {
    Ptr = "/"+m_Role;
  }
  else
  {
    Ptr = "/"+m_Role+Ptr;
  }
  
  return Ptr;
}


// =====================================================================
// =====================================================================


void SettingsBackend::prepareForData()
{
  // set up doc as object
  if (!m_Config.is_object())
  {
    m_Config = openfluid::thirdparty::json::object();
  }

  // check that role key type is object
  auto itRole = m_Config.find(m_Role);
  if (itRole!=m_Config.end() && !(*itRole).is_object())
  {
    m_Config.erase(itRole);
  }
  
  // set up role key in doc if not exists
  if (m_Config.find(m_Role) == m_Config.end())
  {
    m_Config[m_Role] = openfluid::thirdparty::json::object(); 
  }
}


// =====================================================================
// =====================================================================


bool SettingsBackend::load()
{
  std::ifstream FileStream;

  FileStream.open(m_BackendFile.c_str(),std::ifstream::in);
  if (!FileStream.is_open())
  {
    return false;
  }

  try
  {
    m_Config = openfluid::thirdparty::json::parse(FileStream);
  }
  catch (openfluid::thirdparty::json::parse_error&)
  {
    return false;
  }
  
  if (!m_Config.is_object())
  {
    return false;
  }

  return true;
}


// =====================================================================
// =====================================================================


bool SettingsBackend::autoSave() const
{
  if (isAutoSave())
  {
    return save();
  }

  return false;
}


// =====================================================================
// =====================================================================


bool SettingsBackend::save() const
{
  auto BF = openfluid::tools::FilesystemPath(m_BackendFile);
  auto BFDir = openfluid::tools::FilesystemPath(BF.dirname());

  if (BFDir.makeDirectory())
  {
    std::ofstream OutFS(m_BackendFile,std::ofstream::out);

    if (OutFS.is_open())
    {
      writeToStream(OutFS);
    }    
  }
  return false;
}


// =====================================================================
// =====================================================================


void SettingsBackend::writeToStream(std::ostream& OutS) const
{
  OutS << m_Config.dump(2);
}


// =====================================================================
// =====================================================================


const SettingValue SettingsBackend::getValue(const std::string& Pointer) const
{
  std::string Ptr = getPointerWithRole(Pointer);

  try 
  {
    const openfluid::thirdparty::json Val = m_Config.at(openfluid::thirdparty::json::json_pointer(Ptr));

    if (Val.is_string())
    {
      return SettingValue(Val.get<std::string>());
    }
    else if (Val.is_boolean())
    {
      return SettingValue(Val.get<bool>());
    }
    else if (Val.is_number_integer())
    {
      return SettingValue(Val.get<int>());
    }
    else if (Val.is_number_float())
    {
      return SettingValue(Val.get<double>());
    }
    else if (Val.is_array())
    {
      std::vector<std::string> StrVect;
      for (auto& v : Val)
      {
        if (v.is_string())
        {
          StrVect.push_back(v.get<std::string>()); 
        }
        else
        {
          return SettingValue::fromJSON(Val); // not an array of strings
        }
      }
      return SettingValue(StrVect);
    }
    else
    {
      return SettingValue::fromJSON(Val);
    }
  }
  catch (...)
  { }

  return SettingValue();
}


// =====================================================================
// =====================================================================


void SettingsBackend::setValue(const std::string& ParentPointer, const std::string& Key, 
                               const SettingValue& Val, bool AutoCreate)
{
  std::string ParentPtr = getPointerWithRole(ParentPointer);

  if (!AutoCreate)
  {
    try 
    {
      m_Config.at(openfluid::thirdparty::json::json_pointer(ParentPtr));
    }
    catch (...)
    {
      throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,"Path '" + ParentPtr + "' does not exist to create the '" + Key + "' key "
      );
    }
  }

  openfluid::thirdparty::json JSONValue = openfluid::thirdparty::json::object();

  if (Val.is<std::string>())
  {
    JSONValue = Val.get<std::string>();
  }
  else if (Val.is<bool>())
  {
    JSONValue = Val.get<bool>();
  }
  else if (Val.is<int>())
  {
    JSONValue = Val.get<int>();
  }
  else if (Val.is<double>())
  {
    JSONValue = Val.get<double>();
  }
  else if (Val.is<std::vector<std::string>>())
  {
    JSONValue = Val.get<std::vector<std::string>>();
  }
  else if (Val.isJSONValue())
  {
    JSONValue = Val.JSONValue();
  }

  m_Config[openfluid::thirdparty::json::json_pointer(ParentPtr + "/" + Key)] = JSONValue;

  autoSave();
}


// =====================================================================
// =====================================================================


void SettingsBackend::remove(const std::string& Pointer)
{
  std::string Ptr = getPointerWithRole(Pointer);

  try
  {
    const auto JSONptr = openfluid::thirdparty::json::json_pointer(Ptr);
    const std::string Key = JSONptr.back();
    const auto ParentJSONptr = JSONptr.parent_pointer();

    m_Config.at(ParentJSONptr).erase(Key);
    autoSave();
  }
  catch (...)
  {

  }
}


// =====================================================================
// =====================================================================


bool SettingsBackend::exists(const std::string& Pointer) const
{
  std::string Ptr = getPointerWithRole(Pointer);

  try 
  {
    auto Val = m_Config.at(openfluid::thirdparty::json::json_pointer(Ptr));
  }
  catch (...)
  {
    return false;
  }
  
  return true;
}


} }  // namespaces

