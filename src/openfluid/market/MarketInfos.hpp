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
  @file MarketInfos.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_MARKET_MARKETINFOS_HPP__
#define __OPENFLUID_MARKET_MARKETINFOS_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>


namespace openfluid { namespace market {

// =====================================================================
// =====================================================================


class OPENFLUID_API MarketInfo
{
  public:

    std::string Name;

    std::string Description;

    std::string Contact;

    MarketInfo()
    {
      Name.clear();
      Description.clear();
      Contact.clear();
    }

    ~MarketInfo() { }


    void clear()
    {
      Name.clear();
      Description.clear();
      Contact.clear();
    }


};


// =====================================================================
// =====================================================================


class OPENFLUID_API PackageInfo
{
  public:

    // Package type
    enum PackageType { SIM, OBS, BUILD, DATA};

    typedef std::map<PackageType,std::list<openfluid::ware::WareID_t> > Dependencies_t;

    std::string URL;

    std::string License;

    // List of dependencies
    Dependencies_t Dependencies;

    std::string BuildOptions;

    PackageInfo()
    {
      URL.clear();
      License.clear();
      Dependencies.clear();
      BuildOptions.clear();
    }

    ~PackageInfo() {  }

};


// =====================================================================
// =====================================================================


class OPENFLUID_API MetaPackageInfo
{
  public:

    // Package format
    enum SelectionType { NONE, BIN, SRC, FLUIDX};

    openfluid::ware::WareID_t ID;

    // List of PackageInfo for each format
    std::map<SelectionType,PackageInfo> AvailablePackages;

    // Selected format
    SelectionType Selected;

    std::string Name;

    std::string Description;

    std::string Authors;

    std::string Version;


    MetaPackageInfo()
    {
      ID.clear();
      Selected = NONE;
      AvailablePackages.clear();
      Name.clear();
      Authors.clear();
      Description.clear();
      Version.clear();
    }

    ~MetaPackageInfo()
    {
    }

};


// =====================================================================
// =====================================================================


typedef std::map<openfluid::ware::WareID_t,MetaPackageInfo> MetaPackagesCatalog_t;


} } // namespaces


#endif /* __OPENFLUID_MARKET_MARKETINFOS_HPP__ */
