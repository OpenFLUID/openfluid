CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT("tests.primitives.land.prod")

FIND_PACKAGE(OpenFLUID REQUIRED)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(doc)
ADD_SUBDIRECTORY(tests)

