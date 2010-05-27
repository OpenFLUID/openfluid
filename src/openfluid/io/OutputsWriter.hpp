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
  \file OutputsWriter.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OUTPUTSWRITER_HPP__
#define __OUTPUTSWRITER_HPP__

#include <string>
#include <openfluid/dllexport.hpp>
#include <openfluid/core.hpp>
#include <openfluid/base.hpp>


namespace openfluid { namespace io {


class DLLEXPORT OutputsWriter
{
  private:

    std::string m_DirPath;
    openfluid::base::OutputDescriptor m_OutDesc;


    static std::string generateOuputFilename(const std::string UnitClass, const openfluid::core::UnitID_t UnitID,
                                   const std::string Suffix, const std::string VectorName = "");

    static std::string generateOutputScalarsFileHeader(const openfluid::core::UnitClass_t UnitClass, const openfluid::core::UnitID_t UnitID,
                                             const std::string Filename, const std::vector<std::string> ScalarsNames,
                                             const std::string CommentChar);

    static std::string generateOutputVectorFileHeader(const openfluid::core::UnitClass_t UnitClass, const openfluid::core::UnitID_t UnitID,
                                            const std::string Filename, const std::string VectorName,
                                            const std::string CommentChar);


    static std::string generateOutputScalarsFileContent(const openfluid::core::Unit* aUnit,
                                              const std::vector<std::string> ScalarsNames,
                                              openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                                              openfluid::base::SimulationInfo *SimInfo,
                                              std::string DateFormat,
                                              std::string ColSeparator,
                                              unsigned int Precision);

    static std::string generateOutputVectorFileContent(const openfluid::core::Unit* aUnit, const std::string VectorName,
                                              openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                                              openfluid::base::SimulationInfo *SimInfo,
                                              std::string DateFormat,
                                              std::string ColSeparator,
                                              unsigned int Precision);

    void saveUnitFileOutput(openfluid::core::Unit* aUnit, int FileOutputIndex, int OutputSetIndex,
                            openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                            openfluid::base::SimulationInfo *SimInfo);

    void prepareUnitFileOutput(openfluid::core::Unit* aUnit, int FileOutputIndex, int OutputSetIndex);


  public:
    OutputsWriter(const std::string DirPath, const openfluid::base::OutputDescriptor& OutDesc);

    ~OutputsWriter();

    void prepareDirectory();

    void saveToDirectory(openfluid::core::TimeStep_t CurrentStep,
                         openfluid::base::SimulationInfo *SimInfo, bool WithoutKeep);

};


} } //namespaces


#endif /* __OUTPUTSWRITER_HPP__ */
