


INCLUDE_DIRECTORIES(${QT_INCLUDES})


FILE(GLOB OPENFLUID_BUILDEREXT_CPP PluggableBuilderExtension.cpp
                                   PluggableModalExtension.cpp
                                   PluggableModelessExtension.cpp
                                   PluggableWorkspaceExtension.cpp                                                                    

                                  )

FILE(GLOB OPENFLUID_BUILDEREXT_HPP BuilderExtensionSignature.hpp
                                   PluggableBuilderExtension.hpp
                                   PluggableModalExtension.hpp
                                   PluggableModelessExtension.hpp
                                   PluggableWorkspaceExtension.hpp                                                                    
                                  )


ADD_LIBRARY(openfluid-builderext SHARED ${OPENFLUID_BUILDEREXT_CPP} ${OPENFLUID_BUILDEREXT_HPP})

SET_TARGET_PROPERTIES(openfluid-builderext PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        )

TARGET_LINK_LIBRARIES(openfluid-builderext
                      openfluid-base
                      openfluid-core
                      openfluid-tools                                           
                      openfluid-fluidx
                      ${QT_QTCORE_LIBRARIES}
                      ${QT_QTGUI_LIBRARIES}    
                     )



INSTALL(TARGETS openfluid-builderext
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_BUILDEREXT_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/builderext)



   