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
  \file StdoutFileOStream.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BASE_STDOUTFILEOSTREAM_HPP__
#define __OPENFLUID_BASE_STDOUTFILEOSTREAM_HPP__

#include <iostream>
#include <fstream>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/null.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>


#include <openfluid/dllexport.hpp>


namespace openfluid { namespace base {


class OPENFLUID_API StdoutAndFileOutputStream
{
  private:

    typedef boost::iostreams::tee_device<std::ostream,std::ofstream> TeeDevice;


  public:

    typedef boost::iostreams::stream<TeeDevice> TeeStream;


  private:

    TeeDevice* mp_LoggerDevice;

    std::ofstream m_FileLogger;

    TeeStream m_Logger;

    void tieStreams();


  public:

    StdoutAndFileOutputStream();

    StdoutAndFileOutputStream(std::string LogFilePath);

    ~StdoutAndFileOutputStream();

    inline std::ofstream& getFile() { return m_FileLogger; };

    inline std::ostream& getStdout() { return std::cout; };

    inline TeeStream& get() { return m_Logger; };

    bool open(std::string LogFilePath);

    void close();

    void flush();

#if defined WIN32
    inline bool isOpened() { return m_FileLogger.is_open(); };
#elif defined __unix__ || defined __APPLE__
    inline bool isOpened() const { return m_FileLogger.is_open(); };
#endif

};




} }  // namespaces


#endif /* __OPENFLUID_BASE_STDOUTFILEOSTREAM_HPP__ */
