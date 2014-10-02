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
 \file ProjectCentral.hpp

 \author Jean-Christophe Fabre <fabrejc@supagro.inra.fr>
 */

#ifndef __PROJECTCENTRAL_HPP__
#define __PROJECTCENTRAL_HPP__


#include <QObject>

#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/base/IOListener.hpp>

#include "ProjectCheckInfos.hpp"





class ProjectCentral : QObject
{
  Q_OBJECT;

  private:

    openfluid::fluidx::FluidXDescriptor* mp_FXDesc;

    openfluid::fluidx::AdvancedFluidXDescriptor* mp_AdvancedFXDesc;

    ProjectCheckInfos m_CheckInfos;

    openfluid::base::IOListener m_IOListener;

    void deleteData();

    void setDefaultDescriptors();

    void checkModel();

    void checkSpatialDomain();

    void checkDatastore();

    void checkMonitoring();

    void checkRunConfig();

    void checkGeneratorParam(const std::string& MinParamName,
                             const std::string& MaxParamName,
                             openfluid::fluidx::ModelItemDescriptor* Item,
                             const std::string& ID);

    bool isParamSet(openfluid::fluidx::ModelItemDescriptor* Item,
                    const std::string& ParamName);

    bool isParamIsDouble(openfluid::fluidx::ModelItemDescriptor* Item,
                         const std::string& ParamName);

    double getParamAsDouble(openfluid::fluidx::ModelItemDescriptor* Item,
                            const std::string& ParamName);

    static QStringList convertUpdatedUnitsClassesToQStringList(const std::vector<openfluid::ware::SignatureHandledUnitsClassItem>& UnitsClassesVector);


  public:

    ProjectCentral(QString PrjPath = "");

   ~ProjectCentral();

   void run();

   void check();

   bool save();

   bool saveAs(const QString& NewPrjName, const QString& NewPrjPath);

   const ProjectCheckInfos* getCheckInfos() const
   {  return &m_CheckInfos; }

   openfluid::fluidx::AdvancedFluidXDescriptor& getAdvancedDescriptors()
   { return *mp_AdvancedFXDesc; }

   const openfluid::fluidx::AdvancedFluidXDescriptor& getAdvancedDescriptors() const
   { return *mp_AdvancedFXDesc; }

};

#endif /* __PROJECTCENTRAL_HPP__ */

