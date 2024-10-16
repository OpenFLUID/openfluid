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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/ware/WareIssues.hpp>
#include <openfluid/tools/MiscHelpers.hpp>


#ifndef __OPENFLUID_WARE_WARESIGNATURE_HPP__
#define __OPENFLUID_WARE_WARESIGNATURE_HPP__


namespace openfluid { namespace ware {


class OPENFLUID_API WareSignature
{
  public:

    virtual openfluid::ware::WareType getType() const = 0;

    using Person_t = std::pair<std::string,std::string>;

    using Link_t = std::pair<std::string,std::string>;

    using PeopleList_t = std::vector<Person_t>;

    using LinksList_t = std::vector<Link_t>;


    /**
      Information about build context
    */    
    SignatureBuildInfo BuildInfo;

    WareID_t ID;

    WareName_t Name;

    std::string Description;

    /**
      Version string
    */
    WareVersion_t Version;

    /**
      Development status
    */
    WareStatus_t Status;

    /**
      Authors as a list of pairs [name,email]
    */
    PeopleList_t Authors;

    /**
      Contacts as a list of pairs [name,email]
    */
    PeopleList_t Contacts; 

    /**
      SPDX code for software license
    */
    std::string License;

    /**
      List of tags
    */
    std::vector<std::string> Tags;

    /**
      List of links
    */
    LinksList_t Links;


    /**
      Issues
    */ 
    WareIssues Issues;
    

    WareSignature()
    {
      clear();
    }

    /*
    // TODO make this class non-copyable
    
    WareSignature(const WareSignature&) = delete;
    
    WareSignature(WareSignature&&) = default;

    WareSignature& operator=(const WareSignature&) = delete;

    WareSignature& operator=(WareSignature&&) = default;
    */ 

    virtual ~WareSignature() = default;


    // =====================================================================
    // =====================================================================


    void clear()
    {
      BuildInfo = SignatureBuildInfo();
      ID.clear();
      Name.clear();
      Description.clear();
      Version.clear();
      Status = WareStatus_t::EXPERIMENTAL;
      Authors.clear();
      License.clear();
      Contacts.clear();
      Tags.clear();
      Links.clear();
      Issues.clear();
    }


    // =====================================================================
    // =====================================================================


    std::string getStatusAsString() const
    {
      if (Status == WareStatus_t::BETA)
      {
        return "beta";
      }
      else if (Status == WareStatus_t::STABLE)
      {
        return "stable";
      }

      return "experimental";
    }


    // =====================================================================
    // =====================================================================


    void setStatusFromString(const std::string& StatusStr)
    {
      Status = WareStatus_t::EXPERIMENTAL;

      if (StatusStr == "beta")
      {
        Status = WareStatus_t::BETA;
      }
      else if (StatusStr == "stable")
      {
        Status = WareStatus_t::STABLE;
      }
    }

};


// =====================================================================
// =====================================================================


/**
Class for storage of the definition of data handled by the simulator.
*/
class OPENFLUID_API SignatureDataItem
{
  public:

    std::string Name;
    std::string Description;
    std::string SIUnit;
    openfluid::core::Value::Type DataType = openfluid::core::Value::NONE;

    SignatureDataItem()
    { }

    SignatureDataItem(const std::string& N, const std::string& D, const std::string& SI);

    SignatureDataItem(const std::string& N, const std::string& D, const std::string& SI,
                      openfluid::core::Value::Type T);
};


// =====================================================================
// =====================================================================


/**
Class for storage of the definition of data handled by the simulator.
*/
class OPENFLUID_API SignatureSpatialDataItem : public SignatureDataItem
{
  public:

    openfluid::core::UnitsClass_t UnitsClass;

    SignatureSpatialDataItem() : SignatureDataItem()
    { }

    SignatureSpatialDataItem(const std::string& N, const openfluid::core::UnitsClass_t& U,
                             const std::string& D, const std::string& SI) :
      SignatureDataItem(N,D,SI),UnitsClass(U)
    { }

    SignatureSpatialDataItem(const std::string& N, const openfluid::core::UnitsClass_t& U,
                             const std::string& D, const std::string& SI, openfluid::core::Value::Type T) :
      SignatureDataItem(N,D,SI,T),UnitsClass(U)
    { }
};


// =====================================================================
// =====================================================================


/**
  Class for storage of the definition of the data handled by the ware. This is part of the signature.
*/
class OPENFLUID_API SignatureHandledData
{
  public:

    std::vector<SignatureDataItem> UsedParams;

    std::vector<SignatureDataItem> RequiredParams;

    std::vector<SignatureSpatialDataItem> RequiredVars;

    std::vector<SignatureSpatialDataItem> UsedVars;

    std::vector<SignatureSpatialDataItem> RequiredAttribute; // TOIMPL add plural

    std::vector<SignatureSpatialDataItem> UsedAttribute; // TOIMPL add plural

    std::vector<std::string> RequiredExtraFiles; // TODO add description associated with each file?

    std::vector<std::string> UsedExtraFiles; // TODO add description associated with each file?
    

    SignatureHandledData()
    {
      clear();
    }

    ~SignatureHandledData() {}


    void clear()
    {
      UsedParams.clear();
      RequiredParams.clear();
      RequiredVars.clear();
      UsedVars.clear();
      RequiredAttribute.clear();
      UsedAttribute.clear();
      RequiredExtraFiles.clear();
      UsedExtraFiles.clear();
    }

};


class OPENFLUID_API DataWareSignature : public WareSignature
{
  public:
    /**
      Handled data
    */
    SignatureHandledData HandledData;
    
    void clear()
    {
      WareSignature::clear();
      HandledData.clear();
    }

};


} } // openfluid::ware


#endif /* __OPENFLUID_WARE_WARESIGNATURE_HPP__ */
