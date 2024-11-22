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
  @file WareSignatureSerializer.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#ifndef __OPENFLUID_WARESDEV_WARESIGNATURESERIALIZER_HPP__
#define __OPENFLUID_WARESDEV_WARESIGNATURESERIALIZER_HPP__


#include <string>
#include <vector>
#include <fstream>
#include <utility>

#include <openfluid/thirdparty/JSON.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/utils/InternalLogger.hpp>
#include <openfluid/waresdev/WareCppWriterHelpers.hpp>
#include <openfluid/config.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace waresdev {


inline openfluid::ware::WareType detectWareTypeFromJson(const openfluid::thirdparty::json& Json)
{
  if (Json.contains("simulator"))
  {
    return openfluid::ware::WareType::SIMULATOR; 
  }
  else if (Json.contains("observer"))
  {
    return openfluid::ware::WareType::OBSERVER; 
  }
  else if (Json.contains("builderext"))
  {
    return openfluid::ware::WareType::BUILDEREXT; 
  }

  return openfluid::ware::WareType::UNDEFINED;
}


// =====================================================================
// =====================================================================


inline openfluid::ware::WareType detectWareType(const std::string& Path)
{
  std::ifstream InFile(Path,std::ifstream::in);

  try 
  {
    auto Json = openfluid::thirdparty::json::parse(InFile);
  
    return detectWareTypeFromJson(Json);
  }
  catch (openfluid::thirdparty::json::exception& E)
  {
    openfluid::utils::log::error("Waresdev", E.what());
    return openfluid::ware::WareType::UNDEFINED;
  }

  return openfluid::ware::WareType::UNDEFINED;
}


// =====================================================================
// =====================================================================


inline std::pair<openfluid::ware::WareType,openfluid::ware::WareID_t> detectWareTypeAndID(const std::string& Path)
{
  openfluid::ware::WareType Type = openfluid::ware::WareType::UNDEFINED;
  openfluid::ware::WareID_t ID;

  std::ifstream InFile(Path,std::ifstream::in);

  try 
  {
    auto Json = openfluid::thirdparty::json::parse(InFile);
  
    Type = detectWareTypeFromJson(Json);

    const std::string TmpID = Json.value("id","");
    if (openfluid::tools::isValidWareID(TmpID))
    {
      ID = TmpID;
    }
  }
  catch (openfluid::thirdparty::json::exception& E)
  {
    openfluid::utils::log::error("Waresdev", E.what());
    Type = openfluid::ware::WareType::UNDEFINED;
  }  

  return {Type,ID};
}


// =====================================================================
// =====================================================================


template<class SignatureType>
class OPENFLUID_API WareSignatureSerializer
{
  protected:

    const std::string m_LinkUID;

    // TODO review and refactor methods names, responsibilities and organization for better consistency


    std::string toWareCPPBase(const SignatureType& Sign) const;

    std::string toWareCMakeBase(const SignatureType& Sign) const;

    WareSignatureSerializer():
      m_LinkUID(openfluid::tools::generatePseudoUniqueIdentifier(24))
    { }


  public:

    ~WareSignatureSerializer()
    { }

    virtual SignatureType fromJSON(const openfluid::thirdparty::json& Json) const = 0;

    virtual openfluid::thirdparty::json toJSON(const SignatureType& Sign) const = 0;

    virtual std::string toWareCPP(const SignatureType& Sign) const = 0;

    virtual std::string toWareCMake(const SignatureType& Sign) const = 0;

    static SignatureType fromJSONBase(const openfluid::thirdparty::json& Json);

    static openfluid::thirdparty::json toJSONBase(const SignatureType& Sign);

    SignatureType readFromJSONFile(const std::string& FilePath) const;

    void writeToJSONFile(const SignatureType& Sign, const std::string& FilePath) const;

    void writeToWareCPPFile(const SignatureType& Sign, const std::string& FilePath) const;

    void writeToParamsUICPPFile(const SignatureType& Sign, const std::string& FilePath) const;

    void writeToWareCMakeFile(const SignatureType& Sign, const std::string& FilePath) const;

    void writeToParamsUICMakeFile(const SignatureType& Sign, const std::string& FilePath) const;

    virtual void writeToBuildFiles(const SignatureType& Sign, const std::string& Path) const = 0;
};


// =====================================================================
// =====================================================================


