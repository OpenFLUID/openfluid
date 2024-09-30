INCLUDE(${OpenFLUID_DIR}/QtNamespaceFacade.cmake)

SET(OPENFLUID_UI_SPATIAL_CPP MapScene.cpp MapView.cpp MapItemGraphics.cpp 
                             PointGraphics.cpp PunctualGraphics.cpp MultiPointGraphics.cpp
                             LineStringGraphics.cpp LinearGraphics.cpp MultiLineStringGraphics.cpp
                             PolygonGraphics.cpp SurfacicGraphics.cpp MultiPolygonGraphics.cpp
                            )

SET(OPENFLUID_UI_SPATIAL_HPP MapScene.hpp MapView.hpp MapItemGraphics.hpp
                             PointGraphics.hpp PunctualGraphics.hpp MultiPointGraphics.hpp
                             LineStringGraphics.hpp LinearGraphics.hpp MultiLineStringGraphics.hpp
                             PolygonGraphics.hpp SurfacicGraphics.hpp MultiPolygonGraphics.hpp
                            )

ADD_LIBRARY(openfluid-ui-spatial SHARED ${OPENFLUID_UI_SPATIAL_CPP} ${OPENFLUID_UI_SPATIAL_HPP})

SET_TARGET_PROPERTIES(openfluid-ui-spatial PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}"
                                           SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                           LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                           RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                           ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                           AUTOMOC ON
                                           COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                           DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-ui-spatial
                      openfluid-base
                      openfluid-core
                      openfluid-tools
                      openfluid-machine
                      openfluid-fluidx
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTWIDGETS_LIBRARIES}
                     )



INSTALL(TARGETS openfluid-ui-spatial
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})


INSTALL(FILES ${OPENFLUID_UI_SPATIAL_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/ui/spatial)

IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()
