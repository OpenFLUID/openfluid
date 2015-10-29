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
  @file TreeValue.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_CORE_TREEVALUE_HPP__
#define __OPENFLUID_CORE_TREEVALUE_HPP__

#include <string>

#include <openfluid/core/CompoundValue.hpp>
#include <openfluid/core/Tree.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {


class OPENFLUID_API TreeValue : public CompoundValue, public Tree<std::string,double>
{

  public:

    /**
      Default constructor
    */
    TreeValue() : CompoundValue(), Tree<std::string,double>()
    { }

    /**
      Copy constructor
    */
    TreeValue(const TreeValue& Val) :
      CompoundValue(),
      Tree<std::string,double>(static_cast<const Tree<std::string,double>& >(Val))
    { }

    virtual ~TreeValue()
    { }

    Value& operator =(const Value& Other);

    inline Type getType() const
    { return Value::TREE; }

    Value* clone() const
    { return new TreeValue(*this); };

    void writeToStream(std::ostream& OutStm) const;

    void writeQuotedToStream(std::ostream& OutStm) const
    { writeToStream(OutStm); }

};


} }  // namespaces


#endif /* __OPENFLUID_CORE_TREEVALUE_HPP__ */
