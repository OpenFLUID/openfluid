INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(OPENFLUID_TOOLS_CPP ColumnTextParser.cpp
                        ChronologicalSerie.cpp
                        ProgressiveColumnFileReader.cpp ProgressiveChronFileReader.cpp
                        ChronFileInterpolator.cpp ChronFileLinearInterpolator.cpp 
                        DistributionTables.cpp DistributionBindings.cpp
                        Filesystem.cpp
                        FileLogger.cpp
                        DataHelpers.cpp IDHelpers.cpp QtHelpers.cpp MiscHelpers.cpp
                        )

SET(OPENFLUID_TOOLS_HPP ColumnTextParser.hpp 
                        ChronologicalSerie.hpp 
                        ProgressiveColumnFileReader.hpp ProgressiveChronFileReader.hpp
                        ChronFileInterpolator.hpp ChronFileLinearInterpolator.hpp
                        DistributionTables.hpp DistributionBindings.hpp
                        Console.hpp
                        Filesystem.hpp
                        FileLogger.hpp
                        DataHelpers.hpp IDHelpers.hpp QtHelpers.hpp MiscHelpers.hpp
                        Timer.hpp
                        FortranCPP.hpp
                        )


ADD_LIBRARY(openfluid-tools SHARED ${OPENFLUID_TOOLS_CPP} ${OPENFLUID_TOOLS_HPP})

SET_TARGET_PROPERTIES(openfluid-tools PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        AUTOMOC ON
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-tools 
                      openfluid-core
                      Qt5::Core
                      Qt5::Xml
                     )

IF(WIN32)
  TARGET_LINK_LIBRARIES(openfluid-tools "userenv.lib")
ENDIF()


INSTALL(TARGETS openfluid-tools
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   
INSTALL(FILES ${OPENFLUID_TOOLS_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/tools)

   
IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  
   
   
