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
 @file WareSrcChecker.hpp
 
 @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
 @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#ifndef __OPENFLUID_WARESDEV_WARESRCCHECKER_HPP__
#define __OPENFLUID_WARESDEV_WARESRCCHECKER_HPP__


#include <string>

#include <openfluid/utils/ReportingData.hpp>
#include <openfluid/utils/CppLangHelpers.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace waresdev {


/**
  Class to check wares source code
*/
class OPENFLUID_API WareSrcChecker
{
  public:

    using ReportingData = openfluid::utils::ReportingData<std::string,std::string>;

    using ChecksList = std::vector<std::string>;


  private:

    const openfluid::tools::FilesystemPath m_SrcPathObj;

    const ChecksList m_IgnoredChecks;


    void processReportingItem(ReportingData::ReportingList& List,
                              const std::string& Msg, std::function<bool()> Predicate,
                              ReportingData::ReportingStatus StatusIfFail = 
                                ReportingData::ReportingStatus::WARNING) const;

    WareSrcChecker::ReportingData::ReportingList performStructureCheck(bool OKToRun) const;

    WareSrcChecker::ReportingData::ReportingList performMetainfoCheck(bool OKToRun) const;

    WareSrcChecker::ReportingData::ReportingList performCodeCheck(bool OKToRun) const;

    void updateWithPedanticCheck(ReportingData& RepData) const;

    bool hasMigrationFiles() const;


  public:

    CPPCLASS_DELETE_FIVE(WareSrcChecker);

    /**
      Constructs a checker
      @param[in] SrcPath The path to the sources to check
      @param[in] IgnoredChecks A list of checks to ignore (none by default)
    */
    WareSrcChecker(const std::string& SrcPath, const ChecksList& IgnoredChecks = {});

    /**
      Perform the checks
      @param[in] Pedantic Enables pedantic supplementary checks
      @return The report of the performed checks
    */
    ReportingData performCheck(bool Pedantic = false) const;

    /**
      Tries to detect the OpenFLUID compatibility version of the sources. Currently, 
      it tries to distinguish version 2.2.x from versions below.
      @param[in] PathObj The path to the sources to detect
      @return The numeric representation of the detected OpenFLUID version (see below), 0 if version cannot be detected.
              Returned value  | OpenFLUID version
              --------------- | -----------------
              202000          | 2.2.0
              201000          | 2.1.x or below
    */
    static unsigned int tryDetectWareSrcVersion(const openfluid::tools::FilesystemPath& PathObj);

};


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESRCCHECKER_HPP__ */
