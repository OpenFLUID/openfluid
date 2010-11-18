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
  \file BuilderProject.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */


#include <boost/date_time/posix_time/posix_time.hpp>

#include <openfluid/io.hpp>

#include "BuilderProject.hpp"


// =====================================================================
// =====================================================================


BuilderProject::BuilderProject(Glib::ustring FolderIn)
{
  mp_RunEnv = openfluid::base::RuntimeEnvironment::getInstance();
  mp_Listener = new BuilderMachineListener();
  mp_IOListener = new openfluid::io::IOListener();

  openfluid::machine::ModelInstance * Model = 0;
  openfluid::machine::SimulationBlob * SimBlob = 0;
  SimBlob = new openfluid::machine::SimulationBlob();

  if(FolderIn!="")
  {
    mp_RunEnv->setInputDir(FolderIn);
    mp_RunEnv->setOutputDir(Glib::ustring::compose("%1/OUT",FolderIn));

    std::cout << "* Loading data... " << std::endl;
    openfluid::io::FluidXReader FXReader(mp_IOListener);
    FXReader.loadFromDirectory(FolderIn);

    m_RunDesc = FXReader.getRunDescriptor();
    m_OutputDesc = FXReader.getOutputDescriptor();

    std::cout << "* Building original Blob... " << std::endl;
    openfluid::machine::Factory::buildSimulationBlobFromDescriptors(FXReader.getDomainDescriptor(),
        m_RunDesc,
        m_OutputDesc,
        *SimBlob);

    std::cout << "* Building original model... " << std::endl;
    Model = new openfluid::machine::ModelInstance(*SimBlob,mp_Listener);
    openfluid::machine::Factory::buildModelInstanceFromDescriptor(FXReader.getModelDescriptor(),
        *SimBlob, *Model);
  }
  else
  {
    m_RunDesc = openfluid::base::RunDescriptor(120,
        openfluid::core::DateTime(2000,01,01,00,00,00),
        openfluid::core::DateTime(2000,01,01,07,00,00));
    m_RunDesc.setFilled(true);

    m_OutputDesc = openfluid::base::OutputDescriptor();
    openfluid::base::DomainDescriptor m_DomainDescriptor = openfluid::base::DomainDescriptor();

    std::cout << "* Building original Blob... " << std::endl;
    openfluid::machine::Factory::buildSimulationBlobFromDescriptors(m_DomainDescriptor,
        m_RunDesc, m_OutputDesc,
        *SimBlob);

    std::cout << "* Building original model... " << std::endl;
    Model = new openfluid::machine::ModelInstance(*SimBlob,mp_Listener);
  }

  DomainModule * DomainMod = new DomainModule(SimBlob->getCoreRepository());
  ModelModule * ModeleMod = new ModelModule(*Model);

  addModule(ModeleMod,"model");
  addModule(DomainMod,"domain");
  addModule(new SimulationModule(m_RunDesc,m_OutputDesc,DomainMod->getIDsByClassMap(),ModeleMod->getVarsByClassMap()),"simulation");
  addModule(new ResultsModule(),"results");

}


// =====================================================================
// =====================================================================


BuilderProject::~BuilderProject()
{
  for(DockItemsPtrByNameMap_t::const_iterator it = m_DockItems.begin() ; it != m_DockItems.end() ; ++it)
  {
    gdl_dock_item_hide_item(GDL_DOCK_ITEM(it->second));

    gtk_widget_destroy(it->second);
  }

  for(ModulesPtrByNameMap_t::const_iterator it = m_Modules.begin() ; it != m_Modules.end() ; ++it)
  {
    delete it->second;
  }

//  if(mp_Engine)
//    delete mp_Engine;
//  delete mp_RunEnv;
//  delete mp_Listener;
//  delete mp_Engine;
//  delete mp_ExecMsgs;
//  delete mp_Model;

}


// =====================================================================
// =====================================================================


