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
 \file WareDescriptor.hpp
 \brief Header of ...

 \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#ifndef __WAREDESCRIPTOR_HPP__
#define __WAREDESCRIPTOR_HPP__

#include <openfluid/ware/PluggableWare.hpp>

namespace openfluid {
namespace fluidx {

class WareDescriptor
{
  public:

    enum WareType
    {
      NoWareType, PluggedFunction, Generator, PluggedObserver
    };

  private:

    static void getParamsRecurs(const boost::property_tree::ptree& Parent,
                                const std::string& Name,
                                std::map<std::string, std::string>& Contents);

  protected:

    WareType m_ModelItemType;
    openfluid::ware::WareParams_t m_Params;  // Function parameters set

  public:

    WareDescriptor();

    virtual ~WareDescriptor();

    /**
     * Setting an empty key parameter does nothing
     */
    void setParameter(const openfluid::ware::WareParamKey_t& Key,
                      const openfluid::ware::WareParamKey_t& Value);

    void setParameters(const openfluid::ware::WareParams_t& Params);

    openfluid::ware::WareParams_t getParameters();

    std::map<std::string, std::string> getParametersAsMap();

    /**
     * @warning be careful with Params structure, that must match a real xml structure
     * (see. http://www.boost.org/doc/libs/1_41_0/doc/html/boost/property_tree/basic_ptree.html#id973373-bb)
     */
    static std::map<std::string, std::string> getParamsAsMap(
        const openfluid::ware::WareParams_t& Params);

    bool isType(WareType MIType) const;

    WareType getType() const;

    /**
     * Does nothing if Key is empty or if key doesn't exist
     */
    void eraseParameter(const openfluid::ware::WareParamKey_t& Key);

    static bool eraseParamRecurs(boost::property_tree::ptree& pt,
                                 boost::property_tree::path& Path);

    void clearParameters();

    /**
     * Return true if the parameter with ParameterKey exists as a leaf, false otherwise
     */
    bool hasParameter(std::string ParameterKey);

    /**
     * Return true if the insertion of the parameter with ParameterKey will respects xml rules, ie :
     * - it exists *and* has no child (is empty) *and* doesn't end with a dot
     * - or it doesn't already exist *and* doesn't end with a dot *and* his parent, if has one, has at least a child (parent is not empty)
     * Return false otherwise
     */
    bool isInsertable(std::string ParameterKey);

};

}
}  // namespaces

#endif /* __WAREDESCRIPTOR_HPP__ */
