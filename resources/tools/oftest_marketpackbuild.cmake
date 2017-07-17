###
### Usage cmake -DREPOS_ROOT_DIR=...
###             -DMARKETPLACE_ROOT_DIR=... 
###             -DREPOS_VERSION=... -DREPOS_ARCH=... 
###             -P oftest_marketpackbuild.cmake
###


CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)


IF(NOT EXISTS ${REPOS_ROOT_DIR})
  MESSAGE(FATAL_ERROR "wrong or missing REPOS_ROOT_DIR variable")
ENDIF()

IF(${REPOS_ARCH} STREQUAL "i386")
  SET(REPOS_ARCH "linux-i386")
ELSEIF(${REPOS_ARCH} STREQUAL "amd64")
  SET(REPOS_ARCH "linux-x86-64")
ENDIF()

# Create marketplace
FILE(MAKE_DIRECTORY "${MARKETPLACE_ROOT_DIR}")
FILE(WRITE "${MARKETPLACE_ROOT_DIR}/OpenFLUID-Marketplace"
     "[openfluid-marketplace]\n"
     "name = OpenFLUID-Market generated automatically\n"    
      "description = OpenFLUID-Market generated automatically\n"
      "contact = (unknown)\n")
FILE(MAKE_DIRECTORY "${MARKETPLACE_ROOT_DIR}/licenses")


MESSAGE(STATUS "Creating market packages for tests :")


SET(REPOS_TYPES "simulators" "observers" "builderexts")
      
##### Simulators, Observers & Builder-extensions ##### 
FOREACH(type ${REPOS_TYPES})

  GET_FILENAME_COMPONENT(TYPE_NAME ${type} NAME)
  
  # Create type directory
  FILE(MAKE_DIRECTORY "${MARKETPLACE_ROOT_DIR}/${TYPE_NAME}")
  FILE(MAKE_DIRECTORY "${MARKETPLACE_ROOT_DIR}/${TYPE_NAME}/src")
  FILE(MAKE_DIRECTORY "${MARKETPLACE_ROOT_DIR}/${TYPE_NAME}/${REPOS_ARCH}")
  
  
  # Set catalog name
  string(SUBSTRING ${TYPE_NAME} 0 1 TYPE_FIRST_LETTER)
  string(TOUPPER ${TYPE_FIRST_LETTER} MAJ)
  
  string(LENGTH ${TYPE_NAME} TYPE_LENGTH)
  MATH(EXPR LAST_CHAR_POS "${TYPE_LENGTH} - 1")
  string(SUBSTRING ${TYPE_NAME} 1 ${LAST_CHAR_POS} TYPE_END)
  
  SET(CATALOG_TYPE_NAME "${MAJ}${TYPE_END}")
  
  # Create catalog
  SET(MARKET_CATALOG_FILE "${MARKETPLACE_ROOT_DIR}/Catalog${CATALOG_TYPE_NAME}_${REPOS_VERSION}")
  FILE(WRITE ${MARKET_CATALOG_FILE})
  
  
  
  FILE(GLOB REPOS_PACKAGES "${REPOS_ROOT_DIR}/${TYPE_NAME}/*")
  
  FOREACH(pack ${REPOS_PACKAGES})
  
    GET_FILENAME_COMPONENT(PACK_NAME ${pack} NAME)
      
    MESSAGE(STATUS "  ${PACK_NAME}")
      
    SET(PACKAGE_NAME "")
    SET(PACKAGE_DESC "")
    SET(PACKAGE_AUTHORS "")
    SET(PACKAGE_VERSION "")
    SET(PACKAGE_LICENSE "")
      
     
    # Add package informations to the catalog  
    FILE(APPEND ${MARKET_CATALOG_FILE}
          "[${PACK_NAME}]\n")

    FILE(APPEND ${MARKET_CATALOG_FILE}
          "name = ${PACKAGE_NAME}\n")       
     
    FILE(APPEND ${MARKET_CATALOG_FILE}
          "description = ${PACKAGE_DESC}\n")
            
    FILE(APPEND ${MARKET_CATALOG_FILE}
          "authors = ${PACKAGE_AUTHORS}\n")
    
    FILE(APPEND ${MARKET_CATALOG_FILE}
          "version = ${PACKAGE_VERSION}\n")
          

    FILE(GLOB REPOS_FORMATS "${REPOS_ROOT_DIR}/${TYPE_NAME}/${PACK_NAME}/*")
      
    FOREACH(format ${REPOS_FORMATS})
   
      GET_FILENAME_COMPONENT(FORMAT_NAME ${format} NAME)
      
      IF(NOT(${FORMAT_NAME} STREQUAL "src"))
        SET(FORMAT_NAME ${REPOS_ARCH})
      ENDIF()
     
      FILE(GLOB FILES_TO_PACK "${format}/*")
    
      # Execute packaging command for the current format
      EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND}
                  "-E" "chdir" "${format}"
                  ${CMAKE_COMMAND} "-E" "tar" "czf" "${MARKETPLACE_ROOT_DIR}/${TYPE_NAME}/${FORMAT_NAME}/${PACK_NAME}_${REPOS_VERSION}_${FORMAT_NAME}.ofpk"
                  ${FILES_TO_PACK})
          
      FILE(APPEND ${MARKET_CATALOG_FILE}
          "arch.${FORMAT_NAME}.license = ${PACKAGE_LICENSE}\n")
         
      FILE(APPEND ${MARKET_CATALOG_FILE}
          "arch.${FORMAT_NAME}.file = ${PACK_NAME}_${REPOS_VERSION}_${FORMAT_NAME}.ofpk\n")                

    ENDFOREACH() #format
    
    FILE(APPEND ${MARKET_CATALOG_FILE}
          "\n")

  ENDFOREACH() #package
  
