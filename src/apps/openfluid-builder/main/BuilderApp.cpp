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
  \file BuilderApp.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <iostream>

#include <boost/filesystem.hpp>

#include <glibmm/i18n.h>

#include <openfluid/config.hpp>

#include "builderconfig.hpp"

#include "BuilderHelper.hpp"
#include "BuilderApp.hpp"


// =====================================================================
// =====================================================================


BuilderApp::BuilderApp(int argc, char** argv)
{
  Gtk::Main Kit(argc, argv);

  mp_Project = 0;

  try
  {
    mp_Builder = Gtk::Builder::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,MAINWINDOW_UI_FILE));
  }
  catch (const Glib::FileError & E)
  {
    std::cerr << E.what() << std::endl;
  }

  mp_Builder->get_widget("MainWindow",mp_MainWindow);

  if(mp_MainWindow)
  {
    mp_Builder->get_widget("MainMenuBar",mp_MainMenuBar);
    mp_Builder->get_widget("MainToolBar",mp_MainToolBar);
    mp_Builder->get_widget("MainToolBarContainer",mp_MainToolBarContainer);
    mp_Builder->get_widget("ProjectContainer",mp_ProjectContainer);
    mp_Builder->get_widget("ViewportHome",mp_HomeContainer);

    createActions();

    createDock();

    setMainWindowProperties();

    setHomePanelProperties();

    setAboutBoxProperties();


    mp_MainWindow->show_all_children();

    Kit.run(*mp_MainWindow);
  }
}


// =====================================================================
// =====================================================================


BuilderApp::~BuilderApp()
{
  delete mp_MainWindow; // delete all child widgets created with manage() or get_widget()
}


// =====================================================================
// =====================================================================


void BuilderApp::createActions()
{
  /* labels and tooltips for menuitems and toolbuttons linked with actions
   must be define here so they can be translatable
   (action's labels and tooltips cannot be translatables in Glade) */

  // File actions
  Glib::RefPtr<Gtk::Action> ActionNewEmpty;
  Glib::RefPtr<Gtk::Action> ActionNewFrom;
  Glib::RefPtr<Gtk::Action> ActionOpen;
  Glib::RefPtr<Gtk::Action> ActionSave;
  Glib::RefPtr<Gtk::Action> ActionSaveAs;
  Glib::RefPtr<Gtk::Action> ActionClose;
  Glib::RefPtr<Gtk::Action> ActionQuit;
  // Edit actions
  Glib::RefPtr<Gtk::Action> ActionPreferences;
  Glib::RefPtr<Gtk::Action> ActionExtensions;
  // Project actions
  Glib::RefPtr<Gtk::Action> ActionCheckProject;
  Glib::RefPtr<Gtk::Action> ActionRun;
  Glib::RefPtr<Gtk::Action> ActionProperties;
  // View actions
  Glib::RefPtr<Gtk::Action> ActionDefaultLayoutPre;
  Glib::RefPtr<Gtk::Action> ActionDefaultLayoutPost;
  Glib::RefPtr<Gtk::Action> ActionDefaultLayoutAllTabbed;
  Glib::RefPtr<Gtk::Action> ActionLayoutManager;
  // Help actions
  Glib::RefPtr<Gtk::Action> ActionDoc;
  Glib::RefPtr<Gtk::Action> ActionDemo;
  Glib::RefPtr<Gtk::Action> ActionAbout;

  ActionNewEmpty = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionNewEmpty"));
  ActionNewFrom = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionNewFrom"));
  ActionOpen = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionOpen"));
  ActionSave = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionSave"));
  ActionSaveAs = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionSaveAs"));
  ActionProperties = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionProperties"));
  ActionPreferences = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionPreferences"));
  ActionClose = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionClose"));
  ActionQuit = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionQuit"));
  ActionCheckProject = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionCheckProject"));
  ActionRun = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionRun"));
  ActionDefaultLayoutPre = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionDefaultLayoutPre"));
  ActionDefaultLayoutPost = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionDefaultLayoutPost"));
  ActionDefaultLayoutAllTabbed = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionDefaultLayoutAllTabbed"));
  ActionLayoutManager  = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionLayoutManager"));
  ActionExtensions = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionExtensions"));
  ActionDoc = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionDoc"));
  ActionDemo = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionDemo"));
  ActionAbout = Glib::RefPtr<Gtk::Action>::cast_dynamic(mp_Builder->get_object("ActionAbout"));

  ActionNewEmpty->set_label(_("Empty"));
  ActionNewFrom->set_label(_("From..."));
  ActionOpen->set_label(_("_Open..."));
//  ActionSave->set_label(_("_Save")); //existing default translations in gtk-stock
  ActionSaveAs->set_label(_("Save _As..."));
  ActionProperties->set_label(_("Project Properties..."));
  ActionPreferences->set_label(_("OpenFLUID Builder Preferences..."));
//  ActionClose->set_label(); //existing default translations in gtk-stock
//  ActionQuit->set_label(); //existing default translations in gtk-stock
  ActionCheckProject->set_label(_("Check Project"));
  ActionRun->set_label(_("Run Simulation"));
  ActionDefaultLayoutPre->set_label(_("Pre-simulation"));
  ActionDefaultLayoutPost->set_label(_("Post-simulation"));
  ActionDefaultLayoutAllTabbed->set_label(_("All tabbed"));
  ActionLayoutManager->set_label(_("Layout Manager"));
  ActionExtensions->set_label(_("Extensions"));
  ActionDoc->set_label(_("Documentation"));
  ActionDemo->set_label(_("Demo Project"));
  ActionAbout->set_label(_("_About OpenFLUID Builder"));

  ActionNewEmpty->set_tooltip(_("Create new empty project"));
  ActionNewFrom->set_tooltip(_("Create new project from an existing one"));
  ActionOpen->set_tooltip(_("Open an existing project"));
  ActionSave->set_tooltip(_("Save project"));
  ActionCheckProject->set_tooltip(_("Check project"));
  ActionRun->set_tooltip(_("Run simulation"));
  ActionDemo->set_tooltip(_("Open demo project"));

  ActionNewEmpty->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionNewEmpty));
  ActionNewFrom->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionNewFrom));
  ActionOpen->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionOpen));
  ActionSave->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionSave));
  ActionSaveAs->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionSaveAs));
  ActionProperties->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionProperties));
  ActionPreferences->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionPreferences));
  ActionClose->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionClose));
