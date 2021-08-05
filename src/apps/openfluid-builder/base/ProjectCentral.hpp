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
 @file ProjectCentral.hpp

 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 */

#ifndef __OPENFLUID_BUILDERAPP_PROJECTCENTRAL_HPP__
#define __OPENFLUID_BUILDERAPP_PROJECTCENTRAL_HPP__


#include <QObject>
#include <QMap>
#include <QStringListModel>

#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/fluidx/FluidXIO.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>

#include "ProjectCheckInfos.hpp"


class ProjectCentral : QObject
{
  Q_OBJECT;

  private:

    static ProjectCentral* mp_Instance;

    openfluid::fluidx::FluidXDescriptor m_FXDesc;

    openfluid::fluidx::FluidXIO::LoadingReport m_LoadingReport;

    ProjectCheckInfos m_CheckInfos;

    /**
      List of simulators IDs involved in the model
    */
    QStringList m_SimulatorsIDsList;

    /**
      List of observer IDs involved in the monitoring
    */
    QStringList m_ObserversIDsList;

    /**
      List of parameters by simulators involved in the model
    */
    QMap<QString,QStringList> m_SimulatorsParamsLists;

    /**
      List of variables by units classes involved in the model
    */
    QMap<QString,QStringList> m_VariablesNamesLists;

    /**
      List of units classes in the spatial domain
    */
    QStringList m_UnitsClassesList;

    /**
      List of spatial attributes by units classes
    */
    QMap<QString,QStringList> m_AttributesLists;

    QStringListModel* mp_AllNamesListModel;


    ProjectCentral(const QString& PrjPath = "");

    ~ProjectCentral();

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

    static QStringList convertUpdatedUnitsClassesToQStringList(
        const std::vector<openfluid::ware::SignatureUnitsClassItem>& UnitsClassesVector);


  public:

    enum class RunMode
    {
      UNDEFINED,
      DEFAULT,
      CLI
    };

    static ProjectCentral* initInstance(const QString& PrjPath = "");

    static void kill();

    static ProjectCentral* instance();

    void run(RunMode Mode = RunMode::DEFAULT);

    void check();

    bool save();

    bool saveAs(const QString& NewPrjName, const QString& NewPrjPath);

    const ProjectCheckInfos* checkInfos() const
    {
      return &m_CheckInfos;
    }

    openfluid::fluidx::FluidXDescriptor& descriptors()
    {
      return m_FXDesc;
    }

    const openfluid::fluidx::FluidXDescriptor& descriptors() const
    {
      return m_FXDesc;
    }

    const openfluid::fluidx::FluidXIO::LoadingReport& loadingReport() const
    {
      return m_LoadingReport;
    }

    const QStringList& simulatorsIDsList() const
    {
      return m_SimulatorsIDsList;
    }

    const QStringList& observersIDsList() const
    {
      return m_ObserversIDsList;
    }

    const QMap<QString,QStringList>& simulatorsParamsLists() const
    {
      return m_SimulatorsParamsLists;
    }

    /**
      Returns a list of all parameters for simulators involved in the model, sorted without duplicate
    */
    QStringList getSimulatorsParamsList() const;

    const QMap<QString,QStringList>& variablesNamesLists() const
    {
      return m_VariablesNamesLists;
    }

    /**
      Returns a list of all variables names for all units class, sorted without duplicate
    */
    QStringList getVariablesNamesList() const;

    const QStringList& unitsClassesList() const
    {
      return m_UnitsClassesList;
    }

    const QMap<QString,QStringList>& attributesLists() const
    {
      return m_AttributesLists;
    }

    /**
      Returns a list of all attributes for all units class, sorted without duplicate
    */
    QStringList getAttributesList() const;

    /**
      Returns a list of all names from all lists, sorted without duplicate
    */
    QStringList getAllNamesList() const;

    QStringListModel* allNamesListModel() const
    {
      return mp_AllNamesListModel;
    }

    static QString getRunModeStr(RunMode Mode); 

    static RunMode getRunModeValue(const QString& Str); 

};

#endif /* __OPENFLUID_BUILDERAPP_PROJECTCENTRAL_HPP__ */