void BuilderProject::addModule(ModuleInterface * Module, Glib::ustring ModuleName)
{
  // create DockItem
  Gtk::Container * Container = Module->getContainer();
  const Glib::ustring & RefModuleName = Module->getModuleName();
  const Glib::ustring & RefModuleLongName = "  " + Module->getModuleLongName();
  const Gtk::StockID & RefStockId = *(Module->getStockId());

  GtkWidget * DockItem = gdl_dock_item_new_with_stock(RefModuleName.c_str(),RefModuleLongName.c_str(),RefStockId.get_c_str(),GDL_DOCK_ITEM_BEH_CANT_CLOSE);

  gtk_container_add (GTK_CONTAINER(DockItem), GTK_WIDGET(Container->gobj()));

   //TODO: check if moduleName exists before insert
  m_DockItems[ModuleName] = DockItem;

  m_Modules[ModuleName] = Module;

}


// =====================================================================
// =====================================================================


ModuleInterface * BuilderProject::getModule(Glib::ustring ModuleName)
{
  if(m_Modules.find(ModuleName) != m_Modules.end())
    return m_Modules[ModuleName];
  else
    throw openfluid::base::OFException("BuilderProject::getModule","Wrong ModuleName");
}

// =====================================================================
// =====================================================================


void BuilderProject::actionDefaultLayout(LayoutType Layout)
{
    // Default layout, base for others

  gdl_dock_item_dock_to(GDL_DOCK_ITEM (m_DockItems["simulation"]), GDL_DOCK_ITEM (m_DockItems["model"]), GDL_DOCK_BOTTOM, -1);
  gdl_dock_item_dock_to(GDL_DOCK_ITEM (m_DockItems["results"]), GDL_DOCK_ITEM (m_DockItems["simulation"]), GDL_DOCK_BOTTOM, -1);
  gdl_dock_item_dock_to(GDL_DOCK_ITEM (m_DockItems["domain"]), GDL_DOCK_ITEM (m_DockItems["model"]), GDL_DOCK_RIGHT, -1);

  switch(Layout)
  {
    case PreSimulation:
      gdl_dock_item_iconify_item(GDL_DOCK_ITEM (m_DockItems["simulation"]));
      gdl_dock_item_iconify_item(GDL_DOCK_ITEM (m_DockItems["results"]));
      break;
    case PostSimulation:
      gdl_dock_item_iconify_item(GDL_DOCK_ITEM (m_DockItems["model"]));
      gdl_dock_item_iconify_item(GDL_DOCK_ITEM (m_DockItems["domain"]));
      gdl_dock_item_iconify_item(GDL_DOCK_ITEM (m_DockItems["simulation"]));
      break;
    case AllTabbed:
      gdl_dock_item_dock_to(GDL_DOCK_ITEM (m_DockItems["domain"]), GDL_DOCK_ITEM (m_DockItems["model"]), GDL_DOCK_CENTER, -1);
      gdl_dock_item_dock_to(GDL_DOCK_ITEM (m_DockItems["simulation"]), GDL_DOCK_ITEM (m_DockItems["model"]), GDL_DOCK_CENTER, -1);
      gdl_dock_item_dock_to(GDL_DOCK_ITEM (m_DockItems["results"]), GDL_DOCK_ITEM (m_DockItems["model"]), GDL_DOCK_CENTER, -1);
      break;
    default:;
  }
}


// =====================================================================
// =====================================================================


