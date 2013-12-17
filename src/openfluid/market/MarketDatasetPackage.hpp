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


/*
 * MarketDatasetPackage.hpp
 *
 *  Created on: 4 mars 2013
 *      Author: Manuel CHATAIGNER
 */

#ifndef __MARKETDATASETPACKAGE_HPP__
#define __MARKETDATASETPACKAGE_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/market/MarketPackage.hpp>


namespace openfluid { namespace market {


class DLLEXPORT MarketDatasetPackage : public MarketPackage
{
  public:

    MarketDatasetPackage(const openfluid::ware::WareID_t& ID, const std::string& PackageURL);

    std::string getInstallPath() const;

    PackageInfo::PackageType getPackageType() const;

    void process();

    MetaPackageInfo::SelectionType getFormat() const { return MetaPackageInfo::FLUIDX; };

};


} } // namespaces


#endif /* __MARKETDATASETPACKAGE_HPP__ */
