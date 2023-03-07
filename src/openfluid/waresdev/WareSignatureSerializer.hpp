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
#include <openfluid/config.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace waresdev {

// TOIMPL move into WareSignatureSerializer class? or elsewhere?
inline openfluid::ware::WareType detectWareType(const std::string& Path)
{
  std::ifstream InFile(Path,std::ifstream::in);

  try 
  {
    auto Json = openfluid::thirdparty::json::parse(InFile);
  
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
  }
  catch (openfluid::thirdparty::json::exception& E)
  {
    return openfluid::ware::WareType::UNDEFINED;
  }  

  return openfluid::ware::WareType::UNDEFINED;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
class OPENFLUID_API WareSignatureSerializer
{
  protected:

    const std::string m_LinkUID;

    // TOIMPL review and refactor methods names, responsibilities and organization for better consistency

    static std::string getHead(const std::string CommentChar);

    static std::string getCPPHead(const std::string& WareIncludeStr, const std::string& WareTypeStr);

    static std::string getQuotedString(const std::string& Str);

    static std::string getCPPDateTimeString(const openfluid::core::DateTime& DT);

    static std::string getCPPVectorString(const std::vector<std::string>& StrVect,bool AutoQuote = false);

    static std::string getCPPEntry(const std::string& Member);

    static std::string getCPPAssignment(const std::string& Member, const std::string& Value, bool AutoQuote = false);

    static std::string getCPPMethod(const std::string& Member, const std::string& Method, 
                                    const std::vector<std::string>& Args, const std::string& Access = ".");

    std::string getCPPLinkUIDProc() const;

    static std::string getCPPTail();

    static std::string getCMakeHead();

    SignatureType fromJSONBase(const openfluid::thirdparty::json& Json) const;

    openfluid::thirdparty::json toJSONBase(const SignatureType& Sign) const;

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
std::string WareSignatureSerializer<SignatureType>::getHead(const std::string CommentChar)
{
  std::string Head =
  CommentChar + " ======\n" +
  CommentChar + " AUTOMATICALLY GENERATED FILE. DO NOT EDIT MANUALLY\n" +
  CommentChar + " ======\n\n"; 

  return Head;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
std::string WareSignatureSerializer<SignatureType>::getCPPHead(const std::string& WareIncludeStr,
                                                               const std::string& WareTypeStr)
{
  std::string Head = getHead("//");
  
  Head += 
    "#include <"+WareIncludeStr+">\n\n"
    "extern \"C\" {\n"
    "OPENFLUID_PLUGIN "+WareTypeStr+"* "+std::string(WARESIGNATURE_PROC_NAME)+"()\n"
    "{\n"
    +WareTypeStr+"* Signature = new "+WareTypeStr+"();\n\n"; 

  return Head;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
std::string WareSignatureSerializer<SignatureType>::getCPPLinkUIDProc() const
{
  std::string Str =
    "extern \"C\" {\n"
    "OPENFLUID_PLUGIN const std::string* "+std::string(WARELINKUID_PROC_NAME)+"()\n"
    "{\n"
    "return new std::string(\""+m_LinkUID+"\");\n"
    "}\n"
    "}"; 
  return Str;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
std::string WareSignatureSerializer<SignatureType>::getCPPTail()
{
  return "\nreturn Signature;\n}\n}\n";
}


// =====================================================================
// =====================================================================


template<class SignatureType>
SignatureType WareSignatureSerializer<SignatureType>::fromJSONBase(const openfluid::thirdparty::json& Json) const
{
  SignatureType Sign;

  Sign.ID = Json.value("id","");
  if (!openfluid::tools::isValidWareID(Sign.ID))
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
            unsigned int ID = I["id"].get<unsigned int>();

            openfluid::ware::WareIssue Issue;
            Issue.Title = I.value("title","");
            Issue.Description = I.value("description","");
            Issue.Tags = I.value("tags",std::vector<std::string>());
            Issue.Creator = I.value("creator","");
            Issue.CreatedAt = openfluid::core::DateTime::fromISOString(I.value("created_at",""));
            Issue.UpdatedAt = openfluid::core::DateTime::fromISOString(I.value("updated_at",""));
            Issue.IsOpen = (I.value("state","") != "closed");

            Sign.Issues.insert(Issue,ID);
          }
          catch (...)
          {
            // TOIMPL manage exceptions
          }
        }
      }
    }
  }

  return Sign;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
openfluid::thirdparty::json WareSignatureSerializer<SignatureType>::toJSONBase(const SignatureType& Sign) const
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
      auto IObj = openfluid::thirdparty::json::object();
      IObj["id"] = I.first;
      IObj["title"] = I.second.Title;
      IObj["description"] = I.second.Description;
      IObj["tags"] = I.second.Tags;
      IObj["creator"] = I.second.Creator;
      IObj["created_at"] = I.second.CreatedAt.getAsISOString();
      IObj["updated_at"] = I.second.UpdatedAt.getAsISOString();
      IObj["state"] = I.second.IsOpen ? "open" : "closed";
      IssArr.push_back(IObj);
    }
    Json["issues"] = IssArr;
  }

  return Json;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