//  ActionQuit->signal_activate().connect(sigc::ptr_fun(&Gtk::Main::quit));
  ActionQuit->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionQuit));
  ActionCheckProject->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionCheckProject));
  ActionRun->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionRun));
  ActionDefaultLayoutPre->signal_activate().connect(sigc::bind<BuilderProject::LayoutType>(sigc::mem_fun(*this,&BuilderApp::actionDefaultLayout),BuilderProject::PreSimulation));
  ActionDefaultLayoutPost->signal_activate().connect(sigc::bind<BuilderProject::LayoutType>(sigc::mem_fun(*this,&BuilderApp::actionDefaultLayout),BuilderProject::PostSimulation));
  ActionDefaultLayoutAllTabbed->signal_activate().connect(sigc::bind<BuilderProject::LayoutType>(sigc::mem_fun(*this,&BuilderApp::actionDefaultLayout),BuilderProject::AllTabbed));
  ActionLayoutManager->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionLayoutManager));
  ActionExtensions->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionPlugins));
  ActionDoc->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionDoc));
  ActionDemo->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionDemo));
  ActionAbout->signal_activate().connect(sigc::mem_fun(*this,&BuilderApp::actionAbout));

  // add customized icons
  Gtk::StockID * NewDerivedStockId = BuilderHelper::createIconStockId(BUILDER_RESOURCE_PATH, "document-new-derived.svg", "builder-new-derived");
  ActionNewFrom->set_stock_id(*NewDerivedStockId);

  m_ProjectActions.push_back(ActionSave);
  m_ProjectActions.push_back(ActionSaveAs);
  m_ProjectActions.push_back(ActionProperties);
  m_ProjectActions.push_back(ActionClose);
  m_ProjectActions.push_back(ActionCheckProject);
  m_ProjectActions.push_back(ActionRun);
  m_ProjectActions.push_back(ActionDefaultLayoutPre);
  m_ProjectActions.push_back(ActionDefaultLayoutPost);
  m_ProjectActions.push_back(ActionDefaultLayoutAllTabbed);

  for(unsigned int i=0 ; i<m_ProjectActions.size() ; i++)
    m_ProjectActions[i]->set_sensitive(false);

}


// =====================================================================
// =====================================================================


