CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT("tests.cmdline.bext-migration")

INCLUDE(CMake.in.config)

FIND_PACKAGE(OpenFLUIDHelpers REQUIRED)

OPENFLUID_ADD_BUILDEREXT(BEXT)

