ADD_SUBDIRECTORY(miscsrc)

# configure dataset with actual git URL
SET(DATASET_PATH "resources/tests/datasets/OPENFLUID.IN.CustomWareset") #TOIMPL replace "resources/tests" by cmake var to say "here"

FILE(COPY "${CMAKE_SOURCE_DIR}/${DATASET_PATH}/" 
    DESTINATION "${CMAKE_BINARY_DIR}/${DATASET_PATH}")
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/${DATASET_PATH}/openfluid-dataset.in.json
               ${CMAKE_BINARY_DIR}/${DATASET_PATH}/openfluid-dataset.json)