bool BuilderProject::actionCheckProject()
{
  bool IsOk = true;

  ModelModule * ModelMod = 0;
  DomainModule * DomainMod = 0;
  SimulationModule * SimulMod = 0;

  try
  {
    DomainMod = dynamic_cast<DomainModule*>(getModule("domain"));

    SimulMod = dynamic_cast<SimulationModule*>(getModule("simulation"));
    SimulMod->updateOutputDescriptor();

    std::cout << "* Building Blob before execution... " << std::endl;
    mp_SimBlob = new openfluid::machine::SimulationBlob();
    openfluid::machine::Factory::buildSimulationBlobFromDescriptors(*(DomainMod->getDomainDescriptorWOIData()),
        m_RunDesc,
        m_OutputDesc,
        *mp_SimBlob);
    DomainMod->setInputData(mp_SimBlob->getCoreRepository());

    //--------
//    std::cout << "******** DOMAIN ************" << std::endl;
//    const openfluid::core::UnitsListByClassMap_t * Units = mp_SimBlob->getCoreRepository().getUnitsByClass();
//    openfluid::core::UnitsListByClassMap_t::const_iterator ItUnits;
//    for(ItUnits=Units->begin() ; ItUnits!=Units->end() ; ++ItUnits)
//    {
//      std::cout << ItUnits->first << std::endl;
//
//      openfluid::core::UnitsList_t * UUnits = mp_SimBlob->getCoreRepository().getUnits(ItUnits->first) ->getList();
//      openfluid::core::UnitsList_t::iterator ItUUnits;
//      for(ItUUnits=UUnits->begin() ; ItUUnits!=UUnits->end() ; ++ItUUnits)
//      {
//        // Unit
//        std::cout << "id:" << ItUUnits->getID() << " pcsorder:" << ItUUnits->getProcessOrder() << std::endl;
//
//        // Parents & Tos
//        openfluid::core::UnitsListByClassMap_t::const_iterator ItClasses;
//        for(ItClasses=Units->begin() ; ItClasses!=Units->end() ; ++ItClasses)
//        {
//          openfluid::core::UnitsPtrList_t * Parents = ItUUnits->getParentUnits(ItClasses->first);
//          openfluid::core::UnitsPtrList_t * Tos = ItUUnits->getToUnits(ItClasses->first);
//
//          openfluid::core::UnitsPtrList_t::iterator ItParents;
//          openfluid::core::UnitsPtrList_t::iterator ItTos;
//
//          std::cout << "**Parents of class " << ItClasses->first << " : ";
//          if(Parents)
//          {
//            std::cout << std::endl;
//            for(ItParents=Parents->begin() ; ItParents!=Parents->end() ; ++ItParents)
//              std::cout << "   " << (*ItParents)->getClass() << " " << (*ItParents)->getID() << std::endl;
//          }
//          else
//            std::cout << "none" << std::endl;
//
//          std::cout << "**Tos of class " << ItClasses->first << " : ";
//          if(Tos)
//          {
//            std::cout << std::endl;
//            for(ItTos=Tos->begin() ; ItTos!=Tos->end() ; ++ItTos)
//              std::cout << "   " << (*ItTos)->getClass() << " " << (*ItTos)->getID() << std::endl;
//          }
//          else
//            std::cout << "none" << std::endl;
//        }
//
//        // Input Data
//        std::cout << "**Input Data :" << std::endl;
//        std::vector<std::string> IDataNames = ItUUnits->getInputData()->getInputDataNames();
//        for(unsigned int i=0 ; i<IDataNames.size() ; i++)
//        {
//          if(ItUUnits->getInputData()->isDataExist(IDataNames[i]))
//          {
//            std::string Val;
//            ItUUnits->getInputData()->getValue(IDataNames[i],&Val);
//            std::cout << "   " << IDataNames[i] << ":" << Val << std::endl;
//          }
//        }
//      }
//    }
//        std::cout << "***************************" << std::endl;
    //--------
    std::cout << "...Blob created" << std::endl;


    ModelMod = dynamic_cast<ModelModule*>(getModule("model"));

    std::cout << "* Building model before execution... " << std::endl;
    mp_Model = new openfluid::machine::ModelInstance(*mp_SimBlob,mp_Listener);
    openfluid::machine::Factory::buildModelInstanceFromDescriptor(*(ModelMod->getModelDescriptor()),
        *mp_SimBlob, *mp_Model);
    //------
//    std::cout << "******** MODEL ************" << std::endl;
//    std::list<openfluid::machine::ModelItemInstance*> Fcts = mp_Model->getItems();
//    std::list<openfluid::machine::ModelItemInstance*>::iterator ItFct;
//    for(ItFct=Fcts.begin() ; ItFct!=Fcts.end() ; ++ItFct)
//    {
//      openfluid::machine::ModelItemInstance* IInstance = *ItFct;
//      std::cout << "- " << IInstance->Signature->ID << std::endl;
//
//      openfluid::core::FuncParamsMap_t Params = IInstance->Function->m_ParamsMap;
//      openfluid::core::FuncParamsMap_t::iterator ItParam;
//      for(ItParam=Params.begin() ; ItParam!=Params.end() ; ++ItParam)
//        std::cout << "  * " << ItParam->first << " : " << ItParam->second << std::endl;
//    }
//    std::cout << "***************************" << std::endl;
    //------
    std::cout << "...Model created" << std::endl;

    std::cout << "* Creating Engine... " << std::endl;
    mp_Engine = new openfluid::machine::Engine(*mp_SimBlob, *mp_Model, mp_Listener, mp_IOListener);
    std::cout << "...Engine created" << std::endl;

    std::cout << "******** SIMUL ************" << std::endl;
//    openfluid::base::OutputDescriptor OutputDesc = mp_SimBlob->getOutputDescriptor();

    int FilesSize = m_OutputDesc.getFileSets().size();

    for(int i=0 ; i<FilesSize ; i++)
    {
      openfluid::base::OutputFilesDescriptor & OutputFileDesc = m_OutputDesc.getFileSets()[i];

      std::cout << OutputFileDesc.getColSeparator() << " - " << OutputFileDesc.getDateFormat() << " - " << OutputFileDesc.getCommentChar() << std::endl;

      int SetSize = m_OutputDesc.getFileSets()[i].getSets().size();

      for(int j=0 ; j<SetSize ; j++)
      {
//        openfluid::base::OutputSetDescriptor & OutputSetDesc = m_OutputDesc.getFileSets()[i].getSets()[j];
        openfluid::base::OutputSetDescriptor & OutputSetDesc = OutputFileDesc.getSets()[j];

        std::cout << "  "  << OutputSetDesc.getName() << " : " << OutputSetDesc.getUnitsClass() << " - " << OutputSetDesc.getUnitsIDs().size() << "IDs" << std::endl;
      }
    }
    std::cout << "***************************" << std::endl;

  }
  catch(openfluid::base::OFException& E)
  {
    std::cerr << E.what() << std::endl;

    if(mp_Model)
    {
      delete mp_Model;
      mp_Model = 0;
    }
    if(mp_SimBlob)
    {
      delete mp_SimBlob;
      mp_SimBlob = 0;
    }
//    if(mp_Engine)
//    {
//      delete mp_Engine;
//      mp_Engine = 0;
//    }
    return false;
  }

  // !! delete existing variables from previous checkConsistency !

  // Initializing parameters
  if(! ModelMod->checkModule(mp_Model))
    IsOk = false;

  try
  {
    std::cout << "* Preparing data... " << std::endl;
    mp_Engine->prepareData();
    std::cout << "...Data prepared" << std::endl;

    std::cout << "* Checking consistency... " << std::endl;
    mp_Engine->checkConsistency();
    std::cout << "...Consistency checked" << std::endl;
  }
  catch(openfluid::base::OFException& E)
  {
    std::cerr << E.what() << std::endl;
    IsOk = false;
  }

  return IsOk;
}