void BuilderApp::createDock()
{
  Gdl::init();

  // create Dock
  mp_MainDock = new Gdl::Dock;

  // create DockBar
  Gdl::DockBar * MainDockBar = new Gdl::DockBar(*mp_MainDock);

  createDockItemHome();

  // fill top-level box widget with dock elements
  mp_ProjectContainer->pack_start(*MainDockBar,false,false);
  mp_ProjectContainer->pack_end(*mp_MainDock);

  Glib::RefPtr<Gdl::DockMaster> DockMaster = mp_MainDock->property_master();
  DockMaster->property_switcher_style() = Gdl::SWITCHER_STYLE_TABS;

  // create Layout Manager
  mp_LayoutManager = Gdl::DockLayout::create(*mp_MainDock);

  // create placeholders
  gdl_dock_placeholder_new("ph1", GDL_DOCK_OBJECT((*mp_MainDock).gobj()), GDL_DOCK_TOP, FALSE);
  gdl_dock_placeholder_new("ph2", GDL_DOCK_OBJECT((*mp_MainDock).gobj()), GDL_DOCK_BOTTOM, FALSE);
  gdl_dock_placeholder_new("ph3", GDL_DOCK_OBJECT((*mp_MainDock).gobj()), GDL_DOCK_LEFT, FALSE);
  gdl_dock_placeholder_new("ph4", GDL_DOCK_OBJECT((*mp_MainDock).gobj()), GDL_DOCK_RIGHT, FALSE);

}


// =====================================================================
// =====================================================================

void BuilderApp::createDockItemHome()
{
  mp_DockItemHome = new Gdl::DockItem("Home",
      Glib::ustring::compose(_("%1Home"),"  "),
      Gtk::Stock::HOME,
      Gdl::DOCK_ITEM_BEH_NORMAL | Gdl::DOCK_ITEM_BEH_CANT_CLOSE);


  // fill DockItem with widget
  mp_DockItemHome->add(*mp_HomeContainer);

  // fill Dock with DockItem
  mp_MainDock->add_item(*mp_DockItemHome,Gdl::DOCK_TOP);

}

// =====================================================================
// =====================================================================


void BuilderApp::setMainWindowProperties()
{
  // set icon for all windows

  Glib::RefPtr<Gdk::Pixbuf> OF_logo = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/openfluid_image_only.svg",BUILDER_RESOURCE_PATH));

  mp_MainWindow->set_default_icon(OF_logo);


  // set main window size (have to set glade.window.visible=false to make this work)

  int MonitorWidth = Gdk::Screen::get_default()->get_width();
  int MonitorHeight = Gdk::Screen::get_default()->get_height();

  mp_MainWindow->set_default_size(MonitorWidth-(MonitorWidth*0.1),
      MonitorHeight-(MonitorHeight*0.1));

}


// =====================================================================
// =====================================================================


void BuilderApp::setAboutBoxProperties()
{
  Gtk::AboutDialog * DialogBox = 0;

  mp_Builder->get_widget("DialogAbout",DialogBox);

  DialogBox->set_comments("OpenFLUID v" + openfluid::config::FULL_VERSION
      + "\n\nLISAH, Montpellier, France");

}


// =====================================================================
// =====================================================================


void BuilderApp::setHomePanelProperties()
{

  // Buttons for common actions

  Gtk::ButtonBox * ButtonBoxHome = 0;

  mp_Builder->get_widget("ButtonBoxHome",ButtonBoxHome);

  Gtk::Box_Helpers::BoxList ButtonList = ButtonBoxHome->children();

  for(Gtk::Box_Helpers::BoxList::iterator it = ButtonList.begin(); it != ButtonList.end(); ++it)
  {
    Gtk::Button * Button = (Gtk::Button *)(it->get_widget());

    Button->set_size_request(350,65);

    Button->remove(); // remove existing automatic added default label


    Gtk::HBox * BoxInsideButton = Gtk::manage(new Gtk::HBox());

    Gtk::Image * Image = Gtk::manage(new Gtk::Image(Button->get_action()->get_stock_id(),
                                                    Gtk::ICON_SIZE_DIALOG));

    Gtk::Label * Label = Gtk::manage(new Gtk::Label(Button->get_action()->get_tooltip(),
                                                    Gtk::ALIGN_LEFT));

    Label->set_line_wrap(true);


    BoxInsideButton->pack_start(*Image, false, false, 10);

    BoxInsideButton->pack_start(*Label);


    Button->add(*BoxInsideButton);
  }  // end Buttons


  // Home "head" panel

  Gtk::Box * BoxHomeHead = 0;

  mp_Builder->get_widget("BoxHomeHead",BoxHomeHead);


      // Openfluid image

  Glib::RefPtr<Gdk::Pixbuf> PixBuf = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/openfluid_official.svg",BUILDER_RESOURCE_PATH),
                                                                    320, -1, true);

  Gtk::Image * Image = Gtk::manage(new Gtk::Image(PixBuf));

  BoxHomeHead->pack_start(*Image, false, 10);

      // Openfluid version

  Gtk::Label * LabelVersion = Gtk::manage(new Gtk::Label("OpenFLUID v" + openfluid::config::FULL_VERSION
        + "\n\nLISAH, Montpellier, France"));

  BoxHomeHead->pack_start(*LabelVersion, false, 10);

      // Openfluid web link

  Gtk::Label * LabelLink = Gtk::manage(new Gtk::Label());

  LabelLink->set_markup("<a href=\"http://www.umr-lisah.fr/openfluid/\">http://www.umr-lisah.fr/openfluid/</a>");

  BoxHomeHead->pack_start(*LabelLink, false, 10);

}


