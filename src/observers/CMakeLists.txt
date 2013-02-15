CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})
ADD_DEFINITIONS(-DBUILDINGDLL)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")

ADD_OBSERVER(export.vars.files.csv export.vars.files.csv ${OBSERVERS_OUTPUT_PATH})

ADD_OBSERVER(export.spatial-graph.files.dot export.spatial-graph.files.dot ${OBSERVERS_OUTPUT_PATH})
                            