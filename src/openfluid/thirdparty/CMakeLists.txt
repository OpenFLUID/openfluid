INSTALL(FILES JSON.hpp XML.hpp 
        DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/thirdparty)


##### nlohmann::json as JSON

INSTALL(DIRECTORY nlohmannjson-${OFBUILD_3RDPARTY_nlohmannjson_VERSION}/ 
        DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/thirdparty/nlohmannjson-${OFBUILD_3RDPARTY_nlohmannjson_VERSION}
        FILES_MATCHING PATTERN "*.hpp")


##### TinyXML2 as XML

ADD_LIBRARY(openfluid-thirdparty-tinyxml2 SHARED tinyxml2-${OFBUILD_3RDPARTY_tinyxml2_VERSION}/tinyxml2.cpp)

SET_TARGET_PROPERTIES(openfluid-thirdparty-tinyxml2 
                      PROPERTIES POSITION_INDEPENDENT_CODE True
                                 VERSION "${OFBUILD_3RDPARTY_tinyxml2_VERSION}" 
                                 SOVERSION "${OFBUILD_3RDPARTY_tinyxml2_VERSION}"
                                 LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                 RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                 ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}")

INSTALL(TARGETS openfluid-thirdparty-tinyxml2
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})

INSTALL(DIRECTORY tinyxml2-${OFBUILD_3RDPARTY_tinyxml2_VERSION}/ 
        DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/thirdparty/tinyxml2-${OFBUILD_3RDPARTY_tinyxml2_VERSION}
        FILES_MATCHING PATTERN "*.h")
