/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file DomainReader.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <boost/filesystem/path.hpp>

#include "AppTools.h"
#include "DomainReader.h"

// =====================================================================
// =====================================================================


DomainReader::DomainReader()
{

}


// =====================================================================
// =====================================================================


DomainReader::~DomainReader()
{

}


// =====================================================================
// =====================================================================


void DomainReader::loadFromFile(std::string Filename, std::list<openfluid::core::UnitsLink_t>* ToUnitsList)
{
  throw openfluid::base::OFException("under construction");
}


// =====================================================================
// =====================================================================


void DomainReader::loadDomainFromDirectory(std::string DirPath, openfluid::core::CoreRepository* CoreData)
{

  std::list<openfluid::core::UnitsLink_t> ToUnitsList;
  std::list<openfluid::core::UnitsLink_t>::iterator it;

  openfluid::core::Unit* FromUnit;
  openfluid::core::Unit* ToUnit;

  std::vector<std::string> FilesToLoad = GetFilesByExt(DirPath,"ddef.xml",true);

  bool IsOK = true;
  unsigned int i=0;

  std::string CurrentFile;
  boost::filesystem::path CurrentFilePath;

  while (IsOK && i<FilesToLoad.size())
  {
    CurrentFilePath = boost::filesystem::path(FilesToLoad[i]);
    CurrentFile = CurrentFilePath.leaf();
    loadFromFile(CurrentFile, &ToUnitsList);
    i++;
  }


  // Processing topology links
  for (it=ToUnitsList.begin();it!=ToUnitsList.end();++it)
  {
    FromUnit = CoreData->getUnit(it->first.first,it->first.second);
    ToUnit = CoreData->getUnit(it->second.first,it->second.second);

    if (ToUnit != NULL)
    {
      FromUnit->addToUnit(ToUnit);
      ToUnit->addFromUnit(FromUnit);
    }
    else
    {
      std::ostringstream UnitStr;
      UnitStr << FromUnit->getClass() << "#" << FromUnit->getID();
      throw openfluid::base::OFException("kernel","DomainReader::LoadDomainFromDirectory","Target -to- unit referenced by " + UnitStr.str() + " does not exist" );
      IsOK = false;
    }
  }

  CoreData->sortUnitsByProcessOrder();

}


// =====================================================================
// =====================================================================