template<class SignatureType>
SignatureType WareSignatureSerializer<SignatureType>::fromJSONBase(const openfluid::thirdparty::json& Json)
{
  SignatureType Sign;

  Sign.ID = Json.value("id","");
  if (!openfluid::tools::isValidWareID(Sign.ID, true))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Missing or invalid ware ID");
  }


  Sign.Name = Json.value("name","");
  Sign.Description = Json.value("description","");
  Sign.Version = Json.value("version","");
  Sign.setStatusFromString(openfluid::tools::trim(Json.value("status","")));

  // authors
  {
    openfluid::thirdparty::json PeopleArr = Json.value("authors",openfluid::thirdparty::json::array());
    if (PeopleArr.is_array())
    {
      for (const auto& P : PeopleArr)
      {
        if (P.is_object() && P.contains("name"))
        {
          Sign.Authors.push_back({P["name"],P.value("email","")});
        }
      }
    }
  }

  // contacts
  {
    openfluid::thirdparty::json PeopleArr = Json.value("contacts",openfluid::thirdparty::json::array());
    if (PeopleArr.is_array())
    {
      for (const auto& P : PeopleArr)
      {
        if (P.is_object() && P.contains("name"))
        {
          Sign.Contacts.push_back({P["name"],P.value("email","")});
        }
      }
    }
  }

  //license
  Sign.License = openfluid::tools::trim(Json.value("license",""));

  // links
  {
    openfluid::thirdparty::json::array_t LinksArr = Json.value("links",openfluid::thirdparty::json::array());
    for (const auto& L : LinksArr)
    {
      if (L.is_object() && L.contains("url"))
      {
        Sign.Links.push_back({L.value("label",""),L["url"]});
      }
    }
  }
  
  // tags
  {
    openfluid::thirdparty::json TagsArr = Json.value("tags",openfluid::thirdparty::json::array());
    if (TagsArr.is_array())
    {
      for (const auto& T : TagsArr)
      {
        if (T.is_string())
        {
          Sign.Tags.push_back(T.get<std::string>());
        }
      }
    }
  }

  // issues
  {
    openfluid::thirdparty::json IssuesArr = Json.value("issues",openfluid::thirdparty::json::array());
    if (IssuesArr.is_array())
    {
      for (const auto& I : IssuesArr)
      {
        if (I.is_object() && I.contains("id"))
        {
          try
          {
            openfluid::ware::WareIssue Issue = openfluid::ware::WareIssue::fromJSON(I);
            Sign.Issues.insert(Issue,Issue.ID);
          }
          catch (std::exception& E)
          {
            throw E;
          }
        }
      }
    }
  }

  // dependencies
  {
    openfluid::thirdparty::json DependenciesMap = Json.value("dependencies",openfluid::thirdparty::json::object());
    if (DependenciesMap.is_object())
    {
      for (const auto& D : DependenciesMap.items())
      {
        Sign.Dependencies.insert({D.key(), D.value()});
      }
    }
  }

  return Sign;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
openfluid::thirdparty::json WareSignatureSerializer<SignatureType>::toJSONBase(const SignatureType& Sign)
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  Json["id"] = Sign.ID;
  Json["name"] = Sign.Name;
  Json["description"] = Sign.Description;
  Json["version"] = Sign.Version;
  Json["status"] = Sign.getStatusAsString();

  // authors
  {
    auto PeopleArr = openfluid::thirdparty::json::array();
    for (const auto& P : Sign.Authors)
    {
      auto PersonObj = openfluid::thirdparty::json::object();
      PersonObj["name"] = P.first;
      PersonObj["email"] = P.second;
      PeopleArr.push_back(PersonObj);
    }
    Json["authors"] = PeopleArr;
  }

  // contacts
  {
    auto PeopleArr = openfluid::thirdparty::json::array();
    for (const auto& P : Sign.Contacts)
    {
      auto PersonObj = openfluid::thirdparty::json::object();
      PersonObj["name"] = P.first;
      PersonObj["email"] = P.second;
      PeopleArr.push_back(PersonObj);
    }
    Json["contacts"] = PeopleArr;
  }

  // license
  Json["license"] = Sign.License;

  // tags
  Json["tags"] = Sign.Tags;

  // links
  {
    auto LnkArr = openfluid::thirdparty::json::array();
    for (const auto& L : Sign.Links)
    {
      auto LnkObj = openfluid::thirdparty::json::object();
      LnkObj["label"] = L.first;
      LnkObj["url"] = L.second;
      LnkArr.push_back(LnkObj);
    }
    Json["links"] = LnkArr;
  }

  // issues
  {
    auto IssArr = openfluid::thirdparty::json::array();
    for (const auto& I : Sign.Issues.getAll())
    {
      auto IObj = I.second.toJSON(I.first);
      IssArr.push_back(IObj);
    }
    Json["issues"] = IssArr;
  }

  // dependencies
  auto Deps = openfluid::thirdparty::json::object();
  for (const auto& D : Sign.Dependencies)
  {
    Deps[D.first] = D.second;
  }
  Json["dependencies"] = Deps;


  return Json;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
