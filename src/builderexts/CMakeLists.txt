
SET(CMAKE_INCLUDE_CURRENT_DIR ON)


OPNFLD_ADD_BUILDER_EXTENSION_WITH_INSTALL(view.spatial-graph.graphviz view.spatial-graph.graphviz ${BUILDEREXTS_OUTPUT_PATH})
TARGET_LINK_LIBRARIES(view.spatial-graph.graphviz${OPENFLUID_BUILDEREXTS_SUFFIX}
                      openfluid-utils 
                      ${QT_QTSVG_LIBRARIES})																						


OPNFLD_ADD_BUILDER_EXTENSION_WITH_INSTALL(import.spatial-graph.ogr-gdal import.spatial-graph.ogr-gdal ${BUILDEREXTS_OUTPUT_PATH})
TARGET_LINK_LIBRARIES(import.spatial-graph.ogr-gdal${OPENFLUID_BUILDEREXTS_SUFFIX}
                      openfluid-utils
                      openfluid-ui-common
                      ${GDAL_LIBRARIES} )