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

  mp_Builder = Gtk::Builder::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,MAINWINDOW_UI_FILE));

  mp_Builder->get_widget("MainWindow",mp_MainWindow);

  if(mp_MainWindow)
  {
    mp_Builder->get_widget("MainMenuBar",mp_MainMenuBar);
    mp_Builder->get_widget("MainToolBar",mp_MainToolBar);
    mp_Builder->get_widget("MainStatusBar",mp_MainStatusBar);
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
  delete mp_Project;
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
  // create Dock
  mp_MainDock = gdl_dock_new();

  // create Layout Manager
  mp_DockLayout = gdl_dock_layout_new(GDL_DOCK(mp_MainDock));

  // create DockBar
  GtkWidget * DockBar = gdl_dock_bar_new(GDL_DOCK(mp_MainDock));

  createDockItemHome();

  // fill top-level box widget with dock elements
  mp_ProjectContainer->pack_start(*Glib::wrap(DockBar),false,false);
  mp_ProjectContainer->pack_end(*Glib::wrap(mp_MainDock));

  GdlDockMaster * DockMaster = GDL_DOCK_OBJECT_GET_MASTER(mp_MainDock);
  g_object_set(DockMaster, "switcher-style", GDL_SWITCHER_STYLE_TABS, NULL);

  // create placeholders
  gdl_dock_placeholder_new("ph1", GDL_DOCK_OBJECT(mp_MainDock), GDL_DOCK_TOP, FALSE);
  gdl_dock_placeholder_new("ph2", GDL_DOCK_OBJECT(mp_MainDock), GDL_DOCK_BOTTOM, FALSE);
  gdl_dock_placeholder_new("ph3", GDL_DOCK_OBJECT(mp_MainDock), GDL_DOCK_LEFT, FALSE);
  gdl_dock_placeholder_new("ph4", GDL_DOCK_OBJECT(mp_MainDock), GDL_DOCK_RIGHT, FALSE);

}


// =====================================================================
// =====================================================================