std::string WareSignatureSerializer<SignatureType>::toWareCPPBase(const SignatureType& Sign) const
{
  std::string CPP;

  CPP += CppWriter::getCPPAssignment("ID",Sign.ID,true);
  
  // build info
  CPP += CppWriter::getCPPAssignment("BuildInfo.SDKVersion",openfluid::config::VERSION_FULL,true);
  CPP += CppWriter::getCPPAssignment("BuildInfo.BuildType","(WAREBUILD_BUILD_TYPE)");  
  CPP += CppWriter::getCPPAssignment("BuildInfo.CompilerID","(WAREBUILD_COMPILER_ID)");
  CPP += CppWriter::getCPPAssignment("BuildInfo.CompilerVersion","(WAREBUILD_COMPILER_VERSION)");
  CPP += CppWriter::getCPPAssignment("BuildInfo.CompilationFlags","(WAREBUILD_COMPILATION_FLAGS)");

  CPP += CppWriter::getCPPAssignment("Name",openfluid::tools::escapeString(Sign.Name),true);
  CPP += CppWriter::getCPPAssignment("Description",openfluid::tools::escapeString(Sign.Description),true);
  CPP += CppWriter::getCPPAssignment("Version",Sign.Version,true);
  CPP += CppWriter::getCPPAssignment("Status",
                                     "openfluid::ware::"+openfluid::tools::toUpperCase(Sign.getStatusAsString()));

  // authors
  std::vector<std::string> AuthorsStrVect;
  for (const auto& A : Sign.Authors)
  {
    AuthorsStrVect.push_back(CppWriter::getCPPVectorString({A.first,A.second},true));
  }
  CPP += CppWriter::getCPPAssignment("Authors",CppWriter::getCPPVectorString(AuthorsStrVect));
  
  // contact
  std::vector<std::string> ContactsStrVect;
  for (const auto& C : Sign.Contacts)
  {
    ContactsStrVect.push_back(CppWriter::getCPPVectorString({C.first,C.second},true));
  }
  CPP += CppWriter::getCPPAssignment("Contacts",CppWriter::getCPPVectorString(ContactsStrVect));
  
  // license
  CPP += CppWriter::getCPPAssignment("License",Sign.License,true);
  
  // tags
  CPP += CppWriter::getCPPAssignment("Tags",CppWriter::getCPPVectorString(Sign.Tags,true));

  // links
  std::vector<std::string> LinksStrVect;
  for (const auto& L : Sign.Links)
  {
    LinksStrVect.push_back(CppWriter::getCPPVectorString({L.first,L.second},true));
  }
  CPP += CppWriter::getCPPAssignment("Links",CppWriter::getCPPVectorString(LinksStrVect));

  // issues
  for (const auto& I : Sign.Issues())
  {
    std::vector<std::string> IssData;
    IssData.push_back(std::to_string(I.first));
    IssData.push_back(CppWriter::getQuotedString(openfluid::tools::escapeString(I.second.Title)));
    IssData.push_back(CppWriter::getQuotedString(openfluid::tools::escapeString(I.second.Description)));
    IssData.push_back(CppWriter::getCPPVectorString(I.second.Tags,true));
    IssData.push_back(CppWriter::getQuotedString(I.second.Creator));
    IssData.push_back(CppWriter::getCPPDateTimeString(I.second.CreatedAt));
    IssData.push_back(CppWriter::getCPPDateTimeString(I.second.UpdatedAt));
    if (I.second.IsOpen)
    {
      IssData.push_back("true");
    }
    else
    {
      IssData.push_back("false");
    }

    CPP += CppWriter::getCPPMethod("Issues","insert",{CppWriter::getCPPVectorString(IssData),std::to_string(I.first)});
  } 

  return CPP;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
std::string WareSignatureSerializer<SignatureType>::toWareCMakeBase(const SignatureType& Sign) const
{
  std::string CMake;

  CMake += "SET(WARE_ID \""+Sign.ID+"\")\n";
  CMake += "SET(WARE_LINKUID \""+m_LinkUID+"\")\n";

  return CMake;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
SignatureType WareSignatureSerializer<SignatureType>::readFromJSONFile(const std::string& Path) const
{
  std::ifstream InFile(Path,std::ifstream::in);

  try 
  {
    auto FullJson = openfluid::thirdparty::json::parse(InFile);
    auto Sign = fromJSON(FullJson);
    return Sign;
  }
  catch (openfluid::thirdparty::json::exception& E)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Error parsing file "+Path+
                                              " ("+std::string(E.what())+")");
  }
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareSignatureSerializer<SignatureType>::writeToJSONFile(const SignatureType& Sign,
                                                             const std::string& FilePath) const
{
  auto Json = toJSON(Sign);

  std::ofstream OutFile(FilePath,std::ofstream::out);
  OutFile << Json.dump(2);
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareSignatureSerializer<SignatureType>::writeToWareCPPFile(const SignatureType& Sign,
                                                                const std::string& FilePath) const
{
  std::ofstream OutFile(FilePath,std::ofstream::out);

  OutFile << toWareCPP(Sign);
  OutFile << "\n\n";
  OutFile << CppWriter::getCPPLinkUIDProc(m_LinkUID);
  OutFile << "\n\n";
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareSignatureSerializer<SignatureType>::writeToParamsUICPPFile(const SignatureType& Sign,
                                                                    const std::string& FilePath) const
{
  std::ofstream OutFile(FilePath,std::ofstream::out);

  OutFile << CppWriter::getCPPHead("openfluid/builderext/BuilderExtensionSignature.hpp",
                        "openfluid::builderext::BuilderExtensionSignature");

  OutFile << CppWriter::getCPPAssignment("ID",Sign.ID+openfluid::config::WARESDEV_PARAMSUI_IDSUFFIX,true);
  OutFile << CppWriter::getCPPAssignment("Role","openfluid::builderext::ExtensionRole::PARAMETERIZATION");
  OutFile << CppWriter::getCPPAssignment("BuildInfo.SDKVersion",openfluid::config::VERSION_FULL,true);
  OutFile << CppWriter::getCPPAssignment("BuildInfo.BuildType","(WAREBUILD_BUILD_TYPE)");
  OutFile << CppWriter::getCPPAssignment("BuildInfo.CompilerID","(WAREBUILD_COMPILER_ID)");
  OutFile << CppWriter::getCPPAssignment("BuildInfo.CompilerVersion","(WAREBUILD_COMPILER_VERSION)");
  OutFile << CppWriter::getCPPAssignment("BuildInfo.CompilationFlags","(WAREBUILD_COMPILATION_FLAGS)");

  OutFile <<  CppWriter::getCPPTail();

  OutFile << "\n\n";
  OutFile << CppWriter::getCPPLinkUIDProc(m_LinkUID);
  OutFile << "\n\n";
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareSignatureSerializer<SignatureType>::writeToWareCMakeFile(const SignatureType& Sign, 
                                                                  const std::string& FilePath) const
{
  std::ofstream OutFile(FilePath,std::ofstream::out);

  OutFile << toWareCMake(Sign);
  OutFile << "\n\n";
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareSignatureSerializer<SignatureType>::writeToParamsUICMakeFile(const SignatureType& Sign, 
                                                                      const std::string& FilePath) const
{
  std::ofstream OutFile(FilePath,std::ofstream::out);

  OutFile << CppWriter::getHead("#");
  OutFile << "SET(WARE_ID \""+Sign.ID+openfluid::config::WARESDEV_PARAMSUI_IDSUFFIX+"\")\n";
  OutFile << "SET(WARE_LINKUID \""+m_LinkUID+"\")\n";
  OutFile << "SET(WARE_TYPE \"builderext\")\n";
  OutFile << "SET(WARE_IS_BUILDEREXT TRUE)\n";
  OutFile << "SET(WARE_IS_PARAMSUI TRUE)\n";
  OutFile << "\n\n";
}


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESIGNATURESERIALIZER_HPP__ */
