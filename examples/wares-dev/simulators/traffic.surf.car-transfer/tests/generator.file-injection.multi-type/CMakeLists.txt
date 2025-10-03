CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT("generator.file-injection.multi-type-${unitsclass}-${varname}")

FIND_PACKAGE(OpenFLUID REQUIRED)

ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(doc)