void BuilderApp::createDockItemHome()
{
  mp_DockItemHome = gdl_dock_item_new_with_stock("Home",Glib::ustring::compose(_("%1Home"),"  ").c_str(),GTK_STOCK_HOME, GDL_DOCK_ITEM_BEH_CANT_CLOSE);

  // fill DockItem with widget
  gtk_container_add(GTK_CONTAINER(mp_DockItemHome), GTK_WIDGET(mp_HomeContainer->gobj()));

  // fill Dock with DockItem
  gdl_dock_add_item(GDL_DOCK(mp_MainDock), GDL_DOCK_ITEM(mp_DockItemHome), GDL_DOCK_TOP);

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

  Glib::RefPtr<Gdk::Pixbuf> PixBuf = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,OPENFLUID_LOGO_FILE),
                                                                      230, -1, true);
  DialogBox->set_logo(PixBuf);

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

  Glib::RefPtr<Gdk::Pixbuf> PixBuf = Gdk::Pixbuf::create_from_file(Glib::ustring::compose("%1/%2",BUILDER_RESOURCE_PATH,OPENFLUID_LOGO_FILE),
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
  if(mp_Project)
  {
    actionClose();
  }

  Gtk::Dialog * DialogNewFrom = 0;
  Gtk::FileChooserWidget * FileChooser = 0;
  Gtk::CheckButton * CbModel = 0;
  Gtk::CheckButton * CbModelParameters = 0;
  Gtk::CheckButton * CbDomain = 0;
  Gtk::CheckButton * CbDomainInputData = 0;;
  Gtk::CheckButton * CbDomainEvents = 0;
  Gtk::CheckButton * CbConfig = 0;
  Gtk::CheckButton * CbConfigRun = 0;
  Gtk::CheckButton * CbConfigOutput = 0;

  mp_Builder->get_widget("DialogNewFrom",DialogNewFrom);
  mp_Builder->get_widget("FileChooserDialogNewFrom",FileChooser);
  mp_Builder->get_widget("CbModel",CbModel);
  mp_Builder->get_widget("CbModelParameters",CbModelParameters);
  mp_Builder->get_widget("CbDomain",CbDomain);
  mp_Builder->get_widget("CbDomainInputData",CbDomainInputData);
  mp_Builder->get_widget("CbDomainEvents",CbDomainEvents);
  mp_Builder->get_widget("CbConfig",CbConfig);
  mp_Builder->get_widget("CbConfigRun",CbConfigRun);
  mp_Builder->get_widget("CbConfigOutput",CbConfigOutput);

  /* TODO: set default folder from Preferences */
  FileChooser->set_current_folder(openfluid::base::RuntimeEnvironment::getInstance()->getInputDir()+"/..");

  std::vector<Gtk::CheckButton*> CbModelDepends;
  std::vector<Gtk::CheckButton*> CbDomainDepends;
  std::vector<Gtk::CheckButton*> CbConfigDepends;

  CbModelDepends.push_back(CbModelParameters);
  CbDomainDepends.push_back(CbDomainInputData);
  CbDomainDepends.push_back(CbDomainEvents);
  CbConfigDepends.push_back(CbConfigRun);
  CbConfigDepends.push_back(CbConfigOutput);

  CbModel->signal_toggled().connect(sigc::bind<Gtk::CheckButton*,std::vector<Gtk::CheckButton*> >
                                      (sigc::mem_fun(*this,&BuilderApp::onDialogNewFromCbToggled),CbModel,CbModelDepends)
                                      );
  CbDomain->signal_toggled().connect(sigc::bind<Gtk::CheckButton*,std::vector<Gtk::CheckButton*> >
                                      (sigc::mem_fun(*this,&BuilderApp::onDialogNewFromCbToggled),CbDomain,CbDomainDepends)
                                      );
  CbConfig->signal_toggled().connect(sigc::bind<Gtk::CheckButton*,std::vector<Gtk::CheckButton*> >
                                      (sigc::mem_fun(*this,&BuilderApp::onDialogNewFromCbToggled),CbConfig,CbConfigDepends)
                                      );

  CbModel->set_active(true);
  CbDomain->set_active(true);
  CbConfig->set_active(true);

  if(DialogNewFrom->run() == Gtk::RESPONSE_OK)
  {
    createProject(FileChooser->get_filename()/*, NEW */);
  }

  DialogNewFrom->hide();
}


// =====================================================================
// =====================================================================


void BuilderApp::actionOpen()
{
  if(mp_Project)
  {
    actionClose();
  }

  Gtk::FileChooserDialog  DialogOpen(_("Select existing project folder"),Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

  /* TODO: set default folder from Preferences */
  DialogOpen.set_current_folder(openfluid::base::RuntimeEnvironment::getInstance()->getInputDir()+"/..");

  DialogOpen.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
  DialogOpen.add_button(Gtk::Stock::OPEN,Gtk::RESPONSE_OK);

  if(DialogOpen.run() == Gtk::RESPONSE_OK)
  {
    createProject(DialogOpen.get_filename());
  }

  DialogOpen.hide();
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
  mp_Project->actionCheckProject();
}


// =====================================================================
// =====================================================================


void BuilderApp::actionRun()
{
  mp_Project->actionRun();
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

    GList * ItemListC = gdl_dock_get_named_items(GDL_DOCK(mp_MainDock));

    for(unsigned int i=0 ; i<g_list_length(ItemListC) ; i++)
    {
      if(GDL_DOCK_ITEM_ICONIFIED(GDL_DOCK_ITEM(g_list_nth_data(ItemListC,i))))
          gdl_dock_item_show_item(GDL_DOCK_ITEM(g_list_nth_data(ItemListC,i)));
      gdl_dock_item_hide_item(GDL_DOCK_ITEM(g_list_nth_data(ItemListC,i)));
    }


    // "re-add" Home item at right place

    gdl_dock_add_item(GDL_DOCK(mp_MainDock), GDL_DOCK_ITEM(mp_DockItemHome), GDL_DOCK_TOP);


    // "re-add" Top item at right place

    GtkWidget * DockItemTop = mp_Project->getTopDockItem();

    gdl_dock_add_item(GDL_DOCK (mp_MainDock), GDL_DOCK_ITEM(DockItemTop), GDL_DOCK_BOTTOM);


    // reorder all

    if(Layout == BuilderProject::AllTabbed)
    {
      gdl_dock_item_dock_to(GDL_DOCK_ITEM(DockItemTop), GDL_DOCK_ITEM(mp_DockItemHome), GDL_DOCK_CENTER, -1);
    }
    else
    {
      gdl_dock_item_iconify_item(GDL_DOCK_ITEM(mp_DockItemHome));
    }
    mp_Project->actionDefaultLayout(Layout);
  }

}