// =====================================================================
// =====================================================================


void BuilderProject::actionRun()
{
  if(actionCheckProject())
  {
    // !! delete existing variable values from previous run !

    try
    {
      ResultsModule * ResultsMod = 0;

      Gtk::Dialog * Dialog = new Gtk::Dialog("Simulation Run", true, true);

      Gtk::ScrolledWindow * Win = Gtk::manage(new Gtk::ScrolledWindow);

      Gtk::TextView * TextView = Gtk::manage(new Gtk::TextView);

      Glib::RefPtr<Gtk::TextBuffer> TextBuffer = TextView->get_buffer();

      BuilderMachineListener * Listener = dynamic_cast<BuilderMachineListener*>(mp_Listener);


      Listener->setTextView(TextView);

      TextView->set_editable(false);
      TextView->set_visible(true);
      TextView->set_cursor_visible(false);

      Pango::FontDescription Desc("Monospace");
      TextView->modify_font(Desc);

      Win->set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);
      Win->add(*TextView);

      Dialog->set_default_size(600,400);
      Dialog->get_vbox()->pack_start(*Win);
      Dialog->add_button(Gtk::Stock::OK,Gtk::RESPONSE_OK);


      Dialog->show_all();


      std::ostringstream ss;

      ss << "Simulation from " << mp_Engine->getSimulationInfo()->getStartTime().getAsISOString()
         << " to " << mp_Engine->getSimulationInfo()->getEndTime().getAsISOString() << std::endl
         << "         -> " <<  (mp_Engine->getSimulationInfo()->getStepsCount()) << " time steps of " << mp_Engine->getSimulationInfo()->getTimeStep() << " seconds" << std::endl;

      ss << std::endl;

      Listener->displayText(ss);
      ss.str("");

      ss << std::endl << "**** Running simulation ****" << std::endl;
      boost::posix_time::ptime m_FullStartTime = boost::posix_time::microsec_clock::local_time();

      Listener->displayText(ss);
      ss.str("");

      boost::posix_time::ptime m_EffectiveStartTime = boost::posix_time::microsec_clock::local_time();
      mp_Engine->run();
      boost::posix_time::ptime m_EffectiveEndTime = boost::posix_time::microsec_clock::local_time();

      ss << "**** Simulation completed ****" << std::endl << std::endl;
      ss << std::endl;

      Listener->displayText(ss);
      ss.str("");

      mp_SimBlob->getExecutionMessages().resetWarningFlag();
      openfluid::base::RuntimeEnvironment::getInstance()->setEffectiveSimulationDuration(m_EffectiveEndTime-m_EffectiveStartTime);

      if (openfluid::base::RuntimeEnvironment::getInstance()->isWriteSimReport())
      {
        ss << "* Saving simulation report... ";
        mp_Engine->saveReports();

        Listener->displayText(ss);
        ss.str("");

        ss << "[Done]" << std::endl;

        Listener->displayText(ss);
        ss.str("");

        mp_SimBlob->getExecutionMessages().resetWarningFlag();
      }


      boost::posix_time::ptime m_FullEndTime = boost::posix_time::microsec_clock::local_time();

      if (openfluid::base::RuntimeEnvironment::getInstance()->isWriteResults() || openfluid::base::RuntimeEnvironment::getInstance()->isWriteSimReport())
        ss << std::endl;

      Listener->displayText(ss);
      ss.str("");

      boost::posix_time::time_duration FullSimDuration = m_FullEndTime - m_FullStartTime;


      ss << std::endl;

      ss << "Simulation run time: " << boost::posix_time::to_simple_string(openfluid::base::RuntimeEnvironment::getInstance()->getEffectiveSimulationDuration()) << std::endl;
      ss << "     Total run time: " << boost::posix_time::to_simple_string(FullSimDuration) << std::endl;
      ss << std::endl;

      Listener->displayText(ss);
      ss.str("");

      // after Run !
      ResultsMod = dynamic_cast<ResultsModule*>(getModule("results"));
      ResultsMod->init(m_OutputDesc,&(mp_SimBlob->getCoreRepository()),mp_Engine->getSimulationInfo());


      if(Dialog->run())
      {
        delete Dialog;
        
        Listener->unsetTextView();
        
//        actionDefaultLayout(PostSimulation);
      }
    }
    catch (openfluid::base::OFException& E)
    {
      std::cerr << E.what() << std::endl;
    }
  }

//  if(mp_Model)
//  {
//    delete mp_Model;
//    mp_Model = 0;
//  }
//  if(mp_SimBlob)
//  {
//    delete mp_SimBlob;
//    mp_SimBlob = 0;
//  }
//  if(mp_Engine)
//  {
//    delete mp_Engine;
//    mp_Engine = 0;
//  }

}

