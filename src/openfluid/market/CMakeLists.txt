

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})


SET(OPENFLUID_MARKET_CPP MarketPackage.cpp
                         MarketSrcPackage.cpp MarketSrcSimulatorPackage.cpp MarketSrcObserverPackage.cpp MarketSrcBuilderextPackage.cpp
                         MarketBinPackage.cpp MarketBinSimulatorPackage.cpp MarketBinObserverPackage.cpp MarketBinBuilderextPackage.cpp
                         MarketDatasetPackage.cpp
                         MarketClient.cpp
                   

)

SET(OPENFLUID_MARKET_HPP MarketPackage.hpp
                         MarketSrcPackage.hpp MarketSrcSimulatorPackage.hpp MarketSrcObserverPackage.hpp MarketSrcBuilderextPackage.hpp
                         MarketBinPackage.hpp MarketBinSimulatorPackage.hpp MarketBinObserverPackage.hpp MarketBinBuilderextPackage.hpp
                         MarketDatasetPackage.hpp
                         MarketInfos.hpp MarketClient.hpp
                         )


ADD_LIBRARY(openfluid-market SHARED ${OPENFLUID_MARKET_CPP} ${OPENFLUID_MARKET_HPP})

SET_TARGET_PROPERTIES(openfluid-market PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-market
                      openfluid-base
                      openfluid-tools
                      openfluid-utils
                      Qt5::Core
                     )


INSTALL(TARGETS openfluid-market
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   
INSTALL(FILES ${OPENFLUID_MARKET_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/market)


IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  