// =====================================================================
// =====================================================================


void BuilderApp::actionLayoutManager()
{
  gdl_dock_layout_run_manager (mp_DockLayout);
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

  if(DialogAbout->run())
    DialogAbout->hide();
}


// =====================================================================
// =====================================================================


void BuilderApp::createProject(Glib::ustring FolderIn)
{
  try
  {
    mp_Project = new BuilderProject(FolderIn);

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

      // add ToolItems
      std::vector<Gtk::ToolItem *> ToolItems = Module->getToolItems();

      if(!ToolItems.empty())
      {
        mp_MainToolBar->add(*(Gtk::manage(new Gtk::SeparatorToolItem())));

        for(unsigned int i=0 ; i<ToolItems.size() ; i++)
        {
          mp_MainToolBar->add(*(ToolItems[i]));
        }

        mp_MainToolBar->show_all();
      }

      // add Status widgets
      if(Gtk::Widget * StatusBarWidget = Module->getStatusBarWidget())
      {
        mp_MainStatusBar->pack_start(*StatusBarWidget,false,false,10);
        mp_MainStatusBar->show_all_children();
      }

    }


    for(unsigned int i=0 ; i<m_ProjectActions.size() ; i++)
      m_ProjectActions[i]->set_sensitive(true);

  }
  catch (openfluid::base::OFException& E)
  {
    Gtk::MessageDialog DialogError(E.what(),false,Gtk::MESSAGE_ERROR);

    if(DialogError.run())
    {
      DialogError.hide();
    }
  }

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

    // remove ToolItems
    std::vector<Gtk::ToolItem *> ToolItems = Module->getToolItems();

    if(!ToolItems.empty())
    {
      for(unsigned int i=0 ; i<ToolItems.size() ; i++)
      {
        mp_MainToolBar->remove(*(ToolItems[i]));
      }

    }

    // remove remaining separators
    while(Gtk::SeparatorToolItem * Sep = dynamic_cast<Gtk::SeparatorToolItem *>(mp_MainToolBar->get_nth_item(mp_MainToolBar->get_n_items()-1)))
    {
      mp_MainToolBar->remove(*Sep);
    }

    // remove StatusBar widget
    if(Gtk::Widget * StatusBarWidget = Module->getStatusBarWidget())
      mp_MainStatusBar->remove(*StatusBarWidget);

  }


  for(unsigned int i=0 ; i<m_ProjectActions.size() ; i++)
    m_ProjectActions[i]->set_sensitive(false);

  if(!Glib::wrap(mp_DockItemHome)->is_visible())
    gdl_dock_item_show_item(GDL_DOCK_ITEM (mp_DockItemHome));

  delete mp_Project;
  mp_Project = 0;

}


// =====================================================================
// =====================================================================


void BuilderApp::onDialogNewFromCbToggled(Gtk::CheckButton* Cb,std::vector<Gtk::CheckButton*> CbDepends)
{
  for(unsigned int i=0 ; i<CbDepends.size() ; i++)
  {
    CbDepends[i]->set_sensitive(Cb->get_active());
    CbDepends[i]->set_active(Cb->get_active());
  }
}
