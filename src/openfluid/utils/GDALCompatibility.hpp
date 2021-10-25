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
  @file GDALCompatibility.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  
  This file contains macros to help ensuring compatibility 
  between GDAL version 1.xx and 2.xx of this library.
  These macros mainly apply to vector part (OGR) of the library
*/


#ifndef __OPENFLUID_UTILS_GDALCOMPATIBILITY_HPP__
#define __OPENFLUID_UTILS_GDALCOMPATIBILITY_HPP__


#include <gdal.h>


#if !defined(GDAL_VERSION_MAJOR)
  #error "GDAL_VERSION_MAJOR not defined"
#endif


#if (GDAL_VERSION_MAJOR < 2)
  #error "GDAL version 2 or higher is required"
#endif


/**
  Macro for compatibility of vector drivers registering
*/
#define GDALAllRegister_COMPAT() GDALAllRegister()


/**
  Macro for compatibility of vector data source type
*/
#define GDALDataset_COMPAT GDALDataset


/**
  Macro for compatibility of vector driver type
*/
#define GDALDriver_COMPAT GDALDriver


/**
  Macro for compatibility of vector data opening (in read-write mode)
  @param _M_path Path to the file(s) to open
  @return a pointer to a GDALDataset_COMPAT data source
*/
#define GDALOpenRW_COMPAT(_M_path) \
    static_cast<GDALDataset_COMPAT*>(GDALOpenEx(_M_path,GDAL_OF_VECTOR | GDAL_OF_UPDATE,nullptr,nullptr,nullptr));


/**
  Macro for compatibility of vector data opening (in read only mode)
  @param _M_path Path to the file(s) to open
  @return a pointer to a GDALDataset_COMPAT data source
*/
#define GDALOpenRO_COMPAT(_M_path) \
    static_cast<GDALDataset_COMPAT*>(GDALOpenEx(_M_path,GDAL_OF_VECTOR | GDAL_OF_READONLY,nullptr,nullptr,nullptr));


/**
  Macro for compatibility of syncing/flushing a dataset to disk
  @param _M_dataset The dataset
*/
#define GDALFlush_COMPAT(_M_dataset) _M_dataset->FlushCache()


/**
  Macro for compatibility of vector data copy
  @param _M_driver Driver to use for the copy
  @param _M_srcptr Data source to copy
  @param _M_destpath Path to the copied file(s)
  @return a pointer to a GDALDataset_COMPAT data source for the newly copied files
*/
#define GDALCopy_COMPAT(_M_driver,_M_srcptr,_M_destpath) \
    _M_driver->CreateCopy(_M_destpath,_M_srcptr,false,nullptr,nullptr,nullptr)


/**
  Macro for compatibility of vector data creation
  @param _M_driver Driver to use for the creation
  @param _M_path Path to the file(s) to create
  @return a pointer to a GDALDataset_COMPAT data source for the newly created files
*/
#define GDALCreate_COMPAT(_M_driver,_M_path) \
    _M_driver->Create(_M_path,0,0,0,GDT_Unknown,nullptr)


/**
  Macro for compatibility of data source closing
  @param _M_ptr Pointer to the data source to close
*/
#define GDALClose_COMPAT(_M_ptr) GDALClose(_M_ptr);


/**
  Macro for compatibility of physical deletion of vector data
  @param _M_driver Driver to use for the deletion
  @param _M_path Path to the file(s) to delete
*/
#define GDALDelete_COMPAT(_M_driver,_M_path) _M_driver->Delete(_M_path)


/**
  Macro for compatibility of getting a driver by its name
  @param _M_name Name of the driver
  @return a pointer to a GDALDriver_COMPAT driver, NULL if not found
*/
#define GDALGetDriverByName_COMPAT(_M_name) GetGDALDriverManager()->GetDriverByName(_M_name)


/**
  Macro for compatibility of getting a the name of the driver from a driver object
  @param _M_driver The driver object
  @return the name of the driver
*/
#define GDALGetDriverName_COMPAT(_M_driver) _M_driver->GetDescription()


/**
  Macro for compatibility of Integer64 and Integer64List type
*/
#define GDALOFTInteger64_COMPAT OFTInteger64
#define GDALOFTIntegerList64_COMPAT OFTIntegerList64


#endif /* __OPENFLUID_UTILS_GDALCOMPATIBILITY_HPP__ */
