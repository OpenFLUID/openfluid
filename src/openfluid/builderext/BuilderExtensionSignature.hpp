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
  @file BuilderExtensionSignature.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_BUILDEREXT_BUILDEREXTENSIONSIGNATURE_HPP__
#define __OPENFLUID_BUILDEREXT_BUILDEREXTENSIONSIGNATURE_HPP__


#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/config.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace builderext {


enum class ExtensionCategory { SPATIAL, MODEL, RESULTS, OTHER };

enum class ExtensionRole { UNKNOWN, FEATURE, PARAMETERIZATION};

enum class ExtensionMode { UNKNOWN, MODAL, MODELESS, WORKSPACE};


class OPENFLUID_API BuilderExtensionSignature : public openfluid::ware::WareSignature
{
  public:

    ExtensionCategory Category;

    ExtensionMode Mode;

    ExtensionRole Role;

    std::string MenuText;

    std::map<std::string,std::string> ConfigParameters;  // NOTE currently not used 


    BuilderExtensionSignature():
      Category(ExtensionCategory::OTHER), Mode(ExtensionMode::UNKNOWN), Role(ExtensionRole::UNKNOWN)
    {  }

    virtual ~BuilderExtensionSignature() = default;

    openfluid::ware::WareType getType() const
    {
      return openfluid::ware::WareType::BUILDEREXT;
    }

    std::string getModeAsString() const
    {
      if (Mode == ExtensionMode::MODAL)
      {
        return "modal";
      }
      else if (Mode == ExtensionMode::MODELESS)
      {
        return "modeless";
      }
      else if (Mode == ExtensionMode::WORKSPACE)
      {
        return "workspace";
      }
      return "";
    }

    bool setModeFromString(const std::string& M)
    {
      if (M == "modal")
      {
        Mode = ExtensionMode::MODAL;
        return true;
      }
      else if (M == "modeless")
      {
        Mode = ExtensionMode::MODELESS;
        return true;
      }
      else if (M == "workspace")
      {
        Mode = ExtensionMode::WORKSPACE;
        return true;
      }
      return false;
    }

    std::string getCategoryAsString() const
    {
      if (Category == ExtensionCategory::MODEL)
      {
        return "model";
      }
      else if (Category == ExtensionCategory::SPATIAL)
      {
        return "spatial";
      }
      else if (Category == ExtensionCategory::RESULTS)
      {
        return "results";
      }
      return "other";
    }

    bool setCategoryFromString(const std::string& C)
    {
      if (C == "model")
      {
        Category = ExtensionCategory::MODEL;
        return true;
      }
      else if (C == "spatial")
      {
        Category = ExtensionCategory::SPATIAL;
        return true;
      }
      else if (C == "results")
      {
        Category = ExtensionCategory::RESULTS;
        return true;
      }
      else if (C == "other")
      {
        Category = ExtensionCategory::OTHER;
        return true;
      }
      return false;
    }
};


} } // namespaces


#endif /* __OPENFLUID_BUILDEREXT_BUILDEREXTENSIONSIGNATURE_HPP__ */
