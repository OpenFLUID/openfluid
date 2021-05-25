
SET(OPENFLUID_LANDR_CPP LandREntity.cpp LineStringEntity.cpp  PolygonEntity.cpp
                        PolygonEdge.cpp
                        LandRGraph.cpp PolygonGraph.cpp LineStringGraph.cpp
                        VectorDataset.cpp RasterDataset.cpp
                        LandRTools.cpp
                        GEOSHelpers.cpp
                        )

SET(OPENFLUID_LANDR_HPP LandREntity.hpp LineStringEntity.hpp PolygonEntity.hpp
                        PolygonEdge.hpp
                        LandRGraph.hpp PolygonGraph.hpp LineStringGraph.hpp
                        VectorDataset.hpp RasterDataset.hpp
                        LandRTools.hpp
                        GEOSHelpers.hpp
                        )


FIND_PACKAGE(GEOS REQUIRED)

OPENFLUID_ADD_GEOS_DEFINITIONS()

INCLUDE_DIRECTORIES(${GEOS_INCLUDE_DIR} ${GDAL_INCLUDE_DIR})


ADD_LIBRARY(openfluid-landr SHARED ${OPENFLUID_LANDR_CPP} ${OPENFLUID_LANDR_HPP})

SET_TARGET_PROPERTIES(openfluid-landr PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-landr
                      openfluid-core openfluid-base
                      ${GDAL_LIBRARIES} ${GEOS_LIBRARY})


INSTALL(TARGETS openfluid-landr
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   
INSTALL(FILES ${OPENFLUID_LANDR_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/landr)


IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()
