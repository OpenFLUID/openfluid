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
  \file FluidXWriter.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include <fstream>
#include <openfluid/io/FluidXWriter.hpp>
#include <openfluid/machine/Generator.hpp>
#include <boost/filesystem.hpp>

namespace openfluid { namespace io {


FluidXWriter::FluidXWriter(openfluid::io::IOListener* Listener)
: m_InstType(openfluid::core::InstantiationInfo::DESCRIPTOR)
{
  mp_Listener = Listener;
  if (mp_Listener == NULL) mp_Listener = new openfluid::io::IOListener();

  m_DomainStr.clear();
  m_ModelStr.clear();
  m_RunStr.clear();
  m_OutputStr.clear();

  m_IndentStr = "  ";
}


// =====================================================================
// =====================================================================


FluidXWriter::~FluidXWriter()
{

}


// =====================================================================
// =====================================================================


std::string FluidXWriter::getGeneratorMethodAsStr(openfluid::base::GeneratorDescriptor::GeneratorMethod Method) const
{
  if (Method == openfluid::base::GeneratorDescriptor::Fixed)
    return "fixed";

  if (Method == openfluid::base::GeneratorDescriptor::Random)
    return "random";

  if (Method == openfluid::base::GeneratorDescriptor::Interp)
    return "interp";

  if (Method == openfluid::base::GeneratorDescriptor::Inject)
    return "inject";

  return "";
}


// =====================================================================
// =====================================================================


std::string FluidXWriter::getParamsAsStr(const openfluid::core::FuncParamsMap_t& Params) const
{
  std::string ParamsStr = "";

  openfluid::core::FuncParamsMap_t::const_iterator itParams;

  for (itParams = Params.begin();itParams != Params.end() ; ++itParams)
    ParamsStr += m_IndentStr + m_IndentStr + m_IndentStr + "<param name=\"" + (*itParams).first +"\" value=\"" + (*itParams).second +"\"/>\n";

  return ParamsStr;
}


// =====================================================================
// =====================================================================


void FluidXWriter::setModelToWrite(openfluid::machine::ModelInstance& MInstance)
{
  if (MInstance.getItemsCount() > 0)
  {
    std::ostringstream Contents;

    Contents << m_IndentStr << "<model>\n";

    if (MInstance.getGlobalParameters().size() > 0)
    {
      Contents << m_IndentStr << m_IndentStr << "<gparams>\n";
      Contents << getParamsAsStr(MInstance.getGlobalParameters());
      Contents << m_IndentStr << m_IndentStr << "</gparams>\n";
    }


    const std::list<openfluid::machine::ModelItemInstance*> Items = MInstance.getItems();
    std::list<openfluid::machine::ModelItemInstance*>::const_iterator itFuncs;

    for (itFuncs=Items.begin();itFuncs!=Items.end();++itFuncs)
    {

      if ((*itFuncs)->ItemType == openfluid::base::ModelItemDescriptor::PluggedFunction)
      {
        Contents << m_IndentStr << m_IndentStr << "<function fileID=\"" << (*itFuncs)->Signature->ID << "\">\n";
        Contents << getParamsAsStr((*itFuncs)->Params);
        Contents << m_IndentStr << m_IndentStr << "</function>\n";
      }

      if ((*itFuncs)->ItemType == openfluid::base::ModelItemDescriptor::Generator)
      {
        Contents << m_IndentStr << m_IndentStr << "<generator varname=\"" << ((openfluid::machine::Generator*)(*itFuncs)->Function)->getVariableName() << "\" "
                                               << "unitclass=\"" <<  ((openfluid::machine::Generator*)(*itFuncs)->Function)->getUnitClass()  << "\" "
                                               << "method=\"" <<  getGeneratorMethodAsStr(((openfluid::machine::Generator*)(*itFuncs)->Function)->getGeneratorMethod()) << "\"";


        if (((openfluid::machine::Generator*)(*itFuncs)->Function)->isVectorVariable())
          Contents << " varsize=\"" <<  ((openfluid::machine::Generator*)(*itFuncs)->Function)->getVariableSize() << "\"";

        Contents << ">\n";
        Contents << getParamsAsStr((*itFuncs)->Params);
        Contents << m_IndentStr << m_IndentStr << "</generator>\n";
      }

    }


    Contents << m_IndentStr << "</model>\n";

    m_ModelStr  = Contents.str();
  }
}


// =====================================================================
// =====================================================================


void FluidXWriter::setRunConfigurationToWrite(openfluid::base::RunDescriptor& RDescriptor)
{
  if (RDescriptor.isFilled())
  {
    std::ostringstream Contents;

    Contents << m_IndentStr << "<run>\n";
    Contents << m_IndentStr << m_IndentStr << "<deltat>" << RDescriptor.getDeltaT() << "</deltat>\n";
    Contents << m_IndentStr << m_IndentStr << "<period begin=\"" << RDescriptor.getBeginDate().getAsISOString() <<
                                                   "\" end=\"" << RDescriptor.getEndDate().getAsISOString() << "\" />\n";

    if (RDescriptor.isUserValuesBufferSize())
      Contents << m_IndentStr << m_IndentStr << "<valuesbuffer steps=\"" << RDescriptor.getValuesBufferSize() << "\" />\n";

    Contents << m_IndentStr << m_IndentStr << "<filesbuffer kbytes=\"" << RDescriptor.getFilesBufferSizeInKB() << "\" />\n";
    Contents << m_IndentStr << "</run>\n";

    m_RunStr = Contents.str();
  }

}


// =====================================================================
// =====================================================================


void FluidXWriter::setDomainToWrite(const openfluid::core::CoreRepository& CoreData)
{
  if (!CoreData.getUnitsGlobally()->empty())
  {
    std::ostringstream Contents;

    Contents << m_IndentStr << "<domain>\n";

    Contents << m_IndentStr << m_IndentStr << "<definition>\n";

    const openfluid::core::UnitsListByClassMap_t* UnitsByClass = CoreData.getUnitsByClass();
    const openfluid::core::UnitsList_t* UnitsList = NULL;
    const openfluid::core::UnitsPtrList_t* AllUnitsPtrList = NULL;
    openfluid::core::UnitsPtrList_t::const_iterator itAllUnits;
    std::vector<openfluid::core::UnitClass_t> ClassVector;
    openfluid::core::Unit* TheUnit;

    openfluid::core::UnitsListByClassMap_t::const_iterator itUnitsClass;
    openfluid::core::UnitsList_t::const_iterator itUnitsList;


    for (itUnitsClass=UnitsByClass->begin();itUnitsClass!=UnitsByClass->end();++itUnitsClass)
    {
      ClassVector.push_back((*itUnitsClass).first);
    }

    for (itUnitsClass=UnitsByClass->begin();itUnitsClass!=UnitsByClass->end();++itUnitsClass)
    {

      UnitsList=((*itUnitsClass).second).getList();

      for (itUnitsList=UnitsList->begin();itUnitsList!=UnitsList->end();++itUnitsList)
      {
        TheUnit = const_cast<openfluid::core::Unit*>(&(*itUnitsList));

        if (TheUnit->getInstantiationType() == m_InstType)
        {
          Contents << m_IndentStr << m_IndentStr << m_IndentStr << "<unit class=\"" << TheUnit->getClass() << "\" ID=\"" << TheUnit->getID() << "\" pcsorder=\"" << TheUnit->getProcessOrder() << "\">\n";

          for (unsigned int i=0;i<ClassVector.size();i++)
          {


            const openfluid::core::UnitsPtrList_t* ToUnits = const_cast<openfluid::core::UnitsPtrList_t*>(TheUnit->getToUnits(ClassVector[i]));


            if (ToUnits != NULL)
            {

              std::string DestClassStr = ClassVector[i];
              openfluid::core::UnitsPtrList_t::const_iterator itToUnits;

              for (itToUnits=ToUnits->begin();itToUnits!=ToUnits->end();++itToUnits)
              {
                Contents << m_IndentStr << m_IndentStr << m_IndentStr << m_IndentStr << "<to class=\"" << (*itToUnits)->getClass() << "\" ID=\"" << (*itToUnits)->getID() << "\" />\n";
              }
            }

            const openfluid::core::UnitsPtrList_t* ParentUnits = const_cast<openfluid::core::UnitsPtrList_t*>(TheUnit->getParentUnits(ClassVector[i]));

            if (ParentUnits != NULL)
            {
              std::string DestClassStr = ClassVector[i];
              openfluid::core::UnitsPtrList_t::const_iterator itParentUnits;

              for (itParentUnits=ParentUnits->begin();itParentUnits!=ParentUnits->end();++itParentUnits)
              {
                Contents << m_IndentStr << m_IndentStr << m_IndentStr << m_IndentStr << "<childof class=\"" << (*itParentUnits)->getClass() << "\" ID=\"" << (*itParentUnits)->getID() << "\" />\n";

              }
            }
          }
          Contents << m_IndentStr << m_IndentStr << m_IndentStr << "</unit>\n";
        }
      }
    }

    Contents << m_IndentStr << m_IndentStr << "</definition>\n";


    std::vector<openfluid::core::InputDataName_t> IDataNames;

    for (itUnitsClass=UnitsByClass->begin();itUnitsClass!=UnitsByClass->end();++itUnitsClass)
    {

      UnitsList = ((*itUnitsClass).second).getList();

      if (!UnitsList->empty())
      {

        TheUnit = const_cast<openfluid::core::Unit*>(&(UnitsList->front()));

        IDataNames.clear();
        IDataNames = TheUnit->getInputData()->getInputDataNames();

        if (!IDataNames.empty())
        {
          Contents << m_IndentStr << m_IndentStr << "<inputdata unitclass=\"" <<  ((*itUnitsClass).first) << "\" colorder=\"";

          for (unsigned int j=0;j<IDataNames.size();j++)
          {
            Contents << IDataNames[j];
            if (j!=(IDataNames.size()-1)) Contents << ";";
          }

          Contents << "\">\n";

          for (itUnitsList=UnitsList->begin();itUnitsList!=UnitsList->end();++itUnitsList)
          {
            TheUnit = const_cast<openfluid::core::Unit*>(&(*itUnitsList));
            if (TheUnit->getInstantiationType() == m_InstType)
            {
              Contents << TheUnit->getID() << "\t";

              for (unsigned int j=0;j<IDataNames.size();j++)
              {
                std::string ValueStr;
                TheUnit->getInputData()->getValue(IDataNames[j],&ValueStr);
                Contents << ValueStr;
                if (j!=(IDataNames.size()-1)) Contents << "\t";
              }
              Contents << "\n";
            }
          }
          Contents << m_IndentStr << m_IndentStr << "</inputdata>\n";
        }
      }
    }



    Contents << m_IndentStr << m_IndentStr << "<calendar>\n";

    AllUnitsPtrList = CoreData.getUnitsGlobally();
    for (itAllUnits = AllUnitsPtrList->begin();itAllUnits != AllUnitsPtrList->end();++itAllUnits)
    {
      if ((*itAllUnits)->getInstantiationType() == m_InstType && (*itAllUnits)->getEvents()->getCount() > 0)
      {
        openfluid::core::EventsList_t *Events = (*itAllUnits)->getEvents()->getEventsList();
        openfluid::core::EventsList_t::iterator itEvents;

        for (itEvents = Events->begin();itEvents != Events->end();++itEvents)
        {
          if((*itEvents).getInstantiationType() == m_InstType)
          {
            openfluid::core::Event::EventInfosMap_t Infos = (*itEvents).getInfos();
            openfluid::core::Event::EventInfosMap_t::iterator itInfos;


            Contents << m_IndentStr << m_IndentStr << m_IndentStr << "<event unitclass=\"" << (*itAllUnits)->getClass() << "\" " <<
                "unitID=\"" << (*itAllUnits)->getID() << "\" " <<
                "date=\"" << (*itEvents).getDateTime().getAsISOString() << "\">\n";

            for (itInfos = Infos.begin();itInfos != Infos.end();++itInfos)
            {
              Contents << m_IndentStr << m_IndentStr << m_IndentStr << m_IndentStr;
              Contents << "<info key=\"" << itInfos->first << "\" value=\"" << itInfos->second << "\" />\n";
            }

            Contents << m_IndentStr << m_IndentStr << m_IndentStr <<"</event>\n";
          }

        }

      }
    }

    Contents << m_IndentStr << m_IndentStr << "</calendar>\n";


    Contents << m_IndentStr << "</domain>\n";

    m_DomainStr = Contents.str();
  }
}


// =====================================================================
// =====================================================================


void FluidXWriter::setOutputConfigurationToWrite(openfluid::base::OutputDescriptor& ODescriptor)
{
  std::ostringstream Contents;

  if (!ODescriptor.getFileSets().empty())
  {

    std::vector<openfluid::base::OutputFilesDescriptor> FileSetDesc = ODescriptor.getFileSets();

    if (!FileSetDesc.empty())
    {
      Contents << m_IndentStr << "<output>\n";


      for (unsigned int i = 0; i< FileSetDesc.size();i++)
      {
        std::string ColSep = FileSetDesc[i].getColSeparator();
        boost::algorithm::replace_all(ColSep,"\t","\\t");

        std::string CommentChar = FileSetDesc[i].getCommentChar();
        boost::algorithm::replace_all(CommentChar,"\t","\\t");

        Contents << m_IndentStr << m_IndentStr << "<files colsep=\"" << ColSep << "\" " <<
            "dtformat=\"" << FileSetDesc[i].getDateFormat() << "\" " <<
            "commentchar=\"" << CommentChar << "\">\n";

        std::vector<openfluid::base::OutputSetDescriptor> SetsDesc = FileSetDesc[i].getSets();

        for (unsigned int j = 0; j< SetsDesc.size();j++)
        {
          std::string VarsStr = "";
          std::string IDsStr = "";

          if (SetsDesc[j].isAllUnits()) IDsStr = "*";
          else
          {
            std::ostringstream IDsStrStr;
            IDsStrStr.clear();
            for (unsigned int k = 0; k< SetsDesc[j].getUnitsIDs().size();k++)
            {
              IDsStrStr << SetsDesc[j].getUnitsIDs()[k];
              if ((k != SetsDesc[j].getUnitsIDs().size()-1))
                IDsStrStr << ";";
            }
            IDsStr = IDsStrStr.str();
          }

          if (SetsDesc[j].isAllVariables()) VarsStr = "*";
          else
          {
            for (unsigned int k = 0; k< SetsDesc[j].getVariables().size();k++)
            {
              VarsStr += SetsDesc[j].getVariables()[k];
              if ((k != (SetsDesc[j].getVariables().size()-1)) /*|| (!SetsDesc[j].getVariables().empty())*/)
                VarsStr += ";";
            }
          }


          Contents << m_IndentStr << m_IndentStr << m_IndentStr;
          Contents << "<set name=\"" << SetsDesc[j].getName() << "\" " <<
              "unitsclass=\"" << SetsDesc[j].getUnitsClass() << "\" " <<
              "unitsIDs=\"" << IDsStr << "\" " <<
              "vars=\"" << VarsStr << "\" " <<
              "precision=\"" << SetsDesc[j].getPrecision() << "\" />\n";
        }
        Contents << m_IndentStr << m_IndentStr << "</files>\n";
      }
      Contents << m_IndentStr << "</output>\n";
    }
  }

  m_OutputStr = Contents.str();
}


// =====================================================================
// =====================================================================


void FluidXWriter::prepareOutputDir(std::string DirPath)
{

  boost::filesystem::path OutputDirPath(DirPath);

  if (!boost::filesystem::exists(OutputDirPath))
  {
    boost::filesystem::create_directory(OutputDirPath);
    if (!boost::filesystem::exists(OutputDirPath))
      throw openfluid::base::OFException("OpenFLUID framework","FluidXWriter::prepareOutputDir","Error creating output directory");
  }
}


// =====================================================================
// =====================================================================


void FluidXWriter::WriteToManyFiles(std::string DirPath)
{
  mp_Listener->onWrite();

  std::ofstream OutFile;
  std::string OutFilename;

  prepareOutputDir(DirPath);

  // model
  OutFilename= boost::filesystem::path(DirPath+"/model.fluidx").string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(),std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n";
  OutFile << m_ModelStr << "\n\n";
  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::OK);

