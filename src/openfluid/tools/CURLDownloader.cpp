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
  \file CURLDownloader.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <openfluid/tools/CURLDownloader.hpp>
#include <curl/curl.h>
#include <stdio.h>


// =====================================================================
// =====================================================================


namespace openfluid { namespace tools {


int CURLDownloader::CURLWriterToBuffer(char *Data, size_t Size, size_t NMemb, std::string *BufferIn)
{
  /*
    http://projectivemotion.com/news/using-libcurl-with-c-simple-oop-static-class-exampleusando-libcurl-en-c-ejemplo-de-oop
  */

  // Is there anything in the buffer?
  if (BufferIn != NULL)
  {
    // Append the data to the buffer
    BufferIn->append(Data, Size * NMemb);

    // How much did we write?
    return Size * NMemb;
  }

  return 0;
}


// =====================================================================
// =====================================================================


int CURLDownloader::CURLWriterToFile(void *Ptr, size_t Size, size_t NMemb, FILE *FileStream)
{
  /*
    http://stackoverflow.com/questions/1636333/download-file-using-libcurl-in-c-c
  */

  if (Ptr != NULL)
  {

    size_t Written;
    Written = fwrite(Ptr, Size, NMemb, FileStream);
    return Written;

  }

  return 0;
}


// =====================================================================
// =====================================================================


CURLDownloader::ErrorCode CURLDownloader::downloadToFile(const std::string URL,
                                                             const std::string FilePath)
{
  /*
    http://stackoverflow.com/questions/1636333/download-file-using-libcurl-in-c-c
  */

  CURL *Curl;
  CURLcode Result;
  char ErrorBuffer[CURL_ERROR_SIZE];
  FILE* FP;

  Curl = curl_easy_init();
  if (Curl)
  {
    FP = fopen(FilePath.c_str(),"wb");
    curl_easy_setopt(Curl, CURLOPT_ERRORBUFFER, ErrorBuffer);
    curl_easy_setopt(Curl, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(Curl, CURLOPT_HEADER, 0);
    curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION, CURLDownloader::CURLWriterToFile);
    curl_easy_setopt(Curl, CURLOPT_WRITEDATA, FP);
    curl_easy_setopt(Curl, CURLOPT_FAILONERROR, 1);

    Result = curl_easy_perform(Curl);
    curl_easy_cleanup(Curl);
    fclose(FP);
  }

  if (Result == CURLE_OK) return CURLDownloader::ErrorCode(NO_ERROR);

  return CURLDownloader::ErrorCode(CURL_ERROR);
}


// =====================================================================
// =====================================================================


CURLDownloader::ErrorCode CURLDownloader::downloadToString(const std::string URL,
                                                               std::string& Contents)
{
  /*
    http://projectivemotion.com/news/using-libcurl-with-c-simple-oop-static-class-exampleusando-libcurl-en-c-ejemplo-de-oop
  */

  CURL *Curl;
  CURLcode Result;
  char ErrorBuffer[CURL_ERROR_SIZE];

  Contents.clear();

  Curl = curl_easy_init();


  if (Curl)
  {
    curl_easy_setopt(Curl, CURLOPT_ERRORBUFFER, ErrorBuffer);
    curl_easy_setopt(Curl, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(Curl, CURLOPT_HEADER, 0);
    curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION, CURLDownloader::CURLWriterToBuffer);
    curl_easy_setopt(Curl, CURLOPT_WRITEDATA, &Contents);
    curl_easy_setopt(Curl, CURLOPT_FAILONERROR, 1);

    Result = curl_easy_perform(Curl);
    curl_easy_cleanup(Curl);
  }

  if (Result == CURLE_OK) return CURLDownloader::ErrorCode(NO_ERROR);

  return CURLDownloader::ErrorCode(CURL_ERROR);

}

// =====================================================================
// =====================================================================



} } // namespaces

