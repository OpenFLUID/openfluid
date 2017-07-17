

FILE(GLOB OPENFLUID_BUILDEREXT_CPP PluggableBuilderExtension.cpp
                                   PluggableParameterizationExtension.cpp
                                   PluggableFeatureExtension.cpp
                                   PluggableModalExtension.cpp
                                   PluggableModelessExtension.cpp
                                   PluggableWorkspaceExtension.cpp                                                                                                
                                  )

FILE(GLOB OPENFLUID_BUILDEREXT_HPP FluidXUpdateFlags.hpp
                                   BuilderExtensionSignature.hpp                                    
                                   PluggableBuilderExtension.hpp 
                                   PluggableParameterizationExtension.hpp PluggableFeatureExtension.hpp
                                   PluggableModalExtension.hpp
                                   PluggableModelessExtension.hpp
                                   PluggableWorkspaceExtension.hpp                                   
                                  )


ADD_LIBRARY(openfluid-builderext SHARED ${OPENFLUID_BUILDEREXT_CPP} ${OPENFLUID_BUILDEREXT_HPP})

SET_TARGET_PROPERTIES(openfluid-builderext PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        AUTOMOC ON
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-builderext
                      openfluid-base
                      openfluid-core
                      openfluid-ware
                      openfluid-tools                                           
                      openfluid-fluidx
                      Qt5::Core
                      Qt5::Widgets                      
                     )



INSTALL(TARGETS openfluid-builderext
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_BUILDEREXT_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/builderext)



   