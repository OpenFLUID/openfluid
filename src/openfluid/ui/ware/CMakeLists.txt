

SET(OPENFLUID_UI_WARE_CPP ParameterizationWidget.cpp)


SET(OPENFLUID_UI_WARE_HPP ParameterizationWidget.hpp)
                                   


ADD_LIBRARY(openfluid-ui-ware SHARED ${OPENFLUID_UI_WARE_CPP} ${OPENFLUID_UI_WARE_HPP})

SET_TARGET_PROPERTIES(openfluid-ui-ware PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                          SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                          LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                          RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                          ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                          AUTOMOC ON
                                          COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                          DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-ui-ware
                      ${QT_QTCORE_LIBRARIES}
                      ${QT_QTGUI_LIBRARIES}    
                     )



INSTALL(TARGETS openfluid-ui-ware
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_UI_WARE_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/ui/ware)



   