CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT("tests.cmdline.sim-migration")

# [SKIP-2.2]>-------
SET(SKIPPABLE 1)
# -------<[SKIP-2.2]

INCLUDE(CMake.in.config)

FIND_PACKAGE(OpenFLUIDHelpers REQUIRED)

SET(CUSTOM_VAR foo)

OPENFLUID_ADD_SIMULATOR(SIM)

MESSAGE(STATUS 'custom message')


# [SKIP-2.2]>-------
SET(SKIPPABLE 2)
# -------<[SKIP-2.2]
