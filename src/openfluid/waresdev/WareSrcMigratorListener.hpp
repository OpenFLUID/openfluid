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
  @file WareSrcMigratorListener.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#ifndef __OPENFLUID_WARESDEV_WARESRCMIGRATORLISTENER_HPP__
#define __OPENFLUID_WARESDEV_WARESRCMIGRATORLISTENER_HPP__


#include <openfluid/waresdev/WareSrcOpsListener.hpp>


namespace openfluid { namespace waresdev {


class WareSrcMigratorListener : virtual public WareSrcOpsListener
{
  public:

    CPPCLASS_DEFAULT_FIVE(WareSrcMigratorListener) 

    virtual void onPrecheckStart()
    { }

    virtual void onPrecheckEnd(openfluid::base::Listener::Status /*Status*/)
    { }

    virtual void onPrepareStart()
    { }

    virtual void onPrepareEnd(openfluid::base::Listener::Status /*Status*/)
    { }

    virtual void onProcessSignatureStart()
    { }

    virtual void onProcessSignatureEnd(openfluid::base::Listener::Status /*Status*/)
    { }

    virtual void onProcessCMakeConfigStart()
    { }

    virtual void onProcessCMakeConfigEnd(openfluid::base::Listener::Status /*Status*/)
    { }

    virtual void onCreateStructureStart()
    { }

    virtual void onCreateStructureEnd(openfluid::base::Listener::Status /*Status*/)
    { }

    virtual void onDispatchFilesStart()
    { }

    virtual void onDispatchFilesEnd(openfluid::base::Listener::Status /*Status*/)
    { }

    virtual void onCleanSrcFilesStart()
    { }

    virtual void onCleanSrcFilesEnd(openfluid::base::Listener::Status /*Status*/)
    { }

    virtual void onDocStart()
    { }

    virtual void onDocEnd(openfluid::base::Listener::Status /*Status*/)
    { }

};


} } //namespaces


#endif /* __OPENFLUID_WARESDEV_WARESRCMIGRATORLISTENER_HPP__ */
