
ADD_DEFINITIONS(${LibXML2_CFLAGS})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${LibXML2_INCLUDE_DIRS})
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})

FILE(GLOB OPENFLUID_IO_CPP *.cpp)
FILE(GLOB OPENFLUID_IO_HPP *.hpp)


ADD_LIBRARY(openfluid-io SHARED ${OPENFLUID_IO_CPP} ${OPENFLUID_IO_HPP})

SET_TARGET_PROPERTIES(openfluid-io PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-io 
                      openfluid-base
                      openfluid-core
                      openfluid-tools
                      ${LibXML2_LIBRARIES}                      
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_DATE_TIME_LIBRARY})




INSTALL(TARGETS openfluid-io
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_IO_HPP} DESTINATION ${INCLUDE_INSTALL_PATH}/io)

   