ENDFOREACH() #type



##### Datasets #####

SET(TYPE_NAME "datasets")

# Create directory
FILE(MAKE_DIRECTORY "${MARKETPLACE_ROOT_DIR}/${TYPE_NAME}")

# Create catalog
SET(MARKET_CATALOG_FILE "${MARKETPLACE_ROOT_DIR}/CatalogDatasets_${REPOS_VERSION}")
FILE(WRITE ${MARKET_CATALOG_FILE})
  
  
FILE(GLOB REPOS_PACKAGES "${REPOS_ROOT_DIR}/${TYPE_NAME}/*")
  
FOREACH(pack ${REPOS_PACKAGES})
  
  GET_FILENAME_COMPONENT(PACK_NAME ${pack} NAME)
  
  MESSAGE(STATUS "  ${PACK_NAME}")
      
  SET(PACKAGE_NAME "")
  SET(PACKAGE_DESC "")
  SET(PACKAGE_AUTHORS "")
  SET(PACKAGE_VERSION "")
  SET(PACKAGE_LICENSE "")
      
      
  # Add package informations to the catalog  
  FILE(APPEND ${MARKET_CATALOG_FILE}
        "[${PACK_NAME}]\n")

  FILE(APPEND ${MARKET_CATALOG_FILE}
        "name = ${PACKAGE_NAME}\n")       
     
  FILE(APPEND ${MARKET_CATALOG_FILE}
        "description = ${PACKAGE_DESC}\n")
        
  FILE(APPEND ${MARKET_CATALOG_FILE}
        "authors = ${PACKAGE_AUTHORS}\n")
      
  FILE(APPEND ${MARKET_CATALOG_FILE}
        "version = ${PACKAGE_VERSION}\n")
            

  FILE(GLOB FILES_TO_PACK "${pack}/*")
      
  # Execute packaging command
  EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND}
            "-E" "chdir" "${pack}"
            ${CMAKE_COMMAND} "-E" "tar" "czf" "${MARKETPLACE_ROOT_DIR}/${TYPE_NAME}/${PACK_NAME}_${REPOS_VERSION}_data.ofpk"
            ${FILES_TO_PACK})
            
  FILE(APPEND ${MARKET_CATALOG_FILE}
        "license = ${PACKAGE_LICENSE}\n")
           
  FILE(APPEND ${MARKET_CATALOG_FILE}
        "file = ${PACK_NAME}_${REPOS_VERSION}_data.ofpk\n")                
   
  FILE(APPEND ${MARKET_CATALOG_FILE}
        "\n")   
            
ENDFOREACH() #package


