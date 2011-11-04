/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/

/**
  \file MapValue.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __MAPVALUE_HPP__
#define __MAPVALUE_HPP__


#include <map>
#include <boost/shared_ptr.hpp>

#include <openfluid/core/CompoundValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/core/VectorValue.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {

class DLLEXPORT MapValue : public CompoundValue
{
  public:
    typedef std::map<std::string,boost::shared_ptr<Value> > Map_t;


  private:

    Map_t m_Value;

  public:

    /**
      Default constructor
    */
    MapValue() : CompoundValue() {};

    /**
      Copy constructor
    */
    MapValue(const MapValue& Val);

    MapValue(const Map_t& Val) : CompoundValue(), m_Value(Val) {};

    Value& operator =(const Value& Other);

    ~MapValue();

    inline Type getType() const { return Value::MAP; };

    Value* clone() const { return new MapValue(*this); };

    void writeToStream(std::ostream& OutStm) const;

    /**
      Sets a new value for element at the given key
    */
    void set(const std::string& Key, Value* Element);

    /**
      Sets a new double value for element at the given key
    */
    inline void setDouble(const std::string& Key, const double& Val)
      { set(Key,new DoubleValue(Val)); };

    /**
      Sets a new double value for element at the given key
    */
    inline void setInteger(const std::string& Key, const long& Val)
      { set(Key,new IntegerValue(Val)); };

    /**
      Sets a new boolean value for element at the given key
    */
    inline void setBoolean(const std::string& Key, const bool& Val)
      { set(Key,new BooleanValue(Val)); };

    /**
      Sets a new string value for element at the given key
    */
    inline void setString(const std::string& Key, const std::string& Val)
      { set(Key,new StringValue(Val)); };

    /**
      Sets a new VectorValue value for element at the given key
    */
    inline void setVectorValue(const std::string& Key, const VectorValue& Val)
      { set(Key,new VectorValue(Val)); };

    /**
      Sets a new MatrixValue value for element at the given key
    */
    inline void setMatrixValue(const std::string& Key, const MatrixValue& Val)
      { set(Key,new MatrixValue(Val)); };

    /**
      Operator to get/set a new value for element given between []
    */
    Value& operator[](const std::string& Key);

    /**
      Returns the element of the map for key Key
    */
    Value& get(const std::string& Key);

    /**
      Returns a double element of the map for key Key
    */
    inline double getDouble(const std::string& Key) { return get(Key).asDoubleValue().get(); };

    /**
      Returns a long element of the map for key Key
    */
    inline long getInteger(const std::string& Key) { return get(Key).asIntegerValue().get(); };

    /**
      Returns a bool element of the map for key Key
    */
    inline bool getBoolean(const std::string& Key) { return get(Key).asBooleanValue().get(); };

    /**
      Returns a string element of the map for key Key
    */
    inline std::string getString(const std::string& Key) { return get(Key).asStringValue().get(); };

    /**
      Returns a VectorValue element of the map for key Key
    */
    inline VectorValue getVectorValue(const std::string& Key) { return get(Key).asVectorValue(); };

    /**
      Returns a MatrixValue element of the map for key Key
    */
    inline MatrixValue getMatrixValue(const std::string& Key) { return get(Key).asMatrixValue(); };



    bool remove(const std::string& Key);

    /**
      Returns the size of the map
    */
    inline unsigned long getSize() const { return m_Value.size(); };

    /**
      Returns the size of the map
    */
    unsigned long size() const { return m_Value.size(); };

    inline bool isKeyExist(const std::string& Key) const { return (m_Value.find(Key) != m_Value.end()); };

    void clear();

};


} }  // namespaces


// =====================================================================
// =====================================================================


#endif /* __MAPVALUE_HPP__ */