// =====================================================================
// =====================================================================


void BuilderApp::actionNewEmpty()
{
  if(mp_Project)
  {
    actionClose();
  }

  createProject();
}


// =====================================================================
// =====================================================================


void BuilderApp::actionNewFrom()
{
  actionNewEmpty();
}


// =====================================================================
// =====================================================================


void BuilderApp::actionOpen()
{
  Glib::ustring Folder ="";

  Gtk::FileChooserDialog * DialogOpen;
  mp_Builder->get_widget("DialogOpen",DialogOpen);

  // add Buttons before the first show() call, because glade's buttons don't work in FileChooserDialog
  if(! DialogOpen->is_realized())
  {
    DialogOpen->add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
    DialogOpen->add_button(Gtk::Stock::OPEN,Gtk::RESPONSE_OK);
    /* TODO: set default folder from Preferences */
    DialogOpen->set_current_folder(openfluid::base::RuntimeEnvironment::getInstance()->getInputDir()+"/..");
  }

  int ok = 0;

  while(ok == 0)
  {
    int response = DialogOpen->run();

    if(response == Gtk::RESPONSE_DELETE_EVENT || response == Gtk::RESPONSE_CANCEL)
    {
      DialogOpen->hide();
      ok =1;
    }
    else if (response == Gtk::RESPONSE_OK)
    {
      // TODO: Change FileChooserDialog action from "Select folder" to "Open" ? (Gnome Bug 593385)
      Folder = DialogOpen->get_filename();

      if(Folder != "")
      {
        //      DialogOpen->hide();
        try
        {
          mp_Project = new BuilderProject();
          DialogOpen->hide();

          createProject();
          ok = 1;
        }
        catch (openfluid::base::OFException& E)
        {
          Gtk::MessageDialog * DialogError = 0;
          mp_Builder->get_widget("DialogError",DialogError);
          DialogError->set_message(E.what());

          if(DialogError->run())
          {
            DialogError->hide();
            delete DialogError; // utile ?
          }
        }
      }
    }
  }

}


// =====================================================================
// =====================================================================


void BuilderApp::actionSave()
{
//  mp_Layout_manager->save_to_file(builderconfig::RESOURCE_PATH+"/default_layout.xml");
}


// =====================================================================
// =====================================================================


void BuilderApp::actionSaveAs()
{

}


// =====================================================================
// =====================================================================


void BuilderApp::actionClose()
{
  deleteProject(); // ask for saving
}


// =====================================================================
// =====================================================================


void BuilderApp::actionQuit()
{
  actionClose();
  Gtk::Main::quit(); // ask for confirmation ?
}

// =====================================================================
// =====================================================================


void BuilderApp::actionCheckProject()
{

}


// =====================================================================
// =====================================================================


void BuilderApp::actionRun()
{

}


// =====================================================================
// =====================================================================


void BuilderApp::actionProperties()
{

}


// =====================================================================
// =====================================================================


void BuilderApp::actionPreferences()
{

}


// =====================================================================
// =====================================================================


