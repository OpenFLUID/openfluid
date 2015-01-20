
FILE(GLOB OPENFLUID_SCIENTIFIC_CPP *.cpp)
FILE(GLOB OPENFLUID_SCIENTIFIC_HPP *.hpp)


ADD_LIBRARY(openfluid-scientific SHARED ${OPENFLUID_SCIENTIFIC_CPP} ${OPENFLUID_SCIENTIFIC_HPP})

SET_TARGET_PROPERTIES(openfluid-scientific PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        AUTOMOC ON
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-scientific 
                      openfluid-core
                      )




INSTALL(TARGETS openfluid-scientific
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_SCIENTIFIC_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/scientific)

   
IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  
   
   
