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

#include <rapidjson/pointer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/tools/Filesystem.hpp>
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
  if (!m_Config.IsObject())
  {
    m_Config.SetObject();
  }

  // check that role key type is object
  if (m_Config.HasMember(m_Role.c_str()) && !m_Config[m_Role.c_str()].IsObject())
  {
    m_Config.RemoveMember(rapidjson::StringRef(m_Role.c_str()));
  }
  
  // set up role key in doc if not exists
  if (!m_Config.HasMember(m_Role.c_str()))
  {
    m_Config.AddMember(rapidjson::StringRef(m_Role.c_str()),rapidjson::Value(rapidjson::kObjectType),
                                             m_Config.GetAllocator()); 
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

  rapidjson::IStreamWrapper InputStream(FileStream);
  m_Config.ParseStream(InputStream);

  if (m_Config.HasParseError() || !m_Config.IsObject())
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
  if (openfluid::tools::Filesystem::makeDirectory(openfluid::tools::Filesystem::dirname(m_BackendFile)))
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
  rapidjson::OStreamWrapper OutSW { OutS };
  rapidjson::PrettyWriter<rapidjson::OStreamWrapper> Writer(OutSW);
  Writer.SetIndent(' ',2);
  m_Config.Accept(Writer);
}


// =====================================================================
// =====================================================================


const SettingValue SettingsBackend::getValue(const std::string& Pointer) const
{
  std::string Ptr = getPointerWithRole(Pointer);

  const rapidjson::Value* Val = rapidjson::Pointer(Ptr.c_str()).Get(m_Config);

  if (Val != nullptr)
  {
    if (Val->IsString())
    {
      return SettingValue(std::string(Val->GetString()));
    }
    else if (Val->IsBool())
    {
      return SettingValue(Val->GetBool());
    }
    else if (Val->IsInt())
    {
      return SettingValue(Val->GetInt());
    }
    else if (Val->IsDouble())
    {
      return SettingValue(Val->GetDouble());
    }
    else if (Val != nullptr && Val->IsArray())
    {
      std::vector<std::string> StrVect;
      for (auto& v : Val->GetArray())
      {
        if (v.IsString())
        {
          StrVect.push_back(v.GetString()); 
        }
        else
        {
          return SettingValue(Val); // not an array of strings
        }
      }
      return StrVect;
    }
    else
    {
      return SettingValue(Val);
    }
  }

  return SettingValue();
}


// =====================================================================
// =====================================================================


void SettingsBackend::setValue(const std::string& ParentPointer, const std::string& Key, 
                               const SettingValue& Val, bool AutoCreate)
{
  std::string ParentPtr = getPointerWithRole(ParentPointer);

  if (rapidjson::Pointer(ParentPtr.c_str()).Get(m_Config) != nullptr || AutoCreate)
  {
    rapidjson::Value JSONValue;
    rapidjson::Value::AllocatorType& Allocator = m_Config.GetAllocator();

    if (Val.is<std::string>())
    {
      JSONValue.SetString(Val.get<std::string>().c_str(),Allocator);
    }    
    else if (Val.is<bool>())
    {
      JSONValue.SetBool(Val.get<bool>());
    }
    else if (Val.is<int>())
    {
      JSONValue.SetInt(Val.get<int>());
    }
    else if (Val.is<double>())
    {
      JSONValue.SetDouble(Val.get<double>());
    }
    else if (Val.is<std::vector<std::string>>())
    {
      JSONValue = rapidjson::Value(rapidjson::kArrayType);

      for (auto Str: Val.get<std::vector<std::string>>())
      {
        rapidjson::Value StrVal;
        StrVal.SetString(Str.c_str(),Allocator);
        JSONValue.PushBack(StrVal,Allocator);
      }
    }
    else if (Val.isJSONValue())
    {
      JSONValue.CopyFrom(*(Val.JSONValue()),Allocator);
    }
    else
    {
      JSONValue.SetObject();
    }

    std::string Ptr = ParentPtr + "/" + Key;
    rapidjson::Pointer(Ptr.c_str()).Set(m_Config,JSONValue);
    autoSave();
  }
  else
  { 
    throw openfluid::base::FrameworkException(
      OPENFLUID_CODE_LOCATION,"Path '" + ParentPtr + "' does not exist to create the '" + Key + "' key "
    );
  }

}


// =====================================================================
// =====================================================================


void SettingsBackend::remove(const std::string& Pointer)
{
  std::string Ptr = getPointerWithRole(Pointer);

  rapidjson::Pointer(Ptr.c_str()).Erase(m_Config);
  autoSave();
}


// =====================================================================
// =====================================================================


bool SettingsBackend::exists(const std::string& Pointer) const
{
  std::string Ptr = getPointerWithRole(Pointer);

  return (rapidjson::Pointer(Ptr.c_str()).Get(m_Config) != nullptr);
}


} }  // namespaces

