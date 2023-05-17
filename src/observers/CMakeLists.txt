

OFBUILD_ADD_OBSERVER(export.vars.files.csv)
OFBUILD_ADD_OBSERVER(export.vars.files.csv-multicols)
OFBUILD_ADD_OBSERVER(export.vars.files.kml-anim)
OFBUILD_ADD_OBSERVER(export.vars.files.kml-plot)
OFBUILD_ADD_OBSERVER(export.vars.files.geovector)
OFBUILD_ADD_OBSERVER(export.vars.plot.gnuplot)
OFBUILD_ADD_OBSERVER(export.spatial-graph.files.dot)

INSTALL(DIRECTORY "${OFBUILD_DIST_OBSERVERS_DIR}/"
        DESTINATION "${OPENFLUID_OBSERVERS_INSTALL_PATH}")
