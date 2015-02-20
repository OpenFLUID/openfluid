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
  @file WareSignature.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/tools/MiscHelpers.hpp>


#ifndef __OPENFLUID_WARE_WARESIGNATURE_HPP__
#define __OPENFLUID_WARE_WARESIGNATURE_HPP__


/**
  Macro for declaration of the ware name
*/
#define DECLARE_NAME(name) Signature->Name = name;

/**
  Macro for declaration of the ware description
*/
#define DECLARE_DESCRIPTION(desc) Signature->Description = desc;

/**
  Macro for declaration of the ware author name and email
*/
#define DECLARE_AUTHOR(name,email) Signature->Authors.push_back(std::make_pair((name),(email)));

/**
  Macro for declaration of the ware version
*/
#define DECLARE_VERSION(version) Signature->Version = version;

/**
  Macro for declaration of the ware status
*/
#define DECLARE_STATUS(status) Signature->Status = status;

/**
  Macro for declaration of ABI version used to build the ware
*/
#define DECLARE_SDKVERSION Signature->setABIVersion(openfluid::config::FULL_VERSION);


// =====================================================================
// =====================================================================


namespace openfluid { namespace ware {


// =====================================================================
// =====================================================================


class OPENFLUID_API WareSignature
{
  public:

    typedef std::list<std::pair<std::string,std::string> > AuthorsList_t;

    WareID_t ID;

    WareName_t Name;

    std::string Description;

    /**
    Version number
    */
    WareVersion_t Version;

    /**
    Development status
    */
    WareStatus_t Status;

    /**
    ABI version number used to build the ware
    */
    WareVersion_t ABIVersion;

    /**
    Authors as a list of pairs [name,email]
    */
    AuthorsList_t Authors;


    WareSignature() :
      ID(""),Name(""),Description(""),Version(""),
      Status(EXPERIMENTAL),ABIVersion("")
      {
        Authors.clear();
      }


    virtual ~WareSignature()
      {}


    void setABIVersion(WareVersion_t Version)
    {
      ABIVersion = Version;
    }


    std::string getAuthorsAsString() const
    {
      std::string Str = "";

      for (openfluid::ware::WareSignature::AuthorsList_t::const_iterator it = Authors.begin(); it != Authors.end();++it)
      {
        if (it != Authors.begin())
          Str+= ", ";

        Str += openfluid::tools::replaceEmptyString((*it).first,("(unknown author)"));
        Str += " <";
        Str += openfluid::tools::replaceEmptyString((*it).second,("(unknown author email)"));
        Str += ">";
      }
      return Str;
    }


};



} } // openfluid::ware



#endif /* __OPENFLUID_WARE_WARESIGNATURE_HPP__ */