std::string WareSignatureSerializer<SignatureType>::getQuotedString(const std::string& Str)
{
  return "\""+Str+"\"";
}


// =====================================================================
// =====================================================================


template<class SignatureType>
std::string WareSignatureSerializer<SignatureType>::getCPPEntry(const std::string& Member)
{
  return "Signature->"+Member;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
std::string WareSignatureSerializer<SignatureType>::getCPPAssignment(const std::string& Member,
                                                                     const std::string& Value,
                                                                     bool AutoQuote)
{
  std::string QuotedVal(Value);
  
  if (AutoQuote)
  {
    QuotedVal = getQuotedString(QuotedVal);
  }
  
  return getCPPEntry(Member)+" = "+QuotedVal+";\n";
}


// =====================================================================
// =====================================================================


template<class SignatureType>
std::string WareSignatureSerializer<SignatureType>::getCPPMethod(const std::string& Member, const std::string& Method, 
                                                                 const std::vector<std::string>& Args,
                                                                 const std::string& Access)
{
  std::string ArgsStr;
  bool FirstArg = true;
  
  for (const auto& A : Args)
  {
    if (FirstArg)
    {
      FirstArg = false;
    }
    else
    {
      ArgsStr += ",";
    }

    ArgsStr += A;
  }

  return getCPPEntry(Member)+Access+Method+"("+ArgsStr+");\n";
}


// =====================================================================
// =====================================================================


template<class SignatureType>
std::string WareSignatureSerializer<SignatureType>::getCPPVectorString(const std::vector<std::string>& StrVect,
                                                                       bool AutoQuote)
{
  std::string Str;

  Str += "{";
  bool isFirst = true;
  for (const auto& S : StrVect)
  {
    if (!isFirst)
    {
      Str += ",";
    }
    if (AutoQuote)
    {
      Str += getQuotedString(S);
    }
    else
    {
      Str += S;
    }
    isFirst = false;
  }
  Str += "}";

  return Str;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
std::string WareSignatureSerializer<SignatureType>::getCPPDateTimeString(const openfluid::core::DateTime& DT)
{
  std::string Str = "openfluid::core::DateTime(";

  Str += std::to_string(DT.getYear())+",";
  Str += std::to_string(DT.getMonth())+",";
  Str += std::to_string(DT.getDay())+",";
  Str += std::to_string(DT.getHour())+",";
  Str += std::to_string(DT.getMinute())+",";
  Str += std::to_string(DT.getSecond())+")";

  return Str;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
std::string WareSignatureSerializer<SignatureType>::toWareCPPBase(const SignatureType& Sign) const
{
  std::string CPP;

  CPP += getCPPAssignment("ID",Sign.ID,true);
  
  // build info
  CPP += getCPPAssignment("BuildInfo.SDKVersion",openfluid::config::VERSION_FULL,true);
  CPP += getCPPAssignment("BuildInfo.BuildType","(WAREBUILD_BUILD_TYPE)");
  CPP += getCPPAssignment("BuildInfo.CompilerID","(WAREBUILD_COMPILER_ID)");
  CPP += getCPPAssignment("BuildInfo.CompilerVersion","(WAREBUILD_COMPILER_VERSION)");
  CPP += getCPPAssignment("BuildInfo.CompilationFlags","(WAREBUILD_COMPILATION_FLAGS)");

  CPP += getCPPAssignment("Name",openfluid::tools::escapeString(Sign.Name),true);
  CPP += getCPPAssignment("Description",openfluid::tools::escapeString(Sign.Description),true);
  CPP += getCPPAssignment("Version",Sign.Version,true);
  CPP += getCPPAssignment("Status","openfluid::ware::"+openfluid::tools::toUpperCase(Sign.getStatusAsString()));

  // authors
  std::vector<std::string> AuthorsStrVect;
  for (const auto& A : Sign.Authors)
  {
    AuthorsStrVect.push_back(getCPPVectorString({A.first,A.second},true));
  }
  CPP += getCPPAssignment("Authors",getCPPVectorString(AuthorsStrVect));
  
  // contact
  std::vector<std::string> ContactsStrVect;
  for (const auto& C : Sign.Contacts)
  {
    ContactsStrVect.push_back(getCPPVectorString({C.first,C.second},true));
  }
  CPP += getCPPAssignment("Contacts",getCPPVectorString(ContactsStrVect));
  
  // license
  CPP += getCPPAssignment("License",Sign.License,true);
  
  // tags
  CPP += getCPPAssignment("Tags",getCPPVectorString(Sign.Tags,true));

  // links
  std::vector<std::string> LinksStrVect;
  for (const auto& L : Sign.Links)
  {
    LinksStrVect.push_back(getCPPVectorString({L.first,L.second},true));
  }
  CPP += getCPPAssignment("Links",getCPPVectorString(LinksStrVect));

  // issues
  for (const auto& I : Sign.Issues())
  {
    std::vector<std::string> IssData;

    IssData.push_back(getQuotedString(openfluid::tools::escapeString(I.second.Title)));
    IssData.push_back(getQuotedString(openfluid::tools::escapeString(I.second.Description)));
    IssData.push_back(getCPPVectorString(I.second.Tags,true));
    IssData.push_back(getQuotedString(I.second.Creator));
    IssData.push_back(getCPPDateTimeString(I.second.CreatedAt));
    IssData.push_back(getCPPDateTimeString(I.second.UpdatedAt));
    if (I.second.IsOpen)
    {
      IssData.push_back("true");
    }
    else
    {
      IssData.push_back("false");
    }

    CPP += getCPPMethod("Issues","insert",{getCPPVectorString(IssData),std::to_string(I.first)});
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
  OutFile << getCPPLinkUIDProc();
  OutFile << "\n\n";
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareSignatureSerializer<SignatureType>::writeToParamsUICPPFile(const SignatureType& Sign,
                                                                    const std::string& FilePath) const
{
  std::ofstream OutFile(FilePath,std::ofstream::out);

  OutFile << getCPPHead("openfluid/builderext/BuilderExtensionSignature.hpp",
                        "openfluid::builderext::BuilderExtensionSignature");

  OutFile << getCPPAssignment("ID",Sign.ID+openfluid::config::WARESDEV_PARAMSUI_IDSUFFIX,true);
  OutFile << getCPPAssignment("Role","openfluid::builderext::ExtensionRole::PARAMETERIZATION");
  OutFile << getCPPAssignment("BuildInfo.SDKVersion",openfluid::config::VERSION_FULL,true);
  OutFile << getCPPAssignment("BuildInfo.BuildType","(WAREBUILD_BUILD_TYPE)");
  OutFile << getCPPAssignment("BuildInfo.CompilerID","(WAREBUILD_COMPILER_ID)");
  OutFile << getCPPAssignment("BuildInfo.CompilerVersion","(WAREBUILD_COMPILER_VERSION)");
  OutFile << getCPPAssignment("BuildInfo.CompilationFlags","(WAREBUILD_COMPILATION_FLAGS)");

  OutFile <<  getCPPTail();

  OutFile << "\n\n";
  OutFile << getCPPLinkUIDProc();
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

  OutFile << getHead("#");
  OutFile << "SET(WARE_ID \""+Sign.ID+openfluid::config::WARESDEV_PARAMSUI_IDSUFFIX+"\")\n";
  OutFile << "SET(WARE_LINKUID \""+m_LinkUID+"\")\n";
  OutFile << "SET(WARE_TYPE \"builderext\")\n";
  OutFile << "SET(WARE_IS_BUILDEREXT TRUE)\n";
  OutFile << "SET(WARE_IS_PARAMSUI TRUE)\n";
  OutFile << "\n\n";
}


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESIGNATURESERIALIZER_HPP__ */
