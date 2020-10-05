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
  @file NullValue.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_CORE_NULLVALUE_HPP__
#define __OPENFLUID_CORE_NULLVALUE_HPP__


#include <openfluid/core/Value.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {


/**
  NullValue is a container for null value, containing no data.\n

  @see Value

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "Values"],
    "title" : "NullValue",
    "text" : "openfluid::core::NullValue %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
class OPENFLUID_API NullValue : public Value
{
  public:

    /**
      Default constructor
    */
    NullValue() = default;

    /**
      Copy constructor
    */
    NullValue(const NullValue&) = default;

    NullValue& operator=(const Value&) override;

    NullValue& operator=(Value&&) override;

    NullValue& operator=(const NullValue&) = default;

    NullValue& operator=(NullValue&&) = default;

    virtual ~NullValue() = default;

    inline Type getType() const override
    {
      return Value::NULLL;
    }

    Value* clone() const override
    {
      return new NullValue(*this);
    }

    inline bool isSimple() const override
    {
      return false;
    }

    inline bool isCompound() const override
    {
      return false;
    }

    void writeToStream(std::ostream& OutStm) const override;

    void writeQuotedToStream(std::ostream& OutStm) const override
    {
      writeToStream(OutStm);
    }
};


} }  // namespaces


#endif /* __OPENFLUID_CORE_NULLVALUE_HPP__ */
