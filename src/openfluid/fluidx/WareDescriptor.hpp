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
 \file WareDescriptor.hpp
 \brief Header of ...

 \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#ifndef __WAREDESCRIPTOR_HPP__
#define __WAREDESCRIPTOR_HPP__

#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/dllexport.hpp>

namespace openfluid {
namespace fluidx {

class DLLEXPORT WareDescriptor
{
  public:

    enum WareType
    {
      NoWareType, PluggedSimulator, Generator, PluggedObserver
    };

  protected:

    WareType m_WareType;

    openfluid::ware::WareParams_t m_Params;  // Simulator parameters set

    bool m_Enabled;

  public:

    WareDescriptor();

    virtual ~WareDescriptor();

    void setParameter(const openfluid::ware::WareParamKey_t& Key,
                      const openfluid::ware::WareParamValue_t& Value);

    void setParameters(const openfluid::ware::WareParams_t& Params);

    openfluid::ware::WareParams_t getParameters();

    bool isType(WareType MIType) const;

    WareType getType() const;

    void eraseParameter(const openfluid::ware::WareParamKey_t& Key);

    void clearParameters();

    bool hasParameter(std::string ParameterKey);

    void setEnabled(bool Enabled);

    bool isEnabled() const;

};

}
}  // namespaces

#endif /* __WAREDESCRIPTOR_HPP__ */
