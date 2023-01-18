
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT("migrate_signature")

FIND_PACKAGE(OpenFLUID REQUIRED ware waresdev tools)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

ADD_EXECUTABLE(migrate_signature migrate_signature.cpp)

TARGET_INCLUDE_DIRECTORIES(migrate_signature PRIVATE "${OpenFLUID_HOME_DIR}/share/openfluid/waresdev/migration/include"
                                             PRIVATE "${OpenFLUID_INCLUDE_DIRS}")

TARGET_INCLUDE_DIRECTORIES(migrate_signature PRIVATE $ENV{OpenFLUID_MIGRATION_INCLUDE_PATHS})

LINK_DIRECTORIES(${OpenFLUID_LIBRARY_DIRS})

TARGET_LINK_LIBRARIES(migrate_signature
                      ${OpenFLUID_LIBRARIES}
                     )
