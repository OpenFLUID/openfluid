SET(OPENFLUID_PACKAGE_TITLE "OpenFLUID")
SET(OPENFLUID_PACKAGE_SUMMARY "OpenFLUID software environment for modelling landscapes functionning")
SET(OPENFLUID_PACKAGE_DESCRIPTION "This package contains the OpenFLUID software environment for modelling landscapes functionning, mainly for spatial fluxes modelling. It also includes API and tools for development.")
SET(OPENFLUID_PACKAGE_LICENSE "GPLv3 (with special exception)")

SET(CPACK_CMAKE_GENERATOR "${CMAKE_GENERATOR}")

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/resources/packaging/readme.txt ${CMAKE_BINARY_DIR}/readme.txt COPYONLY)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/COPYING ${CMAKE_BINARY_DIR}/copyright.txt COPYONLY)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/resources/packaging/welcome.txt ${CMAKE_BINARY_DIR}/welcome.txt COPYONLY)


SET(CPACK_INSTALL_CMAKE_PROJECTS "${PROJECT_BINARY_DIR};${PROJECT_NAME};ALL;/") 

SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_BINARY_DIR}/copyright.txt")
SET(CPACK_RESOURCE_FILE_README "${CMAKE_BINARY_DIR}/readme.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_BINARY_DIR}/welcome.txt")  

