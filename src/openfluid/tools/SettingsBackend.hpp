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
  @file SettingsBackend.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_TOOLS_SETTINGSBACKEND_HPP__
#define __OPENFLUID_TOOLS_SETTINGSBACKEND_HPP__


#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <variant>

#include <openfluid/thirdparty/JSON.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {

/**
  Stores and manages values for SettingsBackend
*/
class OPENFLUID_API SettingValue 
{
  private:

    std::variant<std::monostate,bool,int,double,std::string,std::vector<std::string>> m_Value;

    openfluid::thirdparty::json m_JSONValue = openfluid::thirdparty::json::value_t::null;


  public:
    
    /**
      Constructor of an empty value
      @see isNull()
    */
    SettingValue()
    { }

    /**
      Constructor from a boolean value
    */
    SettingValue(bool Val) : m_Value(Val)
    { }

    /**
      Constructor from an integer value
    */
    SettingValue(int Val) : m_Value(Val)
    { }

    /**
      Constructor from a double value
    */
    SettingValue(double Val) : m_Value(Val)
    { }

    /**
      Constructor from an char* value (mainly for casting to std::string)
    */
    SettingValue(const char* Val) : m_Value(std::string(Val))
    { }

    /**
      Constructor from a string value
    */
    SettingValue(const std::string& Val) : m_Value(Val)
    { }

    /**
      Constructor from a vector of strings
    */
    SettingValue(const std::vector<std::string>& Val) : m_Value(Val)
    { }

    /**
      Constructor from a native JSON value
    */
    static SettingValue fromJSON(const openfluid::thirdparty::json& Val)
    {
      SettingValue StngVal;
      StngVal.m_JSONValue = Val;
      return StngVal;
    }


    /**
      Returns the stored value if it matches the given type.
      If the stored value dos not match the type, it returns a default value if given, 
      otherwise throws an exception.
      @tparam T The requested type
      @param[in] DefaultValue The value returned by default
      @return the stored value or the default value
    */
    template<typename T>
    const T get(const std::optional<T>& DefaultValue = std::nullopt) const
    {
      if (std::holds_alternative<T>(m_Value))
      {
        return std::get<T>(m_Value);
      }

      if (DefaultValue.has_value())
      {
        return DefaultValue.value();
      }

      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "Setting value is not of correct type");
    }

    /**
      Returns if the stored value is a JSON tree, otherwise returns nullptr
      @return the stored JSON tree
    */
    const openfluid::thirdparty::json JSONValue() const
    {
      return m_JSONValue;
    }

    /**
      Checks if the stored value matches the given type
      @tparam T The requested type
      @return true if the type is correct
    */
    template<typename T>
    bool is() const
    {
      return std::holds_alternative<T>(m_Value);
    }

    /**
      Returns true if the stored value is a JSON tree
      @return true if the value is a JSON structure
    */
    bool isJSONValue() const
    {
      return !m_JSONValue.is_null();
    }

    /**
      Check if the value is null (it is neither a stored value nor a JSON tree)
      @return true if the value is null
    */
    bool isNull() const
    {
      return (m_Value.index() == 0) && (m_JSONValue.is_null());
    }
};


// =====================================================================
// =====================================================================


/**
  Manages settings using a JSON-based backend.
  The JSON structure is stored under a key representing the "role" of the settings

  e.g. with a "myapp-config" role
  @code{.json}
  {
    "myapp-config": {
      "automatic": true,
      "properties": {
        "size": 5,
        "color": "blue"
      },
      "last_use": "2021-07-16 14:11:37"
    }
  }
  @endcode

  Access to values is made using JSON pointers (see rfc6901 https://datatracker.ietf.org/doc/html/rfc6901)
*/
class OPENFLUID_API SettingsBackend
{
  private:

    openfluid::thirdparty::json m_Config;

    std::string m_BackendFile;

    bool m_AutoSave;

    void prepareForData();

    bool load();

    bool autoSave() const;
    

  public:

    /**
      Constructor of for a settings backend.
      If no BackendFile is given, the settings are not persistent and will be lost when the object is destroyed.
      @param[in] BackendFile Path to file where settings are stored. Default is empty (no persistent storage)
      @param[in] AutoSave If true, automatically saves settings to file (i any) at each modification. Enabled by default
    */
    SettingsBackend(const std::string& BackendFile = "", bool AutoSave = true);

    ~SettingsBackend();

    /**
      Returns the path of the backend file for persistance
      @return The path to the backend file
    */
    std::string getBackendFilePath() const
    {
      return m_BackendFile;
    }

    /**
      Enables or disables the automatic saving to backend file
      @param[in] AutoSave true to enable, false to disable
    */
    void enableAutoSave(bool AutoSave)
    {
      m_AutoSave = AutoSave;
    }

    /**
      Returns the status of the automatic saving (enabled/disabled)
      @return true if enabled, false if disabled
    */
    bool isAutoSave() const
    {
      return m_AutoSave && !m_BackendFile.empty();
    }

    /**
      Triggers the saving of the settings to the backend file
      @return true if the operation has been completed 
    */
    bool save() const;

    /**
      Writes the JSON structure of the settings to a stream
      @param[out] OutS The stream in which the JSON is written
    */
    void writeToStream(std::ostream& OutS) const;

    /**
      Returns a reference to the raw JSON data structure
      @return a reference to the JSON document
    */ 
    openfluid::thirdparty::json& data()
    {
      return m_Config;
    }

    /**
      Gets a value located at the given pointer. If the pointed location does not exist, a null value is returned
      @param[in] Pointer The pointer to the requested value
      @return the value located at the pointer
    */ 
    const SettingValue getValue(const std::string& Pointer) const;

    /**
      Creates or updates a value named by its key as a child of the parent pointer
      @param[in] ParentPointer the parent pointer in which the key/value will be created or updated
      @param[in] Key the key for the value
      @param[in] Value the value
      @param[in] AutoCreate if true, the path of the parent pointer is automatically created if it does not exist.
                            Default is true 
    */
    void setValue(const std::string& ParentPointer, const std::string& Key, 
                  const SettingValue& Value, bool AutoCreate = true);

    /**
      Removes the value located by the given pointer
      @param[in] Pointer the pointer to the value to remove
    */
    void remove(const std::string& Pointer);

    /**
      Checks if a value exists at the location given by the pointer
      @param[in] Pointer the pointer to the value to check
    */
    bool exists(const std::string& Pointer) const;
};


} }  // namespaces


#endif /* __OPENFLUID_TOOLS_SETTINGSBACKEND_HPP__ */
