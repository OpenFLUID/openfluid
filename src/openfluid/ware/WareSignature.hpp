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
  \file WareSignature.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/dllexport.hpp>


#ifndef __WARESIGNATURE_HPP__
#define __WARESIGNATURE_HPP__


/**
  Macro for declaration of the ware ID
*/
#define DECLARE_SIGNATURE_ID(id) Signature->ID = id;

/**
  Macro for declaration of the ware name
*/
#define DECLARE_SIGNATURE_NAME(name) Signature->Name = name;

/**
  Macro for declaration of the ware description
*/
#define DECLARE_SIGNATURE_DESCRIPTION(desc) Signature->Description = desc;

/**
  Macro for declaration of the ware author name
*/
#define DECLARE_SIGNATURE_AUTHORNAME(name) Signature->Author = name;

/**
  Macro for declaration of the ware author email
*/
#define DECLARE_SIGNATURE_AUTHOREMAIL(email) Signature->AuthorEmail = email;

/**
  Macro for declaration of the ware version
*/
#define DECLARE_SIGNATURE_VERSION(version) Signature->Version = version;

/**
  Macro for declaration of the ware status
*/
#define DECLARE_SIGNATURE_STATUS(status) Signature->Status = status;

/**
  Macro for declaration of ABI version used to build the ware
*/
#define DECLARE_SIGNATURE_SDKVERSION Signature->setABIVersion(openfluid::config::FULL_VERSION);


// =====================================================================
// =====================================================================


namespace openfluid { namespace ware {


typedef std::string WareID_t;

typedef std::string WareName_t;

typedef std::string WareVersion_t;


/**
  Ware status
*/
enum WareStatus_t
{
  /**
    Experimental status, for testing only
  */
  EXPERIMENTAL,

  /**
    Beta status, on the road to a stable status
  */
  BETA,

  /**
    Stable status
  */
  STABLE
};


// =====================================================================
// =====================================================================


class DLLEXPORT WareSignature
{
  public:

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
    Author name
    */
    std::string Author;

    /**
    Author email
    */
    std::string AuthorEmail;


    WareSignature() :
      ID(""),Name(""),Description(""),Version(""),
      Status(EXPERIMENTAL),ABIVersion(""),Author(""),AuthorEmail("")
      {}


    virtual ~WareSignature()
      {}


    void setABIVersion(WareVersion_t Version)
    {
      ABIVersion = Version;
    }

};



} } // openfluid::ware



#endif /* __WARESIGNATURE_HPP__ */
