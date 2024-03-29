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
  @file MapValue.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_CORE_MAPVALUE_HPP__
#define __OPENFLUID_CORE_MAPVALUE_HPP__


#include <map>
#include <memory>

#include <openfluid/core/CompoundValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/core/VectorValue.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {

/**
  MapValue is a container for a key => value map,
  where keys are strings and values can be any type derived from openfluid::core::Value.\n

  @see Value

  <I>Example : declaration</I>
  @snippet misc/values.cpp map_decl

  <I>Example : setting the contained values</I>
  @snippet misc/values.cpp map_set

  <I>Example : getting the contained values</I>
  @snippet misc/values.cpp map_get

  <I>Example : testing the contained elements</I>
  @snippet misc/values.cpp map_test

  <I>Example : conversion from string</I>
  @snippet misc/values.cpp map_fromstr

  <I>Example : conversion to string</I>
  @snippet misc/values.cpp map_tostr

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Types", "Values"],
    "title" : "MapValue",
    "text" : "openfluid::core::MapValue %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
class OPENFLUID_API MapValue : public CompoundValue
{
  public:

    typedef std::map<std::string,std::shared_ptr<Value>> Map_t;

    typedef Map_t::iterator iterator;

    typedef Map_t::const_iterator const_iterator;


  private:

    Map_t m_Value;


  public:

    /**
      Default constructor
    */
    MapValue() : CompoundValue()
    { }

    /**
      Copy constructor
    */
    MapValue(const MapValue& Val);

    /**
      Move constructor
    */
    MapValue(MapValue&& Val);

    MapValue(const Map_t& Val) : CompoundValue(), m_Value(Val)
    { }

    MapValue& operator=(const Value& Other) override;

    MapValue& operator=(Value&& Other) override;

    MapValue& operator=(const MapValue& Other) = default;

    MapValue& operator=(MapValue&& Other) = default;

    ~MapValue();

    inline Type getType() const override
    {
      return Value::MAP;
    }

    Value* clone() const override
    {
      return new MapValue(*this);
    }

    void writeToStream(std::ostream& OutStm) const override;

    void writeQuotedToStream(std::ostream& OutStm) const override
    {
      writeToStream(OutStm);
    }

    /**
      Sets a new value at the given key
      @param[in] Key the key to add
      @param[in] Element the element to add, must be derived from openfluid::core::Value
    */
    void set(const std::string& Key, Value* Element);

    /**
      Sets a new double value at the given key
      @param[in] Key the key to add
      @param[in] Val the value to add
    */
    inline void setDouble(const std::string& Key, const double& Val)
    {
      set(Key,new DoubleValue(Val));
    }

    /**
      Sets a new long value at the given key
      @param[in] Key the key to add
      @param[in] Val the value to add

    */
    inline void setInteger(const std::string& Key, const long& Val)
    {
      set(Key,new IntegerValue(Val));
    }

    /**
      Sets a new boolean value at the given key
      @param[in] Key the key to add
      @param[in] Val the value to add
    */
    inline void setBoolean(const std::string& Key, const bool& Val)
    {
      set(Key,new BooleanValue(Val));
    }

    /**
      Sets a new string value at the given key
      @param[in] Key the key to add
      @param[in] Val the value to add
    */
    inline void setString(const std::string& Key, const std::string& Val)
    {
      set(Key,new StringValue(Val));
    }

    /**
      Sets a new VectorValue value at the given key
      @param[in] Key the key to add
      @param[in] Val the value to add
    */
    inline void setVectorValue(const std::string& Key, const VectorValue& Val)
    {
      set(Key,new VectorValue(Val));
    }

    /**
      Sets a new MatrixValue value at the given key
      @param[in] Key the key to add
      @param[in] Val the value to add
    */
    inline void setMatrixValue(const std::string& Key, const MatrixValue& Val)
    {
      set(Key,new MatrixValue(Val));
    }

    /**
      Sets a new MapValue value at the given key
      @param[in] Key the key to add
      @param[in] Val the value to add
    */
    inline void setMapValue(const std::string& Key, const MapValue& Val)
    {
      set(Key,new MapValue(Val));
    }

    /**
      Operator to get/set a value at a key given between []
      @return the value at the given key
    */
    Value& operator[](const std::string& Key);

    /**
      Returns a reference to the value of the map at the given key
      @param[in] Key the key of the requested value
      @return the value at the given key
    */
    Value& at(const std::string& Key);

    /**
      Returns a const reference to the value of the map at the given key
      @param[in] Key the key of the requested value
      @return the value at the given key
    */
    const Value& at(const std::string& Key) const;

    /**
      Returns the double value of the map at the given key
      @param[in] Key the key of the requested value
      @return the value at the given key
    */
    inline double getDouble(const std::string& Key) const
    {
      return at(Key).asDoubleValue().get();
    }

    /**
      Returns the long value of the map at the given key
      @param[in] Key the key of the requested value
      @return the value at the given key
    */
    inline long getInteger(const std::string& Key) const
    {
      return at(Key).asIntegerValue().get();
    }

    /**
      Returns the boolean value of the map at the given key
      @param[in] Key the key of the requested value
      @return the value at the given key
    */
    inline bool getBoolean(const std::string& Key) const
    {
      return at(Key).asBooleanValue().get();
    }

    /**
      Returns the string value of the map at the given key
      @param[in] Key the key of the requested value
      @return the value at the given key
    */
    inline std::string getString(const std::string& Key) const
    {
      return at(Key).asStringValue().get();
    }

    /**
      Returns the VectorValue value of the map at the given key
      @param[in] Key the key of the requested value
      @return the value at the given key
    */
    inline VectorValue getVectorValue(const std::string& Key) const
    {
      return at(Key).asVectorValue();
    }

    /**
      Returns the MatrixValue value of the map at the given key
      @param[in] Key the key of the requested value
      @return the value at the given key
    */
    inline MatrixValue getMatrixValue(const std::string& Key) const
    {
      return at(Key).asMatrixValue();
    }

    /**
      Returns the MapValue value of the map at the given key
      @param[in] Key the key of the requested value
      @return the value at the given key
    */
    inline MapValue getMapValue(const std::string& Key) const
    {
      return at(Key).asMapValue();
    }

    /**
      Removes the value corresponding to the given key
      @param[in] Key the key to remove
    */
    bool remove(const std::string& Key);

    /**
      Returns the size of the map
      @return size of the map
    */
    inline unsigned long getSize() const
    {
      return m_Value.size();
    }

    /**
      Returns the size of the map
      @return size of the map
    */
    unsigned long size() const
    {
      return m_Value.size();
    }

    /**
      Checks if the given key exists
      @param[in] Key the key to check
      @return true if the given key is present
    */
    inline bool isKeyExist(const std::string& Key) const
    {
      return (m_Value.find(Key) != m_Value.end());
    }

    /**
      Returns the list of keys of the map
      @return a std::vector of std::string containing the keys of the map
    */
    std::vector<std::string> getKeys() const;

    /**
      Clears the map by removing all values
    */
    void clear();

    /**
      Returns an iterator referring to the first element in the map
      @return an iterator to the first element in the map
    */
    inline iterator begin()
    {
      return m_Value.begin();
    }

    /**
      Returns a constant iterator referring to the first element in the map
      @return a constant iterator to the first element in the map
    */
    inline const_iterator begin() const
    {
      return m_Value.begin();
    }

    /**
      Returns an iterator referring to the past-the-end element in the map
      @return an iterator to the past-the-end element in the map
    */
    inline iterator end()
    {
      return m_Value.end();
    }

    /**
      Returns a constant iterator referring to the past-the-end element in the map
      @return a constant iterator to the past-the-end element in the map
    */
    inline const_iterator end() const
    {
      return m_Value.end();
    }

};


} }  // namespaces


// =====================================================================
// =====================================================================


#endif /* __OPENFLUID_CORE_MAPVALUE_HPP__ */
