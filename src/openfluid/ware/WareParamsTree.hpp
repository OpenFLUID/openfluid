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
  @file WareParamsTree.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/



#ifndef __OPENFLUID_WARE_WAREPARAMSTREE_HPP__
#define __OPENFLUID_WARE_WAREPARAMSTREE_HPP__


#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/core/Tree.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace ware {


class OPENFLUID_API WareParamsTree
{
  public:

    typedef openfluid::core::Tree<std::string,openfluid::core::StringValue> WareParamsTreeNode;

  private:

    WareParamsTreeNode m_Root;


  public:

    WareParamsTree()
    { };

    WareParamsTree(const WareParams_t& Params);

    void setParams(const WareParams_t& Params);

    WareParamsTreeNode& root()
    { return m_Root; }

    const WareParamsTreeNode& root() const
    { return m_Root; }

    openfluid::core::StringValue getValueUsingFullKey(const std::string& FullName) const;

    openfluid::core::StringValue getValueUsingFullKey(const std::string& FullName,
                                                      const openfluid::core::StringValue& DefaultVal) const;

};


} }  // namespaces

#endif /* __OPENFLUID_WARE_WAREPARAMSTREE_HPP__ */