  // model
  OutFilename= boost::filesystem::path(DirPath+"/domain.fluidx").string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(),std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n";
  OutFile << m_DomainStr << "\n\n";
  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::OK);

  // run
  OutFilename= boost::filesystem::path(DirPath+"/run.fluidx").string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(),std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n";
  OutFile << m_RunStr << "\n\n";
  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::OK);

  // model
  OutFilename= boost::filesystem::path(DirPath+"/output.fluidx").string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(),std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n";
  OutFile << m_OutputStr << "\n\n";
  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::OK);

  mp_Listener->onWritten(openfluid::base::Listener::OK);

}


// =====================================================================
// =====================================================================


void FluidXWriter::WriteToSingleFile(std::string FilePath)
{
  mp_Listener->onWrite();

  std::ofstream OutFile;

  prepareOutputDir(boost::filesystem::path(FilePath).branch_path().string());

  std::string OutFilename = boost::filesystem::path(FilePath).string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(),std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n";

  OutFile << m_ModelStr << "\n\n";
  OutFile << m_DomainStr << "\n\n";
  OutFile << m_RunStr << "\n\n";
  OutFile << m_OutputStr << "\n\n";

  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::OK);

  mp_Listener->onWritten(openfluid::base::Listener::OK);

}



} } // namespaces

