CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})
ADD_DEFINITIONS(-DBUILDINGDLL)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")

ADD_OBSERVER_WITH_INSTALL(export.vars.files.csv export.vars.files.csv ${OBSERVERS_OUTPUT_PATH})

ADD_OBSERVER_WITH_INSTALL(export.spatial-graph.files.dot export.spatial-graph.files.dot ${OBSERVERS_OUTPUT_PATH})

ADD_OBSERVER_WITH_INSTALL(export.vars.files.kml-anim export.vars.files.kml-anim ${OBSERVERS_OUTPUT_PATH})

ADD_OBSERVER_WITH_INSTALL(export.vars.files.kml-plot export.vars.files.kml-plot ${OBSERVERS_OUTPUT_PATH})

ADD_DEFINITIONS(-DTRILIBRARY)
ENABLE_LANGUAGE(C)
ADD_OBSERVER_WITH_INSTALL(export.vars.files.vtk export.vars.files.vtk ${OBSERVERS_OUTPUT_PATH})

                            