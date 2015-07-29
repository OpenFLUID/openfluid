

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")
INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIRS} ${QT_INCLUDES})


OPNFLD_ADD_OBSERVER(export.vars.files.csv ${OBSERVERS_OUTPUT_PATH})


OPNFLD_ADD_OBSERVER(export.vars.files.kml-anim ${OBSERVERS_OUTPUT_PATH})

                      
OPNFLD_ADD_OBSERVER(export.vars.files.kml-plot ${OBSERVERS_OUTPUT_PATH})


OPNFLD_ADD_OBSERVER(export.vars.files.geovector ${OBSERVERS_OUTPUT_PATH})

                      
OPNFLD_ADD_OBSERVER(export.vars.plot.gnuplot ${OBSERVERS_OUTPUT_PATH})

                      
OPNFLD_ADD_OBSERVER(export.spatial-graph.files.dot ${OBSERVERS_OUTPUT_PATH})

                            