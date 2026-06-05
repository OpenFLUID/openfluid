ADD_SUBDIRECTORY(miscsrc)

FILE(RELATIVE_PATH REL_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
# configure dataset with actual git URL
SET(DATASET_PATH "${REL_PATH}/datasets/OPENFLUID.IN.CustomWareset")

FILE(COPY "${CMAKE_SOURCE_DIR}/${DATASET_PATH}" 
     DESTINATION "${CMAKE_BINARY_DIR}/${REL_PATH}/datasets/")
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/${DATASET_PATH}/openfluid-dataset.in.json
               ${CMAKE_BINARY_DIR}/${DATASET_PATH}/openfluid-dataset.json)
