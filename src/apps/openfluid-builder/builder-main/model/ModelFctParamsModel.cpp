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
 \file ModelFctParamsModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelFctParamsModel.hpp"

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include <openfluid/fluidx/AdvancedModelDescriptor.hpp>

#include <openfluid/ware/GeneratorSignature.hpp>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/FunctionSignatureRegistry.hpp>

// =====================================================================
// =====================================================================

ModelFctParamsModelImpl::ModelFctParamsModelImpl(
    openfluid::fluidx::AdvancedModelDescriptor& Model) :
    mp_Item(0), mp_ItemSignature(0), mp_Model(&Model)
{
}

// =====================================================================
// =====================================================================

void ModelFctParamsModelImpl::setModelItemDescriptor(
    openfluid::fluidx::ModelItemDescriptor* Item)
{
  mp_Item = Item;

  if (mp_Item)
  {
    mp_ItemSignature =
        openfluid::machine::FunctionSignatureRegistry::getInstance()->getSignatureItemInstance(
            mp_Item);
    m_signal_ItemInit.emit();
  }
}

// =====================================================================
// =====================================================================

std::map<std::string, std::string> ModelFctParamsModelImpl::getParams()
{
  std::map<std::string, std::string> ParamsMap;

  BOOST_FOREACH(openfluid::ware::SignatureHandledDataItem Param, mp_ItemSignature->Signature->HandledData.FunctionParams){
  ParamsMap[Param.DataName] = Param.DataUnit;
}

  std::map<std::string, std::string> Params = mp_Item->getParametersAsMap();

  for (std::map<std::string, std::string>::iterator it = Params.begin();
      it != Params.end(); ++it)
  {
    //add possible params which are not in signature
    if (ParamsMap.find(it->first) == ParamsMap.end())
      ParamsMap[it->first];
  }

  return ParamsMap;
}

// =====================================================================
// =====================================================================

std::map<std::string, std::string> ModelFctParamsModelImpl::getParamValues()
{
  return mp_Item->getParametersAsMap();
}

// =====================================================================
// =====================================================================

std::map<std::string, std::string> ModelFctParamsModelImpl::getGlobalValues()
{
  return openfluid::fluidx::WareDescriptor::getParamsAsMap(
      mp_Model->getGlobalParameters());
}

// =====================================================================
// =====================================================================

std::vector<std::string> ModelFctParamsModelImpl::getRequiredFiles()
{
  return mp_ItemSignature->Signature->HandledData.RequiredExtraFiles;
}

// =====================================================================
// =====================================================================

std::vector<std::string> ModelFctParamsModelImpl::getUsedFiles()
{
  return mp_ItemSignature->Signature->HandledData.UsedExtraFiles;
}

// =====================================================================
// =====================================================================

void ModelFctParamsModelImpl::setParamValue(std::string ParamName,
                                            std::string ParamValue)
{
  mp_Item->setParameter(ParamName, ParamValue);

  updateInterpGeneratorRequiredExtraFiles();

  m_signal_ParamsChanged.emit();
}

// =====================================================================
// =====================================================================

void ModelFctParamsModelImpl::updateInterpGeneratorRequiredExtraFiles()
{
//  if (mp_Item->isType(openfluid::fluidx::ModelItemDescriptor::Generator))
//  {
//    openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method =
//        (static_cast<openfluid::ware::GeneratorSignature*>(mp_ItemSignature->Signature))->m_GeneratorMethod;
//
//    if (Method == openfluid::fluidx::GeneratorDescriptor::Interp || Method
//        == openfluid::fluidx::GeneratorDescriptor::Inject)
//    {
//      openfluid::ware::WareParams_t GlobalParams =
//          mp_Model->getGlobalParameters();
//
//      std::string Sources = "";
//      if (mp_Item->getParameters().find("sources") != mp_Item->getParameters().not_found() && mp_Item->getParameters().get<
//          std::string>("sources")
//                                                                                              != "")
//        Sources = mp_Item->getParameters().get<std::string>("sources");
//      else if (GlobalParams.find("sources") != GlobalParams.not_found()
//          && GlobalParams.get<std::string>("sources") != "")
//        Sources = GlobalParams.get<std::string>("sources");
//
//      std::string Distrib = "";
//      if (mp_Item->getParameters().find("distribution") != mp_Item->getParameters().not_found() && mp_Item->getParameters().get<
//          std::string>("distribution")
//                                                                                                   != "")
//        Distrib = mp_Item->getParameters().get < std::string > ("distribution");
//      else if (GlobalParams.find("distribution") != GlobalParams.not_found()
//          && GlobalParams.get<std::string>("distribution") != "")
//        Distrib = GlobalParams.get<std::string>("distribution");
//
//      mp_ItemSignature->Signature->HandledData.RequiredExtraFiles.clear();
//
//      mp_ItemSignature->Signature->HandledData.RequiredExtraFiles.push_back(Sources);
//      mp_ItemSignature->Signature->HandledData.RequiredExtraFiles.push_back(Distrib);
//
//      m_signal_RequiredFilesChangedFromApp.emit();
//    }
//  }
}

// =====================================================================
// =====================================================================

void ModelFctParamsModelImpl::updateGlobalValues()
{
  updateInterpGeneratorRequiredExtraFiles();

  m_signal_GlobalValueChanged.emit();
}

// =====================================================================
// =====================================================================

void ModelFctParamsModelImpl::updateParamsValues()
{
  m_signal_ParamsChangedFromApp.emit();
}

// =====================================================================
// =====================================================================

void ModelFctParamsModelImpl::whenRequiredFileChanged()
{
  m_signal_RequiredFileChanged.emit();
}

// =====================================================================
// =====================================================================

sigc::signal<void> ModelFctParamsModelImpl::signal_ItemInit()
{
  return m_signal_ItemInit;
}

// =====================================================================
// =====================================================================

sigc::signal<void> ModelFctParamsModelImpl::signal_GlobalValueChanged()
{
  return m_signal_GlobalValueChanged;
}

// =====================================================================
// =====================================================================

sigc::signal<void> ModelFctParamsModelImpl::signal_RequiredFileChanged()
{
  return m_signal_RequiredFileChanged;
}

// =====================================================================
// =====================================================================

sigc::signal<void> ModelFctParamsModelImpl::signal_ParamsChanged()
{
  return m_signal_ParamsChanged;
}

// =====================================================================
// =====================================================================

sigc::signal<void> ModelFctParamsModelImpl::signal_ParamsChangedFromApp()
{
  return m_signal_ParamsChangedFromApp;
}

// =====================================================================
// =====================================================================

sigc::signal<void> ModelFctParamsModelImpl::signal_RequiredFilesChangedFromApp()
{
  return m_signal_RequiredFilesChangedFromApp;
}
