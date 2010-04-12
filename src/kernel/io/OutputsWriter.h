/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


/**
  \file OutputsWriter.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OUTPUTSWRITER_H___
#define __OUTPUTSWRITER_H___

#include <string>
#include "openfluid-core.h"
#include "openfluid-base.h"
#include "OutputDescriptor.h"

class OutputsWriter
{
  private:

    std::string m_DirPath;
    OutputDescriptor m_OutDesc;


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
    OutputsWriter(const std::string DirPath, const OutputDescriptor& OutDesc);

    ~OutputsWriter();

    void prepareDirectory();

    void saveToDirectory(openfluid::core::TimeStep_t CurrentStep,
                         openfluid::base::SimulationInfo *SimInfo, bool WithoutKeep);

};



#endif /* __OUTPUTSWRITER_H___ */