SET(CPACK_PACKAGE_NAME "openfluid")
SET(CPACK_PACKAGE_VERSION "${FULL_VERSION}")
SET(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
SET(CPACK_PACKAGE_CONTACT "Jean-Christophe Fabre <fabrejc@supagro.inra.fr>")
SET(CPACK_PACKAGE_VENDOR "LISAH (INRA-IRD-SupAgro)")
SET(CPACK_PACKAGE_DESCRIPTION "${OPENFLUID_PACKAGE_TITLE}.\n ${OPENFLUID_PACKAGE_DESCRIPTION}")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${OPENFLUID_PACKAGE_SUMMARY}")


##### Windows
IF(WIN32)
  SET(CPACK_GENERATOR "NSIS")
  SET(CPACK_SOURCE_GENERATOR "ZIP")
  SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/resources/packaging;${CMAKE_MODULE_PATH}")        

  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "OpenFLUID-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
  
  SET(CPACK_PACKAGE_NAME "OpenFLUID")
  
  SET(CPACK_NSIS_DISPLAY_NAME "OpenFLUID")
  SET(CPACK_NSIS_HELP_LINK "http://www.umr-lisah.fr/openfluid/")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http://www.umr-lisah.fr/openfluid/")
  SET(CPACK_NSIS_CONTACT "Jean-Christophe Fabre <fabrejc@users.sourceforge.net>")  
  SET(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}/resources/packaging\\\\OF_logo.bmp")  
  SET(CPACK_NSIS_MENU_LINKS "http://www.umr-lisah.fr/openfluid/" "OpenFLUID web site" "http://www.umr-lisah.fr/openfluid/community/" "OpenFLUID community site")
  SET(CPACK_PACKAGE_EXECUTABLES "openfluid-builder" "OpenFLUID-Builder")   
   
  SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME} ${FULL_VERSION}")
  SET(CPACK_NSIS_MODIFY_PATH ON)

  FIND_PATH(MINGW_BINARY_DIR mingwm10.dll PATHS
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MinGW;InstallLocation]/bin" 
  c:/MinGW/bin /MinGW/bin)
  
  GET_FILENAME_COMPONENT(LIBCURL_LIBRARY_DIR ${CURL_LIBRARY} PATH)
  
  FILE(TO_CMAKE_PATH "${Boost_INCLUDE_DIR}" BOOST_INCLUDEDIR_MOD)
  FILE(TO_CMAKE_PATH "${Boost_LIBRARY_DIRS}" BOOST_LIBRARYDIR_MOD)
  FILE(TO_CMAKE_PATH "${LibXML2_INCLUDE_DIRS}" LIBXML2_INCLUDEDIR_MOD)
  FILE(TO_CMAKE_PATH "$ENV{GTKMM_BASEPATH}" GTKMM_BASEPATH_MOD)  
  FILE(TO_CMAKE_PATH "${MINGW_BINARY_DIR}" MINGW_BINARYDIR_MOD)
  FILE(TO_CMAKE_PATH "${LIBCURL_LIBRARY_DIR}" LIBCURL_LIBRARYDIR_MOD)
  FILE(TO_CMAKE_PATH "${GDAL_LIBRARY}" GDAL_LIBRARY_MOD)
  FILE(TO_CMAKE_PATH "$ENV{GDAL_DIR}" GDAL_DIR_MOD)
  FILE(TO_CMAKE_PATH "$ENV{GDAL_DIR}/bin" GDAL_BINDIR_MOD)
 
  # Boost, libXML2
  INSTALL(DIRECTORY "${BOOST_INCLUDEDIR_MOD}/boost" DESTINATION include)
  INSTALL(DIRECTORY "${LIBXML2_INCLUDEDIR_MOD}/" DESTINATION include)
  INSTALL(DIRECTORY "${BOOST_LIBRARYDIR_MOD}/" DESTINATION ${BIN_INSTALL_PATH} FILES_MATCHING PATTERN "*.dll")
  INSTALL(DIRECTORY "${BOOST_LIBRARYDIR_MOD}/" DESTINATION ${LIB_INSTALL_PATH} FILES_MATCHING PATTERN "*.lib")  
  
  # pkg-config
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\pkg-config.exe" DESTINATION bin)
  
  # MinGW
  INSTALL(FILES "${MINGW_BINARYDIR_MOD}/mingwm10.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${MINGW_BINARYDIR_MOD}/libgcc_s_dw2-1.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${MINGW_BINARYDIR_MOD}/libstdc++-6.dll" DESTINATION ${BIN_INSTALL_PATH})
  
  # libCURL 
  INSTALL(FILES "${LIBCURL_LIBRARYDIR_MOD}/libcurl.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${LIBCURL_LIBRARYDIR_MOD}/libeay32.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${LIBCURL_LIBRARYDIR_MOD}/libssl32.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${LIBCURL_LIBRARYDIR_MOD}/libidn-11.dll" DESTINATION ${BIN_INSTALL_PATH})
  
  # libGDAL
  IF(NOT GDAL_DIR_MOD)
    MESSAGE(FATAL_ERROR "environment variable GDAL_DIR is not set!")
  ENDIF(NOT GDAL_DIR_MOD)
  
  INSTALL(FILES "${GDAL_LIBRARY_MOD}" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GDAL_BINDIR_MOD}/libcurl-4.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GDAL_BINDIR_MOD}/libgeos_c-1.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GDAL_BINDIR_MOD}/libgeos-3-2-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GDAL_BINDIR_MOD}/libpq.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GDAL_BINDIR_MOD}/libxerces-c-3-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GDAL_BINDIR_MOD}/libgdal-1.dll" DESTINATION ${BIN_INSTALL_PATH})
  
  # libgeos
  IF(OPENFLUID_ENABLE_LANDR)
    # TODO add geos devel if enabled
  ENDIF()
  
  # GTKmm
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\gspawn-win32-helper.exe" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\gspawn-win32-helper-console.exe" DESTINATION ${BIN_INSTALL_PATH})

  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libgtkmm-2.4-1.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libgdkmm-2.4-1.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libpangomm-1.4-1.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libatkmm-1.6-1.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libcairomm-1.0-1.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libgiomm-2.4-1.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libglibmm-2.4-1.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libsigc-2.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})  
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libgtk-win32-2.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libgdk-win32-2.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libgdk_pixbuf-2.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libpango-1.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libpangocairo-1.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libpangoft2-1.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libpangowin32-1.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libatk-1.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libcairo-2.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libgio-2.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libgmodule-2.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libgobject-2.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libgthread-2.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libglib-2.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libexpat-1.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libfontconfig-1.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libpng14-14.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\intl.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\freetype6.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\zlib1.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libgcc_s_sjlj-1.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libxml++-2.6-2.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libxml2-2.dll  " DESTINATION ${BIN_INSTALL_PATH})  
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libglademm-2.4-1.dll" DESTINATION ${BIN_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/bin\\\\libglade-2.0-0.dll" DESTINATION ${BIN_INSTALL_PATH})
  
  
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\atk-1.0" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\atkmm-1.6" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\cairo" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\cairomm-1.0" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\gdkmm-2.4" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\giomm-2.4" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\glib-2.0" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\glibmm-2.4" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\gtk-2.0" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\gtkmm-2.4" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\libglade-2.0" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\libglademm-2.4" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\libxml2" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\libxml++-2.6" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\pango-1.0" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\pangomm-1.4" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/include\\\\sigc++-2.0" DESTINATION ${INCLUDE_INSTALL_PATH})

  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/include\\\\expat.h" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/include\\\\expat_external.h" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/include\\\\ft2build.h" DESTINATION ${INCLUDE_INSTALL_PATH})  
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/include\\\\libintl.h" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/include\\\\png.h" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/include\\\\pngconf.h" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/include\\\\zconf.h" DESTINATION ${INCLUDE_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/include\\\\zlib.h" DESTINATION ${INCLUDE_INSTALL_PATH})

  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/lib\\\\gdkmm-2.4" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/lib\\\\giomm-2.4" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/lib\\\\glib-2.0" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/lib\\\\glibmm-2.4" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/lib\\\\gtk-2.0" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/lib\\\\gtkmm-2.4" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/lib\\\\libglademm-2.4" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/lib\\\\libxml++-2.6" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/lib\\\\pkgconfig" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/lib\\\\sigc++-2.0" DESTINATION ${LIB_INSTALL_PATH})

  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libglib-2.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\atk-1.0.def" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\atk-1.0.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\cairo.def" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\cairo.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\gailutil.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\gdk_pixbuf-2.0.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\gdk-win32-2.0.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\gio-2.0.def" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\gio-2.0.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\glade-2.0.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\glib-2.0.def" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\glib-2.0.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\gmodule-2.0.def" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\gmodule-2.0.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\gobject-2.0.def" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\gobject-2.0.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\gthread-2.0.def" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\gthread-2.0.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\gtk-win32-2.0.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libintl.def" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\intl.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libatk-1.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libatkmm-1.6.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libcairo.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libcairomm-1.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libgailutil.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libgdk_pixbuf-2.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libgdkmm-2.4.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libgdk-win32-2.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libgio-2.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libgiomm-2.4.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libglade-2.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libglademm-2.4.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libglib-2.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libglibmm_generate_extra_defs-2.4.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libglibmm-2.4.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libgmodule-2.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libgobject-2.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libgthread-2.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libgtkmm-2.4.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libgtk-win32-2.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libintl.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libpango-1.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libpangocairo-1.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libpangoft2-1.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libpangomm-1.4.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libpangowin32-1.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libpng.def" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libpng.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libpng14.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libsigc-2.0.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libxml++-2.6.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libxml2.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libxml2.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\libz.dll.a" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\pango-1.0.def" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\pango-1.0.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\pangocairo-1.0.def" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\pangocairo-1.0.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\pangoft2-1.0.def" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\pangoft2-1.0.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\pangowin32-1.0.def" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\pangowin32-1.0.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\zdll.lib" DESTINATION ${LIB_INSTALL_PATH})
  INSTALL(FILES "${GTKMM_BASEPATH_MOD}/lib\\\\zlib.def" DESTINATION ${LIB_INSTALL_PATH})

  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/etc\\\\gtk-2.0" DESTINATION etc)
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/etc\\\\pango" DESTINATION etc)

  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/share\\\\locale" DESTINATION ${SHARE_INSTALL_PATH})
  INSTALL(DIRECTORY "${GTKMM_BASEPATH_MOD}/share\\\\themes" DESTINATION ${SHARE_INSTALL_PATH})    
