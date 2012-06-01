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
  \file FluidXWriter.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __FLUIDXWRITER_HPP__
#define __FLUIDXWRITER_HPP__

#include <string>

#include <openfluid/dllexport.hpp>
#include <openfluid/base/GeneratorDescriptor.hpp>
#include <openfluid/core/InstantiationInfo.hpp>

namespace openfluid {
namespace base {
class RunDescriptor;
class OutputDescriptor;
}
namespace core {
class CoreRepository;
class Datastore;
}
namespace machine {
class ModelInstance;
}
}


namespace openfluid { namespace io {

class IOListener;


class DLLEXPORT FluidXWriter
{
  private:

    std::string m_ModelStr;
    std::string m_RunStr;
    std::string m_DomainStr;
    std::string m_OutputStr;
    std::string m_DataStr;

    openfluid::core::InstantiationInfo::Type m_InstType;

    openfluid::io::IOListener* mp_Listener;

    std::string m_IndentStr;

    void prepareOutputDir(std::string DirPath);

    std::string getGeneratorMethodAsStr(openfluid::base::GeneratorDescriptor::GeneratorMethod Method) const;

    std::string getParamsAsStr(const openfluid::core::FuncParamsMap_t& Params) const;

  public:


    FluidXWriter(openfluid::io::IOListener* Listener);

    ~FluidXWriter();

    void setModelToWrite(openfluid::machine::ModelInstance& MInstance);

    void setRunConfigurationToWrite(openfluid::base::RunDescriptor& RDescriptor);

    void setDomainToWrite(const openfluid::core::CoreRepository& CoreData);

    void setOutputConfigurationToWrite(openfluid::base::OutputDescriptor& ODescriptor);

    void setDatastoreToWrite(const openfluid::core::Datastore& Store);

    void setInstantiationType(const openfluid::core::InstantiationInfo::Type& InstType)
    { m_InstType = InstType; };

    void WriteToManyFiles(std::string DirPath);

    void WriteToSingleFile(std::string FilePath);

};

} } // namespaces


#endif /* __FLUIDXWRITER_HPP__ */