void BuilderApp::actionDefaultLayout(BuilderProject::LayoutType Layout)
{
  if(mp_Project)
  {
    // first "de-iconify" all

    Glib::ListHandle<Gdl::DockItem *> ItemList = mp_MainDock->get_named_items();

    for(Glib::ListHandle<Gdl::DockItem *>::const_iterator it = ItemList.begin(); it != ItemList.end(); ++it)
    {
      Gdl::DockItem * Item = *it;
      Item->show_item();
      Item->hide_item();
    }


    // "re-"add Home item at right place

    mp_MainDock->add_item(*mp_DockItemHome,Gdl::DOCK_TOP);


    // "re"-add Top item at right place

    Gdl::DockItem * DockItemTop = mp_Project->getTopDockItem();

    mp_MainDock->add_item(*DockItemTop,Gdl::DOCK_BOTTOM);


    // reorder all

    if(Layout == BuilderProject::AllTabbed)
    {
      DockItemTop->dock_to(*mp_DockItemHome,Gdl::DOCK_CENTER);
    }
    else
    {
      mp_DockItemHome->iconify_item();
    }

    mp_Project->actionDefaultLayout(Layout);
  }

}


// =====================================================================
// =====================================================================


void BuilderApp::actionLayoutManager()
{
  mp_LayoutManager->run_manager();
}


// =====================================================================
// =====================================================================


void BuilderApp::actionPlugins()
{

}


// =====================================================================
// =====================================================================


void BuilderApp::actionDoc()
{

}


// =====================================================================
// =====================================================================


void BuilderApp::actionDemo()
{

}


// =====================================================================
// =====================================================================


void BuilderApp::actionAbout()
{
  Gtk::Dialog * DialogAbout;
  mp_Builder->get_widget("DialogAbout",DialogAbout);

  DialogAbout->show();

  int response = DialogAbout->run();

  if(response == Gtk::RESPONSE_DELETE_EVENT || response == Gtk::RESPONSE_CANCEL)
    DialogAbout->hide();
}


// =====================================================================
// =====================================================================


void BuilderApp::createProject()
{
  mp_Project = new BuilderProject();

  actionDefaultLayout(BuilderProject::PreSimulation);


  // add modules elements

  BuilderProject::ModulesPtrByNameMap_t Modules = mp_Project->getModules();

  for(BuilderProject::ModulesPtrByNameMap_t::const_iterator it = Modules.begin() ; it!= Modules.end() ; ++it)
  {
    ModuleInterface * Module = it->second;

    // add Menu to MenuBar
    if(Gtk::Menu * Menu = Module->getMenu())
    {
      Menu->detach(); // detach the Menu from glade's MenuBar

      Gtk::Menu_Helpers::MenuList::iterator It = mp_MainMenuBar->items().end();

      It --; It --; // to put the Menu at 2d next-to-last position in MenuBar

      mp_MainMenuBar->items().insert(It,Gtk::Menu_Helpers::MenuElem(Module->getModuleName(), *Menu));
    }

    // add ToolBar to a HandleBox
    if(Gtk::Toolbar * ToolBar = Module->getToolBar())
    {
      Gtk::HandleBox * HandleBox = new Gtk::HandleBox();

      HandleBox->add(*ToolBar);

      mp_MainToolBarContainer->pack_start(*HandleBox,false,false);

      HandleBox->set_visible(true);

      ToolBar->set_show_arrow(false);
    }

  }


  for(unsigned int i=0 ; i<m_ProjectActions.size() ; i++)
    m_ProjectActions[i]->set_sensitive(true);

}


// =====================================================================
// =====================================================================


void BuilderApp::deleteProject()
{
  // delete modules elements

  BuilderProject::ModulesPtrByNameMap_t Modules = mp_Project->getModules();

  for(BuilderProject::ModulesPtrByNameMap_t::const_iterator it = Modules.begin() ; it!= Modules.end() ; ++it)
  {
    ModuleInterface * Module = it->second;

    // delete Menu
    if(Module->getMenu())
    {
      Gtk::Menu_Helpers::MenuList::iterator It = mp_MainMenuBar->items().end();

      It --; It --; // remove the Menu at 2d next-to-last position in MenuBar

      mp_MainMenuBar->items().erase(--It);
    }

    // delete ToolBar HandleBox
    if(Gtk::Toolbar * ToolBar = Module->getToolBar())
    {
      mp_MainToolBarContainer->remove(*(ToolBar->get_parent()));
    }

  }


  for(unsigned int i=0 ; i<m_ProjectActions.size() ; i++)
    m_ProjectActions[i]->set_sensitive(false);

  if(!mp_DockItemHome->is_visible())
    mp_DockItemHome->show_item();

  delete mp_Project;
  mp_Project = 0;

}