ENDIF(WIN32)

##### UNIX
IF(UNIX AND NOT APPLE AND NOT CYGWIN)
  SET(CPACK_SOURCE_GENERATOR "TGZ")
  IF(OF_DISTRO_IS_UBUNTU)  
    SET(CPACK_GENERATOR "DEB")      
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}-dev, libboost-date-time${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}-dev, libboost-filesystem${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}-dev, libboost-math${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}-dev, libboost-program-options${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}-dev, libboost-regex${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}-dev, libboost-iostreams${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}-dev, libboost-test${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}-dev, libxml2-dev, libcurl-dev, libglibmm-2.4-dev, libgtkmm-2.4-dev, libgdal1-dev, gcc, g++, cmake (>= 2.6.0)")    
    IF(OPENFLUID_ENABLE_LANDR)
      IF(${OF_DISTRO_VERSION} VERSION_GREATER "12.04")
        SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, libgeos++-dev (>= 3.2.0)")
      ELSE()
        SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, libgeos-dev (>= 3.2.0)")    
      ENDIF()
    ENDIF()  
    SET(CPACK_DEBIAN_PACKAGE_RECOMMENDS "gnuplot (>= 4.2), p7zip-full") 
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${OF_SYSTEM_ARCH}")
    SET(CPACK_DEBIAN_PACKAGE_SECTION "science")
    SET(CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
    SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${OF_DISTRO_CODE}_${OF_SYSTEM_ARCH}")
  ENDIF(OF_DISTRO_IS_UBUNTU)
  
  IF(OF_DISTRO_IS_CENTOS)  
    SET(CPACK_GENERATOR "RPM") 
    SET(CPACK_RPM_PACKAGE_REQUIRES "gcc-c++, gtkmm24-devel >= 2.18, glibmm24-devel >= 2.18, boost-devel >= 1.40, gdal-devel, libcurl-devel, libxml2-devel")
    IF(OPENFLUID_ENABLE_LANDR)
      SET(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, libgeos-devel >= 3.2.0")
    ENDIF()  
    SET(CPACK_RPM_PACKAGE_ARCHITECTURE "${OF_SYSTEM_ARCH}")
    SET(CPACK_PACKAGE_DESCRIPTION "This package contains the OpenFLUID software environment for modelling landscapes functionning, mainly oriented on spatial fluxes modelling. It also includes API and tools for development.")
    SET(CPACK_RPM_PACKAGE_DESCRIPTION "${OPENFLUID_PACKAGE_DESCRIPTION}")
    SET(CPACK_RPM_PACKAGE_SUMMARY "${OPENFLUID_PACKAGE_SUMMARY}")
    SET(CPACK_RPM_PACKAGE_GROUP "science")
    SET(CPACK_RPM_PACKAGE_LICENSE "${OPENFLUID_PACKAGE_LICENSE}")
    SET(CPACK_RPM_PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
    STRING(SUBSTRING ${OF_DISTRO_VERSION} 0 1 CENTOS_EL_VERSION)
    SET(CPACK_RPM_PACKAGE_RELEASE "1.el${CENTOS_EL_VERSION}")
    IF(VERSION_STATUS)
      SET(CPACK_RPM_PACKAGE_RELEASE "0.${VERSION_STATUS}.el${CENTOS_EL_VERSION}")
    ENDIF(VERSION_STATUS)    
    SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_RPM_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.${OF_SYSTEM_ARCH}")
  ENDIF(OF_DISTRO_IS_CENTOS)
ENDIF(UNIX AND NOT APPLE AND NOT CYGWIN)

##### Apple
IF(APPLE)
  SET(CPACK_GENERATOR "OSXX11;TGZ;PackageMaker")
  SET(CPACK_SOURCE_GENERATOR "TGZ")  
ENDIF(APPLE)



SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_src")
SET(CPACK_SOURCE_IGNORE_FILES "${CMAKE_BINARY_DIR}/*;/\\\\.svn/;"/\\\\..+$"")

INCLUDE(CPack)