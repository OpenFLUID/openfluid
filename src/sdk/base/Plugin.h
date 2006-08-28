/**
  \file Plugin.h
  \brief header file for generic plugin interface definition

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>

  The plugin class defines the minimal structure for a plugin
  ALL PLUGINS MUST INHERIT FROM THIS CLASS TO BE LOADED
  It sets some essential methods for the plugin
  The PluginProc type defines the handle function for the plugin integration
  into the host application. Every plugin must have the function:
  \code
  extern "C"
  {
    DLLIMPORT Plugin* GetM2k5Plugin(wxString SelfPath);
  };
  \endcode
  returning an instance of the plugin class, inherited from the class defined here.
*/


#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include <wx/wx.h>
#include "Function.h"

// =====================================================================
// =====================================================================


#ifdef WIN32
  #ifdef BUILDPLUGDLL
    #define DLLIMPORT __declspec(dllexport)
  #else
    #define DLLIMPORT __declspec(dllimport)
  #endif
  #define CALL __stdcall
#else
  #define DLLIMPORT
  #define CALL
#endif


// =====================================================================
// =====================================================================

#define PLUGIN_PROC_NAME "GetMHYDASPlugin"

// =====================================================================
// =====================================================================



namespace mhydasdk { namespace base {


/**
  Plugin function list
*/
enum FunctionTypeList
{
  FUNC_SU_PRODUCTION,
  FUNC_SU_TRANSFER,
  FUNC_RS_TRANSFER,
  FUNC_RSGU_EXCHANGE,
  FUNC_SUGU_EXCHANGE
};


/**
  Plugin modules list
*/
enum ModuleTypeList
{
  MOD_HYDROLOGY
};




// =====================================================================
// =====================================================================

/**
  Structure encapsulating the plugin signature,
  returned from the plugin to the host app for registering
*/
struct Signature
{

  /**
    Plugin function type
  */
  FunctionTypeList FunctionType;

  /**
    Plugin module type
  */
  ModuleTypeList ModuleType;

  /**
    Plugin identity
  */
  wxString ID;

  /**
    Plugin name
  */
  wxString Name;

    /**
    Major version number
  */
  wxString MajorVersion;

  /**
    Minor version number
  */
  wxString MinorVersion;

  /**
    Author's name
  */
  wxString Author;

  /**
    Author's email
  */
  wxString AuthorEmail;
};



// =====================================================================
// =====================================================================


/**
  Abstact class for plugin interface

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class Plugin : public wxObject
{

  protected:


  public:
    /**
      Constructor
    */
    Plugin();

    /**
      Virtual destructor
    */
    virtual ~Plugin();

    /**
      Returns the plugin signature
      \return \link Signature Signature \endlink
    */
    virtual Signature getSignature()=0;

    /**
      Returns the plugin function
      \return Function
    */
    virtual Function *getFunction()=0;


};


// =====================================================================
// =====================================================================

typedef Plugin*(*GetPluginProc)();


} } // namespace mhydasdk::base



#endif
