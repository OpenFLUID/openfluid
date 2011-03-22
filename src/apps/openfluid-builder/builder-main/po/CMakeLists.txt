## OpenFLUID/src/apps/openfluid-builder/main/po/CMakeLists.txt

IF(BUILDER_NLS_ENABLE)

    FIND_PROGRAM(MSGFMT_EXECUTABLE msgfmt)
    
    FILE(GLOB Builder_PO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.po)
    
    FOREACH(Builder_PO_INPUT ${Builder_PO_FILES})
    
        GET_FILENAME_COMPONENT(Builder_PO_INPUT_BASE ${Builder_PO_INPUT} NAME_WE)
                
        SET(Builder_MO_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${Builder_PO_INPUT_BASE}.mo)
        
        ADD_CUSTOM_COMMAND(DEPENDS ${Builder_PO_INPUT}
                           COMMAND ${MSGFMT_EXECUTABLE} -c -o ${Builder_MO_OUTPUT} ${Builder_PO_INPUT} 
                           OUTPUT ${Builder_MO_OUTPUT})
        
        ADD_CUSTOM_TARGET(i18n-${Builder_PO_INPUT_BASE} ALL 
                          COMMENT "Building i18n messages for ${Builder_PO_INPUT_BASE}"
                          DEPENDS ${Builder_MO_OUTPUT})
        
        INSTALL(FILES ${Builder_MO_OUTPUT} DESTINATION /usr/share/locale/${Builder_PO_INPUT_BASE}/LC_MESSAGES RENAME ${BUILDER_NLS_PACKAGE}.mo)
        
    ENDFOREACH(Builder_PO_INPUT ${Builder_PO_FILES})

ENDIF(BUILDER_NLS_ENABLE)