## OpenFLUID/src/apps/openfluid-builder/CMakeLists.txt


FILE(MAKE_DIRECTORY "${BUILDEREXT_OUTPUT_PATH}")

ADD_SUBDIRECTORY(builderext)

ADD_SUBDIRECTORY(main)

ADD_SUBDIRECTORY(extensions)