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
 \file BuilderDomain.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef BUILDERDOMAIN_HPP_
#define BUILDERDOMAIN_HPP_

#include <openfluid/core/TypeDefs.hpp>
#include <map>
#include <set>

namespace openfluid {
namespace fluidx {
class DomainDescriptor;
class UnitDescriptor;
class InputDataDescriptor;
}
namespace core {
class Event;
}
}


namespace openfluid {
namespace guicommon {

// =====================================================================
// =====================================================================

class BuilderUnit
{

  public:

    openfluid::fluidx::UnitDescriptor* mp_UnitDesc;

    std::map<openfluid::core::InputDataName_t, std::string*> m_IData;

    std::list<openfluid::core::Event*> m_Events;

    BuilderUnit(openfluid::fluidx::UnitDescriptor& UnitDesc) :
        mp_UnitDesc(&UnitDesc)
    {
    }
    ;

};

// =====================================================================
// =====================================================================

class BuilderDomain
{
  public:

    typedef std::map<std::string, std::map<int, BuilderUnit> > UnitsByIdByClass_t;

  private:

    openfluid::fluidx::DomainDescriptor* mp_DomainDesc;

    /**
     * Units by ID by ClassName
     */
    std::map<std::string, std::map<int, BuilderUnit> > m_Units;

    std::map<std::string, std::set<std::string> > m_IDataNames;

    void dispatchUnits();

    void dispatchIData();

    void checkIDataConsistency();

    void dispatchEvents();

  public:

    BuilderDomain(openfluid::fluidx::DomainDescriptor& DomainDesc);

    ~BuilderDomain();

    /**
     *
     * @return May return an empty map. Never throws.
     */
    const std::map<std::string, std::map<int, BuilderUnit> >& getUnitsByIdByClass();

    /**
     * @throw openfluid::base::OFException if Unit doesn't exist
     * @param ClassName
     * @param ID
     * @return
     */
    const BuilderUnit& getUnit(std::string ClassName, int ID);

    /**
     * @throw openfluid::base::OFException if Unit doesn't exist
     * @param ClassName
     * @param ID
     * @return
     */
    const openfluid::fluidx::UnitDescriptor& getUnitDescriptor(
        std::string ClassName, int ID);

    /**
     *
     * @param ClassName
     * @return An empty set if ClassName doesn't exist. Never throws.
     */
    std::set<int> getIDsOfClass(std::string ClassName);

    bool isClassNameExists(std::string ClassName);

    /**
     *
     * @return May return an empty set. Never throws.
     */
    std::set<std::string> getClassNames();

    /**
     * @details Add this UnitDesc ID to the IData descriptor for all input data of this UnitDesc class,
     * with a default value of "-"
     * @throw openfluid::base::OFException if Unit already exists
     * @param UnitDesc
     */
    void addUnit(openfluid::fluidx::UnitDescriptor* UnitDesc);

    /**
     * Does nothing if Unit doesn't exist
     *
     * @param ClassName
     * @param ID
     */
    void deleteUnit(std::string ClassName, int ID);

    /**
     * @throw openfluid::base::OFException if Unit or DataName doesn't exist
     * @param ClassName
     * @param ID
     * @param IDataName
     * @return
     */
    std::string& getInputData(std::string ClassName, int ID,
                              std::string IDataName);

    /**
     *
     * @param ClassName
     * @return An empty set if ClassName doesn't exist or has no InputData. Never throws.
     */
    std::set<std::string> getInputDataNames(std::string ClassName);

    /**
     * @throw openfluid::base::OFException if ClassName doesn't exist
     * or if IDataName already exists for class ClassName
     * @param ClassName
     * @param IDataName
     * @param DefaultValue
     */
    void addInputData(std::string ClassName, std::string IDataName,
                      std::string DefaultValue);

    /**
     * @throw openfluid::base::OFException if ClassName doesn't exist
     * or if IDataName doesn't exist for class ClassName
     * @param ClassName
     * @param IDataName
     */
    void deleteInputData(std::string ClassName, std::string IDataName);

    /**
     * Does nothing if NewDataName is the same as OldDataName
     * @throw openfluid::base::OFException if ClassName doesn't exist
     * or if OldDataName doesn't exist for the class ClassName
     * @param ClassName
     * @param OldIDataName
     * @param NewIDataName
     */
    void renameInputData(std::string ClassName, std::string OldIDataName,
                         std::string NewIDataName);

    void clearDomain();

};

// =====================================================================
// =====================================================================

}} // namespaces

#endif /* BUILDERDOMAIN_HPP_ */
